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
#include "SceneSettingsDialog_EnvironmentPage.hpp"
#include "ChooseMaterialsPrompt.hpp"
#include "ChooseObjectsPrompt.hpp"
#include "ApplicationControls.hpp"
#include "ApplicationAccessor.hpp"
#include "ObjectFilterType.hpp"
#include "StringUtilities.hpp"
#include "EnvironmentFar.hpp"
#include "Strings.hpp"
#include "PropertyValues.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
const wxString SceneSettingsDialog_EnvironmentPage::TITLE(wxT("Environment"));


//Local data--------------------------------------------------------------------
static const wxChar* VALUE_SOURCE_SYNC_TEXT[] = {wxT("Separate from MASTER_NAME"), wxT("Copy from MASTER_NAME"), wxT("Copy to MASTER_NAME")};
static const ValueSourceSync VALUE_SOURCE_SYNC_VALUE[] = {ValueSourceSync::SEPARATE, ValueSourceSync::FROM_MASTER_TO_SLAVE, ValueSourceSync::FROM_SLAVE_TO_MASTER};
static const int VALUE_SOURCE_SYNC_COUNT = FINJIN_COUNT_OF(VALUE_SOURCE_SYNC_VALUE);

static const wxChar* FOG_MODES_TEXT[] = {wxT("None"), wxT("Linear"), wxT("Exp")};
static const wxChar* FOG_MODES_VALUE[] = {wxT(""), wxT("linear"), wxT("exp")};
static const int FOG_MODE_COUNT = FINJIN_COUNT_OF(FOG_MODES_VALUE);


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(SceneSettingsDialog_EnvironmentPage, SettingsPage)
    EVT_COLOURPICKER_CHANGED(SceneSettingsDialog_EnvironmentPage::BACKGROUND_COLOR, SceneSettingsDialog_EnvironmentPage::OnBackgroundColorChanged)
    EVT_CHOICE(SceneSettingsDialog_EnvironmentPage::BACKGROUND_COLOR_SYNC_CHOICE, SceneSettingsDialog_EnvironmentPage::OnBackgroundColorSyncChoice)
    EVT_COLOURPICKER_CHANGED(SceneSettingsDialog_EnvironmentPage::AMBIENT_LIGHT_COLOR, SceneSettingsDialog_EnvironmentPage::OnAmbientLightColorChanged)
    EVT_CHOICE(SceneSettingsDialog_EnvironmentPage::AMBIENT_LIGHT_COLOR_SYNC_CHOICE, SceneSettingsDialog_EnvironmentPage::OnAmbientLightColorSyncChoice)
    EVT_SPINNER_VALUE_CHANGED(SceneSettingsDialog_EnvironmentPage::NEAR_DISTANCE_SPINNER, SceneSettingsDialog_EnvironmentPage::OnNearDistanceSpinner)
    EVT_RADIOBUTTON(SceneSettingsDialog_EnvironmentPage::FAR_DISTANCE_RADIO_BUTTON, SceneSettingsDialog_EnvironmentPage::OnFarDistanceRadioButton)
    EVT_RADIOBUTTON(SceneSettingsDialog_EnvironmentPage::FAR_BOX_OBJECT_RADIO_BUTTON, SceneSettingsDialog_EnvironmentPage::OnFarBoxObjectRadioButton)
    EVT_SPINNER_VALUE_CHANGED(SceneSettingsDialog_EnvironmentPage::FAR_DISTANCE_SPINNER, SceneSettingsDialog_EnvironmentPage::OnFarDistanceSpinner)
    EVT_BUTTON(SceneSettingsDialog_EnvironmentPage::FAR_BOX_OBJECT_BUTTON, SceneSettingsDialog_EnvironmentPage::OnFarBoxObjectButton)
    EVT_CHOICE(SceneSettingsDialog_EnvironmentPage::FOG_MODE_CHOICE, SceneSettingsDialog_EnvironmentPage::OnFogModeChoice)
    EVT_CHOICE(SceneSettingsDialog_EnvironmentPage::FOG_SYNC_CHOICE, SceneSettingsDialog_EnvironmentPage::OnFogSyncChoice)
    EVT_SPINNER_VALUE_CHANGED(SceneSettingsDialog_EnvironmentPage::FOG_START_SPINNER, SceneSettingsDialog_EnvironmentPage::OnFogStartSpinner)
    EVT_SPINNER_VALUE_CHANGED(SceneSettingsDialog_EnvironmentPage::FOG_END_SPINNER, SceneSettingsDialog_EnvironmentPage::OnFogEndSpinner)
    EVT_SPINNER_VALUE_CHANGED(SceneSettingsDialog_EnvironmentPage::FOG_DENSITY_SPINNER, SceneSettingsDialog_EnvironmentPage::OnFogDensitySpinner)
    EVT_COLOURPICKER_CHANGED(SceneSettingsDialog_EnvironmentPage::FOG_COLOR, SceneSettingsDialog_EnvironmentPage::OnFogColorChanged)
    EVT_COLOURPICKER_CHANGED(SceneSettingsDialog_EnvironmentPage::SHADOW_COLOR, SceneSettingsDialog_EnvironmentPage::OnShadowColorChanged)
    EVT_SPINNER_VALUE_CHANGED(SceneSettingsDialog_EnvironmentPage::SHADOW_FAR_DISTANCE_SPINNER, SceneSettingsDialog_EnvironmentPage::OnShadowFarDistanceSpinner)
