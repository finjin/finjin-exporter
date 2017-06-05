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
#include "SceneExportSettings.hpp"
#include "StringUtilities.hpp"
#include "FileUtilities.hpp"
#include "MathUtilities.hpp"
#include "PropertyValues.hpp"
#include "ApplicationAccessor.hpp"
#include "ExporterUtilities.hpp"
#include "MacroExpander.hpp"
#include "finjin/common/WxStreamingFileFormat.hpp"
#include "AssetClass.hpp"
#include "FinjinGlobalSettings.hpp"
#include "Strings.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"
#include "finjin/common/WxDataChunkWriterController.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------

//SceneExportSettings::EmbeddedAndLinkingHint
SceneExportSettings::EmbeddedAndLinkingHint::EmbeddedAndLinkingHint() : isEmbedded(false), linkToMainObject(false)
{
}

SceneExportSettings::EmbeddedAndLinkingHint::EmbeddedAndLinkingHint(const wxString& isEmbedded, const wxString& linkToMainObject) : isEmbedded(false), linkToMainObject(false)
{
    if (!isEmbedded.empty())
        this->isEmbedded = StringUtilities::ToBool(isEmbedded);

    if (!linkToMainObject.empty())
        this->linkToMainObject = StringUtilities::ToBool(linkToMainObject);
}

SceneExportSettings::EmbeddedAndLinkingHint::EmbeddedAndLinkingHint(bool isEmbedded, bool linkToMainObject)
{
    this->isEmbedded = isEmbedded;
    this->linkToMainObject = linkToMainObject;
}

//SceneExportSettings
SceneExportSettings::SceneExportSettings()
{
    this->currentFormatData = nullptr;
    this->time = TimeAccessor();
    this->conversionManager.Initialize(UpAxis::Y);
    this->scale = 1.0f;
    this->scaledWorldUnitsPerMeter = 1.0f;
    this->copyBitmaps = false;
    this->convertBitmapsToTextures = false;
    this->includeParentMaterialInName = false;

    this->byteOrder = ByteOrder::NATIVE;
}

void SceneExportSettings::Initialize(FinjinSceneSettingsAccessor sceneSettings, SceneExportSettingsFlags flags)
{
    wxString value; //Temporary value

    if (AnySet(flags & SceneExportSettingsFlags::USE_CURRENT_TIME))
        this->time = TimeAccessor::GetCurrentTime();
    else
        this->time = ApplicationAccessor::GetAnimationRange().start;

    this->scale = sceneSettings.GetSceneScaleValue();
    this->scaledWorldUnitsPerMeter = this->scale / ApplicationAccessor::GetMasterScaleMeters();

    this->noMaterialName = sceneSettings.GetNoMaterialName();
    if (this->noMaterialName.empty())
        this->noMaterialName = Strings::NO_MATERIAL_NAME;

    auto& globalSettings = FinjinGlobalSettings::GetInstance();

    this->byteOrder = globalSettings.binaryByteOrder;
    if (this->byteOrder == NATIVE)
        this->byteOrder = ::GetByteOrder();

    this->copyBitmaps = globalSettings.copyBitmaps;
    this->convertBitmapsToTextures = globalSettings.convertBitmapsToTextures;

    if (globalSettings.embedMeshes)
        this->embeddedAssetTypes.push_back(AssetClass::MESH);
    if (globalSettings.embedMaterials)
        this->embeddedAssetTypes.push_back(AssetClass::MATERIAL);
    if (globalSettings.embedTextures)
        this->embeddedAssetTypes.push_back(AssetClass::TEXTURE);
    if (globalSettings.embedPrefabs)
        this->embeddedAssetTypes.push_back(AssetClass::PREFAB);

    this->includeParentMaterialInName = sceneSettings.GetIncludeParentMaterialName();
}

