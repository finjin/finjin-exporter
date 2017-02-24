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
#include "ManualLodDialog.hpp"
#include "StringUtilities.hpp"
#include "Dialogs.hpp"
#include "Strings.hpp"
#include "WindowPlacementManager.hpp"
#include "ApplicationControls.hpp"
#include "ChooseObjectsPrompt.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(ManualLodDialog, FinjinDialog)
    EVT_BUTTON(wxID_OK, ManualLodDialog::OnOK)    
    EVT_BUTTON(ManualLodDialog::MESH_BUTTON, ManualLodDialog::OnMeshButton)
END_EVENT_TABLE()

ManualLodDialog::ManualLodDialog(wxWindow* parent, const wxString& title, ObjectAccessor baseObject, ObjectAccessor object, float distance, ObjectAccessorSet* excludedLodObjects, bool autoDistance) : 
    FinjinDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize( 319,124 ), wxDEFAULT_DIALOG_STYLE, wxT("ManualLodDialog"))
{
    this->baseObject = baseObject;
    this->object = object;
    this->distance = distance;
    if (excludedLodObjects != nullptr)
        this->excludedLodObjects = *excludedLodObjects;
    this->autoDistance = autoDistance;

    SetSizeHints(wxDefaultSize, wxDefaultSize);
    
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    objectLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Object"), wxDefaultPosition, wxDefaultSize, 0 );
    objectLabel->Wrap( -1 );
    gridSizer->Add( objectLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    
    wxBoxSizer* nodeSizer;
    nodeSizer = new wxBoxSizer( wxHORIZONTAL );
    
    objectEdit = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 160,-1 ), wxTE_READONLY );
    
    nodeSizer->Add( objectEdit, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
    
    objectButton = new wxButton( this, wxID_ANY, wxT("Choose..."), wxDefaultPosition, wxDefaultSize, 0 );
    nodeSizer->Add( objectButton, 0, wxTOP|wxRIGHT, 5 );
    
    gridSizer->Add( nodeSizer, 1, wxEXPAND, 5 );
    
    distanceLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Distance"), wxDefaultPosition, wxDefaultSize, 0 );
    distanceLabel->Wrap( -1 );
    gridSizer->Add( distanceLabel, 0, wxALL, 5 );
    
    wxBoxSizer* distanceSizer;
    distanceSizer = new wxBoxSizer( wxHORIZONTAL );
    
    distanceSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(90, -1), 0, 0, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE, SpinnerControl::WORLD_UNIT, 2);
    distanceSizer->Add( distanceSpinner, 0, wxTOP|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
    
    gridSizer->Add( distanceSizer, 1, wxEXPAND, 5 );
    
    topSizer->Add( gridSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
    
    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );
    
    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );
    
    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );
    
    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    SetSizer(topSizer);

    this->objectButton->SetId(MESH_BUTTON);

    SetGUIData();
    
    if (!WindowPlacementManager::RestorePlacement(this))
    {
        Layout();
        Fit();
        Centre(wxBOTH);
    }
}

ManualLodDialog::~ManualLodDialog()
{
}

void ManualLodDialog::SetGUIData()
{
    this->objectEdit->SetValue(this->object.GetFullName());

    if (this->autoDistance)
    {
        const TimeAccessor currentTime = FinjinObjectSettingsAccessor::GetAutoManualLodDistanceTime();
        const FinjinVector3 baseObjectPosition = this->baseObject.GetNodeTransformation(currentTime).GetTranslation();
        if (this->object.IsValid())
            this->distance = (this->object.GetNodeTransformation(currentTime).GetTranslation() - baseObjectPosition).Length();
        else
            this->distance = 0;
    }
    this->distanceSpinner->SetValue(this->distance);
    this->distanceSpinner->Enable(!this->autoDistance);
}

void ManualLodDialog::OnOK(wxCommandEvent& event)
{
    WindowPlacementManager::SavePlacement(this);

    this->distance = this->distanceSpinner->GetValue();
    
    if (!this->object.IsValid())
        Dialogs::ShowMessage(this, Strings::OBJECT_REQUIRED, Strings::DATA_ENTRY_ERROR, wxINFORMATION_DIALOG_FLAGS);
    else
        MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
}

void ManualLodDialog::OnMeshButton(wxCommandEvent& event)
{
    ChooseObjectsPrompt choose(this, HowMany::CHOOSE_ONE, ObjectFilterType::MESH, 0, &this->excludedLodObjects);
    if (choose.ShowPrompt())
    {
        this->object = choose.GetObject();
        SetGUIData();
    }    
}
