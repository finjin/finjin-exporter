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
#include "GlobalSettingsDialog_UserDataClassPage.hpp"
#include "FinjinGlobalSettings.hpp"
#include "StringSplitter.hpp"
#include "StringUtilities.hpp"
#include "FileUtilities.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "Strings.hpp"
#include "ApplicationAccessor.hpp"
#include "RelativeLocationPrompt.hpp"
#include "ApplicationControls.hpp"
#include "FinjinResourceManager.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
const wxString GlobalSettingsDialog_UserDataClassPage::TITLE(wxT("User Data"));


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(GlobalSettingsDialog_UserDataClassPage, SettingsPage)
    EVT_CHOICE(GlobalSettingsDialog_UserDataClassPage::BASE_DIRECTORY_CHOICE, GlobalSettingsDialog_UserDataClassPage::OnBaseDirectoryChoice)
    EVT_BUTTON(GlobalSettingsDialog_UserDataClassPage::BASE_DIRECTORY_BUTTON, GlobalSettingsDialog_UserDataClassPage::OnBaseDirectoryButton)
    EVT_BUTTON(GlobalSettingsDialog_UserDataClassPage::ADD_DIRECTORY_BUTTON, GlobalSettingsDialog_UserDataClassPage::OnAddDirectoryButton)
    EVT_BUTTON(GlobalSettingsDialog_UserDataClassPage::ADD_FILE_BUTTON, GlobalSettingsDialog_UserDataClassPage::OnAddFileButton)
END_EVENT_TABLE()

GlobalSettingsDialog_UserDataClassPage::GlobalSettingsDialog_UserDataClassPage(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* resourceLocationsTopSizer;
    resourceLocationsTopSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("File Locations") ), wxVERTICAL );

    baseDirectoryLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Base Directory"), wxDefaultPosition, wxDefaultSize, 0 );
    baseDirectoryLabel->Wrap( -1 );
    resourceLocationsTopSizer->Add( baseDirectoryLabel, 0, wxALL, 5 );

    wxBoxSizer* baseDirectorySizer;
    baseDirectorySizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString baseDirectoryChoiceChoices;
    baseDirectoryChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, baseDirectoryChoiceChoices, 0 );
    baseDirectoryChoice->SetSelection( 0 );
    baseDirectorySizer->Add( baseDirectoryChoice, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    baseDirectoryText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 285,-1 ), wxTE_READONLY );
    baseDirectorySizer->Add( baseDirectoryText, 1, wxBOTTOM, 5 );

    baseDirectoryButton = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxSize(STANDARD_BUTTON_WIDTH,-1 ), 0 );
    baseDirectorySizer->Add( baseDirectoryButton, 0, wxBOTTOM|wxLEFT, 5 );

    resourceLocationsTopSizer->Add( baseDirectorySizer, 0, wxEXPAND, 5 );

    resourceLocationsDescriptionLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("These locations are used to locate files that contain user data classes, which can be used to \nconfigure scene and object user data. Directories will be searched non-recursively for \nfiles with the .cfg-userdatatypes extension."), wxDefaultPosition, wxDefaultSize, 0 );
    resourceLocationsDescriptionLabel->Wrap( -1 );
    resourceLocationsTopSizer->Add( resourceLocationsDescriptionLabel, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

    wxBoxSizer* resourceLocationsSizer;
    resourceLocationsSizer = new wxBoxSizer( wxHORIZONTAL );

    resourceLocationsText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_DONTWRAP|wxTE_MULTILINE );
    resourceLocationsText->SetMinSize( wxSize( 365,150 ) );

    resourceLocationsSizer->Add( resourceLocationsText, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxTOP, 5 );

    wxBoxSizer* resourceLocationsButtonsSizer;
    resourceLocationsButtonsSizer = new wxBoxSizer(wxVERTICAL);

    addDirectoryButton = new wxButton( this, wxID_ANY, wxT("Add Directory..."), wxDefaultPosition, wxSize(STANDARD_BUTTON_WIDTH,-1 ), 0 );
    resourceLocationsButtonsSizer->Add( addDirectoryButton, 0, wxTOP|wxLEFT, 5 );

    addFileButton = new wxButton( this, wxID_ANY, wxT("Add File..."), wxDefaultPosition, wxSize(STANDARD_BUTTON_WIDTH,-1 ), 0 );
    resourceLocationsButtonsSizer->Add( addFileButton, 0, wxBOTTOM|wxLEFT, 5 );

    resourceLocationsSizer->Add( resourceLocationsButtonsSizer, 0, wxEXPAND, 5 );

    resourceLocationsTopSizer->Add( resourceLocationsSizer, 1, wxEXPAND, 5 );

    topSizer->Add( resourceLocationsTopSizer, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    this->baseDirectoryChoice->SetId(BASE_DIRECTORY_CHOICE);
    this->baseDirectoryButton->SetId(BASE_DIRECTORY_BUTTON);
    this->addDirectoryButton->SetId(ADD_DIRECTORY_BUTTON);
    this->addFileButton->SetId(ADD_FILE_BUTTON);

    SetGUIData();

    SetSizer(topSizer);
    Layout();
}

GlobalSettingsDialog_UserDataClassPage::~GlobalSettingsDialog_UserDataClassPage()
{
}

