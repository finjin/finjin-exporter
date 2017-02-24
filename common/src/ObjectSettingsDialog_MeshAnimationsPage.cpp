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
#include "ObjectSettingsDialog_MeshAnimationsPage.hpp"
#include "ApplicationControls.hpp"
#include "StringUtilities.hpp"
#include "Strings.hpp"
#include "AnimationUtilities.hpp"
#include "MeshAnimationSettingsDialog.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "PropertyValues.hpp"
#include "MeshSkeleton.hpp"
#include "ChooseObjectsPrompt.hpp"
#include "ApplicationAccessor.hpp"
#include "ModalDialogFix.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
const wxString ObjectSettingsDialog_MeshAnimationsPage::TITLE(wxT("Mesh Animations"));
static ObjectSettingsDialog_MeshAnimationsPage* page = nullptr;


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(ObjectSettingsDialog_MeshAnimationsPage, SettingsPage)
    EVT_CHOICE(ObjectSettingsDialog_MeshAnimationsPage::MESH_ANIMATION_TYPE_CHOICE, ObjectSettingsDialog_MeshAnimationsPage::OnMeshAnimationTypeChoice)
    EVT_CHOICE(ObjectSettingsDialog_MeshAnimationsPage::REFERENCE_POSE_CHOICE, ObjectSettingsDialog_MeshAnimationsPage::OnReferencePoseChoice)
    EVT_CHECKBOX(ObjectSettingsDialog_MeshAnimationsPage::EXPORT_SKELETON_CHECKBOX, ObjectSettingsDialog_MeshAnimationsPage::OnExportSkeletonCheckBox)
    EVT_CHECKBOX(ObjectSettingsDialog_MeshAnimationsPage::EMBED_SKELETON_CHECKBOX, ObjectSettingsDialog_MeshAnimationsPage::OnEmbedSkeletonInMeshCheckBox)
    EVT_TEXT(ObjectSettingsDialog_MeshAnimationsPage::SKELETON_ANIMATION_SAMPLING_TEXT, ObjectSettingsDialog_MeshAnimationsPage::OnSkeletonAnimationSamplingTextChanged)
    EVT_CHOICE(ObjectSettingsDialog_MeshAnimationsPage::SKELETON_ANIMATION_SAMPLING_CHOICE, ObjectSettingsDialog_MeshAnimationsPage::OnSkeletonAnimationSamplingChoice)
    EVT_TEXT(ObjectSettingsDialog_MeshAnimationsPage::VERTEX_ANIMATION_SAMPLING_TEXT, ObjectSettingsDialog_MeshAnimationsPage::OnVertexAnimationSamplingTextChanged)
    EVT_CHOICE(ObjectSettingsDialog_MeshAnimationsPage::VERTEX_ANIMATION_SAMPLING_CHOICE, ObjectSettingsDialog_MeshAnimationsPage::OnVertexAnimationSamplingChoice)
    EVT_BUTTON(ObjectSettingsDialog_MeshAnimationsPage::ANIMATED_ROOT_CHOOSE_BONE_BUTTON, ObjectSettingsDialog_MeshAnimationsPage::OnAnimatedRootChooseBoneButton)
    EVT_BUTTON(ObjectSettingsDialog_MeshAnimationsPage::ANIMATED_ROOT_CHOOSE_OBJECT_BUTTON, ObjectSettingsDialog_MeshAnimationsPage::OnAnimatedRootChooseObjectButton)
    EVT_BUTTON(ObjectSettingsDialog_MeshAnimationsPage::ADD_ANIMATION_BUTTON, ObjectSettingsDialog_MeshAnimationsPage::OnAddAnimationButton)
    EVT_BUTTON(ObjectSettingsDialog_MeshAnimationsPage::EDIT_ANIMATION_BUTTON, ObjectSettingsDialog_MeshAnimationsPage::OnEditAnimationButton)
    EVT_BUTTON(ObjectSettingsDialog_MeshAnimationsPage::COPY_ANIMATION_BUTTON, ObjectSettingsDialog_MeshAnimationsPage::OnCopyAnimationButton)
    EVT_BUTTON(ObjectSettingsDialog_MeshAnimationsPage::REMOVE_ANIMATION_BUTTON, ObjectSettingsDialog_MeshAnimationsPage::OnRemoveAnimationButton)
    EVT_BUTTON(ObjectSettingsDialog_MeshAnimationsPage::SYNC_TO_BIPED_MIXER_BUTTON, ObjectSettingsDialog_MeshAnimationsPage::OnSyncToBipedMixerButton)
    EVT_LIST_ITEM_ACTIVATED(ObjectSettingsDialog_MeshAnimationsPage::MESH_ANIMATION_LIST, ObjectSettingsDialog_MeshAnimationsPage::OnMeshAnimationListActivate)    
    EVT_LIST_COL_CLICK(ObjectSettingsDialog_MeshAnimationsPage::MESH_ANIMATION_LIST, ObjectSettingsDialog_MeshAnimationsPage::OnMeshAnimationListColumnClick)    
    EVT_LIST_ITEM_SELECTED(ObjectSettingsDialog_MeshAnimationsPage::MESH_ANIMATION_LIST, ObjectSettingsDialog_MeshAnimationsPage::OnMeshAnimationListSelectionChange)    
    EVT_LIST_ITEM_DESELECTED(ObjectSettingsDialog_MeshAnimationsPage::MESH_ANIMATION_LIST, ObjectSettingsDialog_MeshAnimationsPage::OnMeshAnimationListSelectionChange)    
END_EVENT_TABLE()

