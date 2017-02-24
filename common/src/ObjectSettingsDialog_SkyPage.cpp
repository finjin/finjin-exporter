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
#include "ObjectSettingsDialog_SkyPage.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
const wxString ObjectSettingsDialog_SkyPage::TITLE(wxT("Sky"));


//Implementation---------------------------------------------------------------
ObjectSettingsDialog_SkyPage::ObjectSettingsDialog_SkyPage(wxWindow* parent, FinjinObjectSettingsAccessor objectSettings, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    this->objectSettings = objectSettings;

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxStaticBoxSizer* generalSettingsSizer;
    generalSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("General Settings") ), wxVERTICAL );
    
    enableCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
    
    generalSettingsSizer->Add( enableCheckbox, 0, wxALL, 5 );
    
    topSizer->Add( generalSettingsSizer, 0, wxEXPAND|wxTOP, 5 );
    
    SetSizer(topSizer);
    Layout();
}

ObjectSettingsDialog_SkyPage::~ObjectSettingsDialog_SkyPage()
{
}

bool ObjectSettingsDialog_SkyPage::GetGUIData()
{
    //Enable
    this->objectSettings.SetEnableSky(this->enableCheckbox->GetValue());

    return true;
}

bool ObjectSettingsDialog_SkyPage::SetGUIData()
{
    //Enable
    this->enableCheckbox->SetValue(this->objectSettings.GetEnableSky());

    return true;
}
