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
#include "MaterialExporter.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "FileUtilities.hpp"
#include "ExporterUtilities.hpp"
#include "StringUtilities.hpp"
#include "Dialogs.hpp"
#include "wxApplicationWindow.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "Strings.hpp"
#include "ApplicationAccessor.hpp"
#include "ProgressDialog.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"
#include "FinjinGlobalSettings.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
MaterialExporter::FactoryList MaterialExporter::factories;


//Implementation----------------------------------------------------------------
MaterialExporter::MaterialExporter()
{
}

void MaterialExporter::SetProgressCalculator(std::shared_ptr<ProgressCalculator> progress)
{
    this->materialsProgress = progress;
}

void MaterialExporter::SetBitmapsProgressCalculator(std::shared_ptr<ProgressCalculator> progress)
{
    this->bitmapsProgress = progress;
}

bool MaterialExporter::Export
    (
    WxDataChunkWriter& writer,
    const MaterialAccessorMap<std::shared_ptr<MaterialExporterHandler> >& materials,
    FinjinSceneSettingsAccessor sceneSettings,
    const SceneExportSettings& sceneExportSettings
    )
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("MaterialExporter::Export()"));

    if (this->materialsProgress != nullptr)
        this->materialsProgress->SetRange(materials.size());

    //Sort the materials by name-----------------------------
    struct SortedMaterial
    {
        wxString materialName;
        wxString sortName;
        MaterialAccessor material;
        std::shared_ptr<MaterialExporterHandler> exporterHandler;

        bool operator < (const SortedMaterial& other) const
        {
            return this->sortName < other.sortName;
        }
    };

    std::vector<SortedMaterial> sortedMaterials;
    sortedMaterials.reserve(materials.size());
    for (auto& item : materials)
    {
        SortedMaterial sorted;

        sorted.material = item.first;
        sorted.exporterHandler = item.second;

        sorted.materialName = sceneExportSettings.GetMaterialName(sorted.material);

        sorted.sortName = sorted.materialName;
        sorted.sortName.MakeLower();

        sortedMaterials.push_back(sorted);
    }
    std::sort(sortedMaterials.begin(), sortedMaterials.end());

    //Now export-----------------------------
    std::set<wxString> exportedMaterialNames;

    if (!sortedMaterials.empty())
    {
        for (auto sorted : sortedMaterials)
        {
            //Don't write a material with the same name. This works because materials are written out in sorted order
            if (exportedMaterialNames.find(sorted.materialName) == exportedMaterialNames.end())
            {
                exportedMaterialNames.insert(sorted.materialName);
            }
        }

        FINJIN_WX_DECLARE_ERROR(error);

        writer.WriteChunk(StandardAssetDocumentChunkNames::MATERIALS, [this, &exportedMaterialNames, &sortedMaterials, &sceneSettings, &sceneExportSettings](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, exportedMaterialNames.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            exportedMaterialNames.clear();

            size_t materialIndex = 0;
            for (auto sorted : sortedMaterials)
            {
                //Don't write a material with the same name. This works because materials are written out in sorted order
                if (exportedMaterialNames.find(sorted.materialName) == exportedMaterialNames.end())
                {
                    //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Exporting material '%s'."), sorted.material.GetLocalName().wx_str());

                    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, sceneExportSettings.GetMaterialName(sorted.material));

                    WxChunkName materialChunkName(StandardAssetDocumentChunkNames::MATERIAL, materialIndex++);
                    writer.WriteChunk(materialChunkName, [this, &sorted](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        sorted.exporterHandler->Write(writer, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                    }, error);
                    if (error)
                    {
                        auto message = wxString::Format(wxT("Failed to export material (%s). Aborting export process."), sorted.materialName.wx_str());
                        FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, message);

                        break;
                    }

                    if (this->materialsProgress != nullptr)
                        this->materialsProgress->Update(1);

                    sorted.exporterHandler->GetBitmapFileNames(this->bitmapFileNames);

                    exportedMaterialNames.insert(sorted.materialName);
                }
                else
                {
                    auto message = wxString::Format(wxT("Duplicate material name encountered (%s). Try enabling the 'Include Parent Material in Name' setting on the Materials page of the Finjin Scene Settings dialog."), sorted.materialName.wx_str());
                    FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, message);
                }
            }
        }, error);
        if (error)
        {
            FINJIN_WX_SET_ERROR_NO_MESSAGE(error);
            return false;
        }
    }

    return true;
}