ObjectSettingsDialog_MeshAnimationsPage::ObjectSettingsDialog_MeshAnimationsPage(wxWindow* parent, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    page = this;

    FinjinTimeInterval animationRange = ApplicationAccessor::GetAnimationRange();

    this->settingGuiData = false;
    this->sortColumn = 0;
    this->invertSorting = 1;

    this->object = object;
    this->objectSettings = objectSettings;
    this->sceneSettings = sceneSettings;
    this->trackDetector.Detect(this->object);
    MeshSkeleton meshSkeleton(nullptr);
    CoordinateSystemConverter nullConverter(ApplicationAccessor::GetUpAxis());
    if (meshSkeleton.Initialize(this->object, nullConverter, 1, nullptr))
        meshSkeleton.GetBones(this->meshBones);            
    this->animatedSkeletonRootObject = this->objectSettings.GetAnimatedRoot();
    
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* animationTypeSizer;
    animationTypeSizer = new wxBoxSizer( wxHORIZONTAL );
    
    meshAnimationTypeLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Mesh Animation Type"), wxDefaultPosition, wxDefaultSize, 0 );
    meshAnimationTypeLabel->Wrap( -1 );
    animationTypeSizer->Add( meshAnimationTypeLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    
    wxArrayString meshAnimationTypeChoiceChoices;
    meshAnimationTypeChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 140,-1 ), meshAnimationTypeChoiceChoices, 0 );
    meshAnimationTypeChoice->SetSelection( 0 );
    animationTypeSizer->Add( meshAnimationTypeChoice, 0, wxRIGHT|wxLEFT|wxBOTTOM, 5 );
    
    selectedMeshAnimationTypeLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    selectedMeshAnimationTypeLabel->Wrap( -1 );
    animationTypeSizer->Add( selectedMeshAnimationTypeLabel, 0, wxALL, 5 );
    
    topSizer->Add( animationTypeSizer, 0, wxEXPAND|wxTOP, 5 );
    
    wxStaticBoxSizer* skeletonSettingsSizer;
    skeletonSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Skeleton Settings") ), wxVERTICAL );

    wxBoxSizer* alwaysExportSizer;
    alwaysExportSizer = new wxBoxSizer( wxHORIZONTAL );
    
    exportSkeletonLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Export Skeleton"), wxDefaultPosition, wxSize( 90,-1 ), 0 );
    exportSkeletonLabel->Wrap( -1 );
    alwaysExportSizer->Add( exportSkeletonLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5 );
    
    exportSkeletonCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Skeleton"), wxDefaultPosition, wxDefaultSize, 0 );
    alwaysExportSizer->Add( exportSkeletonCheckbox, 0, wxALL, 5 );

    exportedMeshDeformedBySkeletonCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Skeleton Deforms Exported Mesh"), wxDefaultPosition, wxDefaultSize, 0 );
    
    alwaysExportSizer->Add( exportedMeshDeformedBySkeletonCheckbox, 0, wxALL, 5 );

    embedSkeletonCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Embed in Mesh"), wxDefaultPosition, wxDefaultSize, 0 );
    alwaysExportSizer->Add( embedSkeletonCheckbox, 0, wxALL, 5 );
    
    linkSkeletonToObjectCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Link to Mesh"), wxDefaultPosition, wxDefaultSize, 0 );
    alwaysExportSizer->Add( linkSkeletonToObjectCheckbox, 0, wxALL, 5 );

    skeletonSettingsSizer->Add( alwaysExportSizer, 0, wxEXPAND, 5 );

    wxBoxSizer* referencePoseSizer;
    referencePoseSizer = new wxBoxSizer( wxHORIZONTAL );
    
    referencePoseLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Reference Pose"), wxDefaultPosition, wxSize( 90,-1 ), 0 );
    referencePoseLabel->Wrap( -1 );
    referencePoseSizer->Add( referencePoseLabel, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
    
    wxArrayString referencePoseChoiceChoices;
    referencePoseChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxSize(90, -1), referencePoseChoiceChoices, 0 );
    referencePoseChoice->SetSelection( 0 );
    referencePoseSizer->Add( referencePoseChoice, 0, wxBOTTOM|wxRIGHT, 5 );
    
    referencePoseTimeSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(70, -1), 0, animationRange.start.GetValue(), animationRange.end.GetValue(), SpinnerControl::UI_TIME_UNIT);
    referencePoseSizer->Add( referencePoseTimeSpinner, 0, wxRIGHT, 5 );
    
    referencePoseNoteLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("(Use only with Skeleton animation type)"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
    referencePoseNoteLabel->Wrap( -1 );
    referencePoseSizer->Add( referencePoseNoteLabel, 0, wxALL, 5 );
    
    skeletonSettingsSizer->Add( referencePoseSizer, 0, wxEXPAND, 5 );

    wxBoxSizer* skeletonNameSizer;
    skeletonNameSizer = new wxBoxSizer( wxHORIZONTAL );

    skeletonNameLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Skeleton Name"), wxDefaultPosition, wxSize( 90,-1 ), 0 );
    skeletonNameLabel->Wrap( -1 );
    skeletonNameSizer->Add( skeletonNameLabel, 0, wxALL, 5 );
    
    skeletonNameText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), 0 );
    skeletonNameSizer->Add( skeletonNameText, 0, wxRIGHT, 5 );

    skeletonSettingsSizer->Add( skeletonNameSizer, 0, wxEXPAND, 5 );

    wxFlexGridSizer* animatedRootSizer;
    animatedRootSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
    animatedRootSizer->SetFlexibleDirection( wxBOTH );
    animatedRootSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    animatedRootLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Animated Root"), wxDefaultPosition, wxSize( 90,-1 ), 0 );
    animatedRootLabel->Wrap( -1 );
    animatedRootSizer->Add( animatedRootLabel, 0, wxALL, 5 );
    
    animatedRootText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 320,-1 ), wxTE_READONLY );
    animatedRootSizer->Add( animatedRootText, 1, 0, 5 );
    
    
    animatedRootSizer->Add( 0, 0, 1, wxEXPAND, 5 );
    
    wxBoxSizer* animatedSkeletonRootButtonsSizer;
    animatedSkeletonRootButtonsSizer = new wxBoxSizer( wxHORIZONTAL );
    
    animatedRootChooseBoneButton = new wxButton( this, wxID_ANY, wxT("Choose Bone..."), wxDefaultPosition, wxSize(STANDARD_BUTTON_WIDTH,-1 ), 0 );
    animatedSkeletonRootButtonsSizer->Add( animatedRootChooseBoneButton, 0, wxRIGHT, 5 );
    
    animatedRootChooseObjectButton = new wxButton( this, wxID_ANY, wxT("Choose Object..."), wxDefaultPosition, wxSize(STANDARD_BUTTON_WIDTH,-1 ), 0 );
    animatedSkeletonRootButtonsSizer->Add( animatedRootChooseObjectButton, 0, wxRIGHT, 5 );
    
    animatedRootSizer->Add( animatedSkeletonRootButtonsSizer, 1, wxEXPAND, 5 );
    
    skeletonSettingsSizer->Add( animatedRootSizer, 0, wxEXPAND, 0 );

    wxBoxSizer* skeletonAnimationSamplingSizer;
    skeletonAnimationSamplingSizer = new wxBoxSizer( wxHORIZONTAL );
    
    skeletonAnimationSamplingLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Skeleton Animation Sampling"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
    skeletonAnimationSamplingLabel->Wrap( -1 );
    skeletonAnimationSamplingLabel->SetMinSize( wxSize( 140,-1 ) );
    
    skeletonAnimationSamplingSizer->Add( skeletonAnimationSamplingLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );
    
    wxBoxSizer* skeletonAnimationSamplingValuesSizer;
    skeletonAnimationSamplingValuesSizer = new wxBoxSizer( wxHORIZONTAL );
    
    skeletonAnimationSamplingText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    skeletonAnimationSamplingValuesSizer->Add( skeletonAnimationSamplingText, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );
    
    wxArrayString skeletonAnimationSamplingChoiceChoices;
    skeletonAnimationSamplingChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, skeletonAnimationSamplingChoiceChoices, 0 );
    skeletonAnimationSamplingChoice->SetSelection( 0 );
    skeletonAnimationSamplingValuesSizer->Add( skeletonAnimationSamplingChoice, 1, wxALL, 5 );
    
    skeletonAnimationSamplingSizer->Add( skeletonAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );
    
    skeletonSettingsSizer->Add( skeletonAnimationSamplingSizer, 1, wxEXPAND, 5 );

    removeBonesWithNoInfluenceCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Remove Bones With No Influence"), wxDefaultPosition, wxDefaultSize, 0 );    
    skeletonSettingsSizer->Add( removeBonesWithNoInfluenceCheckBox, 0, wxALL, 5 );
    
    topSizer->Add( skeletonSettingsSizer, 0, wxEXPAND|wxTOP, 5 );
    
    wxStaticBoxSizer* poseSettingsSizer;
    poseSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Pose Settings") ), wxVERTICAL );
    
    exportPosesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Poses"), wxDefaultPosition, wxDefaultSize, 0 );
    
    poseSettingsSizer->Add( exportPosesCheckbox, 0, wxALL, 5 );
    
    topSizer->Add( poseSettingsSizer, 0, wxEXPAND|wxTOP, 5 );
    
    wxStaticBoxSizer* morphSettingsSizer;
    morphSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Morph Settings") ), wxVERTICAL );
    
    wxBoxSizer* vertexAnimationSamplingSizer;
    vertexAnimationSamplingSizer = new wxBoxSizer( wxHORIZONTAL );
    
    vertexAnimationSamplingLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Vertex Animation Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    vertexAnimationSamplingLabel->Wrap( -1 );
    vertexAnimationSamplingLabel->SetMinSize( wxSize( 140,-1 ) );
    
    vertexAnimationSamplingSizer->Add( vertexAnimationSamplingLabel, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
    
    wxBoxSizer* vertexAnimationSamplingValuesSizer;
    vertexAnimationSamplingValuesSizer = new wxBoxSizer( wxHORIZONTAL );
    
    vertexAnimationSamplingText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    vertexAnimationSamplingValuesSizer->Add( vertexAnimationSamplingText, 0, wxBOTTOM|wxLEFT, 5 );
    
    wxArrayString vertexAnimationSamplingChoiceChoices;
    vertexAnimationSamplingChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, vertexAnimationSamplingChoiceChoices, 0 );
    vertexAnimationSamplingChoice->SetSelection( 0 );
    vertexAnimationSamplingValuesSizer->Add( vertexAnimationSamplingChoice, 1, wxBOTTOM|wxLEFT, 5 );
    
    vertexAnimationSamplingSizer->Add( vertexAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );
    
    morphSettingsSizer->Add( vertexAnimationSamplingSizer, 1, wxEXPAND, 0 );
    
    topSizer->Add( morphSettingsSizer, 0, wxEXPAND|wxTOP, 5 );
    
    wxStaticBoxSizer* meshAnimationsSizer;
    meshAnimationsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Mesh Animations") ), wxVERTICAL );
    
    wxGridSizer* meshAnimationsControlsSizer;
    meshAnimationsControlsSizer = new wxGridSizer( 1, 2, 0, 0 );
    
    wxBoxSizer* meshAnimationsButtonsSizer;
    meshAnimationsButtonsSizer = new wxBoxSizer( wxHORIZONTAL );
    
    addMeshAnimationButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxSize(65, -1), 0 );
    meshAnimationsButtonsSizer->Add( addMeshAnimationButton, 0, wxLEFT, 5 );
    
    editMeshAnimationButton = new wxButton( this, wxID_ANY, wxT("Edit..."), wxDefaultPosition, wxSize(65, -1), 0 );
    meshAnimationsButtonsSizer->Add( editMeshAnimationButton, 0, 0, 5 );

    copyMeshAnimationButton = new wxButton( this, wxID_ANY, wxT("Copy"), wxDefaultPosition, wxSize(65, -1), 0 );
    meshAnimationsButtonsSizer->Add( copyMeshAnimationButton, 0, 0, 5 );

    removeMeshAnimationButton = new wxButton( this, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxSize(65, -1), 0 );
    meshAnimationsButtonsSizer->Add( removeMeshAnimationButton, 0, 0, 5 );
    
    meshAnimationsControlsSizer->Add( meshAnimationsButtonsSizer, 1, wxEXPAND, 5 );
    
    syncToBipedMixerButton = new wxButton( this, wxID_ANY, wxT("Sync to Mixer"), wxDefaultPosition, wxDefaultSize, 0 );
    meshAnimationsControlsSizer->Add( syncToBipedMixerButton, 0, wxALIGN_RIGHT|wxRIGHT|wxLEFT, 5 );
    
    meshAnimationsSizer->Add( meshAnimationsControlsSizer, 0, wxEXPAND, 5 );
    
    meshAnimationsList = new ApplicationListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_HRULES|wxLC_VRULES );
    meshAnimationsSizer->Add( meshAnimationsList, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    topSizer->Add( meshAnimationsSizer, 1, wxEXPAND|wxTOP, 5 );

    this->meshAnimationTypeChoice->SetId(MESH_ANIMATION_TYPE_CHOICE);
    this->referencePoseChoice->SetId(REFERENCE_POSE_CHOICE);
    this->exportSkeletonCheckbox->SetId(EXPORT_SKELETON_CHECKBOX);
    this->embedSkeletonCheckbox->SetId(EMBED_SKELETON_CHECKBOX);
    this->skeletonAnimationSamplingText->SetId(SKELETON_ANIMATION_SAMPLING_TEXT);
    this->skeletonAnimationSamplingChoice->SetId(SKELETON_ANIMATION_SAMPLING_CHOICE);
    this->vertexAnimationSamplingText->SetId(VERTEX_ANIMATION_SAMPLING_TEXT);
    this->vertexAnimationSamplingChoice->SetId(VERTEX_ANIMATION_SAMPLING_CHOICE);
    this->animatedRootText->SetId(ANIMATED_ROOT_TEXT);
    this->animatedRootChooseBoneButton->SetId(ANIMATED_ROOT_CHOOSE_BONE_BUTTON);
    this->animatedRootChooseObjectButton->SetId(ANIMATED_ROOT_CHOOSE_OBJECT_BUTTON);
    this->addMeshAnimationButton->SetId(ADD_ANIMATION_BUTTON);
    this->editMeshAnimationButton->SetId(EDIT_ANIMATION_BUTTON);
    this->copyMeshAnimationButton->SetId(COPY_ANIMATION_BUTTON);
    this->removeMeshAnimationButton->SetId(REMOVE_ANIMATION_BUTTON);
    this->syncToBipedMixerButton->SetId(SYNC_TO_BIPED_MIXER_BUTTON);
    this->meshAnimationsList->SetId(MESH_ANIMATION_LIST);

    SetSizer(topSizer);
    Layout();
}

