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
#include "GlobalSettingsDialog_AnimationsPage.hpp"
#include "FinjinGlobalSettings.hpp"
#include "Strings.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "ApplicationAccessor.hpp"
#include "RelativeLocationPrompt.hpp"
#include "Dialogs.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
const wxString GlobalSettingsDialog_AnimationsPage::TITLE(wxT("Animations"));


//Local data--------------------------------------------------------------------
static const wxChar* SAMPLE_TYPES_TEXT[] = {wxT("Seconds Between Samples"), wxT("Samples Per Second")};
static const SampleType SAMPLE_TYPES_VALUE[] = {SampleType::INTERVAL, SampleType::RATE};
static const int SAMPLE_TYPE_COUNT = FINJIN_COUNT_OF(SAMPLE_TYPES_VALUE);


//Local functions---------------------------------------------------------------
static double GetSampleInterval(SampleType sampleType, double sampling)
{
    double sampleInterval = 0;
    if (sampleType == SampleType::INTERVAL)
        sampleInterval = sampling;
    else if (sampleType == SampleType::RATE)
    {
        double rate = sampling;
        if (rate != 0)
            sampleInterval = 1 / rate;
    }
    if (sampleInterval <= 0)
        sampleInterval = FinjinGlobalSettings::DEFAULT_SAMPLE_INTERVAL;
    return sampleInterval;
}


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(GlobalSettingsDialog_AnimationsPage, SettingsPage)
END_EVENT_TABLE()

