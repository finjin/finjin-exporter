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
#include "MeshAnimationSettingsDialog.hpp"
#include "WindowPlacementManager.hpp"
#include "StringUtilities.hpp"
#include "Dialogs.hpp"
#include "Strings.hpp"
#include "RemoveBoneTranslationsDialog.hpp"
#include "IncludeExcludeBoneDialog.hpp"
#include "StringSplitter.hpp"
#include "ApplicationAccessor.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "ApplicationControls.hpp"
#include "MeshAnimationTracksDetector.hpp"
#include "PropertyValues.hpp"
#include "ModalDialogFix.hpp"

using namespace Finjin::Exporter;


//Globals----------------------------------------------------------------------
static const wxChar* TIME_SCALING_TEXT[] = {wxT("None"), wxT("Scale"), wxT("Length"), wxT("Normalize")};
static const wxChar* TIME_SCALING_VALUE[] = {wxT(""), wxT(""), wxT("length"), wxT("normalize")};
static const int TIME_SCALING_COUNT = FINJIN_COUNT_OF(TIME_SCALING_VALUE);
static const int TIME_SCALING_SCALE_INDEX = 1;

static const wxChar* SAMPLE_TYPES_TEXT[] = {wxT("Use Object Setting"), wxT("Seconds Between Samples"), wxT("Samples Per Second")};
static const SampleType SAMPLE_TYPES_VALUE[] = {SampleType::USE_PARENT, SampleType::INTERVAL, SampleType::RATE};
static const int SAMPLE_TYPE_COUNT = FINJIN_COUNT_OF(SAMPLE_TYPES_VALUE);

static const wxString BONE_KEY(wxT("bone"));
static const wxString BONE_TREE_KEY(wxT("boneTree"));
static const wxString INCLUDE_TEXT(wxT("include"));
static const wxString EXCLUDE_TEXT(wxT("exclude"));

static const wxChar* INHERITED_BOOLS_TEXT[] = {wxT("Use Object Setting"), wxT("Yes"), wxT("No")};
static const int INHERITED_BOOLS_VALUE[] = {INHERITED_USE_PARENT_BOOL, INHERITED_TRUE, INHERITED_FALSE};
static const int INHERITED_BOOL_COUNT = FINJIN_COUNT_OF(INHERITED_BOOLS_VALUE);


//Local functions--------------------------------------------------------------
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

    if (inputScaleType == TIME_SCALING_VALUE[1] && value >= 0)
        scale = value;
    else if (inputScaleType == TIME_SCALING_VALUE[2])
    {
        scaleType = TIME_SCALING_VALUE[2];
        scale = value;
    }
    else if (inputScaleType == TIME_SCALING_VALUE[3])
        scaleType = TIME_SCALING_VALUE[3];
}


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(MeshAnimationSettingsDialog, FinjinDialog)
    EVT_BUTTON(wxID_OK, MeshAnimationSettingsDialog::OnOK)        
    EVT_CHOICE(MeshAnimationSettingsDialog::TRACK_CHOICE, MeshAnimationSettingsDialog::OnTrackChoice)
    EVT_CHOICE(MeshAnimationSettingsDialog::TIME_SCALE_CHOICE, MeshAnimationSettingsDialog::OnTimeScaleChoice)
    EVT_CHOICE(MeshAnimationSettingsDialog::SAMPLING_TYPE_CHOICE, MeshAnimationSettingsDialog::OnSamplingTypeChoice)
    EVT_CHECKBOX(MeshAnimationSettingsDialog::OVERRIDE_ANIMATED_ROOT_START_TIME_CHECKBOX, MeshAnimationSettingsDialog::OnOverrideAnimatedRootStartTimeCheckbox)
    EVT_BUTTON(MeshAnimationSettingsDialog::ADD_REMOVE_BONE_TRANSLATION_BUTTON, MeshAnimationSettingsDialog::OnRemoveBoneTranslationButton)
    EVT_BUTTON(MeshAnimationSettingsDialog::ADD_INCLUDE_EXCLUDE_BONE_BUTTON, MeshAnimationSettingsDialog::OnAddIncludeExcludeBoneButton)
    EVT_CHECKBOX(MeshAnimationSettingsDialog::EMBED_ANIMATION_CHECKBOX, MeshAnimationSettingsDialog::OnEmbedCheckbox)