void SceneExportSettings::MakeExportPaths(AssetClass defaultAssetType, const wxString& exportFilePath)
{
    this->defaultAssetType = defaultAssetType;
    auto defaultExportDirectoryPath = FileUtilities::GetDirectoryPath(exportFilePath);
    auto defaultExtNoDot = FileUtilities::GetExtension(exportFilePath, false);
    auto defaultFileFormat = WxStreamingFileFormatUtilities::Parse(defaultExtNoDot);

    auto& globalSettings = FinjinGlobalSettings::GetInstance();

    StaticVector<WxStreamingFileFormat, (size_t)WxStreamingFileFormat::COUNT> fileFormats;
    fileFormats.push_back(defaultFileFormat);
    if (globalSettings.additionalExportBinaryFiles && defaultFileFormat != WxStreamingFileFormat::STREAMING_BINARY)
        fileFormats.push_back(WxStreamingFileFormat::STREAMING_BINARY);
    if (globalSettings.additionalExportTextFiles && defaultFileFormat != WxStreamingFileFormat::STREAMING_TEXT)
        fileFormats.push_back(WxStreamingFileFormat::STREAMING_TEXT);
    if (globalSettings.additionalExportConfigFiles && defaultFileFormat != WxStreamingFileFormat::STREAMING_CONFIG)
        fileFormats.push_back(WxStreamingFileFormat::STREAMING_CONFIG);
    if (globalSettings.additionalExportJsonFiles && defaultFileFormat != WxStreamingFileFormat::STREAMING_JSON)
        fileFormats.push_back(WxStreamingFileFormat::STREAMING_JSON);

    MacroExpander pathExpander;
    auto globalExportDirectory = globalSettings.exportDirectory;
    pathExpander.Expand(globalExportDirectory);

    for (auto fileFormat : fileFormats)
    {
        FormatData formatData;
        formatData.fileFormat = fileFormat;

        for (size_t i = 0; i < (size_t)AssetClass::COUNT; i++)
        {
            if (fileFormat == defaultFileFormat)
            {
                //The default file format files go at the top level
                if ((AssetClass)i == defaultAssetType)
                    formatData.assetDirectories[i] = defaultExportDirectoryPath;
                else
                {
                    formatData.assetDirectories[i] = FileUtilities::JoinPath(defaultExportDirectoryPath, globalExportDirectory);
                    formatData.assetDirectories[i] = FileUtilities::JoinPath(formatData.assetDirectories[i], AssetClassUtilities::ToString(i, true));
                }
            }
            else
            {
                //Non-default formats go into additional subdirectories
                formatData.assetDirectories[i] = FileUtilities::JoinPath(defaultExportDirectoryPath, globalExportDirectory);
                formatData.assetDirectories[i] = FileUtilities::JoinPath(formatData.assetDirectories[i], wxT("additional-formats"));
                formatData.assetDirectories[i] = FileUtilities::JoinPath(formatData.assetDirectories[i], WxStreamingFileFormatUtilities::ToString(fileFormat));
                formatData.assetDirectories[i] = FileUtilities::JoinPath(formatData.assetDirectories[i], AssetClassUtilities::ToString(i, true));
            }
        }

        this->fileFormats.push_back(formatData);
    }

    this->currentFormatData = &this->fileFormats[0];
}

bool SceneExportSettings::HasCurrentFileFormat() const
{
    return this->currentFormatData != nullptr;
}

void SceneExportSettings::NextFileFormat()
{
    if (this->currentFormatData != nullptr)
    {
        auto index = this->currentFormatData - this->fileFormats.begin();
        index++;
        if (index < this->fileFormats.size())
            this->currentFormatData = &this->fileFormats[index];
        else
            this->currentFormatData = nullptr;
    }
}

WxStreamingFileFormat SceneExportSettings::GetCurrentFileFormat() const
{
    if (this->currentFormatData != nullptr)
        return this->currentFormatData->fileFormat;
    else
        return WxStreamingFileFormat::STREAMING_TEXT;
}

wxString SceneExportSettings::GetCurrentAssetDirectory(AssetClass assetType) const
{
    wxString result;
    if (this->currentFormatData != nullptr)
    {
        result = this->currentFormatData->assetDirectories[assetType];
        if (!IsEmbeddedAssetType(assetType))
            FileUtilities::CreateDirectoryRecursive(result);
    }
    return result;
}

wxString SceneExportSettings::GetMaterialName(MaterialAccessor material) const
{
    return ExporterUtilities::GetMaterialName(material, this->noMaterialName, this->includeParentMaterialInName);
}

wxString SceneExportSettings::GetAssetName(ExportableObject* exportableObject, GeometryStateBase* geometryState) const
{
    wxString objectName;

    objectName = geometryState->meshName;

    if (objectName.empty())
        objectName = geometryState->createObject.GetLocalName(true);

    if (objectName.empty())
        objectName = exportableObject->object.GetLocalName(true);

    ExporterUtilities::FixObjectName(objectName);
    return objectName;
}