ObjectSettingsDialog_MeshAnimationsPage::~ObjectSettingsDialog_MeshAnimationsPage()
{
}

bool ObjectSettingsDialog_MeshAnimationsPage::GetGUIData()
{
    return GetGUIData(ALL_IDS);
}

bool ObjectSettingsDialog_MeshAnimationsPage::GetGUIData(int ids)
{
    //Animation type
    if (ids & MESH_ANIMATION_TYPE_CHOICE)
    {
        int selectedTypeIndex = this->meshAnimationTypeChoice->GetSelection();
        auto meshAnimationType = (MeshAnimationType*)this->meshAnimationTypeChoice->GetClientData(selectedTypeIndex);
        this->objectSettings.SetMeshAnimationType(meshAnimationType->name);
    }
    
    //Skeleton name
    this->objectSettings.SetSkeletonName(this->skeletonNameText->GetValue());

    //Export skeleton
    this->objectSettings.SetExportSkeleton(this->exportSkeletonCheckbox->GetValue());

    //Reference pose/time
    int referencePoseIndex = this->referencePoseChoice->GetSelection();
    this->objectSettings.SetSkeletonReferencePoseType((wxChar*)this->referencePoseChoice->GetClientData(referencePoseIndex));
    TimeAccessor referencePoseTime;
    referencePoseTime.SetValue(this->referencePoseTimeSpinner->GetValue());
    this->objectSettings.SetSkeletonReferencePoseTime(referencePoseTime);

    //Skeleton deforms exported mesh
    this->objectSettings.SetExportedMeshDeformedBySkeleton(this->exportedMeshDeformedBySkeletonCheckbox->GetValue());

    //Embed/link skeleton
    this->objectSettings.SetEmbedSkeletonInMesh(this->embedSkeletonCheckbox->GetValue());
    this->objectSettings.SetLinkSkeletonToMesh(this->linkSkeletonToObjectCheckbox->GetValue());

    //Export poses
    this->objectSettings.SetExportPoses(this->exportPosesCheckbox->GetValue());

    //Remove bones with no vertex influence
    this->objectSettings.SetRemoveBonesWithNoVertexInfluence(this->removeBonesWithNoInfluenceCheckBox->GetValue());

    //Skeleton animation sampling
    if (ids & (SKELETON_ANIMATION_SAMPLING_TEXT | SKELETON_ANIMATION_SAMPLING_CHOICE))
    {
        int selectedSkeletonAnimationSampleTypeIndex = this->skeletonAnimationSamplingChoice->GetSelection();
        auto skeletonAnimationSampleType = (SampleType)*(int*)this->skeletonAnimationSamplingChoice->GetClientData(selectedSkeletonAnimationSampleTypeIndex);    
        auto skeletonAnimationSampling = StringUtilities::ToDouble(this->skeletonAnimationSamplingText->GetValue());
        if (skeletonAnimationSampling < 0)
            skeletonAnimationSampling = 0;
        this->objectSettings.SetSkeletonAnimationSampling(skeletonAnimationSampling, skeletonAnimationSampleType, skeletonAnimationSampleType != SampleType::USE_PARENT);
    }

    //Vertex animation sampling
    if (ids & (VERTEX_ANIMATION_SAMPLING_TEXT | VERTEX_ANIMATION_SAMPLING_CHOICE))
    {
        int selectedVertexAnimationSampleTypeIndex = this->vertexAnimationSamplingChoice->GetSelection();
        auto vertexAnimationSampleType = (SampleType)*(int*)this->vertexAnimationSamplingChoice->GetClientData(selectedVertexAnimationSampleTypeIndex);
        auto vertexAnimationSampling = StringUtilities::ToDouble(this->vertexAnimationSamplingText->GetValue());
        if (vertexAnimationSampling < 0)
            vertexAnimationSampling = 0;
        this->objectSettings.SetVertexAnimationSampling(vertexAnimationSampling, vertexAnimationSampleType, vertexAnimationSampleType != SampleType::USE_PARENT);
    }

    //Animated skeleton root
    if (ids & (ANIMATED_ROOT_TEXT | ANIMATED_ROOT_CHOOSE_BONE_BUTTON | ANIMATED_ROOT_CHOOSE_OBJECT_BUTTON))
        this->objectSettings.SetAnimatedRoot(this->animatedSkeletonRootObject);
    
    return true;
}

