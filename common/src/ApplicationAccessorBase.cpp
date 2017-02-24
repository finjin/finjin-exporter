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
#include "ApplicationAccessorBase.hpp"
#include "FileUtilities.hpp"
#include "StringUtilities.hpp"
#include "Strings.hpp"
#include "RegisterSceneSettingsTypes.hpp"
#include "RegisterDetectableObjectTypes.hpp"
#include "RegisterExporterHandlerFactories.hpp"
#include "RegisterMaterialExporterHandlerFactories.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
bool ApplicationAccessorBase::exportEnabled = false;


//Implementation---------------------------------------------------------------
wxString ApplicationAccessorBase::GetUserHomeApplicationDirectory()
{
    wxString appName = wxT("FinjinExporter/");
    appName += APP_VERSION_NAME;
    
    wxString dir = FileUtilities::JoinPath(wxStandardPaths::Get().GetUserLocalDataDir(), appName);
    FileUtilities::EnsureTrailingPathSeparator(dir);
    FileUtilities::CreateFileDirectoryRecursive(dir);
    return dir;
}

void ApplicationAccessorBase::RegisterTypes()
{
    RegisterSceneSettingsTypes();
    RegisterDetectableObjectTypes();    
    RegisterExporterHandlerFactories();
    RegisterMaterialExporterHandlerFactories();
}

wxString ApplicationAccessorBase::GetSceneAuthor()
{
    return wxEmptyString;
}

wxString ApplicationAccessorBase::GetLastAuthor()
{
    return wxEmptyString;
}

bool ApplicationAccessorBase::SupportsProjects()
{
    return true;
}

bool ApplicationAccessorBase::SupportsNoMaterialAssignment()
{
    return true;
}

bool ApplicationAccessorBase::NeedsExtraMaterials()
{
    return false;
}

bool ApplicationAccessorBase::Supports3DTextureCoordinates()
{
    return true;
}

bool ApplicationAccessorBase::SupportsNoteTracks()
{
    return false;
}

bool ApplicationAccessorBase::SupportsMaterialStructureAndColorChangeNotifications()
{
    return true;
}

bool ApplicationAccessorBase::GetStaticTextForegroundColor(wxColor& color)
{
    return false;
}

float ApplicationAccessorBase::GetInternalToUIScale()
{
    return 1;
}

wxString ApplicationAccessorBase::FormatWorldValue(float value)
{
    return wxString::Format(wxT("%.2f"), value);
}

void ApplicationAccessorBase::RegisterModelessDialog(wxWindow* window)
{
}

void ApplicationAccessorBase::UnregisterModelessDialog(wxWindow* window)
{
}

bool ApplicationAccessorBase::UsesFullNames()
{
    return true;
}

wxString ApplicationAccessorBase::GetApplicationHelpLocation()
{
    return wxEmptyString;
}

void ApplicationAccessorBase::EnableShortcutKeys(bool enable)
{
}

void ApplicationAccessorBase::EnableExport(bool enable)
{
    exportEnabled = enable;
}

bool ApplicationAccessorBase::IsExportEnabled()
{
    return exportEnabled;
}

bool ApplicationAccessorBase::InQuietMode()
{
    return false;
}
