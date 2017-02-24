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
#if defined(__WXMSW__)
    #include "FileVersionInfo.hpp"
    #define PLUGIN_EXTENSION wxT(".mll")
#elif defined (__WXMAC__)
    #define PLUGIN_EXTENSION wxT(".bundle")
#endif
#include "FileUtilities.hpp"

using namespace Finjin::Exporter;


//Local classes----------------------------------------------------------------
class InstallationItem_MayaPlugin : public PluginInstallationItem
{
public:
    bool CanInstall(const InstallationContext& context) const
    {
        wxString mayaDirectory;
        if (QueryMayaInstallPath(mayaDirectory) && wxDirExists(mayaDirectory))
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
        auto sourceDirectoryName = wxT("exporters/") + FormatDirectoryName(GetApplicationVersion(), context.buildVersion);
        auto sourceFileName = sourceDirectoryName + wxT("/") + GetBaseFileName();

        QueryMayaInstallPath(context.applicationRootDirectory);

        //Install plugin file
        wxString destinationDirectory = context.applicationRootDirectory;
    #if defined (__WXMAC__)
        destinationDirectory += wxT("MacOS/");
    #else
        destinationDirectory += wxT("bin/");
    #endif
        destinationDirectory += wxT("plug-ins/");
        
        if (wxDirExists(destinationDirectory))
        {
            auto destinationFileName = destinationDirectory + GetBaseFileName();
            context.LogFileCopy(sourceFileName, destinationFileName);
        }
        else
            context.LogDoesNotExist(wxT("Maya plugins directory"));

    //#if defined(__WXMSW__)
        //Install Attribute Editor (AE) MEL script files
        destinationDirectory = context.applicationRootDirectory + wxT("scripts/AETemplates/");
        if (wxDirExists(destinationDirectory))
        {
            for (int i = 0; ; i++)
            {
                const wxString& scriptBaseFileName = GetAEScriptBaseFileName(i);
                if (scriptBaseFileName.empty())
                    break;

                sourceFileName = wxT("exporters/mel/") + scriptBaseFileName;
                auto destinationFileName = destinationDirectory + scriptBaseFileName;
                context.LogFileCopy(sourceFileName, destinationFileName);
            }            
        }
        else
            context.LogDoesNotExist(wxT("Maya scripts/AETemplates directory"));
        
        //Install icon files
        destinationDirectory = context.applicationRootDirectory + wxT("icons/");
        if (wxDirExists(destinationDirectory))
        {
            for (int i = 0; ; i++)
            {
                auto& iconBaseFileName = GetIconBaseFileName(i);
                if (iconBaseFileName.empty())
                    break;

                sourceFileName = wxT("exporters/Icons/") + iconBaseFileName;
                auto destinationFileName = destinationDirectory + iconBaseFileName;
                context.LogFileCopy(sourceFileName, destinationFileName);
            }            
        }
        else
            context.LogDoesNotExist(wxT("Maya icons directory"));
    //#endif
    }

    void Uninstall(InstallationContext& context)
    {
        if (QueryMayaInstallPath(context.applicationRootDirectory) && wxDirExists(context.applicationRootDirectory))
        {
            //Delete plugin file
            auto destinationFileName = context.applicationRootDirectory;
        #if defined (__WXMAC__)
            destinationFileName += wxT("MacOS/");
        #else
            destinationFileName += wxT("bin/");
        #endif
            destinationFileName += wxT("plug-ins/") + GetBaseFileName();
            
            context.LogOptionalFileDelete(destinationFileName);
            
        //#if defined(__WXMSW__)
            //Delete Attribute Editor (AE) MEL script files
            for (int i = 0; ; i++)
            {
                auto& scriptBaseFileName = GetAEScriptBaseFileName(i);
                if (scriptBaseFileName.empty())
                    break;

                destinationFileName = context.applicationRootDirectory + wxT("scripts/AETemplates/") + scriptBaseFileName;
                context.LogOptionalFileDelete(destinationFileName);
            }

            //Delete icon files
            for (int i = 0; ; i++)
            {
                auto& iconBaseFileName = GetIconBaseFileName(i);
                if (iconBaseFileName.empty())
                    break;

                destinationFileName = context.applicationRootDirectory + wxT("icons/") + iconBaseFileName;
                context.LogOptionalFileDelete(destinationFileName);
            }

            //Delete log/settings directory
            auto appDirectory = wxT("finjin-exporter/") + GetApplicationVersion().directoryName + (wxT("-") APPLICATION_PLATFORM);
            auto localSettingsDirectory = FileUtilities::JoinPath(wxStandardPaths::Get().GetUserLocalDataDir(), appDirectory);
            if (wxDirExists(localSettingsDirectory))
            {
                context.LogDirectoryDelete(localSettingsDirectory);
            }
        //#endif
        }
    }

protected:
    virtual int GetMayaVersionNumber() const = 0;

    static const wxString& GetBaseFileName()
    {
        static const wxString name(wxT("finjin-exporter") PLUGIN_EXTENSION);
        return name;
    }

