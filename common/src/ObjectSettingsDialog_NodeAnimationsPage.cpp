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
#include "ObjectSettingsDialog_NodeAnimationsPage.hpp"
#include "Strings.hpp"
#include "StringUtilities.hpp"
#include "ApplicationControls.hpp"
#include "NodeAnimationSettingsDialog.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "AnimationUtilities.hpp"
#include "ModalDialogFix.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
const wxString ObjectSettingsDialog_NodeAnimationsPage::TITLE(wxT("Node Animations"));
static ObjectSettingsDialog_NodeAnimationsPage* page = nullptr;


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(ObjectSettingsDialog_NodeAnimationsPage, SettingsPage)
    EVT_TEXT(ObjectSettingsDialog_NodeAnimationsPage::ANIMATION_SAMPLING_TEXT, ObjectSettingsDialog_NodeAnimationsPage::OnAnimationSamplingTextChanged)
    EVT_CHOICE(ObjectSettingsDialog_NodeAnimationsPage::ANIMATION_SAMPLING_CHOICE, ObjectSettingsDialog_NodeAnimationsPage::OnAnimationSamplingChoice)
    EVT_BUTTON(ObjectSettingsDialog_NodeAnimationsPage::ADD_ANIMATION_BUTTON, ObjectSettingsDialog_NodeAnimationsPage::OnAddAnimationButton)
    EVT_BUTTON(ObjectSettingsDialog_NodeAnimationsPage::EDIT_ANIMATION_BUTTON, ObjectSettingsDialog_NodeAnimationsPage::OnEditAnimationButton)
    EVT_BUTTON(ObjectSettingsDialog_NodeAnimationsPage::COPY_ANIMATION_BUTTON, ObjectSettingsDialog_NodeAnimationsPage::OnCopyAnimationButton)
    EVT_BUTTON(ObjectSettingsDialog_NodeAnimationsPage::REMOVE_ANIMATION_BUTTON, ObjectSettingsDialog_NodeAnimationsPage::OnRemoveAnimationButton)
    EVT_LIST_ITEM_ACTIVATED(ObjectSettingsDialog_NodeAnimationsPage::NODE_ANIMATION_LIST, ObjectSettingsDialog_NodeAnimationsPage::OnNodeAnimationListActivate)    
    EVT_LIST_COL_CLICK(ObjectSettingsDialog_NodeAnimationsPage::NODE_ANIMATION_LIST, ObjectSettingsDialog_NodeAnimationsPage::OnNodeAnimationListColumnClick)    
    EVT_LIST_ITEM_SELECTED(ObjectSettingsDialog_NodeAnimationsPage::NODE_ANIMATION_LIST, ObjectSettingsDialog_NodeAnimationsPage::OnNodeAnimationListSelectionChange)    
    EVT_LIST_ITEM_DESELECTED(ObjectSettingsDialog_NodeAnimationsPage::NODE_ANIMATION_LIST, ObjectSettingsDialog_NodeAnimationsPage::OnNodeAnimationListSelectionChange)    
END_EVENT_TABLE()