bool GlobalSettingsDialog_UserDataClassPage::GetGUIData()
{
    auto& settings = FinjinGlobalSettings::GetInstance();

    //Base directory choice/edit
    BaseDirectory baseDirectory(this->baseDirectoryChoice, this->baseDirectoryText);
    baseDirectory.GetGUIData(settings.baseUserDataTypesDirectoryType, settings.baseUserDataTypesDirectory);

    //Locations
    wxString locationsText = this->resourceLocationsText->GetValue();
    StringSplitter locations(locationsText, wxT('\n'));
    settings.userDataTypesLocations.clear();
    wxString name, type;
    for (size_t i = 0; i < locations.size(); i++)
    {
        if (FileUtilities::FileOrDirectoryExists(locations[i]))
        {
            //The location name was specified by hand without a leading type string
            name = locations[i];

            //Figure out type
            if (wxDirExists(locations[i]))
                settings.userDataTypesLocations.push_back(UserDataTypesLocation(UserDataTypesLocation::DIRECTORY, name));
            else
                settings.userDataTypesLocations.push_back(UserDataTypesLocation(UserDataTypesLocation::FILE, name));
        }
        else
        {
            wxString type;
            if (StringUtilities::ParseKeyValueLine(locations[i], type, name))
            {
                if (type == wxT("directory"))
                    settings.userDataTypesLocations.push_back(UserDataTypesLocation(UserDataTypesLocation::DIRECTORY, name));
                else if (type == wxT("file"))
                    settings.userDataTypesLocations.push_back(UserDataTypesLocation(UserDataTypesLocation::FILE, name));
            }
        }
    }

    //If the locations are valid, then hold onto them
    if (FinjinResourceManager::GetInstance().ValidateUserDataTypesLocations())
    {
        //Notify that the locations have changed
        FinjinResourceManager::GetInstance().UpdateUserDataTypes();
        return true;
    }
    else
        return false;
}

bool GlobalSettingsDialog_UserDataClassPage::SetGUIData()
{
    auto& settings = FinjinGlobalSettings::GetInstance();

    //Base directory
    BaseDirectory baseDirectory(settings.baseUserDataTypesDirectoryType, settings.baseUserDataTypesDirectory);
    baseDirectory.SetGUIData(this->baseDirectoryChoice, this->baseDirectoryText);

    //Locations
    wxString locationsText;
    for (auto& location : settings.userDataTypesLocations)
        StringUtilities::AppendKeyAndValue(locationsText, location.GetTypeString(), location.name, wxT("\n"));
    this->resourceLocationsText->SetValue(locationsText);

    this->currentCustomDirectory = settings.baseUserDataTypesDirectory;
    UpdateBaseDirectoryControls();

    return true;
}

void GlobalSettingsDialog_UserDataClassPage::UpdateBaseDirectoryControls()
{
    int baseDirectoryChoice = BaseDirectory::GetBaseDirectoryTypeChoice(this->baseDirectoryChoice);

    //Base directory text
    if (baseDirectoryChoice == BaseDirectory::CUSTOM_DIRECTORY_CHOICE)
    {
        this->baseDirectoryText->SetValue(this->currentCustomDirectory);
    }
    else
    {
        BaseDirectory baseDirectory(this->baseDirectoryChoice, this->baseDirectoryText);
        this->baseDirectoryText->SetValue(baseDirectory.GetBaseDirectory());
    }

    //Base directory button
    this->baseDirectoryButton->Show(baseDirectoryChoice == BaseDirectory::CUSTOM_DIRECTORY_CHOICE);
    Layout();
}

void GlobalSettingsDialog_UserDataClassPage::OnBaseDirectoryChoice(wxCommandEvent& event)
{
    UpdateBaseDirectoryControls();
}

void GlobalSettingsDialog_UserDataClassPage::OnBaseDirectoryButton(wxCommandEvent& event)
{
    SUSPEND_CUSTOM_CONTROLS;
    wxDirDialog dialog(this, Strings::CHOOSE_DIRECTORY, wxEmptyString);
    if (dialog.ShowModal() == wxID_OK)
    {
        int baseDirectoryChoice = BaseDirectory::GetBaseDirectoryTypeChoice(this->baseDirectoryChoice);
        if (baseDirectoryChoice == BaseDirectory::CUSTOM_DIRECTORY_CHOICE)
        {
            this->currentCustomDirectory = dialog.GetPath();
            FileUtilities::EnsureTrailingPathSeparator(this->currentCustomDirectory);
            this->baseDirectoryText->SetValue(this->currentCustomDirectory);
        }
        else if (baseDirectoryChoice == BaseDirectory::PROJECT_DIRECTORY_CHOICE)
        {
            auto directory = dialog.GetPath();
            FileUtilities::EnsureTrailingPathSeparator(directory);
            this->baseDirectoryText->SetValue(directory);
        }
    }
}

void GlobalSettingsDialog_UserDataClassPage::OnAddDirectoryButton(wxCommandEvent& event)
{
    BaseDirectory baseDirectory(this->baseDirectoryChoice, this->baseDirectoryText);
    RelativeLocationPrompt prompt(this, baseDirectory.GetBaseDirectory(), Strings::CHOOSE_DIRECTORY);
    auto path = prompt.ShowPrompt();
    if (!path.empty())
    {
        //Set into text box
        auto text = this->resourceLocationsText->GetValue();
        StringUtilities::AppendKeyAndValue(text, wxT("directory"), path, wxT("\n"));
        this->resourceLocationsText->SetValue(text);
    }
}

void GlobalSettingsDialog_UserDataClassPage::OnAddFileButton(wxCommandEvent& event)
{
    BaseDirectory baseDirectory(this->baseDirectoryChoice, this->baseDirectoryText);
    RelativeLocationPrompt prompt(this, baseDirectory.GetBaseDirectory(), Strings::CHOOSE_FILE, Strings::USER_DATA_TYPES_FILE_FILTER);
    auto path = prompt.ShowPrompt();
    if (!path.empty())
    {
        //Set into text box
        auto text = this->resourceLocationsText->GetValue();
        StringUtilities::AppendKeyAndValue(text, wxT("file"), path, wxT("\n"));
        this->resourceLocationsText->SetValue(text);
    }
}
