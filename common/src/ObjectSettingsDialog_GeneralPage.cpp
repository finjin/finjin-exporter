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
#include "ObjectSettingsDialog_GeneralPage.hpp"
#include "Strings.hpp"
#include "StringSplitter.hpp"
#include "StringUtilities.hpp"
#include "ApplicationControls.hpp"
#include "PropertyValues.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "ObjectSettingsDialog.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "ApplicationAccessor.hpp"
#include "ModalDialogFix.hpp"
#include "ChooseObjectsPrompt.hpp"
#include "FinjinGlobalSettings.hpp"
#include "FlagDialog.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
const wxString ObjectSettingsDialog_GeneralPage::TITLE(wxT("General"));


//Globals----------------------------------------------------------------------
static const wxChar* VISIBILITIES_TEXT[] = {wxT("Default"), wxT("Visible"), wxT("Hidden"), wxT("Tree Visible"), wxT("Tree Hidden")};
static const wxChar* VISIBILITIES_VALUE[] = {wxT(""), wxT("visible"), wxT("hidden"), wxT("tree-visible"), wxT("tree-hidden")};
static const int VISIBILITIES_COUNT = FINJIN_COUNT_OF(VISIBILITIES_VALUE);

static const wxChar* SORT_CHILDREN_TEXT[] = {wxT("None"), wxT("Name"), wxT("Child Order")};
static const wxChar* SORT_CHILDREN_VALUE[] = {wxT(""), wxT("name"), wxT("child-order")};
static const int SORT_CHILDREN_COUNT = FINJIN_COUNT_OF(SORT_CHILDREN_VALUE);


//Local functions--------------------------------------------------------------
static bool IsObjectVisibility(const wxString& visibility)
{
    return 
        visibility == PropertyValues::Visibility::VISIBLE || 
        visibility == PropertyValues::Visibility::HIDDEN;
}


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(ObjectSettingsDialog_GeneralPage, SettingsPage)
    EVT_CHOICE(ObjectSettingsDialog_GeneralPage::TYPE_CHOICE, ObjectSettingsDialog_GeneralPage::OnTypeChoice)    
    EVT_CHOICE(ObjectSettingsDialog_GeneralPage::VISIBILITY_CHOICE, ObjectSettingsDialog_GeneralPage::OnVisibilityChoice)    
    EVT_BUTTON(ObjectSettingsDialog_GeneralPage::EXPORTED_PARENT_BUTTON, ObjectSettingsDialog_GeneralPage::OnExportedParentButton)
    EVT_CHECKBOX(ObjectSettingsDialog_GeneralPage::USE_ROOT_AS_EXPORTED_PARENT_CHECKBOX, ObjectSettingsDialog_GeneralPage::UseRootAsExportedParent)
    EVT_BUTTON(ObjectSettingsDialog_GeneralPage::ADD_FLAG_BUTTON, ObjectSettingsDialog_GeneralPage::OnAddFlagButton)    
END_EVENT_TABLE()

