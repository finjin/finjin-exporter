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
#include "ChooseFilesDialog.hpp"
#include "ApplicationControls.hpp"
#include "WindowPlacementManager.hpp"
#include "ApplicationAccessor.hpp"
#include "FileUtilities.hpp"
#include "StringUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(ChooseFilesDialog, FinjinDialog)
    EVT_BUTTON(wxID_OK, ChooseFilesDialog::OnOK)    
    EVT_TEXT(ChooseFilesDialog::NAME_TEXT, ChooseFilesDialog::OnNameTextChanged)    
    EVT_LISTBOX_DCLICK(ChooseFilesDialog::FILES_LIST, ChooseFilesDialog::OnFilesListDClick)
END_EVENT_TABLE()

ChooseFilesDialog::ChooseFilesDialog
    (
    wxWindow* parent, 
    bool singleSelection, 
    const std::vector<FileName>* chooseFiles,
    const wxString* chooseDirectory,
    const std::vector<wxString>* chooseExtensions,
    wxWindowID id, 
    const wxString& title, 
    const wxPoint& pos
    ) 
    : FinjinDialog( parent, id, !title.empty() ? title : (singleSelection ? wxString(wxT("Choose a File")) : wxString(wxT("Choose Files"))), pos, wxSize( 380,456 ), wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, wxT("ChooseFilesDialog"))
{
    if (chooseFiles != nullptr)
        this->allFileNames = *chooseFiles;
    else if (chooseDirectory != nullptr)
    {
        wxDir dir(*chooseDirectory);
        FileName fileName;
        auto cont = dir.GetFirst(&fileName.baseName, wxEmptyString, wxDIR_FILES);
        while (cont)
        {
            auto add = true;
            if (chooseExtensions != nullptr)
            {
                add = false;
                for (size_t i = 0; i < chooseExtensions->size(); i++)
                {
                    if (StringUtilities::EndsWithNoCase(fileName.baseName, (*chooseExtensions)[i]))
                    {
                        add = true;
                        break;
                    }
                }
            }
            
            if (add)
            {
                fileName.fullPath = FileUtilities::JoinPath(*chooseDirectory, fileName.baseName);
                this->allFileNames.push_back(fileName);
            }

            cont = dir.GetNext(&fileName.baseName);
        }
    }
    
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
    
    nameSizer->Add( nameTextBox, 1, wxBOTTOM|wxEXPAND|wxLEFT, 5 );
    
    topSizer->Add( nameSizer, 0, wxEXPAND|wxRIGHT|wxTOP, 5 );
    
    wxBoxSizer* filesListSizer;
    filesListSizer = new wxBoxSizer( wxHORIZONTAL );
    
    wxBoxSizer* listBoxSizer;
    listBoxSizer = new wxBoxSizer( wxVERTICAL );
    
    long selectionStyle = singleSelection ? wxLB_SINGLE : wxLB_MULTIPLE;
    filesListBox = new ApplicationListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, wxLB_NEEDED_SB|selectionStyle); 
    filesListBox->SetMinSize( wxSize( 200,250 ) );
    
    listBoxSizer->Add( filesListBox, 1, wxEXPAND, 5 );
    
    filesListSizer->Add( listBoxSizer, 1, wxEXPAND, 5 );
    
    topSizer->Add( filesListSizer, 1, wxEXPAND|wxRIGHT|wxLEFT, 5 );
    
    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );
    
    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );
    
    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );
    
    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT, 5 );

    this->nameTextBox->SetId(NAME_TEXT);
    this->filesListBox->SetId(FILES_LIST);
    
    SetGUIData();

    this->SetSizer( topSizer );
    this->SetMinSize(wxSize(380, 456));
    
    if (!WindowPlacementManager::RestorePlacement(this))
    {
        this->Layout();
        Centre( wxBOTH );
    }
}

ChooseFilesDialog::~ChooseFilesDialog()
{
}

void ChooseFilesDialog::SetGUIData()
{   
    FilterFiles();
}

void ChooseFilesDialog::FilterFiles(bool saveSelection)
{
    int selection = 0;
    wxArrayInt selections;
    if (saveSelection)
    {
        if (this->filesListBox->GetWindowStyle() & wxLB_SINGLE)
            selection = this->filesListBox->GetSelection();
        else
            this->filesListBox->GetSelections(selections);
    }
    
    //Clear list box
    this->filesListBox->Clear();
    
    wxString nameFilter = this->nameTextBox->GetValue();
    nameFilter.MakeLower();

    //Fill list with objects
    wxString name;
    this->filesListBox->Freeze();
    for (size_t i = 0; i < this->allFileNames.size(); i++)
    {
        //Check name
        name = this->allFileNames[i].baseName;
        name.MakeLower();
        if (nameFilter.empty() || name.Find(nameFilter) != -1)
        {
            int insertedAt = this->filesListBox->Append(this->allFileNames[i].baseName);
            this->filesListBox->SetClientData(insertedAt, &this->allFileNames[i]);
        }        
    }

    if (saveSelection)
    {
        if (this->filesListBox->GetWindowStyle() & wxLB_SINGLE)
            this->filesListBox->SetSelection(selection);
        else
        {
            for (size_t i = 0; i < selections.size(); i++)
                this->filesListBox->SetSelection(selections[i]);
        }
    }

    this->filesListBox->Thaw();
}

void ChooseFilesDialog::HandleOK()
{
    WindowPlacementManager::SavePlacement(this);

    if (this->filesListBox->GetWindowStyle() & wxLB_SINGLE)
    {
        //Single selection
        int selection = this->filesListBox->GetSelection();
        if (selection >= 0)
        {
            this->chosenFileNames.resize(1);
            this->chosenFileNames[0] = *(FileName*)this->filesListBox->GetClientData(selection);
        }
    }
    else
    {
        //Multiple selection
        wxArrayInt selections;
        this->filesListBox->GetSelections(selections);
        this->chosenFileNames.resize(selections.GetCount());
        for (int i = 0; i < (int)selections.GetCount(); i++)
            this->chosenFileNames[i] = *(FileName*)this->filesListBox->GetClientData(selections[i]);
    }
}

void ChooseFilesDialog::OnOK(wxCommandEvent& event)
{
    HandleOK();
    MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
}

void ChooseFilesDialog::OnNameTextChanged(wxCommandEvent& event)
{
    FilterFiles();
}

void ChooseFilesDialog::OnFilesListDClick(wxCommandEvent& event)
{    
    HandleOK();
    EndModal(wxID_OK);
}
