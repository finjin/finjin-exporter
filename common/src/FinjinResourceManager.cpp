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
#include "FinjinResourceManager.hpp"
#include "FileUtilities.hpp"
#include "StringUtilities.hpp"
#include "Dialogs.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "FinjinGlobalSettings.hpp"
#include "Strings.hpp"
#include "PropertyValues.hpp"
#include "MacroExpander.hpp"
#include "ApplicationAccessor.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
FinjinResourceManager& FinjinResourceManager::GetInstance()
{
    static FinjinResourceManager* instance;
    if (instance == nullptr)
        instance = new FinjinResourceManager;
    return *instance;
}

void FinjinResourceManager::AddUserDataTypesChangedListener(UserDataTypesChangedListener* listener)
{
    this->userDataTypesChangedListeners.push_back(listener);
}

void FinjinResourceManager::RemoveUserDataTypesChangedListener(UserDataTypesChangedListener* listener)
{
    this->userDataTypesChangedListeners.remove(listener);
}

void FinjinResourceManager::UpdateGlobalResources()
{
    UpdateVertexFormats();
    UpdateUserDataTypes();
}

void FinjinResourceManager::UpdateVertexFormats()
{
    this->vertexFormats.clear();

    auto& globalSettings = FinjinGlobalSettings::GetInstance();
    if (!globalSettings.vertexFormatPath.empty())
    {
        auto path = globalSettings.GetWorkingVertexFormatFilePath();
        
        WxByteBuffer bytes;
        if (FileUtilities::ReadBinaryFile(path, bytes))
        {
            FINJIN_WX_DECLARE_ERROR(error);
            WxGpuVertexFormatStruct::Initialize(this->vertexFormats, bytes, 1, std::numeric_limits<size_t>::max(), 1, 0, error);

            if (error)
            {
                ApplicationAccessor::LogMessage(wxT("There was an error parsing vertex formats."));
            }
        }
    }
}

void FinjinResourceManager::UpdateUserDataTypes()
{
    NotifyUserDataTypesChangingListeners();

    auto& globalSettings = FinjinGlobalSettings::GetInstance();
    auto baseDirectory = globalSettings.GetWorkingBaseUserDataTypesDirectory();

    this->userDataTypes.Clear();
    auto userDataTypesLocationCount = globalSettings.userDataTypesLocations.size();
    wxString name;
    wxString location;
    MacroExpander pathExpander;
    for (size_t i = 0; i < userDataTypesLocationCount; i++)
    {   
        auto type = globalSettings.userDataTypesLocations[i].type;

        name = globalSettings.userDataTypesLocations[i].name;        
        pathExpander.Expand(name);
        location = FileUtilities::JoinPath(baseDirectory, name);
                
        if (type == UserDataTypesLocation::Type::FILE)
        {
            //It's a file
            this->userDataTypes.Load(location);
        }
        else if (type == UserDataTypesLocation::Type::DIRECTORY)
        {
            //It's a directory

            //Load all user data types files
            auto search = FileUtilities::JoinPath(location, Strings::USER_DATA_TYPES_FILE_WILDCARD);            

            //ApplicationAccessor::LogMessage(wxT("Looking for user data files in %s"), search.wx_str());

            wxFileSystem fileSystem;
            auto foundFile = fileSystem.FindFirst(search, wxFILE);
            while (!foundFile.empty())
            {
                //Load file
                wxURI fileUri(foundFile);
                auto filePath = fileUri.GetPath().substr(1);
                this->userDataTypes.Load(filePath);

                //ApplicationAccessor::LogMessage(wxT("Found a file at %s"), filePath.wx_str());

                //Find next file
                foundFile = fileSystem.FindNext();
            }
        }
    }

    NotifyUserDataTypesChangedListeners();
}

UserDataTypes& FinjinResourceManager::GetUserDataTypes() 
{
    return this->userDataTypes;
}

wxString FinjinResourceManager::FindVertexFormatName(const std::vector<WxGpuVertexFormatStruct::Element>& vertexFormatElements) const
{
    for (auto& format : this->vertexFormats)
    {
        if (format.elements.size() == vertexFormatElements.size())
        {
            for (size_t i = 0; i < vertexFormatElements.size(); i++)
            {
                auto& element = format.elements[i];
                auto& otherElement = vertexFormatElements[i];
                if (element.elementID == otherElement.elementID && element.type == otherElement.type)
                    return format.typeName;
            }
        }
    }

    return wxEmptyString;
}

void FinjinResourceManager::RefreshSceneResources(ManagedResourceType resourceTypes, bool force, FinjinSceneSettingsAccessor sceneSettings)
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("FinjinResourceManager::RefreshSceneResources(%x, %d)"), resourceTypes, (int)force);

    if (!sceneSettings.IsValid())
        sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings(false);

    if (AnySet(resourceTypes & ManagedResourceType::VERTEX_FORMATS))
        UpdateVertexFormats();
    
    if (AnySet(resourceTypes & ManagedResourceType::USER_DATA_TYPES))
        UpdateUserDataTypes();
}