ObjectSettingsDialog_GeneralPage::ObjectSettingsDialog_GeneralPage(ObjectSettingsDialog* dialog, wxWindow* parent, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    this->object = object;
    this->objectSettings = objectSettings;
    this->dialog = dialog;

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* typeSizer;
    typeSizer = new wxBoxSizer( wxHORIZONTAL );
    
    typeLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0 );
    typeLabel->Wrap( -1 );
    typeSizer->Add( typeLabel, 0, wxALL, 5 );
    
    wxArrayString typeChoiceChoices;
    typeChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, typeChoiceChoices, 0 );
    typeChoice->SetSelection( 0 );
    typeSizer->Add( typeChoice, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    isPrefabCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export as Prefab"), wxDefaultPosition, wxDefaultSize, 0 );    
    typeSizer->Add( isPrefabCheckbox, 0, wxALL, 5 );

    isPrefabInstanceCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Also Export as Prefab Instance"), wxDefaultPosition, wxDefaultSize, 0 );
	typeSizer->Add( isPrefabInstanceCheckbox, 0, wxALL, 5 );
    isPrefabInstanceCheckbox->Show(false); //TODO: Remove this when I figure out how to fully support this setting

    defaultTypeLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    defaultTypeLabel->Wrap( -1 );
    typeSizer->Add( defaultTypeLabel, 0, wxALL, 5 );
    
    topSizer->Add( typeSizer, 0, wxEXPAND|wxTOP, 5 );
    
    wxStaticBoxSizer* renderingSizer;
    renderingSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Rendering") ), wxVERTICAL );
    
    wxFlexGridSizer* renderingGridSizer;
    renderingGridSizer = new wxFlexGridSizer( 3, 2, 0, 0 );
    renderingGridSizer->SetFlexibleDirection( wxBOTH );
    renderingGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    visibilityLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Visibility"), wxDefaultPosition, wxDefaultSize, 0 );
    visibilityLabel->Wrap( -1 );
    renderingGridSizer->Add( visibilityLabel, 0, wxALL, 5 );
    
    wxBoxSizer* visibilitySizer;
    visibilitySizer = new wxBoxSizer( wxHORIZONTAL );
    
    wxArrayString visibilityChoiceChoices;
    visibilityChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, visibilityChoiceChoices, 0 );
    visibilityChoice->SetSelection( 0 );
    visibilitySizer->Add( visibilityChoice, 0, wxRIGHT|wxLEFT, 5 );
    
    visibilityAffectObjectOnlyCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Affect Object Only"), wxDefaultPosition, wxDefaultSize, 0 );
    
    visibilitySizer->Add( visibilityAffectObjectOnlyCheckbox, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
    
    renderingGridSizer->Add( visibilitySizer, 1, wxEXPAND, 5 );
    
    renderQueueLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Render Queue"), wxDefaultPosition, wxDefaultSize, 0 );
    renderQueueLabel->Wrap( -1 );
    renderingGridSizer->Add( renderQueueLabel, 0, wxALL, 5 );
    
    wxBoxSizer* renderQueueSizer;
    renderQueueSizer = new wxBoxSizer( wxHORIZONTAL );
    
    renderQueueText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    renderQueueText->SetMaxLength( 0 ); 
    renderQueueSizer->Add( renderQueueText, 0, wxALL, 5 );
    
    renderQueuePriorityLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Priority"), wxDefaultPosition, wxDefaultSize, 0 );
    renderQueuePriorityLabel->Wrap( -1 );
    renderQueueSizer->Add( renderQueuePriorityLabel, 0, wxALL, 5 );
    
    renderQueuePrioritySpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(50, -1), 0, 0, 65535, SpinnerControl::INT_UNIT);
    renderQueueSizer->Add( renderQueuePrioritySpinner, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
    
    renderingGridSizer->Add( renderQueueSizer, 1, wxEXPAND, 5 );
    
    renderDistanceLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Render Distance"), wxDefaultPosition, wxDefaultSize, 0 );
    renderDistanceLabel->Wrap( -1 );
    renderingGridSizer->Add( renderDistanceLabel, 0, wxALL, 5 );
    
    wxBoxSizer* renderDistanceSizer;
    renderDistanceSizer = new wxBoxSizer( wxHORIZONTAL );
    
    renderDistanceSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(90, -1), 0, 0, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE_RANGE, SpinnerControl::WORLD_UNIT, 2);
    renderDistanceSizer->Add( renderDistanceSpinner, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
    
    renderingGridSizer->Add( renderDistanceSizer, 1, wxEXPAND, 5 );
    
    renderingSizer->Add( renderingGridSizer, 0, wxEXPAND, 5 );
    
    topSizer->Add( renderingSizer, 0, wxEXPAND|wxTOP, 5 );
    
    wxStaticBoxSizer* hierarchySizer;
    hierarchySizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Hierarchy") ), wxVERTICAL );
    
    wxBoxSizer* exportedParentSizer;
    exportedParentSizer = new wxBoxSizer( wxHORIZONTAL );
    
    exportedParentLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Exported Parent"), wxDefaultPosition, wxDefaultSize, 0 );
    exportedParentLabel->Wrap( -1 );
    exportedParentSizer->Add( exportedParentLabel, 0, wxALL, 5 );
    
    exportedParentText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 180,-1 ), wxTE_READONLY );
    exportedParentSizer->Add( exportedParentText, 0, wxLEFT, 5 );
    
    exportedParentButton = new wxButton( this, wxID_ANY, wxT("Choose..."), wxDefaultPosition, wxDefaultSize, 0 );
    exportedParentSizer->Add( exportedParentButton, 0, wxRIGHT|wxLEFT, 5 );

    useRootAsExportedParentCheckbox = new wxCheckBox( this, wxID_ANY, wxT("No Parent"), wxDefaultPosition, wxDefaultSize, 0 );    
    exportedParentSizer->Add( useRootAsExportedParentCheckbox, 0, wxALL, 5 );
    
    hierarchySizer->Add( exportedParentSizer, 1, wxEXPAND, 5 );

    isAttachedObjectCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Attach Object to Parent Node"), wxDefaultPosition, wxDefaultSize, 0 );
    
    hierarchySizer->Add( isAttachedObjectCheckbox, 0, wxALL, 5 );

    wxBoxSizer* childrenSizer;
    childrenSizer = new wxBoxSizer( wxHORIZONTAL );
    
    ignoreChildrenCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Ignore Children"), wxDefaultPosition, wxDefaultSize, 0 );
    
    childrenSizer->Add( ignoreChildrenCheckbox, 0, wxALL, 5 );
    
    sortChildrenLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Sort Children By"), wxDefaultPosition, wxDefaultSize, 0 );
    sortChildrenLabel->Wrap( -1 );
    childrenSizer->Add( sortChildrenLabel, 0, wxALL, 5 );
    
    wxArrayString sortChildrenChoiceChoices;
    sortChildrenChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, sortChildrenChoiceChoices, 0 );
    sortChildrenChoice->SetSelection( 0 );
    childrenSizer->Add( sortChildrenChoice, 0, wxRIGHT, 5 );
    
    childOrderLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Order"), wxDefaultPosition, wxDefaultSize, 0 );
    childOrderLabel->Wrap( -1 );
    childrenSizer->Add( childOrderLabel, 0, wxALL, 5 );
    
    childOrderSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(50, -1), 0, 0, 255, SpinnerControl::INT_UNIT);
    childrenSizer->Add( childOrderSpinner, 0, wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );
    
    hierarchySizer->Add( childrenSizer, 1, wxEXPAND, 5 );
    
    topSizer->Add( hierarchySizer, 0, wxEXPAND|wxTOP, 5 );
    
    noteTrackNamesSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Note Track Names (One name per line, per note track)") ), wxVERTICAL );
    
    noteTrackNamesText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,80 ), wxTE_DONTWRAP|wxTE_MULTILINE );
    noteTrackNamesSizer->Add( noteTrackNamesText, 1, wxALL|wxEXPAND, 5 );
    
    topSizer->Add( noteTrackNamesSizer, 0, wxEXPAND|wxTOP, 5 );
    
    wxBoxSizer* flagsSizer;
    flagsSizer = new wxBoxSizer( wxHORIZONTAL );
    
    wxStaticBoxSizer* queryFlagsSizer;
    queryFlagsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Flags") ), wxVERTICAL );
    
    wxBoxSizer* queryFlagsEnableSizer;
    queryFlagsEnableSizer = new wxBoxSizer( wxHORIZONTAL );
    
    addFlagButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxDefaultSize, 0 );
    queryFlagsEnableSizer->Add( addFlagButton, 0, wxLEFT|wxRIGHT, 5 );
    
    
    queryFlagsSizer->Add( queryFlagsEnableSizer, 0, wxEXPAND, 5 );
    
    flagsText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_DONTWRAP|wxTE_MULTILINE );
    flagsText->SetMaxLength( 0 ); 
    flagsText->SetMinSize( wxSize( -1,60 ) );
    
    queryFlagsSizer->Add( flagsText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    
    flagsSizer->Add( queryFlagsSizer, 1, wxEXPAND|wxRIGHT, 5 );
    
    topSizer->Add( flagsSizer, 1, wxEXPAND|wxTOP, 5 );

    this->typeChoice->SetId(TYPE_CHOICE);
    this->visibilityChoice->SetId(VISIBILITY_CHOICE);
    this->exportedParentButton->SetId(EXPORTED_PARENT_BUTTON);
    this->useRootAsExportedParentCheckbox->SetId(USE_ROOT_AS_EXPORTED_PARENT_CHECKBOX);
    this->addFlagButton->SetId(ADD_FLAG_BUTTON);
    
    SetSizer(topSizer);
    Layout();
}

