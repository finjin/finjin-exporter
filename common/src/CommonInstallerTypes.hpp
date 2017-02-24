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


#pragma once


//Includes---------------------------------------------------------------------\
//C RunTime Header Files
#include <stdlib.h>
#include <memory.h>

//Standard library includes
#include <list>
#include <vector>
#include <string>
#include <set>
#include <memory>

//wxWidgets includes
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/statbox.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/msgdlg.h>
#include <wx/frame.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/filename.h>
#include <wx/filesys.h>
#include <wx/panel.h>
#include <wx/event.h>
#include <wx/control.h>
#include <wx/radiobut.h>
#include <wx/app.h>    
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/mimetype.h>
#include <wx/stdpaths.h>
#include <wx/tokenzr.h>


//Types------------------------------------------------------------------------
enum class InstallationMode
{
    INSTALL,
    UNINSTALL,

    COUNT
};

struct ApplicationVersion
{
    wxString displayName;
    wxString directoryName;
};

struct BuildVersion
{
    BuildVersion()
    {
    #if defined(_WIN64) || defined(__LP64__)
        this->directoryName = wxT("x64");                
    #elif defined(__WXMSW__)
        this->directoryName = wxT("Win32");
    #else
        this->directoryName = wxT("x86");
    #endif
    }

    wxString directoryName;
};

struct FileVersion
{
    wxString displayName;
    wxString fileBaseName;
};

inline wxString FormatDirectoryName
    (
    const ApplicationVersion& appVersion, 
    const BuildVersion& buildVersion
    )
{
    return wxString::Format
        (
        wxT("%s-%s"), 
        appVersion.directoryName.wx_str(), 
        buildVersion.directoryName.wx_str()
        );
}

inline wxString FormatFileName
    (
    const FileVersion& fileVersion, 
    const BuildVersion& buildVersion
    )
{
    wxFileName fileNameParts(fileVersion.fileBaseName);
    auto ext = fileNameParts.GetExt();
    auto name = fileNameParts.GetName();

    return wxString::Format
        (
        wxT("%s-%s.%s"), 
        name.wx_str(), 
        buildVersion.directoryName.wx_str(), 
        ext.wx_str()
        );
}

#if defined(__WXMSW__)
    #define APPLICATION_EXTENSION wxT(".exe")
    #define SHARED_LIBRARY_EXTENSION wxT(".dll")
#elif defined(__WXMAC__)
    #define APPLICATION_EXTENSION wxT(".app")
    #define SHARED_LIBRARY_EXTENSION wxT(".dylib")
#else
    #define APPLICATION_EXTENSION wxT("")
    #define SHARED_LIBRARY_EXTENSION wxT(".dso")
#endif

//Used in some files to determine whether the code is being used with the exporter install projects
#define FINJIN_EXPORTER_INSTALLER 1