END_EVENT_TABLE()

MeshAnimationSettingsDialog::Settings::Settings()
{
    this->interval = ApplicationAccessor::GetAnimationRange();
    this->scale = 0;
    this->copyFirstKeyToLast = false;
    this->exportAsMorph = false;
    this->overrideAnimatedRootStartTime = false;
    this->animatedRootStartTime = this->interval.start;
    this->morphWholeObject = INHERITED_USE_PARENT_BOOL;
    this->useAnimationStartTime = false;
    this->embedAnimation = true;
    this->linkToObject = true;
    this->looped = false;
    this->sampleInterval = 0;
    this->sampleType = SampleType::INTERVAL;    
}

void MeshAnimationSettingsDialog::Settings::GetSettings(FinjinMeshAnimationSettingsAccessor& anim)
{
    anim.SetTrack(this->trackName);
    anim.SetAnimationName(this->animationName);
    anim.SetTimeScaleType(this->scaleType);
    anim.SetTimeScale(this->scale);
    anim.SetCopyFirstKeyToLast(this->copyFirstKeyToLast);
    anim.SetExportAsMorph(this->exportAsMorph);
    anim.SetOverrideAnimatedRootStartTime(this->overrideAnimatedRootStartTime);
    anim.SetAnimatedRootStartTime(this->animatedRootStartTime);
    anim.SetMorphWholeObject(this->morphWholeObject);
    anim.SetUseAnimationStartTime(this->useAnimationStartTime);
    anim.SetLinkToMainObject(this->linkToObject);
    anim.SetEmbedAnimation(this->embedAnimation);
    anim.SetLooped(this->looped);
    anim.SetSampleInterval(this->sampleInterval);
    anim.SetSampleType(this->sampleType);
    anim.SetTimeInterval(this->interval);    
    
    anim.ClearBoneTranslationMasks();
    for (size_t i = 0; i < this->boneTranslationMasks.size(); i++)
        anim.AddBoneTranslationMask(this->boneTranslationMasks[i].object, this->boneTranslationMasks[i].mask);

    anim.ClearAllowExportBones();
    for (size_t i = 0; i < this->allowExportBones.size(); i++)
        anim.AddAllowExportBone(this->allowExportBones[i].object, this->allowExportBones[i].allow, this->allowExportBones[i].recursive);
}

void MeshAnimationSettingsDialog::Settings::SetSettings(FinjinMeshAnimationSettingsAccessor& anim)
{
    this->trackName = anim.GetTrack();
    this->animationName = anim.GetAnimationName();
    this->scaleType = anim.GetTimeScaleType();
    this->scale = anim.GetTimeScale();
    this->copyFirstKeyToLast = anim.GetCopyFirstKeyToLast();
    this->exportAsMorph = anim.GetExportAsMorph();
    this->overrideAnimatedRootStartTime = anim.GetOverrideAnimatedRootStartTime();
    this->animatedRootStartTime = anim.GetAnimatedRootStartTime();
    this->morphWholeObject = anim.GetMorphWholeObject();
    this->useAnimationStartTime = anim.GetUseAnimationStartTime();
    this->linkToObject = anim.GetLinkToMainObject();
    this->embedAnimation = anim.GetEmbedAnimation();
    this->looped = anim.GetLooped();
    this->sampleInterval = anim.GetSampleInterval();
    this->sampleType = anim.GetSampleType();
    this->interval = anim.GetTimeInterval();
    
    int boneTranslationMaskCount = anim.GetBoneTranslationMaskCount();
    this->boneTranslationMasks.resize(boneTranslationMaskCount);
    for (int i = 0; i < boneTranslationMaskCount; i++)
        anim.GetBoneTranslationMask(i, this->boneTranslationMasks[i].object, this->boneTranslationMasks[i].mask);        

    int allowExportBonesCount = anim.GetAllowExportBoneCount();
    this->allowExportBones.resize(allowExportBonesCount);
    for (int i = 0; i < allowExportBonesCount; i++)
        anim.GetAllowExportBone(i, this->allowExportBones[i].object, this->allowExportBones[i].allow, this->allowExportBones[i].recursive);
}