bool ObjectSettingsDialog_MeshAnimationsPage::SetGUIData()
{    
    this->settingGuiData = true;

    auto showMixer = this->mixerControl.Initialize(this->object, this->objectSettings);
    this->syncToBipedMixerButton->Show(showMixer);

    //Animation type
    auto meshAnimationType = this->objectSettings.GetMeshAnimationType();
    this->defaultMeshAnimationType.Detect(this->object, 0, true, this->objectSettings);
    this->meshAnimationType.Detect(this->object, &meshAnimationType, true, this->objectSettings);
    this->meshAnimationTypeChoice->Freeze();
    this->meshAnimationTypeChoice->Append(Strings::DEFAULT);    
    this->meshAnimationTypeChoice->SetClientData(0, this->defaultMeshAnimationType.selectedType);
    for (int i = 0; i < (int)this->meshAnimationType.types.size(); i++)
    {
        this->meshAnimationTypeChoice->Append(this->meshAnimationType.types[i].displayName);    
        this->meshAnimationTypeChoice->SetClientData(i + 1, &this->meshAnimationType.types[i]);
    }
    this->meshAnimationTypeChoice->Thaw();
    this->meshAnimationTypeChoice->SetSelection(this->meshAnimationType.FindTypeIndex(this->meshAnimationType.selectedType->name) + 1);

    //Skeleton name
    this->skeletonNameText->SetValue(this->objectSettings.GetSkeletonName());

    //Export skeleton
    this->exportSkeletonCheckbox->SetValue(this->objectSettings.GetExportSkeleton());
    
    //Reference pose/time
    this->referencePoseChoice->Freeze();
    for (int i = 0; i < Strings::SKELETON_REFERENCE_POSE_COUNT; i++)
    {
        this->referencePoseChoice->Append(Strings::SKELETON_REFERENCE_POSE_TEXT[i]);
        this->referencePoseChoice->SetClientData(i, (wxChar*)Strings::SKELETON_REFERENCE_POSE_VALUE[i]);
        if (this->objectSettings.GetSkeletonReferencePoseType() == Strings::SKELETON_REFERENCE_POSE_VALUE[i])
            this->referencePoseChoice->SetSelection(i);
    }
    this->referencePoseChoice->Thaw();
    if (this->referencePoseChoice->GetSelection() < 0)
        this->referencePoseChoice->SetSelection(0);
    this->referencePoseTimeSpinner->SetValue(this->objectSettings.GetSkeletonReferencePoseTime().GetValue());

    //Skeleton deforms exported mesh
    this->exportedMeshDeformedBySkeletonCheckbox->SetValue(this->objectSettings.GetExportedMeshDeformedBySkeleton());

    //Embed/link skeleton
    this->embedSkeletonCheckbox->SetValue(this->objectSettings.GetEmbedSkeletonInMesh());
    this->linkSkeletonToObjectCheckbox->SetValue(this->objectSettings.GetLinkSkeletonToMesh());

    //Export poses checkbox
    this->exportPosesCheckbox->SetValue(this->objectSettings.GetExportPoses());

    //Remove bones with no vertex influence
    this->removeBonesWithNoInfluenceCheckBox->SetValue(this->objectSettings.GetRemoveBonesWithNoVertexInfluence());

    //Skeleton animation sampling value
    this->skeletonAnimationSamplingText->SetValue(this->objectSettings.GetSkeletonAnimationSampleValueText());
    
    //Skeleton animation sampling combo
    this->skeletonAnimationSamplingChoice->Freeze();
    for (int i = 0; i < Strings::OBJECT_SAMPLE_TYPE_COUNT; i++)
    {
        this->skeletonAnimationSamplingChoice->Append(Strings::OBJECT_SAMPLE_TYPES_TEXT[i]);
        this->skeletonAnimationSamplingChoice->SetClientData(i, (void*)&Strings::OBJECT_SAMPLE_TYPES_VALUE[i]);
        if (this->objectSettings.GetSkeletonAnimationSampleType() == Strings::OBJECT_SAMPLE_TYPES_VALUE[i])
            this->skeletonAnimationSamplingChoice->SetSelection(i);
    }
    this->skeletonAnimationSamplingChoice->Thaw();
    if (this->objectSettings.GetSkeletonAnimationSampleInterval() == 0 ||
        this->skeletonAnimationSamplingChoice->GetSelection() < 0)
    {
        this->skeletonAnimationSamplingChoice->SetSelection(0);
    }

    //Vertex animation sampling value
    this->vertexAnimationSamplingText->SetValue(this->objectSettings.GetVertexAnimationSampleValueText());

    //Vertex animation sampling combo
    this->vertexAnimationSamplingChoice->Freeze();
    for (int i = 0; i < Strings::OBJECT_SAMPLE_TYPE_COUNT; i++)
    {
        this->vertexAnimationSamplingChoice->Append(Strings::OBJECT_SAMPLE_TYPES_TEXT[i]);
        this->vertexAnimationSamplingChoice->SetClientData(i, (void*)&Strings::OBJECT_SAMPLE_TYPES_VALUE[i]);
        if (this->objectSettings.GetVertexAnimationSampleType() == Strings::OBJECT_SAMPLE_TYPES_VALUE[i])
            this->vertexAnimationSamplingChoice->SetSelection(i);
    }
    this->vertexAnimationSamplingChoice->Thaw();
    if (this->objectSettings.GetVertexAnimationSampleInterval() == 0 ||
        this->vertexAnimationSamplingChoice->GetSelection() < 0)
    {
        this->vertexAnimationSamplingChoice->SetSelection(0);
    }

    //Animated skeleton root
    this->animatedRootChooseBoneButton->Show(!this->meshBones.empty());

    //Mesh animations
    this->meshAnimationsList->InsertColumn(0, Strings::NAME, wxLIST_FORMAT_LEFT, 185);
    this->meshAnimationsList->InsertColumn(1, Strings::TRACK, wxLIST_FORMAT_LEFT, 60);
    this->meshAnimationsList->InsertColumn(2, TimeAccessor::GetUIUnit(), wxLIST_FORMAT_LEFT, 65);
    this->meshAnimationsList->InsertColumn(3, Strings::LENGTH, wxLIST_FORMAT_LEFT, 55);    
    this->meshAnimationsList->InsertColumn(4, Strings::SAMPLE_INTERVAL_RATE, wxLIST_FORMAT_LEFT, 115);
    this->objectSettings.GetMeshAnimations(this->meshAnimations);
    UpdateMeshAnimationListGUIData();
        
    //Final updates
    UpdateMeshAnimationTypeDefaultText();
    UpdateExportSkeletonControls();
    UpdateReferencePoseControls();
    UpdateSkeletonAnimationSamplingTypeEdit();
    UpdateVertexAnimationSamplingTypeEdit();
    UpdateAnimatedSkeletonRootEdit();
    UpdateMeshAnimationListButtons();
    
    this->settingGuiData = false;
    
    return true;
}

