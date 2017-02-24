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
#include "Logger.hpp"
#include "MayaUtilities.hpp"
#include "MeshMorpher.hpp"
#include "MeshSkeleton.hpp"
#include "TimeChanger.hpp"

using namespace Finjin::Exporter;


//Local classes----------------------------------------------------------------
struct OrderedUVSetName
{
    OrderedUVSetName(const MString& name)
    {
        this->name = name;
        this->isValid = name.length() > 0;
    }
    OrderedUVSetName(bool isValid)
    {
        this->isValid = isValid;
    }
    MString name;
    bool isValid;
};
typedef std::vector<OrderedUVSetName> OrderedUVSetNames;
        

//Local functions--------------------------------------------------------------
static MaterialAccessor::Culling GetCulling(MFnMesh& mesh)
{
    MaterialAccessor::Culling culling = MaterialAccessor::DEFAULT_CULLING;

    if (!mesh.object().isNull())
    {
        MPlug backfaceCullingPlug = mesh.findPlug("backfaceCulling");
        MPlug doubleSidedPlug = mesh.findPlug("doubleSided");
        MPlug oppositePlug = mesh.findPlug("opposite");
    
        if (!backfaceCullingPlug.isNull() && !doubleSidedPlug.isNull() && !oppositePlug.isNull())
        {
            short backfaceCulling = backfaceCullingPlug.asShort();
            bool doubleSided = doubleSidedPlug.asBool();
            bool opposite = oppositePlug.asBool();            
            if (doubleSided && backfaceCulling == 0)
            {
                //Double-sided and backface culling off
                culling = MaterialAccessor::NO_CULLING;        
            }
            else if (!doubleSided && opposite)
            {
                //Single-sided and opposite faces
                culling = MaterialAccessor::OPPOSITE_CULLING;
            }
        }
    }

    return culling;
}

static bool HasColors(MObject& meshObject)
{
    MItMeshPolygon faceIter(meshObject);
    for (; !faceIter.isDone(); faceIter.next())
    {
        MIntArray faceIndices;
        faceIter.getVertices(faceIndices);
        
        int triangleCount;
        faceIter.numTriangles(triangleCount);
        for (int triangleIndex = 0; triangleIndex < triangleCount; triangleIndex++)
        {
            //Extract triangle vertex indices
            MPointArray trianglePoints;
            MIntArray triangleIndices;
            faceIter.getTriangle(triangleIndex, trianglePoints, triangleIndices);
            
            //Convert triangle-indices to face-relative indices
            MIntArray localTriangleIndices;
            MayaUtilities::GetLocalMeshIndices(localTriangleIndices, faceIndices, triangleIndices);
            for (int i = 0; i < 3; i++)
            {
                int index = localTriangleIndices[i];
                if (index != -1 && faceIter.hasColor(index))
                    return true;
            }
        }
    }

    return false;
}

static void GetMeshEdges(MObject& meshObject, FinjinEdgeSet& allMeshEdges)
{
    MStatus status;
    MFnMesh mesh(meshObject, &status);
    if (status == MStatus::kSuccess)
    {    
        //Get all mesh edges    
        int2 edgeVertexIndices;
        unsigned int edgeCount = mesh.numEdges();
        for (unsigned int edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++)
        {
            mesh.getEdgeVertices(edgeIndex, edgeVertexIndices);
            allMeshEdges.insert(FinjinEdge(edgeVertexIndices[0], edgeVertexIndices[1]));            
        }
    }
}


//Implementation---------------------------------------------------------------
bool GeometryState::CanHandle(ObjectAccessor object, TimeAccessor time)
{
    if (object.obj.hasFn(MFn::kMesh) || object.obj.hasFn(MFn::kNurbsSurface) || object.obj.hasFn(MFn::kNurbsCurve))
    {
        MFnDagNode dagNode(object.obj);
        return !dagNode.isIntermediateObject() && !MayaUtilities::IsEmptyMeshOrCurve(object.obj);
    }
    else
        return false;
}

