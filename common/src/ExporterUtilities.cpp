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
#include "ExporterUtilities.hpp"
#include "GeometryState.hpp"
#include "MathUtilities.hpp"
#include "StringUtilities.hpp"
#include "FinjinSceneDocument.hpp"
#include "FileUtilities.hpp"
#include "PropertyValues.hpp"
#include "EnvironmentFar.hpp"
#include "MacroExpander.hpp"
#include "Logger.hpp"
#include "AssetClass.hpp"
#include "WrappedFileWriter.hpp"
#include "Strings.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
bool ExporterUtilities::ConvertCopyImage(const wxString& sourceFileName, WxDataChunkWriter& writer, const WxChunkPropertyName& sizePropertyName, const WxChunkPropertyName& contentPropertyName, bool convertToTexture)
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("ConvertCopyImage(%s)"), sourceFileName.wx_str());

    FINJIN_WX_DECLARE_ERROR(error);

    WxByteBuffer content;
    if (convertToTexture)
    {
        WrappedFileWriter::Wrap(sourceFileName, writer.GetByteOrder(), content, error);
        if (error)
            return false;
    }
    else
    {
        if (!FileUtilities::ReadBinaryFile(sourceFileName, content))
            return false;
    }

    writer.WriteCount(sizePropertyName, content.size(), error);
    if (error)
        return false;

    writer.WriteBlob(contentPropertyName, content.data(), content.size(), error);
    if (error)
        return false;

    return true;
}

bool ExporterUtilities::ConvertCopyImage(const wxString& sourceFileName, ByteOrder byteOrder, const wxString& destinationFileName, bool convertToTexture)
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("ConvertCopyImage(%s, %s)"), sourceFileName.wx_str(), destinationFileName.wx_str());

    auto result = false;

    if (!convertToTexture && sourceFileName == destinationFileName)
    {
        //Source and destination are the same
        result = true;
    }
    else
    {
        if (convertToTexture)
        {
            FINJIN_WX_DECLARE_ERROR(error);
            WrappedFileWriter::Wrap(sourceFileName, byteOrder, destinationFileName, error);
            if (error)
            {
                FINJIN_EXPORTER_LOG_MESSAGE
                    (
                    ERROR_LOG_MESSAGE,
                    Strings::COULD_NOT_COPY_IMAGE_FILE_ERROR_DESTINATION_PERMISSIONS_FORMAT,
                    sourceFileName.wx_str(),
                    destinationFileName.wx_str()
                    );
            }
        }
        else
        {
            if (FileUtilities::EnsureFileReplaceable(destinationFileName))
                result = wxCopyFile(sourceFileName, destinationFileName);
            else
            {
                FINJIN_EXPORTER_LOG_MESSAGE
                    (
                    ERROR_LOG_MESSAGE,
                    Strings::COULD_NOT_COPY_IMAGE_FILE_ERROR_DESTINATION_PERMISSIONS_FORMAT,
                    sourceFileName.wx_str(),
                    destinationFileName.wx_str()
                    );
            }
        }
    }

    return result;
}

wxString ExporterUtilities::GetMaterialName(MaterialAccessor material, const wxString& defaultName, bool includeParentMaterialName)
{
    MaterialAccessor parentMaterial = material.GetParentMaterial();

    wxString parentMaterialName;
    if (includeParentMaterialName && parentMaterial.IsValid())
    {
        parentMaterialName = parentMaterial.GetLocalName();
        if (!parentMaterialName.empty())
        {
            FixObjectName(parentMaterialName);
            parentMaterialName += wxT("-");
        }
    }

    wxString materialName;
    if (material.IsValid())
    {
        auto materialLocalName = material.GetLocalName();
        FixObjectName(materialLocalName);
        materialName = parentMaterialName + materialLocalName;
    }
    else
        materialName = defaultName;

    return materialName;
}

void ExporterUtilities::FixObjectName(wxString& fileName)
{
    fileName.Replace(wxT(" "), wxEmptyString);
    fileName.Replace(wxT(":"), wxEmptyString);
    fileName.Replace(wxT("["), wxEmptyString);
    fileName.Replace(wxT("]"), wxEmptyString);
    fileName.Replace(wxT("*"), wxEmptyString);
    fileName.Replace(wxT("#"), wxEmptyString);
    fileName.Replace(wxT("?"), wxEmptyString);
    fileName.Replace(wxT("\""), wxEmptyString);
    fileName.Replace(wxT("'"), wxEmptyString);
    fileName.Replace(wxT("<"), wxEmptyString);
    fileName.Replace(wxT(">"), wxEmptyString);
    fileName.Replace(wxT("|"), wxEmptyString);

    fileName.Replace(wxT("\\"), wxT("/"));
    fileName.Replace(wxT("!"), wxT("/"));
    FileUtilities::RemoveLeadingSeparators(fileName);
}

