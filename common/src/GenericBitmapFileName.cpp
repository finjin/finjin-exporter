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
#include "GenericBitmapFileName.hpp"
#include "FileUtilities.hpp"

using namespace Finjin::Exporter;


//Locals-----------------------------------------------------------------------
static const wxChar* SEPARATE_CUBIC_SUFFIXES[] = {wxT("fr"), wxT("bk"), wxT("lf"), wxT("rt"), wxT("up"), wxT("dn")};


//Implementation----------------------------------------------------------------
GenericBitmapFileName::GenericBitmapFileName()
{
    this->type = STANDARD;
    this->frameCount = 0;
}

GenericBitmapFileName::GenericBitmapFileName(const wxString& fileName, Type type)
{
    this->fileName = fileName;
    this->type = type;
    this->frameCount = 0;
}

GenericBitmapFileName::GenericBitmapFileName(const wxString& fileName, int frameCount)
{
    this->fileName = fileName;
    this->type = ANIMATED;
    this->frameCount = frameCount;
}

int GenericBitmapFileName::GetAllFileNamesCount() const
{
    int count = 0;

    if (this->type == STANDARD)
        count = 1;
    else if (this->type == ANIMATED)
        count = this->frameCount;
    else if (this->type == CUBIC)
        count = FINJIN_COUNT_OF(SEPARATE_CUBIC_SUFFIXES);

    return count;
}

void GenericBitmapFileName::GetAllFileNames(std::vector<wxString>& fileNames) const
{
    fileNames.resize(GetAllFileNamesCount());
    if (!fileNames.empty())
    {
        if (this->type == STANDARD)
            fileNames[0] = this->fileName;
        else if (this->type == ANIMATED)
        {
            auto fileNameNoExtension = FileUtilities::RemoveExtension(this->fileName);
            auto extension = FileUtilities::GetExtension(this->fileName, true);
        
            for (int i = 0; i < this->frameCount; i++)
                fileNames[i] = wxString::Format(wxT("%s_%d%s"), fileNameNoExtension.wx_str(), i, extension.wx_str());        
        }
        else if (this->type == CUBIC)
        {
            auto fileNameNoExtension = FileUtilities::RemoveExtension(this->fileName);
            auto extension = FileUtilities::GetExtension(this->fileName, true);

            for (int i = 0; i < FINJIN_COUNT_OF(SEPARATE_CUBIC_SUFFIXES); i++)
                fileNames[i] = wxString::Format(wxT("%s_%s%s"), fileNameNoExtension.wx_str(), SEPARATE_CUBIC_SUFFIXES[i], extension.wx_str());        
        }
    }
}

bool GenericBitmapFileName::operator < (const GenericBitmapFileName& fileName) const
{
    return this->fileName < fileName.fileName;
}
