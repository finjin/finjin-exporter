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
#include "ChooseMaterialsDialog.hpp"
#include "ApplicationControls.hpp"
#include "WindowPlacementManager.hpp"
#include "ApplicationAccessor.hpp"
#include "MaterialAccessor.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(ChooseMaterialsDialog, FinjinDialog)
    EVT_BUTTON(wxID_OK, ChooseMaterialsDialog::OnOK)    
    EVT_TEXT(ChooseMaterialsDialog::NAME_TEXT, ChooseMaterialsDialog::OnNameTextChanged)
    EVT_LISTBOX_DCLICK(ChooseMaterialsDialog::OBJECTS_LIST, ChooseMaterialsDialog::OnObjectsListDClick)
    EVT_CHECKBOX(ChooseMaterialsDialog::SHOW_FULL_NAMES_CHECKBOX, ChooseMaterialsDialog::OnShowFullNamesCheckBox)
    EVT_BUTTON(ChooseMaterialsDialog::SELECT_ALL_BUTTON, ChooseMaterialsDialog::OnSelectAllButton)
    EVT_BUTTON(ChooseMaterialsDialog::SELECT_NONE_BUTTON, ChooseMaterialsDialog::OnSelectNoneButton)
    EVT_BUTTON(ChooseMaterialsDialog::SELECT_INVERT_BUTTON, ChooseMaterialsDialog::OnSelectInvertButton)    
END_EVENT_TABLE()

ChooseMaterialsDialog::ChooseMaterialsDialog(wxWindow* parent, bool singleSelection, wxWindowID id, const wxString& title, const wxPoint& pos) 
    : FinjinDialog( parent, id, !title.empty() ? title : (singleSelection ? wxString(wxT("Choose Material")) : wxString(wxT("Choose Materials"))), pos, wxSize( 380,456 ), wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, wxT("ChooseMaterialsDialog"))
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
    
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );
    
    wxBoxSizer* nameSizer;
    nameSizer = new wxBoxSizer( wxVERTICAL );
    
    nameLabel = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    nameLabel->Wrap( -1 );
    nameSizer->Add( nameLabel, 0, wxALL, 5 );
    
    nameTextBox = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
    nameTextBox->SetMinSize( wxSize( 200,-1 ) );
    
    nameSizer->Add( nameTextBox, 1, wxEXPAND|wxBOTTOM|wxLEFT|wxRIGHT, 5 );
    
    topSizer->Add( nameSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
    
    wxBoxSizer* objectsListSizer;
    objectsListSizer = new wxBoxSizer( wxVERTICAL );

    showFullNamesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Show Full Names"), wxDefaultPosition, wxDefaultSize, 0 );
    objectsListSizer->Add( showFullNamesCheckbox, 0, wxLEFT|wxTOP, 5 );
    showFullNamesCheckbox->Show(ApplicationAccessor::UsesFullNames());
    
    long selectionStyle = singleSelection ? wxLB_SINGLE : wxLB_MULTIPLE;
    objectsListBox = new ApplicationListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, wxLB_NEEDED_SB|selectionStyle ); 
    objectsListBox->SetMinSize( wxSize( 200,250 ) );
    
    objectsListSizer->Add( objectsListBox, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
    
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
    
    SetGUIData();

    this->SetSizer( topSizer );
    this->SetMinSize(wxSize(380, 456));
    
    if (!WindowPlacementManager::RestorePlacement(this))
    {
        this->Layout();
        Centre( wxBOTH );
    }
}

ChooseMaterialsDialog::~ChooseMaterialsDialog()
{
}

void ChooseMaterialsDialog::SetGUIData()
{       
    //Get all materials
    MaterialAccessorVector materials;
    MaterialAccessor::GetAllMaterials(materials);

    //Add the materials to the list of filter materials
    this->filteredSceneObjects.clear();
    this->filteredSceneObjects.reserve(materials.size());
    for (size_t i = 0; i < materials.size(); i++)
    {
        FilteredObject choosableObject;
        choosableObject.object = materials[i];
        this->filteredSceneObjects.push_back(choosableObject);
    }
    
    //Sort into alphabetical order
    std::sort(this->filteredSceneObjects.begin(), this->filteredSceneObjects.end());

    //Show filtered objects
    FilterObjects();
}