wxString SceneExportSettings::GetAssetName(AssetClass assetType, ExportableObject* exportableObject) const
{
    return GetAssetName(assetType, exportableObject->object, exportableObject->objectSettings);
}

wxString SceneExportSettings::GetAssetName(AssetClass assetType, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings) const
{
    wxString objectName;

    if (assetType == AssetClass::MESH)
        objectName = objectSettings.GetMeshName();
    else if (assetType == AssetClass::SKELETON)
        objectName = objectSettings.GetSkeletonName();

    if (objectName.empty())
        objectName = object.GetLocalName(true);

    ExporterUtilities::FixObjectName(objectName);
    return objectName;
}

bool SceneExportSettings::IsEmbeddedAssetType(AssetClass assetType, EmbeddedAndLinkingHint hint) const
{
    return (hint.isEmbedded.IsSet() && hint.isEmbedded) || this->embeddedAssetTypes.contains(assetType);
}

AssetReference SceneExportSettings::GetAssetReference(MaterialAccessor material, EmbeddedAndLinkingHint hint) const
{
    auto name = GetMaterialName(material);
    return GetAssetReference(AssetClass::MATERIAL, name, hint);
}

AssetReference SceneExportSettings::GetAssetReference(ExportableObject* exportableObject, GeometryStateBase* geometryState, EmbeddedAndLinkingHint hint) const
{
    auto objectName = GetAssetName(exportableObject, geometryState);
    return GetAssetReference(AssetClass::MESH, objectName, hint);
}

AssetReference SceneExportSettings::GetAssetReference(AssetClass assetType, ExportableObject* exportableObject, EmbeddedAndLinkingHint hint) const
{
    auto objectName = GetAssetName(assetType, exportableObject);
    return GetAssetReference(assetType, objectName, hint);
}

AssetReference SceneExportSettings::GetAssetReference(AssetClass assetType, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, EmbeddedAndLinkingHint hint) const
{
    auto objectName = GetAssetName(assetType, object, objectSettings);
    return GetAssetReference(assetType, objectName, hint);
}

AssetReference SceneExportSettings::GetAssetReference(AssetClass assetType, const wxString& objectName, EmbeddedAndLinkingHint hint) const
{
    if (IsEmbeddedAssetType(assetType, hint))
        return AssetReference::ForObject(objectName);
    else
    {
        auto assetFilePaths = GetAssetFilePaths(assetType, objectName);
        FileUtilities::CreateFileDirectoryRecursive(assetFilePaths.absolutePath);
        return AssetReference::ForObjectInLocalFile(assetFilePaths.relativePath, objectName);
    }
}

AssetReference SceneExportSettings::GetAssetFileReference(AssetClass assetType, const AssetPaths& fileName, EmbeddedAndLinkingHint hint) const
{
    auto objectName = fileName.relativePath;
    FileUtilities::UnifySeparators(objectName);

    if (IsEmbeddedAssetType(assetType, hint))
    {
        if (assetType == AssetClass::TEXTURE)
            return AssetReference::ForObject(ExporterUtilities::GetTextureName(objectName));
        else
            return AssetReference::ForObject(objectName);
    }
    else
    {
        FileUtilities::CreateFileDirectoryRecursive(fileName.absolutePath);
        if (assetType == AssetClass::TEXTURE)
            return AssetReference::ForObjectInLocalFile(fileName.relativePath, ExporterUtilities::GetTextureName(objectName));
        else
            return AssetReference::ForObjectInLocalFile(fileName.relativePath, objectName);
    }
}

void SceneExportSettings::AppendAssetExtension(wxString& s, AssetClass assetType) const
{
    s += wxT(".");
    s += WxStreamingFileFormatUtilities::MakeExtension(AssetClassUtilities::ToString(assetType), GetCurrentFileFormat());
}

AssetPaths SceneExportSettings::GetAssetFilePaths(AssetClass assetType, const wxString& objectName, EmbeddedAndLinkingHint hint) const
{
    AssetPaths result;

    result.relativePath = objectName;
    AppendAssetExtension(result.relativePath, assetType);

    result.absolutePath = GetCurrentAssetDirectory(assetType);
    result.absolutePath = FileUtilities::JoinPath(result.absolutePath, result.relativePath);

    if (!IsEmbeddedAssetType(assetType, hint))
        FileUtilities::CreateFileDirectoryRecursive(result.absolutePath);

    return result;
}