    static const wxString& GetAEScriptBaseFileName(int i)
    {
        static const wxString names[] = 
            {
            wxT("AEFinjinCustomDummyTemplate.mel"),
            wxT("AEFinjinStaticGeometryTemplate.mel"),
            wxT("AEFinjinInstancedGeometryTemplate.mel"),
            wxT("FinjinFunctions.mel"),
            wxEmptyString
            };
        return names[i];
    }

    static const wxString& GetIconBaseFileName(int i)
    {
        static const wxString names[] = 
            {
            wxEmptyString
            };
        return names[i];
    }

    bool QueryMayaInstallPath(wxString& installPath) const
    {
        //Clear out install path
        installPath.clear();

    #if defined(__WXMSW__)
        //Format the appropriate version key
        wxString key;
        key << wxT("Software\\Autodesk\\Maya\\") << GetMayaVersionNumber() << wxT("\\Setup\\InstallPath");

        //Open the registry key
        HKEY hKey = nullptr;
        auto result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, key.wx_str(), 0, KEY_READ, &hKey);
        if (result == ERROR_SUCCESS)
        {
            //Query the installation directory value
            TCHAR path[1024];
            DWORD dataType = REG_SZ;
            DWORD dataSize = sizeof(path);
            result = RegQueryValueEx(hKey, wxT("MAYA_INSTALL_LOCATION"), nullptr, &dataType, (BYTE*)path, &dataSize);
            if (result == ERROR_SUCCESS)
            {
                //Success
                installPath = path;
            }

            //Close key
            RegCloseKey(hKey);
        }
    #elif defined (__WXMAC__)
        //Format the appropriate versioned location
        installPath << wxT("/Applications/Autodesk/maya") << GetMayaVersionNumber() << wxT("/Maya.app/Contents/");
    #endif

        return !installPath.empty();
    }
};

class InstallationItem_Maya2015 : public InstallationItem_MayaPlugin
{
public:
    const ApplicationVersion& GetApplicationVersion() const
    {
        static const ApplicationVersion appVersion = {wxT("Maya 2015"), wxT("maya2015")};
        return appVersion;
    }

    int GetMayaVersionNumber() const
    {
        return 2015;
    }
};

class InstallationItem_Maya2016 : public InstallationItem_MayaPlugin
{
public:
    const ApplicationVersion& GetApplicationVersion() const
    {
        static const ApplicationVersion appVersion = {wxT("Maya 2016"), wxT("maya2016")};
        return appVersion;
    }

    int GetMayaVersionNumber() const
    {
        return 2016;
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
            bool required;
            wxString fileName = GetSourceFileName(index, &required);
            if (fileName.empty())
                break;
            if (required && !wxFileExists(fileName) && !wxDirExists(fileName))
                return false;
        }
        return true;
    }
    
    void Install(InstallationContext& context)
    {
        wxString binDirectory = context.applicationRootDirectory + wxT("bin/");

        for (int index = 0; ; index++)
        {
            wxString sourceFileName = GetSourceFileName(index);        
            if (sourceFileName.empty())
                break;
            
            if (wxFileExists(sourceFileName) || wxDirExists(sourceFileName))
            {
                wxString destinationFileName = binDirectory + GetBaseFileName(index);
                
            #if defined (__WXMAC__)
                //Copy application directory
                context.LogDirectoryCopy(sourceFileName, destinationFileName);
            #else
                //Copy application file
                context.LogFileCopy(sourceFileName, destinationFileName);
            #endif
            }
        }
    }

    void Uninstall(InstallationContext& context)
    {
        wxString binDirectory = context.applicationRootDirectory + wxT("bin/");

        for (int index = 0; ; index++)
        {
            const wxString& sourceFileName = GetBaseFileName(index);        
            if (sourceFileName.empty())
                break;
            
            wxString destinationFileName = binDirectory + sourceFileName;
            
        #if defined (__WXMAC__)
            //Delete application directory
            context.LogOptionalDirectoryDelete(destinationFileName);
        #else
            //Delete application file
            context.LogOptionalFileDelete(destinationFileName);
        #endif
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
            fileName += wxT("/") APPLICATION_PLATFORM wxT("/");
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
        SetApplicationName(wxT("Maya"));
        SetExtraSuccessMessage(wxT("If this is your first time installing Finjin you must:\n\n     1) Run Maya.\n\n     2) Open the 'Window->Settings/Preferences->Plug-in Manager' window.\n\n     3) Find 'FinjinExporter") PLUGIN_EXTENSION wxT("' and check 'Loaded' and 'Auto Load'."), InstallationMode::INSTALL);

#if defined(__WXMSW__)
    #ifdef _WIN64
        AddPluginInstallationItem(new InstallationItem_Maya2016);
        AddPluginInstallationItem(new InstallationItem_Maya2015);
    #endif

        AddOtherInstallationItem(new InstallationItem_Viewers);
#elif defined (__WXMAC__)
        AddPluginInstallationItem(new InstallationItem_Maya2016);
        AddPluginInstallationItem(new InstallationItem_Maya2015);
        
        AddOtherInstallationItem(new InstallationItem_Viewers);
#endif

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
