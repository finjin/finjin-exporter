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
#include "NodeAnimationSettingsDialog.hpp"
#include "WindowPlacementManager.hpp"
#include "StringUtilities.hpp"
#include "Dialogs.hpp"
#include "Strings.hpp"
#include "ApplicationControls.hpp"
#include "ApplicationAccessor.hpp"
#include "PropertyValues.hpp"

#if defined(__WXMAC__)
    #define DIALOG_WIDTH 386
    #define DIALOG_HEIGHT 337
#else
    #define DIALOG_WIDTH 336
    #define DIALOG_HEIGHT 317
#endif

using namespace Finjin::Exporter;


//Globals-----------------------------------------------------------------------
static const wxChar* TIME_SCALING_TEXT[] = {wxT("None"), wxT("Scale"), wxT("Length"), wxT("Normalize")};
static const wxChar* TIME_SCALING_VALUE[] = {wxT(""), wxT(""), wxT("length"), wxT("normalize")};
static const int TIME_SCALING_COUNT = FINJIN_COUNT_OF(TIME_SCALING_VALUE);
static const int TIME_SCALING_SCALE_INDEX = 1;

static const wxChar* SAMPLE_TYPES_TEXT[] = {wxT("Use Object Setting"), wxT("Seconds Between Samples"), wxT("Samples Per Second")};
static const SampleType SAMPLE_TYPES_VALUE[] = {SampleType::USE_PARENT, SampleType::INTERVAL, SampleType::RATE};
static const int SAMPLE_TYPE_COUNT = FINJIN_COUNT_OF(SAMPLE_TYPES_VALUE);

static const wxChar* TRANSLATION_INTERPOLATION_TYPES_TEXT[] = {wxT("Linear"), wxT("Spline")};
static const wxChar* TRANSLATION_INTERPOLATION_TYPES_VALUE[] = {wxT(""), wxT("spline")};
static const int TRANSLATION_INTERPOLATION_TYPE_COUNT = FINJIN_COUNT_OF(TRANSLATION_INTERPOLATION_TYPES_VALUE);

static const wxChar* ROTATION_INTERPOLATION_TYPES_TEXT[] = {wxT("Linear"), wxT("Spherical")};
static const wxChar* ROTATION_INTERPOLATION_TYPES_VALUE[] = {wxT(""), wxT("spherical")};
static const int ROTATION_INTERPOLATION_TYPE_COUNT = FINJIN_COUNT_OF(ROTATION_INTERPOLATION_TYPES_VALUE);


//Local functions---------------------------------------------------------------
static void SetSampleValue(double& sampleInterval, SampleType& sampleType, double value, SampleType inputSampleType)
{
    sampleType = SampleType::INTERVAL;
    sampleInterval = 0;

    if (inputSampleType == SampleType::INTERVAL)
        sampleInterval = value;
    else if (inputSampleType == SampleType::RATE)
    {
        if (value != 0)
        {
            sampleType = SampleType::RATE;
            sampleInterval = 1 / value;
        }
    }
}

static void SetTimeScale(double& scale, wxString& scaleType, double value, const wxString& inputScaleType)
{
    scale = 0;
    scaleType = wxEmptyString;

    if (inputScaleType == TIME_SCALING_VALUE[1] && value > 0)
        scale = value;
    else if (inputScaleType == TIME_SCALING_VALUE[2])
    {
        scaleType = TIME_SCALING_VALUE[2];
        scale = value;
    }
    else if (inputScaleType == TIME_SCALING_VALUE[3])
        scaleType = TIME_SCALING_VALUE[3];
}


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(NodeAnimationSettingsDialog, FinjinDialog)
    EVT_BUTTON(wxID_OK, NodeAnimationSettingsDialog::OnOK)
    EVT_CHOICE(NodeAnimationSettingsDialog::TIME_SCALE_CHOICE, NodeAnimationSettingsDialog::OnTimeScaleChoice)
    EVT_CHOICE(NodeAnimationSettingsDialog::SAMPLING_TYPE_CHOICE, NodeAnimationSettingsDialog::OnSamplingTypeChoice)
    EVT_CHECKBOX(NodeAnimationSettingsDialog::EMBED_ANIMATION_CHECKBOX, NodeAnimationSettingsDialog::OnEmbedCheckbox)
END_EVENT_TABLE()

NodeAnimationSettingsDialog::Settings::Settings()
{
    this->interval = ApplicationAccessor::GetAnimationRange();
    this->scale = 0;
    this->copyFirstKeyToLast = false;
    this->useAnimationStartTime = false;
    this->embedAnimation = true;
    this->linkToObject = true;
    this->enabled = false;
    this->looped = false;
    this->sampleInterval = 0;
    this->sampleType = SampleType::INTERVAL;
}