ObjectSettingsDialog_NodeAnimationsPage::ObjectSettingsDialog_NodeAnimationsPage
    (
    wxWindow* parent, 
    ObjectAccessor object, 
    FinjinObjectSettingsAccessor objectSettings, 
    FinjinSceneSettingsAccessor sceneSettings, 
    wxWindowID id, 
    const wxPoint& pos, 
    const wxSize& size, 
    long style 
    ) : SettingsPage( parent, id, pos, size, style )
{
    page = this;

    this->settingGuiData = false;
    this->sortColumn = 0;
    this->invertSorting = 1;

    this->object = object;
    this->objectSettings = objectSettings;
    this->sceneSettings = sceneSettings;

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxStaticBoxSizer* nodeAnimationDefaultsSizer;
    nodeAnimationDefaultsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Settings") ), wxVERTICAL );
    
    wxFlexGridSizer* nodeAnimationsDefaultsGridSizer;
    nodeAnimationsDefaultsGridSizer = new wxFlexGridSizer( 1, 2, 0, 0 );
    nodeAnimationsDefaultsGridSizer->SetFlexibleDirection( wxBOTH );
    nodeAnimationsDefaultsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    defaultAnimationSamplingLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Animation Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    defaultAnimationSamplingLabel->Wrap( -1 );
    nodeAnimationsDefaultsGridSizer->Add( defaultAnimationSamplingLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
    
    wxBoxSizer* animationSamplingSizer;
    animationSamplingSizer = new wxBoxSizer( wxHORIZONTAL );
    
    defaultAnimationSamplingText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    animationSamplingSizer->Add( defaultAnimationSamplingText, 0, wxLEFT, 5 );
    
    wxArrayString defaultAnimationSamplingChoiceChoices;
    defaultAnimationSamplingChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, defaultAnimationSamplingChoiceChoices, 0 );
    defaultAnimationSamplingChoice->SetSelection( 0 );
    animationSamplingSizer->Add( defaultAnimationSamplingChoice, 1, wxRIGHT|wxLEFT, 5 );
    
    nodeAnimationsDefaultsGridSizer->Add( animationSamplingSizer, 1, wxEXPAND, 5 );
    
    nodeAnimationDefaultsSizer->Add( nodeAnimationsDefaultsGridSizer, 0, wxEXPAND, 5 );

    topSizer->Add( nodeAnimationDefaultsSizer, 0, wxEXPAND|wxTOP, 5 );
    
    wxStaticBoxSizer* nodeAnimationsSizer;
    nodeAnimationsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Node Animations") ), wxVERTICAL );
    
    wxBoxSizer* nodeAnimationsButtonsSizer;
    nodeAnimationsButtonsSizer = new wxBoxSizer( wxHORIZONTAL );
    
    addNodeAnimationButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxSize(65, -1), 0 );
    nodeAnimationsButtonsSizer->Add( addNodeAnimationButton, 0, wxLEFT, 5 );
    
    editNodeAnimationButton = new wxButton( this, wxID_ANY, wxT("Edit..."), wxDefaultPosition, wxSize(65, -1), 0 );
    nodeAnimationsButtonsSizer->Add( editNodeAnimationButton, 0, 0, 5 );

    copyNodeAnimationButton = new wxButton( this, wxID_ANY, wxT("Copy"), wxDefaultPosition, wxSize(65, -1), 0 );
    nodeAnimationsButtonsSizer->Add( copyNodeAnimationButton, 0, 0, 5 );

    removeNodeAnimationButton = new wxButton( this, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxSize(65, -1), 0 );
    nodeAnimationsButtonsSizer->Add( removeNodeAnimationButton, 0, 0, 5 );
    
    nodeAnimationsSizer->Add( nodeAnimationsButtonsSizer, 0, wxEXPAND, 5 );
    
    nodeAnimationsList = new ApplicationListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_HRULES|wxLC_VRULES );
    nodeAnimationsSizer->Add( nodeAnimationsList, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    topSizer->Add( nodeAnimationsSizer, 1, wxEXPAND|wxTOP, 5 );

    this->defaultAnimationSamplingText->SetId(ANIMATION_SAMPLING_TEXT);
    this->defaultAnimationSamplingChoice->SetId(ANIMATION_SAMPLING_CHOICE);
    this->addNodeAnimationButton->SetId(ADD_ANIMATION_BUTTON);
    this->editNodeAnimationButton->SetId(EDIT_ANIMATION_BUTTON);
    this->copyNodeAnimationButton->SetId(COPY_ANIMATION_BUTTON);
    this->removeNodeAnimationButton->SetId(REMOVE_ANIMATION_BUTTON);
    this->nodeAnimationsList->SetId(NODE_ANIMATION_LIST);
    
    SetSizer(topSizer);
    Layout();
}

ObjectSettingsDialog_NodeAnimationsPage::~ObjectSettingsDialog_NodeAnimationsPage()
{
}