void ChooseMaterialsDialog::FilterObjects(bool saveSelection)
{
    int selection = 0;
    wxArrayInt selections;
    if (saveSelection)
    {
        if (this->objectsListBox->GetWindowStyle() & wxLB_SINGLE)
            selection = this->objectsListBox->GetSelection();
        else
            this->objectsListBox->GetSelections(selections);
    }
    bool useFullNames = this->showFullNamesCheckbox->GetValue();

    //Clear list box and filtered objects
    this->objectsListBox->Clear();
    
    wxString nameFilter = this->nameTextBox->GetValue();
    nameFilter.MakeLower();

    //Fill list with objects
    wxString name;
    this->objectsListBox->Freeze();
    for (size_t i = 0; i < this->filteredSceneObjects.size(); i++)
    {
        //Check name
        MaterialAccessor object = this->filteredSceneObjects[i].object;
        name = useFullNames ? object.GetFullName() : object.GetLocalName();
        name.MakeLower();
        if (nameFilter.empty() || name.Find(nameFilter) != -1)
        {
            int insertedAt = this->objectsListBox->Append(useFullNames ? object.GetFullName() : object.GetLocalName());
            this->objectsListBox->SetClientData(insertedAt, &this->filteredSceneObjects[i]);
        }        
    }

    if (saveSelection)
    {
        if (this->objectsListBox->GetWindowStyle() & wxLB_SINGLE)
            this->objectsListBox->SetSelection(selection);
        else
        {
            for (size_t i = 0; i < selections.size(); i++)
                this->objectsListBox->SetSelection(selections[i]);
        }
    }

    this->objectsListBox->Thaw();
}

void ChooseMaterialsDialog::HandleOK()
{
    WindowPlacementManager::SavePlacement(this);

    if (this->objectsListBox->GetWindowStyle() & wxLB_SINGLE)
    {
        //Single selection
        int selection = this->objectsListBox->GetSelection();
        if (selection >= 0)
        {
            this->chosenObjects.resize(1);
            FilteredObject* chosenObject = (FilteredObject*)this->objectsListBox->GetClientData(selection);
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
            FilteredObject* chosenObject = (FilteredObject*)this->objectsListBox->GetClientData(selections[i]);
            this->chosenObjects[i] = chosenObject->object;
        }
    }
}

void ChooseMaterialsDialog::OnOK(wxCommandEvent& event)
{
    HandleOK();
    MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
}

void ChooseMaterialsDialog::OnNameTextChanged(wxCommandEvent& WXUNUSED(event))
{
    FilterObjects();
}

void ChooseMaterialsDialog::OnShowFullNamesCheckBox(wxCommandEvent& WXUNUSED(event))
{
    FilterObjects(true);
}

void ChooseMaterialsDialog::OnObjectsListDClick(wxCommandEvent& event)
{
    HandleOK();
    MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
}

void ChooseMaterialsDialog::OnSelectAllButton(wxCommandEvent& WXUNUSED(event))
{
    for (size_t i = 0; i < this->objectsListBox->GetCount(); i++)
        this->objectsListBox->SetSelection((int)i, true);
}

void ChooseMaterialsDialog::OnSelectNoneButton(wxCommandEvent& WXUNUSED(event))
{
    for (size_t i = 0; i < this->objectsListBox->GetCount(); i++)
        this->objectsListBox->SetSelection((int)i, false);
}

void ChooseMaterialsDialog::OnSelectInvertButton(wxCommandEvent& WXUNUSED(event))
{
    for (size_t i = 0; i < this->objectsListBox->GetCount(); i++)
        this->objectsListBox->SetSelection((int)i, !this->objectsListBox->IsSelected((int)i));
}