wxString ExporterUtilities::GetTextureName(const wxString& relativeFilePath)
{
    auto copy = relativeFilePath;
    FileUtilities::UnifySeparators(copy);
    return FileUtilities::RemoveExtension(copy);
}

bool ExporterUtilities::GetPlane(FinjinVector4& plane, GeometryState& geomState)
{
    auto result = false;

    for (auto submesh : geomState.submeshes)
    {
        if (submesh->geometryType == GeometryStateSubmesh::MESH && submesh->indices.size() >= 3)
        {
            auto& vertexA = submesh->vertexList[submesh->indices[0]];
            auto& vertexB = submesh->vertexList[submesh->indices[1]];
            auto& vertexC = submesh->vertexList[submesh->indices[2]];

            plane = MathUtilities::CreatePlane(vertexA.position, vertexB.position, vertexC.position);
            result = true;

            break;
        }
    }

    return result;
}

std::unique_ptr<FinjinSceneDocument_Clipping> ExporterUtilities::ExportEnvironmentRange
    (
    FinjinSceneDocument& scene,
    FinjinSceneSettingsAccessor sceneSettings,
    SceneExportSettings& sceneExportSettings
    )
{
    std::unique_ptr<FinjinSceneDocument_Clipping> clipping;

    float environmentNear = sceneSettings.GetEnvironmentNear() * sceneExportSettings.scale;

    EnvironmentFar environmentFar(sceneSettings.GetEnvironmentFarObject(), sceneSettings.GetEnvironmentFar());
    environmentFar.value *= sceneExportSettings.scale;

    if (environmentNear < environmentFar.value && environmentFar.value > 0)
        clipping.reset(new FinjinSceneDocument_Clipping(&scene, environmentNear, environmentFar.value));

    return clipping;
}

std::unique_ptr<FinjinSceneDocument_Shadows> ExporterUtilities::ExportShadows
    (
    FinjinSceneDocument& scene,
    FinjinSceneSettingsAccessor sceneSettings,
    SceneExportSettings& sceneExportSettings
    )
{
    std::unique_ptr<FinjinSceneDocument_Shadows> shadows;

    wxString shadowTechnique = sceneSettings.GetShadowTechnique();
    if (!shadowTechnique.empty())
    {
        shadows.reset(new FinjinSceneDocument_Shadows(&scene));

        shadows->technique = shadowTechnique;
        shadows->farDistance = sceneSettings.GetShadowFarDistance() * sceneExportSettings.scale;
        shadows->color = sceneSettings.GetShadowColor();
    }

    return shadows;
}

FinjinSceneDocument_SceneNode::NodeVisibility ExporterUtilities::GetNodeVisibility
    (
    const wxString& visibility,
    FinjinSceneDocument_SceneNode::NodeVisibility defaultValue
    )
{
    FinjinSceneDocument_SceneNode::NodeVisibility value = defaultValue;

    if (visibility == PropertyValues::Visibility::VISIBLE)
        value = FinjinSceneDocument_SceneNode::NODE_VISIBLE;
    else if (visibility == PropertyValues::Visibility::HIDDEN)
        value = FinjinSceneDocument_SceneNode::NODE_HIDDEN;
    else if (visibility == PropertyValues::Visibility::TREE_VISIBLE)
        value = FinjinSceneDocument_SceneNode::NODE_TREE_VISIBLE;
    else if (visibility == PropertyValues::Visibility::TREE_HIDDEN)
        value = FinjinSceneDocument_SceneNode::NODE_TREE_HIDDEN;

    return value;
}

void ExporterUtilities::WriteTransformProperties(WxDataChunkWriter& writer, const TransformAccessor& transform, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    MatrixAccessor transformMatrix = transform.GetMatrix();
    float m44[4][4];
    transformMatrix.GetExportMatrix44(m44);
    writer.WriteFloats(StandardAssetDocumentPropertyNames::TRANSFORM, &m44[0][0], 16, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}
