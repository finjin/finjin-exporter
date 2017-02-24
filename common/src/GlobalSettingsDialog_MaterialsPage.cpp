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
#include "GlobalSettingsDialog_MaterialsPage.hpp"
#include "FinjinGlobalSettings.hpp"
#include "Strings.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "ApplicationAccessor.hpp"
#include "RelativeLocationPrompt.hpp"
#include "Dialogs.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
const wxString GlobalSettingsDialog_MaterialsPage::TITLE(wxT("Materials"));


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(GlobalSettingsDialog_MaterialsPage, SettingsPage)    
END_EVENT_TABLE()

GlobalSettingsDialog_MaterialsPage::GlobalSettingsDialog_MaterialsPage(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxStaticBoxSizer* materialNamingSizer;
    materialNamingSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Materials") ), wxVERTICAL );
    
    wxFlexGridSizer* materialNamingGridSizer;
    materialNamingGridSizer = new wxFlexGridSizer( 1, 3, 0, 0 );
    materialNamingGridSizer->SetFlexibleDirection( wxBOTH );
    materialNamingGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    noMaterialNameLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("\"No Material\" Name"), wxDefaultPosition, wxDefaultSize, 0 );
    noMaterialNameLabel->Wrap( -1 );
    materialNamingGridSizer->Add( noMaterialNameLabel, 0, wxALIGN_TOP|wxALL, 5 );
    
    noMaterialNameText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), 0 );
    materialNamingGridSizer->Add( noMaterialNameText, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    noMaterialNameDescriptionLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Name of the default material assigned to\nobjects that have no material."), wxDefaultPosition, wxDefaultSize, 0 );
    noMaterialNameDescriptionLabel->Wrap( -1 );
    materialNamingGridSizer->Add( noMaterialNameDescriptionLabel, 0, wxRIGHT, 5 );
    
    materialNamingSizer->Add( materialNamingGridSizer, 1, wxEXPAND, 5 );

    includeParentMaterialInNameCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Include Parent Material in Name"), wxDefaultPosition, wxDefaultSize, 0 );    
    materialNamingSizer->Add( includeParentMaterialInNameCheckbox, 0, wxALL, 5 );

    createExtraMaterialsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Create Extra Materials if Necessary"), wxDefaultPosition, wxDefaultSize, 0 );    
    materialNamingSizer->Add( createExtraMaterialsCheckbox, 0, wxALL, 5 );
    
    topSizer->Add( materialNamingSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

    
    wxStaticBoxSizer* bitmapExportControlSizer;
    bitmapExportControlSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Bitmaps") ), wxVERTICAL );
    
    copyBitmapsToExportDirectoryCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Copy Bitmaps to Export Directory"), wxDefaultPosition, wxDefaultSize, 0 );
    bitmapExportControlSizer->Add( copyBitmapsToExportDirectoryCheckbox, 0, wxALL, 5 );
    
    convertBitmapsToTexturesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Convert Bitmaps to Textures (.texture)"), wxDefaultPosition, wxDefaultSize, 0 );
    bitmapExportControlSizer->Add( convertBitmapsToTexturesCheckbox, 0, wxALL, 5 );
    
    
    topSizer->Add( bitmapExportControlSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

    SetSizer(topSizer);
    Layout();
}

GlobalSettingsDialog_MaterialsPage::~GlobalSettingsDialog_MaterialsPage()
{
}

bool GlobalSettingsDialog_MaterialsPage::GetGUIData()
{
    auto& settings = FinjinGlobalSettings::GetInstance();
    
    //Include parent material name
    settings.includeParentMaterialName = this->includeParentMaterialInNameCheckbox->GetValue();

    //Create extra materials
    settings.createExtraMaterials = this->createExtraMaterialsCheckbox->GetValue();
    
    //"No material" name
    settings.noMaterialName = this->noMaterialNameText->GetValue();

    //Copy/convert bitmaps
    settings.copyBitmaps = this->copyBitmapsToExportDirectoryCheckbox->GetValue();
    settings.convertBitmapsToTextures = this->convertBitmapsToTexturesCheckbox->GetValue();

    //External viewer
    //if (this->finjinViewerButton->GetValue())
        settings.externalViewer = FinjinGlobalSettings::FINJIN_VIEWER;
    
    return true;
}

bool GlobalSettingsDialog_MaterialsPage::SetGUIData()
{
    auto& settings = FinjinGlobalSettings::GetInstance();

    this->noMaterialNameLabel->Show(ApplicationAccessor::SupportsNoMaterialAssignment());
    this->noMaterialNameText->Show(ApplicationAccessor::SupportsNoMaterialAssignment());
    this->noMaterialNameDescriptionLabel->Show(ApplicationAccessor::SupportsNoMaterialAssignment());
    this->includeParentMaterialInNameCheckbox->Show(ApplicationAccessor::SupportsNestedMaterials());
    this->createExtraMaterialsCheckbox->Show(ApplicationAccessor::NeedsExtraMaterials());

    //Include parent material name
    this->includeParentMaterialInNameCheckbox->SetValue(settings.includeParentMaterialName);

    //Create extra materials
    this->createExtraMaterialsCheckbox->SetValue(settings.createExtraMaterials);

    //"No material" name
    this->noMaterialNameText->SetValue(settings.noMaterialName);

    //Copy/convert bitmaps
    this->copyBitmapsToExportDirectoryCheckbox->SetValue(settings.copyBitmaps);
    this->convertBitmapsToTexturesCheckbox->SetValue(settings.convertBitmapsToTextures);

    return true;
}
