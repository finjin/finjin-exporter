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
#include "FinjinGlobalSettings.hpp"
#include "ApplicationAccessor.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "StringUtilities.hpp"
#include "FileUtilities.hpp"
#include "MacroExpander.hpp"
#include "finjin/common/WxConfigDocumentWriter.hpp"
#include "finjin/common/WxConfigDocumentReader.hpp"

using namespace Finjin::Exporter;


//Local functions---------------------------------------------------------------
static wxString GetConfigSettingsFileName()
{
    return FileUtilities::JoinPath(ApplicationAccessor::GetFinjinHomeDirectory(true), FINJIN_EXPORTER_GLOBAL_SETTINGS_FILE_NAME);
}


//Static initialization---------------------------------------------------------
const double FinjinGlobalSettings::DEFAULT_SAMPLE_INTERVAL = .25;
static FinjinGlobalSettings* instance = nullptr;


//Implementation----------------------------------------------------------------
FinjinGlobalSettings::FinjinGlobalSettings()
{
    if (instance == nullptr)
        instance = this;

    this->loaded = false;
    this->detailedLogging = false;
    this->blobTextFormat = WxDataChunkBlobTextFormat::BYTE_ARRAY;
    this->binaryByteOrder = ByteOrder::NATIVE;
    this->additionalExportJsonFiles = false;
    this->additionalExportTextFiles = false;
    this->additionalExportBinaryFiles = false;
    this->additionalExportConfigFiles = false;
    this->embedMeshes = false;
    this->embedMaterials = false;
    this->embedTextures = false;
    this->embedPrefabs = false;
    this->exportAuthor = false;
    this->exportApplicationName = false;
    this->exportExporterVersion = false;
    this->exportExportTime = false;
    this->maxBytesPerLine = FINJIN_EXPORTER_DEFAULT_BYTES_PER_LINE;
    this->sceneScaleDivide = 1;
    this->skeletonAnimationSampleType = SampleType::INTERVAL;
    this->skeletonAnimationSampleInterval = DEFAULT_SAMPLE_INTERVAL;
    this->vertexAnimationSampleType = SampleType::INTERVAL;
    this->vertexAnimationSampleInterval = DEFAULT_SAMPLE_INTERVAL;
    this->nodeAnimationSampleType = SampleType::INTERVAL;
    this->nodeAnimationSampleInterval = DEFAULT_SAMPLE_INTERVAL;
    this->baseVertexFormatDirectoryType = BaseDirectory::APPLICATION;
    this->includeParentMaterialName = false;
    this->createExtraMaterials = false;
    this->copyBitmaps = true;
    this->convertBitmapsToTextures = true;
    this->externalViewer = FINJIN_VIEWER;
    this->runExternalViewerAfterExport = false;
    this->baseUserDataTypesDirectoryType = BaseDirectory::APPLICATION;
}

FinjinGlobalSettings::~FinjinGlobalSettings()
{
    if (instance == this)
        instance = nullptr;
}

void FinjinGlobalSettings::GetSceneSettingsDefaults(FinjinSceneSettingsAccessor& sceneSettings)
{
    sceneSettings.SetSceneScaleUnit(this->sceneScaleUnit);
    sceneSettings.SetSceneScaleDivide(this->sceneScaleDivide);
}

FinjinGlobalSettings& FinjinGlobalSettings::GetInstance()
{
    static FinjinGlobalSettings globalInstance; //This will initialize the global instance if it hasn't already been initialized

    instance->Load();

    return *instance;
}