void GeometryState::GetSubmeshesSettings(ObjectAccessor object, SubmeshesSettings& submeshesSettings, GeometryStateFlags flags)
{
    submeshesSettings.clear();
    
    //Get the mesh, converting from a NURBs surface if necessary
    MObject meshObject;
    MFnMesh mesh;
    MFnDagNode tesselatedMeshTransformNode;
    if (object.obj.hasFn(MFn::kNurbsSurface))
    {
        MFnNurbsSurface nurbsSurface(object.obj);
        MObject tesselatedMeshTransform = nurbsSurface.tesselate();
        tesselatedMeshTransformNode.setObject(tesselatedMeshTransform);
        meshObject = tesselatedMeshTransformNode.child(0);        
    }
    else if (object.obj.hasFn(MFn::kMesh))
        meshObject = object.obj;        
 
    if (!meshObject.isNull())
    {
        mesh.setObject(meshObject);

        auto culling = MaterialAccessor::DEFAULT_CULLING;
        if (NoneSet(flags & GeometryStateFlags::NO_EXTRA_MATERIALS))
            culling = GetCulling(mesh);        

        //Get submesh materials
        MaterialAccessorVector submeshMaterials;
        MaterialAccessorSet materialSet;
        MObjectArray shaders;
        MIntArray shaderPolygonIndices;
        mesh.getConnectedShaders(0, shaders, shaderPolygonIndices);                
        for (unsigned int i = 0; i < shaderPolygonIndices.length(); i++)
        {
            MObject shader;
            if (shaderPolygonIndices[i] != -1)
                shader = MayaUtilities::GetBestMaterial(shaders[shaderPolygonIndices[i]]);

            MaterialAccessor material(shader);
            material.culling = culling;
            if (materialSet.find(material) == materialSet.end())
            {
                materialSet.insert(material);
                submeshMaterials.push_back(material);
            }
        }

        //Get UV set names
        TextureCoordinateSetMappings textureCoordinateSets;
        MStringArray uvSetNames;
        mesh.getUVSetNames(uvSetNames);
        textureCoordinateSets.resize(uvSetNames.length());
        for (size_t textureCoordinateSetIndex = 0; 
            textureCoordinateSetIndex < uvSetNames.length(); 
            textureCoordinateSetIndex++)
        {
            textureCoordinateSets[textureCoordinateSetIndex].source.name = ApplicationStringToWxString(uvSetNames[textureCoordinateSetIndex]);
        }

        //Done
        submeshesSettings.Initialize(submeshMaterials, textureCoordinateSets);

        //Clean up
        if (!tesselatedMeshTransformNode.object().isNull())
            MayaUtilities::DeleteObject(tesselatedMeshTransformNode);
    }
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
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("GeometryState::Create(%s)"), object.GetLocalName().wx_str());

    if (!CanHandle(object, time))
        return false;

    TimeChanger timeChanger(time);

    auto fastVertices = AnySet(flags & GeometryStateFlags::FAST_VERTICES);

    StartCreate(meshName, object, coordinateConverter, scale, time, flags, referencePose);

    //Get the mesh, converting from a NURBs surface if necessary
    MObject meshObject;
    MFnMesh mesh;
    MFnNurbsCurve nurbsCurve;
    MFnDagNode tesselatedMeshTransformNode;
    if (object.obj.hasFn(MFn::kNurbsSurface))
    {
        MFnNurbsSurface nurbsSurface(object.obj);
        MObject tesselatedMeshTransform = nurbsSurface.tesselate();
        tesselatedMeshTransformNode.setObject(tesselatedMeshTransform);
        meshObject = tesselatedMeshTransformNode.child(0);        
    }
    else if (object.obj.hasFn(MFn::kMesh))
        meshObject = object.obj;        
    else if (object.obj.hasFn(MFn::kNurbsCurve))
        nurbsCurve.setObject(object.obj);
    
    //Maya has issues when an ANIMATED skeleton is attached to the mesh:
    //  -Calling mesh.getConnectedShaders() causes mesh.numVertices() to return 0
    //  -Using MItMeshPolygon causes mesh.numVertices() to return 0
    //  -Calling mesh.getEdgeVertices() after using MItMeshPolygon will cause Maya to crash, even though 
    //   mesh.numEdges() returns a valid value
    //To avoid these issues, get the necessary data before initializing the MFnMesh that is used for
    //the duration of this function
    auto hasRealVertexColors = false;
    FinjinEdgeSet allMeshEdges;
    int numVerts = 0;
    if (!meshObject.isNull())
    {
        hasRealVertexColors = HasColors(meshObject); 
        GetMeshEdges(meshObject, allMeshEdges);
        
        mesh.setObject(meshObject);        
        
        numVerts = mesh.numVertices();
    }    

    this->submeshProperties.vertexFormatElements.push_back(WxGpuVertexFormatStructUtilities::CreateElement(WxGpuVertexFormatStructMetadata::ElementID::POSITION, WxNumericStructElementType::FLOAT3));

    //Geometry type and coloring
    if (!mesh.object().isNull())
    {
        this->submeshProperties.vertexFormatElements.push_back(WxGpuVertexFormatStructUtilities::CreateElement(WxGpuVertexFormatStructMetadata::ElementID::NORMAL, WxNumericStructElementType::FLOAT3));

        this->geometryType = GeometryStateSubmesh::MESH;
        this->submeshProperties.hasVertexColors = hasRealVertexColors || AnySet(flags & GeometryStateFlags::FORCE_WHITE_VERTEX_COLORS);        
    }
    else if (!nurbsCurve.object().isNull())
    {
        this->geometryType = GeometryStateSubmesh::SHAPE;
        this->submeshProperties.hasNormals = false;
        this->submeshProperties.requiresDummyBone = true;        
    }
    auto culling = MaterialAccessor::DEFAULT_CULLING;
    if (NoneSet(flags & GeometryStateFlags::NO_EXTRA_MATERIALS))
        culling = GetCulling(mesh);        

    //Create the submeshes that faces will be placed into
    std::vector<bool> explodedEdgeVisibilities;
    std::vector<Vertex> explodedVertices;

    MaterialAccessorVector materials;
    std::vector<GeometryStateSubmesh*> primitiveSubmeshes;
    {
        MaterialsToSubmeshes materialsToSubmeshes;

        if (this->geometryType == GeometryStateSubmesh::MESH)
        {
            //Get shaders and face-to-shader info, creating a "default" submesh if 
            //at least one face doesn't have a shader associated with it
            MObjectArray shaders;
            MIntArray shaderPolygonIndices;
            mesh.getConnectedShaders(0, shaders, shaderPolygonIndices);
            primitiveSubmeshes.resize(shaderPolygonIndices.length());
            
            for (unsigned int i = 0; i < shaderPolygonIndices.length(); i++)
            {
                MObject shader;
                if (shaderPolygonIndices[i] != -1)
                    shader = MayaUtilities::GetBestMaterial(shaders[shaderPolygonIndices[i]]);
                MaterialAccessor material(shader);
                material.culling = culling;

                auto isNew = false;
                primitiveSubmeshes[i] = GetMaterialSubmesh(materialsToSubmeshes, material, &isNew);
                if (isNew)
                {
                    materials.push_back(material);
                    primitiveSubmeshes[i]->Reserve(numVerts);
                }
            }
        }
        else
        {
            //Create just one submesh for the shape
            auto isNew = false;
            auto objectMaterialSubmesh = GetMaterialSubmesh(materialsToSubmeshes, MObject(), &isNew);
            primitiveSubmeshes.push_back(objectMaterialSubmesh);
            if (isNew)
                objectMaterialSubmesh->Reserve(numVerts);
        }
    }

    int textureCoordinateSetIndexForTangents = -1;

    //Get all faces and vertices, putting them into the appropriate submeshes
    GeometryStateSubmesh::SubmeshesTextureCoordSetDimensionArray submeshesTextureCoordSetDimensions; 
    if (this->geometryType == GeometryStateSubmesh::MESH)
    {
        //Get all vertices and normals
        MPointArray points;
        mesh.getPoints(points, GET_POINTS_NORMALS_SPACE);
        MFloatVectorArray normals;
        mesh.getNormals(normals, GET_POINTS_NORMALS_SPACE);
        MFloatVectorArray tangents;
        mesh.getTangents(tangents, GET_POINTS_NORMALS_SPACE);

        //Get the points
        this->meshPositions.resize(points.length());
        for (unsigned int pointIndex = 0; pointIndex < points.length(); pointIndex++)
        {
            MPoint point(points[pointIndex]);
            coordinateConverter.ConvertPoint(point);
            this->meshPositions[pointIndex].Set(point.x * scale, point.y * scale, point.z * scale);
        }

        //bool flipNormals = false;
        //mesh.findPlug("opposite", true).getValue(flipNormals);
        //double normalScale = flipNormals ? -1.0 : 1.0;

        //Get UV set names and resize various data structures used to track UV info
        MStringArray uvSetNames;
        mesh.getUVSetNames(uvSetNames);

        TextureCoordinateSetMappings textureCoordinateSetMappings;
        textureCoordinateSetMappings.resize(uvSetNames.length());
        for (size_t textureCoordinateSetIndex = 0; textureCoordinateSetIndex < uvSetNames.length(); textureCoordinateSetIndex++)
            textureCoordinateSetMappings[textureCoordinateSetIndex].source.name = ApplicationStringToWxString(uvSetNames[textureCoordinateSetIndex]);
        this->submeshesSettings.Initialize(materials, textureCoordinateSetMappings);
        if (submeshesSettings != nullptr)
            submeshesSettings->SyncTo(this->submeshesSettings, true);
        else
            submeshesSettings = &this->submeshesSettings;

        //Set more information for each submesh
        std::vector<OrderedUVSetNames> submeshesOrderedUVSetNames;
        submeshesOrderedUVSetNames.resize(submeshesSettings->size());
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

            submeshesOrderedUVSetNames[submeshIndex].reserve(textureCoordinateSetMappings.size());

            for (size_t textureCoordinateSetIndex = 0; textureCoordinateSetIndex < textureCoordinateSetMappings.size(); textureCoordinateSetIndex++)
            {
                auto componentCount = textureCoordinateSetMappings[textureCoordinateSetIndex].GetComponentCount();

                //Dimensions
                textureCoordSetDimensions[textureCoordinateSetIndex] = componentCount;

                //Hold onto valid channel for generating tangents
                if (textureCoordinateSetIndexForTangents == -1 && componentCount == 2)
                    textureCoordinateSetIndexForTangents = textureCoordinateSetMappings[textureCoordinateSetIndex].destinationIndex;

                MString uvSetName = WxStringToApplicationString(textureCoordinateSetMappings[textureCoordinateSetIndex].source.name);
                if (MayaUtilities::FindString(uvSetNames, uvSetName) != -1)
                    submeshesOrderedUVSetNames[submeshIndex].push_back(uvSetName);
                else
                    submeshesOrderedUVSetNames[submeshIndex].push_back(false);
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

            //Color
            if (submesh->submeshProperties.hasVertexColors)
                submesh->submeshProperties.vertexFormatElements.push_back(WxGpuVertexFormatStructUtilities::CreateElement(WxGpuVertexFormatStructMetadata::ElementID::COLOR_0, WxNumericStructElementType::FLOAT4));

            //Point size
            if (submesh->submeshProperties.primitiveType == PrimitiveType::POINTS)
                submesh->submeshProperties.vertexFormatElements.push_back(WxGpuVertexFormatStructUtilities::CreateElement(WxGpuVertexFormatStructMetadata::ElementID::POINT_SIZE, WxNumericStructElementType::FLOAT1));

            submeshIndex++;
        }
        
        //Process faces
        MItMeshPolygon faceIter(mesh.object());
        this->meshNormals.normalFaces.resize(faceIter.count());
        this->meshNormals.tangentFaces.resize(this->meshNormals.normalFaces.size());
        explodedEdgeVisibilities.reserve(this->meshNormals.normalFaces.size() * 3);
        explodedVertices.reserve(this->meshNormals.normalFaces.size() * 3);
        for (unsigned int faceIndex = 0; !faceIter.isDone(); faceIter.next(), faceIndex++)
        {
            auto submesh = primitiveSubmeshes[faceIndex];

            auto& orderedUVSetNames = submeshesOrderedUVSetNames[submesh->index];

            //Get face normals
            MVectorArray normalArray;
            faceIter.getNormals(normalArray, GET_POINTS_NORMALS_SPACE);
            this->meshNormals.normalFaces[faceIndex].SetDegree(normalArray.length());
            this->meshNormals.tangentFaces[faceIndex].SetDegree(normalArray.length());
            for (unsigned int cornerIndex = 0; cornerIndex < normalArray.length(); cornerIndex++)
            {
                FinjinVector3 normal(normalArray[cornerIndex].x, normalArray[cornerIndex].y, normalArray[cornerIndex].z);
                coordinateConverter.ConvertPoint(normal);
                this->meshNormals.normalFaces[faceIndex].Set(cornerIndex, faceIter.normalIndex(cornerIndex), normal);

                auto tangentIndex = faceIter.tangentIndex(cornerIndex);
                FinjinVector4 tangent(tangents[tangentIndex].x, tangents[tangentIndex].y, tangents[tangentIndex].z, 1);
                //auto tangentID = mesh.getTangentId(faceIndex, faceIndices[cornerIndex]);
                //tangent.w = mesh.isRightHandedTangent(tangentID, &uvSetName) ? 1.0f : -1.0f;
                coordinateConverter.ConvertPoint((FinjinVector3&)tangent);
                this->meshNormals.tangentFaces[faceIndex].Set(cornerIndex, tangentIndex, tangent);
            }

            //Process each triangle in the face
            int triangleCount;
            faceIter.numTriangles(triangleCount);
            for (int triangleIndex = 0; triangleIndex < triangleCount; triangleIndex++)
            {
                submesh->primitiveCount++;

                //Get triangle
                MPointArray trianglePoints;
                MIntArray triangleIndices;
                faceIter.getTriangle(triangleIndex, trianglePoints, triangleIndices);

                //Convert triangle indices from mesh-relative to face-relative indices
                MIntArray faceIndices;
                faceIter.getVertices(faceIndices);
                MIntArray localTriangleVertexIndices;
                MayaUtilities::GetLocalMeshIndices(localTriangleVertexIndices, faceIndices, triangleIndices);

                //Process triangle vertices
                for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
                {
                    //Visibility
                    FinjinEdge edge(triangleIndices[vertexIndex], triangleIndices[(vertexIndex + 1) % 3]);
                    explodedEdgeVisibilities.push_back(allMeshEdges.find(edge) != allMeshEdges.end());

                    Vertex vertex;

                    //Original index
                    vertex.originalCornerIndex = vertexIndex;
                    vertex.originalFaceVertexIndex = triangleIndices[vertexIndex];
                    vertex.originalFaceIndex = faceIndex;
                    
                    //Position
                    MPoint position(points[faceIter.vertexIndex(localTriangleVertexIndices[vertexIndex])]);
                    coordinateConverter.ConvertPoint(position);
                    vertex.SetPosition(position.x * scale, position.y * scale, position.z * scale);

                    //Normal
                    MFloatVector normalVector(normals[faceIter.normalIndex(localTriangleVertexIndices[vertexIndex])]);
                    //normalVector *= normalScale;
                    MPoint normal(normalVector);
                    coordinateConverter.ConvertPoint(normal);
                    vertex.SetNormal(normal.x, normal.y, normal.z);

                    //Tangent
                    if (this->submeshProperties.generateTangents && textureCoordinateSetIndexForTangents >= 0)
                    {
                        auto tangentIndex = faceIter.tangentIndex(localTriangleVertexIndices[vertexIndex]);
                        vertex.tangent.Set(tangents[tangentIndex].x, tangents[tangentIndex].y, tangents[tangentIndex].z, 1);
                        //auto tangentID = mesh.getTangentId(faceIndex, faceIndices[localTriangleVertexIndices[vertexIndex]]);
                        //vertex.tangent.w = mesh.isRightHandedTangent(tangentID, &orderedUVSetNames[textureCoordinateSetIndexForTangents]) ? 1.0f : -1.0f;
                        coordinateConverter.ConvertPoint((FinjinVector3&)vertex.tangent);
                    }
                    
                    //Point size
                    if (submesh->submeshProperties.primitiveType == PrimitiveType::POINTS)
                        vertex.pointSize = submesh->submeshProperties.pointSize;

                    //Color
                    if (faceIter.hasColor(localTriangleVertexIndices[vertexIndex]))
                    {
                        MColor color;
                        faceIter.getColor(color, localTriangleVertexIndices[vertexIndex]);                        
                        vertex.SetColor(color.r, color.g, color.b, color.a);
                    }

                    //Texture coordinates
                    float2 uv;
                    for (size_t textureCoordinateSetIndex = 0; 
                        textureCoordinateSetIndex < orderedUVSetNames.size(); 
                        textureCoordinateSetIndex++)
                    {
                        if (orderedUVSetNames[textureCoordinateSetIndex].isValid)
                        {
                            auto& uvSetName = orderedUVSetNames[textureCoordinateSetIndex].name;
                            if (faceIter.getUV(localTriangleVertexIndices[vertexIndex], uv, &uvSetName) == MStatus::kSuccess)
                                vertex.AddTexCoord((int)textureCoordinateSetIndex, uv[0], 1 - uv[1], 0);
                            else
                                vertex.AddTexCoord((int)textureCoordinateSetIndex, 0, 0, 0);
                        }
                        else
                            vertex.AddTexCoord((int)textureCoordinateSetIndex, 0, 0, 0);
                    }

                    //Add vertex
                    explodedVertices.push_back(vertex);
                }
            }
        }
    }
    else
    {
        //Everything goes into the same submesh
        auto submesh = primitiveSubmeshes[0];

        MPointArray controlPoints;
        nurbsCurve.getCVs(controlPoints);
        int controlPointCount = controlPoints.length();
        
        //Reserve an appropriate number of indices
        int totalControlPointCount = controlPointCount;
        submesh->indices.reserve(totalControlPointCount * 2);

        //Convert control points to vertices
        std::vector<unsigned int> controlPointIndices;
        controlPointIndices.reserve(controlPointCount);
        for (unsigned int controlPointIndex = 0; controlPointIndex < controlPoints.length(); controlPointIndex++)
        {
            Vertex vertex;

            //Original index
            vertex.originalCornerIndex = 0;
            vertex.originalFaceVertexIndex = 0;

            //Position
            auto position = controlPoints[controlPointIndex];
            coordinateConverter.ConvertPoint(position);
            vertex.SetPosition(position.x * scale, position.y * scale, position.z * scale);

            //Add vertex and index
            auto isNewVertex = false;
            unsigned int actualVertexIndex = submesh->vertexList.Add(vertex, &isNewVertex, fastVertices);
            controlPointIndices.push_back(actualVertexIndex);
            
            //Initialize/update bounds
            UpdateBounds(controlPointIndex == 0, isNewVertex, vertex.position);
        }

        //Determine the line segment count
        int segmentCount = controlPointCount;            
        if (nurbsCurve.form() == MFnNurbsCurve::kOpen)
            segmentCount--;

        //Add to primitive count
        submesh->primitiveCount += segmentCount;

        //Get line segment indices
        for (int segmentIndex = 0; segmentIndex < segmentCount; segmentIndex++)
        {
            submesh->indices.push_back(controlPointIndices[segmentIndex]);
            submesh->indices.push_back(controlPointIndices[(segmentIndex + 1) % controlPointCount]);
        }
    }

    //Delete the tesselated mesh if necessary
    if (!tesselatedMeshTransformNode.object().isNull())
        MayaUtilities::DeleteObject(tesselatedMeshTransformNode);

    return FinishCreate(submeshesTextureCoordSetDimensions, explodedVertices, explodedEdgeVisibilities, textureCoordinateSetIndexForTangents, primitiveSubmeshes);
}

