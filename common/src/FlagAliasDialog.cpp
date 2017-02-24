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
#include "FlagAliasDialog.hpp"
#include "StringUtilities.hpp"
#include "Dialogs.hpp"
#include "Strings.hpp"
#include "WindowPlacementManager.hpp"
#include "ApplicationControls.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(FlagAliasDialog, FinjinDialog)
    EVT_BUTTON(wxID_OK, FlagAliasDialog::OnOK)    
END_EVENT_TABLE()

FlagAliasDialog::FlagAliasDialog(wxWindow* parent, const wxString& title, const std::set<wxString>& usedNames, const std::set<int>& usedBits, const wxString& name, int bit) : 
    FinjinDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize( 207,127 ), wxDEFAULT_DIALOG_STYLE, wxT("FlagAliasDialog"))
{
    this->name = name;
    this->bit = bit;
    this->usedNames = usedNames;
    this->usedBits = usedBits;

    SetSizeHints(wxDefaultSize, wxDefaultSize);
    
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    nameLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    nameLabel->Wrap( -1 );
    gridSizer->Add( nameLabel, 0, wxALL, 5 );
    
    nameText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), 0 );
    gridSizer->Add( nameText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
    
    bitLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Bit"), wxDefaultPosition, wxDefaultSize, 0 );
    bitLabel->Wrap( -1 );
    gridSizer->Add( bitLabel, 0, wxALL, 5 );
    
    bitSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(50, -1), 0, 0, FINJIN_EXPORTER_MAX_FLAG_BITS - 1, SpinnerControl::INT_UNIT);
    gridSizer->Add( bitSpinner, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
    
    topSizer->Add( gridSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
    
    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );
    
    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );
    
    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );
    
    topSizer->Add( okCancelSizer, 0, wxALIGN_CENTER, 5 );
    
    SetGUIData();

    SetSizer(topSizer);
    
    if (!WindowPlacementManager::RestorePlacement(this))
    {
        Layout();
        Fit();
        Centre(wxBOTH);
    }
}

FlagAliasDialog::~FlagAliasDialog()
{
}

void FlagAliasDialog::SetGUIData()
{
    this->nameText->SetValue(this->name);
    this->bitSpinner->SetValue(this->bit);
}

void FlagAliasDialog::OnOK(wxCommandEvent& event)
{
    WindowPlacementManager::SavePlacement(this);

    this->name = this->nameText->GetValue();
    this->bit = this->bitSpinner->GetIntValue();
    
    if (this->name.empty())
    {
        Dialogs::ShowMessage(this, Strings::FLAG_NAME_REQUIRED, Strings::DATA_ENTRY_ERROR, wxINFORMATION_DIALOG_FLAGS);
        this->nameText->SetFocus();
    }
    else if (StringUtilities::IsInteger(this->name))
    {
        Dialogs::ShowMessage(this, Strings::FLAG_NAME_CANT_BE_INTEGER, Strings::DATA_ENTRY_ERROR, wxINFORMATION_DIALOG_FLAGS);
        this->nameText->SetFocus();
    }
    else if (this->usedNames.find(this->name) != this->usedNames.end())
    {
        Dialogs::ShowMessage(this, Strings::UNIQUE_FLAG_NAME_REQUIRED, Strings::DATA_ENTRY_ERROR, wxINFORMATION_DIALOG_FLAGS);
        this->nameText->SetFocus();
    }
    else if (this->usedBits.find(bit) != this->usedBits.end())
        Dialogs::ShowMessage(this, Strings::UNIQUE_FLAG_BIT_REQUIRED, Strings::DATA_ENTRY_ERROR, wxINFORMATION_DIALOG_FLAGS);        
    else
        MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
}
