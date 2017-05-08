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
#include "ProgressDialog.hpp"
#include "ApplicationControls.hpp"
#include "wxApplicationWindow.hpp"
#include "WindowPlacementManager.hpp"
#include "MathUtilities.hpp"
#include "wxApplicationWindow.hpp"
#include "Dialogs.hpp"

#if defined(__WXMAC__)
    #define DIALOG_WIDTH 598
    #define DIALOG_HEIGHT 418
#else
    #define DIALOG_WIDTH 498
    #define DIALOG_HEIGHT 318
#endif

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
ProgressDialog* ProgressDialog::instance = nullptr;


//Implementation----------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(ProgressDialog, FinjinDialog)

BEGIN_EVENT_TABLE(ProgressDialog, FinjinDialog)
    EVT_BUTTON(wxID_OK, ProgressDialog::OnOK)
END_EVENT_TABLE()

ProgressDialog::ProgressDialog(const wxString& title, LogMessageType logLevel, const wxString& totalProgressText) : LogListener(logLevel)
{
    this->totalProgressText = totalProgressText;

    //The parent of this dialog will be the application window, which isn't a real wxWindow, and will be deleted when the dialog is closed
    FinjinDialog::Create(new wxApplicationWindow(true), wxID_ANY, title, wxDefaultPosition, wxSize( DIALOG_WIDTH,DIALOG_HEIGHT ), wxSYSTEM_MENU|wxCAPTION|wxRESIZE_BORDER|wxMAXIMIZE_BOX, wxT("ProgressDialog"));

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* gaugeSizer;
    gaugeSizer = new wxBoxSizer(wxVERTICAL);

    totalProgressLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, totalProgressText, wxDefaultPosition, wxDefaultSize, 0 );
    totalProgressLabel->Wrap( -1 );
    gaugeSizer->Add( totalProgressLabel, 0, wxRIGHT|wxLEFT|wxTOP, 5 );

    totalProgressGauge = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
    gaugeSizer->Add( totalProgressGauge, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT|wxTOP, 5 );

    progressLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Current progress"), wxDefaultPosition, wxDefaultSize, 0 );
    progressLabel->Wrap( -1 );
    gaugeSizer->Add( progressLabel, 0, wxALL, 5 );

    progressGauge = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
    gaugeSizer->Add( progressGauge, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

    topSizer->Add( gaugeSizer, 0, wxEXPAND, 5 );

    progressText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,150 ), wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY );
    progressText->SetMinSize( wxSize( 365,150 ) );

    topSizer->Add( progressText, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );
    okButton->Show(false);

    topSizer->Add( okCancelSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    SetSizer(topSizer);

    if (!WindowPlacementManager::RestorePlacement(this))
    {
        Layout();
        Centre(wxBOTH);
    }

    const int PROGRESS_RANGE = 100;
    this->totalProgressGauge->SetRange(PROGRESS_RANGE);
    this->progressGauge->SetRange(PROGRESS_RANGE);

    instance = this;
    Logger::AddListener(this);
}

ProgressDialog::~ProgressDialog()
{
    instance = nullptr;
}

int ProgressDialog::ShowModal()
{
    ShowCloseButton(true);
    return FinjinDialog::ShowModal();
}

void ProgressDialog::FinishedTask(const wxString& totalProgressLabel)
{
    Logger::RemoveListener(this);

    UpdateProgress(1, 1);
    SetTotalProgressLabel(totalProgressLabel);
    ShowSubprogress(false);
    ShowCloseButton(true);
}

void ProgressDialog::SetTotalProgressLabel(const wxString& text)
{
    this->totalProgressText = text;
    this->totalProgressLabel->SetLabel(this->totalProgressText);
}

void ProgressDialog::SetSubprogressLabel(const wxString& text)
{
    this->progressLabel->SetLabel(text);

    //Set the total progress label again since it sometimes doesn't redraw properly when dialog is run
    //modelessly during an export
    this->totalProgressLabel->SetLabel(this->totalProgressText);
}

void ProgressDialog::UpdateProgress(float totalProgress, float subprogress)
{
    this->totalProgressGauge->SetValue(RoundToInt(totalProgress * this->totalProgressGauge->GetRange()));
    this->progressGauge->SetValue(RoundToInt(subprogress * this->progressGauge->GetRange()));
}

void ProgressDialog::ShowSubprogress(bool show)
{
    this->progressGauge->Show(show);
    this->progressLabel->Show(show);
    Layout();
}

void ProgressDialog::ShowCloseButton(bool show)
{
    this->okButton->Show(show);
    Layout();
}

void ProgressDialog::LogMessage(LogMessageType type, const wxString& message)
{
    wxString messageWithNewline = message + wxT("\n");
    this->progressText->AppendText(messageWithNewline);

    //Move cursor to the end position so that the window scrolls all the way down
    this->progressText->ShowPosition(this->progressText->GetLastPosition());
}

ProgressDialog* ProgressDialog::GetInstance()
{
    return instance;
}

void ProgressDialog::OnOK(wxCommandEvent& event)
{
    //Save position/size
    WindowPlacementManager::SavePlacement(this);

    //Delete parent "window", as described in the constructor
    auto parent = GetParent();
    Reparent(Dialogs::GetNullParent());
    if (parent != nullptr && parent != Dialogs::GetNullParent())
        parent->Destroy();

    if (IsModal())
        MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
    else
        MODELESS_DIALOG_ON_CLOSE(event)
}
