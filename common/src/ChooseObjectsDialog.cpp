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
#include "ChooseObjectsDialog.hpp"
#include "ApplicationControls.hpp"
#include "WindowPlacementManager.hpp"
#include "ApplicationAccessor.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(ChooseObjectsDialog, FinjinDialog)
    EVT_BUTTON(wxID_OK, ChooseObjectsDialog::OnOK)
    EVT_TEXT(ChooseObjectsDialog::NAME_TEXT, ChooseObjectsDialog::OnNameTextChanged)
    EVT_CHECKBOX(ChooseObjectsDialog::SHOW_FULL_NAMES_CHECKBOX, ChooseObjectsDialog::OnShowFullNamesCheckBox)
    EVT_LISTBOX_DCLICK(ChooseObjectsDialog::OBJECTS_LIST, ChooseObjectsDialog::OnObjectsListDClick)
    EVT_BUTTON(ChooseObjectsDialog::SELECT_ALL_BUTTON, ChooseObjectsDialog::OnSelectAllButton)
    EVT_BUTTON(ChooseObjectsDialog::SELECT_NONE_BUTTON, ChooseObjectsDialog::OnSelectNoneButton)
    EVT_BUTTON(ChooseObjectsDialog::SELECT_INVERT_BUTTON, ChooseObjectsDialog::OnSelectInvertButton)
    EVT_CHECKBOX(ChooseObjectsDialog::MESHES_CHECKBOX, ChooseObjectsDialog::OnMeshesCheckBox)
    EVT_CHECKBOX(ChooseObjectsDialog::PLANES_CHECKBOX, ChooseObjectsDialog::OnPlanesCheckBox)
    EVT_CHECKBOX(ChooseObjectsDialog::LIGHTS_CHECKBOX, ChooseObjectsDialog::OnLightsCheckBox)
    EVT_CHECKBOX(ChooseObjectsDialog::CAMERAS_CHECKBOX, ChooseObjectsDialog::OnCamerasCheckBox)
    EVT_CHECKBOX(ChooseObjectsDialog::OTHER_CHECKBOX, ChooseObjectsDialog::OnOtherCheckBox)
END_EVENT_TABLE()