void ObjectSettingsDialog_MeshAnimationsPage::UpdateMeshAnimationTypeDefaultText()
{
    //Get selected type
    int animationTypeIndex = this->meshAnimationTypeChoice->GetSelection();    
    auto animationType = (MeshAnimationType*)this->meshAnimationTypeChoice->GetClientData(animationTypeIndex);

    //Show default text if necessary
    wxString animationTypeDefaultText;
    if (animationTypeIndex == 0)
        animationTypeDefaultText = wxString::Format(Strings::DEFAULT_FORMAT, animationType->displayName.wx_str());
    this->selectedMeshAnimationTypeLabel->SetLabel(animationTypeDefaultText);
}

void ObjectSettingsDialog_MeshAnimationsPage::UpdateExportSkeletonControls()
{
    auto exportSkeleton = this->exportSkeletonCheckbox->GetValue();
    this->exportedMeshDeformedBySkeletonCheckbox->Show(false); //!exportSkeleton; //Feature disabled because it provides no major benefit at the moment
    this->embedSkeletonCheckbox->Enable(exportSkeleton);
    this->linkSkeletonToObjectCheckbox->Enable(exportSkeleton && !this->embedSkeletonCheckbox->GetValue());
    Layout();
}

void ObjectSettingsDialog_MeshAnimationsPage::UpdateEmbedSkeletonControls()
{
    auto exportSkeleton = this->exportSkeletonCheckbox->GetValue();
    this->linkSkeletonToObjectCheckbox->Enable(exportSkeleton && !this->embedSkeletonCheckbox->GetValue());
    Layout();
}