void NodeAnimationSettingsDialog::Settings::GetSettings(FinjinNodeAnimationSettingsAccessor& anim)
{
    anim.SetAnimationName(this->animationName);
    anim.SetTimeScaleType(this->scaleType);
    anim.SetTimeScale(this->scale);
    anim.SetCopyFirstKeyToLast(this->copyFirstKeyToLast);
    anim.SetUseAnimationStartTime(this->useAnimationStartTime);
    anim.SetLinkToMainObject(this->linkToObject);
    anim.SetEmbedAnimation(this->embedAnimation);
    anim.SetEnabled(this->enabled);
    anim.SetLooped(this->looped);
    anim.SetSampleInterval(this->sampleInterval);
    anim.SetSampleType(this->sampleType);
    anim.SetTimeInterval(this->interval);
    anim.SetTranslationInterpolationType(this->translationInterpolationType);
    anim.SetRotationInterpolationType(this->rotationInterpolationType);
}

void NodeAnimationSettingsDialog::Settings::SetSettings(FinjinNodeAnimationSettingsAccessor& anim)
{
    this->animationName = anim.GetAnimationName();
    this->scaleType = anim.GetTimeScaleType();
    this->scale = anim.GetTimeScale();
    this->copyFirstKeyToLast = anim.GetCopyFirstKeyToLast();
    this->useAnimationStartTime = anim.GetUseAnimationStartTime();
    this->linkToObject = anim.GetLinkToMainObject();
    this->embedAnimation = anim.GetEmbedAnimation();
    this->enabled = anim.GetEnabled();
    this->looped = anim.GetLooped();
    this->sampleInterval = anim.GetSampleInterval();
    this->sampleType = anim.GetSampleType();
    this->interval = anim.GetTimeInterval();
    this->translationInterpolationType = anim.GetTranslationInterpolationType();
    this->rotationInterpolationType = anim.GetRotationInterpolationType();
}