ObjectSettingsDialog_GeneralPage::~ObjectSettingsDialog_GeneralPage()
{
}

bool ObjectSettingsDialog_GeneralPage::GetGUIData()
{
    //Type combo    
    int selectedTypeIndex = this->typeChoice->GetCurrentSelection();
    DetectableObjectType* type = (DetectableObjectType*)this->typeChoice->GetClientData(selectedTypeIndex);
    wxString typeName;
    if (type != this->dialog->defaultObjectType.selectedType)        
        typeName = type->GetDescriptor().GetTypeName();
    this->objectSettings.SetType(typeName);

    //Visibility
    int selectedVisibilityIndex = this->visibilityChoice->GetSelection();
    wxString visibility = (wxChar*)this->visibilityChoice->GetClientData(selectedVisibilityIndex);
    this->objectSettings.SetVisibility(visibility);

    //Visibility affects object only
    this->objectSettings.SetVisibilityAffectObjectOnly(this->visibilityAffectObjectOnlyCheckbox->GetValue() && IsObjectVisibility(visibility));
        
    //Render queue
    this->objectSettings.SetRenderQueueName(this->renderQueueText->GetValue());
    
    //Render queue priority
    this->objectSettings.SetRenderPriority(this->renderQueuePrioritySpinner->GetIntValue());

    //Rendering distance
    this->objectSettings.SetRenderingDistance(this->renderDistanceSpinner->GetValue());
    
    //Exported parent
    this->objectSettings.SetExportedParent(this->exportedParentObject);
    this->objectSettings.SetUseRootAsExportedParent(this->useRootAsExportedParentCheckbox->GetValue());

    //Attached object
    this->objectSettings.SetAttachedObject(this->isAttachedObjectCheckbox->GetValue());

    //Ignore children
    this->objectSettings.SetIgnoreChildren(this->ignoreChildrenCheckbox->GetValue());

    //Sort children
    int selectedSortChildrenIndex = this->sortChildrenChoice->GetSelection();
    this->objectSettings.SetChildSorting((wxChar*)this->sortChildrenChoice->GetClientData(selectedSortChildrenIndex));
    
    //Child order
    this->objectSettings.SetChildOrder(this->childOrderSpinner->GetIntValue());
    
    //Is prefab    
    this->objectSettings.SetIsPrefab(this->isPrefabCheckbox->GetValue());

    //Is prefab instance
    this->objectSettings.SetIsPrefabInstance(this->isPrefabInstanceCheckbox->GetValue());

    //Note track names
    wxString noteTrackNamesText = this->noteTrackNamesText->GetValue();
    StringSplitter noteTrackNames(noteTrackNamesText, wxT('\n'), StringSplitterFlags::TRIM | StringSplitterFlags::IGNORE_EMPTY);
    this->objectSettings.ClearNoteTrackNames();
    for (size_t i = 0; i < noteTrackNames.size(); i++)
        this->objectSettings.AddNoteTrackName(noteTrackNames[i]);
    
    //Flags
    this->objectSettings.ClearFlags();    

    wxString flagsText = this->flagsText->GetValue();
    StringSplitter flags(flagsText, wxT('\n'), StringSplitterFlags::TRIM | StringSplitterFlags::IGNORE_EMPTY);

    std::set<wxString> flagNames;
    for (size_t i = 0; i < flags.size(); i++)
    {
        if (!StringUtilities::IsInteger(flags[i]))
        {
            //Make sure name is unique
            if (flagNames.find(flags[i]) == flagNames.end())
            {
                flagNames.insert(flags[i]);
                this->objectSettings.AddFlag(flags[i]);
            }
        }
    }

    return true;
}