void ObjectSettingsDialog_MeshAnimationsPage::UpdateReferencePoseControls()
{
    int referencePoseIndex = this->referencePoseChoice->GetSelection();
    wxString value = (wxChar*)this->referencePoseChoice->GetClientData(referencePoseIndex);
    this->referencePoseTimeSpinner->Show(!value.empty());
    this->referencePoseNoteLabel->Show(!value.empty()); //The note only applies to the time-based reference pose
}

void ObjectSettingsDialog_MeshAnimationsPage::UpdateSkeletonAnimationSamplingTypeEdit()
{
    int selectedIndex = this->skeletonAnimationSamplingChoice->GetSelection();
    bool show = selectedIndex > 0;
    this->skeletonAnimationSamplingText->Show(show);
    Layout();
}

void ObjectSettingsDialog_MeshAnimationsPage::UpdateVertexAnimationSamplingTypeEdit()
{
    int selectedIndex = this->vertexAnimationSamplingChoice->GetSelection();
    bool show = selectedIndex > 0;
    this->vertexAnimationSamplingText->Show(show);
    Layout();
}

void ObjectSettingsDialog_MeshAnimationsPage::UpdateAnimatedSkeletonRootEdit()
{
    wxString name;
    if (this->animatedSkeletonRootObject.IsValid())
        name = this->animatedSkeletonRootObject.GetFullName();
    this->animatedRootText->SetValue(name);
}

void ObjectSettingsDialog_MeshAnimationsPage::OnEditAnimation()
{
    int selection = this->meshAnimationsList->GetSelection();
    if (selection >= 0)
    {
        auto& anim = this->meshAnimations[this->meshAnimationsList->GetItemData(selection)];
        MeshAnimationSettingsDialog::Settings settings(anim);
        
        //Get all the current animation names and remove the one being edited
        std::vector<wxString> animationNames;
        this->objectSettings.GetMeshAnimationNames(animationNames);
        StringUtilities::RemoveName(animationNames, settings.animationName);

        //Get valid track locations
        int selectedTrack = this->trackDetector.FindTrackIndex(anim.GetTrack());        
        SUSPEND_CUSTOM_CONTROLS;
        MeshAnimationSettingsDialog dialog(this, Strings::EDIT_MESH_ANIMATION, this->meshBones, animationNames, this->trackDetector.tracks, selectedTrack, settings);
        ModalDialogFix modalDialogFix(&dialog);
        if (dialog.ShowModal() == wxID_OK)
        {
            dialog.settings.GetSettings(anim);
            UpdateMeshAnimationListGUIData((int)this->meshAnimationsList->GetItemData(selection));
        }
    }
}

void ObjectSettingsDialog_MeshAnimationsPage::UpdateMeshAnimationListButtons()
{
    int selectedItemCount = this->meshAnimationsList->GetSelectedItemCount();
    this->editMeshAnimationButton->Enable(selectedItemCount == 1);
    this->copyMeshAnimationButton->Enable(selectedItemCount == 1);
    this->removeMeshAnimationButton->Enable(selectedItemCount > 0);
}

void ObjectSettingsDialog_MeshAnimationsPage::UpdateMeshAnimationListGUIData(int selectedAnimationIndex)
{
    if (selectedAnimationIndex < 0)
    {
        int selectedIndex = this->meshAnimationsList->GetSelection();
        if (selectedIndex >= 0)
            selectedAnimationIndex = (int)this->meshAnimationsList->GetItemData(selectedIndex);
    }
    
    while (this->meshAnimationsList->GetItemCount() < (int)this->meshAnimations.size())
        this->meshAnimationsList->InsertItem(this->meshAnimationsList->GetItemCount(), wxEmptyString);
    while (this->meshAnimationsList->GetItemCount() > (int)this->meshAnimations.size())
        this->meshAnimationsList->DeleteItem(this->meshAnimationsList->GetItemCount() - 1);
    
    this->meshAnimationsList->SetSequentialItemData();

    for (int i = 0; i < (int)this->meshAnimations.size(); i++)
        UpdateMeshAnimationGUIData(i);

    if (selectedAnimationIndex >= 0)
        this->meshAnimationsList->SetSelection(selectedAnimationIndex);
    
    SortMeshAnimationsList();
}