NodeAnimationSettingsDialog::NodeAnimationSettingsDialog
    (
    wxWindow* parent,
    const wxString& title,
    const std::vector<wxString>& allAnimationNames,
    Settings settings
    ) : FinjinDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize( DIALOG_WIDTH,DIALOG_HEIGHT ), wxDEFAULT_DIALOG_STYLE, wxT("NodeAnimationSettingsDialog"))
{
    FinjinTimeInterval animationRange = ApplicationAccessor::GetAnimationRange();

    this->allAnimationNames = allAnimationNames;
    this->settings = settings;

    SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);

    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 4, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    nameLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    nameLabel->Wrap( -1 );
    gridSizer->Add( nameLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer* nameSizer;
    nameSizer = new wxBoxSizer( wxHORIZONTAL );

    nameText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
    nameSizer->Add( nameText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    gridSizer->Add( nameSizer, 1, wxEXPAND, 5 );

    startEndFramesLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Start/End ") + TimeAccessor::GetUIUnit(), wxDefaultPosition, wxDefaultSize, 0 );
    startEndFramesLabel->Wrap( -1 );
    gridSizer->Add( startEndFramesLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer* framesSizer;
    framesSizer = new wxBoxSizer( wxHORIZONTAL );

    startFrameSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(70, -1), 0, animationRange.start.GetValue(), animationRange.end.GetValue(), SpinnerControl::UI_TIME_UNIT);
    framesSizer->Add( startFrameSpinner, 0, wxTOP|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

    inBetweenFrameLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
    inBetweenFrameLabel->Wrap( -1 );
    framesSizer->Add( inBetweenFrameLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    endFrameSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(70, -1), 0, animationRange.start.GetValue(), animationRange.end.GetValue(), SpinnerControl::UI_TIME_UNIT);
    framesSizer->Add( endFrameSpinner, 0, wxTOP|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

    gridSizer->Add( framesSizer, 1, wxEXPAND, 5 );

    timeScalingLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Time Scaling"), wxDefaultPosition, wxDefaultSize, 0 );
    timeScalingLabel->Wrap( -1 );
    gridSizer->Add( timeScalingLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer* timeScalingSizer;
    timeScalingSizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString timeScalingChoiceChoices;
    timeScalingChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, timeScalingChoiceChoices, 0 );
    timeScalingChoice->SetSelection( 0 );
    timeScalingSizer->Add( timeScalingChoice, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    timeScalingSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(60, -1), 0, 0, 100, SpinnerControl::FLOAT_UNIT, 2);
    timeScalingSizer->Add( timeScalingSpinner, 0, wxTOP|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );

    gridSizer->Add( timeScalingSizer, 1, wxEXPAND, 5 );

    samplingLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    samplingLabel->Wrap( -1 );
    gridSizer->Add( samplingLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer* samplingSizer;
    samplingSizer = new wxBoxSizer( wxHORIZONTAL );

    samplingText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    samplingSizer->Add( samplingText, 0, wxTOP|wxLEFT, 5 );

    wxArrayString samplingComboChoices;
    samplingChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, samplingComboChoices, 0 );
    samplingChoice->SetSelection( 0 );
    samplingSizer->Add( samplingChoice, 0, wxTOP|wxLEFT, 5 );

    gridSizer->Add( samplingSizer, 1, wxEXPAND, 5 );

    topSizer->Add( gridSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxFlexGridSizer* interpolationSizer;
    interpolationSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
    interpolationSizer->SetFlexibleDirection( wxBOTH );
    interpolationSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    translationInterpolationLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Interpolation"), wxDefaultPosition, wxDefaultSize, 0 );
    translationInterpolationLabel->Wrap( -1 );
    interpolationSizer->Add( translationInterpolationLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    rotationInterpolationLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Rotation Interpolation"), wxDefaultPosition, wxDefaultSize, 0 );
    rotationInterpolationLabel->Wrap( -1 );
    interpolationSizer->Add( rotationInterpolationLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxArrayString translationInterpolationChoiceChoices;
    translationInterpolationChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, translationInterpolationChoiceChoices, 0 );
    translationInterpolationChoice->SetSelection( 0 );
    interpolationSizer->Add( translationInterpolationChoice, 1, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxArrayString rotationInterpolationChoiceChoices;
    rotationInterpolationChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, rotationInterpolationChoiceChoices, 0 );
    rotationInterpolationChoice->SetSelection( 0 );
    interpolationSizer->Add( rotationInterpolationChoice, 1, wxTOP|wxRIGHT|wxLEFT, 5 );

    topSizer->Add( interpolationSizer, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* checkBoxesSizer;
    checkBoxesSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* embedSizer;
    embedSizer = new wxBoxSizer( wxHORIZONTAL );

    embedAnimationCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Embed Animation"), wxDefaultPosition, wxDefaultSize, 0 );
    embedSizer->Add( embedAnimationCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    linkToObjectCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Link to Node"), wxDefaultPosition, wxDefaultSize, 0 );
    embedSizer->Add( linkToObjectCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );


    checkBoxesSizer->Add( embedSizer, 0, wxEXPAND, 5 );

    enabledCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Enabled"), wxDefaultPosition, wxDefaultSize, 0 );

    checkBoxesSizer->Add( enabledCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    loopedCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Looped"), wxDefaultPosition, wxDefaultSize, 0 );

    checkBoxesSizer->Add( loopedCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    useDefinedAnimationStartTimeCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Use Defined Animation Start Time"), wxDefaultPosition, wxDefaultSize, 0 );

    checkBoxesSizer->Add( useDefinedAnimationStartTimeCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    copyFirstAnimationKeyToLastCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Copy First Animation Key to Last"), wxDefaultPosition, wxDefaultSize, 0 );

    checkBoxesSizer->Add( copyFirstAnimationKeyToLastCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    topSizer->Add( checkBoxesSizer, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );

    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT|wxTOP, 5 );

    this->timeScalingChoice->SetId(TIME_SCALE_CHOICE);
    this->samplingChoice->SetId(SAMPLING_TYPE_CHOICE);
    this->embedAnimationCheckbox->SetId(EMBED_ANIMATION_CHECKBOX);

    SetGUIData();

    SetSizer(topSizer);
    SetMinSize(wxSize(DIALOG_WIDTH, DIALOG_HEIGHT));

    if (!WindowPlacementManager::RestorePlacement(this))
    {
        Layout();
        Centre(wxBOTH);
    }
}

NodeAnimationSettingsDialog::~NodeAnimationSettingsDialog()
{
}

void NodeAnimationSettingsDialog::GetGUIData()
{
    //Animation name edit
    this->settings.animationName = this->nameText->GetValue();

    //Start/end frame
    this->settings.interval.start.SetValue(this->startFrameSpinner->GetValue());
    this->settings.interval.end.SetValue(this->endFrameSpinner->GetValue());

    //Sample type combo and edit
    auto sampling = StringUtilities::ToDouble(this->samplingText->GetValue());
    int selectedSamplingTypeIndex = this->samplingChoice->GetSelection();
    SampleType sampleType = (SampleType)*(int*)this->samplingChoice->GetClientData(selectedSamplingTypeIndex);
    SetSampleValue(this->settings.sampleInterval, this->settings.sampleType, sampling, sampleType);

    //Time scaling type combo and edit
    float scale = this->timeScalingSpinner->GetValue();
    int selectedTimeScaleIndex = this->timeScalingChoice->GetSelection();
    wxString scaleType = (wxChar*)this->timeScalingChoice->GetClientData(selectedTimeScaleIndex);
    SetTimeScale(this->settings.scale, this->settings.scaleType, scale, scaleType);

    //Translation interpolation type combo
    int selectedTranslationInterpolationTypeIndex = this->translationInterpolationChoice->GetSelection();
    this->settings.translationInterpolationType = (wxChar*)this->translationInterpolationChoice->GetClientData(selectedTranslationInterpolationTypeIndex);

    //Rotation interpolation type combo
    int selectedRotationInterpolationTypeIndex = this->rotationInterpolationChoice->GetSelection();
    this->settings.rotationInterpolationType = (wxChar*)this->rotationInterpolationChoice->GetClientData(selectedRotationInterpolationTypeIndex);

    //Embed animation
    this->settings.embedAnimation = this->embedAnimationCheckbox->GetValue();

    //Link to object
    this->settings.linkToObject = this->linkToObjectCheckbox->GetValue();

    //Enabled check
    this->settings.enabled = this->enabledCheckbox->GetValue();

    //Looped check
    this->settings.looped = this->loopedCheckbox->GetValue();

    //Use defined animation start time check
    this->settings.useAnimationStartTime = this->useDefinedAnimationStartTimeCheckbox->GetValue();

    //Copy-first-key-to-last check
    this->settings.copyFirstKeyToLast = this->copyFirstAnimationKeyToLastCheckbox->GetValue();
}

void NodeAnimationSettingsDialog::SetGUIData()
{
    //Animation name edit
    this->nameText->SetValue(this->settings.animationName);

    //Start/end frame
    this->startFrameSpinner->SetValue(this->settings.interval.start.GetValue());
    this->endFrameSpinner->SetValue(this->settings.interval.end.GetValue());

    //Sample value
    this->samplingText->SetValue(PropertyValues::ChooseAnimationSampleValue(this->settings.sampleType, this->settings.sampleInterval));

    //Sample type combo
    this->samplingChoice->Freeze();
    for (int i = 0; i < SAMPLE_TYPE_COUNT; i++)
    {
        this->samplingChoice->Append(SAMPLE_TYPES_TEXT[i]);
        this->samplingChoice->SetClientData(i, (void*)&SAMPLE_TYPES_VALUE[i]);
        if (this->settings.sampleType == SAMPLE_TYPES_VALUE[i])
            this->samplingChoice->SetSelection(i);
    }
    this->samplingChoice->Thaw();
    if (this->settings.sampleInterval == 0 || this->samplingChoice->GetSelection() < 0)
        this->samplingChoice->SetSelection(0);

    //Time scaling type combo
    this->timeScalingChoice->Freeze();
    for (int i = 0; i < TIME_SCALING_COUNT; i++)
    {
        this->timeScalingChoice->Append(TIME_SCALING_TEXT[i]);
        this->timeScalingChoice->SetClientData(i, (void*)TIME_SCALING_VALUE[i]);
        if (this->timeScalingChoice->GetSelection() < 0 && this->settings.scaleType == TIME_SCALING_VALUE[i])
            this->timeScalingChoice->SetSelection(i);
    }
    this->timeScalingChoice->Thaw();
    if (this->timeScalingChoice->GetSelection() < 0)
        this->timeScalingChoice->SetSelection(0);
    if (this->settings.scale > 0 && this->timeScalingChoice->GetSelection() == 0)
        this->timeScalingChoice->SetSelection(TIME_SCALING_SCALE_INDEX);

    //Time scaling edit
    this->timeScalingSpinner->SetValue(this->settings.scale);

    //Translation interpolation type combo
    this->translationInterpolationChoice->Freeze();
    for (int i = 0; i < TRANSLATION_INTERPOLATION_TYPE_COUNT; i++)
    {
        this->translationInterpolationChoice->Append(TRANSLATION_INTERPOLATION_TYPES_TEXT[i]);
        this->translationInterpolationChoice->SetClientData(i, (wxChar*)TRANSLATION_INTERPOLATION_TYPES_VALUE[i]);
        if (this->settings.translationInterpolationType == TRANSLATION_INTERPOLATION_TYPES_VALUE[i])
            this->translationInterpolationChoice->SetSelection(i);
    }
    this->translationInterpolationChoice->Thaw();
    if (this->translationInterpolationChoice->GetSelection() < 0)
        this->translationInterpolationChoice->SetSelection(0);

    //Rotation interpolation type combo
    this->rotationInterpolationChoice->Freeze();
    for (int i = 0; i < ROTATION_INTERPOLATION_TYPE_COUNT; i++)
    {
        this->rotationInterpolationChoice->Append(ROTATION_INTERPOLATION_TYPES_TEXT[i]);
        this->rotationInterpolationChoice->SetClientData(i, (wxChar*)ROTATION_INTERPOLATION_TYPES_VALUE[i]);
        if (this->settings.rotationInterpolationType == ROTATION_INTERPOLATION_TYPES_VALUE[i])
            this->rotationInterpolationChoice->SetSelection(i);
    }
    this->rotationInterpolationChoice->Thaw();
    if (this->rotationInterpolationChoice->GetSelection() < 0)
        this->rotationInterpolationChoice->SetSelection(0);

    //Embed animation
    this->embedAnimationCheckbox->SetValue(this->settings.embedAnimation);

    //Link to object
    this->linkToObjectCheckbox->SetValue(this->settings.linkToObject);

    //Enabled check
    this->enabledCheckbox->SetValue(this->settings.enabled);

    //Looped check
    this->loopedCheckbox->SetValue(this->settings.looped);

    //Use defined animation start time check
    this->useDefinedAnimationStartTimeCheckbox->SetValue(this->settings.useAnimationStartTime);

    //Copy-first-key-to-last check
    this->copyFirstAnimationKeyToLastCheckbox->SetValue(this->settings.copyFirstKeyToLast);

    //Final updates
    this->nameText->SetFocus();
    UpdateTimeScaleEdit(false);
    UpdateSamplingTypeEdit(false);
    UpdateEmbedControls(false);
    Layout();
}

void NodeAnimationSettingsDialog::UpdateTimeScaleEdit(bool layout)
{
    int selectedTimeScaleIndex = this->timeScalingChoice->GetSelection();
    bool show = selectedTimeScaleIndex == 1 || selectedTimeScaleIndex == 2;
    this->timeScalingSpinner->Show(show);
    if (layout)
        Layout();
}

void NodeAnimationSettingsDialog::UpdateSamplingTypeEdit(bool layout)
{
    int selectedSamplingTypeIndex = this->samplingChoice->GetSelection();
    bool show = selectedSamplingTypeIndex > 0;
    this->samplingText->Show(show);
    if (layout)
        Layout();
}

void NodeAnimationSettingsDialog::UpdateEmbedControls(bool layout)
{
    auto embed = this->embedAnimationCheckbox->GetValue();
    this->linkToObjectCheckbox->Enable(!embed);
    if (layout)
        Layout();
}

void NodeAnimationSettingsDialog::OnOK(wxCommandEvent& event)
{
    GetGUIData();

    auto success = true;

    if (this->settings.animationName.empty())
    {
        Dialogs::ShowMessage(this, Strings::ANIMATION_NAME_REQUIRED, Strings::DATA_ENTRY_ERROR, wxINFORMATION_DIALOG_FLAGS);
        this->nameText->SetFocus();
        success = false;
    }
    else if (StringUtilities::ContainsName(this->allAnimationNames, this->settings.animationName))
    {
        Dialogs::ShowMessage(this, Strings::ANIMATION_NAME_ALREADY_DEFINED, Strings::DATA_ENTRY_ERROR, wxINFORMATION_DIALOG_FLAGS);
        this->nameText->SetFocus();
        success = false;
    }
    else if (this->settings.interval.start >= this->settings.interval.end)
    {
        Dialogs::ShowMessage(this, Strings::START_FRAME_MUST_BE_LESS_THAN_END_FRAME, Strings::DATA_ENTRY_ERROR, wxINFORMATION_DIALOG_FLAGS);
        this->startFrameSpinner->SetFocus();
        success = false;
    }

    if (success)
        MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
}

void NodeAnimationSettingsDialog::OnTimeScaleChoice(wxCommandEvent& event)
{
    UpdateTimeScaleEdit();
}

void NodeAnimationSettingsDialog::OnSamplingTypeChoice(wxCommandEvent& event)
{
    UpdateSamplingTypeEdit();
}

void NodeAnimationSettingsDialog::OnEmbedCheckbox(wxCommandEvent& event)
{
    UpdateEmbedControls();
}
