//Copyright (c) 2017 Finjin
//
//This file is part of Finjin Exporter (finjin-exporter).
//
//Finjin Exporter is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//Finjin Exporter is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with Finjin Exporter.  If not, see <http://www.gnu.org/licenses/>.


//Includes----------------------------------------------------------------------
#include "FinjinPrecompiled.hpp"
#include "GeometryState.hpp"
#include "MaxMaterialUtilities.hpp"
#include "MaxUtilities.hpp"
#include "MeshMorpher.hpp"
#include "MeshSkeleton.hpp"

using namespace Finjin::Exporter;


//Local classes----------------------------------------------------------------
struct TextureMapChannels : std::vector<std::pair<int, MeshMap*>>
{
    MeshMap* alphaMapChannel;

    TextureMapChannels()
    {
        this->alphaMapChannel = nullptr;
    }

    void Initialize(Mesh& mesh)
    {
        reserve(FINJIN_EXPORTER_MAX_TEXTURE_COORD_SETS);
        for (int mapNumber = -NUM_HIDDENMAPS; mapNumber < mesh.numMaps; mapNumber++)
        {
            auto& map = mesh.Map(mapNumber);
            if (map.IsUsed())
            {
                if (mapNumber < 0)
                {
                    //Hidden map
                    if (mapNumber == MAP_ALPHA)
                    {
                        //Alpha map
                        this->alphaMapChannel = &mesh.Map(mapNumber);
                    }
                }
                else if (map.vnum > 0 && map.fnum > 0)
                {
                    //Texture map
                    Add(mapNumber, &map);
                }
            }
        }
    }

    void Add(int mapNumber, MeshMap* map)
    {
        push_back(std::make_pair(mapNumber, map));
    }

    MeshMap* GetMap(int mapNumber)
    {
        for (size_t i = 0; i < size(); i++)
        {
            if ((*this)[i].first == mapNumber)
                return (*this)[i].second;
        }
            
        return nullptr;
    }

    MeshMap* GetFirstNonempty() 
    {
        for (auto& item : *this)
        {
            if (item.second->fnum > 0)
                return item.second;
        }
        return nullptr;
    }
};


//Implementation---------------------------------------------------------------
bool GeometryState::CanHandle(ObjectAccessor object)
{
    auto result = false;

    auto maxObject = object.node->EvalWorldState(0).obj;
    if (maxObject != nullptr)
    {
        result = 
            maxObject->SuperClassID() == SHAPE_CLASS_ID ||
            maxObject->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0));        
    }

    return result;
}

void GeometryState::GetSubmeshesSettings(ObjectAccessor object, SubmeshesSettings& submeshesSettings, GeometryStateFlags flags)
{
    submeshesSettings.clear();

    //Evaluate object
    ShapeObject* shapeObject = nullptr;
    auto maxObject = object.node->EvalWorldState(0).obj;
    if (!maxObject->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
        return;

    auto triObject = static_cast<TriObject*>(maxObject->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0)));
    auto mesh = &triObject->GetMesh();

    //Get submesh materials
    MaterialAccessor material = object.GetMaterial();
    MaterialAccessorVector submeshMaterials;
    material.Expand(submeshMaterials);
    if (submeshMaterials.empty())
        submeshMaterials.push_back(MaterialAccessor());
    
    //Get all the texture mapping channels
    TextureMapChannels textureMapChannels;
    textureMapChannels.Initialize(*mesh);
    
    TextureCoordinateSetMappings textureCoordinateSets;
    textureCoordinateSets.resize(textureMapChannels.size());
    for (size_t textureCoordinateSetIndex = 0; 
        textureCoordinateSetIndex < textureMapChannels.size(); 
        textureCoordinateSetIndex++)
    {
        textureCoordinateSets[textureCoordinateSetIndex].source.number = textureMapChannels[textureCoordinateSetIndex].first;
    }

    //Done
    submeshesSettings.Initialize(submeshMaterials, textureCoordinateSets);
}