bool ObjectSettingsDialog_NodeAnimationsPage::GetGUIData()
{
    return GetGUIData(ALL_IDS);
}

bool ObjectSettingsDialog_NodeAnimationsPage::GetGUIData(int ids)
{
    //Node animation sampling combo and text
    if (ids & (ANIMATION_SAMPLING_TEXT | ANIMATION_SAMPLING_CHOICE))
    {
        int selectedNodeAnimationSampleTypeIndex = this->defaultAnimationSamplingChoice->GetSelection();
        auto nodeAnimationSampleType = (SampleType)*(int*)this->defaultAnimationSamplingChoice->GetClientData(selectedNodeAnimationSampleTypeIndex);    
        auto nodeAnimationSampling = StringUtilities::ToDouble(this->defaultAnimationSamplingText->GetValue());
        if (nodeAnimationSampling < 0)
            nodeAnimationSampling = 0;
        this->objectSettings.SetNodeAnimationSampling(nodeAnimationSampling, nodeAnimationSampleType, nodeAnimationSampleType != SampleType::USE_PARENT);
    }
    
    return true;
}

bool ObjectSettingsDialog_NodeAnimationsPage::SetGUIData()
{
    this->settingGuiData = true;

    //Node animation sampling value
    this->defaultAnimationSamplingText->SetValue(this->objectSettings.GetNodeAnimationSampleValueText());
    
    //Node animation sampling combo
    this->defaultAnimationSamplingChoice->Freeze();
    for (int i = 0; i < Strings::OBJECT_SAMPLE_TYPE_COUNT; i++)
    {
        this->defaultAnimationSamplingChoice->Append(Strings::OBJECT_SAMPLE_TYPES_TEXT[i]);
        this->defaultAnimationSamplingChoice->SetClientData(i, (void*)&Strings::OBJECT_SAMPLE_TYPES_VALUE[i]);
        if (this->objectSettings.GetNodeAnimationSampleType() == Strings::OBJECT_SAMPLE_TYPES_VALUE[i])
            this->defaultAnimationSamplingChoice->SetSelection(i);
    }
    this->defaultAnimationSamplingChoice->Thaw();
    if (this->objectSettings.GetNodeAnimationSampleInterval() == 0 ||
        this->defaultAnimationSamplingChoice->GetSelection() < 0)
    {
        this->defaultAnimationSamplingChoice->SetSelection(0);
    }
    
    //Node animations
    this->nodeAnimationsList->InsertColumn(0, Strings::NAME, wxLIST_FORMAT_LEFT, 175);
    this->nodeAnimationsList->InsertColumn(1, TimeAccessor::GetUIUnit(), wxLIST_FORMAT_LEFT, 65);
    this->nodeAnimationsList->InsertColumn(2, Strings::LENGTH, wxLIST_FORMAT_LEFT, 55);
    this->nodeAnimationsList->InsertColumn(3, Strings::SAMPLE_INTERVAL_RATE, wxLIST_FORMAT_LEFT, 115);    
    this->nodeAnimationsList->InsertColumn(4, Strings::LOOPED, wxLIST_FORMAT_LEFT, 60);
    this->objectSettings.GetNodeAnimations(this->nodeAnimations);
    UpdateNodeAnimationListGUIData();

    //Final updates
    UpdateAnimationSamplingTypeEdit();    
    UpdateNodeAnimationListButtons();

    this->settingGuiData = false;
    
    return true;
}

void ObjectSettingsDialog_NodeAnimationsPage::UpdateAnimationSamplingTypeEdit()
{
    int selectedIndex = this->defaultAnimationSamplingChoice->GetSelection();
    bool show = selectedIndex > 0;
    this->defaultAnimationSamplingText->Show(show);
    Layout();
}

void ObjectSettingsDialog_NodeAnimationsPage::UpdateNodeAnimationListButtons()
{
    int selectedItemCount = this->nodeAnimationsList->GetSelectedItemCount();
    this->editNodeAnimationButton->Enable(selectedItemCount == 1);
    this->copyNodeAnimationButton->Enable(selectedItemCount == 1);
    this->removeNodeAnimationButton->Enable(selectedItemCount > 0);
}

