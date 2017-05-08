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
#include "Exporter.hpp"
#include "FinjinSceneDocument.hpp"
#include "ExporterHandler.hpp"
#include "FinjinSceneExporterHandler.hpp"
#include "FileUtilities.hpp"
#include "StringUtilities.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "Strings.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "wxApplicationWindow.hpp"
#include "ProgressDialog.hpp"
#include "ApplicationAccessor.hpp"
#include "ExporterSystem.hpp"
#include "FinjinGlobalSettings.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
Exporter::FactoryList Exporter::factories;
wxString Exporter::fileFilter;
wxString Exporter::lastInputFileName;


//Local functions---------------------------------------------------------------
static bool RunExportSequence
    (
    ExporterHandlerFactory* factory,
    const wxString& fileName,
    bool selected,
    FinjinSceneSettingsAccessor sceneSettings
    )
{
    auto result = false;
    wxString resultMessage;

    ApplicationAccessor::EnableExport();

    //Log start time
    auto logInfo = Logger::CanHandle(INFO_LOG_MESSAGE);
    auto startTime = wxDateTime::Now();
    wxDateTime endTime;
    if (logInfo)
    {
        auto formattedStartTime = Strings::FormatTime(startTime);
        FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, Strings::STARTED_EXPORT_ON_FORMAT, formattedStartTime.wx_str());
    }

    //Initialize the export handler
    {
        std::unique_ptr<ExporterHandler> exportHandler(factory->CreateHandler());
        if (exportHandler->Initialize(AssetClass::SCENE, fileName, selected, sceneSettings))
        {
            //Run export
            if (exportHandler->RunExport())
            {
                //Export was a success

                //Get the finish time now in case there are 'after' commands
                endTime = wxDateTime::Now();

                result = true;
                resultMessage = Strings::EXPORT_FINISHED;
            }
            else
                resultMessage = Strings::ERROR_RUNNING_EXPORTER;
        }
        else
            resultMessage = Strings::ERROR_INITIALIZING_EXPORTER;
        exportHandler->Shutdown();
    }

    //Log end time
    if (logInfo)
    {
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
    if (ProgressDialog::GetInstance() != nullptr)
        ProgressDialog::GetInstance()->FinishedTask(resultMessage);

    ApplicationAccessor::EnableExport(false);

    return result;
}


//Implementation----------------------------------------------------------------
bool Exporter::ExportWithFileName
    (
    const wxString& fileName,
    bool selected,
    ExporterHandlerFactory* factory,
    bool allowStatusDialogs
    )
{
    wxString exportedFileName;
    auto gotSaveFileName = false;
    auto sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();

    if (!fileName.empty())
    {
        //Ensure input file name is in an accessible directory
        gotSaveFileName = FileUtilities::CreateFileDirectoryRecursive(fileName);
        if (gotSaveFileName)
            exportedFileName = fileName;
    }

    //Do the export
    auto result = false;
    if (gotSaveFileName)
    {
        //Perform the export
        Exporter exporter;
        auto flags = allowStatusDialogs ? ExporterFlags::NONE : ExporterFlags::NO_STATUS_DIALOGS;
        result = exporter.Export(exportedFileName, selected, sceneSettings, flags, factory);

        //Save file name for next run
        lastInputFileName = exportedFileName;
    }

    if (result && allowStatusDialogs)
        RunExternalViewerAfterExport();

    return result;
}

bool Exporter::ExportWithDialog
    (
    bool selected,
    bool forceSaveDialog,
    ExporterHandlerFactory* factory
    )
{
    wxString exportedFileName;
    auto gotSaveFileName = false;

    auto sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();

    //If the default scene file name didn't work, try to get the file name from the user
    if (!gotSaveFileName)
    {
        SUSPEND_CUSTOM_CONTROLS;

        wxApplicationWindow appWindow;
        auto defaultDirectory = FileUtilities::GetDirectoryPath(lastInputFileName);
        auto defaultFile = FileUtilities::GetFileName(lastInputFileName, false);
        wxFileDialog dialog(&appWindow, Strings::CHOOSE_FILE, defaultDirectory, defaultFile, GetFileFilter(), wxFD_SAVE);

        //Set filter index
        if (!lastInputFileName.empty())
        {
            int filterIndex = GetFilterIndexForFileName(lastInputFileName);
            if (filterIndex != -1)
                dialog.SetFilterIndex(filterIndex);
        }

        if (dialog.ShowModal() == wxID_OK)
        {
            gotSaveFileName = true;
            exportedFileName = dialog.GetPath();
        }
    }

    //Do the export
    auto result = false;
    if (gotSaveFileName)
    {
        //Perform the export
        Exporter exporter;
        result = exporter.Export(exportedFileName, selected, sceneSettings, ExporterFlags::NONE, factory);

        //Save file name for next run
        lastInputFileName = exportedFileName;
    }

    if (result)
        RunExternalViewerAfterExport();

    return result;
}

