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
#include "FileFinder.hpp"
#include "FileUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
void FileFinder::AddDirectory(const wxString& directory)
{
    this->directories.push_back(directory);
}

wxString FileFinder::FindFile(const wxString& fileName)
{
    wxString baseName = FileUtilities::GetFileName(fileName);
    wxString result;

    for (std::list<wxString>::iterator directoryIterator = this->directories.begin();
        directoryIterator != this->directories.end();
        ++directoryIterator)
    {
        const wxString& directory = *directoryIterator;
        result = FindFileInDirectory(baseName, directory);
        if (!result.empty())
            break;
    }

    return result;
}

wxString FileFinder::FindFileInDirectory(const wxString& baseName, const wxString& directory)
{
    wxArrayString files;
    wxDir::GetAllFiles(directory, &files, baseName, wxDIR_FILES | wxDIR_DIRS | wxDIR_HIDDEN);

    wxString result;
    if (!files.empty())
        result = files[0];

    return result;
}