void ObjectSettingsDialog_NodeAnimationsPage::UpdateNodeAnimationListGUIData(int selectedAnimationIndex)
{
    if (selectedAnimationIndex < 0)
    {
        int selectedIndex = this->nodeAnimationsList->GetSelection();
        if (selectedIndex >= 0)
            selectedAnimationIndex = (int)this->nodeAnimationsList->GetItemData(selectedIndex);
    }
    
    while (this->nodeAnimationsList->GetItemCount() < (int)this->nodeAnimations.size())
        this->nodeAnimationsList->InsertItem(this->nodeAnimationsList->GetItemCount(), wxEmptyString);
    while (this->nodeAnimationsList->GetItemCount() > (int)this->nodeAnimations.size())
        this->nodeAnimationsList->DeleteItem(this->nodeAnimationsList->GetItemCount() - 1);
    
    this->nodeAnimationsList->SetSequentialItemData();

    for (int i = 0; i < (int)this->nodeAnimations.size(); i++)
        UpdateNodeAnimationGUIData(i);

    if (selectedAnimationIndex >= 0)
        this->nodeAnimationsList->SetSelection(selectedAnimationIndex);
    
    SortNodeAnimationsList();
}

void ObjectSettingsDialog_NodeAnimationsPage::UpdateNodeAnimationGUIData(int index)
{
    auto& anim = this->nodeAnimations[this->nodeAnimationsList->GetItemData(index)];
    
    auto interval = anim.GetTimeInterval();    
    auto framesText = interval.ToString();

    auto length = interval.GetDuration().ToSecondsDouble() * anim.GetRealTimeScale();
    auto lengthText = wxString::Format(wxT("%.2fs"), length);

    auto sampling = GetAnimationSamplingText(anim);
    
    auto looped = GetAnimationLoopedText(anim);
        
    this->nodeAnimationsList->SetItem(index, 0, anim.GetAnimationName());
    this->nodeAnimationsList->SetItem(index, 1, framesText);        
    this->nodeAnimationsList->SetItem(index, 2, lengthText);
    this->nodeAnimationsList->SetItem(index, 3, sampling);
    this->nodeAnimationsList->SetItem(index, 4, looped);    
}

void ObjectSettingsDialog_NodeAnimationsPage::SortNodeAnimationsList()
{
    static wxListCtrlCompare compareFunctions[] =
    {
        CompareAnimationNames,
        CompareAnimationFrames,
        CompareAnimationLength,
        CompareAnimationSampling,
        CompareAnimationLooped,
    };

    this->nodeAnimationsList->SortItems(compareFunctions[this->sortColumn], (long)this);    
}

int wxCALLBACK ObjectSettingsDialog_NodeAnimationsPage::CompareAnimationNames(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
    //ObjectSettingsDialog_NodeAnimationsPage* page = (ObjectSettingsDialog_NodeAnimationsPage*)sortData;
    auto& a1 = page->nodeAnimations[item1];
    auto& a2 = page->nodeAnimations[item2];

    auto value1 = a1.GetAnimationName();
    auto value2 = a2.GetAnimationName();

    int result = 0;
    if (value1 < value2)
        result = -1;
    else if (value1 > value2)
        result = 1;
    
    return result * page->invertSorting;
}

int wxCALLBACK ObjectSettingsDialog_NodeAnimationsPage::CompareAnimationFrames(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
    //ObjectSettingsDialog_NodeAnimationsPage* page = (ObjectSettingsDialog_NodeAnimationsPage*)sortData;
    auto& a1 = page->nodeAnimations[item1];
    auto& a2 = page->nodeAnimations[item2];

    auto value1 = a1.GetTimeInterval();
    auto value2 = a2.GetTimeInterval();

    int result = value1.CompareTo(value2);
    return result * page->invertSorting;
}

