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
#include "InstallationDialogBase.hpp"
#include "SystemUtilities.hpp"
#include "FileVersionInfo.hpp"
#include "FileUtilities.hpp"

using namespace Finjin::Exporter;


//Local classes----------------------------------------------------------------
class InstallationItem_3DSMaxPlugin : public PluginInstallationItem
{
public:
    InstallationItem_3DSMaxPlugin(bool isStandard = true)
    {
        this->isStandard = isStandard;
    }

    bool CanInstall(const InstallationContext& context) const
    {
        wxString maxDirectory;
        if (QueryMaxInstallPath(maxDirectory) && wxDirExists(maxDirectory))
        {
            auto sourceDirectoryName = wxT("exporters/") + FormatDirectoryName(GetApplicationVersion(), context.buildVersion);
            if (wxDirExists(sourceDirectoryName))
            {
                auto sourceFileName = sourceDirectoryName + wxT("/") + GetBaseFileName();

                return wxFileExists(sourceFileName);
            }
        }
        return false;
    }

    void Install(InstallationContext& context)
    {
        QueryMaxInstallPath(context.applicationRootDirectory);

        auto destinationDirectory = context.applicationRootDirectory + wxT("plugins/");
        if (wxDirExists(destinationDirectory))
        {
            auto sourceDirectoryName = wxT("exporters/") + FormatDirectoryName(GetApplicationVersion(), context.buildVersion);
            auto sourceFileName = sourceDirectoryName + wxT("/") + GetBaseFileName();

            auto destinationFileName = destinationDirectory + GetBaseFileName();
            context.LogFileCopy(sourceFileName, destinationFileName);
        }
        else
            context.LogDoesNotExist(wxT("3DS Max plugins directory"));
    }

    void Uninstall(InstallationContext& context)
    {
        if (QueryMaxInstallPath(context.applicationRootDirectory) && wxDirExists(context.applicationRootDirectory))
        {
            auto destinationDirectory = context.applicationRootDirectory + wxT("plugins/");
            if (wxDirExists(destinationDirectory))
            {
                //Tell user what's about to happen
                auto message = wxString::Format(wxT("%s will be started as part of the uninstall process.\n\nAfter %s starts, close it."), GetApplicationVersion().displayName.wx_str(), GetApplicationVersion().displayName.wx_str());
                wxMessageDialog dialog(context.mainWindow, message, wxT("Finjin Uninstaller"), wxOK | wxCANCEL | wxICON_INFORMATION | wxCENTER);
                if (dialog.ShowModal() == wxID_OK)
                {
                    //Create uninstall file
                    auto uninstallTextFileName = context.applicationRootDirectory + wxT("finjin-exporter-uninstall.txt");
                    FileUtilities::WriteTextFile(uninstallTextFileName, wxEmptyString);

                    //Launch 3DS Max
                    auto command = wxT("\"") + context.applicationRootDirectory + wxT("3dsmax.exe") + wxT("\"");
                    if (context.LogExec(command, false))
                    {
                        //Delete uninstall file if Finjin plugin didn't delete it
                        if (wxFileExists(uninstallTextFileName))
                            wxRemoveFile(uninstallTextFileName);

                        //Delete plugin file
                        auto destinationFileName = destinationDirectory + GetBaseFileName();
                        context.LogOptionalFileDelete(destinationFileName);
                    }
                }
                else
                    context.LogFailure(wxString::Format(wxT("You must allow %s to be started."), GetApplicationVersion().displayName.wx_str()));
            }
        }       

        //Delete log/settings directory
        auto appDirectory = wxT("FinjinExporter/3DS") + GetApplicationVersion().directoryName + (wxT("-") APPLICATION_PLATFORM);
        auto localSettingsDirectory = FileUtilities::JoinPath(wxStandardPaths::Get().GetUserLocalDataDir(), appDirectory);
        if (wxDirExists(localSettingsDirectory))
        {
            context.LogDirectoryDelete(localSettingsDirectory);
        }
    }

protected:
    virtual int GetMaxVersionNumber() const = 0;    

    const wxString& GetMaxSubkey() const
    {
        static const wxString standardMax = wxT("3dsMax");
        static const wxString designMax = wxT("3dsMaxDesign");
        return this->isStandard ? standardMax : designMax;
    }

    static const wxString& GetBaseFileName()
    {
        static const wxString name(wxT("finjin-exporter.gup"));
        return name;
    }

    bool QueryMaxInstallPath(wxString& installPath) const
    {
        static const int FIRST_LOCALIZED_VERSION = 9; //Max 9 is the start of localized installs
        static const int LAST_LOCALIZED_VERSION = 14; //Max 14 (2011) is the end of localized installs

        //Clear out install path
        installPath.clear();

        //Find the 3DS Max key
        wxString key = wxT("Software\\Autodesk\\");
        wxString maxSubkey = SystemUtilities::FindSubkey(key, GetMaxSubkey());
        if (!maxSubkey.empty())
        {
            key << maxSubkey << wxT("\\") << GetMaxVersionNumber() << wxT(".0");
        
            //There may be a localized subkey containing the information
            wxString subKey;
            if (GetMaxVersionNumber() >= FIRST_LOCALIZED_VERSION)
                subKey = SystemUtilities::FindSubkeyStartingWith(key, wxT("MAX"));

            if (GetMaxVersionNumber() < FIRST_LOCALIZED_VERSION || 
                GetMaxVersionNumber() > LAST_LOCALIZED_VERSION || 
                !subKey.empty())
            {
                if (!subKey.empty())
                    key << wxT("\\") << subKey;

                //Open the registry key
                HKEY hKey = nullptr;
                auto result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, key.wx_str(), 0, KEY_READ, &hKey);
                if (result == ERROR_SUCCESS)
                {
                    //Query the installation directory value
                    TCHAR path[1024];
                    DWORD dataType = REG_SZ;
                    DWORD dataSize = sizeof(path);
                    result = RegQueryValueEx(hKey, wxT("Installdir"), nullptr, &dataType, (BYTE*)path, &dataSize);
                    if (result == ERROR_SUCCESS)
                    {
                        //Success
                        installPath = path;
                    }
                    
                    //Close key
                    RegCloseKey(hKey);
                }
            }
        }

