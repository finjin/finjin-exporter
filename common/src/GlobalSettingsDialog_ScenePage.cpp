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
#include "GlobalSettingsDialog_ScenePage.hpp"
#include "FinjinGlobalSettings.hpp"
#include "Strings.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "ApplicationAccessor.hpp"
#include "RelativeLocationPrompt.hpp"
#include "Dialogs.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
const wxString GlobalSettingsDialog_ScenePage::TITLE(wxT("Scene"));


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(GlobalSettingsDialog_ScenePage, SettingsPage)    
END_EVENT_TABLE()

GlobalSettingsDialog_ScenePage::GlobalSettingsDialog_ScenePage(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxStaticBoxSizer* sceneDefaultsSizer;
    sceneDefaultsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Scene Defaults") ), wxVERTICAL );
    
    wxBoxSizer* sceneScaleSizer;
    sceneScaleSizer = new wxBoxSizer( wxHORIZONTAL );
    
    sceneScaleLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Scale"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
    sceneScaleLabel->Wrap( -1 );
    sceneScaleSizer->Add( sceneScaleLabel, 0, wxALL, 5 );
    
    wxArrayString sceneScaleChoiceChoices;
    sceneScaleChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, sceneScaleChoiceChoices, 0 );
    sceneScaleChoice->SetSelection( 0 );
    sceneScaleSizer->Add( sceneScaleChoice, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    divideByLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Divided By"), wxDefaultPosition, wxDefaultSize, 0 );
    divideByLabel->Wrap( -1 );
    sceneScaleSizer->Add( divideByLabel, 0, wxALL, 5 );
    
    dividedBySpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(70, -1), 0, 0, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE, SpinnerControl::FLOAT_UNIT, 1);
    sceneScaleSizer->Add( dividedBySpinner, 0, wxBOTTOM|wxLEFT|wxRIGHT, 5 );
    
    
    sceneDefaultsSizer->Add( sceneScaleSizer, 0, wxEXPAND, 5 );
    
    
    topSizer->Add( sceneDefaultsSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
    
    
    SetSizer(topSizer);
    Layout();
}

GlobalSettingsDialog_ScenePage::~GlobalSettingsDialog_ScenePage()
{
}

bool GlobalSettingsDialog_ScenePage::GetGUIData()
{
    auto& settings = FinjinGlobalSettings::GetInstance();
    
    //Scene scale units choice
    int selectedScaleIndex = this->sceneScaleChoice->GetSelection();
    settings.sceneScaleUnit = (wxChar*)this->sceneScaleChoice->GetClientData(selectedScaleIndex);

    //Scale "divide by" edit
    settings.sceneScaleDivide = this->dividedBySpinner->GetValue();

    return true;
}

bool GlobalSettingsDialog_ScenePage::SetGUIData()
{
    auto& settings = FinjinGlobalSettings::GetInstance();

    //Scene scale units choice
    this->sceneScaleChoice->Freeze();
    for (int i = 0; i < Strings::STANDARD_SCENE_SCALE_COUNT; i++)
    {
        this->sceneScaleChoice->Append(Strings::STANDARD_SCENE_SCALES_TEXT[i]);
        this->sceneScaleChoice->SetClientData(i, (wxChar*)Strings::STANDARD_SCENE_SCALES_VALUE[i]);
        if (settings.sceneScaleUnit == Strings::STANDARD_SCENE_SCALES_VALUE[i])
            this->sceneScaleChoice->SetSelection(i);
    }
    this->sceneScaleChoice->Thaw();
    if (this->sceneScaleChoice->GetSelection() < 0)
        this->sceneScaleChoice->SetSelection(0);

    //Scale "divide by" value    
    this->dividedBySpinner->SetValue(settings.sceneScaleDivide);

    return true;
}
