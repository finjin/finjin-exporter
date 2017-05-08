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
#include "ExporterSystem.hpp"
#include "FinjinResourceManager.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "HelpSystem.hpp"
#include "FinjinGlobalSettings.hpp"
#include "Dialogs.hpp"
#include "Strings.hpp"
#include "ApplicationAccessor.hpp"
#include "Exporter.hpp"
#include "FileUtilities.hpp"
#include "GeometryState.hpp"
#include "ArgsArray.hpp"
#include "LoggerListeners.hpp"
#include "finjin/common/WxUuid.hpp"

#if defined(__WXMSW__)
    #pragma comment(lib, "wxmsw31u_core.lib")
    #pragma comment(lib, "wxmsw31u_adv.lib")
    #pragma comment(lib, "wxbase31u.lib")
    #pragma comment(lib, "wxtiff.lib")
    #pragma comment(lib, "wxjpeg.lib")
    #pragma comment(lib, "wxpng.lib")
    #pragma comment(lib, "wxzlib.lib")
    #pragma comment(lib, "wxregexu.lib")
    #pragma comment(lib, "wxexpat.lib")
    #pragma comment(lib, "FreeImage-x64.lib")
    #pragma comment(lib, "LibJPEG-x64.lib")
    #pragma comment(lib, "LibMNG-x64.lib")
    #pragma comment(lib, "LibPNG-x64.lib")
    #pragma comment(lib, "LibTIFF-x64.lib")

    #if defined(FINJIN_EXPORTER_ENABLE_D3D_FUNCTIONALITY)
        #pragma comment(lib, "d3d9.lib")
        #pragma comment(lib, "d3dx9.lib")
    #endif
#endif

using namespace Finjin::Exporter;


//Globals-----------------------------------------------------------------------
//static ApplicationConsoleLogListener appConsoleLogListener(INFO_LOG_MESSAGE);
static WxFileLogListener mainLogListener(INFO_LOG_MESSAGE, FINJIN_EXPORTER_LOG_FILE_NAME);
static std::unique_ptr<HelpSystem> helpSystem;
static WxRandomUuidCreator randomUuidCreator;


//Implementation----------------------------------------------------------------
bool ExporterSystem::IsInitialized()
{
    return helpSystem != nullptr;
}

void ExporterSystem::Initialize()
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("ExporterSystem::Initialize()"));

    if (!IsInitialized())
    {
        auto logFileName = FileUtilities::JoinPath(ApplicationAccessor::GetFinjinHomeDirectory(true), FINJIN_EXPORTER_LOG_FILE_NAME);
        if (wxFileExists(logFileName))
            wxRemoveFile(logFileName);

        //Set up logging
        //Logger::AddListener(&appConsoleLogListener);
        Logger::AddListener(&mainLogListener);
        if (FinjinGlobalSettings::GetInstance().detailedLogging || LogListener::DEFAULT_LOG_LEVEL == DEBUG_LOG_MESSAGE)
            EnableDetailedLogging(true, false);

        //Create whatever initial dialogs are needed
        Dialogs::Initialize();

        //Create help system
        auto helpFileName = GetHelpFileName();
        if (wxFileExists(helpFileName))
            helpSystem.reset(new HelpSystem(helpFileName));

        FinjinResourceManager::GetInstance().UpdateGlobalResources();
    }
}

void ExporterSystem::Shutdown()
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("ExporterSystem::Shutdown()"));

    if (IsInitialized())
    {
        //Save global settings. This isn't necessary, but it forces the global settings file to exist
        FinjinGlobalSettings::GetInstance().Save();

        //Destroy any modeless dialogs that happened to be created
        Dialogs::Shutdown();

        //Delete help system
        helpSystem.reset();
    }
}

wxString ExporterSystem::GetHelpFileName()
{
    return ApplicationAccessor::GetApplicationHelpLocation();
}

void ExporterSystem::ShowHelp()
{
    if (helpSystem != nullptr)
        helpSystem->Show();
}

void ExporterSystem::OnBeforeOpenScene()
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("ExporterSystem::OnBeforeOpenScene()"));

    FinjinResourceManager::GetInstance().ClearSceneResources();
}

void ExporterSystem::OnAfterOpenScene()
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("ExporterSystem::OnAfterOpenScene()"));

    RefreshResources();
}

void ExporterSystem::OnSceneReset()
{
    FinjinResourceManager::GetInstance().ClearSceneResources();

    RefreshResources();
}

