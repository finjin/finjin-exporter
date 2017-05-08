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
#include "InstallationDetailsDialog.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(InstallationDetailsDialog, wxDialog)
    EVT_BUTTON(wxID_OK, InstallationDetailsDialog::OnOK)
    EVT_CLOSE(InstallationDetailsDialog::OnCloseWindow)
END_EVENT_TABLE()

InstallationDetailsDialog::InstallationDetailsDialog(wxWindow* parent, const wxString& results, InstallationMode mode) :
    wxDialog(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 710,500 ), wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER, wxT("InstallationDetailsDialog"))
{
    SetTitle((mode == InstallationMode::INSTALL) ? wxT("Finjin Installation Details") : wxT("Finjin Uninstallation Details"));
#if defined(__WXMSW__)
    SetIcon(wxIcon(wxT("IDI_FINJIN"), wxBITMAP_TYPE_ICO_RESOURCE));
#endif
    SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* resultsSizer;
    resultsSizer = new wxBoxSizer( wxVERTICAL );

    resultsText = new wxTextCtrl( this, wxID_ANY, results, wxDefaultPosition, wxSize( 305,-1 ), wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY );
    resultsSizer->Add( resultsText, 1, wxALL|wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

    topSizer->Add( resultsSizer, 1, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );

    topSizer->Add( okCancelSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    this->SetSizer( topSizer );

    this->Layout();
    Centre( wxBOTH );
}

InstallationDetailsDialog::~InstallationDetailsDialog()
{
}

void InstallationDetailsDialog::OnOK(wxCommandEvent& event)
{
    event.Skip();
}

void InstallationDetailsDialog::OnCloseWindow(wxCloseEvent& event)
{
    EndModal(wxID_OK);
}
