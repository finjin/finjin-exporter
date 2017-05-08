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
#include "UserDataSettingsDialog.hpp"
#include "WindowPlacementManager.hpp"
#include "UserDataSettingsPage.hpp"
#include "Strings.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(UserDataSettingsDialog, FinjinDialog)
    EVT_CLOSE(UserDataSettingsDialog::OnCloseWindow)
    EVT_BUTTON(wxID_OK, UserDataSettingsDialog::OnOK)
END_EVENT_TABLE()

UserDataSettingsDialog::UserDataSettingsDialog()
{
}

UserDataSettingsDialog::UserDataSettingsDialog
    (
    wxWindow* parent,
    const wxString& objectName,
    UserDataSettingsAccessor& userDataSettings,
    UserDataUsage classUsage,
    FinjinSceneSettingsAccessor sceneSettings
    )
{
    Create(parent, objectName, userDataSettings, classUsage, sceneSettings);
}

bool UserDataSettingsDialog::Create
    (
    wxWindow* parent,
    const wxString& objectName,
    UserDataSettingsAccessor& userDataSettings,
    UserDataUsage classUsage,
    FinjinSceneSettingsAccessor sceneSettings
    )
{
    this->userDataSettings = &userDataSettings;
    this->classUsage = classUsage;
    this->sceneSettings = sceneSettings;

    auto title = wxString::Format(Strings::USER_DATA_SETTINGS_DIALOG_TITLE_FORMAT, objectName.wx_str());
    if (!FinjinDialog::Create(parent, wxID_ANY, title, wxDefaultPosition, wxSize(520, 665), wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU, wxT("FinjinUserDataSettingsDialog")))
        return false;

    CreateControls();

    return true;
}

void UserDataSettingsDialog::CreateControls()
{
    wxButton* closeButton;

    SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* pageSizer = new wxBoxSizer(wxVERTICAL);

    pageSizer->AddSpacer(5); //Need a little extra space at the top

    this->userDataPage = new UserDataSettingsPage(this, *this->userDataSettings, this->classUsage, this->sceneSettings);
    pageSizer->Add(this->userDataPage, 1, wxEXPAND, 5);

    topSizer->Add( pageSizer, 1, wxEXPAND | wxDOWN | wxRIGHT | wxLEFT, 5);

    wxGridSizer* bottomSizer;
    bottomSizer = new wxGridSizer( 1, 1, 0, 0 );

    closeButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    bottomSizer->Add( closeButton, 0, wxALIGN_RIGHT|wxALL, 5 );

    topSizer->Add( bottomSizer, 0, wxEXPAND, 5 );

    this->userDataPage->SetGUIData();

    SetSizer(topSizer);

    if (!WindowPlacementManager::RestorePlacement(this))
    {
        Layout();
        Centre(wxBOTH);
    }

    SetMinSize(GetSize());
    SetMaxSize(wxGetDisplaySize());
}

void UserDataSettingsDialog::OnCloseWindow(wxCloseEvent& event)
{
    WindowPlacementManager::SavePlacement(this);
    EndModal(wxID_OK);
}

void UserDataSettingsDialog::OnOK(wxCommandEvent& event)
{
    WindowPlacementManager::SavePlacement(this);
    this->userDataPage->GetGUIData();
    MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
}