bool GeometryState::Create
    (
    const wxString& meshName,
    ObjectAccessor object, 
    const CoordinateSystemConverter& coordinateConverter, 
    float scale,
    TimeAccessor time, 
    GeometryStateFlags flags,
    SubmeshesSettings* submeshesSettings,
    const SkeletonReferencePose* referencePose
    )
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("GeometryState::Create(%s)"), object.GetFullName().wx_str());

    if (!CanHandle(object))
        return false;

    auto fastVertices = AnySet(flags & GeometryStateFlags::FAST_VERTICES);

    if (!MaxUtilities::VerifyMeshModifierStack(object.node))
        flags |= GeometryStateFlags::NO_MORPHER_CONTROL | GeometryStateFlags::NO_SKELETON_CONTROL;

    StartCreate(meshName, object, coordinateConverter, scale, time, flags, referencePose);

    //Evaluate object
    ShapeObject* shapeObject = nullptr;
    PolyShape shape;
    TriObject* triObject = nullptr;
    Mesh* mesh = nullptr;
    auto maxObject = object.node->EvalWorldState(time.GetNativeTime()).obj;
    if (maxObject->SuperClassID() == SHAPE_CLASS_ID)
    {
        shapeObject = static_cast<ShapeObject*>(maxObject);        
        shapeObject->MakePolyShape(0, shape);
    }
    else if (maxObject->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
    {
        triObject = static_cast<TriObject*>(maxObject->ConvertToType(time.GetNativeTime(), Class_ID(TRIOBJ_CLASS_ID, 0)));
        mesh = &triObject->GetMesh();
    }

    this->submeshProperties.vertexFormatElements.push_back(WxGpuVertexFormatStructUtilities::CreateElement(WxGpuVertexFormatStructMetadata::ElementID::POSITION, WxNumericStructElementType::FLOAT3));
    
    //Geometry type and coloring
    auto hasRealVertexColors = false;
    MeshMap* colorMapChannel = nullptr;
    if (mesh != nullptr)
    {
        this->submeshProperties.vertexFormatElements.push_back(WxGpuVertexFormatStructUtilities::CreateElement(WxGpuVertexFormatStructMetadata::ElementID::NORMAL, WxNumericStructElementType::FLOAT3));

        this->geometryType = GeometryStateSubmesh::MESH;
        hasRealVertexColors = mesh->mapSupport(mesh->curVCChan) ? true : false;
        if (hasRealVertexColors)
            colorMapChannel = &mesh->Map(mesh->curVCChan);
        this->submeshProperties.hasVertexColors = hasRealVertexColors || AnySet(flags & GeometryStateFlags::FORCE_WHITE_VERTEX_COLORS);
    }
    else // if (shapeObject != nullptr)
    {
        this->geometryType = GeometryStateSubmesh::SHAPE;
        this->submeshProperties.hasNormals = false;
        this->submeshProperties.requiresDummyBone = true;
    }

    //Create the submeshes that faces will be placed into
    std::vector<bool> explodedEdgeVisibilities;
    std::vector<Vertex> explodedVertices;

    MaterialAccessorVector objectMaterials;
    MaterialAccessor objectMaterial = object.GetMaterial();
    objectMaterial.Expand(objectMaterials);

    std::vector<GeometryStateSubmesh*> primitiveSubmeshes;
    MaterialAccessorVector submeshesMaterials;
    {
        submeshesMaterials = objectMaterials;
        if (submeshesMaterials.empty())
            submeshesMaterials.push_back(MaterialAccessor());

        MaterialsToSubmeshes materialsToSubmeshes;
        if (this->geometryType == GeometryStateSubmesh::MESH)
        {
            explodedEdgeVisibilities.reserve(mesh->numFaces * 3);
            explodedVertices.reserve(mesh->numFaces * 3);

            //Resize the polygon-to-submesh array
            primitiveSubmeshes.resize(mesh->numFaces);

            //Iterate over faces to determine materials
            for (int faceIndex = 0; faceIndex < mesh->numFaces; faceIndex++)
            {
                auto& face = mesh->faces[faceIndex];
                MaterialAccessor faceMaterial = objectMaterial.IsValid() ? objectMaterials[face.getMatID() % objectMaterials.size()] : nullptr;

                //Get the submesh for the face's material and fill in some basic info
                auto isNew = false;
                primitiveSubmeshes[faceIndex] = GetMaterialSubmesh(materialsToSubmeshes, faceMaterial, &isNew);
                if (isNew)
                    primitiveSubmeshes[faceIndex]->Reserve(mesh->numVerts);
            }
        }
        else
        {
            //Count up the number of segments
            size_t explodedVertexCount = 0;
            int segmentCount = 0;
            for (int lineIndex = 0; lineIndex < shape.numLines; lineIndex++)
            {
                auto& line = shape.lines[lineIndex];
                segmentCount += line.Segments();

                explodedVertexCount += line.Segments() + 1;
            }

            //Resize the segment-to-submesh array
            primitiveSubmeshes.resize(segmentCount);

            //Iterate over lines and segments to determine materials
            int primitiveSubmeshIndex = 0;
            for (int lineIndex = 0; lineIndex < shape.numLines; lineIndex++)
            {
                auto& line = shape.lines[lineIndex];

                for (int segmentIndex = 0; segmentIndex < line.Segments(); segmentIndex++)
                {
                    MtlID mtlID = line.GetMatID(segmentIndex);
                    MaterialAccessor segmentMaterial = objectMaterial.IsValid() ? objectMaterials[line.GetMatID(segmentIndex) % objectMaterials.size()] : nullptr;

                    //Get the submesh for the line's segment and fill in some basic info
                    auto isNew = false;
                    auto submesh = primitiveSubmeshes[primitiveSubmeshIndex++] = GetMaterialSubmesh(materialsToSubmeshes, segmentMaterial, &isNew);
                    if (isNew)
                        submesh->Reserve(mesh->numVerts);
                }                
            }
        }
    }

    //Get all the texture mapping channels    
    TextureMapChannels textureMapChannels;
    textureMapChannels.Initialize(*mesh);
    
    TextureCoordinateSetMappings textureCoordinateSetMappings;
    textureCoordinateSetMappings.resize(textureMapChannels.size());
    for (size_t textureCoordinateSetIndex = 0; textureCoordinateSetIndex < textureMapChannels.size(); textureCoordinateSetIndex++)
    {
        textureCoordinateSetMappings[textureCoordinateSetIndex].source.number = textureMapChannels[textureCoordinateSetIndex].first;
    }
    this->submeshesSettings.Initialize(submeshesMaterials, textureCoordinateSetMappings);
    if (submeshesSettings != nullptr)
        submeshesSettings->SyncTo(this->submeshesSettings, true);
    else
        submeshesSettings = &this->submeshesSettings;

    int textureCoordinateSetIndexForTangents = -1;

    //Set more information for each submesh
    GeometryStateSubmesh::SubmeshesTextureCoordSetDimensionArray submeshesTextureCoordSetDimensions; 
    submeshesTextureCoordSetDimensions.resize(submeshesSettings->size());
    size_t submeshIndex = 0;
    for (auto submesh : this->submeshes)
    {
        int foundMappingIndex = submeshesSettings->FindIndexOfMaterial(submesh->material);
        auto& submeshSettings = (*submeshesSettings)[foundMappingIndex];
        
        //Submesh name
        submesh->name = submeshSettings.submeshName;

        //Submesh mapping index
        submesh->mappingIndex = foundMappingIndex;

        if (submesh->submeshProperties.useCustomSubmeshes)
        {
            //Render queue settings
            submesh->submeshProperties.renderQueueName = submeshSettings.renderQueueName;
            submesh->submeshProperties.renderPriority = submeshSettings.renderPriority;
        }

        //Texture coordinate sets
        auto& textureCoordinateSetMappings = submeshSettings.textureCoordinateSetMappings;
        auto& textureCoordSetDimensions = submeshesTextureCoordSetDimensions[submeshIndex];
        textureCoordSetDimensions.resize(textureCoordinateSetMappings.size());
        for (size_t textureCoordinateSetIndex = 0; textureCoordinateSetIndex < textureCoordinateSetMappings.size(); textureCoordinateSetIndex++)
        {
            auto componentCount = textureCoordinateSetMappings[textureCoordinateSetIndex].GetComponentCount();

            //Dimensions
            textureCoordSetDimensions[textureCoordinateSetIndex] = componentCount;

            //Hold onto valid channel for generating tangents
            if (textureCoordinateSetIndexForTangents == -1 && this->geometryType == GeometryStateSubmesh::MESH && componentCount == 2)
                textureCoordinateSetIndexForTangents = textureCoordinateSetMappings[textureCoordinateSetIndex].destinationIndex;
        }

        submeshIndex++;
    }

    //Update vertex format for each submesh
    submeshIndex = 0;
    for (auto submesh : this->submeshes)
    {
        int foundMappingIndex = submeshesSettings->FindIndexOfMaterial(submesh->material);
        auto& submeshSettings = (*submeshesSettings)[foundMappingIndex];
        
        auto& textureCoordinateSetMappings = submeshSettings.textureCoordinateSetMappings;
        auto& textureCoordSetDimensions = submeshesTextureCoordSetDimensions[submeshIndex];
        
        //Tangent/binormal
        if (this->submeshProperties.generateTangents && textureCoordinateSetIndexForTangents >= 0)
        {
            //Tangent
            submesh->submeshProperties.vertexFormatElements.push_back(WxGpuVertexFormatStructUtilities::CreateElement(WxGpuVertexFormatStructMetadata::ElementID::TANGENT, WxNumericStructElementType::FLOAT4));

            //Binormal
            if (this->submeshProperties.generateBinormals)
                submesh->submeshProperties.vertexFormatElements.push_back(WxGpuVertexFormatStructUtilities::CreateElement(WxGpuVertexFormatStructMetadata::ElementID::BINORMAL, WxNumericStructElementType::FLOAT3));
        }

        //Texture coordinates
        for (size_t textureCoordinateSetIndex = 0; textureCoordinateSetIndex < textureCoordinateSetMappings.size(); textureCoordinateSetIndex++)
        {            
            submesh->submeshProperties.vertexFormatElements.push_back(WxGpuVertexFormatStructUtilities::CreateTextureCoordinate(textureCoordinateSetIndex, textureCoordSetDimensions[textureCoordinateSetIndex]));
        }

        //Point size
        if (submesh->submeshProperties.primitiveType == PrimitiveType::POINTS)
            submesh->submeshProperties.vertexFormatElements.push_back(WxGpuVertexFormatStructUtilities::CreateElement(WxGpuVertexFormatStructMetadata::ElementID::POINT_SIZE, WxNumericStructElementType::FLOAT1));

        //Color
        if (submesh->submeshProperties.hasVertexColors)
            submesh->submeshProperties.vertexFormatElements.push_back(WxGpuVertexFormatStructUtilities::CreateElement(WxGpuVertexFormatStructMetadata::ElementID::COLOR_0, WxNumericStructElementType::FLOAT4));

        submeshIndex++;
    }

    //Get all faces and vertices, putting them into the appropriate submeshes
    if (this->geometryType == GeometryStateSubmesh::MESH)
    {
        //Get the points
        this->meshPositions.resize(mesh->numVerts);
        for (int vertexIndex = 0; vertexIndex < mesh->numVerts; vertexIndex++)
        {
            auto point = mesh->verts[vertexIndex];
            coordinateConverter.ConvertPoint(point);
            this->meshPositions[vertexIndex].Set(point.x * scale, point.y * scale, point.z * scale);
        }

        //Get the normals and tangents
        this->meshNormals.Create(*mesh, coordinateConverter, this->submeshProperties.primitiveType);        
        
        //Get the vertices
        for (int faceIndex = 0; faceIndex < mesh->numFaces; faceIndex++)
        {
            auto& face = mesh->faces[faceIndex];

            int faceMaterialIndex = -1;
            if (!objectMaterials.empty())
                faceMaterialIndex = face.getMatID() % objectMaterials.size();
            
            auto submesh = primitiveSubmeshes[faceIndex];
            submesh->primitiveCount++;

            //Get vertices and face indices
            for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
            {
                //Visibility
                explodedEdgeVisibilities.push_back((face.flags & (EDGE_A << vertexIndex)) ? true : false);
                
                Vertex vertex;

                //Original index
                vertex.originalCornerIndex = vertexIndex;
                vertex.originalFaceVertexIndex = face.v[vertexIndex];
                vertex.originalFaceIndex = faceIndex;
                vertex.smoothingGroups = face.smGroup;
                
                //Position
                auto position = mesh->verts[face.v[vertexIndex]];
                coordinateConverter.ConvertPoint(position);
                vertex.SetPosition(position.x * scale, position.y * scale, position.z * scale);

                //Normal
                auto normal = this->meshNormals.GetNormal(vertexIndex, face.v[vertexIndex], faceIndex, face.smGroup);
                vertex.SetNormal(normal.x, normal.y, normal.z);

                //Tangent
                auto tangent = this->meshNormals.GetTangent(vertexIndex, face.v[vertexIndex], faceIndex, face.smGroup);
                vertex.tangent = tangent;

                //Color
                if (submesh->submeshProperties.hasVertexColors && hasRealVertexColors)
                {
                    auto color = colorMapChannel->tv[colorMapChannel->tf[faceIndex].t[vertexIndex]];
                    float alpha = 1;
                    if (textureMapChannels.alphaMapChannel != nullptr)
                        alpha = textureMapChannels.alphaMapChannel->tv[textureMapChannels.alphaMapChannel->tf[faceIndex].t[vertexIndex]].x;
                    vertex.SetColor(color.x, color.y, color.z, alpha);
                }

                //Point size
                if (submesh->submeshProperties.primitiveType == PrimitiveType::POINTS)
                    vertex.pointSize = submesh->submeshProperties.pointSize;

                //Texture coordinates
                auto& textureCoordinateSetMappings = (*submeshesSettings)[submesh->mappingIndex].textureCoordinateSetMappings;
                for (size_t textureCoordinateSetIndex = 0; textureCoordinateSetIndex < textureCoordinateSetMappings.size(); ++textureCoordinateSetIndex)
                {
                    FinjinVector3 textureCoordinate;

                    //Make sure the mapping channel is valid
                    auto map = textureMapChannels.GetMap(textureCoordinateSetMappings[textureCoordinateSetIndex].source.number);
                    if (map != nullptr &&
                        vertexIndex < map->vnum && faceIndex < map->fnum) //Prevents a rare problem that occurs if the map channel was invalidated
                    {
                        auto& maxTextureCoordinate = map->tv[map->tf[faceIndex].t[vertexIndex]];                    
                        textureCoordinate.Set(maxTextureCoordinate.x, 1.0f - maxTextureCoordinate.y, maxTextureCoordinate.z);
                    }
                    
                    vertex.AddTexCoord((int)textureCoordinateSetIndex, textureCoordinate);
                }

                //Add vertex
                explodedVertices.push_back(vertex);
            }
        }        
    }
    else
    { 
        //Iterate over lines and segments
        int absoluteSegmentIndex = 0;
        for (int lineIndex = 0; lineIndex < shape.numLines; lineIndex++)
        {
            auto& line = shape.lines[lineIndex];
            
            for (int segmentIndex = 0; segmentIndex < line.Segments(); segmentIndex++)
            {
                auto submesh = primitiveSubmeshes[absoluteSegmentIndex++];
                submesh->primitiveCount++;

                for (int vertexIndex = 0; vertexIndex < 2; vertexIndex++)
                {
                    Vertex vertex;

                    //Original index
                    vertex.originalCornerIndex = 0;
                    vertex.originalFaceVertexIndex = 0;
                    
                    //Position
                    auto position = line.pts[(segmentIndex + vertexIndex) % line.Verts()].p;
                    coordinateConverter.ConvertPoint(position);
                    vertex.SetPosition(position.x * scale, position.y * scale, position.z * scale); 

                    auto isNewVertex = false;
                    unsigned int actualVertexIndex = submesh->vertexList.Add(vertex, &isNewVertex, fastVertices);
                    submesh->indices.push_back(actualVertexIndex);

                    //Initialize/update bounds
                    UpdateBounds(lineIndex == 0 && segmentIndex == 0 && vertexIndex == 0, isNewVertex, vertex.position);
                }
            }
        }
    }

    //Clean up
    if (triObject != nullptr && triObject != maxObject)
        triObject->DeleteThis();

    return FinishCreate(submeshesTextureCoordSetDimensions, explodedVertices, explodedEdgeVisibilities, textureCoordinateSetIndexForTangents, primitiveSubmeshes);
}