AssetPaths SceneExportSettings::ResolveAssetPaths(AssetClass assetType, const wxString& fileName, EmbeddedAndLinkingHint hint) const
{
    AssetPaths result;

    auto fileNameNoExtension = FileUtilities::GetFileNameNoExtension(fileName);
    auto ext = FileUtilities::GetExtension(fileName, true);

    auto objectName = fileNameNoExtension;
    ExporterUtilities::FixObjectName(objectName);

    auto projectPath = ApplicationAccessor::GetCurrentProjectDirectory();
    if (fileName.StartsWith(projectPath))
    {
        auto fileDirectory = FileUtilities::RemoveFileName(fileName);

        wxString tempRelativePath = fileDirectory.substr(projectPath.length());
        FileUtilities::UnifySeparators(tempRelativePath);
        FileUtilities::RemoveLeadingSeparators(tempRelativePath);

        result.relativePath = FileUtilities::JoinPath(tempRelativePath, objectName);
    }
    else
    {
        result.relativePath = objectName;
    }

    if (assetType == AssetClass::TEXTURE && this->convertBitmapsToTextures)
    {
        result.relativePath += wxT(".");
        result.relativePath += Strings::TEXTURE_FILE_EXTENSION;
    }
    else
        result.relativePath += ext;

    result.absolutePath = GetCurrentAssetDirectory(assetType);
    result.absolutePath = FileUtilities::JoinPath(result.absolutePath, result.relativePath);

    if (!IsEmbeddedAssetType(assetType, hint))
        FileUtilities::CreateFileDirectoryRecursive(result.absolutePath);

    return result;
}

bool SceneExportSettings::ChunkNameToAssetType(const WxChunkName& chunkName, AssetClass& assetType)
{
    if (chunkName == StandardAssetDocumentChunkNames::MATERIAL)
    {
        assetType = AssetClass::MATERIAL;
        return true;
    }
    else if (chunkName == StandardAssetDocumentChunkNames::MESH)
    {
        assetType = AssetClass::MESH;
        return true;
    }
    else if (chunkName == StandardAssetDocumentChunkNames::SKELETON)
    {
        assetType = AssetClass::SKELETON;
        return true;
    }
    else if (chunkName == StandardAssetDocumentChunkNames::PREFAB)
    {
        assetType = AssetClass::PREFAB;
        return true;
    }
    else if (chunkName == StandardAssetDocumentChunkNames::NODE_ANIMATION)
    {
        assetType = AssetClass::NODE_ANIMATION;
        return true;
    }
    else if (chunkName == StandardAssetDocumentChunkNames::SKELETON_ANIMATION)
    {
        assetType = AssetClass::SKELETON_ANIMATION;
        return true;
    }
    else if (chunkName == StandardAssetDocumentChunkNames::MORPH_ANIMATION)
    {
        assetType = AssetClass::MORPH_ANIMATION;
        return true;
    }
    else if (chunkName == StandardAssetDocumentChunkNames::POSE_ANIMATION)
    {
        assetType = AssetClass::POSE_ANIMATION;
        return true;
    }

    return false;
}

WxChunkPropertyName SceneExportSettings::AssetTypeToAssetReferencePropertyName(AssetClass assetType)
{
    WxChunkPropertyName propertyName;

    switch (assetType)
    {
        case AssetClass::MATERIAL:
        {
            propertyName = StandardAssetDocumentPropertyNames::MATERIAL_REF;
            break;
        }
        case AssetClass::MESH:
        {
            propertyName = StandardAssetDocumentPropertyNames::MESH_REF;
            break;
        }
        case AssetClass::SKELETON:
        {
            propertyName = StandardAssetDocumentPropertyNames::SKELETON_REF;
            break;
        }
        case AssetClass::PREFAB:
        {
            propertyName = StandardAssetDocumentPropertyNames::PREFAB_REF;
            break;
        }
        case AssetClass::NODE_ANIMATION:
        {
            propertyName = StandardAssetDocumentPropertyNames::NODE_ANIMATION_REF;
            break;
        }
        case AssetClass::SKELETON_ANIMATION:
        {
            propertyName = StandardAssetDocumentPropertyNames::SKELETON_ANIMATION_REF;
            break;
        }
        case AssetClass::MORPH_ANIMATION:
        {
            propertyName = StandardAssetDocumentPropertyNames::MORPH_ANIMATION_REF;
            break;
        }
        case AssetClass::POSE_ANIMATION:
        {
            propertyName = StandardAssetDocumentPropertyNames::POSE_ANIMATION_REF;
            break;
        }
        default: break;
    }

    return propertyName;
}