void FinjinResourceManager::ClearSceneResources(ManagedResourceType resourceTypes)
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("FinjinResourceManager::ClearSceneResources(%x)"), (int)resourceTypes);

    if (AnySet(resourceTypes & ManagedResourceType::VERTEX_FORMATS))
    {
        this->vertexFormats.clear();
    }

    if (AnySet(resourceTypes & ManagedResourceType::USER_DATA_TYPES))
    {
        if (!this->userDataTypes.classes.empty())
        {
            NotifyUserDataTypesChangingListeners();

            this->userDataTypes.Clear();
            
            NotifyUserDataTypesChangedListeners();
        }
    }
}

ManagedResourceType FinjinResourceManager::ValidateResourceLocations(ManagedResourceType resourceTypes, FinjinSceneSettingsAccessor sceneSettings)
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("FinjinResourceManager::ValidateResourceLocations(%x)"), (int)resourceTypes);

    auto result = ManagedResourceType::NONE;
    
    if (resourceTypes != ManagedResourceType::NONE)
    {
        if (!sceneSettings.IsValid())
            sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();

        if (AnySet(resourceTypes & ManagedResourceType::VERTEX_FORMATS))
        {
            if (!ValidateVertexFormatLocation())
            {
                if (Dialogs::ShowVertexFormatDialog())
                    result |= ManagedResourceType::VERTEX_FORMATS;
            }
            else
                result |= ManagedResourceType::VERTEX_FORMATS;
        }

        if (AnySet(resourceTypes & ManagedResourceType::USER_DATA_TYPES))
        {
            if (!ValidateUserDataTypesLocations())
            {
                if (Dialogs::ShowUserDataClassSettingsDialog())
                    result |= ManagedResourceType::USER_DATA_TYPES;
            }
            else
                result |= ManagedResourceType::USER_DATA_TYPES;
        }
    }

    return result;
}

void FinjinResourceManager::GetInvalidUserDataTypesLocations
    (
    std::vector<UserDataTypesLocation>& invalidLocations,
    const std::vector<UserDataTypesLocation>& locations, 
    const wxString& baseDirectory
    )
{
    if (!FileUtilities::FileOrDirectoryExists(baseDirectory))
    {
        UserDataTypesLocation location;
        location.type = UserDataTypesLocation::DIRECTORY;
        location.name = baseDirectory;
        invalidLocations.push_back(location);
    }
    else
    {
        wxString fullPath;

        for (size_t i = 0; i < locations.size(); i++)
        {
            fullPath = FileUtilities::JoinPath(baseDirectory, locations[i].name);
            if (!FileUtilities::FileOrDirectoryExists(fullPath))
                invalidLocations.push_back(locations[i]);            
        }
    }
}
 
bool FinjinResourceManager::ValidateVertexFormatLocation()
{
    auto& globalSettings = FinjinGlobalSettings::GetInstance();
    if (!globalSettings.vertexFormatPath.empty())
    {
        auto path = globalSettings.GetWorkingVertexFormatFilePath();
        if (!wxFileExists(path))
        {
            auto message = Strings::VERTEX_FORMAT_LOCATION_NOT_FOUND_FORMAT;
            message += path;
            message += wxT("\n\n");
            message += Strings::VERTEX_FORMAT_LOCATION_CORRECTION_INSTRUCTIONS;
        
            Dialogs::ShowMessage(message, Strings::INVALID_RESOURCE_LOCATION, wxINFORMATION_DIALOG_FLAGS);

            return false;
        }
    }
    
    return true;
}

bool FinjinResourceManager::ValidateUserDataTypesLocations()
{
    auto& globalSettings = FinjinGlobalSettings::GetInstance();

    //Get the resource locations
    std::vector<UserDataTypesLocation> locations = globalSettings.userDataTypesLocations;
    MacroExpander pathExpander;
    for (auto& resourceLocation : locations)
        pathExpander.Expand(resourceLocation.name);        
    
    //Get the base directory
    auto baseDirectory = globalSettings.GetWorkingBaseUserDataTypesDirectory();

    //Get the invalid resource locations
    std::vector<UserDataTypesLocation> invalidLocations;
    GetInvalidUserDataTypesLocations(invalidLocations, locations, baseDirectory);
    if (!invalidLocations.empty())
    {
        auto message = Strings::RESOURCE_LOCATION_NOT_FOUND_FORMAT;        
        for (size_t i = 0; i < invalidLocations.size(); i++)
        {
            message += invalidLocations[i].name;
            message += wxT("\n");
        }
        message += wxT("\n");
        message += Strings::RESOURCE_LOCATION_CORRECTION_INSTRUCTIONS;
        
        Dialogs::ShowMessage(message, Strings::INVALID_RESOURCE_LOCATION, wxINFORMATION_DIALOG_FLAGS);
        
        return false;
    }
    else
        return true;
}

void FinjinResourceManager::NotifyUserDataTypesChangingListeners()
{
    for (auto listener : this->userDataTypesChangedListeners)
        listener->UserDataTypesChanging();
}

void FinjinResourceManager::NotifyUserDataTypesChangedListeners()
{
    for (auto listener : this->userDataTypesChangedListeners)
        listener->UserDataTypesChanged();
}
