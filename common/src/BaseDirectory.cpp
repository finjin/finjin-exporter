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
#include "BaseDirectory.hpp"
#include "FileUtilities.hpp"
#include "StringUtilities.hpp"
#include "ApplicationAccessor.hpp"
#include "Strings.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
const int BaseDirectory::APPLICATION_DIRECTORY_CHOICE = 0;
const int BaseDirectory::PROJECT_DIRECTORY_CHOICE = 1;
const int BaseDirectory::CUSTOM_DIRECTORY_CHOICE = 2;


//Implementation----------------------------------------------------------------
BaseDirectory::BaseDirectory()
{
    this->type = APPLICATION;
    Init();
}

BaseDirectory::BaseDirectory(Type type, const wxString& customDirectory)
{
    this->type = type;
    this->customDirectory = customDirectory;
    if (this->type == CUSTOM && this->customDirectory.empty())
        this->type = APPLICATION;
    Init();
}

BaseDirectory::BaseDirectory(const wxString& customDirectory)
{
    this->type = !customDirectory.empty() ? CUSTOM : APPLICATION;
    this->customDirectory = customDirectory;
    Init();
}

BaseDirectory::BaseDirectory(wxChoice* baseDirectoryTypeChoice, ApplicationTextCtrl* baseDirectoryText)
{
    int baseDirectoryChoiceIndex = std::max(0, baseDirectoryTypeChoice->GetCurrentSelection());
    int baseDirectoryChoice = *(int*)baseDirectoryTypeChoice->GetClientData(baseDirectoryChoiceIndex);
    switch (baseDirectoryChoice)
    {
        case APPLICATION_DIRECTORY_CHOICE:
        {
            this->type = APPLICATION;
            break;
        }
        case PROJECT_DIRECTORY_CHOICE:
        {
            this->type = PROJECT;
            break;
        }
        case CUSTOM_DIRECTORY_CHOICE:
        {
            this->customDirectory = baseDirectoryText->GetValue();
            this->type = !this->customDirectory.empty() ? CUSTOM : APPLICATION;
            break;
        }
    }

    Init();
}

const wxString& BaseDirectory::GetCustomDirectory() const
{
    return this->customDirectory;
}

const wxString& BaseDirectory::GetBaseDirectory() const
{
    return this->baseDirectory;
}

void BaseDirectory::GetGUIData(Type& type, wxString& directory)
{
    type = GetSupportedType();
    directory = GetBaseDirectory();
}

void BaseDirectory::SetGUIData(wxChoice* baseDirectoryTypeChoice, ApplicationTextCtrl* baseDirectoryText)
{
    //Base directory type choice-------------

    //Add items
    baseDirectoryTypeChoice->Freeze();
    baseDirectoryTypeChoice->Clear();
    int applicationDirectoryIndex = baseDirectoryTypeChoice->Append(ApplicationAccessor::APPLICATION_SHORT_NAME);
    baseDirectoryTypeChoice->SetClientData(applicationDirectoryIndex, (void*)&APPLICATION_DIRECTORY_CHOICE);
    int projectDirectoryIndex = -1;
    if (ApplicationAccessor::SupportsProjects())
    {
        projectDirectoryIndex = baseDirectoryTypeChoice->Append(Strings::PROJECT);
        baseDirectoryTypeChoice->SetClientData(projectDirectoryIndex, (void*)&PROJECT_DIRECTORY_CHOICE);
    }
    int customDirectoryIndex = baseDirectoryTypeChoice->Append(Strings::CUSTOM);
    baseDirectoryTypeChoice->SetClientData(customDirectoryIndex, (void*)&CUSTOM_DIRECTORY_CHOICE);
    baseDirectoryTypeChoice->Thaw();

    //Select appropriate item
    auto supportedType = GetSupportedType();
    if (supportedType == APPLICATION || supportedType == CUSTOM || projectDirectoryIndex < 0)
    {
        if (supportedType == CUSTOM)
            baseDirectoryTypeChoice->SetSelection(customDirectoryIndex);
        else
            baseDirectoryTypeChoice->SetSelection(applicationDirectoryIndex);
    }
    else if (supportedType == PROJECT)
    {
        baseDirectoryTypeChoice->SetSelection(projectDirectoryIndex);
    }

    //Base directory edit--------------------------
    baseDirectoryText->SetValue(this->baseDirectory);
}

int BaseDirectory::GetBaseDirectoryTypeChoice(wxChoice* baseDirectoryTypeChoice)
{
    return *(int*)baseDirectoryTypeChoice->GetClientData(baseDirectoryTypeChoice->GetSelection());
}

wxString BaseDirectory::ToString(Type type)
{
    switch (type)
    {
        case APPLICATION: return wxT("application"); break;
        case CUSTOM: return wxT("custom"); break;
        case PROJECT: return wxT("project"); break;
    }
    return wxT("");
}

BaseDirectory::Type BaseDirectory::ParseType(const wxString& s)
{
    if (s == wxT("application"))
        return APPLICATION;
    else if (s == wxT("custom"))
        return CUSTOM;
    else
        return PROJECT;
}

void BaseDirectory::Init()
{
    StringUtilities::TrimWhitespace(this->customDirectory);

    switch (GetSupportedType())
    {
        case APPLICATION:
        {
            this->baseDirectory = ApplicationAccessor::GetApplicationDirectory();
            break;
        }
        case CUSTOM:
        {
            this->baseDirectory = this->customDirectory;
            break;
        }
        case PROJECT:
        {
            this->baseDirectory = ApplicationAccessor::GetCurrentProjectDirectory();
            break;
        }
    }

    if (!this->baseDirectory.empty())
        FileUtilities::EnsureTrailingPathSeparator(this->baseDirectory);
}

BaseDirectory::Type BaseDirectory::GetSupportedType() const
{
    if (ApplicationAccessor::SupportsProjects())
    {
        //All types are supported
        return (this->type == CUSTOM && !this->customDirectory.empty()) ? CUSTOM : this->type;
    }
    else
    {
        //PROJECT is not supported
        if (this->type == PROJECT)
            return APPLICATION;
        else
            return (this->type == CUSTOM && !this->customDirectory.empty()) ? CUSTOM : this->type;
    }
}