bool ObjectSettingsDialog_GeneralPage::SetGUIData()
{
    this->noteTrackNamesSizer->Show(ApplicationAccessor::SupportsNoteTracks());

    //Type    
    this->typeChoice->Freeze();
    this->typeChoice->Append(Strings::DEFAULT);
    this->typeChoice->SetClientData(0, this->dialog->defaultObjectType.selectedType);    
    for (int i = 0; i < (int)this->dialog->objectType.detectedTypes.size(); i++)
    {
        int itemIndex = this->typeChoice->Append(this->dialog->objectType.detectedTypes[i].type->GetDescriptor().GetDisplayName());
        this->typeChoice->SetClientData(itemIndex, this->dialog->objectType.detectedTypes[i].type);

        if (this->dialog->objectType.selectedType == this->dialog->objectType.detectedTypes[i].type)
            this->typeChoice->SetSelection(itemIndex);
    }
    this->typeChoice->Thaw();
    if (this->typeChoice->GetSelection() < 0)
        this->typeChoice->SetSelection(0);

    //Visibility
    this->visibilityChoice->Freeze();
    for (int i = 0; i < VISIBILITIES_COUNT; i++)
    {
        this->visibilityChoice->Append(VISIBILITIES_TEXT[i]);
        this->visibilityChoice->SetClientData(i, (wxChar*)VISIBILITIES_VALUE[i]);
        if (this->objectSettings.GetVisibility() == VISIBILITIES_VALUE[i])
            this->visibilityChoice->SetSelection(i);
    }
    this->visibilityChoice->Thaw();
    if (this->visibilityChoice->GetSelection() < 0)
        this->visibilityChoice->SetSelection(0);

    //Visibility affects object only
    this->visibilityAffectObjectOnlyCheckbox->SetValue(this->objectSettings.GetVisibilityAffectObjectOnly());

    //Render queue
    this->renderQueueText->SetValue(this->objectSettings.GetRenderQueueName());
    
    //Render queue priority
    this->renderQueuePrioritySpinner->SetValue(this->objectSettings.GetRenderPriority());
    
    //Rendering distance
    this->renderDistanceSpinner->SetValue(this->objectSettings.GetRenderingDistance());
       
    //Exported parent
    this->exportedParentObject = this->objectSettings.GetExportedParent();
    this->exportedParentText->SetValue(this->exportedParentObject.GetFullName());
    this->useRootAsExportedParentCheckbox->SetValue(this->objectSettings.GetUseRootAsExportedParent());

    //Attached object
    this->isAttachedObjectCheckbox->SetValue(this->objectSettings.GetAttachedObject());

    //Ignore children
    this->ignoreChildrenCheckbox->SetValue(this->objectSettings.GetIgnoreChildren());

    //Sort children
    this->sortChildrenChoice->Freeze();
    for (int i = 0; i < SORT_CHILDREN_COUNT; i++)
    {
        this->sortChildrenChoice->Append(SORT_CHILDREN_TEXT[i]);
        this->sortChildrenChoice->SetClientData(i, (wxChar*)SORT_CHILDREN_VALUE[i]);
        if (this->objectSettings.GetChildSorting() == SORT_CHILDREN_VALUE[i])
            this->sortChildrenChoice->SetSelection(i);        
    }
    this->sortChildrenChoice->Thaw();
    if (this->sortChildrenChoice->GetSelection() < 0)
        this->sortChildrenChoice->SetSelection(0);

    //Child order
    this->childOrderSpinner->SetValue(this->objectSettings.GetChildOrder());
    
    //Is prefab
    this->isPrefabCheckbox->SetValue(this->objectSettings.GetIsPrefab());

    //Is prefab instance
    this->isPrefabInstanceCheckbox->SetValue(this->objectSettings.GetIsPrefabInstance());

    //Note track names
    int noteTrackNameCount = this->objectSettings.GetNoteTrackNameCount();
    wxString noteTracksText;
    for (int i = 0; i < noteTrackNameCount; i++)
    {
        noteTracksText += this->objectSettings.GetNoteTrackName(i);
        noteTracksText += wxT("\n");
    }
    this->noteTrackNamesText->SetValue(noteTracksText);

    //Flags
    this->addFlagButton->Enable(FinjinGlobalSettings::GetInstance().HasFlags());

    wxString flagText;
    int flagCount = this->objectSettings.GetFlagCount();
    wxString flagName;
    for (int i = 0; i < flagCount; i++)
    {
        this->objectSettings.GetFlag(i, flagName);
        if (!flagName.empty())
        {
            flagText += flagName;
            flagText += wxT("\n");
        }        
    }
    this->flagsText->SetValue(flagText);    

    UpdateObjectTypeDefaultText();
    UpdateVisibilityControls();
    UpdateExportedParentControls();
            
    return true;
}

