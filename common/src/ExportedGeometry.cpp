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
#include "ExportedGeometry.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "SceneExportSettings.hpp"
#include "ProgressDialog.hpp"
#include "ApplicationAccessor.hpp"
#include "GeometryState.hpp"
#include "DetectableObjectType.hpp"
#include "FileUtilities.hpp"
#include "MaterialExporter.hpp"
#include "ExporterUtilities.hpp"
#include "FinjinSceneDocument.hpp"
#include "IgnoreObjectType.hpp"
#include "SimpleMeshInfo.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "ApplicationAccessor.hpp"
#include "MeshAnimationsEvaluator.hpp"
#include "MathUtilities.hpp"
#include "AnimationExportSettings.hpp"
#include "AnimationUtilities.hpp"
#include "FinjinGlobalSettings.hpp"
#include "SimpleUri.hpp"
#include "finjin/common/WxNumericStruct.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyValues.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Local functions---------------------------------------------------------------
static void WriteVertexChannels
    (
    WxDataChunkWriter& writer,
    const wxString& formatName,
    const std::vector<WxGpuVertexFormatStruct::Element>& vertexFormatElements,
    const VertexList& vertexList,
    WxError& error
    )
{
    FINJIN_WX_ERROR_METHOD_START(error);

    //Format name
    if (!formatName.empty())
    {
        writer.WriteString(StandardAssetDocumentPropertyNames::FORMAT, formatName, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Channel formats
    writer.WriteChunk(StandardAssetDocumentChunkNames::FORMAT, [&vertexFormatElements](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        //Count
        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, vertexFormatElements.size(), error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Each element
        for (size_t elementIndex = 0; elementIndex < vertexFormatElements.size(); elementIndex++)
        {
            auto& element = vertexFormatElements[elementIndex];

            writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::FORMAT_ELEMENT, elementIndex), [&element](WxDataChunkWriter& writer, WxError& error)
            {
                FINJIN_WX_ERROR_METHOD_START(error)

                //ID
                writer.WriteString(StandardAssetDocumentPropertyNames::ID, WxGpuVertexFormatStructMetadata::ElementIDToString(element.elementID), error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Type
                writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, WxNumericStructElementTypeUtilities::ToString(element.type), error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Vertex count
    writer.WriteCount(StandardAssetDocumentPropertyNames::VERTEX_COUNT, vertexList.size(), error);

    //Channels
    writer.WriteChunk(StandardAssetDocumentChunkNames::CHANNELS, [&vertexFormatElements, &vertexList](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        //Channel count
        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, vertexFormatElements.size(), error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Each channel
        for (size_t vertexFormatElementIndex = 0; vertexFormatElementIndex < vertexFormatElements.size(); vertexFormatElementIndex++)
        {
            auto& element = vertexFormatElements[vertexFormatElementIndex];

            WxChunkName channelChunkName(StandardAssetDocumentChunkNames::CHANNEL, vertexFormatElementIndex);
            writer.WriteChunk(channelChunkName, [&element, &vertexList](WxDataChunkWriter& writer, WxError& error)
            {
                FINJIN_WX_ERROR_METHOD_START(error);

                auto& firstVertex = vertexList[0];

                //Values
                switch (element.elementID)
                {
                    case WxGpuVertexFormatStructMetadata::ElementID::BINORMAL:
                    {
                        writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VALUES, &firstVertex.binormal.x, vertexList.size(), WxDataChunkWriteStride(3, sizeof(Vertex)), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        break;
                    }
                    case WxGpuVertexFormatStructMetadata::ElementID::BLEND_INDICES_0: break;
                    case WxGpuVertexFormatStructMetadata::ElementID::BLEND_INDICES_1: break;
                    case WxGpuVertexFormatStructMetadata::ElementID::BLEND_INDICES_2: break;
                    case WxGpuVertexFormatStructMetadata::ElementID::BLEND_INDICES_3: break;
                    case WxGpuVertexFormatStructMetadata::ElementID::BLEND_WEIGHT_0: break;
                    case WxGpuVertexFormatStructMetadata::ElementID::BLEND_WEIGHT_1: break;
                    case WxGpuVertexFormatStructMetadata::ElementID::BLEND_WEIGHT_2: break;
                    case WxGpuVertexFormatStructMetadata::ElementID::BLEND_WEIGHT_3: break;
                    case WxGpuVertexFormatStructMetadata::ElementID::COLOR_0:
                    case WxGpuVertexFormatStructMetadata::ElementID::COLOR_1:
                    case WxGpuVertexFormatStructMetadata::ElementID::COLOR_2:
                    case WxGpuVertexFormatStructMetadata::ElementID::COLOR_3:
                    {
                        auto colorIndex = (size_t)element.elementID - (size_t)WxGpuVertexFormatStructMetadata::ElementID::COLOR_0;
                        auto colorDimension = (size_t)element.type - (size_t)WxNumericStructElementType::FLOAT1 + 1;

                        writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VALUES, &firstVertex.colors[colorIndex].r, vertexList.size() * colorDimension, WxDataChunkWriteStride(colorDimension, sizeof(Vertex)), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        break;
                    }
                    case WxGpuVertexFormatStructMetadata::ElementID::NORMAL:
                    {
                        writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VALUES, &firstVertex.normal.x, vertexList.size() * 3, WxDataChunkWriteStride(3, sizeof(Vertex)), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        break;
                    }
                    case WxGpuVertexFormatStructMetadata::ElementID::POSITION:
                    {
                        writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VALUES, &firstVertex.position.x, vertexList.size() * 3, WxDataChunkWriteStride(3, sizeof(Vertex)), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        break;
                    }
                    case WxGpuVertexFormatStructMetadata::ElementID::POSITION_TRANSFORMED:
                    {
                        writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VALUES, &firstVertex.position.x, vertexList.size() * 3, WxDataChunkWriteStride(3, sizeof(Vertex)), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        break;
                    }
                    case WxGpuVertexFormatStructMetadata::ElementID::POINT_SIZE:
                    {
                        writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VALUES, &firstVertex.pointSize, vertexList.size() * 1, WxDataChunkWriteStride(1, sizeof(Vertex)), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        break;
                    }
                    case WxGpuVertexFormatStructMetadata::ElementID::TANGENT:
                    {
                        writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VALUES, &firstVertex.tangent.x, vertexList.size() * 4, WxDataChunkWriteStride(4, sizeof(Vertex)), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        break;
                    }
                    case WxGpuVertexFormatStructMetadata::ElementID::TEX_COORD_0:
                    case WxGpuVertexFormatStructMetadata::ElementID::TEX_COORD_1:
                    case WxGpuVertexFormatStructMetadata::ElementID::TEX_COORD_2:
                    case WxGpuVertexFormatStructMetadata::ElementID::TEX_COORD_3:
                    case WxGpuVertexFormatStructMetadata::ElementID::TEX_COORD_4:
                    case WxGpuVertexFormatStructMetadata::ElementID::TEX_COORD_5:
                    case WxGpuVertexFormatStructMetadata::ElementID::TEX_COORD_6:
                    case WxGpuVertexFormatStructMetadata::ElementID::TEX_COORD_7:
                    {
                        auto textureCoordinateSetIndex = (int)element.elementID - (int)WxGpuVertexFormatStructMetadata::ElementID::TEX_COORD_0;
                        auto textureCoordinateDimension = (size_t)element.type - (size_t)WxNumericStructElementType::FLOAT1 + 1;

                        auto& textureCoordinate = firstVertex.GetTextureCoordinate(textureCoordinateSetIndex);

                        writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VALUES, &textureCoordinate.x, vertexList.size() * textureCoordinateDimension, WxDataChunkWriteStride(textureCoordinateDimension, sizeof(Vertex)), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        break;
                    }
                    default: break;
                }
            }, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}


//Implementation----------------------------------------------------------------
void ExportedGeometry::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    const FinjinVector3 nullAnimatedVertexNormal(1, 0, 0);

    auto usingSkeleton = this->exportSkeleton && this->geometryState->meshSkeleton.IsValid();
    auto usingMorpher = this->exportPoses && this->geometryState->meshMorpher.IsValid();

    //Type
    writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, StandardAssetDocumentPropertyValues::TypeName::MESH, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Name
    auto name = this->sceneExportSettings->GetAssetName(AssetClass::MESH, this->exportableObject);
    writer.WriteString(StandardAssetDocumentPropertyNames::NAME, name, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Bounds
    float boundingVolumeScale = 1.0f;
    GeometryState::ProcessedLocalBounds allBounds[2] =
        {
        this->geometryState->standardBounds.GetProcessed(boundingVolumeScale), //Non-animated
        this->geometryState->animatedBounds.GetProcessed(boundingVolumeScale) //Animated
        };
    size_t allBoundsCount = FINJIN_COUNT_OF(allBounds);
    if (this->geometryState->standardBounds == this->geometryState->animatedBounds)
        allBoundsCount--; //Don't export animated

    for (size_t i = 0; i < allBoundsCount; i++)
    {
        auto& bounds = allBounds[i];

        auto isAnimated = i == 1;

        //Local bounding volumes
        writer.WriteChunk(StandardAssetDocumentChunkNames::BOUNDING_VOLUME, [this, &bounds, isAnimated](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            //Animated
            if (isAnimated)
            {
                writer.WriteBool(StandardAssetDocumentPropertyNames::ANIMATED, isAnimated, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }

            //Cube
            {
                //Center
                auto center = bounds.cube.GetCenter();
                writer.WriteFloats(StandardAssetDocumentPropertyNames::CUBE_CENTER, &center.x, 3, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Size
                auto size = bounds.cube.GetSize().x;
                writer.WriteFloat(StandardAssetDocumentPropertyNames::CUBE_SIZE, size, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }

            //Box
            if (!bounds.cube.AlmostEquals(bounds.box))
            {
                //Center
                auto center = bounds.box.GetCenter();
                writer.WriteFloats(StandardAssetDocumentPropertyNames::BOX_CENTER, &center.x, 3, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Size
                auto size = bounds.box.GetSize();
                writer.WriteFloats(StandardAssetDocumentPropertyNames::BOX_SIZE, &size.x, 3, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }

            //Standard sphere radius
            writer.WriteFloat(StandardAssetDocumentPropertyNames::RADIUS, bounds.radius, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Shared index/vertex buffers----------------------
    if (!this->geometryState->indexBuffer16.empty() || !this->geometryState->indexBuffer32.empty())
    {
        writer.WriteChunk(StandardAssetDocumentChunkNames::INDEX_BUFFER, [this](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            if (!this->geometryState->indexBuffer16.empty())
            {
                //Type
                writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, StandardAssetDocumentPropertyValues::IndexBufferType::UINT16, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Count
                writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->geometryState->indexBuffer16.size(), error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Indices
                writer.WriteUInt16s(StandardAssetDocumentPropertyNames::VALUES, this->geometryState->indexBuffer16.data(), this->geometryState->indexBuffer16.size(), error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
            else
            {
                //Type
                writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, StandardAssetDocumentPropertyValues::IndexBufferType::UINT32, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Count
                writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->geometryState->indexBuffer32.size(), error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Indices
                writer.WriteUInt32s(StandardAssetDocumentPropertyNames::VALUES, this->geometryState->indexBuffer32.data(), this->geometryState->indexBuffer32.size(), error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    if (!this->geometryState->vertexBuffers.empty())
    {
        writer.WriteChunk(StandardAssetDocumentChunkNames::VERTEX_BUFFERS, [this](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            //Count
            writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->geometryState->vertexBuffers.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Each vertex buffer
            for (size_t vertexBufferIndex = 0; vertexBufferIndex < this->geometryState->vertexBuffers.size(); vertexBufferIndex++)
            {
                auto& vertexBuffer = this->geometryState->vertexBuffers[vertexBufferIndex];

                WxChunkName vertexBufferChunkName(StandardAssetDocumentChunkNames::VERTEX_BUFFER, vertexBufferIndex);
                writer.WriteChunk(vertexBufferChunkName, [this, &vertexBuffer](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    WriteVertexChannels(writer, vertexBuffer.formatName, vertexBuffer.vertexFormatElements, vertexBuffer.vertices, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
    //End of shared index/vertex buffers----------------------

    //Submeshes--------------------
    //LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Creating submeshes"));
    SubmeshNaming submeshNaming;
    this->sceneSettings.GetSubmeshNamingValue(submeshNaming);

    std::set<wxString> submeshNames;
    writer.WriteChunk(StandardAssetDocumentChunkNames::SUBMESHES, [this, &submeshNames, &submeshNaming, usingSkeleton, usingMorpher](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        //Count
        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->geometryState->submeshes.size(), error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Each submesh
        for (auto submeshState : this->geometryState->submeshes)
        {
            WxChunkName submeshChunkName(StandardAssetDocumentChunkNames::SUBMESH, submeshState->index);
            writer.WriteChunk(submeshChunkName, [this, submeshState, &submeshNames, &submeshNaming, usingSkeleton, usingMorpher](WxDataChunkWriter& writer, WxError& error)
            {
                FINJIN_WX_ERROR_METHOD_START(error);

                //Submesh name
                auto submeshName = submeshState->name;
                if (submeshName.empty() || submeshNames.find(submeshName) != submeshNames.end())
                {
                    //Determine material name
                    wxString materialName;
                    if (submeshState->index < (int)this->alternateSubmeshMaterialNames.size())
                        materialName = this->alternateSubmeshMaterialNames[submeshState->index];
                    else
                        materialName = this->sceneExportSettings->GetMaterialName(submeshState->material);

                    //Ensure submesh name is unique
                    auto submeshBaseName = submeshNaming.GetSubmeshName(materialName);
                    int renameCount = -2;
                    do
                    {
                        if (renameCount == -2 && submeshNaming.type == SubmeshNaming::MATERIAL)
                            submeshName = submeshBaseName;
                        else if (renameCount < 0)
                            submeshName = wxString::Format(wxT("%s%d"), submeshBaseName.wx_str(), submeshState->index);
                        else if (renameCount >= 0)
                            submeshName = wxString::Format(wxT("%s%d_%d"), submeshBaseName.wx_str(), submeshState->index, renameCount);
                        renameCount++;
                    }while (submeshNames.find(submeshName) != submeshNames.end());
                }
                writer.WriteString(StandardAssetDocumentPropertyNames::NAME, submeshName, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Material ref
                if (submeshState->material.IsValid() || this->allowNoMaterialAssignment)
                {
                    auto materialRef = this->sceneExportSettings->GetAssetReference(submeshState->material);
                    writer.WriteString(StandardAssetDocumentPropertyNames::MATERIAL_REF, materialRef.ToUriString(), error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }

                //Primitive type
                switch (submeshState->submeshProperties.primitiveType)
                {
                    case PrimitiveType::TRIANGLES:
                    {
                        //Primitive type
                        writer.WriteString(StandardAssetDocumentPropertyNames::PRIMITIVE_TYPE, StandardAssetDocumentPropertyValues::PrimitiveType::TRIANGLE_LIST, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        break;
                    }
                    case PrimitiveType::LINES:
                    {
                        //Primitive type
                        writer.WriteString(StandardAssetDocumentPropertyNames::PRIMITIVE_TYPE, StandardAssetDocumentPropertyValues::PrimitiveType::LINE_LIST, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        break;
                    }
                    case PrimitiveType::POINTS:
                    {
                        writer.WriteString(StandardAssetDocumentPropertyNames::PRIMITIVE_TYPE, StandardAssetDocumentPropertyValues::PrimitiveType::POINT_LIST, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        break;
                    }
                    default: break;
                }

                //Index buffer--------------------------------
                if (!submeshState->indices.empty())
                {
                    writer.WriteChunk(StandardAssetDocumentChunkNames::INDEX_BUFFER, [this, submeshState, &submeshNames, &submeshNaming](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        //Type
                        auto use32BitIndexes = submeshState->vertexList.size() > std::numeric_limits<uint16_t>::max();
                        writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, use32BitIndexes ? StandardAssetDocumentPropertyValues::IndexBufferType::UINT32 : StandardAssetDocumentPropertyValues::IndexBufferType::UINT16, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        //Count
                        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, submeshState->indices.size(), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        //Indices
                        if (use32BitIndexes)
                        {
                            writer.WriteUInt32s(StandardAssetDocumentPropertyNames::VALUES, submeshState->indices.data(), submeshState->indices.size(), error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }
                        else
                        {
                            std::vector<uint16_t> indices;
                            submeshState->GetIndices(indices);

                            writer.WriteUInt16s(StandardAssetDocumentPropertyNames::VALUES, indices.data(), indices.size(), error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }
                    }, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }
                else if (!submeshState->indexBufferRange.empty())
                {
                    writer.WriteChunk(StandardAssetDocumentChunkNames::INDEX_BUFFER_RANGE, [this, submeshState, &submeshNames, &submeshNaming](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        //Buffer index
                        if (submeshState->indexBufferRange.bufferIndex > 0)
                        {
                            writer.WriteCount(StandardAssetDocumentPropertyNames::BUFFER, submeshState->indexBufferRange.bufferIndex, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }

                        //Start index
                        if (submeshState->indexBufferRange.startIndex > 0)
                        {
                            writer.WriteCount(StandardAssetDocumentPropertyNames::START, submeshState->indexBufferRange.startIndex, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }

                        //Element count
                        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, submeshState->indexBufferRange.elementCount, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                    }, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }
                //End of indices--------------------------------

                //Vertex buffer-----------------------------------
                if (!submeshState->vertexList.IsEmpty())
                {
                    writer.WriteChunk(StandardAssetDocumentChunkNames::VERTEX_BUFFER, [this, submeshState](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        //Channels
                        WriteVertexChannels(writer, submeshState->submeshProperties.vertexFormatName, submeshState->submeshProperties.vertexFormatElements, submeshState->vertexList, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                    }, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }
                else if (!submeshState->vertexBufferRange.empty())
                {
                    writer.WriteChunk(StandardAssetDocumentChunkNames::VERTEX_BUFFER_RANGE, [this, submeshState, &submeshNames, &submeshNaming](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        //Buffer index
                        if (submeshState->vertexBufferRange.bufferIndex > 0)
                        {
                            writer.WriteCount(StandardAssetDocumentPropertyNames::BUFFER, submeshState->vertexBufferRange.bufferIndex, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }

                        //Start index
                        if (submeshState->vertexBufferRange.startIndex > 0)
                        {
                            writer.WriteCount(StandardAssetDocumentPropertyNames::START, submeshState->vertexBufferRange.startIndex, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }

                        //Element count
                        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, submeshState->vertexBufferRange.elementCount, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                    }, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }
                //End of vertex buffer-----------------------------

                //Bone assignments---------------------------------
                if (usingSkeleton)
                {
                    submeshState->BuildVertexBoneAssignments(this->geometryState->meshSkeleton);
                    if (!submeshState->vertexBoneAssignments.empty())
                    {
                        writer.WriteChunk(StandardAssetDocumentChunkNames::VERTEX_BONE_ASSIGNMENTS, [this, submeshState](WxDataChunkWriter& writer, WxError& error)
                        {
                            FINJIN_WX_ERROR_METHOD_START(error);

                            //Count
                            writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, submeshState->vertexBoneAssignments.size(), error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                            //Vertex index
                            writer.WriteStridedUInt32s(StandardAssetDocumentPropertyNames::VERTEX, &submeshState->vertexBoneAssignments[0].vertexIndex, submeshState->vertexBoneAssignments.size(), WxDataChunkWriteStride(1, sizeof(GeometryStateSubmesh::VertexBoneAssignment)), error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                            //Bone index
                            writer.WriteStridedUInt32s(StandardAssetDocumentPropertyNames::BONE, &submeshState->vertexBoneAssignments[0].boneIndex, submeshState->vertexBoneAssignments.size(), WxDataChunkWriteStride(1, sizeof(GeometryStateSubmesh::VertexBoneAssignment)), error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                            //Weight
                            writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::WEIGHT, &submeshState->vertexBoneAssignments[0].weight, submeshState->vertexBoneAssignments.size(), WxDataChunkWriteStride(1, sizeof(GeometryStateSubmesh::VertexBoneAssignment)), error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                    }
                }
                //End of bone assignments-----------------------------
            }, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    //End of submeshes---------------

    //Skeleton
    if (usingSkeleton)
    {
        writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, this->sceneExportSettings->GetAssetName(AssetClass::SKELETON, this->exportableObject));
        writer.SetContextString(WxDataChunkWriter::ContextIndex::EMBED_OBJECT, StringUtilities::ToString(this->embedSkeleton));
        writer.SetContextString(WxDataChunkWriter::ContextIndex::LINK_TO_MAIN_OBJECT, StringUtilities::ToString(this->linkSkeletonToMesh));

        writer.WriteChunk(StandardAssetDocumentChunkNames::SKELETON, [this](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            //Name
            writer.WriteString(StandardAssetDocumentPropertyNames::NAME, writer.GetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Bones
            writer.WriteChunk(StandardAssetDocumentChunkNames::BONES, [this](WxDataChunkWriter& writer, WxError& error)
            {
                //Count
                auto boneCount = this->geometryState->meshSkeleton.bones.size();
                writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, boneCount, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Each bone
                for (size_t boneIndex = 0; boneIndex < boneCount; boneIndex++)
                {
                    auto bone = this->geometryState->meshSkeleton.bones[boneIndex];

                    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, bone->object.GetLocalName());

                    WxChunkName boneChunkName(StandardAssetDocumentChunkNames::BONE, boneIndex);
                    writer.WriteChunk(boneChunkName, [this, &bone](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        //Name
                        writer.WriteString(StandardAssetDocumentPropertyNames::NAME, writer.GetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        //Parent index
                        if (bone->parent != nullptr)
                        {
                            auto parentIndex = this->geometryState->meshSkeleton.GetBoneIndex(bone->parent);
                            writer.WriteCount(StandardAssetDocumentPropertyNames::PARENT, parentIndex, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }

                        //Child count
                        auto childCount = this->geometryState->meshSkeleton.GetChildBoneCount(bone.get());
                        if (childCount > 0)
                        {
                            writer.WriteCount(StandardAssetDocumentPropertyNames::CHILD_COUNT, childCount, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }

                        //Transform
                        ExporterUtilities::WriteTransformProperties(writer, bone->initialRelativeTransform, error);
                    }, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }

                //Animations
                if (this->exportAnimations && !geometryState->meshSkeletonAnimations.empty())
                {
                    //FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Adding skeleton animations"));

                    writer.WriteChunk(StandardAssetDocumentChunkNames::SKELETON_ANIMATIONS, [this](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        //Count the number of animations that should appear in the file with the skeleton, either directly or by reference
                        size_t referencedAnimationCount = 0;
                        for (size_t animationIndex = 0; animationIndex < geometryState->meshSkeletonAnimations.size(); animationIndex++)
                        {
                            auto& animation = *geometryState->meshSkeletonAnimations[animationIndex].get();
                            SceneExportSettings::EmbeddedAndLinkingHint embedLinkHint(animation.embedAnimation, animation.linkToMainObject);
                            if (sceneExportSettings->IsEmbeddedAssetType(AssetClass::SKELETON_ANIMATION, embedLinkHint) || animation.linkToMainObject)
                                referencedAnimationCount++;
                        }

                        //Count
                        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, referencedAnimationCount, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        //Each animation
                        size_t referencedAnimationChunkIndex = 0;
                        for (size_t animationIndex = 0; animationIndex < geometryState->meshSkeletonAnimations.size(); animationIndex++)
                        {
                            auto& animation = *geometryState->meshSkeletonAnimations[animationIndex].get();

                            writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, animation.name);
                            writer.SetContextString(WxDataChunkWriter::ContextIndex::EMBED_OBJECT, StringUtilities::ToString(animation.embedAnimation));
                            writer.SetContextString(WxDataChunkWriter::ContextIndex::LINK_TO_MAIN_OBJECT, StringUtilities::ToString(animation.linkToMainObject));

                            WxChunkName animationChunkName(StandardAssetDocumentChunkNames::SKELETON_ANIMATION, referencedAnimationChunkIndex);

                            SceneExportSettings::EmbeddedAndLinkingHint embedLinkHint(animation.embedAnimation, animation.linkToMainObject);
                            if (sceneExportSettings->IsEmbeddedAssetType(AssetClass::SKELETON_ANIMATION, embedLinkHint) || animation.linkToMainObject)
                                referencedAnimationChunkIndex++;

                            writer.WriteChunk(animationChunkName, [this, &animation](WxDataChunkWriter& writer, WxError& error)
                            {
                                FINJIN_WX_ERROR_METHOD_START(error);

                                //Name
                                writer.WriteString(StandardAssetDocumentPropertyNames::NAME, writer.GetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME), error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                //Length
                                writer.WriteTimeDuration(StandardAssetDocumentPropertyNames::LENGTH, animation.GetDuration(), error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                //Bones
                                writer.WriteChunk(StandardAssetDocumentChunkNames::BONES, [this, &animation](WxDataChunkWriter& writer, WxError& error)
                                {
                                    FINJIN_WX_ERROR_METHOD_START(error);

                                    //Count
                                    writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, animation.boneAnimations.size(), error);
                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                    //Bones
                                    for (size_t boneIndex = 0; boneIndex < animation.boneAnimations.size(); boneIndex++)
                                    {
                                        auto& boneAnimation = *animation.boneAnimations[boneIndex].get();

                                        WxChunkName boneAnimationsChunkName(StandardAssetDocumentChunkNames::BONE, boneIndex);
                                        writer.WriteChunk(boneAnimationsChunkName, [this, &boneAnimation](WxDataChunkWriter& writer, WxError& error)
                                        {
                                            FINJIN_WX_ERROR_METHOD_START(error);

                                            //Bone index
                                            auto actualBoneIndex = this->geometryState->meshSkeleton.GetBoneIndex(boneAnimation.bone);
                                            writer.WriteCount(StandardAssetDocumentPropertyNames::BONE, actualBoneIndex, error);
                                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                            //Keys
                                            writer.WriteChunk(StandardAssetDocumentChunkNames::KEYS, [this, &boneAnimation](WxDataChunkWriter& writer, WxError& error)
                                            {
                                                FINJIN_WX_ERROR_METHOD_START(error);

                                                //Count
                                                writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, boneAnimation.keys.size(), error);
                                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                                auto& firstKey = boneAnimation.keys[0];

                                                //Times
                                                writer.WriteStridedTimeDurations(StandardAssetDocumentPropertyNames::TIME, &firstKey.time, boneAnimation.keys.size(), WxDataChunkWriteStride(1, sizeof(PRSKey)), error);
                                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                                //Transforms
                                                writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::TRANSFORM, &firstKey.m44[0][0], boneAnimation.keys.size() * 16, WxDataChunkWriteStride(16, sizeof(PRSKey)), error);
                                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                            }, error);
                                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                        }, error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                    }
                                }, error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                            }, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }
                    }, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }
            }, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    auto hasTangents = this->geometryState->HasSubmeshWithVertexElement(WxGpuVertexFormatStructMetadata::ElementID::TANGENT);

    //Morph animations
    if (this->exportAnimations && !geometryState->meshMorphAnimations.empty())
    {
        //LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Adding morph animations"));

        writer.WriteChunk(StandardAssetDocumentChunkNames::MORPH_ANIMATIONS, [this, hasTangents](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            //Each animation
            for (size_t animationIndex = 0; animationIndex < geometryState->meshMorphAnimations.size(); animationIndex++)
            {
                auto& animation = *geometryState->meshMorphAnimations[animationIndex].get();

                writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, animation.name);
                writer.SetContextString(WxDataChunkWriter::ContextIndex::EMBED_OBJECT, StringUtilities::ToString(animation.embedAnimation));
                writer.SetContextString(WxDataChunkWriter::ContextIndex::LINK_TO_MAIN_OBJECT, StringUtilities::ToString(animation.linkToMainObject));

                WxChunkName animationChunkName(StandardAssetDocumentChunkNames::MORPH_ANIMATION, animationIndex);
                writer.WriteChunk(animationChunkName, [this, &animation, hasTangents](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    //Name
                    writer.WriteString(StandardAssetDocumentPropertyNames::NAME, writer.GetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME), error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    //Length
                    writer.WriteTimeDuration(StandardAssetDocumentPropertyNames::LENGTH, animation.GetDuration(), error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    //Subanimations
                    writer.WriteChunk(StandardAssetDocumentChunkNames::SUBANIMATIONS, [this, &animation, hasTangents](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        //Count
                        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, animation.subanimations.size(), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        //Each subanimation
                        for (size_t subanimIndex = 0; subanimIndex < animation.subanimations.size(); subanimIndex++)
                        {
                            auto& subanimation = *animation.subanimations[subanimIndex].get();

                            WxChunkName subanimChunkName(StandardAssetDocumentChunkNames::SUBANIMATION, subanimIndex);
                            writer.WriteChunk(subanimChunkName, [this, &animation, &subanimation, hasTangents](WxDataChunkWriter& writer, WxError& error)
                            {
                                FINJIN_WX_ERROR_METHOD_START(error);

                                //Submesh index
                                writer.WriteCount(StandardAssetDocumentPropertyNames::SUBMESH, subanimation.submesh->index, error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                //Channel count
                                size_t channelCount = 2;
                                if (hasTangents)
                                    channelCount++;

                                //Channel formats
                                writer.WriteChunk(StandardAssetDocumentChunkNames::FORMAT, [this, hasTangents, channelCount](WxDataChunkWriter& writer, WxError& error)
                                {
                                    FINJIN_WX_ERROR_METHOD_START(error);

                                    //Count
                                    writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, channelCount, error);
                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                    //Position
                                    writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::FORMAT_ELEMENT, 0), [](WxDataChunkWriter& writer, WxError& error)
                                    {
                                        FINJIN_WX_ERROR_METHOD_START(error)

                                        //ID
                                        writer.WriteString(StandardAssetDocumentPropertyNames::ID, WxGpuVertexFormatStructMetadata::ElementIDToString(WxGpuVertexFormatStructMetadata::ElementID::POSITION), error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                        //Type
                                        writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, WxNumericStructElementTypeUtilities::ToString(WxNumericStructElementType::FLOAT3), error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                    }, error);
                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                    //Normal
                                    writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::FORMAT_ELEMENT, 1), [](WxDataChunkWriter& writer, WxError& error)
                                    {
                                        FINJIN_WX_ERROR_METHOD_START(error)

                                        //ID
                                        writer.WriteString(StandardAssetDocumentPropertyNames::ID, WxGpuVertexFormatStructMetadata::ElementIDToString(WxGpuVertexFormatStructMetadata::ElementID::NORMAL), error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                        //Type
                                        writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, WxNumericStructElementTypeUtilities::ToString(WxNumericStructElementType::FLOAT3), error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                    }, error);
                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                    //Tangent
                                    if (hasTangents)
                                    {
                                        writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::FORMAT_ELEMENT, 2), [](WxDataChunkWriter& writer, WxError& error)
                                        {
                                            FINJIN_WX_ERROR_METHOD_START(error)

                                            //ID
                                            writer.WriteString(StandardAssetDocumentPropertyNames::ID, WxGpuVertexFormatStructMetadata::ElementIDToString(WxGpuVertexFormatStructMetadata::ElementID::TANGENT), error);
                                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                            //Type
                                            writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, WxNumericStructElementTypeUtilities::ToString(WxNumericStructElementType::FLOAT4), error);
                                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                        }, error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                    }
                                }, error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                //Keys
                                writer.WriteChunk(StandardAssetDocumentChunkNames::KEYS, [this, &subanimation, hasTangents, channelCount](WxDataChunkWriter& writer, WxError& error)
                                {
                                    FINJIN_WX_ERROR_METHOD_START(error);

                                    //Count
                                    writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, subanimation.keys.size(), error);
                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                    //Each key
                                    for (size_t keyIndex = 0; keyIndex < subanimation.keys.size(); keyIndex++)
                                    {
                                        auto& key = subanimation.keys[keyIndex];

                                        WxChunkName keyChunkName(StandardAssetDocumentChunkNames::KEY, keyIndex);
                                        writer.WriteChunk(keyChunkName, [this, &key, hasTangents, channelCount](WxDataChunkWriter& writer, WxError& error)
                                        {
                                            FINJIN_WX_ERROR_METHOD_START(error);

                                            //Time
                                            writer.WriteTimeDuration(StandardAssetDocumentPropertyNames::TIME, key.time, error);
                                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                            //Vertex count
                                            writer.WriteCount(StandardAssetDocumentPropertyNames::VERTEX_COUNT, key.points.size(), error);
                                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                            //Channels
                                            writer.WriteChunk(StandardAssetDocumentChunkNames::CHANNELS, [this, &key, hasTangents, channelCount](WxDataChunkWriter& writer, WxError& error)
                                            {
                                                FINJIN_WX_ERROR_METHOD_START(error);

                                                //Count
                                                writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, channelCount, error);
                                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                                auto& firstPoint = key.points[0];

                                                //Position
                                                writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::CHANNEL, 0), [this, &key, &firstPoint](WxDataChunkWriter& writer, WxError& error)
                                                {
                                                    FINJIN_WX_ERROR_METHOD_START(error);

                                                    //Values
                                                    writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VALUES, &firstPoint.position.x, key.points.size() * 3, WxDataChunkWriteStride(3, sizeof(SampledMeshPoint)), error);
                                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                                }, error);
                                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                                //Normal
                                                writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::CHANNEL, 1), [this, &key, &firstPoint](WxDataChunkWriter& writer, WxError& error)
                                                {
                                                    FINJIN_WX_ERROR_METHOD_START(error);

                                                    //Values
                                                    writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VALUES, &firstPoint.normal.x, key.points.size() * 3, WxDataChunkWriteStride(3, sizeof(SampledMeshPoint)), error);
                                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                                }, error);
                                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                                //Tangent
                                                if (hasTangents)
                                                {
                                                    writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::CHANNEL, 2), [this, &key, &firstPoint](WxDataChunkWriter& writer, WxError& error)
                                                    {
                                                        FINJIN_WX_ERROR_METHOD_START(error);

                                                        //Values
                                                        writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VALUES, &firstPoint.tangent.x, key.points.size() * 4, WxDataChunkWriteStride(4, sizeof(SampledMeshPoint)), error);
                                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                                    }, error);
                                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                                }
                                            }, error);
                                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                        }, error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                    }
                                }, error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                            }, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }
                    }, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    if (usingMorpher)
    {
        //LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Adding poses"));

        //Poses
        writer.WriteChunk(StandardAssetDocumentChunkNames::POSES, [this, hasTangents](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            //Count
            writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->geometryState->meshMorpher.morphTargets.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Each pose
            size_t morphTargetIndex = 0;
            for (auto& morphTarget : geometryState->meshMorpher.morphTargets)
            {
                WxChunkName morphTargetChunkName(StandardAssetDocumentChunkNames::POSE, morphTargetIndex++);
                writer.WriteChunk(morphTargetChunkName, [this, &morphTarget, hasTangents](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    //Subposes
                    writer.WriteChunk(StandardAssetDocumentChunkNames::SUBPOSES, [this, &morphTarget, hasTangents](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        //Count
                        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, morphTarget->subtargets.size(), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        //Each subpose
                        for (size_t subtargetIndex = 0; subtargetIndex < morphTarget->subtargets.size(); subtargetIndex++)
                        {
                            WxChunkName subposeChunkName(StandardAssetDocumentChunkNames::SUBPOSE, subtargetIndex);
                            writer.WriteChunk(subposeChunkName, [this, &morphTarget, subtargetIndex, hasTangents](WxDataChunkWriter& writer, WxError& error)
                            {
                                FINJIN_WX_ERROR_METHOD_START(error);

                                auto& subtarget = *morphTarget->subtargets[subtargetIndex].get();

                                //Submesh index
                                writer.WriteCount(StandardAssetDocumentPropertyNames::SUBMESH, subtarget.index, error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                //Channel count
                                size_t channelCount = 2;
                                if (hasTangents)
                                    channelCount++;

                                //Channel formats
                                writer.WriteChunk(StandardAssetDocumentChunkNames::FORMAT, [this, hasTangents, channelCount](WxDataChunkWriter& writer, WxError& error)
                                {
                                    FINJIN_WX_ERROR_METHOD_START(error);

                                    //Count
                                    writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, channelCount, error);
                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                    //Position
                                    writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::FORMAT_ELEMENT, 0), [](WxDataChunkWriter& writer, WxError& error)
                                    {
                                        FINJIN_WX_ERROR_METHOD_START(error)

                                        //ID
                                        writer.WriteString(StandardAssetDocumentPropertyNames::ID, WxGpuVertexFormatStructMetadata::ElementIDToString(WxGpuVertexFormatStructMetadata::ElementID::POSITION), error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                        //Type
                                        writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, WxNumericStructElementTypeUtilities::ToString(WxNumericStructElementType::FLOAT3), error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                    }, error);
                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                    //Normals
                                    writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::FORMAT_ELEMENT, 1), [](WxDataChunkWriter& writer, WxError& error)
                                    {
                                        FINJIN_WX_ERROR_METHOD_START(error)

                                        //ID
                                        writer.WriteString(StandardAssetDocumentPropertyNames::ID, WxGpuVertexFormatStructMetadata::ElementIDToString(WxGpuVertexFormatStructMetadata::ElementID::NORMAL), error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                        //Type
                                        writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, WxNumericStructElementTypeUtilities::ToString(WxNumericStructElementType::FLOAT3), error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                    }, error);
                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                    //Tangents
                                    if (hasTangents)
                                    {
                                        writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::FORMAT_ELEMENT, 2), [](WxDataChunkWriter& writer, WxError& error)
                                        {
                                            FINJIN_WX_ERROR_METHOD_START(error)

                                            //ID
                                            writer.WriteString(StandardAssetDocumentPropertyNames::ID, WxGpuVertexFormatStructMetadata::ElementIDToString(WxGpuVertexFormatStructMetadata::ElementID::TANGENT), error);
                                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                            //Type
                                            writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, WxNumericStructElementTypeUtilities::ToString(WxNumericStructElementType::FLOAT4), error);
                                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                        }, error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                    }
                                }, error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                //Channels
                                writer.WriteChunk(StandardAssetDocumentChunkNames::CHANNELS, [this, &subtarget, hasTangents, channelCount](WxDataChunkWriter& writer, WxError& error)
                                {
                                    FINJIN_WX_ERROR_METHOD_START(error);

                                    //Count
                                    writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, channelCount, error);
                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                    auto& firstOffset = subtarget.offsets[0];

                                    //Indices
                                    writer.WriteStridedCounts(StandardAssetDocumentPropertyNames::VERTEX, &firstOffset.index, subtarget.offsets.size(), WxDataChunkWriteStride(1, sizeof(MeshMorphTarget::Offset)), error);
                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                    //Position
                                    writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::CHANNEL, 0), [this, &subtarget, &firstOffset](WxDataChunkWriter& writer, WxError& error)
                                    {
                                        FINJIN_WX_ERROR_METHOD_START(error);

                                        //Values
                                        writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VALUES, &firstOffset.positionOffset.x, subtarget.offsets.size() * 3, WxDataChunkWriteStride(3, sizeof(MeshMorphTarget::Offset)), error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                    }, error);
                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                    //Normals
                                    writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::CHANNEL, 1), [this, &subtarget, &firstOffset](WxDataChunkWriter& writer, WxError& error)
                                    {
                                        FINJIN_WX_ERROR_METHOD_START(error);

                                        //Values
                                        writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VALUES, &firstOffset.normalOffset.x, subtarget.offsets.size() * 3, WxDataChunkWriteStride(3, sizeof(MeshMorphTarget::Offset)), error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                    }, error);
                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                    //Tangents
                                    if (hasTangents)
                                    {
                                        writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::CHANNEL, 2), [this, &subtarget, &firstOffset](WxDataChunkWriter& writer, WxError& error)
                                        {
                                            FINJIN_WX_ERROR_METHOD_START(error);

                                            //Values
                                            writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VALUES, &firstOffset.tangentOffset.x, subtarget.offsets.size() * 4, WxDataChunkWriteStride(4, sizeof(MeshMorphTarget::Offset)), error);
                                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                        }, error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                    }
                                }, error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                            }, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }
                    }, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Pose animations
        if (this->exportAnimations)
        {
            //LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Adding pose animations"));

            //Poses
            writer.WriteChunk(StandardAssetDocumentChunkNames::POSE_ANIMATIONS, [this](WxDataChunkWriter& writer, WxError& error)
            {
                FINJIN_WX_ERROR_METHOD_START(error);

                //Count
                writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, geometryState->meshMorpherAnimations.size(), error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Each animation
                for (size_t animationIndex = 0; animationIndex < geometryState->meshMorpherAnimations.size(); animationIndex++)
                {
                    auto& animation = *geometryState->meshMorpherAnimations[animationIndex].get();

                    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, animation.name);
                    writer.SetContextString(WxDataChunkWriter::ContextIndex::EMBED_OBJECT, StringUtilities::ToString(animation.embedAnimation));
                    writer.SetContextString(WxDataChunkWriter::ContextIndex::LINK_TO_MAIN_OBJECT, StringUtilities::ToString(animation.linkToMainObject));

                    writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::POSE_ANIMATION, animationIndex), [this, &animation](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        //Name
                        writer.WriteString(StandardAssetDocumentPropertyNames::NAME, writer.GetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        //Length
                        writer.WriteTimeDuration(StandardAssetDocumentPropertyNames::LENGTH, animation.GetDuration(), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        //Subanimations
                        writer.WriteChunk(StandardAssetDocumentChunkNames::SUBANIMATIONS, [this, &animation](WxDataChunkWriter& writer, WxError& error)
                        {
                            FINJIN_WX_ERROR_METHOD_START(error);

                            //Count
                            writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, animation.subanimations.size(), error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                            //Each subpose
                            for (size_t subanimIndex = 0; subanimIndex < animation.subanimations.size(); subanimIndex++)
                            {
                                auto& subanimation = *animation.subanimations[subanimIndex].get();

                                WxChunkName subposeChunkName(StandardAssetDocumentChunkNames::SUBANIMATION, subanimIndex);
                                writer.WriteChunk(subposeChunkName, [this, &animation, &subanimation](WxDataChunkWriter& writer, WxError& error)
                                {
                                    FINJIN_WX_ERROR_METHOD_START(error);

                                    //Submesh index
                                    writer.WriteCount(StandardAssetDocumentPropertyNames::SUBMESH, subanimation.submesh->index, error);
                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                    //Keys
                                    writer.WriteChunk(StandardAssetDocumentChunkNames::KEYS, [this, &subanimation](WxDataChunkWriter& writer, WxError& error)
                                    {
                                        FINJIN_WX_ERROR_METHOD_START(error);

                                        //Count
                                        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, subanimation.keys.size(), error);
                                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                        //Each key
                                        for (size_t keyIndex = 0; keyIndex < subanimation.keys.size(); keyIndex++)
                                        {
                                            auto& key = subanimation.keys[keyIndex];

                                            WxChunkName keyChunkName(StandardAssetDocumentChunkNames::KEY, keyIndex);
                                            writer.WriteChunk(keyChunkName, [this, &key](WxDataChunkWriter& writer, WxError& error)
                                            {
                                                FINJIN_WX_ERROR_METHOD_START(error);

                                                //Time
                                                writer.WriteTimeDuration(StandardAssetDocumentPropertyNames::TIME, key.time, error);
                                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                                //Count
                                                writer.WriteCount(StandardAssetDocumentPropertyNames::INFLUENCE_COUNT, key.influences.size(), error);
                                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                                if (!key.influences.empty())
                                                {
                                                    //Influences
                                                    writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::INFLUENCE, &key.influences[0].influence, key.influences.size(), WxDataChunkWriteStride(1, sizeof(MeshMorpher::MorphSubtargetInfluence)), error);
                                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                                    //Targets
                                                    std::vector<size_t> subtargets;
                                                    subtargets.resize(key.influences.size());
                                                    for (size_t influenceIndex = 0; influenceIndex < key.influences.size(); influenceIndex++)
                                                        subtargets[influenceIndex] = key.influences[influenceIndex].subtarget->index;
                                                    writer.WriteCounts(StandardAssetDocumentPropertyNames::SUBMESH, subtargets.data(), subtargets.size(), error);
                                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                                }
                                            }, error);
                                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                        }
                                    }, error);
                                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                                }, error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                            }
                        }, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                    }, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }
            }, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }

    //Manual level of detail meshes
    if (!this->exportableObject->objectExportSettings.manualLods.empty())
    {
        writer.WriteChunk(StandardAssetDocumentChunkNames::MANUAL_LODS, [this](WxDataChunkWriter& writer, WxError& error)
        {
            //Count
            writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->exportableObject->objectExportSettings.manualLods.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Each manual LOD
            FINJIN_WX_ERROR_METHOD_START(error);
            for (size_t manualLodIndex = 0; manualLodIndex < this->exportableObject->objectExportSettings.manualLods.size(); manualLodIndex++)
            {
                auto& manualLod = this->exportableObject->objectExportSettings.manualLods[manualLodIndex];
                auto& manualLodObjectSettings = this->exportableObject->objectExportSettings.manualLodsObjectSettings[manualLodIndex];

                WxChunkName manualLodChunkName(StandardAssetDocumentChunkNames::MANUAL_LOD, manualLodIndex);
                writer.WriteChunk(manualLodChunkName, [this, &manualLod, &manualLodObjectSettings](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    //Asset reference
                    writer.WriteString(StandardAssetDocumentPropertyNames::MESH_REF, this->sceneExportSettings->GetAssetReference(AssetClass::MESH, manualLod.object, manualLodObjectSettings).ToUriString(), error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    //Distance
                    writer.WriteFloat(StandardAssetDocumentPropertyNames::DISTANCE, manualLod.distance, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}