int wxCALLBACK ObjectSettingsDialog_NodeAnimationsPage::CompareAnimationLength(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
    //ObjectSettingsDialog_NodeAnimationsPage* page = (ObjectSettingsDialog_NodeAnimationsPage*)sortData;
    auto& a1 = page->nodeAnimations[item1];
    auto& a2 = page->nodeAnimations[item2];

    auto value1 = a1.GetTimeInterval().GetDuration().ToSecondsDouble() * a1.GetRealTimeScale();
    auto value2 = a2.GetTimeInterval().GetDuration().ToSecondsDouble() * a2.GetRealTimeScale();

    int result = 0;
    if (value1 < value2)
        result = -1;
    else if (value1 > value2)
        result = 1;
    
    return result * page->invertSorting;
}

int wxCALLBACK ObjectSettingsDialog_NodeAnimationsPage::CompareAnimationSampling(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
    //ObjectSettingsDialog_NodeAnimationsPage* page = (ObjectSettingsDialog_NodeAnimationsPage*)sortData;
    auto& a1 = page->nodeAnimations[item1];
    auto& a2 = page->nodeAnimations[item2];

    auto value1 = page->GetAnimationSamplingText(a1);
    auto value2 = page->GetAnimationSamplingText(a2);

    int result = 0;
    if (value1 < value2)
        result = -1;
    else if (value1 > value2)
        result = 1;
    
    return result * page->invertSorting;
}

int wxCALLBACK ObjectSettingsDialog_NodeAnimationsPage::CompareAnimationLooped(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
    //ObjectSettingsDialog_NodeAnimationsPage* page = (ObjectSettingsDialog_NodeAnimationsPage*)sortData;
    auto& a1 = page->nodeAnimations[item1];
    auto& a2 = page->nodeAnimations[item2];

    auto value1 = page->GetAnimationLoopedText(a1);
    auto value2 = page->GetAnimationLoopedText(a2);

    int result = 0;
    if (value1 < value2)
        result = -1;
    else if (value1 > value2)
        result = 1;
    
    return result * page->invertSorting;
}

wxString ObjectSettingsDialog_NodeAnimationsPage::GetAnimationSamplingText(FinjinNodeAnimationSettingsAccessor& anim)
{
    SampleType sampleType;
    double sampleInterval;
    anim.GetSampleTypeAndIntervalValues(sampleType, sampleInterval, this->objectSettings, this->sceneSettings);
    return AnimationUtilities::FormatSampling(sampleType, sampleInterval);
}

wxString ObjectSettingsDialog_NodeAnimationsPage::GetAnimationLoopedText(FinjinNodeAnimationSettingsAccessor& anim)
{
    return anim.GetLooped() ? Strings::YES : Strings::NO;
}

void ObjectSettingsDialog_NodeAnimationsPage::OnEditAnimation()
{
    int selection = this->nodeAnimationsList->GetSelection();
    if (selection >= 0)
    {
        auto anim = this->nodeAnimations[this->nodeAnimationsList->GetItemData(selection)];
        NodeAnimationSettingsDialog::Settings settings(anim);
        
        //Get all the current animation names and remove the one being edited
        std::vector<wxString> animationNames;
        this->objectSettings.GetNodeAnimationNames(animationNames);
        StringUtilities::RemoveName(animationNames, settings.animationName);

        SUSPEND_CUSTOM_CONTROLS;
        NodeAnimationSettingsDialog dialog(this, Strings::EDIT_NODE_ANIMATION, animationNames, settings);
        ModalDialogFix modalDialogFix(&dialog);
        if (dialog.ShowModal() == wxID_OK)
        {
            dialog.settings.GetSettings(anim);
            UpdateNodeAnimationListGUIData((int)this->nodeAnimationsList->GetItemData(selection));
        }
    }
}

void ObjectSettingsDialog_NodeAnimationsPage::OnAnimationSamplingTextChanged(wxCommandEvent& event)
{
    if (this->settingGuiData)
        return;

    GetGUIData(ANIMATION_SAMPLING_TEXT);

    UpdateNodeAnimationListGUIData();
}

