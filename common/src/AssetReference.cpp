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
#include "AssetReference.hpp"
#include "SimpleUri.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
bool AssetReference::IsValid() const
{
    return !this->filePath.empty() || !this->objectName.empty();
}

AssetReference AssetReference::Parse(const wxString& s)
{
    AssetReference result;

    if (!s.empty())
    {
        wxURI uri(s);

        if (uri.GetScheme() == wxT("file"))
        {
            result.filePath = uri.GetPath();
            result.objectName = uri.GetFragment();
        }
        else if (uri.GetScheme() == wxT("object"))
        {
            result.objectName = uri.GetPath();
        }
    }

    return result;
}

AssetReference AssetReference::ForObject(const wxString& objectName)
{
    AssetReference result;
    result.objectName = objectName;
    return result;
}

AssetReference AssetReference::ForLocalFile(const wxString& filePath)
{
    AssetReference result;
    result.filePath = filePath;
    return result;
}

AssetReference AssetReference::ForObjectInLocalFile(const wxString& filePath, const wxString& objectName)
{
    AssetReference result;
    result.filePath = filePath;
    result.objectName = objectName;
    return result;
}

wxString AssetReference::ToUriString() const
{
    SimpleUri result;
    if (!this->filePath.empty())
    {
        result.SetScheme(wxT("file"));
        if (!this->filePath.empty())
        {
            if (this->filePath.StartsWith(wxT("/")))
                result.SetPath(this->filePath);
            else
                result.SetPath(wxT("/") + this->filePath);
        }
        if (!this->objectName.empty())
            result.SetFragment(this->objectName);
    }
    else if (!this->objectName.empty())
    {
        result.SetScheme(wxT("object"));
        result.SetPath(this->objectName);
    }
    return result.ToString();
}