ChooseObjectsDialog::ChooseObjectsDialog
    (
    wxWindow* parent,
    bool singleSelection,
    ObjectFilterType types,
    const ObjectAccessorSet* chooseObjects,
    const ObjectAccessorSet* excludeObjects,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos
    )
    : FinjinDialog( parent, id, !title.empty() ? title : (singleSelection ? wxString(wxT("Choose Object")) : wxString(wxT("Choose Objects"))), pos, wxSize( 380,456 ), wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, wxT("ChooseObjectsDialog"))
{
    this->filteredSceneObjects.SetTypes(types);
    if (chooseObjects != nullptr)
        this->filteredSceneObjects.SetChooseObjects(*chooseObjects);
    if (excludeObjects != nullptr)
        this->filteredSceneObjects.SetExcludeObjects(*excludeObjects);

    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* nameSizer;
    nameSizer = new wxBoxSizer( wxVERTICAL );

    nameLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    nameLabel->Wrap( -1 );
    nameSizer->Add( nameLabel, 0, wxALL, 5 );

    nameTextBox = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
    nameTextBox->SetMinSize( wxSize( 200,-1 ) );

    nameSizer->Add( nameTextBox, 1, wxEXPAND|wxBOTTOM|wxLEFT, 5 );

    topSizer->Add( nameSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* objectsListSizer;
    objectsListSizer = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer* listBoxSizer;
    listBoxSizer = new wxBoxSizer( wxVERTICAL );

    showFullNamesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Show Full Names"), wxDefaultPosition, wxDefaultSize, 0 );
    listBoxSizer->Add( showFullNamesCheckbox, 0, wxLEFT|wxTOP, 5 );
    showFullNamesCheckbox->Show(ApplicationAccessor::UsesFullNames());

    long selectionStyle = singleSelection ? wxLB_SINGLE : wxLB_MULTIPLE;
    objectsListBox = new ApplicationListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, wxLB_NEEDED_SB|selectionStyle );
    objectsListBox->SetMinSize( wxSize( 200,250 ) );

    listBoxSizer->Add( objectsListBox, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    objectsListSizer->Add( listBoxSizer, 1, wxEXPAND, 5 );

    wxStaticBoxSizer* typesSizer;
    typesSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Types") ), wxVERTICAL );

    typesSizer->SetMinSize( wxSize( 110,-1 ) );
    meshesCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Meshes"), wxDefaultPosition, wxDefaultSize, 0 );
    meshesCheckBox->Show(AnySet(types & ObjectFilterType::MESH));

    typesSizer->Add( meshesCheckBox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    planesCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Planes"), wxDefaultPosition, wxDefaultSize, 0 );
    planesCheckBox->Show(AnySet(types & ObjectFilterType::PLANE));

    typesSizer->Add( planesCheckBox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    lightsCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Lights"), wxDefaultPosition, wxDefaultSize, 0 );
    lightsCheckBox->Show(AnySet(types & ObjectFilterType::LIGHT));

    typesSizer->Add( lightsCheckBox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    camerasCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Cameras"), wxDefaultPosition, wxDefaultSize, 0 );
    camerasCheckBox->Show(AnySet(types & ObjectFilterType::CAMERA));

    typesSizer->Add( camerasCheckBox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    otherCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Other"), wxDefaultPosition, wxDefaultSize, 0 );
    otherCheckBox->Show(AnySet(types & ObjectFilterType::OTHER));

    typesSizer->Add( otherCheckBox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    objectsListSizer->Add( typesSizer, 0, wxEXPAND, 5 );

    topSizer->Add( objectsListSizer, 1, wxEXPAND|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* buttonsSizer;
    buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

    allButton = new wxButton( this, wxID_ANY, wxT("All"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsSizer->Add( allButton, 0, wxALL, 5 );

    noneButton = new wxButton( this, wxID_ANY, wxT("None"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsSizer->Add( noneButton, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );

    invertButton = new wxButton( this, wxID_ANY, wxT("Invert"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsSizer->Add( invertButton, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );

    if (singleSelection)
        buttonsSizer->Show(false);

    topSizer->Add( buttonsSizer, 0, wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );

    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT, 5 );

    this->nameTextBox->SetId(NAME_TEXT);
    this->showFullNamesCheckbox->SetId(SHOW_FULL_NAMES_CHECKBOX);
    this->objectsListBox->SetId(OBJECTS_LIST);
    this->allButton->SetId(SELECT_ALL_BUTTON);
    this->noneButton->SetId(SELECT_NONE_BUTTON);
    this->invertButton->SetId(SELECT_INVERT_BUTTON);
    this->meshesCheckBox->SetId(MESHES_CHECKBOX);
    this->planesCheckBox->SetId(PLANES_CHECKBOX);
    this->lightsCheckBox->SetId(LIGHTS_CHECKBOX);
    this->camerasCheckBox->SetId(CAMERAS_CHECKBOX);
    this->otherCheckBox->SetId(OTHER_CHECKBOX);

    SetGUIData();

    this->SetSizer( topSizer );
    this->SetMinSize(wxSize(380, 456));

    if (!WindowPlacementManager::RestorePlacement(this))
    {
        this->Layout();
        Centre( wxBOTH );
    }
}

ChooseObjectsDialog::~ChooseObjectsDialog()
{
}

void ChooseObjectsDialog::SetGUIData()
{
    //Check the appropriate checkboxes
    this->meshesCheckBox->SetValue(this->meshesCheckBox->IsShown());
    this->planesCheckBox->SetValue(this->planesCheckBox->IsShown());
    this->lightsCheckBox->SetValue(this->lightsCheckBox->IsShown());
    this->camerasCheckBox->SetValue(this->camerasCheckBox->IsShown());
    this->otherCheckBox->SetValue(this->otherCheckBox->IsShown());

    //Get objects
    this->filteredSceneObjects.Initialize(this->showFullNamesCheckbox->GetValue());

    //Show filtered objects
    FilterObjects();
}

void ChooseObjectsDialog::FilterObjects(bool saveSelection)
{
    std::vector<FilteredSceneObjects::FilteredObject> selectedObjects;
    if (saveSelection)
    {
        if (this->objectsListBox->GetWindowStyle() & wxLB_SINGLE)
        {
            int selection = this->objectsListBox->GetSelection();
            if (selection >= 0)
            {
                selectedObjects.resize(1);
                selectedObjects[0] = *(FilteredSceneObjects::FilteredObject*)this->objectsListBox->GetClientData(selection);
            }
        }
        else
        {
            wxArrayInt selections;
            this->objectsListBox->GetSelections(selections);
            selectedObjects.resize(selections.size());
            for (int i = 0; i < (int)selections.size(); i++)
                selectedObjects[i] = *(FilteredSceneObjects::FilteredObject*)this->objectsListBox->GetClientData(i);
        }
    }
    bool useFullNames = this->showFullNamesCheckbox->GetValue();
    this->filteredSceneObjects.Sort(useFullNames);

    //Clear list box and filtered objects
    this->objectsListBox->Clear();

    //Determine which types are currently selected
    auto typeFilter = ObjectFilterType::NONE;
    if (this->meshesCheckBox->GetValue())
        typeFilter |= ObjectFilterType::MESH;
    if (this->planesCheckBox->GetValue())
        typeFilter |= ObjectFilterType::PLANE;
    if (this->lightsCheckBox->GetValue())
        typeFilter |= ObjectFilterType::LIGHT;
    if (this->camerasCheckBox->GetValue())
        typeFilter |= ObjectFilterType::CAMERA;
    if (this->otherCheckBox->GetValue())
        typeFilter |= ObjectFilterType::OTHER;

    wxString nameFilter = this->nameTextBox->GetValue();
    nameFilter.MakeLower();

    //Fill list with objects
    wxString name;
    this->objectsListBox->Freeze();
    wxArrayInt selections;
    selections.reserve(selectedObjects.size());
    for (size_t i = 0; i < this->filteredSceneObjects.size(); i++)
    {
        auto& filteredObject = this->filteredSceneObjects[i];

        //Check type
        if (AnySet(filteredObject.type & typeFilter))
        {
            //Check name
            ObjectAccessor object = filteredObject.object;
            name = useFullNames ? object.GetFullName() : object.GetLocalName();
            name.MakeLower();
            if (nameFilter.empty() || name.Find(nameFilter) != -1)
            {
                int insertedAt = this->objectsListBox->Append(useFullNames ? object.GetFullName() : object.GetLocalName());
                this->objectsListBox->SetClientData(insertedAt, &filteredObject);

                //Save index in selected objects array
                if (std::find(selectedObjects.begin(), selectedObjects.end(), filteredObject) != selectedObjects.end())
                    selections.push_back((int)i);
            }
        }
    }

    if (saveSelection)
    {
        if (this->objectsListBox->GetWindowStyle() & wxLB_SINGLE)
        {
            if (!selections.empty())
                this->objectsListBox->SetSelection(selections[0]);
        }
        else
        {
            for (size_t i = 0; i < selections.size(); i++)
                this->objectsListBox->SetSelection(selections[i]);
        }
    }

    this->objectsListBox->Thaw();
}

void ChooseObjectsDialog::HandleOK()
{
    WindowPlacementManager::SavePlacement(this);

    if (this->objectsListBox->GetWindowStyle() & wxLB_SINGLE)
    {
        //Single selection
        int selection = this->objectsListBox->GetSelection();
        if (selection >= 0)
        {
            this->chosenObjects.resize(1);
            FilteredSceneObjects::FilteredObject* chosenObject =
                (FilteredSceneObjects::FilteredObject*)this->objectsListBox->GetClientData(selection);
            this->chosenObjects[0] = chosenObject->object;
        }
    }
    else
    {
        //Multiple selection
        wxArrayInt selections;
        this->objectsListBox->GetSelections(selections);
        this->chosenObjects.resize(selections.GetCount());
        for (int i = 0; i < (int)selections.GetCount(); i++)
        {
            FilteredSceneObjects::FilteredObject* chosenObject =
                (FilteredSceneObjects::FilteredObject*)this->objectsListBox->GetClientData(selections[i]);
            this->chosenObjects[i] = chosenObject->object;
        }
    }
}

void ChooseObjectsDialog::OnOK(wxCommandEvent& event)
{
    HandleOK();
    MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
}

void ChooseObjectsDialog::OnNameTextChanged(wxCommandEvent& event)
{
    FilterObjects();
}

void ChooseObjectsDialog::OnShowFullNamesCheckBox(wxCommandEvent& event)
{
    FilterObjects(true);
}

void ChooseObjectsDialog::OnObjectsListDClick(wxCommandEvent& event)
{
    HandleOK();
    EndModal(wxID_OK);
}

void ChooseObjectsDialog::OnSelectAllButton(wxCommandEvent& event)
{
    for (size_t i = 0; i < this->objectsListBox->GetCount(); i++)
        this->objectsListBox->SetSelection((int)i, true);
}

void ChooseObjectsDialog::OnSelectNoneButton(wxCommandEvent& event)
{
    for (size_t i = 0; i < this->objectsListBox->GetCount(); i++)
        this->objectsListBox->SetSelection((int)i, false);
}

void ChooseObjectsDialog::OnSelectInvertButton(wxCommandEvent& event)
{
    for (size_t i = 0; i < this->objectsListBox->GetCount(); i++)
        this->objectsListBox->SetSelection((int)i, !this->objectsListBox->IsSelected((int)i));
}

void ChooseObjectsDialog::OnMeshesCheckBox(wxCommandEvent& event)
{
    FilterObjects();
}

void ChooseObjectsDialog::OnPlanesCheckBox(wxCommandEvent& event)
{
    FilterObjects();
}

void ChooseObjectsDialog::OnLightsCheckBox(wxCommandEvent& event)
{
    FilterObjects();
}

void ChooseObjectsDialog::OnCamerasCheckBox(wxCommandEvent& event)
{
    FilterObjects();
}

void ChooseObjectsDialog::OnOtherCheckBox(wxCommandEvent& event)
{
    FilterObjects();
}