void ObjectSettingsDialog_NodeAnimationsPage::OnAnimationSamplingChoice(wxCommandEvent& event)
{
    if (this->settingGuiData)
        return;

    GetGUIData(ANIMATION_SAMPLING_CHOICE);

    UpdateAnimationSamplingTypeEdit();
    UpdateNodeAnimationListGUIData();
}

void ObjectSettingsDialog_NodeAnimationsPage::OnAddAnimationButton(wxCommandEvent& event)
{
    std::vector<wxString> animationNames;
    this->objectSettings.GetNodeAnimationNames(animationNames);

    SUSPEND_CUSTOM_CONTROLS;
    NodeAnimationSettingsDialog dialog(this, Strings::ADD_NODE_ANIMATION, animationNames);
    ModalDialogFix modalDialogFix(&dialog);
    if (dialog.ShowModal() == wxID_OK)
    {
        auto anim = this->objectSettings.AddNewNodeAnimation();
        dialog.settings.GetSettings(anim);
        
        this->nodeAnimations.push_back(anim);
        UpdateNodeAnimationListGUIData((int)this->nodeAnimations.size() - 1);
        UpdateNodeAnimationListButtons();
    }
}

void ObjectSettingsDialog_NodeAnimationsPage::OnEditAnimationButton(wxCommandEvent& event)
{
    OnEditAnimation();
}

void ObjectSettingsDialog_NodeAnimationsPage::OnCopyAnimationButton(wxCommandEvent& event)
{
    int selection = this->nodeAnimationsList->GetSelection();
    if (selection >= 0)
    {
        auto anim = this->objectSettings.AddNodeAnimationCopy((int)this->nodeAnimationsList->GetItemData(selection));
        this->nodeAnimations.push_back(anim);
        UpdateNodeAnimationListGUIData();
    }
}

void ObjectSettingsDialog_NodeAnimationsPage::OnRemoveAnimationButton(wxCommandEvent& event)
{
    std::vector<int> selectedItems;
    if (this->nodeAnimationsList->GetSelection(selectedItems))
    {
        std::vector<FinjinNodeAnimationSettingsAccessor> selectedAnimations;
        selectedAnimations.resize(selectedItems.size());
        for (int i = 0; i < selectedItems.size(); i++)
            selectedAnimations[i] = this->nodeAnimations[this->nodeAnimationsList->GetItemData(selectedItems[i])];
        
        for (int i = (int)selectedAnimations.size() - 1; i >= 0; i--)
        {
            //Remove animation from array            
            this->nodeAnimations.erase(std::find(this->nodeAnimations.begin(), this->nodeAnimations.end(), selectedAnimations[i]));
            
            //Remove animation from object
            this->objectSettings.RemoveNodeAnimation(selectedAnimations[i]);
            
            //Remove animation from list
            this->nodeAnimationsList->DeleteItem(selectedItems[i]);            
        }
        
        this->nodeAnimationsList->SetSequentialItemData();

        UpdateNodeAnimationListButtons();

        if (this->nodeAnimationsList->GetItemCount() > 0)
            this->nodeAnimationsList->SetSelection(std::max(selectedItems[0]-1, 0));
    }    
}

void ObjectSettingsDialog_NodeAnimationsPage::OnNodeAnimationListActivate(wxListEvent& event)
{
    OnEditAnimation();
}

void ObjectSettingsDialog_NodeAnimationsPage::OnNodeAnimationListColumnClick(wxListEvent& event)
{
    if (event.GetColumn() == this->sortColumn)
        this->invertSorting *= -1;
    else
        this->sortColumn = event.GetColumn();

    SortNodeAnimationsList();
}

void ObjectSettingsDialog_NodeAnimationsPage::OnNodeAnimationListSelectionChange(wxListEvent& event)
{
    UpdateNodeAnimationListButtons();
}
