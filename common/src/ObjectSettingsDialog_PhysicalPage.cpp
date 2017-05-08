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
#include "ObjectSettingsDialog_PhysicalPage.hpp"
#include "ApplicationControls.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
const wxString ObjectSettingsDialog_PhysicalPage::TITLE(wxT("Physical"));


//Implementation----------------------------------------------------------------
ObjectSettingsDialog_PhysicalPage::ObjectSettingsDialog_PhysicalPage(wxWindow* parent, FinjinObjectSettingsAccessor objectSettings, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
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

    itemTypeLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Item Type"), wxDefaultPosition, wxDefaultSize, 0 );
    itemTypeLabel->Wrap( -1 );
    gridSizer->Add( itemTypeLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    itemTypeText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), 0 );
    gridSizer->Add( itemTypeText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    generalSettingsSizer->Add( gridSizer, 1, wxEXPAND, 5 );

    topSizer->Add( generalSettingsSizer, 0, wxEXPAND|wxTOP, 5 );

    SetSizer(topSizer);
    Layout();
}

ObjectSettingsDialog_PhysicalPage::~ObjectSettingsDialog_PhysicalPage()
{
}

bool ObjectSettingsDialog_PhysicalPage::GetGUIData()
{
    //Item type
    this->objectSettings.SetItemType(this->itemTypeText->GetValue());

    return true;
}

bool ObjectSettingsDialog_PhysicalPage::SetGUIData()
{
    //Item type
    this->itemTypeText->SetValue(this->objectSettings.GetItemType());

    return true;
}