END_EVENT_TABLE()

SceneSettingsDialog_EnvironmentPage::SceneSettingsDialog_EnvironmentPage( wxWindow* parent, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    this->sceneSettings = sceneSettings;

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);

    wxGridSizer* headerSizer;
    headerSizer = new wxGridSizer( 1, 2, 0, 0 );

    exportCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Environment Settings"), wxDefaultPosition, wxDefaultSize, 0 );

    headerSizer->Add( exportCheckbox, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5 );

    topSizer->Add( headerSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* colorsSizer;
    colorsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Colors") ), wxVERTICAL );

    wxFlexGridSizer* colorsGridSizer;
    colorsGridSizer = new wxFlexGridSizer( 2, 3, 0, 0 );
    colorsGridSizer->SetFlexibleDirection( wxBOTH );
    colorsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    backgroundColorLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Background"), wxDefaultPosition, wxDefaultSize, 0 );
    backgroundColorLabel->Wrap( -1 );
    colorsGridSizer->Add( backgroundColorLabel, 0, wxALL, 5 );

    backgroundColor = new Finjin::Exporter::ColorPickerControl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    colorsGridSizer->Add( backgroundColor, 0, wxLEFT, 5 );

    wxArrayString backgroundColorSyncChoiceChoices;
    backgroundColorSyncChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, backgroundColorSyncChoiceChoices, 0 );
    backgroundColorSyncChoice->SetSelection( 0 );
    colorsGridSizer->Add( backgroundColorSyncChoice, 0, wxLEFT|wxRIGHT, 5 );

    ambientLightColorLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Ambient Light"), wxDefaultPosition, wxDefaultSize, 0 );
    ambientLightColorLabel->Wrap( -1 );
    colorsGridSizer->Add( ambientLightColorLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    ambientLightColor = new Finjin::Exporter::ColorPickerControl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    colorsGridSizer->Add( ambientLightColor, 0, wxLEFT, 5 );

    wxArrayString ambientLightColorSyncChoiceChoices;
    ambientLightColorSyncChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ambientLightColorSyncChoiceChoices, 0 );
    ambientLightColorSyncChoice->SetSelection( 0 );
    colorsGridSizer->Add( ambientLightColorSyncChoice, 0, wxLEFT|wxRIGHT, 5 );

    colorsSizer->Add( colorsGridSizer, 1, wxEXPAND, 5 );

    topSizer->Add( colorsSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* environmentRangeSizer;
    environmentRangeSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Environment Range") ), wxVERTICAL );

    wxFlexGridSizer* environmentRangeGridSizer;
    environmentRangeGridSizer = new wxFlexGridSizer( 4, 2, 0, 0 );
    environmentRangeGridSizer->SetFlexibleDirection( wxBOTH );
    environmentRangeGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    nearDistanceLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Near Distance"), wxDefaultPosition, wxDefaultSize, 0 );
    nearDistanceLabel->Wrap( -1 );
    environmentRangeGridSizer->Add( nearDistanceLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    nearDistanceSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(110, -1), 0, 0, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE_RANGE, SpinnerControl::WORLD_UNIT, 2);
    environmentRangeGridSizer->Add( nearDistanceSpinner, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

    farLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Far"), wxDefaultPosition, wxDefaultSize, 0 );
    farLabel->Wrap( -1 );
    environmentRangeGridSizer->Add( farLabel, 0, wxRIGHT|wxLEFT, 5 );


    environmentRangeGridSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    wxBoxSizer* farDistanceLeftSizer;
    farDistanceLeftSizer = new wxBoxSizer( wxHORIZONTAL );


    farDistanceLeftSizer->Add( 20, 0, 1, wxEXPAND, 5 );

    farDistanceButton = new wxRadioButton( this, wxID_ANY, wxT("Distance"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP | wxRB_SINGLE );
    farDistanceLeftSizer->Add( farDistanceButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    environmentRangeGridSizer->Add( farDistanceLeftSizer, 1, 0, 5 );

    wxBoxSizer* farDistanceRightSizer;
    farDistanceRightSizer = new wxBoxSizer( wxHORIZONTAL );

    farDistanceSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(110, -1), 0, 0, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE_RANGE, SpinnerControl::WORLD_UNIT, 2);
    farDistanceRightSizer->Add( farDistanceSpinner, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

    environmentRangeGridSizer->Add( farDistanceRightSizer, 1, wxEXPAND, 5 );

    wxBoxSizer* farBoxObjectLeftSizer;
    farBoxObjectLeftSizer = new wxBoxSizer( wxHORIZONTAL );


    farBoxObjectLeftSizer->Add( 20, 0, 1, wxEXPAND, 5 );

    farBoxObjectButton = new wxRadioButton( this, wxID_ANY, wxT("Box Object"), wxDefaultPosition, wxDefaultSize, wxRB_SINGLE );
    farBoxObjectLeftSizer->Add( farBoxObjectButton, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

    environmentRangeGridSizer->Add( farBoxObjectLeftSizer, 1, wxEXPAND, 5 );

    wxBoxSizer* farBoxObjectRightSizer;
    farBoxObjectRightSizer = new wxBoxSizer( wxHORIZONTAL );

    farBoxObjectText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 220,-1 ), wxTE_READONLY );

    farBoxObjectRightSizer->Add( farBoxObjectText, 0, wxLEFT, 5 );

    chooseFarBoxObjectButton = new wxButton( this, wxID_ANY, wxT("Choose..."), wxDefaultPosition, wxDefaultSize, 0 );
    farBoxObjectRightSizer->Add( chooseFarBoxObjectButton, 0, wxRIGHT|wxLEFT, 5 );

    environmentRangeGridSizer->Add( farBoxObjectRightSizer, 1, wxEXPAND, 5 );

    environmentRangeSizer->Add( environmentRangeGridSizer, 1, wxEXPAND, 5 );

    topSizer->Add( environmentRangeSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* fogSizer;
    fogSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Fog") ), wxVERTICAL );

    wxFlexGridSizer* fogHorizontalSizer;
    fogHorizontalSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
    fogHorizontalSizer->SetFlexibleDirection( wxBOTH );
    fogHorizontalSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    fogModeLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Mode"), wxDefaultPosition, wxSize(40,-1), 0 );
    fogModeLabel->Wrap( -1 );
    fogHorizontalSizer->Add( fogModeLabel, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5 );

    wxBoxSizer* fogModeSizer;
    fogModeSizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString fogModeChoiceChoices;
    fogModeChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, fogModeChoiceChoices, 0 );
    fogModeChoice->SetSelection( 0 );
    fogModeSizer->Add( fogModeChoice, 0, 0, 5 );


    fogModeSizer->Add( 10, 0, 1, wxEXPAND, 5 );

    fogDensityLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Density"), wxDefaultPosition, wxDefaultSize, 0 );
    fogDensityLabel->Wrap( -1 );
    fogModeSizer->Add( fogDensityLabel, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT, 5 );

    fogDensitySpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(90, -1), 0, 0, 1, SpinnerControl::FLOAT_UNIT, 5);
    fogModeSizer->Add( fogDensitySpinner, 0, wxRIGHT, 5 );

    fogStartLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
    fogStartLabel->Wrap( -1 );
    fogModeSizer->Add( fogStartLabel, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT, 5 );

    fogStartSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(70, -1), 0, 0, 1.0, SpinnerControl::FLOAT_UNIT, 2);
    fogModeSizer->Add( fogStartSpinner, 0, 0, 5 );

    fogEndLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("End"), wxDefaultPosition, wxDefaultSize, 0 );
    fogEndLabel->Wrap( -1 );
    fogModeSizer->Add( fogEndLabel, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5 );

    fogEndSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(70, -1), 0, 0, 1.0, SpinnerControl::FLOAT_UNIT, 2);
    fogModeSizer->Add( fogEndSpinner, 0, 0, 5 );

    fogHorizontalSizer->Add( fogModeSizer, 1, wxEXPAND, 5 );

    fogColorLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Color"), wxDefaultPosition, wxDefaultSize, 0 );
    fogColorLabel->Wrap( -1 );
    fogHorizontalSizer->Add( fogColorLabel, 0, wxALL, 5 );

    fogColor = new Finjin::Exporter::ColorPickerControl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    fogHorizontalSizer->Add( fogColor, 0, wxRIGHT|wxTOP, 5 );

    fogSizer->Add( fogHorizontalSizer, 1, wxEXPAND, 5 );

    wxArrayString fogSyncChoiceChoices;
    fogSyncChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, fogSyncChoiceChoices, 0 );
    fogSyncChoice->SetSelection( 0 );
    fogSizer->Add( fogSyncChoice, 0, wxTOP|wxBOTTOM|wxLEFT|wxRIGHT, 5 );

    topSizer->Add( fogSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* shadowsSizer;
    shadowsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Shadows") ), wxVERTICAL );

    wxFlexGridSizer* shadowsGridSizer;
    shadowsGridSizer = new wxFlexGridSizer( 3, 2, 0, 0 );
    shadowsGridSizer->SetFlexibleDirection( wxBOTH );
    shadowsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    shadowTechniqueLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Technique"), wxDefaultPosition, wxDefaultSize, 0 );
    shadowTechniqueLabel->Wrap( -1 );
    shadowsGridSizer->Add( shadowTechniqueLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );



    wxBoxSizer* shadowTechniqueSizer;
    shadowTechniqueSizer = new wxBoxSizer( wxHORIZONTAL );

    shadowTechniqueText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
    shadowTechniqueText->SetMaxLength( 0 );
    shadowTechniqueSizer->Add( shadowTechniqueText, 0, wxALL, 5 );


    shadowsGridSizer->Add( shadowTechniqueSizer, 1, wxEXPAND, 5 );

    shadowFarDistanceLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Far Distance"), wxDefaultPosition, wxDefaultSize, 0 );
    shadowFarDistanceLabel->Wrap( -1 );
    shadowsGridSizer->Add( shadowFarDistanceLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer* shadowFarDistanceSizer;
    shadowFarDistanceSizer = new wxBoxSizer( wxHORIZONTAL );

    shadowFarDistanceSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(90, -1), 0, 0, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE_RANGE, SpinnerControl::WORLD_UNIT, 2);
    shadowFarDistanceSizer->Add( shadowFarDistanceSpinner, 0, wxRIGHT|wxLEFT, 5 );


    shadowsGridSizer->Add( shadowFarDistanceSizer, 1, wxEXPAND, 5 );

    shadowColorLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Color"), wxDefaultPosition, wxDefaultSize, 0 );
    shadowColorLabel->Wrap( -1 );
    shadowsGridSizer->Add( shadowColorLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    shadowColor = new Finjin::Exporter::ColorPickerControl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    shadowsGridSizer->Add( shadowColor, 0, wxRIGHT|wxLEFT, 5 );


    shadowsSizer->Add( shadowsGridSizer, 1, wxEXPAND, 5 );

    topSizer->Add( shadowsSizer, 0, wxEXPAND|wxTOP, 5 );

    this->ambientLightColor->SetId(AMBIENT_LIGHT_COLOR);
    this->ambientLightColorSyncChoice->SetId(AMBIENT_LIGHT_COLOR_SYNC_CHOICE);
    this->backgroundColor->SetId(BACKGROUND_COLOR);
    this->backgroundColorSyncChoice->SetId(BACKGROUND_COLOR_SYNC_CHOICE);
    this->farDistanceButton->SetId(FAR_DISTANCE_RADIO_BUTTON);
    this->farDistanceSpinner->SetId(FAR_DISTANCE_SPINNER);
    this->farBoxObjectButton->SetId(FAR_BOX_OBJECT_RADIO_BUTTON);
    this->chooseFarBoxObjectButton->SetId(FAR_BOX_OBJECT_BUTTON);
    this->fogModeChoice->SetId(FOG_MODE_CHOICE);
    this->fogSyncChoice->SetId(FOG_SYNC_CHOICE);
    this->fogStartSpinner->SetId(FOG_START_SPINNER);
    this->fogEndSpinner->SetId(FOG_END_SPINNER);
    this->fogDensitySpinner->SetId(FOG_DENSITY_SPINNER);
    this->fogColor->SetId(FOG_COLOR);
    this->shadowColor->SetId(SHADOW_COLOR);
    this->shadowFarDistanceSpinner->SetId(SHADOW_FAR_DISTANCE_SPINNER);

    SetSizer(topSizer);
    Layout();
}

SceneSettingsDialog_EnvironmentPage::~SceneSettingsDialog_EnvironmentPage()
{
}

bool SceneSettingsDialog_EnvironmentPage::GetGUIData()
{
    //Export
    this->sceneSettings.SetExportEnvironment(this->exportCheckbox->GetValue());

    //Ambient light color
    int ambientLightColorSyncIndex = this->ambientLightColorSyncChoice->GetSelection();
    ValueSourceSync ambientLightColorSync = (ValueSourceSync)*(int*)this->ambientLightColorSyncChoice->GetClientData(ambientLightColorSyncIndex);
    this->sceneSettings.SetAmbientLightColorValue(ambientLightColorSync, this->ambientLightColor->GetColour());

    //Background color
    int backgroundColorSyncIndex = this->backgroundColorSyncChoice->GetSelection();
    ValueSourceSync backgroundColorSync = (ValueSourceSync)*(int*)this->backgroundColorSyncChoice->GetClientData(backgroundColorSyncIndex);
    this->sceneSettings.SetBackgroundColorValue(backgroundColorSync, this->backgroundColor->GetColour());

    //Environment range near
    this->sceneSettings.SetEnvironmentNear(this->nearDistanceSpinner->GetValue());

    //Environment range far type radio
    this->sceneSettings.SetEnvironmentFar(this->farDistanceSpinner->GetValue());
    this->sceneSettings.SetEnvironmentFarObject(GetEnvironmentObject());

    //Fog sync, mode, color, density, range
    int fogSyncIndex = this->fogSyncChoice->GetSelection();
    ValueSourceSync fogSync = (ValueSourceSync)*(int*)this->fogSyncChoice->GetClientData(fogSyncIndex);
    int fogModeIndex = this->fogModeChoice->GetSelection();
    this->sceneSettings.SetFogValues(fogSync, (wxChar*)this->fogModeChoice->GetClientData(fogModeIndex), this->fogColor->GetColour(), this->fogDensitySpinner->GetValue(), this->fogStartSpinner->GetValue(), this->fogEndSpinner->GetValue(), -1, TimeAccessor::GetCurrentTime());

    //Technique
    this->sceneSettings.SetShadowTechnique(this->shadowTechniqueText->GetValue());

    //Color
    this->sceneSettings.SetShadowColor(this->shadowColor->GetColour());

    //Shadow far distance
    this->sceneSettings.SetShadowFarDistance(this->shadowFarDistanceSpinner->GetValue());

    return true;
}

bool SceneSettingsDialog_EnvironmentPage::SetGUIData()
{
    //Export
    this->exportCheckbox->SetValue(this->sceneSettings.GetExportEnvironment());

    //Ambient light color
    this->ambientLightColor->SetColour(this->sceneSettings.GetAmbientLightColor());
    this->ambientLightColorSyncChoice->Freeze();
    for (int i = 0; i < VALUE_SOURCE_SYNC_COUNT; i++)
    {
        wxString text = VALUE_SOURCE_SYNC_TEXT[i];
        text.Replace(wxT("MASTER_NAME"), ApplicationAccessor::APPLICATION_SHORT_NAME);
        this->ambientLightColorSyncChoice->Append(text);
        this->ambientLightColorSyncChoice->SetClientData(i, (void*)&VALUE_SOURCE_SYNC_VALUE[i]);
        if (this->sceneSettings.GetAmbientLightColorSync() == VALUE_SOURCE_SYNC_VALUE[i])
            this->ambientLightColorSyncChoice->SetSelection(i);
    }
    this->ambientLightColorSyncChoice->Thaw();
    if (this->ambientLightColorSyncChoice->GetSelection() < 0)
        this->ambientLightColorSyncChoice->SetSelection(0);

    //Background color
    this->backgroundColor->SetColour(this->sceneSettings.GetBackgroundColor());
    this->backgroundColorSyncChoice->Freeze();
    for (int i = 0; i < VALUE_SOURCE_SYNC_COUNT; i++)
    {
        wxString text = VALUE_SOURCE_SYNC_TEXT[i];
        text.Replace(wxT("MASTER_NAME"), ApplicationAccessor::APPLICATION_SHORT_NAME);
        this->backgroundColorSyncChoice->Append(text);
        this->backgroundColorSyncChoice->SetClientData(i, (void*)&VALUE_SOURCE_SYNC_VALUE[i]);
        if (this->sceneSettings.GetBackgroundColorSync() == VALUE_SOURCE_SYNC_VALUE[i])
            this->backgroundColorSyncChoice->SetSelection(i);
    }
    this->backgroundColorSyncChoice->Thaw();
    if (this->backgroundColorSyncChoice->GetSelection() < 0)
        this->backgroundColorSyncChoice->SetSelection(0);

    //Environment range near/spin
    this->nearDistanceSpinner->SetValue(this->sceneSettings.GetEnvironmentNear());

    //Environment range far type radio
    this->environmentFarObject = this->sceneSettings.GetEnvironmentFarObject();
    this->farDistanceButton->SetValue(!this->environmentFarObject.IsValid());
    this->farBoxObjectButton->SetValue(this->environmentFarObject.IsValid());

    //Environment range far/spin
    this->farDistanceSpinner->SetValue(this->sceneSettings.GetEnvironmentFar());

    //Environment range far box object
    this->farBoxObjectText->SetValue(this->environmentFarObject.GetFullName());

    //Fog
    wxString fogMode;
    FinjinColor fogColor;
    float fogDensity, fogStart, fogEnd;
    this->sceneSettings.GetFogValues(fogMode, fogColor, fogDensity, fogStart, fogEnd, -1);

    //Fog mode
    this->fogModeChoice->Freeze();
    for (int i = 0; i < FOG_MODE_COUNT; i++)
    {
        this->fogModeChoice->Append(FOG_MODES_TEXT[i]);
        this->fogModeChoice->SetClientData(i, (wxChar*)FOG_MODES_VALUE[i]);
        if (fogMode == FOG_MODES_VALUE[i])
            this->fogModeChoice->SetSelection(i);
    }
    this->fogModeChoice->Thaw();
    if (this->fogModeChoice->GetSelection() < 0)
        this->fogModeChoice->SetSelection(0);

    //Fog exp density
    this->fogDensitySpinner->SetValue(fogDensity);

    //Fog range
    this->fogStartSpinner->SetValue(fogStart);
    this->fogEndSpinner->SetValue(fogEnd);

    //Fog color
    this->fogColor->SetColour(fogColor);

    //Fog sync
    this->fogSyncChoice->Freeze();
    for (int i = 0; i < VALUE_SOURCE_SYNC_COUNT; i++)
    {
        wxString text = VALUE_SOURCE_SYNC_TEXT[i];
        text.Replace(wxT("MASTER_NAME"), ApplicationAccessor::APPLICATION_SHORT_NAME);
        this->fogSyncChoice->Append(text);
        this->fogSyncChoice->SetClientData(i, (void*)&VALUE_SOURCE_SYNC_VALUE[i]);
        if (this->sceneSettings.GetFogSync() == VALUE_SOURCE_SYNC_VALUE[i])
            this->fogSyncChoice->SetSelection(i);
    }
    this->fogSyncChoice->Thaw();
    if (this->fogSyncChoice->GetSelection() < 0)
        this->fogSyncChoice->SetSelection(0);

    //Technique
    this->shadowTechniqueText->SetValue(this->sceneSettings.GetShadowTechnique());

    //Color
    this->shadowColor->SetColour(this->sceneSettings.GetShadowColor());

    //Shadow far distance
    this->shadowFarDistanceSpinner->SetValue(this->sceneSettings.GetShadowFarDistance());

    UpdateAmbientLightColorControls();
    UpdateBackgroundColorControls();
    UpdateFogModeControls();

    return true;
}

void SceneSettingsDialog_EnvironmentPage::UpdateAmbientLightColorControls(bool syncChanged)
{
    ValueSourceSync sync;
    if (syncChanged)
    {
        int ambientLightColorSyncIndex = this->ambientLightColorSyncChoice->GetSelection();
        sync = (ValueSourceSync)*(int*)this->ambientLightColorSyncChoice->GetClientData(ambientLightColorSyncIndex);
    }
    else
        sync = this->sceneSettings.GetAmbientLightColorSync();

    if (sync == ValueSourceSync::FROM_MASTER_TO_SLAVE)
        this->ambientLightColor->SetColour(ApplicationAccessor::GetAmbientLightColor());
    this->ambientLightColor->Enable(sync != ValueSourceSync::FROM_MASTER_TO_SLAVE);
}

void SceneSettingsDialog_EnvironmentPage::UpdateBackgroundColorControls(bool syncChanged)
{
    ValueSourceSync sync;
    if (syncChanged)
    {
        int backgroundColorSyncIndex = this->backgroundColorSyncChoice->GetSelection();
        sync = (ValueSourceSync)*(int*)this->backgroundColorSyncChoice->GetClientData(backgroundColorSyncIndex);
    }
    else
        sync = this->sceneSettings.GetBackgroundColorSync();

    if (sync == ValueSourceSync::FROM_MASTER_TO_SLAVE)
        this->backgroundColor->SetColour(ApplicationAccessor::GetBackgroundColor());
    this->backgroundColor->Enable(sync != ValueSourceSync::FROM_MASTER_TO_SLAVE);
}

void SceneSettingsDialog_EnvironmentPage::UpdateFogModeControls(bool syncChanged)
{
    ValueSourceSync sync;
    if (syncChanged)
    {
        int fogSyncIndex = this->fogSyncChoice->GetSelection();
        sync = (ValueSourceSync)*(int*)this->fogSyncChoice->GetClientData(fogSyncIndex);
    }
    else
        sync = this->sceneSettings.GetFogSync();

    EnvironmentFar environmentFar(GetEnvironmentObject(), this->farDistanceSpinner->GetValue());

    wxString fogMode = (wxChar*)this->fogModeChoice->GetClientData(this->fogModeChoice->GetSelection());
    FinjinColor fogColor = this->fogColor->GetColour();
    float fogDensity = this->fogDensitySpinner->GetValue();
    float fogStart = this->fogStartSpinner->GetValue();
    float fogEnd = this->fogEndSpinner->GetValue();
    this->sceneSettings.SetFogValues(sync, fogMode, fogColor, fogDensity, fogStart, fogEnd, environmentFar.value, TimeAccessor::GetCurrentTime());

    bool applicationHasLinearFog;
    this->sceneSettings.GetFogValues(fogMode, fogColor, fogDensity, fogStart, fogEnd, environmentFar.value, TimeAccessor::GetCurrentTime(), &applicationHasLinearFog);

    fogMode.MakeLower();
    bool syncedFromApplicationLinearFog = sync == ValueSourceSync::FROM_MASTER_TO_SLAVE && applicationHasLinearFog;
    bool showLinear = fogMode.Find(wxT("linear")) >= 0;
    bool showDensity = fogMode.Find(wxT("exp")) >= 0;
    bool showColor = !fogMode.empty();

    this->fogStartLabel->Show(showLinear);
    this->fogStartSpinner->Show(showLinear);
    this->fogStartSpinner->Enable(!syncedFromApplicationLinearFog);

    this->fogEndLabel->Show(showLinear);
    this->fogEndSpinner->Show(showLinear);
    this->fogEndSpinner->Enable(!syncedFromApplicationLinearFog);

    this->fogDensityLabel->Show(showDensity);
    this->fogDensitySpinner->Show(showDensity);

    this->fogColorLabel->Show(showColor);
    this->fogColor->Show(showColor);
    this->fogColor->Enable(sync != ValueSourceSync::FROM_MASTER_TO_SLAVE);

    this->fogSyncChoice->Show(showColor);

    if (sync == ValueSourceSync::FROM_MASTER_TO_SLAVE)
    {
        //Fog range
        this->fogStartSpinner->SetValue(fogStart);
        this->fogEndSpinner->SetValue(fogEnd);

        //Fog color
        this->fogColor->SetColour(fogColor);
    }

    Layout();
}

ObjectAccessor SceneSettingsDialog_EnvironmentPage::GetEnvironmentObject()
{
    ObjectAccessor object;
    if (this->farBoxObjectButton->GetValue())
       object = this->environmentFarObject;
    return object;
}

void SceneSettingsDialog_EnvironmentPage::OnBackgroundColorChanged(wxColourPickerEvent& event)
{
}

void SceneSettingsDialog_EnvironmentPage::OnBackgroundColorSyncChoice(wxCommandEvent& event)
{
    UpdateBackgroundColorControls(true);
}

void SceneSettingsDialog_EnvironmentPage::OnAmbientLightColorChanged(wxColourPickerEvent& event)
{
}

void SceneSettingsDialog_EnvironmentPage::OnAmbientLightColorSyncChoice(wxCommandEvent& event)
{
    UpdateAmbientLightColorControls(true);
}

void SceneSettingsDialog_EnvironmentPage::OnNearDistanceSpinner(SpinnerControlEvent& event)
{
}

void SceneSettingsDialog_EnvironmentPage::OnFarDistanceRadioButton(wxCommandEvent& event)
{
    this->farBoxObjectButton->SetValue(false);
}

void SceneSettingsDialog_EnvironmentPage::OnFarBoxObjectRadioButton(wxCommandEvent& event)
{
    this->farDistanceButton->SetValue(false);
}

void SceneSettingsDialog_EnvironmentPage::OnFarDistanceSpinner(SpinnerControlEvent& event)
{
}

void SceneSettingsDialog_EnvironmentPage::OnFarBoxObjectButton(wxCommandEvent& event)
{
    ChooseObjectsPrompt choose(this, HowMany::CHOOSE_ONE, ObjectFilterType::MESH);
    if (choose.ShowPrompt())
    {
        this->environmentFarObject = choose.GetObject();
        this->farBoxObjectText->SetValue(this->environmentFarObject.GetFullName());
    }
}

void SceneSettingsDialog_EnvironmentPage::OnFogModeChoice(wxCommandEvent& event)
{
    UpdateFogModeControls();
}

void SceneSettingsDialog_EnvironmentPage::OnFogSyncChoice(wxCommandEvent& event)
{
    UpdateFogModeControls(true);
}

void SceneSettingsDialog_EnvironmentPage::OnFogStartSpinner(SpinnerControlEvent& event)
{
}

void SceneSettingsDialog_EnvironmentPage::OnFogEndSpinner(SpinnerControlEvent& event)
{
}

void SceneSettingsDialog_EnvironmentPage::OnFogDensitySpinner(SpinnerControlEvent& event)
{
}

void SceneSettingsDialog_EnvironmentPage::OnFogColorChanged(wxColourPickerEvent& event)
{
}

void SceneSettingsDialog_EnvironmentPage::OnShadowColorChanged(wxColourPickerEvent& event)
{
}

void SceneSettingsDialog_EnvironmentPage::OnShadowFarDistanceSpinner(SpinnerControlEvent& event)
{
}