void ObjectSettingsDialog_GeneralPage::OnTypeChoice(wxCommandEvent& event)
{
    DetectableObjectType* type = UpdateObjectTypeDefaultText();
    this->dialog->SelectedObjectType(type);    

    this->isPrefabCheckbox->Show(type->GetDescriptor().CanBePrefab());
    //this->isPrefabInstanceCheckbox->Show(type->GetDescriptor().CanBePrefab());
}

void ObjectSettingsDialog_GeneralPage::OnVisibilityChoice(wxCommandEvent& event)
{
    UpdateVisibilityControls();
}

void ObjectSettingsDialog_GeneralPage::OnExportedParentButton(wxCommandEvent& event)
{
    ObjectAccessorSet excludeObjects;
    GetObjectTree(this->object, excludeObjects);
    
    ChooseObjectsPrompt choose(this, HowMany::CHOOSE_ONE, ObjectFilterType::ALL, 0, &excludeObjects);
    if (choose.ShowPrompt())
    {
        this->exportedParentObject = choose.GetObject();
        this->exportedParentText->SetValue(this->exportedParentObject.GetFullName());        
    }    
}

void ObjectSettingsDialog_GeneralPage::UseRootAsExportedParent(wxCommandEvent& event)
{
    UpdateExportedParentControls();
}

