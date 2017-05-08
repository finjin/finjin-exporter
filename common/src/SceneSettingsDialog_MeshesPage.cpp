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
#include "SceneSettingsDialog_MeshesPage.hpp"
#include "ChooseObjectsPrompt.hpp"
#include "ObjectFilterType.hpp"
#include "StringSplitter.hpp"
#include "ApplicationControls.hpp"
#include "ApplicationAccessor.hpp"
#include "StringUtilities.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
const wxString SceneSettingsDialog_MeshesPage::TITLE(wxT("Meshes"));


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(SceneSettingsDialog_MeshesPage, SettingsPage)
END_EVENT_TABLE()

SceneSettingsDialog_MeshesPage::SceneSettingsDialog_MeshesPage( wxWindow* parent, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    this->sceneSettings = sceneSettings;

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* meshSettingsSizer;
    meshSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("General Settings") ), wxVERTICAL );

    wxFlexGridSizer* meshSettingsGridSizer;
    meshSettingsGridSizer = new wxFlexGridSizer( 1, 4, 0, 0 );
    meshSettingsGridSizer->SetFlexibleDirection( wxBOTH );
    meshSettingsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    exportVertexColorsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Vertex Colors"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsGridSizer->Add( exportVertexColorsCheckbox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    useExplicitVertexColorCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Explicit Color Fallback (White)"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsGridSizer->Add( useExplicitVertexColorCheckbox, 0, wxALL, 5 );


    meshSettingsSizer->Add( meshSettingsGridSizer, 1, wxEXPAND, 5 );

    animateBoundingVolumesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Animated Bounding Volumes"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsSizer->Add( animateBoundingVolumesCheckbox, 0, wxALL, 5 );

    checkMeshInstancesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Check for Mesh Instances"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsSizer->Add( checkMeshInstancesCheckbox, 0, wxALL, 5 );

    alwaysCreateNewSubmeshesWhenMergingCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Always Create New Submeshes When Merging"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsSizer->Add( alwaysCreateNewSubmeshesWhenMergingCheckBox, 0, wxALL, 5 );


    topSizer->Add( meshSettingsSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* tangentBinormalSettingsSizer;
    tangentBinormalSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Tangent/Binormal Settings") ), wxVERTICAL );

    wxBoxSizer* tangentBinormalSizer;
    tangentBinormalSizer = new wxBoxSizer( wxHORIZONTAL );

    exportTangentsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Tangents"), wxDefaultPosition, wxDefaultSize, 0 );
    tangentBinormalSizer->Add( exportTangentsCheckbox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    exportBinormalsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Binormals"), wxDefaultPosition, wxDefaultSize, 0 );
    tangentBinormalSizer->Add( exportBinormalsCheckbox, 0, wxALL, 5 );


    tangentBinormalSettingsSizer->Add( tangentBinormalSizer, 1, wxEXPAND, 5 );

    topSizer->Add( tangentBinormalSettingsSizer, 0, wxEXPAND|wxTOP, 5 );

    SetSizer(topSizer);
    Layout();
}

SceneSettingsDialog_MeshesPage::~SceneSettingsDialog_MeshesPage()
{
}

bool SceneSettingsDialog_MeshesPage::GetGUIData()
{
    //Vertex colors enabled
    this->sceneSettings.SetExportVertexColors(this->exportVertexColorsCheckbox->GetValue());

    //Explicit vertex color
    this->sceneSettings.SetUseExplicitVertexColor(this->useExplicitVertexColorCheckbox->GetValue());

    //Check for mesh instances
    this->sceneSettings.SetCheckMeshInstances(this->checkMeshInstancesCheckbox->GetValue());

    //Animate bounding volumes
    this->sceneSettings.SetAnimateBoundingVolumes(this->animateBoundingVolumesCheckbox->GetValue());

    //Always create new submeshes when merging
    this->sceneSettings.SetAlwaysCreateNewSubmeshesWhenMerging(this->alwaysCreateNewSubmeshesWhenMergingCheckBox->GetValue());

    //Generate tangents
    this->sceneSettings.SetExportTangents(this->exportTangentsCheckbox->GetValue());

    //Generate binormals
    this->sceneSettings.SetExportBinormals(this->exportBinormalsCheckbox->GetValue());

    return true;
}

bool SceneSettingsDialog_MeshesPage::SetGUIData()
{
    //Vertex colors enabled
    this->exportVertexColorsCheckbox->SetValue(this->sceneSettings.GetExportVertexColors());

    //Explicit vertex color
    this->useExplicitVertexColorCheckbox->SetValue(this->sceneSettings.GetUseExplicitVertexColor());

    //Check for mesh instances
    this->checkMeshInstancesCheckbox->SetValue(this->sceneSettings.GetCheckMeshInstances());

    //Animate bounding volumes
    this->animateBoundingVolumesCheckbox->SetValue(this->sceneSettings.GetAnimateBoundingVolumes());

    //Always create new submeshes when merging
    this->alwaysCreateNewSubmeshesWhenMergingCheckBox->SetValue(this->sceneSettings.GetAlwaysCreateNewSubmeshesWhenMerging());

    //Generate tangents
    this->exportTangentsCheckbox->SetValue(this->sceneSettings.GetExportTangents());

    //Generate binormals
    this->exportBinormalsCheckbox->SetValue(this->sceneSettings.GetExportBinormals());

    Layout();

    return true;
}
