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
#include "RelativeLocationPrompt.hpp"
#include "SuspendCustomControlsHook.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
RelativeLocationPrompt::RelativeLocationPrompt
    (
    wxWindow* parent,
    const wxString baseDirectory,
    const wxString& promptTitle,
    const wxString& filter,
    long style
    )
{
    this->parent = parent;
    this->baseDirectory = baseDirectory;
    this->promptTitle = promptTitle;
    this->locationType = FILE_LOCATION;
    this->filter = filter;    
    this->style = style;
}

RelativeLocationPrompt::RelativeLocationPrompt
    (
    wxWindow* parent,
    const wxString baseDirectory,
    const wxString& promptTitle,
    long style
    )
{
    this->parent = parent;
    this->baseDirectory = baseDirectory;
    this->promptTitle = promptTitle;
    this->locationType = DIRECTORY_LOCATION;
    this->filter = filter;    
    this->style = style;
}

wxString RelativeLocationPrompt::ShowPrompt()
{
    SUSPEND_CUSTOM_CONTROLS;
    
    wxString path;
    if (this->locationType == FILE_LOCATION)
    {
        wxFileDialog dialog(this->parent, this->promptTitle, wxEmptyString, wxEmptyString, this->filter, this->style);
        if (dialog.ShowModal() == wxID_OK)
            path = dialog.GetPath();
    }
    else if (this->locationType == DIRECTORY_LOCATION)
    {   
        wxDirDialog dialog(this->parent, this->promptTitle, wxEmptyString, this->style);        
        if (dialog.ShowModal() == wxID_OK)
            path = dialog.GetPath();
    }

    if (!path.empty() && !this->baseDirectory.empty())
    {
        //Get selected file name relative to current base directory
        wxFileName fileName(path);
        fileName.MakeRelativeTo(this->baseDirectory);
        path = fileName.GetFullPath();
    }

    return path;
}
