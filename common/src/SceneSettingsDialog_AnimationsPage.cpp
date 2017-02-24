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
#include "SceneSettingsDialog_AnimationsPage.hpp"
#include "ApplicationControls.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "RelativeLocationPrompt.hpp"
#include "Strings.hpp"
#include "FileUtilities.hpp"
#include "Exporter.hpp"
#include "ApplicationAccessor.hpp"
#include "FinjinGlobalSettings.hpp"
#include "StringUtilities.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
const wxString SceneSettingsDialog_AnimationsPage::TITLE(wxT("Animations"));


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(SceneSettingsDialog_AnimationsPage, SettingsPage)
    EVT_CHOICE(SceneSettingsDialog_AnimationsPage::SKELETON_ANIMATION_SAMPLING_CHOICE, SceneSettingsDialog_AnimationsPage::OnSkeletonAnimationSamplingChoice)
    EVT_CHOICE(SceneSettingsDialog_AnimationsPage::VERTEX_ANIMATION_SAMPLING_CHOICE, SceneSettingsDialog_AnimationsPage::OnVertexAnimationSamplingChoice)
    EVT_CHOICE(SceneSettingsDialog_AnimationsPage::NODE_ANIMATION_SAMPLING_CHOICE, SceneSettingsDialog_AnimationsPage::OnNodeAnimationSamplingChoice)
END_EVENT_TABLE()