void FinjinGlobalSettings::Save()
{
    wxFile outFile(GetConfigSettingsFileName(), wxFile::write);
    WxFileDataChunkWriterOutput output(outFile);
    WxConfigDocumentWriter writer;
    writer.Create(output);

    //Detailed logging?
    if (this->detailedLogging)
        writer.WriteKeyAndValue(wxT("logging"), wxT("detailed"));

    //Text blob format
    writer.WriteKeyAndValue(wxT("blob-text-format"), this->blobTextFormat == WxDataChunkBlobTextFormat::BYTE_ARRAY ? wxT("byte-array") : wxT("base64"));

    //Binary byte order
    if (this->binaryByteOrder != ByteOrder::NATIVE)
        writer.WriteKeyAndValue(wxT("binary-byte-order"), this->binaryByteOrder == ByteOrder::LITTLE ? wxT("little") : wxT("big"));

    //"Additional export" files
    writer.WriteKeyAndValue(wxT("additional-export-json-files"), StringUtilities::ToString(this->additionalExportJsonFiles));
    writer.WriteKeyAndValue(wxT("additional-export-text-files"), StringUtilities::ToString(this->additionalExportTextFiles));
    writer.WriteKeyAndValue(wxT("additional-export-binary-files"), StringUtilities::ToString(this->additionalExportBinaryFiles));
    writer.WriteKeyAndValue(wxT("additional-export-config-files"), StringUtilities::ToString(this->additionalExportConfigFiles));

    //"Embed"
    writer.WriteKeyAndValue(wxT("embed-meshes"), StringUtilities::ToString(this->embedMeshes));
    writer.WriteKeyAndValue(wxT("embed-materials"), StringUtilities::ToString(this->embedMaterials));
    writer.WriteKeyAndValue(wxT("embed-textures"), StringUtilities::ToString(this->embedTextures));
    writer.WriteKeyAndValue(wxT("embed-prefabs"), StringUtilities::ToString(this->embedPrefabs));

    //Export flags
    writer.WriteKeyAndValue(wxT("export-author"), StringUtilities::ToString(this->exportAuthor));
    writer.WriteKeyAndValue(wxT("export-application-name"), StringUtilities::ToString(this->exportApplicationName));
    writer.WriteKeyAndValue(wxT("export-exporter-version"), StringUtilities::ToString(this->exportExporterVersion));
    writer.WriteKeyAndValue(wxT("export-export-time"), StringUtilities::ToString(this->exportExportTime));

    //Export directory
    writer.WriteKeyAndValue(wxT("export-directory"), this->exportDirectory);

    //Max bytes per line
    {
        wxString maxBytesPerLineText;
        maxBytesPerLineText << maxBytesPerLine;
        writer.WriteKeyAndValue(wxT("max-bytes-per-line"), maxBytesPerLineText);
    }

    //Scene scale
    writer.WriteScopeStart(wxT("sceneScale"));
    {
        writer.WriteKeyAndValue(wxT("unit"), this->sceneScaleUnit);

        wxString divide;
        divide << this->sceneScaleDivide;
        writer.WriteKeyAndValue(wxT("divide"), divide);
    }
    writer.WriteScopeEnd();

    //Sampling
    writer.WriteScopeStart(wxT("skeleton-animation-sampling"));
    {
        writer.WriteKeyAndValue(wxT("type"), this->skeletonAnimationSampleType == SampleType::INTERVAL ? wxT("interval") : wxT("rate"));

        wxString interval;
        interval << this->skeletonAnimationSampleInterval;
        writer.WriteKeyAndValue(wxT("interval"), interval);
    }
    writer.WriteScopeEnd();

    writer.WriteScopeStart(wxT("vertex-animation-sampling"));
    {
        writer.WriteKeyAndValue(wxT("type"), this->vertexAnimationSampleType == SampleType::INTERVAL ? wxT("interval") : wxT("rate"));

        wxString interval;
        interval << this->skeletonAnimationSampleInterval;
        writer.WriteKeyAndValue(wxT("interval"), interval);
    }
    writer.WriteScopeEnd();

    writer.WriteScopeStart(wxT("node-animation-sampling"));
    {
        writer.WriteKeyAndValue(wxT("type"), this->nodeAnimationSampleType == SampleType::INTERVAL ? wxT("interval") : wxT("rate"));

        wxString interval;
        interval << this->skeletonAnimationSampleInterval;
        writer.WriteKeyAndValue(wxT("interval"), interval);
    }
    writer.WriteScopeEnd();

    //Node animation interpolation
    if (!this->nodeTranslationInterpolationType.empty())
    {
        writer.WriteKeyAndValue(wxT("node-animation-translation-interpolation"), this->nodeTranslationInterpolationType);
    }
    if (!this->nodeTranslationInterpolationType.empty())
    {
        writer.WriteKeyAndValue(wxT("node-animation-rotation-interpolation"), this->nodeRotationInterpolationType);
    }

    //Meshes
    writer.WriteKeyAndValue(wxT("submesh-naming"), this->submeshNaming);
    writer.WriteKeyAndValue(wxT("submesh-custom-name"), this->submeshCustomName);

    writer.WriteKeyAndValue(wxT("base-vertex-format-directory-type"), BaseDirectory::ToString(this->baseVertexFormatDirectoryType));
    writer.WriteKeyAndValue(wxT("base-vertex-format-directory"), this->baseVertexFormatDirectory);
    writer.WriteKeyAndValue(wxT("vertex-format-path"), this->vertexFormatPath);

    //Materials
    writer.WriteKeyAndValue(wxT("no-material-name"), this->noMaterialName);
    writer.WriteKeyAndValue(wxT("include-parent-material-name"), StringUtilities::ToString(this->includeParentMaterialName));
    writer.WriteKeyAndValue(wxT("create-materials"), this->createExtraMaterials ? wxT("true") : wxT("false"));

    //Bitmaps
    writer.WriteKeyAndValue(wxT("copy-bitmaps"), StringUtilities::ToString(this->copyBitmaps));
    writer.WriteKeyAndValue(wxT("convert-bitmaps-to-textures"), StringUtilities::ToString(this->convertBitmapsToTextures));

    //External viewer
    wxString externalViewerName;
    externalViewerName = wxT("FinjinViewer");
    writer.WriteKeyAndValue(wxT("external-viewer"), externalViewerName);

    //Run external viewer after export
    writer.WriteKeyAndValue(wxT("run-external-viewer-after-export"), StringUtilities::ToString(this->runExternalViewerAfterExport));

    //User data types
    writer.WriteKeyAndValue(wxT("base-user-data-types-directory-type"), BaseDirectory::ToString(this->baseUserDataTypesDirectoryType));
    writer.WriteKeyAndValue(wxT("base-user-data-types-directory"), this->baseUserDataTypesDirectory);
    writer.WriteScopeStart(wxT("user-data-type-locations"));
    {
        for (auto& location : this->userDataTypesLocations)
            writer.WriteKeyAndValue(location.GetTypeString(), location.name);
    }
    writer.WriteScopeEnd();

    //Flags
    writer.WriteScopeStart(wxT("named-flags"));
    {
        for (auto& f : this->flags)
        {
            writer.WriteKeyAndValue(f.name, StringUtilities::ToString(f.bit));
        }
    }
    writer.WriteScopeEnd();
}