bool GeometryState::SamplePoints
    (
    std::vector<FinjinVector3>& positions, 
    const CoordinateSystemConverter& coordinateConverter,
    float scale,
    TimeAccessor time,
    GeometryStateFlags flags,
    MeshNormals* normals
    )
{
    auto result = false;

    this->meshSkeleton.Enable(NoneSet(flags & GeometryStateFlags::NO_SKELETON_EFFECT));
    this->meshMorpher.Enable(NoneSet(flags & GeometryStateFlags::NO_MORPHER_EFFECT));

    //Evaluate object
    ShapeObject* shapeObject = nullptr;
    PolyShape shape;
    TriObject* triObject = nullptr;
    Mesh* mesh = nullptr;
    auto maxObject = this->createObject.node->EvalWorldState(time.GetNativeTime()).obj;
    if (maxObject->SuperClassID() == SHAPE_CLASS_ID)
    {
        shapeObject = static_cast<ShapeObject*>(maxObject);        
        shapeObject->MakePolyShape(0, shape);

        //Count points
        int pointCount = 0;
        for (int lineIndex = 0; lineIndex < shape.numLines; lineIndex++)
        {
            auto& line = shape.lines[lineIndex];            
            pointCount += line.Verts();
        }

        //Get the points
        positions.reserve(pointCount);
        for (int lineIndex = 0; lineIndex < shape.numLines; lineIndex++)
        {
            auto& line = shape.lines[lineIndex];
            
            for (int pointIndex = 0; pointIndex < line.Verts(); pointIndex++)
            {
                auto point = line.pts[pointIndex].p;                                    
                coordinateConverter.ConvertPoint(point);
                positions.push_back(FinjinVector3(point.x * scale, point.y * scale, point.z * scale));
            }
        }

        result = true;
    }
    else if (maxObject->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
    {        
        triObject = static_cast<TriObject*>(maxObject->ConvertToType(time.GetNativeTime(), Class_ID(TRIOBJ_CLASS_ID, 0)));
        auto mesh = &triObject->GetMesh();

        //Get the points
        positions.resize(mesh->numVerts);
        for (int vertexIndex = 0; vertexIndex < mesh->numVerts; vertexIndex++)
        {
            auto point = mesh->verts[vertexIndex];
            coordinateConverter.ConvertPoint(point);
            positions[vertexIndex].Set(point.x * scale, point.y * scale, point.z * scale);
        }

        //Get the normals
        if (normals != nullptr)
            normals->Create(*mesh, coordinateConverter, this->submeshProperties.primitiveType);
        
        result = true;
    }

    //Clean up
    if (triObject != nullptr && triObject != maxObject)
        triObject->DeleteThis();

    this->meshSkeleton.Restore();
    this->meshMorpher.Restore();
    
    return result;
}