void Exporter::RegisterHandlerFactory(ExporterHandlerFactory& factory)
{
    if (std::find(factories.begin(), factories.end(), &factory) == factories.end())
    {
        //Add to factory list
        factories.push_back(&factory);

        //Invalidate file filter string
        fileFilter.clear();
    }
}

void Exporter::ClearHandlerFactories()
{
    factories.clear();
    fileFilter.clear();
}

const wxString& Exporter::GetFileFilter()
{
    if (fileFilter.empty() && !factories.empty())
    {
        //Rebuild file filter string
        wxString extension;
        wxString description;
        for (auto factory : factories)
        {
            for (int i = 0; i < factory->GetExtensionCount(); i++)
            {
                factory->GetExtension(i, extension, &description);

                if (!fileFilter.empty())
                    fileFilter += wxT("|");

                fileFilter += wxString::Format
                    (
                    wxT("%s (*%s)|*%s"),
                    description.wx_str(),
                    extension.wx_str(),
                    extension.wx_str()
                    );
            }
        }
    }

    return fileFilter;
}

bool Exporter::Export
    (
    const wxString& fileName,
    bool selected,
    FinjinSceneSettingsAccessor sceneSettings,
    ExporterFlags exportFlags,
    ExporterHandlerFactory* factory
    )
{
    auto result = false;

    //Get the factory for the file name
    int extensionIndex = -1;
    if (factory == nullptr)
    {
        //Factory not specified. Get the best one
        factory = GetFactoryForFileName(fileName, &extensionIndex);
    }
    else
    {
        //The factory was specified. Find the index of the best extension
        for (int i = 0; i < factory->GetExtensionCount(); i++)
        {
            if (StringUtilities::EndsWithNoCase(fileName, factory->GetExtension(i)))
            {
                extensionIndex = i;
                break;
            }
        }
    }

    if (factory != nullptr)
    {
        //Show progress dialog
        if (NoneSet(exportFlags & ExporterFlags::NO_STATUS_DIALOGS))
        {
            //Create the progress dialog as a modeless dialog.
            //The dialog takes care of deleting itself when closed
            SUSPEND_CUSTOM_CONTROLS;
            auto progressTitle = wxString::Format(Strings::EXPORT_PROGRESS_DIALOG_TITLE_FORMAT, factory->GetDisplayName().wx_str());
            auto progressDialog = new ProgressDialog(progressTitle, LogListener::DEFAULT_LOG_LEVEL, Strings::EXPORTING_SCENE);
            progressDialog->Show();
        }

        //The factory was chosen with a case-insensitive extension comparison
        //Ensure that the extension has the same capitalization that the handler expects
        auto fileNameWithExtension = fileName;
        if (extensionIndex >= 0)
            FileUtilities::EnsureExtension(fileNameWithExtension, factory->GetExtension(extensionIndex));

        //Run the export
        result = RunExportSequence(factory, fileNameWithExtension, selected, sceneSettings);
    }

    return result;
}

ExporterHandlerFactory* Exporter::GetFactoryForFileName(const wxString& fileName, int* extensionIndex)
{
    ExporterHandlerFactory* foundFactory = nullptr;
    int foundExtensionIndex = -1;

    //Find a factory that handles the specified extension
    wxString extensionWithDot;
    for (auto factory : factories)
    {
        for (int i = 0; i < factory->GetExtensionCount(); i++)
        {
            if (StringUtilities::EndsWithNoCase(fileName, factory->GetExtension(i)))
            {
                foundFactory = factory;
                foundExtensionIndex = i;
                break;
            }
        }
    }

    //Get a default factory if necessary
    if (foundFactory == nullptr && !factories.empty())
        foundFactory = factories.front();

    if (extensionIndex != nullptr)
        *extensionIndex = foundExtensionIndex;

    return foundFactory;
}

int Exporter::GetFilterIndexForFileName(const wxString& fileName)
{
    int filterIndex = 0;
    for (auto factory : factories)
    {
        for (int i = 0; i < factory->GetExtensionCount(); i++)
        {
            if (StringUtilities::EndsWithNoCase(fileName, factory->GetExtension(i)))
                return filterIndex;
            else
                filterIndex++;
        }
    }

    return -1;
}

void Exporter::RunExternalViewerAfterExport()
{
    //Make sure the file exists and we're allowed to run viewer
    if (wxFileExists(lastInputFileName) &&
        FinjinGlobalSettings::GetInstance().runExternalViewerAfterExport)
    {
        ExporterSystem::RunExternalViewer(lastInputFileName);
    }
}