MeshAnimationSettingsDialog::MeshAnimationSettingsDialog
    (
    wxWindow* parent, 
    const wxString& title, 
    const ObjectAccessorSet& meshBones,
    const std::vector<wxString>& allAnimationNames,
    const std::vector<MeshAnimationTrack>& tracks,
    int selectedTrack,
    Settings settings
    ) : FinjinDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize( 342,626 ), wxDEFAULT_DIALOG_STYLE, wxT("MeshAnimationSettingsDialog"))
{
    FinjinTimeInterval animationRange = ApplicationAccessor::GetAnimationRange();

    this->meshBones = meshBones;
    this->allAnimationNames = allAnimationNames;
    this->tracks = tracks;
    this->selectedTrack = selectedTrack;
    this->settings = settings;

    SetSizeHints(wxDefaultSize, wxDefaultSize);
    
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 8, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    nameLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    nameLabel->Wrap( -1 );
    gridSizer->Add( nameLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );
    
    wxBoxSizer* nameSizer;
    nameSizer = new wxBoxSizer( wxHORIZONTAL );
    
    nameText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
    nameSizer->Add( nameText, 0, wxRIGHT|wxLEFT, 5 );
    
    gridSizer->Add( nameSizer, 1, wxEXPAND, 5 );
    
    trackLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Track"), wxDefaultPosition, wxDefaultSize, 0 );
    trackLabel->Wrap( -1 );
    gridSizer->Add( trackLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );
    
    wxBoxSizer* trackSizer;
    trackSizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString trackChoiceChoices;
    trackChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxSize(120, -1), trackChoiceChoices, 0 );
    trackChoice->SetSelection( 0 );
    trackSizer->Add( trackChoice, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

    gridSizer->Add( trackSizer, 1, wxEXPAND, 5 );
    
    startEndFramesLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Start/End ") + TimeAccessor::GetUIUnit(), wxDefaultPosition, wxDefaultSize, 0 );
    startEndFramesLabel->Wrap( -1 );
    gridSizer->Add( startEndFramesLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );
    
    wxBoxSizer* framesSizer;
    framesSizer = new wxBoxSizer( wxHORIZONTAL );
    
    startFrameSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(70, -1), 0, animationRange.start.GetValue(), animationRange.end.GetValue(), SpinnerControl::UI_TIME_UNIT);
    framesSizer->Add( startFrameSpinner, 0, wxTOP|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
    
    inBetweenFrameLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
    inBetweenFrameLabel->Wrap( -1 );
    framesSizer->Add( inBetweenFrameLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );
    
    endFrameSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(70, -1), 0, animationRange.start.GetValue(), animationRange.end.GetValue(), SpinnerControl::UI_TIME_UNIT);
    framesSizer->Add( endFrameSpinner, 0, wxTOP|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
    
    gridSizer->Add( framesSizer, 1, wxEXPAND, 5 );
    
    timeScalingLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Time Scaling"), wxDefaultPosition, wxDefaultSize, 0 );
    timeScalingLabel->Wrap( -1 );
    gridSizer->Add( timeScalingLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );
    
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
    gridSizer->Add( samplingLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );
    
    wxBoxSizer* samplingSizer;
    samplingSizer = new wxBoxSizer( wxHORIZONTAL );
    
    samplingText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    samplingSizer->Add( samplingText, 0, wxTOP|wxLEFT, 5 );
    
    wxArrayString samplingChoiceChoices;
    samplingChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, samplingChoiceChoices, 0 );
    samplingChoice->SetSelection( 0 );
    samplingSizer->Add( samplingChoice, 0, wxTOP|wxLEFT|wxRIGHT, 5 );
    
    gridSizer->Add( samplingSizer, 1, wxEXPAND, 5 );

    morphWholeObjectLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Morph Whole Object"), wxDefaultPosition, wxDefaultSize, 0 );
    morphWholeObjectLabel->Wrap( -1 );
    gridSizer->Add( morphWholeObjectLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5 );

    wxArrayString generateCombinedMorphsChoiceChoices;
    morphWholeObjectChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, generateCombinedMorphsChoiceChoices, 0 );
    morphWholeObjectChoice->SetSelection( 0 );
    gridSizer->Add( morphWholeObjectChoice, 0, wxLEFT|wxTOP|wxBOTTOM, 5 );
    
    topSizer->Add( gridSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
    
    wxBoxSizer* checkBoxesSizer;
    checkBoxesSizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* embedSizer;
    embedSizer = new wxBoxSizer( wxHORIZONTAL );
    
    embedAnimationCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Embed Animation"), wxDefaultPosition, wxDefaultSize, 0 );
    embedSizer->Add( embedAnimationCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
    
    linkToObjectCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Link to Object"), wxDefaultPosition, wxDefaultSize, 0 );
    embedSizer->Add( linkToObjectCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
    
    
    checkBoxesSizer->Add( embedSizer, 0, wxEXPAND, 5 );

    copyFirstAnimationKeyToLastCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Copy First Animation Key to Last"), wxDefaultPosition, wxDefaultSize, 0 );    
    checkBoxesSizer->Add( copyFirstAnimationKeyToLastCheckbox, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
    
    exportAsMorphCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export as Morph"), wxDefaultPosition, wxDefaultSize, 0 );
    
    checkBoxesSizer->Add( exportAsMorphCheckbox, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

    wxBoxSizer* animatedRootStartFrameSizer;
    animatedRootStartFrameSizer = new wxBoxSizer( wxHORIZONTAL );
    
    overrideAnimatedRootStartTimeCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Override Animated Root Start ") + TimeAccessor::GetUIUnit(false), wxDefaultPosition, wxDefaultSize, 0 );
    
    animatedRootStartFrameSizer->Add( overrideAnimatedRootStartTimeCheckbox, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
    
    animatedRootStartTimeSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(70, -1), 0, animationRange.start.GetValue(), animationRange.end.GetValue(), SpinnerControl::UI_TIME_UNIT);
    animatedRootStartFrameSizer->Add( animatedRootStartTimeSpinner, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxTOP, 5 );
    
    checkBoxesSizer->Add( animatedRootStartFrameSizer, 1, wxEXPAND, 5 );

    topSizer->Add(checkBoxesSizer, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
    
    skeletonAnimationSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Skeleton Animation") ), wxVERTICAL );
    
    wxGridSizer* removeBoneTranslationsSizer;
    removeBoneTranslationsSizer = new wxGridSizer( 1, 2, 0, 0 );
    
    removeBoneTranslationsLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Remove Bone Translations"), wxDefaultPosition, wxDefaultSize, 0 );
    removeBoneTranslationsLabel->Wrap( -1 );
    removeBoneTranslationsSizer->Add( removeBoneTranslationsLabel, 0, wxALIGN_BOTTOM|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    addRemoveTranslationsBoneButton = new wxButton( this, wxID_ANY, wxT("Add Bone..."), wxDefaultPosition, wxDefaultSize, 0 );
    removeBoneTranslationsSizer->Add( addRemoveTranslationsBoneButton, 0, wxALIGN_RIGHT|wxRIGHT|wxLEFT, 5 );
    
    skeletonAnimationSizer->Add( removeBoneTranslationsSizer, 0, wxEXPAND, 5 );
    
    removeBoneTranslationsText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,100 ), wxTE_DONTWRAP|wxTE_MULTILINE );
    skeletonAnimationSizer->Add( removeBoneTranslationsText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    wxGridSizer* includedExcludeBonesSizer;
    includedExcludeBonesSizer = new wxGridSizer( 1, 2, 0, 0 );
    
    includeExcludeBonesLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Include/Exclude Bones"), wxDefaultPosition, wxDefaultSize, 0 );
    includeExcludeBonesLabel->Wrap( -1 );
    includedExcludeBonesSizer->Add( includeExcludeBonesLabel, 0, wxALIGN_BOTTOM|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    includeExcludeBonesButton = new wxButton( this, wxID_ANY, wxT("Add Bone..."), wxDefaultPosition, wxDefaultSize, 0 );
    includedExcludeBonesSizer->Add( includeExcludeBonesButton, 0, wxALIGN_RIGHT|wxRIGHT|wxLEFT, 5 );
    
    skeletonAnimationSizer->Add( includedExcludeBonesSizer, 0, wxEXPAND, 5 );
    
    includeExcludeBonesText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,200 ), wxTE_DONTWRAP|wxTE_MULTILINE );
    skeletonAnimationSizer->Add( includeExcludeBonesText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    topSizer->Add( skeletonAnimationSizer, 1, wxEXPAND|wxRIGHT|wxLEFT|wxTOP, 5 );
    
    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );
    
    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );
    
    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );
    
    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT, 5 );
    
    SetGUIData();

    this->trackChoice->SetId(TRACK_CHOICE);
    this->timeScalingChoice->SetId(TIME_SCALE_CHOICE);
    this->samplingChoice->SetId(SAMPLING_TYPE_CHOICE);
    this->overrideAnimatedRootStartTimeCheckbox->SetId(OVERRIDE_ANIMATED_ROOT_START_TIME_CHECKBOX);
    this->addRemoveTranslationsBoneButton->SetId(ADD_REMOVE_BONE_TRANSLATION_BUTTON);
    this->includeExcludeBonesButton->SetId(ADD_INCLUDE_EXCLUDE_BONE_BUTTON);
    this->embedAnimationCheckbox->SetId(EMBED_ANIMATION_CHECKBOX);

    SetSizer(topSizer);
        
    if (!WindowPlacementManager::RestorePlacement(this))
    {
        Layout();
        Centre(wxBOTH);
    }
}

MeshAnimationSettingsDialog::~MeshAnimationSettingsDialog()
{
}

void MeshAnimationSettingsDialog::GetGUIData()
{
    //Track locations choice
    int selectedTrack = this->trackChoice->GetSelection();
    this->settings.trackName = ((MeshAnimationTrack*)this->trackChoice->GetClientData(selectedTrack))->name;

    //Animation name
    this->settings.animationName = this->nameText->GetValue();

    //Start/end frame
    this->settings.interval.start.SetValue(this->startFrameSpinner->GetValue());
    this->settings.interval.end.SetValue(this->endFrameSpinner->GetValue());

    //Sample type
    auto sampling = StringUtilities::ToDouble(this->samplingText->GetValue());
    int selectedSamplingTypeIndex = this->samplingChoice->GetSelection();
    SampleType sampleType = (SampleType)*(int*)this->samplingChoice->GetClientData(selectedSamplingTypeIndex);
    SetSampleValue(this->settings.sampleInterval, this->settings.sampleType, sampling, sampleType);

    //Morph whole object
    int morphWholeObjectIndex = this->morphWholeObjectChoice->GetSelection();
    this->settings.morphWholeObject = (InheritedBool)*(int*)this->morphWholeObjectChoice->GetClientData(morphWholeObjectIndex);
    
    //Time scaling type
    float scale = this->timeScalingSpinner->GetValue();
    int selectedTimeScaleIndex = this->timeScalingChoice->GetSelection();
    wxString scaleType = (wxChar*)this->timeScalingChoice->GetClientData(selectedTimeScaleIndex);
    SetTimeScale(this->settings.scale, this->settings.scaleType, scale, scaleType);
        
    //Copy-first-key-to-last
    this->settings.copyFirstKeyToLast = this->copyFirstAnimationKeyToLastCheckbox->GetValue();

    //Export-as-morph
    this->settings.exportAsMorph = this->exportAsMorphCheckbox->GetValue();

    //Embed animation
    this->settings.embedAnimation = this->embedAnimationCheckbox->GetValue();

    //Link to object
    this->settings.linkToObject = this->linkToObjectCheckbox->GetValue();

    //Animated root start time
    this->settings.overrideAnimatedRootStartTime = this->overrideAnimatedRootStartTimeCheckbox->GetValue();
    this->settings.animatedRootStartTime.SetValue(this->animatedRootStartTimeSpinner->GetValue());

    wxString boneName;

    //Remove bone translations
    this->settings.boneTranslationMasks.clear();
    wxString removeBoneTranslationsText = this->removeBoneTranslationsText->GetValue();
    StringSplitter removeBoneTranslations(removeBoneTranslationsText, wxT('\n'), StringSplitterFlags::TRIM | StringSplitterFlags::IGNORE_EMPTY);
    this->settings.boneTranslationMasks.reserve(removeBoneTranslations.size());
    wxString xyz;
    for (size_t i = 0; i < removeBoneTranslations.size(); i++)
    {
        if (StringUtilities::ParseKeyValueLine(removeBoneTranslations[i], boneName, xyz))
        {
            BoneTranslationMask mask;
            mask.object = ObjectAccessor::GetObjectByName(boneName);
            if (mask.object.IsValid())
            {
                mask.mask = StringUtilities::ToRemoveXYZVector3(xyz);
                this->settings.boneTranslationMasks.push_back(mask);                
            }
        }
    }

    //Include/exclude bones
    wxString exportBonesText = this->includeExcludeBonesText->GetValue();
    StringSplitter exportBones(exportBonesText, wxT('\n'), StringSplitterFlags::TRIM | StringSplitterFlags::IGNORE_EMPTY);
    this->settings.allowExportBones.clear();
    this->settings.allowExportBones.reserve(exportBones.size());
    wxString allowText;
    wxString bonePrefixKey;
    for (size_t i = 0; i < exportBones.size(); i++)
    {
        if (StringUtilities::ParsePrefixedKeyValueLine(exportBones[i], boneName, allowText, &bonePrefixKey))
        {
            AllowExportBone allow;
            allow.object = ObjectAccessor::GetObjectByName(boneName);
            if (allow.object.IsValid())
            {
                allow.allow = allowText == INCLUDE_TEXT;
            
                if (bonePrefixKey == BONE_KEY)
                {
                    allow.recursive = false;
                    this->settings.allowExportBones.push_back(allow);
                }
                else if (bonePrefixKey == BONE_TREE_KEY)
                {
                    allow.recursive = true;
                    this->settings.allowExportBones.push_back(allow);
                }
            }
        }
    }
}

void MeshAnimationSettingsDialog::SetGUIData()
{
    //Track locations choice
    this->trackChoice->Freeze();
    for (size_t i = 0; i < this->tracks.size(); i++)
    {
        this->trackChoice->Append(this->tracks[i].displayName);
        this->trackChoice->SetClientData((unsigned int)i, &this->tracks[i]);
    }
    this->trackChoice->Thaw();
    this->trackChoice->SetSelection(this->selectedTrack);

    //Animation name
    this->nameText->SetValue(this->settings.animationName);

    //Start/end frame
    this->startFrameSpinner->SetValue(this->settings.interval.start.GetValue());
    this->endFrameSpinner->SetValue(this->settings.interval.end.GetValue());

    //Sample value
    this->samplingText->SetValue(PropertyValues::ChooseAnimationSampleValue(this->settings.sampleType, this->settings.sampleInterval));

    //Sample type
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
    
    //Morph whole object
    this->morphWholeObjectChoice->Freeze();
    for (int i = 0; i < INHERITED_BOOL_COUNT; i++)
    {
        this->morphWholeObjectChoice->Append(INHERITED_BOOLS_TEXT[i]);
        this->morphWholeObjectChoice->SetClientData(i, (void*)&INHERITED_BOOLS_VALUE[i]);
        if (this->settings.morphWholeObject == INHERITED_BOOLS_VALUE[i])
            this->morphWholeObjectChoice->SetSelection(i);
    }
    this->morphWholeObjectChoice->Thaw();
    if (this->morphWholeObjectChoice->GetSelection() < 0)
        this->morphWholeObjectChoice->SetSelection(0);
    
    //Time scaling type
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
    
    //Time scaling
    this->timeScalingSpinner->SetValue(this->settings.scale);

    //Copy-first-key-to-last
    this->copyFirstAnimationKeyToLastCheckbox->SetValue(this->settings.copyFirstKeyToLast);

    //Export-as-morph
    this->exportAsMorphCheckbox->SetValue(this->settings.exportAsMorph);

    //Embed animation
    this->embedAnimationCheckbox->SetValue(this->settings.embedAnimation);

    //Link to object
    this->linkToObjectCheckbox->SetValue(this->settings.linkToObject);

    //Animated root start time
    this->overrideAnimatedRootStartTimeCheckbox->SetValue(this->settings.overrideAnimatedRootStartTime);
    if (this->settings.overrideAnimatedRootStartTime)
        this->animatedRootStartTimeSpinner->SetValue(this->settings.animatedRootStartTime.GetValue());

    //Remove bone translations
    wxString removeBoneTranslationsText;
    for (size_t i = 0; i < this->settings.boneTranslationMasks.size(); i++)
    {
        removeBoneTranslationsText += this->settings.boneTranslationMasks[i].object.GetFullName();
        removeBoneTranslationsText += wxT("=");
        removeBoneTranslationsText += StringUtilities::ToRemoveXYZString(this->settings.boneTranslationMasks[i].mask);
        removeBoneTranslationsText += wxT("\n");
    }        
    this->removeBoneTranslationsText->SetValue(removeBoneTranslationsText); 

    //Exclude/include bones
    wxString exportBonesText;
    wxString allow;
    for (size_t i = 0; i < this->settings.allowExportBones.size(); i++)
    {
        allow = this->settings.allowExportBones[i].allow ? INCLUDE_TEXT : EXCLUDE_TEXT;
        if (this->settings.allowExportBones[i].recursive)
            StringUtilities::AppendPrefixedKeyAndValue(exportBonesText, BONE_TREE_KEY, this->settings.allowExportBones[i].object.GetFullName(), allow, wxT("\n"));
        else
            StringUtilities::AppendPrefixedKeyAndValue(exportBonesText, BONE_KEY, this->settings.allowExportBones[i].object.GetFullName(), allow, wxT("\n"));
    }
    this->includeExcludeBonesText->SetValue(exportBonesText);    

    //Final updates
    this->nameText->SetFocus();
    UpdateTrackControls(false);
    UpdateTimeScaleEdit(false);
    UpdateSamplingTypeEdit(false);    
    Layout();
}

void MeshAnimationSettingsDialog::UpdateTrackControls(bool layout)
{
    int selectedTrack = this->trackChoice->GetSelection();
    MeshAnimationTrack* track = (MeshAnimationTrack*)this->trackChoice->GetClientData(selectedTrack);

    bool isObjectBased = MeshAnimationTracksDetector::IsObjectBased(track->location);
    this->morphWholeObjectLabel->Enable(isObjectBased);
    this->morphWholeObjectChoice->Enable(isObjectBased);

    bool isSkeletonBased = MeshAnimationTracksDetector::IsSkeletonBased(track->location);

    bool showExportAsMorph = 
        isSkeletonBased || 
        MeshAnimationTracksDetector::IsMorpherBased(track->location);
    this->exportAsMorphCheckbox->Enable(showExportAsMorph);
    
    this->addRemoveTranslationsBoneButton->Enable(isSkeletonBased);
    this->removeBoneTranslationsText->Enable(isSkeletonBased);

    this->includeExcludeBonesText->Enable(isSkeletonBased);
    this->includeExcludeBonesButton->Enable(isSkeletonBased);

    UpdateEmbedControls(false);
    UpdateOverrideAnimatedRootStartTimeControls(false);
    if (layout)
        Layout();
}

void MeshAnimationSettingsDialog::UpdateTimeScaleEdit(bool layout)
{
    int selectedTimeScaleIndex = this->timeScalingChoice->GetSelection();
    bool show = selectedTimeScaleIndex == 1 || selectedTimeScaleIndex == 2;
    this->timeScalingSpinner->Show(show);    
    
    if (layout)
        Layout();
}

void MeshAnimationSettingsDialog::UpdateSamplingTypeEdit(bool layout)
{
    int selectedSamplingTypeIndex = this->samplingChoice->GetSelection();
    bool show = selectedSamplingTypeIndex > 0;
    this->samplingText->Show(show);
    
    if (layout)
        Layout();
}

void MeshAnimationSettingsDialog::UpdateEmbedControls(bool layout)
{
    auto embed = this->embedAnimationCheckbox->GetValue();
    this->linkToObjectCheckbox->Enable(!embed);
    if (layout)
        Layout();
}

void MeshAnimationSettingsDialog::UpdateOverrideAnimatedRootStartTimeControls(bool layout)
{
    int selectedTrack = this->trackChoice->GetSelection();
    MeshAnimationTrack* track = (MeshAnimationTrack*)this->trackChoice->GetClientData(selectedTrack);

    bool isSkeletonBased = MeshAnimationTracksDetector::IsSkeletonBased(track->location);

    this->overrideAnimatedRootStartTimeCheckbox->Enable(isSkeletonBased);

    bool canChangedStartTime = isSkeletonBased && this->overrideAnimatedRootStartTimeCheckbox->GetValue();
    this->animatedRootStartTimeSpinner->Enable(canChangedStartTime);

    if (layout)
        Layout();
}

void MeshAnimationSettingsDialog::OnOK(wxCommandEvent& event)
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

void MeshAnimationSettingsDialog::OnTrackChoice(wxCommandEvent& event)
{
    UpdateTrackControls();
}

void MeshAnimationSettingsDialog::OnTimeScaleChoice(wxCommandEvent& event)
{
    UpdateTimeScaleEdit();
}

void MeshAnimationSettingsDialog::OnSamplingTypeChoice(wxCommandEvent& event)
{
    UpdateSamplingTypeEdit();    
}

void MeshAnimationSettingsDialog::OnOverrideAnimatedRootStartTimeCheckbox(wxCommandEvent& event)
{
    UpdateOverrideAnimatedRootStartTimeControls();
}

void MeshAnimationSettingsDialog::OnRemoveBoneTranslationButton(wxCommandEvent& event)
{
    SUSPEND_CUSTOM_CONTROLS;
    RemoveBoneTranslationsDialog dialog(this, this->meshBones);
    ModalDialogFix modalDialogFix(&dialog);
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString removeBoneTranslationsText = this->removeBoneTranslationsText->GetValue();
        StringUtilities::AppendKeyAndValue
            (
            removeBoneTranslationsText, 
            dialog.boneObject.GetFullName(), 
            StringUtilities::ToRemoveXYZString(dialog.mask), 
            wxT("\n")
            );
        this->removeBoneTranslationsText->SetValue(removeBoneTranslationsText);
    }
}

void MeshAnimationSettingsDialog::OnAddIncludeExcludeBoneButton(wxCommandEvent& event)
{
    SUSPEND_CUSTOM_CONTROLS;
    IncludeExcludeBoneDialog dialog(this, this->meshBones);
    ModalDialogFix modalDialogFix(&dialog);
    if (dialog.ShowModal() == wxID_OK)
    {
        const wxString& prefix = dialog.isTree ? BONE_TREE_KEY : BONE_KEY;
        const wxString& allow = dialog.include ? INCLUDE_TEXT : EXCLUDE_TEXT;
        
        wxString exportBonesText = this->includeExcludeBonesText->GetValue();
        StringUtilities::AppendPrefixedKeyAndValue(exportBonesText, prefix, dialog.boneObject.GetFullName(), allow, wxT("\n"));
        this->includeExcludeBonesText->SetValue(exportBonesText);
    }
}

void MeshAnimationSettingsDialog::OnEmbedCheckbox(wxCommandEvent& event)
{
    UpdateEmbedControls();
}