void ObjectSettingsDialog_MeshAnimationsPage::UpdateMeshAnimationGUIData(int index)
{
    auto& anim = this->meshAnimations[this->meshAnimationsList->GetItemData(index)];
    
    auto interval = anim.GetTimeInterval();    
    auto framesText = interval.ToString(); 

    double length = interval.GetDuration().ToSecondsDouble() * anim.GetRealTimeScale();
    auto lengthText = wxString::Format(wxT("%.2fs"), length);

    auto sampling = GetAnimationSamplingText(anim);

    auto trackName = GetAnimationTrackText(anim);
    
    this->meshAnimationsList->SetItem(index, 0, anim.GetAnimationName());
    this->meshAnimationsList->SetItem(index, 1, trackName);
    this->meshAnimationsList->SetItem(index, 2, framesText);        
    this->meshAnimationsList->SetItem(index, 3, lengthText);
    this->meshAnimationsList->SetItem(index, 4, sampling);    
}

void ObjectSettingsDialog_MeshAnimationsPage::SortMeshAnimationsList()
{
    static wxListCtrlCompare compareFunctions[] =
    {
        CompareAnimationNames,
        CompareAnimationTrackNames,
        CompareAnimationFrames,
        CompareAnimationLength,
        CompareAnimationSampling
    };

    this->meshAnimationsList->SortItems(compareFunctions[this->sortColumn], (long)this);    
}

int wxCALLBACK ObjectSettingsDialog_MeshAnimationsPage::CompareAnimationNames(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
    //ObjectSettingsDialog_MeshAnimationsPage* page = (ObjectSettingsDialog_MeshAnimationsPage*)sortData;
    auto& a1 = page->meshAnimations[item1];
    auto& a2 = page->meshAnimations[item2];

    auto value1 = a1.GetAnimationName();
    auto value2 = a2.GetAnimationName();

    int result = 0;
    if (value1 < value2)
        result = -1;
    else if (value1 > value2)
        result = 1;
    
    return result * page->invertSorting;
}

int wxCALLBACK ObjectSettingsDialog_MeshAnimationsPage::CompareAnimationTrackNames(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
    //ObjectSettingsDialog_MeshAnimationsPage* page = (ObjectSettingsDialog_MeshAnimationsPage*)sortData;
    auto& a1 = page->meshAnimations[item1];
    auto& a2 = page->meshAnimations[item2];

    auto value1 = page->GetAnimationTrackText(a1);
    auto value2 = page->GetAnimationTrackText(a2);

    int result = 0;
    if (value1 < value2)
        result = -1;
    else if (value1 > value2)
        result = 1;
    
    return result * page->invertSorting;
}

int wxCALLBACK ObjectSettingsDialog_MeshAnimationsPage::CompareAnimationFrames(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
    //ObjectSettingsDialog_MeshAnimationsPage* page = (ObjectSettingsDialog_MeshAnimationsPage*)sortData;
    auto& a1 = page->meshAnimations[item1];
    auto& a2 = page->meshAnimations[item2];

    auto value1 = a1.GetTimeInterval();
    auto value2 = a2.GetTimeInterval();

    int result = value1.CompareTo(value2);
    return result * page->invertSorting;
}

int wxCALLBACK ObjectSettingsDialog_MeshAnimationsPage::CompareAnimationLength(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
    //ObjectSettingsDialog_MeshAnimationsPage* page = (ObjectSettingsDialog_MeshAnimationsPage*)sortData;
    auto& a1 = page->meshAnimations[item1];
    auto& a2 = page->meshAnimations[item2];

    auto value1 = a1.GetTimeInterval().GetDuration().ToSecondsDouble() * a1.GetRealTimeScale();
    auto value2 = a2.GetTimeInterval().GetDuration().ToSecondsDouble() * a2.GetRealTimeScale();

    int result = 0;
    if (value1 < value2)
        result = -1;
    else if (value1 > value2)
        result = 1;
    
    return result * page->invertSorting;
}

int wxCALLBACK ObjectSettingsDialog_MeshAnimationsPage::CompareAnimationSampling(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
    //ObjectSettingsDialog_MeshAnimationsPage* page = (ObjectSettingsDialog_MeshAnimationsPage*)sortData;
    auto& a1 = page->meshAnimations[item1];
    auto& a2 = page->meshAnimations[item2];

    auto value1 = page->GetAnimationSamplingText(a1);
    auto value2 = page->GetAnimationSamplingText(a2);

    int result = 0;
    if (value1 < value2)
        result = -1;
    else if (value1 > value2)
        result = 1;
    
    return result * page->invertSorting;
}

wxString ObjectSettingsDialog_MeshAnimationsPage::GetAnimationTrackText(FinjinMeshAnimationSettingsAccessor& anim)
{
    auto trackName = anim.GetTrack();
    int selectedTrack = this->trackDetector.FindTrackIndex(trackName);
    if (selectedTrack >= 0)
        trackName = this->trackDetector.tracks[selectedTrack].displayName;
    return trackName;
}

wxString ObjectSettingsDialog_MeshAnimationsPage::GetAnimationSamplingText(FinjinMeshAnimationSettingsAccessor& anim)
{
    int selectedTypeIndex = this->meshAnimationTypeChoice->GetSelection();
    auto meshAnimationType = (MeshAnimationType*)this->meshAnimationTypeChoice->GetClientData(selectedTypeIndex);

    SampleType sampleType;
    double sampleInterval;
    anim.GetSampleTypeAndIntervalValues(sampleType, sampleInterval, this->objectSettings, this->sceneSettings, *meshAnimationType, this->trackDetector);
    return AnimationUtilities::FormatSampling(sampleType, sampleInterval);
}

void ObjectSettingsDialog_MeshAnimationsPage::OnMeshAnimationTypeChoice(wxCommandEvent& event)
{
    if (this->settingGuiData)
        return;

    GetGUIData(MESH_ANIMATION_TYPE_CHOICE);

    UpdateMeshAnimationTypeDefaultText();
    UpdateMeshAnimationListGUIData();    
}

void ObjectSettingsDialog_MeshAnimationsPage::OnExportSkeletonCheckBox(wxCommandEvent& event)
{
    UpdateExportSkeletonControls();
}

void ObjectSettingsDialog_MeshAnimationsPage::OnEmbedSkeletonInMeshCheckBox(wxCommandEvent& event)
{
    UpdateEmbedSkeletonControls();
}

void ObjectSettingsDialog_MeshAnimationsPage::OnReferencePoseChoice(wxCommandEvent& event)
{
    UpdateReferencePoseControls();
}

