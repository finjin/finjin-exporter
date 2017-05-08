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
#include "ObjectSettingsDialog_MeshPage.hpp"
#include "ApplicationControls.hpp"
#include "ApplicationAccessor.hpp"
#include "StringUtilities.hpp"
#include "Strings.hpp"
#include "Dialogs.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "GeometryState.hpp"
#include "PropertyValues.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
const wxString ObjectSettingsDialog_MeshPage::TITLE(wxT("Mesh"));


//Globals-----------------------------------------------------------------------
static const wxChar* PRIMITIVE_TYPES_TEXT[] = {wxT("Default"), wxT("Line List"), wxT("Point List")};
static const wxChar* PRIMITIVE_TYPES_VALUE[] = {wxT(""), wxT("lines"), wxT("points")};
static const int PRIMITIVE_TYPE_COUNT = FINJIN_COUNT_OF(PRIMITIVE_TYPES_VALUE);


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(ObjectSettingsDialog_MeshPage, SettingsPage)
END_EVENT_TABLE()

ObjectSettingsDialog_MeshPage::ObjectSettingsDialog_MeshPage(wxWindow* parent, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    this->object = object;
    this->objectSettings = objectSettings;

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* meshSettingsSizer;
    meshSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("General Settings") ), wxVERTICAL );

    wxFlexGridSizer* meshSettingsGridSizer;
    meshSettingsGridSizer = new wxFlexGridSizer( 4, 2, 0, 0 );
    meshSettingsGridSizer->SetFlexibleDirection( wxBOTH );
    meshSettingsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    meshNameLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Mesh Name"), wxDefaultPosition, wxDefaultSize, 0 );
    meshNameLabel->Wrap( -1 );
    meshSettingsGridSizer->Add( meshNameLabel, 0, wxALL, 5 );

    meshNameText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), 0 );
    meshSettingsGridSizer->Add( meshNameText, 0, wxRIGHT|wxLEFT, 5 );

    primitiveTypeLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Primitive Type"), wxDefaultPosition, wxDefaultSize, 0 );
    primitiveTypeLabel->Wrap( -1 );
    meshSettingsGridSizer->Add( primitiveTypeLabel, 0, wxALL, 5 );

    wxArrayString primitiveTypeChoiceChoices;
    primitiveTypeChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 140,-1 ), primitiveTypeChoiceChoices, 0 );
    primitiveTypeChoice->SetSelection( 0 );
    meshSettingsGridSizer->Add( primitiveTypeChoice, 0, wxRIGHT|wxLEFT, 5 );

    pointSizeLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Point Size"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
    pointSizeLabel->Wrap( -1 );
    meshSettingsGridSizer->Add( pointSizeLabel, 0, wxALL, 5 );

    pointSizeSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(70, -1), 0, 1, 4096, SpinnerControl::FLOAT_UNIT, 1);
    meshSettingsGridSizer->Add( pointSizeSpinner, 0, wxBOTTOM|wxLEFT|wxRIGHT, 5 );

    meshSettingsSizer->Add( meshSettingsGridSizer, 1, wxEXPAND, 5 );

    exportVertexColorsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Vertex Colors"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsSizer->Add( exportVertexColorsCheckbox, 0, wxALL, 5 );

    animateBoundingVolumesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Animated Bounding Volumes"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsSizer->Add( animateBoundingVolumesCheckbox, 0, wxALL, 5 );

    checkMeshInstanceCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Check for Mesh Instance"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsSizer->Add( checkMeshInstanceCheckbox, 0, wxALL, 5 );

    topSizer->Add( meshSettingsSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* tangentBinormalSettingsSizer;
    tangentBinormalSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Tangent/Binormal Settings") ), wxVERTICAL );

    wxFlexGridSizer* tangentBinormalSettingsGridSizer;
    tangentBinormalSettingsGridSizer = new wxFlexGridSizer( 6, 2, 0, 0 );
    tangentBinormalSettingsGridSizer->SetFlexibleDirection( wxBOTH );
    tangentBinormalSettingsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    exportTangentsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Tangents"), wxDefaultPosition, wxDefaultSize, 0 );
    tangentBinormalSettingsGridSizer->Add( exportTangentsCheckbox, 0, wxALL, 5 );

    exportBinormalsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Binormals"), wxDefaultPosition, wxDefaultSize, 0 );
    tangentBinormalSettingsGridSizer->Add( exportBinormalsCheckbox, 0, wxALL, 5 );


    tangentBinormalSettingsSizer->Add( tangentBinormalSettingsGridSizer, 1, wxEXPAND, 5 );


    topSizer->Add( tangentBinormalSettingsSizer, 0, wxEXPAND|wxTOP, 5 );

    SetSizer(topSizer);

    Layout();
}