        return !installPath.empty();
    }

protected:
    bool isStandard;
};

class InstallationItem_3DSMax2015 : public InstallationItem_3DSMaxPlugin
{
public:
    InstallationItem_3DSMax2015(bool isStandard = true) : InstallationItem_3DSMaxPlugin(isStandard)
    {
    }

    const ApplicationVersion& GetApplicationVersion() const
    {
        static const ApplicationVersion standardAppVersion = {wxT("3DS Max 2015"), wxT("max2015")};
        static const ApplicationVersion designAppVersion = {wxT("3DS Max Design 2015"), wxT("max2015")};
        return this->isStandard ? standardAppVersion : designAppVersion;
    }

    int GetMaxVersionNumber() const
    {
        return 17;
    }
};

class InstallationItem_3DSMax2016 : public InstallationItem_3DSMaxPlugin
{
public:
    InstallationItem_3DSMax2016() : InstallationItem_3DSMaxPlugin(true)
    {
    }

    const ApplicationVersion& GetApplicationVersion() const
    {
        static const ApplicationVersion standardAppVersion = {wxT("3DS Max 2016"), wxT("max2016")};
        return standardAppVersion;
    }

    int GetMaxVersionNumber() const
    {
        return 18;
    }
};

class InstallationItem_Viewers : public OtherInstallationItem
{
public:
    wxString GetDisplayName() const
    {
        static const wxString name = wxT("Finjin Viewer");
        return name;
    }

    bool CanInstall(const InstallationContext& context) const
    {
        for (int index = 0; ; index++)
        {
            auto required = false;
            auto fileName = GetSourceFileName(index, &required);
            if (fileName.empty())
                break;
            if (required && !wxFileExists(fileName))
                return false;
        }
        return true;
    }
    
    void Install(InstallationContext& context)
    {
        for (int index = 0; ; index++)
        {
            auto sourceFileName = GetSourceFileName(index);        
            if (sourceFileName.empty())
                break;
            
            if (wxFileExists(sourceFileName))
            {
                auto destinationFileName = context.applicationRootDirectory + GetBaseFileName(index);
                context.LogFileCopy(sourceFileName, destinationFileName);
            }
        }
    }

    void Uninstall(InstallationContext& context)
    {
        for (int index = 0; ; index++)
        {
            auto sourceFileName = GetBaseFileName(index);        
            if (sourceFileName.empty())
                break;
            
            //Delete application file
            auto destinationFileName = context.applicationRootDirectory + sourceFileName;
            context.LogOptionalFileDelete(destinationFileName);

            //Delete log and settings files
            if (destinationFileName.EndsWith(APPLICATION_EXTENSION))
            {
                auto logFileName = destinationFileName;
                logFileName.Replace(APPLICATION_EXTENSION, wxT(".log"));
                context.LogOptionalFileDelete(logFileName);

                auto settingsFileName = destinationFileName;
                settingsFileName.Replace(APPLICATION_EXTENSION, wxT("-settings.cfg"));
                context.LogOptionalFileDelete(settingsFileName);
            }
        }
    }

private:
    static wxString GetBaseFileName(int index)
    {
        static const wxString fileNames[] = 
            {
            wxT("finjin-viewer") APPLICATION_EXTENSION,
            wxEmptyString
            };    
        return fileNames[index];
    }

    static wxString GetSourceFileName(int index, bool* required = nullptr)
    {
        static const wxString fileNames[] = 
            {
            wxT("finjin-viewer"),
            wxEmptyString
            };    

        if (required != nullptr)
            *required = index == 0;

        wxString fileName;

        if (!fileNames[index].empty())
        {
            fileName = wxT("viewers/");
            fileName += fileNames[index];
            fileName += wxT("/");

        #ifdef _WIN64
            fileName += wxT("x64/");
        #else
            fileName += wxT("Win32/");
        #endif
            fileName += fileNames[index];
            fileName += APPLICATION_EXTENSION;
        }

        return fileName;
    }
};

class InstallationDialog : public InstallationDialogBase
{
public:
    InstallationDialog()
    {
        SetApplicationName(wxT("3DS Max"));

    #ifdef _WIN64
        AddPluginInstallationItem(new InstallationItem_3DSMax2016);
        AddPluginInstallationItem(new InstallationItem_3DSMax2015);
        AddPluginInstallationItem(new InstallationItem_3DSMax2015(false));        
    #endif
        
        AddOtherInstallationItem(new InstallationItem_Viewers);

        Create();
    }
};


//Entry point------------------------------------------------------------------
class InstallationApp : public wxApp
{
public:
    int OnRun();
};
IMPLEMENT_APP(InstallationApp)

int InstallationApp::OnRun()
{
    InstallationDialog dialog;
    dialog.ShowModal();
    return 0;
}