void ObjectSettingsDialog_MeshAnimationsPage::OnSkeletonAnimationSamplingTextChanged(wxCommandEvent& event)
{
    if (this->settingGuiData)
        return;

    GetGUIData(SKELETON_ANIMATION_SAMPLING_TEXT);

    UpdateMeshAnimationListGUIData();    
}

void ObjectSettingsDialog_MeshAnimationsPage::OnSkeletonAnimationSamplingChoice(wxCommandEvent& event)
{
    if (this->settingGuiData)
        return;

    GetGUIData(SKELETON_ANIMATION_SAMPLING_CHOICE);

    UpdateSkeletonAnimationSamplingTypeEdit();
    UpdateMeshAnimationListGUIData();    
}

void ObjectSettingsDialog_MeshAnimationsPage::OnVertexAnimationSamplingTextChanged(wxCommandEvent& event)
{
    if (this->settingGuiData)
        return;

    GetGUIData(VERTEX_ANIMATION_SAMPLING_TEXT);

    UpdateMeshAnimationListGUIData();    
}

void ObjectSettingsDialog_MeshAnimationsPage::OnVertexAnimationSamplingChoice(wxCommandEvent& event)
{
    if (this->settingGuiData)
        return;

    GetGUIData(VERTEX_ANIMATION_SAMPLING_CHOICE);

    UpdateVertexAnimationSamplingTypeEdit();
    UpdateMeshAnimationListGUIData();    
}

void ObjectSettingsDialog_MeshAnimationsPage::OnAnimatedRootChooseBoneButton(wxCommandEvent& event)
{
    ChooseObjectsPrompt choose(this, HowMany::CHOOSE_ONE, ObjectFilterType::ALL, &this->meshBones);
    if (choose.ShowPrompt())
    {
        this->animatedSkeletonRootObject = choose.GetObject();
        UpdateAnimatedSkeletonRootEdit();            
    }
}

void ObjectSettingsDialog_MeshAnimationsPage::OnAnimatedRootChooseObjectButton(wxCommandEvent& event)
{
    ChooseObjectsPrompt choose(this, HowMany::CHOOSE_ONE, ObjectFilterType::ALL);
    if (choose.ShowPrompt())
    {
        this->animatedSkeletonRootObject = choose.GetObject();
        UpdateAnimatedSkeletonRootEdit();            
    }
}

void ObjectSettingsDialog_MeshAnimationsPage::OnAddAnimationButton(wxCommandEvent& event)
{
    std::vector<wxString> animationNames;
    this->objectSettings.GetMeshAnimationNames(animationNames);

    //Get valid track locations
    int selectedTrack = (this->trackDetector.tracks.size() > 1) ? 1 : 0;

    SUSPEND_CUSTOM_CONTROLS;
    MeshAnimationSettingsDialog dialog(this, Strings::ADD_MESH_ANIMATION, this->meshBones, animationNames, this->trackDetector.tracks, selectedTrack);
    ModalDialogFix modalDialogFix(&dialog);
    if (dialog.ShowModal() == wxID_OK)
    {
        auto anim = this->objectSettings.AddNewMeshAnimation();
        dialog.settings.GetSettings(anim);
        
        this->meshAnimations.push_back(anim);
        UpdateMeshAnimationListGUIData((int)this->meshAnimations.size() - 1);
        UpdateMeshAnimationListButtons();
    }
}

void ObjectSettingsDialog_MeshAnimationsPage::OnEditAnimationButton(wxCommandEvent& event)
{
    OnEditAnimation();
}

void ObjectSettingsDialog_MeshAnimationsPage::OnCopyAnimationButton(wxCommandEvent& event)
{
    int selection = this->meshAnimationsList->GetSelection();
    if (selection >= 0)
    {
        auto anim = this->objectSettings.AddMeshAnimationCopy((int)this->meshAnimationsList->GetItemData(selection));
        this->meshAnimations.push_back(anim);
        UpdateMeshAnimationListGUIData();
    }
}

void ObjectSettingsDialog_MeshAnimationsPage::OnRemoveAnimationButton(wxCommandEvent& event)
{
    std::vector<int> selectedItems;
    if (this->meshAnimationsList->GetSelection(selectedItems))
    {
        std::vector<FinjinMeshAnimationSettingsAccessor> selectedAnimations;
        selectedAnimations.resize(selectedItems.size());
        for (int i = 0; i < selectedItems.size(); i++)
            selectedAnimations[i] = this->meshAnimations[this->meshAnimationsList->GetItemData(selectedItems[i])];
        
        for (int i = (int)selectedAnimations.size() - 1; i >= 0; i--)
        {
            //Remove animation from array            
            this->meshAnimations.erase(std::find(this->meshAnimations.begin(), this->meshAnimations.end(), selectedAnimations[i]));
            
            //Remove animation from object
            this->objectSettings.RemoveMeshAnimation(selectedAnimations[i]);
            
            //Remove animation from list
            this->meshAnimationsList->DeleteItem(selectedItems[i]);            
        }
        
        this->meshAnimationsList->SetSequentialItemData();
        
        UpdateMeshAnimationListButtons();

        if (this->meshAnimationsList->GetItemCount() > 0)
            this->meshAnimationsList->SetSelection(std::max(selectedItems[0]-1, 0));
    }    
}

void ObjectSettingsDialog_MeshAnimationsPage::OnSyncToBipedMixerButton(wxCommandEvent& event)
{
    if (this->mixerControl.SyncSettingsToMixer())
    {
        this->objectSettings.GetMeshAnimations(this->meshAnimations);
        UpdateMeshAnimationListGUIData();
    }
}

void ObjectSettingsDialog_MeshAnimationsPage::OnMeshAnimationListActivate(wxListEvent& event)
{
    OnEditAnimation();
}

void ObjectSettingsDialog_MeshAnimationsPage::OnMeshAnimationListColumnClick(wxListEvent& event)
{
    if (event.GetColumn() == this->sortColumn)
        this->invertSorting *= -1;
    else
        this->sortColumn = event.GetColumn();

    SortMeshAnimationsList();
}

void ObjectSettingsDialog_MeshAnimationsPage::OnMeshAnimationListSelectionChange(wxListEvent& event)
{
    UpdateMeshAnimationListButtons();
}