void ExporterSystem::OnBeforeMerge()
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("ExporterSystem::OnBeforeMerge()"));
}

void ExporterSystem::OnAfterMerge()
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("ExporterSystem::OnAfterMerge()"));
}

void ExporterSystem::EnableDetailedLogging(bool enable, bool saveSetting)
{
    if (saveSetting)
    {
        FinjinGlobalSettings::GetInstance().detailedLogging = enable;
        FinjinGlobalSettings::GetInstance().Save();
    }

    Logger::SetLogLevel(enable ? DEBUG_LOG_MESSAGE : INFO_LOG_MESSAGE);
}

void ExporterSystem::RunExternalViewerWithLastExportedFile()
{
    auto& fileName = Exporter::GetLastFileName();
    if (!fileName.empty())
        RunExternalViewer(fileName);
    else
        Dialogs::ShowMessage(Strings::NO_EXPORTED_FILE_TO_VIEW, Strings::FINJIN_VIEWER_ERROR, wxINFORMATION_DIALOG_FLAGS);
}

void ExporterSystem::RunExternalViewer(const wxString& fileName)
{
    //Make sure that a viewer is configured to run
    //FinjinGlobalSettings& settings = FinjinGlobalSettings::GetInstance();
    wxArrayString commands;
    commands.reserve(50);

    //Executable file name---------
    wxString executableBaseFileName;
    //if (settings.externalViewer == FinjinGlobalSettings::FINJIN_VIEWER)
        executableBaseFileName = wxT("finjin-viewer");
    wxString executableBaseFileNameWithExt = executableBaseFileName + APPLICATION_EXTENSION;

    wxString executableFileName = ApplicationAccessor::GetFinjinHomeDirectory(false);
    executableFileName += executableBaseFileNameWithExt;
#if defined(__WXMAC__)
    //The executable needs to be called directly to prevent the app framework from handling the command line arguments
    executableFileName += wxT("/Contents/MacOS/") + executableBaseFileName;
#endif

    commands.push_back(executableFileName);

    auto sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();

    if (!fileName.empty())
    {
        commands.push_back(wxT("-file"));
        commands.push_back(fileName);
    }

    //Run viewer---------
    ArgsArray argsArray(commands);

    wxString commandString;
    argsArray.ToCommandString(commandString);
    FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, Strings::RUNNING_EXTERNAL_VIEWER_FORMAT, executableBaseFileName.wx_str(), commandString.wx_str());

    wxExecute(argsArray);
}

void ExporterSystem::RefreshResources()
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("ExporterSystem::RefreshResources()"));

    auto sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings(false);
    if (sceneSettings.IsValid())
    {
        //Validate resources
        FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Validating resources"));
        auto validatedResources = FinjinResourceManager::GetInstance().ValidateResourceLocations(ManagedResourceType::ALL, sceneSettings);
        if (AnySet(validatedResources))
        {
            auto resourcesToRefresh = validatedResources;

            //Some resources were validated
            FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Refreshing scene resources"));
            FinjinResourceManager::GetInstance().RefreshSceneResources(resourcesToRefresh, false, sceneSettings);
        }

        //Clear resources that failed to validate
        auto failedResources = ManagedResourceType(~(int)validatedResources);
        FinjinResourceManager::GetInstance().ClearSceneResources(failedResources);
    }
}

void ExporterSystem::SyncSubmeshSettings(const std::vector<ObjectAccessor>& objects)
{
    if (objects.empty())
        return;

    auto sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();
    auto flags = GeometryStateFlags::NONE;
    if (!sceneSettings.GetCreateExtraMaterials())
        flags |= GeometryStateFlags::NO_EXTRA_MATERIALS;

    for (size_t i = 0; i < objects.size(); i++)
    {
        ObjectAccessor object = objects[i];
        if (object.IsValid())
        {
            FinjinObjectSettingsAccessor objectSettings = sceneSettings.GetObjectSettings(object);

            SubmeshesSettings defaultSubmeshesSettings;
            GeometryState::GetSubmeshesSettings(object, defaultSubmeshesSettings, flags);

            SubmeshesSettings currentSubmeshesSettings;
            objectSettings.GetSubmeshesSettings(currentSubmeshesSettings);

            currentSubmeshesSettings.SyncTo(defaultSubmeshesSettings);

            objectSettings.SetSubmeshesSettings(currentSubmeshesSettings);
        }
    }
}