ObjectSettingsDialog_MeshPage::~ObjectSettingsDialog_MeshPage()
{
}

bool ObjectSettingsDialog_MeshPage::GetGUIData()
{
    //Mesh name
    this->objectSettings.SetMeshName(this->meshNameText->GetValue());

    //Primitive type
    int selectedPrimitiveTypeIndex = this->primitiveTypeChoice->GetSelection();
    this->objectSettings.SetPrimitiveType((wxChar*)this->primitiveTypeChoice->GetClientData(selectedPrimitiveTypeIndex));

    //Point size
    this->objectSettings.SetPointSize(pointSizeSpinner->GetValue());

    //Export vertex colors
    this->objectSettings.SetExportVertexColors(this->exportVertexColorsCheckbox->GetValue());

    //Allow mesh instance check
    this->objectSettings.SetCheckMeshInstance(this->checkMeshInstanceCheckbox->GetValue());

    //Animate bounding volumes
    this->objectSettings.SetAnimateBoundingVolumes(animateBoundingVolumesCheckbox->GetValue());

    //Generate tangents
    this->objectSettings.SetExportTangents(this->exportTangentsCheckbox->GetValue());

    //Generate binormals
    this->objectSettings.SetExportBinormals(this->exportBinormalsCheckbox->GetValue());

    return true;
}

bool ObjectSettingsDialog_MeshPage::SetGUIData()
{
    //Mesh name
    this->meshNameText->SetValue(this->objectSettings.GetMeshName());

    //Primitive type
    this->primitiveTypeChoice->Freeze();
    for (int i = 0; i < PRIMITIVE_TYPE_COUNT; i++)
    {
        this->primitiveTypeChoice->Append(PRIMITIVE_TYPES_TEXT[i]);
        this->primitiveTypeChoice->SetClientData(i, (wxChar*)PRIMITIVE_TYPES_VALUE[i]);
        if (this->objectSettings.GetPrimitiveType() == PRIMITIVE_TYPES_VALUE[i])
            this->primitiveTypeChoice->SetSelection(i);
    }
    this->primitiveTypeChoice->Thaw();
    if (this->primitiveTypeChoice->GetSelection() < 0)
        this->primitiveTypeChoice->SetSelection(0);

    //Export vertex colors
    this->exportVertexColorsCheckbox->SetValue(this->objectSettings.GetExportVertexColors());

    //Point size
    pointSizeSpinner->SetValue(this->objectSettings.GetPointSize());

    //Allow mesh instance check
    this->checkMeshInstanceCheckbox->SetValue(this->objectSettings.GetCheckMeshInstance());

    //Animate bounding volumes
    animateBoundingVolumesCheckbox->SetValue(this->objectSettings.GetAnimateBoundingVolumes());

    //Generate tangents
    this->exportTangentsCheckbox->SetValue(this->objectSettings.GetExportTangents());

    //Generate binormals
    this->exportBinormalsCheckbox->SetValue(this->objectSettings.GetExportBinormals());

    return true;
}