void FinjinGlobalSettings::Load()
{
    if (this->loaded)
        return;

    this->loaded = true;

    std::vector<uint8_t> fileContent;
    if (!FileUtilities::ReadBinaryFile(GetConfigSettingsFileName(), fileContent))
        return;

    WxConfigDocumentReader reader;
    wxString currentSectionName;
    for (auto line = reader.Start(fileContent); line != nullptr; line = line = reader.Next())
    {
        switch (line->GetType())
        {
            case WxConfigDocumentLine::Type::SECTION:
            {
                currentSectionName = line->GetSectionName();
                break;
            }
            case WxConfigDocumentLine::Type::SCOPE_START:
            {
                break;
            }
            case WxConfigDocumentLine::Type::SCOPE_END:
            {
                currentSectionName.clear();
                break;
            }
            case WxConfigDocumentLine::Type::KEY_AND_VALUE:
            {
                auto key = line->GetKey();
                auto value = line->GetValue();

                if (currentSectionName == wxT("named-flags"))
                {
                    this->flags.push_back(NamedFlag(key, StringUtilities::ToInt(value)));
                }
                else if (key == wxT("logging"))
                {
                    this->detailedLogging = value == wxT("detailed");
                }
                else if (key == wxT("blob-text-format"))
                {
                    this->blobTextFormat = value == wxT("byte-array") ? WxDataChunkBlobTextFormat::BYTE_ARRAY : WxDataChunkBlobTextFormat::BASE64;
                }
                else if (key == wxT("binary-byte-order"))
                {
                    if (value == wxT("little"))
                        this->binaryByteOrder = ByteOrder::LITTLE;
                    else if (value == wxT("big"))
                        this->binaryByteOrder = ByteOrder::BIG;
                    else
                        this->binaryByteOrder = ByteOrder::NATIVE;
                }
                else if (key == wxT("additional-export-json-files"))
                {
                    this->additionalExportJsonFiles = StringUtilities::ToBool(value);
                }
                else if (key == wxT("additional-export-text-files"))
                {
                    this->additionalExportTextFiles = StringUtilities::ToBool(value);
                }
                else if (key == wxT("additional-export-binary-files"))
                {
                    this->additionalExportBinaryFiles = StringUtilities::ToBool(value);
                }
                else if (key == wxT("additional-export-config-files"))
                {
                    this->additionalExportConfigFiles = StringUtilities::ToBool(value);
                }
                else if (key == wxT("embed-meshes"))
                {
                    this->embedMeshes = StringUtilities::ToBool(value);
                }
                else if (key == wxT("embed-materials"))
                {
                    this->embedMaterials = StringUtilities::ToBool(value);
                }
                else if (key == wxT("embed-textures"))
                {
                    this->embedTextures = StringUtilities::ToBool(value);
                }
                else if (key == wxT("embed-prefabs"))
                {
                    this->embedPrefabs = StringUtilities::ToBool(value);
                }
                else if (key == wxT("export-author"))
                {
                    this->exportAuthor = StringUtilities::ToBool(value);
                }
                else if (key == wxT("export-application-name"))
                {
                    this->exportApplicationName = StringUtilities::ToBool(value);
                }
                else if (key == wxT("export-exporter-version"))
                {
                    this->exportExporterVersion = StringUtilities::ToBool(value);
                }
                else if (key == wxT("export-export-time"))
                {
                    this->exportExportTime = StringUtilities::ToBool(value);
                }
                else if (key == wxT("export-directory"))
                {
                    this->exportDirectory = value;
                }
                else if (key == wxT("max-bytes-per-line"))
                {
                    this->maxBytesPerLine = StringUtilities::ToUInt(value);
                    if (this->maxBytesPerLine < FINJIN_EXPORTER_MIN_BYTES_PER_LINE)
                        this->maxBytesPerLine = FINJIN_EXPORTER_MIN_BYTES_PER_LINE;
                }
                else if (currentSectionName == wxT("skeleton-animation-sampling") && key == wxT("type"))
                {
                    this->skeletonAnimationSampleType = value == wxT("interval") ? SampleType::INTERVAL : SampleType::RATE;
                }
                else if (currentSectionName == wxT("skeleton-animation-sampling") && key == wxT("interval"))
                {
                    this->skeletonAnimationSampleInterval = StringUtilities::ToDouble(value);
                    if (this->skeletonAnimationSampleInterval <= 0)
                        this->skeletonAnimationSampleInterval = DEFAULT_SAMPLE_INTERVAL;
                }
                else if (currentSectionName == wxT("vertex-animation-sampling") && key == wxT("type"))
                {
                    this->vertexAnimationSampleType = value == wxT("interval") ? SampleType::INTERVAL : SampleType::RATE;
                }
                else if (currentSectionName == wxT("vertex-animation-sampling") && key == wxT("interval"))
                {
                    this->vertexAnimationSampleInterval = StringUtilities::ToDouble(value);
                    if (this->vertexAnimationSampleInterval <= 0)
                        this->vertexAnimationSampleInterval = DEFAULT_SAMPLE_INTERVAL;
                }
                else if (currentSectionName == wxT("node-animation-sampling") && key == wxT("type"))
                {
                    this->nodeAnimationSampleType = value == wxT("interval") ? SampleType::INTERVAL : SampleType::RATE;
                }
                else if (currentSectionName == wxT("node-animation-sampling") && key == wxT("interval"))
                {
                    this->nodeAnimationSampleInterval = StringUtilities::ToDouble(value);
                    if (this->nodeAnimationSampleInterval <= 0)
                        this->nodeAnimationSampleInterval = DEFAULT_SAMPLE_INTERVAL;
                }
                else if (key == wxT("node-animation-translation-interpolation"))
                {
                    this->nodeTranslationInterpolationType = value;
                }
                else if (key == wxT("node-animation-rotation-interpolation"))
                {
                    this->nodeRotationInterpolationType = value;
                }
                else if (key == wxT("submesh-naming"))
                {
                    this->submeshNaming = value;
                }
                else if (key == wxT("submesh-custom-name"))
                {
                    this->submeshCustomName = value;
                }
                else if (key == wxT("base-vertex-format-directory-type"))
                {
                    this->baseVertexFormatDirectoryType = BaseDirectory::ParseType(value);
                }
                else if (key == wxT("base-vertex-format-directory"))
                {
                    this->baseVertexFormatDirectory = value;
                }
                else if (key == wxT("vertex-format-path"))
                {
                    this->vertexFormatPath = value;
                }
                else if (key == wxT("no-material-name"))
                {
                    this->noMaterialName = value;
                }
                else if (key == wxT("include-parent-material-name"))
                {
                    this->includeParentMaterialName = StringUtilities::ToBool(value);
                }
                else if (key == wxT("create-materials"))
                {
                    this->createExtraMaterials = StringUtilities::ToBool(value);
                }
                else if (key == wxT("copy-bitmaps"))
                {
                    this->copyBitmaps = StringUtilities::ToBool(value);
                }
                else if (key == wxT("convert-bitmaps-to-textures"))
                {
                    this->convertBitmapsToTextures = StringUtilities::ToBool(value);
                }
                else if (key == wxT("external-viewer"))
                {
                    if (value == wxT("FinjinViewer"))
                        this->externalViewer = FINJIN_VIEWER;
                    else
                        this->runExternalViewerAfterExport = false;
                }
                else if (key == wxT("run-external-viewer-after-export"))
                {
                    this->runExternalViewerAfterExport = StringUtilities::ToBool(value);
                }
                else if (key == wxT("base-user-data-types-directory-type"))
                {
                    this->baseUserDataTypesDirectoryType = BaseDirectory::ParseType(value);
                }
                else if (key == wxT("base-user-data-types-directory"))
                {
                    this->baseUserDataTypesDirectory = value;
                }
                else if (currentSectionName == wxT("user-data-type-locations"))
                {
                    if (key == wxT("file"))
                        this->userDataTypesLocations.push_back(UserDataTypesLocation(UserDataTypesLocation::FILE, value));
                    else
                        this->userDataTypesLocations.push_back(UserDataTypesLocation(UserDataTypesLocation::DIRECTORY, value));
                }

                break;
            }
            default: break;
        }
    }
}

