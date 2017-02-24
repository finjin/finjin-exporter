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
#include "ObjectSettingsDialog_PrefabInstancePage.hpp"
#include "ApplicationControls.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
const wxString ObjectSettingsDialog_PrefabInstancePage::TITLE(wxT("Prefab Instance"));


//Implementation---------------------------------------------------------------
ObjectSettingsDialog_PrefabInstancePage::ObjectSettingsDialog_PrefabInstancePage(wxWindow* parent, FinjinObjectSettingsAccessor objectSettings, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    this->objectSettings = objectSettings;

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxStaticBoxSizer* generalSettingsSizer;
    generalSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("General Settings") ), wxVERTICAL );
    
    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 1, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    modelNameLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Prefab Name"), wxDefaultPosition, wxDefaultSize, 0 );
    modelNameLabel->Wrap( -1 );
    gridSizer->Add( modelNameLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    
    modelNameText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 180,-1 ), 0 );
    gridSizer->Add( modelNameText, 0, wxRIGHT|wxLEFT, 5 );
    
    generalSettingsSizer->Add( gridSizer, 1, wxEXPAND, 5 );
    
    topSizer->Add( generalSettingsSizer, 0, wxEXPAND|wxTOP, 5 );
    
    SetSizer(topSizer);
    Layout();
}

ObjectSettingsDialog_PrefabInstancePage::~ObjectSettingsDialog_PrefabInstancePage()
{
}

bool ObjectSettingsDialog_PrefabInstancePage::GetGUIData()
{
    //Model name
    this->objectSettings.SetPrefabName(this->modelNameText->GetValue());

    return true;
}

bool ObjectSettingsDialog_PrefabInstancePage::SetGUIData()
{
    //Model name
    this->modelNameText->SetValue(this->objectSettings.GetPrefabName());

    return true;
}
