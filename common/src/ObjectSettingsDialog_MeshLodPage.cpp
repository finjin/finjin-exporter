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
#include "ObjectSettingsDialog_MeshLodPage.hpp"
#include "ApplicationControls.hpp"
#include "StringUtilities.hpp"
#include "Strings.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "ManualLodDialog.hpp"
#include "ApplicationAccessor.hpp"
#include "ModalDialogFix.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
const wxString ObjectSettingsDialog_MeshLodPage::TITLE(wxT("Mesh LOD"));


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(ObjectSettingsDialog_MeshLodPage, SettingsPage)
    EVT_LIST_ITEM_ACTIVATED(ObjectSettingsDialog_MeshLodPage::MANUAL_LOD_LIST, ObjectSettingsDialog_MeshLodPage::OnManualLodListActivate)    
    EVT_LIST_ITEM_SELECTED(ObjectSettingsDialog_MeshLodPage::MANUAL_LOD_LIST, ObjectSettingsDialog_MeshLodPage::OnManualLodListActivateSelectionChange)    
    EVT_LIST_ITEM_DESELECTED(ObjectSettingsDialog_MeshLodPage::MANUAL_LOD_LIST, ObjectSettingsDialog_MeshLodPage::OnManualLodListActivateSelectionChange)    
    EVT_BUTTON(ObjectSettingsDialog_MeshLodPage::ADD_MANUAL_LOD_BUTTON, ObjectSettingsDialog_MeshLodPage::OnAddManualLodButton)
    EVT_BUTTON(ObjectSettingsDialog_MeshLodPage::EDIT_MANUAL_LOD_BUTTON, ObjectSettingsDialog_MeshLodPage::OnEditManualLodButton)
    EVT_BUTTON(ObjectSettingsDialog_MeshLodPage::REMOVE_MANUAL_LOD_BUTTON, ObjectSettingsDialog_MeshLodPage::OnRemoveManualLodButton)
    EVT_CHECKBOX(ObjectSettingsDialog_MeshLodPage::CALCULATE_MANUAL_DISTANCES_CHECKBOX, ObjectSettingsDialog_MeshLodPage::OnCalculateDistancesCheckbox)
END_EVENT_TABLE()

ObjectSettingsDialog_MeshLodPage::ObjectSettingsDialog_MeshLodPage(wxWindow* parent, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    this->object = object;
    this->objectSettings = objectSettings;

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxStaticBoxSizer* levelOfDetailSettingsSizer;
    levelOfDetailSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Level-of-Detail (LOD) Settings") ), wxVERTICAL );
    
    wxBoxSizer* manualLodSizer;
    manualLodSizer = new wxBoxSizer( wxHORIZONTAL );
        
    manualLodList = new ApplicationListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_NO_SORT_HEADER|wxLC_HRULES|wxLC_VRULES );
    manualLodSizer->Add( manualLodList, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxTOP, 5 );
    
    wxBoxSizer* manualLodButtonsSizer;
    manualLodButtonsSizer = new wxBoxSizer(wxVERTICAL);
    
    addManualLodButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxSize(STANDARD_BUTTON_WIDTH,-1), 0 );
    manualLodButtonsSizer->Add( addManualLodButton, 0, wxLEFT, 5 );
    
    editManualLodButton = new wxButton( this, wxID_ANY, wxT("Edit..."), wxDefaultPosition, wxSize(STANDARD_BUTTON_WIDTH,-1), 0 );
    manualLodButtonsSizer->Add( editManualLodButton, 0, wxLEFT, 5 );
    
    removeManualLodButton = new wxButton( this, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxSize(STANDARD_BUTTON_WIDTH,-1), 0 );
    manualLodButtonsSizer->Add( removeManualLodButton, 0, wxLEFT, 5 );

    manualLodButtonsSizer->Add( 0, 10, 0, wxEXPAND, 5 );

    calculateManualDistancesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Auto Distances"), wxDefaultPosition, wxDefaultSize, 0 );    
    manualLodButtonsSizer->Add( calculateManualDistancesCheckbox, 0, wxALL, 5 );

    manualLodSizer->Add( manualLodButtonsSizer, 0, wxEXPAND, 5 );
    
    levelOfDetailSettingsSizer->Add( manualLodSizer, 1, wxEXPAND, 5 );
    
    topSizer->Add( levelOfDetailSettingsSizer, 1, wxEXPAND|wxTOP, 5 );
    
    SetSizer(topSizer);

    this->manualLodList->SetId(MANUAL_LOD_LIST);
    this->addManualLodButton->SetId(ADD_MANUAL_LOD_BUTTON);
    this->editManualLodButton->SetId(EDIT_MANUAL_LOD_BUTTON);
    this->removeManualLodButton->SetId(REMOVE_MANUAL_LOD_BUTTON);
    this->calculateManualDistancesCheckbox->SetId(CALCULATE_MANUAL_DISTANCES_CHECKBOX);

    Layout();
}

ObjectSettingsDialog_MeshLodPage::~ObjectSettingsDialog_MeshLodPage()
{
}

void ObjectSettingsDialog_MeshLodPage::GetExcludedLodObjects(ObjectAccessorSet& excludedLodObjects)
{
    excludedLodObjects.insert(this->object);
    for (size_t i = 0; i < this->manualLods.size(); i++)
        excludedLodObjects.insert(this->manualLods[i].object);
}

bool ObjectSettingsDialog_MeshLodPage::GetGUIData()
{
    //Calculate manual LOD distances
    this->objectSettings.SetCalculateManualLodDistances(this->calculateManualDistancesCheckbox->GetValue());
    
    return true;
}