//SceneDataChunkWriterController
SceneDataChunkWriterController::SceneDataChunkWriterController(SceneExportSettings* sceneExportSettings)
{
    this->objectIndex = 0;

    this->sceneExportSettings = sceneExportSettings;
}

bool SceneDataChunkWriterController::RequiresNewOutput(const WxDataChunkWriter& writer, const WxChunkName& chunkName) const
{
    AssetClass assetType;
    if (SceneExportSettings::ChunkNameToAssetType(chunkName, assetType))
    {
        SceneExportSettings::EmbeddedAndLinkingHint embedLinkHint(writer.GetContextString(WxDataChunkWriter::ContextIndex::EMBED_OBJECT), writer.GetContextString(WxDataChunkWriter::ContextIndex::LINK_TO_MAIN_OBJECT));
        return !this->sceneExportSettings->IsEmbeddedAssetType(assetType, embedLinkHint);
    }
    else
        return false;
}

std::unique_ptr<WxDocumentWriterOutput> SceneDataChunkWriterController::AddOutput(WxDataChunkWriter& writer, const WxChunkName& chunkName, WxError& error)
{
    AssetClass assetType;
    SceneExportSettings::ChunkNameToAssetType(chunkName, assetType);

    //Determine object name
    auto objectName = writer.GetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME);
    if (objectName.empty())
    {
        //This shouldn't happen
        objectName = AssetClassUtilities::ToString(assetType) + wxString::Format(wxT("%d"), this->objectIndex++);
        FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, Strings::ENCOUNTERED_CHUNK_WITHOUT_OBJECT_NAME_FORMAT, objectName.wx_str());
    }

    auto filePaths = this->sceneExportSettings->GetAssetFilePaths(assetType, objectName);
    auto outputStream = CreateOutputFileStream(filePaths.absolutePath, error);
    if (error)
    {
        FINJIN_WX_SET_ERROR_NO_MESSAGE(error);
        return nullptr;
    }

    SceneExportSettings::EmbeddedAndLinkingHint embedLinkHint(writer.GetContextString(WxDataChunkWriter::ContextIndex::EMBED_OBJECT), writer.GetContextString(WxDataChunkWriter::ContextIndex::LINK_TO_MAIN_OBJECT));
    if (!embedLinkHint.linkToMainObject.IsSet() || embedLinkHint.linkToMainObject)
    {
        if (chunkName.index != (WxChunkName::Index)-1)
        {
            //It's an indxed chunk. Write the index, then write the asset reference within the chunk
            //Don't use 'chunkName' directly (just use 'chunkName.index') since doing such a thing would cause an infinite recursion/crash
            writer.WriteChunk(chunkName.index, [this, assetType, objectName](WxDataChunkWriter& writer, WxError& error)
            {
                FINJIN_WX_ERROR_METHOD_START(error);

                auto propertyName = SceneExportSettings::AssetTypeToAssetReferencePropertyName(assetType);
                auto assetRef = this->sceneExportSettings->GetAssetReference(assetType, objectName);
                writer.WriteString(propertyName, assetRef.ToUriString(), error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }, error);
            if (error)
            {
                FINJIN_WX_SET_ERROR_NO_MESSAGE(error);
                return nullptr;
            }
        }
        else
        {
            //Chunk isn't indexed. Just write the asset reference
            auto propertyName = SceneExportSettings::AssetTypeToAssetReferencePropertyName(assetType);
            auto assetRef = this->sceneExportSettings->GetAssetReference(assetType, objectName);
            writer.WriteString(propertyName, assetRef.ToUriString(), error);
            if (error)
            {
                FINJIN_WX_SET_ERROR_NO_MESSAGE(error);
                return nullptr;
            }
        }
    }

    return outputStream;
}