void ObjectSettingsDialog_GeneralPage::OnAddFlagButton(wxCommandEvent& event)
{
    auto& globalSettings = FinjinGlobalSettings::GetInstance();

    SUSPEND_CUSTOM_CONTROLS;
    std::vector<wxString> flagNames;
    std::vector<int> flagBits;
    globalSettings.GetFlags(flagNames, flagBits);
    FlagDialog dialog(this, Strings::ADD_FLAG, flagNames, flagBits);
    ModalDialogFix modalDialogFix(&dialog);
    if (dialog.ShowModal() == wxID_OK)
    {
        //Get query flags
        wxString text = this->flagsText->GetValue();

        //Append alias or bit
        wxString flagText;
        if (!dialog.name.empty())
            flagText += dialog.name;                                    
        else if (dialog.bit != -1)
            flagText += StringUtilities::ToString(dialog.bit);                                
        
        if (!flagText.empty())
        {
            text += flagText;
            text += wxT("\n");

            //Set query flags                                    
            this->flagsText->SetValue(text);
        }
    }    
}

DetectableObjectType* ObjectSettingsDialog_GeneralPage::GetSelectedObjectType()
{
    int typeIndex = this->typeChoice->GetSelection();
    return (DetectableObjectType*)this->typeChoice->GetClientData(typeIndex);
}

DetectableObjectType* ObjectSettingsDialog_GeneralPage::UpdateObjectTypeDefaultText()
{
    int typeIndex = this->typeChoice->GetSelection();
    DetectableObjectType* type = (DetectableObjectType*)this->typeChoice->GetClientData(typeIndex);
    
    //Set default text
    wxString typeText;
    if (typeIndex == 0)
        typeText = wxString::Format(Strings::DEFAULT_FORMAT, type->GetDescriptor().GetDisplayName().wx_str());
    this->defaultTypeLabel->SetLabel(typeText);
    Layout();

    return type;
}

void ObjectSettingsDialog_GeneralPage::UpdateVisibilityControls()
{
    //Visibility
    int selectedVisibilityIndex = this->visibilityChoice->GetSelection();
    wxString visibility = (wxChar*)this->visibilityChoice->GetClientData(selectedVisibilityIndex);
    this->visibilityAffectObjectOnlyCheckbox->Show(IsObjectVisibility(visibility));
    Layout();
}

void ObjectSettingsDialog_GeneralPage::UpdateExportedParentControls()
{
    bool enableButton = !this->useRootAsExportedParentCheckbox->GetValue();
    this->exportedParentText->Enable(enableButton);
    this->exportedParentButton->Enable(enableButton);    
}