GlobalSettingsDialog_AnimationsPage::GlobalSettingsDialog_AnimationsPage(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* meshAnimationsSizer;
    meshAnimationsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Animation Sampling") ), wxVERTICAL );

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
    skeletonAnimationSamplingText->SetMaxLength( 0 );
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
    vertexAnimationSamplingText->SetMaxLength( 0 );
    vertexAnimationSamplingValuesSizer->Add( vertexAnimationSamplingText, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    wxArrayString vertexAnimationSamplingChoiceChoices;
    vertexAnimationSamplingChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, vertexAnimationSamplingChoiceChoices, 0 );
    vertexAnimationSamplingChoice->SetSelection( 0 );
    vertexAnimationSamplingValuesSizer->Add( vertexAnimationSamplingChoice, 1, wxALL, 5 );


    vertexAnimationSamplingSizer->Add( vertexAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );


    skeletonAndVertexanimationSamplingSizer->Add( vertexAnimationSamplingSizer, 1, wxEXPAND, 5 );


    meshAnimationsSizer->Add( skeletonAndVertexanimationSamplingSizer, 1, wxEXPAND, 5 );


    wxBoxSizer* nodeAnimationSamplingSizer;
    nodeAnimationSamplingSizer = new wxBoxSizer( wxVERTICAL );

    nodeAnimationSamplingLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Node Animation Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    nodeAnimationSamplingLabel->Wrap( -1 );
    nodeAnimationSamplingSizer->Add( nodeAnimationSamplingLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* nodeAnimationSamplingValuesSizer;
    nodeAnimationSamplingValuesSizer = new wxBoxSizer( wxHORIZONTAL );

    nodeAnimationSamplingText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    nodeAnimationSamplingText->SetMaxLength( 0 );
    nodeAnimationSamplingValuesSizer->Add( nodeAnimationSamplingText, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    wxArrayString nodeAnimationSamplingChoiceChoices;
    nodeAnimationSamplingChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, nodeAnimationSamplingChoiceChoices, 0 );
    nodeAnimationSamplingChoice->SetSelection( 0 );
    nodeAnimationSamplingValuesSizer->Add( nodeAnimationSamplingChoice, 1, wxALL, 5 );


    nodeAnimationSamplingSizer->Add( nodeAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );


    meshAnimationsSizer->Add( nodeAnimationSamplingSizer, 1, wxEXPAND, 5 );


    topSizer->Add( meshAnimationsSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

    SetSizer(topSizer);
    Layout();
}

GlobalSettingsDialog_AnimationsPage::~GlobalSettingsDialog_AnimationsPage()
{
}

bool GlobalSettingsDialog_AnimationsPage::GetGUIData()
{
    auto& settings = FinjinGlobalSettings::GetInstance();

    //Skeleton animation sampling combo and text
    int selectedSkeletonAnimationSampleTypeIndex = this->skeletonAnimationSamplingChoice->GetSelection();
    settings.skeletonAnimationSampleType = (SampleType)*(int*)this->skeletonAnimationSamplingChoice->GetClientData(selectedSkeletonAnimationSampleTypeIndex);
    float skeletonAnimationSampling = StringUtilities::ToFloat(this->skeletonAnimationSamplingText->GetValue());
    settings.skeletonAnimationSampleInterval = GetSampleInterval(settings.skeletonAnimationSampleType, skeletonAnimationSampling);

    //Vertex animation sampling combo and text
    int selectedVertexAnimationSampleTypeIndex = this->vertexAnimationSamplingChoice->GetSelection();
    settings.vertexAnimationSampleType = (SampleType)*(int*)this->vertexAnimationSamplingChoice->GetClientData(selectedVertexAnimationSampleTypeIndex);
    float vertexAnimationSampling = StringUtilities::ToFloat(this->vertexAnimationSamplingText->GetValue());
    settings.vertexAnimationSampleInterval = GetSampleInterval(settings.vertexAnimationSampleType, vertexAnimationSampling);

    //Node animation sampling combo and text
    int selectedNodeAnimationSampleTypeIndex = this->nodeAnimationSamplingChoice->GetSelection();
    settings.nodeAnimationSampleType = (SampleType)*(int*)this->nodeAnimationSamplingChoice->GetClientData(selectedNodeAnimationSampleTypeIndex);
    float nodeAnimationSampling = StringUtilities::ToFloat(this->nodeAnimationSamplingText->GetValue());
    settings.nodeAnimationSampleInterval = GetSampleInterval(settings.nodeAnimationSampleType, nodeAnimationSampling);

    return true;
}

bool GlobalSettingsDialog_AnimationsPage::SetGUIData()
{
    auto& settings = FinjinGlobalSettings::GetInstance();

    //Skeleton animation sampling value
    this->skeletonAnimationSamplingText->SetValue(PropertyValues::ChooseAnimationSampleValue(settings.skeletonAnimationSampleType, settings.skeletonAnimationSampleInterval));

    //Skeleton animation sampling combo
    this->skeletonAnimationSamplingChoice->Freeze();
    for (int i = 0; i < SAMPLE_TYPE_COUNT; i++)
    {
        this->skeletonAnimationSamplingChoice->Append(SAMPLE_TYPES_TEXT[i]);
        this->skeletonAnimationSamplingChoice->SetClientData(i, (void*)&SAMPLE_TYPES_VALUE[i]);
        if (settings.skeletonAnimationSampleType == SAMPLE_TYPES_VALUE[i])
            this->skeletonAnimationSamplingChoice->SetSelection(i);
    }
    this->skeletonAnimationSamplingChoice->Thaw();
    if (settings.skeletonAnimationSampleInterval == 0 ||
        this->skeletonAnimationSamplingChoice->GetSelection() < 0)
    {
        this->skeletonAnimationSamplingChoice->SetSelection(0);
    }

    //Vertex animation sampling value
    this->vertexAnimationSamplingText->SetValue(PropertyValues::ChooseAnimationSampleValue(settings.vertexAnimationSampleType, settings.vertexAnimationSampleInterval));

    //Vertex animation sampling combo
    this->vertexAnimationSamplingChoice->Freeze();
    for (int i = 0; i < SAMPLE_TYPE_COUNT; i++)
    {
        this->vertexAnimationSamplingChoice->Append(SAMPLE_TYPES_TEXT[i]);
        this->vertexAnimationSamplingChoice->SetClientData(i, (void*)&SAMPLE_TYPES_VALUE[i]);
        if (settings.vertexAnimationSampleType == SAMPLE_TYPES_VALUE[i])
            this->vertexAnimationSamplingChoice->SetSelection(i);
    }
    this->vertexAnimationSamplingChoice->Thaw();
    if (settings.vertexAnimationSampleInterval == 0 ||
        this->vertexAnimationSamplingChoice->GetSelection() < 0)
    {
        this->vertexAnimationSamplingChoice->SetSelection(0);
    }

    //Node animation sampling value
    this->nodeAnimationSamplingText->SetValue(PropertyValues::ChooseAnimationSampleValue(settings.nodeAnimationSampleType, settings.nodeAnimationSampleInterval));

    //Node animation sampling combo
    this->nodeAnimationSamplingChoice->Freeze();
    for (int i = 0; i < SAMPLE_TYPE_COUNT; i++)
    {
        this->nodeAnimationSamplingChoice->Append(SAMPLE_TYPES_TEXT[i]);
        this->nodeAnimationSamplingChoice->SetClientData(i, (void*)&SAMPLE_TYPES_VALUE[i]);
        if (settings.nodeAnimationSampleType == SAMPLE_TYPES_VALUE[i])
            this->nodeAnimationSamplingChoice->SetSelection(i);
    }
    this->nodeAnimationSamplingChoice->Thaw();
    if (settings.nodeAnimationSampleInterval == 0 ||
        this->nodeAnimationSamplingChoice->GetSelection() < 0)
    {
        this->nodeAnimationSamplingChoice->SetSelection(0);
    }

    return true;
}
