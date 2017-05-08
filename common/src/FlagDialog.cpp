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
#include "FlagDialog.hpp"
#include "StringUtilities.hpp"
#include "Dialogs.hpp"
#include "Strings.hpp"
#include "WindowPlacementManager.hpp"
#include "ApplicationControls.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(FlagDialog, FinjinDialog)
    EVT_BUTTON(wxID_OK, FlagDialog::OnOK)
END_EVENT_TABLE()

FlagDialog::FlagDialog
    (
    wxWindow* parent,
    const wxString& title,
    const std::vector<wxString>& flagNames,
    const std::vector<int>& flagBits
    )
    : FinjinDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize( 217,133 ), wxDEFAULT_DIALOG_STYLE, wxT("FlagDialog"))
{
    this->flagNames = flagNames;
    this->flagBits = flagBits;
    this->bit = -1;

    SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);

    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 1, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    nameText = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    nameText->Wrap( -1 );
    gridSizer->Add( nameText, 0, wxALL, 5 );

    wxArrayString nameChoiceChoices;
    nameChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, nameChoiceChoices, 0 );
    nameChoice->SetSelection( 0 );
    gridSizer->Add( nameChoice, 0, wxALL, 5 );

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

FlagDialog::~FlagDialog()
{
}

void FlagDialog::SetGUIData()
{
    //Add flag names
    auto nameEnabled = true;
    int selectedFlagNameIndex = 0;
    if (!this->flagNames.empty())
    {
        this->nameChoice->Freeze();
        int index = 0;
        for (auto& name : this->flagNames)
        {
            this->nameChoice->Append(name);
            if (name == this->name)
                selectedFlagNameIndex = index;
            index++;
        }
        this->nameChoice->Thaw();
        this->nameChoice->SetSelection(selectedFlagNameIndex);
    }
    else
        nameEnabled = false;
}

void FlagDialog::OnOK(wxCommandEvent& event)
{
    WindowPlacementManager::SavePlacement(this);

    this->name = wxEmptyString;
    this->bit = -1;

    int selectedFlagNameIndex = this->nameChoice->GetSelection();
    auto flagName = this->flagNames.begin();
    std::advance(flagName, selectedFlagNameIndex);
    this->name = *flagName;

    MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
}

wxString FlagDialog::GetFlagNameFromBit(int bit)
{
    wxString name;

    size_t count = 0;
    for (auto& flagBit : this->flagBits)
    {
        if (flagBit == bit)
        {
            auto flagName = this->flagNames.begin();
            std::advance(flagName, count);
            name = *flagName;
            break;
        }
        count++;
    }

    return name;
}