void MaterialExporter::ExportBitmaps
    (
    WxDataChunkWriter& writer,
    FinjinSceneSettingsAccessor sceneSettings,
    const SceneExportSettings& sceneExportSettings,
    WxError& error
    )
{
    FINJIN_WX_ERROR_METHOD_START(error);

    //Export bitmaps
    std::vector<wxString> allBitmapSourceFileNames;
    for (auto& sourceBitmapFileName : this->bitmapFileNames)
    {
        std::vector<wxString> allSourceFileNames;
        sourceBitmapFileName.GetAllFileNames(allSourceFileNames);
        PushAll(allBitmapSourceFileNames, allSourceFileNames);
    }

    if (allBitmapSourceFileNames.empty())
        return;

    if (this->bitmapsProgress != nullptr)
        this->bitmapsProgress->SetRange(allBitmapSourceFileNames.size());

    //Textures
    writer.WriteChunk(StandardAssetDocumentChunkNames::TEXTURES, [this, &sceneExportSettings, &allBitmapSourceFileNames](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        //Count
        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, allBitmapSourceFileNames.size(), error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Each texture
        for (size_t textureIndex = 0; textureIndex < allBitmapSourceFileNames.size(); textureIndex++)
        {
            auto& sourceFilePath = allBitmapSourceFileNames[textureIndex];

            if (this->bitmapsProgress != nullptr)
                this->bitmapsProgress->Update(1);

            //Texture
            WxChunkName textureChunkName(StandardAssetDocumentChunkNames::TEXTURE, textureIndex);
            writer.WriteChunk(textureChunkName, [this, &sceneExportSettings, &sourceFilePath](WxDataChunkWriter& writer, WxError& error)
            {
                FINJIN_WX_ERROR_METHOD_START(error);

                //Get destination base file name and extension
                auto destination = sceneExportSettings.ResolveAssetPaths(AssetClass::TEXTURE, sourceFilePath);

                //Get path to the destination file if file name isn't absolute
                auto foundFile = true;
                if (wxIsAbsolutePath(sourceFilePath))
                    foundFile = wxFileExists(sourceFilePath);
                else
                {
                    sourceFilePath = FileUtilities::JoinPath(ApplicationAccessor::GetCurrentProjectDirectory(), sourceFilePath);
                    foundFile = wxFileExists(sourceFilePath);
                }

                if (foundFile && sceneExportSettings.IsEmbeddedAssetType(AssetClass::TEXTURE))
                {
                    auto name = ExporterUtilities::GetTextureName(destination.relativePath);
                    writer.WriteString(StandardAssetDocumentPropertyNames::NAME, name, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    if (!ExporterUtilities::ConvertCopyImage(sourceFilePath, writer, StandardAssetDocumentPropertyNames::CONTENT_SIZE, StandardAssetDocumentPropertyNames::CONTENT, sceneExportSettings.convertBitmapsToTextures))
                    {
                    }
                }
                else if (foundFile && sceneExportSettings.copyBitmaps)
                {
                    writer.WriteString(StandardAssetDocumentPropertyNames::TEXTURE_REF, sceneExportSettings.GetAssetFileReference(AssetClass::TEXTURE, destination).ToUriString(), error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    if (!ExporterUtilities::ConvertCopyImage(sourceFilePath, sceneExportSettings.byteOrder, destination.absolutePath, sceneExportSettings.convertBitmapsToTextures))
                    {
                    }
                }
                else
                {
                    writer.WriteString(StandardAssetDocumentPropertyNames::TEXTURE_REF, sceneExportSettings.GetAssetFileReference(AssetClass::TEXTURE, destination).ToUriString(), error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }
            }, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

void MaterialExporter::ExportSceneMaterialsWithDialog()
{
    MaterialAccessorVector materials;
    MaterialAccessor::GetAllMaterials(materials);
    if (!materials.empty())
        ExportMaterialsWithDialog(materials);
    else
        Dialogs::ShowMessage(Strings::EXPORT_MATERIAL_ERROR_NO_SCENE_MATERIALS, Strings::EXPORT_MATERIAL_ERROR_CAPTION, wxINFORMATION_DIALOG_FLAGS);
}

void MaterialExporter::ExportSelectedMaterialsWithDialog()
{
    MaterialAccessorVector selectedMaterials;
    MaterialAccessor::GetSelectedMaterials(selectedMaterials);
    if (!selectedMaterials.empty())
        ExportMaterialsWithDialog(selectedMaterials);
    else
    {
        auto errorMessage = wxString::Format(Strings::EXPORT_MATERIAL_ERROR_NO_SELECTED_MATERIAL_FORMAT, ApplicationAccessor::MATERIAL_SELECTOR_WINDOW_NAME.wx_str());
        Dialogs::ShowMessage(errorMessage, Strings::EXPORT_MATERIAL_ERROR_CAPTION, wxINFORMATION_DIALOG_FLAGS);
    }
}

void MaterialExporter::ExportMaterialsWithDialog(const MaterialAccessorVector& materials)
{
    //Show "save" dialog
    SUSPEND_CUSTOM_CONTROLS;
    wxApplicationWindow appWindow;
    wxFileDialog dialog(&appWindow, Strings::CHOOSE_FILE, wxEmptyString, wxEmptyString, Strings::MATERIAL_FILE_FILTER, wxFD_SAVE);
    if (dialog.ShowModal() == wxID_OK)
    {
        auto dialogFilePath = dialog.GetPath();

        //Create and show progress dialog
        auto progressDialog = new ProgressDialog(Strings::FINJIN_MATERIAL_EXPORTER, LogListener::DEFAULT_LOG_LEVEL, Strings::EXPORTING_MATERIALS);
        progressDialog->Show();

        ApplicationAccessor::EnableExport();

        //Log start time
        auto canLogInfo = Logger::CanHandle(INFO_LOG_MESSAGE);
        auto startTime = wxDateTime::Now();
        if (canLogInfo)
        {
            auto formattedStartTime = Strings::FormatTime(startTime);
            FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, Strings::STARTED_EXPORT_ON_FORMAT, formattedStartTime.wx_str());
        }

        //Get the scene settings
        auto sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();
        SceneExportSettings sceneExportSettings;
        sceneExportSettings.Initialize(sceneSettings, SceneExportSettingsFlags::NONE);
        sceneExportSettings.MakeExportPaths(AssetClass::MATERIAL, dialogFilePath);

        //Collect all materials and names
        MaterialAccessorMap<std::shared_ptr<MaterialExporterHandler> > materialHandlerMap;
        for (auto rawMaterial : materials)
        {
            MaterialAccessorVector expandedMaterials;
            rawMaterial.Expand(expandedMaterials);
            for (auto& material : expandedMaterials)
            {
                if (materialHandlerMap.find(material) == materialHandlerMap.end())
                {
                    auto factory = GetFactoryForMaterial(material);
                    if (factory != nullptr)
                    {
                        std::shared_ptr<MaterialExporterHandler> exporterHandler(factory->CreateHandler());

                        exporterHandler->Initialize(material, sceneSettings, sceneExportSettings);

                        //Add the material if it hasn't been added already
                        materialHandlerMap.insert(std::make_pair(material, exporterHandler));
                    }
                    else
                    {
                        auto message = wxString::Format(wxT("Failed to create export handler for material (%s). Skipping material."), material.GetLocalName().wx_str());
                        FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, message);
                    }
                }
            }
        }

        //Perform export
        MaterialExporter materialExporter;
        materialExporter.InitializeWithDialog();

        WxDataChunkWriter::Settings writerSettings;
        writerSettings.byteOrder = sceneExportSettings.byteOrder;
        writerSettings.maxBytesPerLine = FinjinGlobalSettings::GetInstance().maxBytesPerLine;
        writerSettings.blobTextFormat = FinjinGlobalSettings::GetInstance().blobTextFormat;
        writerSettings.lengthHintPropertyName = StandardAssetDocumentPropertyNames::LENGTH_HINT;

        auto sceneObjectName = FileUtilities::GetFileNameNoExtension(dialogFilePath);
        ExporterUtilities::FixObjectName(sceneObjectName);

        for (; sceneExportSettings.HasCurrentFileFormat(); sceneExportSettings.NextFileFormat())
        {
            auto currentFileFormat = sceneExportSettings.GetCurrentFileFormat();
            FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, Strings::EXPORTING_FILE_FORMAT_FORMAT, WxStreamingFileFormatUtilities::ToString(currentFileFormat).wx_str());

            FINJIN_WX_DECLARE_ERROR(error);

            SceneDataChunkWriterController chunkWriterController(&sceneExportSettings);

            auto exportSceneFileNamesForFormat = sceneExportSettings.GetAssetFilePaths(sceneExportSettings.defaultAssetType, sceneObjectName);
            auto writer = WxStreamingFileFormatUtilities::CreateFileWriter(exportSceneFileNamesForFormat.absolutePath, currentFileFormat, writerSettings, chunkWriterController, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            writer->SetContextStringProcessor(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ExporterUtilities::FixObjectName);

            writer->WriteWriterHeader(error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            materialExporter.Export(*writer.get(), materialHandlerMap, sceneSettings, sceneExportSettings);

            materialExporter.ExportBitmaps(*writer.get(), sceneSettings, sceneExportSettings, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }

        //Log end time
        if (canLogInfo)
        {
            auto endTime = wxDateTime::Now();
            auto formattedEndTime = Strings::FormatTime(endTime);
            auto elapsedTimeText = Strings::FormatElapsedTime(endTime.Subtract(startTime));
            FINJIN_EXPORTER_LOG_MESSAGE
                (
                INFO_LOG_MESSAGE,
                Strings::FINISHED_EXPORT_ON_ELAPSED_TIME_FORMAT,
                formattedEndTime.wx_str(),
                elapsedTimeText.wx_str()
                );
        }

        //Update progress dialog
        progressDialog->FinishedTask(Strings::EXPORT_FINISHED);

        ApplicationAccessor::EnableExport(false);
    }
}

void MaterialExporter::RegisterHandlerFactory(MaterialExporterHandlerFactory& factory)
{
    if (std::find(factories.begin(), factories.end(), &factory) == factories.end())
    {
        //Add to factory list
        factories.push_back(&factory);
    }
}

void MaterialExporter::ClearHandlerFactories()
{
    factories.clear();
}

MaterialExporterHandlerFactory* MaterialExporter::GetFactoryForMaterial(MaterialAccessor material)
{
    MaterialExporterHandlerFactory* foundFactory = nullptr;

    //Find a factory that handles the specified extension
    for (auto factory : factories)
    {
        if (factory->CanHandle(material))
        {
            foundFactory = factory;
            break;
        }
    }

    //Get a default factory if necessary
    if (foundFactory == nullptr && !factories.empty())
        foundFactory = factories.back();

    return foundFactory;
}

void MaterialExporter::InitializeWithDialog()
{
    this->withDialogProgress.AddProgressChangeListener(this);

    SetProgressCalculator(this->withDialogProgress.AddCalculator(wxT("material")));
}

void MaterialExporter::ProgressChanged(ProgressCalculator* progress)
{
    int itemIndex = RoundToInt(progress->GetProgress() * progress->GetRange());
    int itemTotal = RoundToInt(progress->GetRange());

    wxString message;
    if (!progress->GetName().empty())
    {
        message = wxString::Format
            (
            wxT("Exporting %s %d of %d"),
            progress->GetName().wx_str(),
            itemIndex,
            itemTotal
            );
    }

    if (ProgressDialog::GetInstance() != nullptr)
    {
        ProgressDialog::GetInstance()->UpdateProgress(this->withDialogProgress.GetProgress(), progress->GetProgress());
        ProgressDialog::GetInstance()->SetSubprogressLabel(message);
    }

    if (!message.empty())
    {
        FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, message);

        /*ApplicationAccessor::LogMessage
            (
            wxT("%s...progress: %f, total progress: %f"),
            message.wx_str(),
            progress->GetProgress(),
            this->withDialogProgress.GetProgress()
            );*/
    }
    else
    {
        //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Total progress: 1"));
    }
}

const GenericBitmapFileNameSet& MaterialExporter::GetBitmapFileNames() const
{
    return this->bitmapFileNames;
}