SceneSettingsDialog_AnimationsPage::SceneSettingsDialog_AnimationsPage( wxWindow* parent, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    this->settingGuiData = false;

    this->sceneSettings = sceneSettings;
    
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* meshAnimationsSizer;
    meshAnimationsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Mesh Animations") ), wxVERTICAL );
    
    wxGridSizer* skeletonAndVertexanimationSamplingSizer;
    skeletonAndVertexanimationSamplingSizer = new wxGridSizer( 1, 2, 0, 0 );
    
    wxBoxSizer* skeletonAnimationSamplingSizer;
    skeletonAnimationSamplingSizer = new wxBoxSizer( wxVERTICAL );
    
    skeletonAnimationSamplingLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Skeleton Animation Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    skeletonAnimationSamplingLabel->Wrap( -1 );
    skeletonAnimationSamplingSizer->Add( skeletonAnimationSamplingLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
    
    wxBoxSizer* skeletonAnimationSamplingValuesSizer;
    skeletonAnimationSamplingValuesSizer = new wxBoxSizer( wxHORIZONTAL );
    
    skeletonAnimationSamplingText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    skeletonAnimationSamplingValuesSizer->Add( skeletonAnimationSamplingText, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
    
    wxArrayString skeletonAnimationSamplingChoiceChoices;
    skeletonAnimationSamplingChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, skeletonAnimationSamplingChoiceChoices, 0 );
    skeletonAnimationSamplingChoice->SetSelection( 0 );
    skeletonAnimationSamplingValuesSizer->Add( skeletonAnimationSamplingChoice, 1, wxALL, 5 );
    
    skeletonAnimationSamplingSizer->Add( skeletonAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );
    
    skeletonAndVertexanimationSamplingSizer->Add( skeletonAnimationSamplingSizer, 1, wxEXPAND, 5 );
    
    wxBoxSizer* vertexAnimationSamplingSizer;
    vertexAnimationSamplingSizer = new wxBoxSizer( wxVERTICAL );
    
    vertexAnimationSamplingLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Vertex Animation Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    vertexAnimationSamplingLabel->Wrap( -1 );
    vertexAnimationSamplingSizer->Add( vertexAnimationSamplingLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
    
    wxBoxSizer* vertexAnimationSamplingValuesSizer;
    vertexAnimationSamplingValuesSizer = new wxBoxSizer( wxHORIZONTAL );
    
    vertexAnimationSamplingText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    vertexAnimationSamplingValuesSizer->Add( vertexAnimationSamplingText, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
    
    wxArrayString vertexAnimationSamplingChoiceChoices;
    vertexAnimationSamplingChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, vertexAnimationSamplingChoiceChoices, 0 );
    vertexAnimationSamplingChoice->SetSelection( 0 );
    vertexAnimationSamplingValuesSizer->Add( vertexAnimationSamplingChoice, 1, wxALL, 5 );
    
    vertexAnimationSamplingSizer->Add( vertexAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );
    
    skeletonAndVertexanimationSamplingSizer->Add( vertexAnimationSamplingSizer, 1, wxEXPAND, 5 );
    
    meshAnimationsSizer->Add( skeletonAndVertexanimationSamplingSizer, 1, wxEXPAND, 5 );
    
    topSizer->Add( meshAnimationsSizer, 0, wxEXPAND|wxTOP, 5 );


    wxStaticBoxSizer* nodeAnimationsSizer;
    nodeAnimationsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Node Animations") ), wxVERTICAL );
    
    wxBoxSizer* nodeAnimationSamplingSizer;
    nodeAnimationSamplingSizer = new wxBoxSizer( wxVERTICAL );
    
    nodeAnimationSamplingLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Node Animation Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    nodeAnimationSamplingLabel->Wrap( -1 );
    nodeAnimationSamplingSizer->Add( nodeAnimationSamplingLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
    
    wxBoxSizer* nodeAnimationSamplingValuesSizer;
    nodeAnimationSamplingValuesSizer = new wxBoxSizer( wxHORIZONTAL );
    
    nodeAnimationSamplingText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    nodeAnimationSamplingValuesSizer->Add( nodeAnimationSamplingText, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
    
    wxArrayString nodeAnimationSamplingChoiceChoices;
    nodeAnimationSamplingChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, nodeAnimationSamplingChoiceChoices, 0 );
    nodeAnimationSamplingChoice->SetSelection( 0 );
    nodeAnimationSamplingValuesSizer->Add( nodeAnimationSamplingChoice, 1, wxALL, 5 );
    
    nodeAnimationSamplingSizer->Add( nodeAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );
    
    nodeAnimationsSizer->Add( nodeAnimationSamplingSizer, 1, wxEXPAND, 5 );
    
    topSizer->Add( nodeAnimationsSizer, 0, wxEXPAND|wxTOP, 5 );

    this->skeletonAnimationSamplingChoice->SetId(SKELETON_ANIMATION_SAMPLING_CHOICE);
    this->vertexAnimationSamplingChoice->SetId(VERTEX_ANIMATION_SAMPLING_CHOICE);
    this->nodeAnimationSamplingChoice->SetId(NODE_ANIMATION_SAMPLING_CHOICE);

    SetSizer(topSizer);
    Layout();
}

SceneSettingsDialog_AnimationsPage::~SceneSettingsDialog_AnimationsPage()
{
}

bool SceneSettingsDialog_AnimationsPage::GetGUIData()
{
    //Skeleton animation sampling combo and text
    int selectedSkeletonAnimationSampleTypeIndex = this->skeletonAnimationSamplingChoice->GetSelection();
    SampleType skeletonAnimationSampleType = (SampleType)*(int*)this->skeletonAnimationSamplingChoice->GetClientData(selectedSkeletonAnimationSampleTypeIndex);    
    auto skeletonAnimationSampling = StringUtilities::ToDouble(this->skeletonAnimationSamplingText->GetValue());
    if (skeletonAnimationSampling < 0)
        skeletonAnimationSampling = 0;
    this->sceneSettings.SetSkeletonAnimationSampling(skeletonAnimationSampling, skeletonAnimationSampleType, skeletonAnimationSampleType != SampleType::USE_PARENT);
    
    //Vertex animation sampling combo and text
    int selectedVertexAnimationSampleTypeIndex = this->vertexAnimationSamplingChoice->GetSelection();
    SampleType vertexAnimationSampleType = (SampleType)*(int*)this->vertexAnimationSamplingChoice->GetClientData(selectedVertexAnimationSampleTypeIndex);
    auto vertexAnimationSampling = StringUtilities::ToDouble(this->vertexAnimationSamplingText->GetValue());
    if (vertexAnimationSampling < 0)
        vertexAnimationSampling = 0;
    this->sceneSettings.SetVertexAnimationSampling(vertexAnimationSampling, vertexAnimationSampleType, vertexAnimationSampleType != SampleType::USE_PARENT);
    
    //Node animation sampling combo and text
    int selectedNodeAnimationSampleTypeIndex = this->nodeAnimationSamplingChoice->GetSelection();
    SampleType nodeAnimationSampleType = (SampleType)*(int*)this->nodeAnimationSamplingChoice->GetClientData(selectedNodeAnimationSampleTypeIndex);    
    auto nodeAnimationSampling = StringUtilities::ToDouble(this->nodeAnimationSamplingText->GetValue());
    if (nodeAnimationSampling < 0)
        nodeAnimationSampling = 0;
    this->sceneSettings.SetNodeAnimationSampling(nodeAnimationSampling, nodeAnimationSampleType, nodeAnimationSampleType != SampleType::USE_PARENT);
    
    return true;
}

bool SceneSettingsDialog_AnimationsPage::SetGUIData()
{
    this->settingGuiData = true;

    //Skeleton animation sampling value
    this->skeletonAnimationSamplingText->SetValue(this->sceneSettings.GetSkeletonAnimationSampleValueText());
    
    //Skeleton animation sampling combo
    this->skeletonAnimationSamplingChoice->Freeze();
    for (int i = 0; i < Strings::SCENE_SAMPLE_TYPE_COUNT; i++)
    {
        this->skeletonAnimationSamplingChoice->Append(Strings::SCENE_SAMPLE_TYPES_TEXT[i]);
        this->skeletonAnimationSamplingChoice->SetClientData(i, (void*)&Strings::SCENE_SAMPLE_TYPES_VALUE[i]);
        if (this->sceneSettings.GetSkeletonAnimationSampleType() == Strings::SCENE_SAMPLE_TYPES_VALUE[i])
            this->skeletonAnimationSamplingChoice->SetSelection(i);
    }
    this->skeletonAnimationSamplingChoice->Thaw();
    if (this->sceneSettings.GetSkeletonAnimationSampleInterval() == 0 ||
        this->skeletonAnimationSamplingChoice->GetSelection() < 0)
    {
        this->skeletonAnimationSamplingChoice->SetSelection(0);
    }
    
    //Vertex animation sampling value
    this->vertexAnimationSamplingText->SetValue(this->sceneSettings.GetVertexAnimationSampleValueText());

    //Vertex animation sampling combo
    this->vertexAnimationSamplingChoice->Freeze();
    for (int i = 0; i < Strings::SCENE_SAMPLE_TYPE_COUNT; i++)
    {
        this->vertexAnimationSamplingChoice->Append(Strings::SCENE_SAMPLE_TYPES_TEXT[i]);
        this->vertexAnimationSamplingChoice->SetClientData(i, (void*)&Strings::SCENE_SAMPLE_TYPES_VALUE[i]);
        if (this->sceneSettings.GetVertexAnimationSampleType() == Strings::SCENE_SAMPLE_TYPES_VALUE[i])
            this->vertexAnimationSamplingChoice->SetSelection(i);
    }
    this->vertexAnimationSamplingChoice->Thaw();
    if (this->sceneSettings.GetVertexAnimationSampleInterval() == 0 ||
        this->vertexAnimationSamplingChoice->GetSelection() < 0)
    {
        this->vertexAnimationSamplingChoice->SetSelection(0);
    }

    //Node animation sampling value
    this->nodeAnimationSamplingText->SetValue(this->sceneSettings.GetNodeAnimationSampleValueText());
    
    //Node animation sampling combo
    this->nodeAnimationSamplingChoice->Freeze();
    for (int i = 0; i < Strings::SCENE_SAMPLE_TYPE_COUNT; i++)
    {
        this->nodeAnimationSamplingChoice->Append(Strings::SCENE_SAMPLE_TYPES_TEXT[i]);
        this->nodeAnimationSamplingChoice->SetClientData(i, (void*)&Strings::SCENE_SAMPLE_TYPES_VALUE[i]);
        if (this->sceneSettings.GetNodeAnimationSampleType() == Strings::SCENE_SAMPLE_TYPES_VALUE[i])
            this->nodeAnimationSamplingChoice->SetSelection(i);
    }
    this->nodeAnimationSamplingChoice->Thaw();
    if (this->sceneSettings.GetNodeAnimationSampleInterval() == 0 ||
        this->nodeAnimationSamplingChoice->GetSelection() < 0)
    {
        this->nodeAnimationSamplingChoice->SetSelection(0);
    }

    //Final updates
    UpdateNodeAnimationSamplingTypeEdit();    
    UpdateSkeletonAnimationSamplingTypeEdit();
    UpdateVertexAnimationSamplingTypeEdit();

    this->settingGuiData = false;

    return true;
}

void SceneSettingsDialog_AnimationsPage::OnSkeletonAnimationSamplingChoice(wxCommandEvent& event)
{    
    if (this->settingGuiData)
        return;

    GetGUIData();

    UpdateSkeletonAnimationSamplingTypeEdit();    
}

void SceneSettingsDialog_AnimationsPage::OnVertexAnimationSamplingChoice(wxCommandEvent& event)
{    
    if (this->settingGuiData)
        return;

    GetGUIData();

    UpdateVertexAnimationSamplingTypeEdit();    
}

void SceneSettingsDialog_AnimationsPage::OnNodeAnimationSamplingChoice(wxCommandEvent& event)
{    
    if (this->settingGuiData)
        return;

    GetGUIData();

    UpdateNodeAnimationSamplingTypeEdit();    
}

void SceneSettingsDialog_AnimationsPage::UpdateSkeletonAnimationSamplingTypeEdit()
{
    int selectedIndex = this->skeletonAnimationSamplingChoice->GetSelection();
    bool show = selectedIndex > 0;
    this->skeletonAnimationSamplingText->Show(show);
    Layout();
}

void SceneSettingsDialog_AnimationsPage::UpdateVertexAnimationSamplingTypeEdit()
{
    int selectedIndex = this->vertexAnimationSamplingChoice->GetSelection();
    bool show = selectedIndex > 0;
    this->vertexAnimationSamplingText->Show(show);
    Layout();
}

void SceneSettingsDialog_AnimationsPage::UpdateNodeAnimationSamplingTypeEdit()
{
    int selectedIndex = this->nodeAnimationSamplingChoice->GetSelection();
    bool show = selectedIndex > 0;
    this->nodeAnimationSamplingText->Show(show);
    Layout();
}