bool GeometryState::SamplePoints
    (
    std::vector<FinjinVector3>& points, 
    const CoordinateSystemConverter& coordinateConverter,
    float scale,
    TimeAccessor time,
    GeometryStateFlags flags,
    MeshNormals* normals
    )
{
    auto result = false;

    TimeChanger timeChanger(time);

    this->meshSkeleton.Enable(NoneSet(flags & GeometryStateFlags::NO_SKELETON_EFFECT));
    this->meshMorpher.Enable(NoneSet(flags & GeometryStateFlags::NO_MORPHER_EFFECT));

    //Evaluate object
    MFnMesh mesh;
    MFnNurbsCurve nurbsCurve;
    MFnDagNode tesselatedMeshTransformNode;
    if (this->createObject.obj.hasFn(MFn::kNurbsSurface))
    {
        MFnNurbsSurface nurbsSurface(this->createObject.obj);
        MObject tesselatedMeshTransform = nurbsSurface.tesselate();
        tesselatedMeshTransformNode.setObject(tesselatedMeshTransform);
        mesh.setObject(tesselatedMeshTransformNode.child(0));
    }
    else if (this->createObject.obj.hasFn(MFn::kMesh))
        mesh.setObject(this->createObject.obj);
    else if (this->createObject.obj.hasFn(MFn::kNurbsCurve))
        nurbsCurve.setObject(this->createObject.obj);

    if (!mesh.object().isNull())
    {
        //Get the points
        MPointArray meshPoints;
        mesh.getPoints(meshPoints, GET_POINTS_NORMALS_SPACE);
        
        points.resize(meshPoints.length());
        MPoint previousPoint(0, 0, 0);
        for (unsigned int pointIndex = 0; pointIndex < meshPoints.length(); pointIndex++)
        {
            MPoint point(meshPoints[pointIndex]);
            auto isPointValid = true;

            //Every once in a great while Maya will return one or two points that are entirely not-a-number (NaN). 
            //If any are NaN, use the previous point. 
            //It's not correct but it's better than having bad numbers
            if (point.x != point.x || point.y != point.y || point.z != point.z) //NaN testing
            {
                isPointValid = false;
                point = previousPoint;                
            }
            
            coordinateConverter.ConvertPoint(point);
            points[pointIndex].Set(point.x * scale, point.y * scale, point.z * scale);
            
            //Only update the 'previous' point if the current one is valid
            if (isPointValid)
                previousPoint = meshPoints[pointIndex];
        }

        //Get the normals
        if (normals != nullptr)
            normals->Create(mesh, coordinateConverter);

        result = true;        
    }
    else if (!nurbsCurve.object().isNull())
    {
        MPointArray controlPoints;
        nurbsCurve.getCVs(controlPoints);
        int controlPointCount = controlPoints.length();
        
        int totalControlPointCount = controlPointCount;
        
        //Get the points
        points.reserve(totalControlPointCount);
        for (unsigned int controlPointIndex = 0; controlPointIndex < controlPoints.length(); controlPointIndex++)
        {
            MPoint position = controlPoints[controlPointIndex];

            //Position
            coordinateConverter.ConvertPoint(position);
            points.push_back(FinjinVector3(position.x * scale, position.y * scale, position.z * scale));
        }

        result = true;
    }
    
    //Clean up
    if (!tesselatedMeshTransformNode.object().isNull())
        MayaUtilities::DeleteObject(tesselatedMeshTransformNode);

    this->meshSkeleton.Restore();
    this->meshMorpher.Restore();
    
    return result;
}