bool ObjectSettingsDialog_MeshLodPage::SetGUIData()
{
    //Manual LOD list
    this->manualLodList->InsertColumn(0, Strings::DISTANCE, wxLIST_FORMAT_LEFT, 141);
    this->manualLodList->InsertColumn(1, Strings::OBJECT, wxLIST_FORMAT_LEFT, 226);
    
    this->objectSettings.GetManualLods(this->object, this->manualLods);
    for (int i = 0; i < (int)this->manualLods.size(); i++)
        this->manualLodList->InsertItem(i, wxEmptyString);
    UpdateManualLodGUIData();
    this->manualLodList->SetSelection(0);

    //Calculate manual LOD distances
    this->calculateManualDistancesCheckbox->SetValue(this->objectSettings.GetCalculateManualLodDistances());

    //Final updates
    UpdateManualLodButtons();
    
    return true;
}

void ObjectSettingsDialog_MeshLodPage::UpdateManualLodGUIData()
{
    for (int i = 0; i < (int)this->manualLods.size(); i++)
    {
        this->manualLodList->SetItem(i, 0, ApplicationAccessor::FormatWorldValue(this->manualLods[i].distance));
        this->manualLodList->SetItem(i, 1, this->manualLods[i].object.GetFullName());
    }
}

void ObjectSettingsDialog_MeshLodPage::UpdateManualLodButtons()
{
    int selectedItemCount = this->manualLodList->GetSelectedItemCount();
    this->editManualLodButton->Enable(selectedItemCount == 1);
    this->removeManualLodButton->Enable(selectedItemCount > 0);
}

void ObjectSettingsDialog_MeshLodPage::OnEditManualLod()
{
    int selection = this->manualLodList->GetSelection();
    if (selection >= 0)
    {
        ObjectAccessorSet excludedLodObjects;
        GetExcludedLodObjects(excludedLodObjects);

        SUSPEND_CUSTOM_CONTROLS;
        ManualLodDialog dialog(this, Strings::EDIT_MANUAL_LOD, this->object, this->manualLods[selection].object, this->manualLods[selection].distance, &excludedLodObjects, this->calculateManualDistancesCheckbox->GetValue());
        ModalDialogFix modalDialogFix(&dialog);
        if (dialog.ShowModal() == wxID_OK)
        {
            this->objectSettings.SetManualLod(this->manualLods[selection].objectSettingsIndex, dialog.object, dialog.distance);
            this->objectSettings.GetManualLods(this->object, this->manualLods);
            UpdateManualLodGUIData();
        }
    }
}

void ObjectSettingsDialog_MeshLodPage::OnManualLodListActivate(wxListEvent& event)
{
    OnEditManualLod();
}

void ObjectSettingsDialog_MeshLodPage::OnManualLodListActivateSelectionChange(wxListEvent& event)
{
    UpdateManualLodButtons();
}

void ObjectSettingsDialog_MeshLodPage::OnAddManualLodButton(wxCommandEvent& event)
{   
    ObjectAccessorSet excludedLodObjects;
    GetExcludedLodObjects(excludedLodObjects);

    SUSPEND_CUSTOM_CONTROLS;
    ManualLodDialog dialog(this, Strings::ADD_MANUAL_LOD, this->object, ObjectAccessor(), 0, &excludedLodObjects, this->calculateManualDistancesCheckbox->GetValue());
    ModalDialogFix modalDialogFix(&dialog);
    if (dialog.ShowModal() == wxID_OK)
    {
        //Add the LOD to the object settings
        this->objectSettings.AddManualLod(dialog.object, dialog.distance);
        this->objectSettings.GetManualLods(this->object, this->manualLods);
        
        //Add the LOD to the list control
        this->manualLodList->InsertItem((int)this->manualLods.size() - 1, wxEmptyString);
        UpdateManualLodGUIData();
    }
}

void ObjectSettingsDialog_MeshLodPage::OnEditManualLodButton(wxCommandEvent& event)
{    
    OnEditManualLod();
}

void ObjectSettingsDialog_MeshLodPage::OnRemoveManualLodButton(wxCommandEvent& event)
{    
    std::vector<int> selectedItems;
    if (this->manualLodList->GetSelection(selectedItems))
    {
        for (int i = (int)selectedItems.size() - 1; i >= 0; i--)
        {
            int selection = selectedItems[i];

            //Remove LOD from object
            this->objectSettings.RemoveManualLod(selection);

            //Remove LOD from list
            this->manualLodList->DeleteItem(selection);
        }

        UpdateManualLodButtons();

        if (this->manualLodList->GetItemCount() > 0)
            this->manualLodList->SetSelection(std::max(selectedItems[0]-1, 0));
    }
}

void ObjectSettingsDialog_MeshLodPage::OnCalculateDistancesCheckbox(wxCommandEvent& event)
{
    const bool calculateDistances = this->calculateManualDistancesCheckbox->GetValue();

    this->objectSettings.SetCalculateManualLodDistances(calculateDistances);

    if (calculateDistances && !this->manualLods.empty())
    {
        const TimeAccessor currentTime = FinjinObjectSettingsAccessor::GetAutoManualLodDistanceTime();
        const FinjinVector3 baseObjectPosition = this->object.GetNodeTransformation(currentTime).GetTranslation();
        for (size_t i = 0; i < this->manualLods.size(); i++)
            this->manualLods[i].distance = (this->manualLods[i].object.GetNodeTransformation(currentTime).GetTranslation() - baseObjectPosition).Length();
        
        this->manualLods.Sort();

        for (size_t i = 0; i < this->manualLods.size(); i++)
            this->objectSettings.SetManualLod(this->manualLods[i].objectSettingsIndex, this->manualLods[i].object, this->manualLods[i].distance);
        
        UpdateManualLodGUIData();
    }
}