bool FinjinGlobalSettings::HasFlags() const
{
    return !this->flags.empty();
}

void FinjinGlobalSettings::GetFlags(std::vector<wxString>& names, std::vector<int>& bits)
{
    names.resize(this->flags.size());
    bits.resize(this->flags.size());

    size_t i = 0;
    for (auto& f : this->flags)
    {
        names[i] = f.name;
        bits[i] = f.bit;
        i++;
    }
}

void FinjinGlobalSettings::GetFlag(int i, wxString& name, int& bit)
{
    if (i < this->flags.size())
    {
        name = this->flags[i].name;
        bit = this->flags[i].bit;
    }
    else
    {
        name.clear();
        bit = -1;
    }
}

int FinjinGlobalSettings::GetFlagBit(const wxString& name)
{
    for (auto& f : this->flags)
    {
        if (f.name == name)
            return f.bit;
    }

    return -1;
}

wxString FinjinGlobalSettings::GetWorkingBaseUserDataTypesDirectory() const
{
    BaseDirectory directory(this->baseUserDataTypesDirectoryType, this->baseUserDataTypesDirectory);
    auto baseDirectory = directory.GetBaseDirectory();
    MacroExpander pathExpander;
    pathExpander.Expand(baseDirectory);
    return baseDirectory;
}

wxString FinjinGlobalSettings::GetWorkingVertexFormatFilePath() const
{
    BaseDirectory directory(this->baseVertexFormatDirectoryType, this->baseVertexFormatDirectory);
    auto baseDirectory = directory.GetBaseDirectory();
    MacroExpander pathExpander;
    pathExpander.Expand(baseDirectory);
    auto result = FileUtilities::JoinPath(baseDirectory, this->vertexFormatPath);
    return result;
}

bool FinjinGlobalSettings::GetExportAnyDocumentCreationData() const
{
    return
        this->exportAuthor ||
        this->exportApplicationName ||
        this->exportExporterVersion ||
        this->exportExportTime
        ;
}
