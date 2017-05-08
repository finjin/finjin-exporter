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


#include "stdafx.h"

#include "Dialogs.h"

///////////////////////////////////////////////////////////////////////////
using namespace Finjin;

InstallationDialogBase::InstallationDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxSize( 200,-1 ), wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* topModeSizer;
    topModeSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* modeCenterSizer;
    modeCenterSizer = new wxBoxSizer( wxHORIZONTAL );

    installRadioButton = new wxRadioButton( this, wxID_ANY, wxT("Install"), wxDefaultPosition, wxDefaultSize, 0 );
    modeCenterSizer->Add( installRadioButton, 0, wxALL, 5 );

    uninstallRadioButton = new wxRadioButton( this, wxID_ANY, wxT("Uninstall"), wxDefaultPosition, wxDefaultSize, 0 );
    modeCenterSizer->Add( uninstallRadioButton, 0, wxALL, 5 );


    topModeSizer->Add( modeCenterSizer, 0, wxALIGN_CENTER_HORIZONTAL, 0 );


    topSizer->Add( topModeSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 8 );

    wxStaticBoxSizer* aboutSizer;
    aboutSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("About") ), wxVERTICAL );

    aboutLabel = new wxStaticText( this, wxID_ANY, wxT("This tool will install the Finjin Exporter for each of the selected 3DS Max versions."), wxDefaultPosition, wxDefaultSize, 0 );
    aboutLabel->Wrap( -1 );
    aboutLabel->SetMinSize( wxSize( 170,40 ) );

    aboutSizer->Add( aboutLabel, 1, wxALL|wxEXPAND, 5 );


    topSizer->Add( aboutSizer, 0, wxEXPAND|wxLEFT|wxRIGHT, 8 );

    wxStaticBoxSizer* versionsSizer;
    versionsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("??? Versions to Install") ), wxVERTICAL );

    m_checkBox1 = new wxCheckBox( this, wxID_ANY, wxT("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
    versionsSizer->Add( m_checkBox1, 0, wxALL, 5 );

    m_checkBox2 = new wxCheckBox( this, wxID_ANY, wxT("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
    versionsSizer->Add( m_checkBox2, 0, wxALL, 5 );

    m_checkBox3 = new wxCheckBox( this, wxID_ANY, wxT("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
    versionsSizer->Add( m_checkBox3, 0, wxALL, 5 );

    m_checkBox4 = new wxCheckBox( this, wxID_ANY, wxT("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
    versionsSizer->Add( m_checkBox4, 0, wxALL, 5 );


    topSizer->Add( versionsSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 8 );

    wxStaticBoxSizer* otherFilesSizer;
    otherFilesSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Other Files to Install") ), wxVERTICAL );

    m_checkBox11 = new wxCheckBox( this, wxID_ANY, wxT("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
    otherFilesSizer->Add( m_checkBox11, 0, wxALL, 5 );

    m_checkBox21 = new wxCheckBox( this, wxID_ANY, wxT("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
    otherFilesSizer->Add( m_checkBox21, 0, wxALL, 5 );

    m_checkBox31 = new wxCheckBox( this, wxID_ANY, wxT("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
    otherFilesSizer->Add( m_checkBox31, 0, wxALL, 5 );

    m_checkBox41 = new wxCheckBox( this, wxID_ANY, wxT("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
    otherFilesSizer->Add( m_checkBox41, 0, wxALL, 5 );


    topSizer->Add( otherFilesSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 8 );

    wxBoxSizer* detailsSizer;
    detailsSizer = new wxBoxSizer( wxHORIZONTAL );

    showDetailsCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Show installation details"), wxDefaultPosition, wxDefaultSize, 0 );
    detailsSizer->Add( showDetailsCheckBox, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxTOP, 7 );


    topSizer->Add( detailsSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    wxBoxSizer* buttonsSizer;
    buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

    installButton = new wxButton( this, wxID_OK, wxT("&Install"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsSizer->Add( installButton, 0, wxBOTTOM|wxLEFT|wxRIGHT, 5 );

    closeButton = new wxButton( this, wxID_ANY, wxT("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsSizer->Add( closeButton, 0, wxBOTTOM|wxRIGHT, 5 );


    topSizer->Add( buttonsSizer, 0, wxALIGN_CENTER, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

InstallationDialogBase::~InstallationDialogBase()
{
}

AboutDialog::AboutDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    logoBitmap = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("IDB_ABOUT_LOGO"), wxBITMAP_TYPE_RESOURCE ), wxDefaultPosition, wxDefaultSize, 0 );
    logoBitmap->SetMinSize( wxSize( 320,100 ) );

    topSizer->Add( logoBitmap, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10 );

    wxBoxSizer* controlsSizer;
    controlsSizer = new wxBoxSizer( wxVERTICAL );

    descriptionText = new wxStaticText( this, wxID_ANY, wxT("Finjin Exporter for ???"), wxDefaultPosition, wxDefaultSize, 0 );
    descriptionText->Wrap( -1 );
    controlsSizer->Add( descriptionText, 0, wxALIGN_CENTER|wxALL, 5 );

    homeHyperlink = new wxHyperlinkCtrl( this, wxID_ANY, wxT("finjin.com"), wxT("http://finjin.com"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    controlsSizer->Add( homeHyperlink, 0, wxALIGN_CENTER|wxALL, 5 );

    githubHyperlink = new wxHyperlinkCtrl( this, wxID_ANY, wxT("github.com/finjin"), wxT("https://github.com/finjin"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    controlsSizer->Add( githubHyperlink, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    versionText = new wxStaticText( this, wxID_ANY, wxT("Version ? (? build for ? ?)"), wxDefaultPosition, wxDefaultSize, 0 );
    versionText->Wrap( -1 );
    controlsSizer->Add( versionText, 0, wxALIGN_CENTER|wxALL, 5 );

    notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0 );

    controlsSizer->Add( notebook, 1, wxEXPAND | wxALL, 5 );

    closeButton = new wxButton( this, wxID_OK, wxT("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    controlsSizer->Add( closeButton, 0, wxALIGN_CENTER|wxALL, 5 );


    topSizer->Add( controlsSizer, 1, wxEXPAND, 5 );


    this->SetSizer( topSizer );
    this->Layout();

    this->Centre( wxBOTH );
}

AboutDialog::~AboutDialog()
{
}

GlobalSettingsDialog::GlobalSettingsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    topSizer->Add( notebook, 1, wxEXPAND | wxALL, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

GlobalSettingsDialog::~GlobalSettingsDialog()
{
}

SceneSettingsDialog::SceneSettingsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    tabControl = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    topSizer->Add( tabControl, 0, wxALL|wxEXPAND, 5 );

    wxBoxSizer* tabPagesSizer;
    tabPagesSizer = new wxBoxSizer( wxVERTICAL );


    topSizer->Add( tabPagesSizer, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

    wxGridSizer* bottomSizer;
    bottomSizer = new wxGridSizer( 1, 1, 0, 0 );

    closeButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    bottomSizer->Add( closeButton, 0, wxALIGN_RIGHT|wxALL, 5 );


    topSizer->Add( bottomSizer, 0, wxEXPAND, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

SceneSettingsDialog::~SceneSettingsDialog()
{
}

UserDataSettingsDialog::UserDataSettingsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* pageSizer;
    pageSizer = new wxBoxSizer( wxVERTICAL );


    pageSizer->Add( 5, 5, 1, wxEXPAND, 5 );


    topSizer->Add( pageSizer, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

    wxGridSizer* bottomSizer;
    bottomSizer = new wxGridSizer( 1, 1, 0, 0 );

    closeButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    bottomSizer->Add( closeButton, 0, wxALIGN_RIGHT|wxALL, 5 );


    topSizer->Add( bottomSizer, 0, wxEXPAND, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

UserDataSettingsDialog::~UserDataSettingsDialog()
{
}

ObjectSettingsDialog::ObjectSettingsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    topSizer->Add( notebook, 1, wxEXPAND | wxALL, 5 );

    wxGridSizer* bottomSizer;
    bottomSizer = new wxGridSizer( 1, 1, 0, 0 );

    closeButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    bottomSizer->Add( closeButton, 0, wxALIGN_RIGHT|wxALL, 5 );


    topSizer->Add( bottomSizer, 0, wxEXPAND, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ObjectSettingsDialog::~ObjectSettingsDialog()
{
}

GlobalSettingsDialog_AnimationsPage::GlobalSettingsDialog_AnimationsPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* meshAnimationsSizer;
    meshAnimationsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Animation Sampling") ), wxVERTICAL );

    wxGridSizer* skeletonAndVertexanimationSamplingSizer;
    skeletonAndVertexanimationSamplingSizer = new wxGridSizer( 1, 2, 0, 0 );

    wxBoxSizer* skeletonAnimationSamplingSizer;
    skeletonAnimationSamplingSizer = new wxBoxSizer( wxVERTICAL );

    skeletonAnimationSamplingLabel = new wxStaticText( this, wxID_ANY, wxT("Skeleton Animation Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    skeletonAnimationSamplingLabel->Wrap( -1 );
    skeletonAnimationSamplingSizer->Add( skeletonAnimationSamplingLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* skeletonAnimationSamplingValuesSizer;
    skeletonAnimationSamplingValuesSizer = new wxBoxSizer( wxHORIZONTAL );

    skeletonAnimationSamplingText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    skeletonAnimationSamplingText->SetMaxLength( 0 );
    skeletonAnimationSamplingValuesSizer->Add( skeletonAnimationSamplingText, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    wxArrayString skeletonAnimationSamplingChoiceChoices;
    skeletonAnimationSamplingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, skeletonAnimationSamplingChoiceChoices, 0 );
    skeletonAnimationSamplingChoice->SetSelection( 0 );
    skeletonAnimationSamplingValuesSizer->Add( skeletonAnimationSamplingChoice, 1, wxALL, 5 );


    skeletonAnimationSamplingSizer->Add( skeletonAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );


    skeletonAndVertexanimationSamplingSizer->Add( skeletonAnimationSamplingSizer, 1, wxEXPAND, 5 );

    wxBoxSizer* vertexAnimationSamplingSizer;
    vertexAnimationSamplingSizer = new wxBoxSizer( wxVERTICAL );

    vertexAnimationSamplingLabel = new wxStaticText( this, wxID_ANY, wxT("Vertex Animation Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    vertexAnimationSamplingLabel->Wrap( -1 );
    vertexAnimationSamplingSizer->Add( vertexAnimationSamplingLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* vertexAnimationSamplingValuesSizer;
    vertexAnimationSamplingValuesSizer = new wxBoxSizer( wxHORIZONTAL );

    vertexAnimationSamplingText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    vertexAnimationSamplingText->SetMaxLength( 0 );
    vertexAnimationSamplingValuesSizer->Add( vertexAnimationSamplingText, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    wxArrayString vertexAnimationSamplingChoiceChoices;
    vertexAnimationSamplingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, vertexAnimationSamplingChoiceChoices, 0 );
    vertexAnimationSamplingChoice->SetSelection( 0 );
    vertexAnimationSamplingValuesSizer->Add( vertexAnimationSamplingChoice, 1, wxALL, 5 );


    vertexAnimationSamplingSizer->Add( vertexAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );


    skeletonAndVertexanimationSamplingSizer->Add( vertexAnimationSamplingSizer, 1, wxEXPAND, 5 );


    meshAnimationsSizer->Add( skeletonAndVertexanimationSamplingSizer, 1, wxEXPAND, 5 );

    wxBoxSizer* nodeAnimationSamplingSizer;
    nodeAnimationSamplingSizer = new wxBoxSizer( wxVERTICAL );

    nodeAnimationSamplingLabel = new wxStaticText( this, wxID_ANY, wxT("Node Animation Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    nodeAnimationSamplingLabel->Wrap( -1 );
    nodeAnimationSamplingSizer->Add( nodeAnimationSamplingLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* nodeAnimationSamplingValuesSizer;
    nodeAnimationSamplingValuesSizer = new wxBoxSizer( wxHORIZONTAL );

    nodeAnimationSamplingText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    nodeAnimationSamplingText->SetMaxLength( 0 );
    nodeAnimationSamplingValuesSizer->Add( nodeAnimationSamplingText, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    wxArrayString nodeAnimationSamplingChoiceChoices;
    nodeAnimationSamplingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, nodeAnimationSamplingChoiceChoices, 0 );
    nodeAnimationSamplingChoice->SetSelection( 0 );
    nodeAnimationSamplingValuesSizer->Add( nodeAnimationSamplingChoice, 1, wxALL, 5 );


    nodeAnimationSamplingSizer->Add( nodeAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );


    meshAnimationsSizer->Add( nodeAnimationSamplingSizer, 1, wxEXPAND, 5 );


    topSizer->Add( meshAnimationsSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

GlobalSettingsDialog_AnimationsPage::~GlobalSettingsDialog_AnimationsPage()
{
}

GlobalSettingsDialog_FlagsPage::GlobalSettingsDialog_FlagsPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* aliasesSizer;
    aliasesSizer = new wxBoxSizer( wxHORIZONTAL );

    wxStaticBoxSizer* innerAliasesSizer;
    innerAliasesSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Flag Names") ), wxVERTICAL );

    wxGridSizer* queryFlagAliasesControlsSizer;
    queryFlagAliasesControlsSizer = new wxGridSizer( 1, 2, 0, 0 );

    addButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxDefaultSize, 0 );
    queryFlagAliasesControlsSizer->Add( addButton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5 );


    innerAliasesSizer->Add( queryFlagAliasesControlsSizer, 0, wxEXPAND, 5 );

    flagsText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
    flagsText->SetMaxLength( 0 );
    innerAliasesSizer->Add( flagsText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    aliasesSizer->Add( innerAliasesSizer, 1, wxEXPAND|wxRIGHT, 5 );


    topSizer->Add( aliasesSizer, 1, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

GlobalSettingsDialog_FlagsPage::~GlobalSettingsDialog_FlagsPage()
{
}

GlobalSettingsDialog_GeneralPage::GlobalSettingsDialog_GeneralPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* exportFormatSizer;
    exportFormatSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("File Format") ), wxVERTICAL );

    wxBoxSizer* textBlobFormatHorizontalSizer;
    textBlobFormatHorizontalSizer = new wxBoxSizer( wxHORIZONTAL );

    textBlobFormatLabel = new wxStaticText( this, wxID_ANY, wxT("Text File Blob Format"), wxDefaultPosition, wxSize( 140,-1 ), 0 );
    textBlobFormatLabel->Wrap( -1 );
    textBlobFormatHorizontalSizer->Add( textBlobFormatLabel, 0, wxALL, 5 );

    wxArrayString textBlobFormatChoiceChoices;
    textBlobFormatChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, textBlobFormatChoiceChoices, 0 );
    textBlobFormatChoice->SetSelection( 0 );
    textBlobFormatChoice->SetMinSize( wxSize( 80,-1 ) );

    textBlobFormatHorizontalSizer->Add( textBlobFormatChoice, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    blobTextFormatDefaultLabel = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
    blobTextFormatDefaultLabel->Wrap( -1 );
    textBlobFormatHorizontalSizer->Add( blobTextFormatDefaultLabel, 0, wxALL, 5 );


    exportFormatSizer->Add( textBlobFormatHorizontalSizer, 0, wxEXPAND, 5 );

    wxBoxSizer* maxBytesPerLineSizer;
    maxBytesPerLineSizer = new wxBoxSizer( wxHORIZONTAL );

    maxBytesPerLineLabel = new wxStaticText( this, wxID_ANY, wxT("Maximum Bytes Per Line"), wxDefaultPosition, wxSize( 140,-1 ), 0 );
    maxBytesPerLineLabel->Wrap( -1 );
    maxBytesPerLineSizer->Add( maxBytesPerLineLabel, 0, wxALL, 5 );

    maxBytesPerLineSpinner = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    maxBytesPerLineSpinner->SetMaxLength( 0 );
    maxBytesPerLineSizer->Add( maxBytesPerLineSpinner, 0, wxBOTTOM|wxLEFT|wxRIGHT, 5 );


    exportFormatSizer->Add( maxBytesPerLineSizer, 0, wxEXPAND, 5 );

    wxBoxSizer* alwaysExportSizer;
    alwaysExportSizer = new wxBoxSizer( wxHORIZONTAL );

    alwaysExportLabel = new wxStaticText( this, wxID_ANY, wxT("Additional Export Formats"), wxDefaultPosition, wxSize( 140,-1 ), 0 );
    alwaysExportLabel->Wrap( -1 );
    alwaysExportSizer->Add( alwaysExportLabel, 0, wxALL, 5 );

    alwaysExportTextCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Text (.fstd)"), wxDefaultPosition, wxDefaultSize, 0 );
    alwaysExportTextCheckbox->SetValue(true);
    alwaysExportSizer->Add( alwaysExportTextCheckbox, 0, wxALL, 5 );

    alwaysExportBinaryCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Binary (.fsbd)"), wxDefaultPosition, wxDefaultSize, 0 );
    alwaysExportBinaryCheckbox->SetValue(true);
    alwaysExportSizer->Add( alwaysExportBinaryCheckbox, 0, wxALL, 5 );

    alwaysExportConfigCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Config (.cfg)"), wxDefaultPosition, wxDefaultSize, 0 );
    alwaysExportConfigCheckbox->SetValue(true);
    alwaysExportSizer->Add( alwaysExportConfigCheckbox, 0, wxALL, 5 );

    alwaysExportJsonCheckbox = new wxCheckBox( this, wxID_ANY, wxT("JSON (.json)"), wxDefaultPosition, wxDefaultSize, 0 );
    alwaysExportJsonCheckbox->SetValue(true);
    alwaysExportSizer->Add( alwaysExportJsonCheckbox, 0, wxALL, 5 );


    exportFormatSizer->Add( alwaysExportSizer, 1, wxEXPAND, 5 );

    wxBoxSizer* embedSizer;
    embedSizer = new wxBoxSizer( wxHORIZONTAL );

    embedLabel = new wxStaticText( this, wxID_ANY, wxT("Embed Files"), wxDefaultPosition, wxSize( 140,-1 ), 0 );
    embedLabel->Wrap( -1 );
    embedSizer->Add( embedLabel, 0, wxALL, 5 );

    embedMeshesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Meshes"), wxDefaultPosition, wxDefaultSize, 0 );
    embedMeshesCheckbox->SetValue(true);
    embedSizer->Add( embedMeshesCheckbox, 0, wxALL, 5 );

    embedMaterialsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Materials"), wxDefaultPosition, wxDefaultSize, 0 );
    embedMaterialsCheckbox->SetValue(true);
    embedSizer->Add( embedMaterialsCheckbox, 0, wxALL, 5 );

    embedTexturesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Bitmaps / Textures"), wxDefaultPosition, wxDefaultSize, 0 );
    embedTexturesCheckbox->SetValue(true);
    embedSizer->Add( embedTexturesCheckbox, 0, wxALL, 5 );

    embedPrefabsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Prefabs"), wxDefaultPosition, wxDefaultSize, 0 );
    embedPrefabsCheckbox->SetValue(true);
    embedSizer->Add( embedPrefabsCheckbox, 0, wxALL, 5 );


    exportFormatSizer->Add( embedSizer, 1, wxEXPAND, 5 );

    wxBoxSizer* headerExportSizer;
    headerExportSizer = new wxBoxSizer( wxHORIZONTAL );

    exportLabel = new wxStaticText( this, wxID_ANY, wxT("Export"), wxDefaultPosition, wxSize( 140,-1 ), 0 );
    exportLabel->Wrap( -1 );
    headerExportSizer->Add( exportLabel, 0, wxALL, 5 );

    exportApplicationNameCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Application Name"), wxDefaultPosition, wxDefaultSize, 0 );
    exportApplicationNameCheckbox->SetValue(true);
    headerExportSizer->Add( exportApplicationNameCheckbox, 0, wxALL, 5 );

    exportAuthorCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Author"), wxDefaultPosition, wxDefaultSize, 0 );
    exportAuthorCheckbox->SetValue(true);
    headerExportSizer->Add( exportAuthorCheckbox, 0, wxALL, 5 );

    exportTimestampCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Time"), wxDefaultPosition, wxDefaultSize, 0 );
    exportTimestampCheckbox->SetValue(true);
    headerExportSizer->Add( exportTimestampCheckbox, 0, wxALL|wxEXPAND, 5 );

    exporterVersionCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Exporter Version"), wxDefaultPosition, wxDefaultSize, 0 );
    exporterVersionCheckbox->SetValue(true);
    headerExportSizer->Add( exporterVersionCheckbox, 0, wxALL, 5 );


    exportFormatSizer->Add( headerExportSizer, 1, wxEXPAND, 5 );


    topSizer->Add( exportFormatSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxStaticBoxSizer* exportDirectoryTopSizer;
    exportDirectoryTopSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Export Directory") ), wxVERTICAL );

    exportDirectoryDescriptionLabel = new wxStaticText( this, wxID_ANY, wxT("The export directory contains all of the mesh, material and other asset directories that are generated\nwhen a scene is exported. By default it is the same as the exported scene directory, but you may\nadd a path relative to that directory."), wxDefaultPosition, wxDefaultSize, 0 );
    exportDirectoryDescriptionLabel->Wrap( -1 );
    exportDirectoryTopSizer->Add( exportDirectoryDescriptionLabel, 0, wxALL|wxEXPAND, 5 );

    wxBoxSizer* exportDirectorySizer;
    exportDirectorySizer = new wxBoxSizer( wxHORIZONTAL );

    exportDirectoryLabel = new wxStaticText( this, wxID_ANY, wxT("Directory"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
    exportDirectoryLabel->Wrap( -1 );
    exportDirectorySizer->Add( exportDirectoryLabel, 0, wxALL, 5 );

    exportDirectoryText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 440,-1 ), 0 );
    exportDirectoryText->SetMaxLength( 0 );
    exportDirectorySizer->Add( exportDirectoryText, 0, wxLEFT|wxRIGHT, 5 );


    exportDirectoryTopSizer->Add( exportDirectorySizer, 1, wxEXPAND, 5 );


    topSizer->Add( exportDirectoryTopSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxStaticBoxSizer* externalViewerSettingsSizer;
    externalViewerSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("External Viewer") ), wxVERTICAL );

    finjinViewerButton = new wxRadioButton( this, wxID_ANY, wxT("Finjin Viewer"), wxDefaultPosition, wxDefaultSize, wxRB_SINGLE );
    externalViewerSettingsSizer->Add( finjinViewerButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


    externalViewerSettingsSizer->Add( 0, 4, 1, wxEXPAND, 5 );

    runExternalViewerAfterExportCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Run After Export"), wxDefaultPosition, wxDefaultSize, 0 );
    externalViewerSettingsSizer->Add( runExternalViewerAfterExportCheckbox, 0, wxALL, 5 );


    topSizer->Add( externalViewerSettingsSizer, 0, wxALL|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

GlobalSettingsDialog_GeneralPage::~GlobalSettingsDialog_GeneralPage()
{
}

GlobalSettingsDialog_MaterialsPage::GlobalSettingsDialog_MaterialsPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* materialNamingSizer;
    materialNamingSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Materials") ), wxVERTICAL );

    wxFlexGridSizer* materialNamingGridSizer;
    materialNamingGridSizer = new wxFlexGridSizer( 1, 3, 0, 0 );
    materialNamingGridSizer->SetFlexibleDirection( wxBOTH );
    materialNamingGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    noMaterialNameLabel = new wxStaticText( this, wxID_ANY, wxT("\"No Material\" Name"), wxDefaultPosition, wxDefaultSize, 0 );
    noMaterialNameLabel->Wrap( -1 );
    materialNamingGridSizer->Add( noMaterialNameLabel, 0, wxALIGN_TOP|wxALL, 5 );

    noMaterialNameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), 0 );
    noMaterialNameText->SetMaxLength( 0 );
    materialNamingGridSizer->Add( noMaterialNameText, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    noMaterialNameDescriptionLabel = new wxStaticText( this, wxID_ANY, wxT("Name of the default material assigned to\nobjects that have no material."), wxDefaultPosition, wxDefaultSize, 0 );
    noMaterialNameDescriptionLabel->Wrap( -1 );
    materialNamingGridSizer->Add( noMaterialNameDescriptionLabel, 0, wxRIGHT, 5 );


    materialNamingSizer->Add( materialNamingGridSizer, 1, wxEXPAND, 5 );

    includeParentMaterialInNameCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Include Parent Material in Name"), wxDefaultPosition, wxDefaultSize, 0 );
    materialNamingSizer->Add( includeParentMaterialInNameCheckbox, 0, wxALL, 5 );

    createExtraMaterialsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Create Extra Materials if Necessary"), wxDefaultPosition, wxDefaultSize, 0 );
    materialNamingSizer->Add( createExtraMaterialsCheckbox, 0, wxALL, 5 );


    topSizer->Add( materialNamingSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxStaticBoxSizer* bitmapExportControlSizer;
    bitmapExportControlSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Bitmaps") ), wxVERTICAL );

    copyBitmapsToExportDirectoryCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Copy Bitmaps to Export Directory"), wxDefaultPosition, wxDefaultSize, 0 );
    bitmapExportControlSizer->Add( copyBitmapsToExportDirectoryCheckbox, 0, wxALL, 5 );

    convertBitmapsToTexturesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Convert Bitmaps to Textures (.texture)"), wxDefaultPosition, wxDefaultSize, 0 );
    bitmapExportControlSizer->Add( convertBitmapsToTexturesCheckbox, 0, wxALL, 5 );


    topSizer->Add( bitmapExportControlSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

GlobalSettingsDialog_MaterialsPage::~GlobalSettingsDialog_MaterialsPage()
{
}

GlobalSettingsDialog_MeshesPage::GlobalSettingsDialog_MeshesPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* meshNamingSizer;
    meshNamingSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Meshes") ), wxVERTICAL );

    wxFlexGridSizer* meshNamingGridSizer;
    meshNamingGridSizer = new wxFlexGridSizer( 1, 2, 0, 0 );
    meshNamingGridSizer->SetFlexibleDirection( wxBOTH );
    meshNamingGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    submeshNamingLabel = new wxStaticText( this, wxID_ANY, wxT("Submesh Naming"), wxDefaultPosition, wxDefaultSize, 0 );
    submeshNamingLabel->Wrap( -1 );
    meshNamingGridSizer->Add( submeshNamingLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer* submeshNamingSizer;
    submeshNamingSizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString submeshNamingChoiceChoices;
    submeshNamingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), submeshNamingChoiceChoices, 0 );
    submeshNamingChoice->SetSelection( 0 );
    submeshNamingSizer->Add( submeshNamingChoice, 0, wxLEFT|wxRIGHT, 5 );

    submeshNamingCustomText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    submeshNamingCustomText->SetMaxLength( 0 );
    submeshNamingSizer->Add( submeshNamingCustomText, 0, 0, 5 );


    meshNamingGridSizer->Add( submeshNamingSizer, 1, wxEXPAND, 5 );


    meshNamingSizer->Add( meshNamingGridSizer, 0, wxEXPAND, 5 );

    alwaysExportVertexFormatInMeshCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Always Export Vertex Format"), wxDefaultPosition, wxDefaultSize, 0 );
    meshNamingSizer->Add( alwaysExportVertexFormatInMeshCheckbox, 0, wxALL, 5 );


    topSizer->Add( meshNamingSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxStaticBoxSizer* vertexFileLocationsTopSizer;
    vertexFileLocationsTopSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Vertex Format File") ), wxVERTICAL );

    baseDirectoryLabel = new wxStaticText( this, wxID_ANY, wxT("Base Directory"), wxDefaultPosition, wxDefaultSize, 0 );
    baseDirectoryLabel->Wrap( -1 );
    vertexFileLocationsTopSizer->Add( baseDirectoryLabel, 0, wxALL, 5 );

    wxBoxSizer* baseDirectorySizer;
    baseDirectorySizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString baseDirectoryChoiceChoices;
    baseDirectoryChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, baseDirectoryChoiceChoices, 0 );
    baseDirectoryChoice->SetSelection( 0 );
    baseDirectorySizer->Add( baseDirectoryChoice, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    baseDirectoryText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 285,-1 ), wxTE_READONLY );
    baseDirectoryText->SetMaxLength( 0 );
    baseDirectorySizer->Add( baseDirectoryText, 1, wxBOTTOM, 5 );

    baseDirectoryButton = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxSize( 100,-1 ), 0 );
    baseDirectorySizer->Add( baseDirectoryButton, 0, wxBOTTOM|wxLEFT, 5 );


    vertexFileLocationsTopSizer->Add( baseDirectorySizer, 0, wxEXPAND, 5 );

    wxBoxSizer* vertexFormatFileSizer;
    vertexFormatFileSizer = new wxBoxSizer( wxHORIZONTAL );

    vertexFormatFileLabel = new wxStaticText( this, wxID_ANY, wxT("File"), wxDefaultPosition, wxDefaultSize, 0 );
    vertexFormatFileLabel->Wrap( -1 );
    vertexFormatFileSizer->Add( vertexFormatFileLabel, 0, wxALL, 5 );

    vertexFormatFileText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 285,-1 ), 0 );
    vertexFormatFileText->SetMaxLength( 0 );
    vertexFormatFileSizer->Add( vertexFormatFileText, 1, wxBOTTOM|wxLEFT, 5 );

    vertexFormatFileButton = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxSize( 100,-1 ), 0 );
    vertexFormatFileSizer->Add( vertexFormatFileButton, 0, wxBOTTOM|wxLEFT, 5 );


    vertexFileLocationsTopSizer->Add( vertexFormatFileSizer, 0, wxEXPAND, 5 );


    topSizer->Add( vertexFileLocationsTopSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

GlobalSettingsDialog_MeshesPage::~GlobalSettingsDialog_MeshesPage()
{
}

GlobalSettingsDialog_ScenePage::GlobalSettingsDialog_ScenePage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* sceneDefaultsSizer;
    sceneDefaultsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Scene Defaults") ), wxVERTICAL );

    wxBoxSizer* sceneScaleSizer;
    sceneScaleSizer = new wxBoxSizer( wxHORIZONTAL );

    sceneScaleLabel = new wxStaticText( this, wxID_ANY, wxT("Scale"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
    sceneScaleLabel->Wrap( -1 );
    sceneScaleSizer->Add( sceneScaleLabel, 0, wxALL, 5 );

    wxArrayString sceneScaleChoiceChoices;
    sceneScaleChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, sceneScaleChoiceChoices, 0 );
    sceneScaleChoice->SetSelection( 0 );
    sceneScaleSizer->Add( sceneScaleChoice, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    divideByLabel = new wxStaticText( this, wxID_ANY, wxT("Divided By"), wxDefaultPosition, wxDefaultSize, 0 );
    divideByLabel->Wrap( -1 );
    sceneScaleSizer->Add( divideByLabel, 0, wxALL, 5 );

    dividedBySpinner = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    dividedBySpinner->SetMaxLength( 0 );
    sceneScaleSizer->Add( dividedBySpinner, 0, wxBOTTOM|wxLEFT|wxRIGHT, 5 );


    sceneDefaultsSizer->Add( sceneScaleSizer, 0, wxEXPAND, 5 );


    topSizer->Add( sceneDefaultsSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

GlobalSettingsDialog_ScenePage::~GlobalSettingsDialog_ScenePage()
{
}

GlobalSettingsDialog_UserDataClassPage::GlobalSettingsDialog_UserDataClassPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* resourceLocationsTopSizer;
    resourceLocationsTopSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("File Locations") ), wxVERTICAL );

    baseDirectoryLabel = new wxStaticText( this, wxID_ANY, wxT("Base Directory"), wxDefaultPosition, wxDefaultSize, 0 );
    baseDirectoryLabel->Wrap( -1 );
    resourceLocationsTopSizer->Add( baseDirectoryLabel, 0, wxALL, 5 );

    wxBoxSizer* baseDirectorySizer;
    baseDirectorySizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString baseDirectoryChoiceChoices;
    baseDirectoryChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, baseDirectoryChoiceChoices, 0 );
    baseDirectoryChoice->SetSelection( 0 );
    baseDirectorySizer->Add( baseDirectoryChoice, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    baseDirectoryText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 285,-1 ), wxTE_READONLY );
    baseDirectoryText->SetMaxLength( 0 );
    baseDirectorySizer->Add( baseDirectoryText, 1, wxBOTTOM, 5 );

    baseDirectoryButton = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxSize( 100,-1 ), 0 );
    baseDirectorySizer->Add( baseDirectoryButton, 0, wxBOTTOM|wxLEFT, 5 );


    resourceLocationsTopSizer->Add( baseDirectorySizer, 0, wxEXPAND, 5 );

    resourceLocationsDescriptionLabel = new wxStaticText( this, wxID_ANY, wxT("These locations are used to locate files that contain user data classes, which can be used to \nconfigure object and scene user data. Directories will be searched non-recursively for files with \nthe .cfg-userdatatypes extension."), wxDefaultPosition, wxDefaultSize, 0 );
    resourceLocationsDescriptionLabel->Wrap( -1 );
    resourceLocationsTopSizer->Add( resourceLocationsDescriptionLabel, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

    wxBoxSizer* resourceLocationsSizer;
    resourceLocationsSizer = new wxBoxSizer( wxHORIZONTAL );

    resourceLocationsText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_DONTWRAP|wxTE_MULTILINE );
    resourceLocationsText->SetMaxLength( 0 );
    resourceLocationsText->SetMinSize( wxSize( 365,150 ) );

    resourceLocationsSizer->Add( resourceLocationsText, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxTOP, 5 );

    wxBoxSizer* resourceLocationsButtonsSizer;
    resourceLocationsButtonsSizer = new wxBoxSizer( wxVERTICAL );

    addDirectoryButton = new wxButton( this, wxID_ANY, wxT("Add Directory..."), wxDefaultPosition, wxSize( 100,-1 ), 0 );
    resourceLocationsButtonsSizer->Add( addDirectoryButton, 0, wxTOP|wxLEFT, 5 );

    addFileButton = new wxButton( this, wxID_ANY, wxT("Add File..."), wxDefaultPosition, wxSize( 100,-1 ), 0 );
    resourceLocationsButtonsSizer->Add( addFileButton, 0, wxBOTTOM|wxLEFT, 5 );


    resourceLocationsSizer->Add( resourceLocationsButtonsSizer, 0, wxEXPAND, 5 );


    resourceLocationsTopSizer->Add( resourceLocationsSizer, 1, wxEXPAND, 5 );


    topSizer->Add( resourceLocationsTopSizer, 1, wxEXPAND, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

GlobalSettingsDialog_UserDataClassPage::~GlobalSettingsDialog_UserDataClassPage()
{
}

ObjectSettingsDialog_GeneralPage::ObjectSettingsDialog_GeneralPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* typeSizer;
    typeSizer = new wxBoxSizer( wxHORIZONTAL );

    typeLabel = new wxStaticText( this, wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0 );
    typeLabel->Wrap( -1 );
    typeSizer->Add( typeLabel, 0, wxALL, 5 );

    wxArrayString typeChoiceChoices;
    typeChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, typeChoiceChoices, 0 );
    typeChoice->SetSelection( 0 );
    typeSizer->Add( typeChoice, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    defaultTypeLabel = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    defaultTypeLabel->Wrap( -1 );
    typeSizer->Add( defaultTypeLabel, 0, wxALL, 5 );

    isPrefabCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export as Prefab"), wxDefaultPosition, wxDefaultSize, 0 );
    typeSizer->Add( isPrefabCheckbox, 0, wxALL, 5 );

    isPrefabInstanceCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Also Export as Prefab Instance"), wxDefaultPosition, wxDefaultSize, 0 );
    typeSizer->Add( isPrefabInstanceCheckbox, 0, wxALL, 5 );


    topSizer->Add( typeSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* renderingSizer;
    renderingSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Rendering") ), wxVERTICAL );

    wxFlexGridSizer* renderingGridSizer;
    renderingGridSizer = new wxFlexGridSizer( 3, 2, 0, 0 );
    renderingGridSizer->SetFlexibleDirection( wxBOTH );
    renderingGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    visibilityLabel = new wxStaticText( this, wxID_ANY, wxT("Visibility"), wxDefaultPosition, wxDefaultSize, 0 );
    visibilityLabel->Wrap( -1 );
    renderingGridSizer->Add( visibilityLabel, 0, wxALL, 5 );

    wxBoxSizer* visibilitySizer;
    visibilitySizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString visibilityChoiceChoices;
    visibilityChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, visibilityChoiceChoices, 0 );
    visibilityChoice->SetSelection( 0 );
    visibilitySizer->Add( visibilityChoice, 0, wxRIGHT|wxLEFT, 5 );

    visibilityAffectObjectOnlyCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Affect Object Only"), wxDefaultPosition, wxDefaultSize, 0 );
    visibilitySizer->Add( visibilityAffectObjectOnlyCheckbox, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );


    renderingGridSizer->Add( visibilitySizer, 1, wxEXPAND, 5 );

    renderQueueLabel = new wxStaticText( this, wxID_ANY, wxT("Render Queue"), wxDefaultPosition, wxDefaultSize, 0 );
    renderQueueLabel->Wrap( -1 );
    renderingGridSizer->Add( renderQueueLabel, 0, wxALL, 5 );

    wxBoxSizer* renderQueueSizer;
    renderQueueSizer = new wxBoxSizer( wxHORIZONTAL );

    renderQueueText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    renderQueueText->SetMaxLength( 0 );
    renderQueueSizer->Add( renderQueueText, 0, wxALL, 5 );

    renderQueuePriorityLabel = new wxStaticText( this, wxID_ANY, wxT("Priority"), wxDefaultPosition, wxDefaultSize, 0 );
    renderQueuePriorityLabel->Wrap( -1 );
    renderQueueSizer->Add( renderQueuePriorityLabel, 0, wxALL, 5 );

    renderQueuePriorityText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    renderQueuePriorityText->SetMaxLength( 0 );
    renderQueueSizer->Add( renderQueuePriorityText, 0, wxRIGHT|wxLEFT, 5 );


    renderingGridSizer->Add( renderQueueSizer, 1, wxEXPAND, 5 );

    renderDistanceLabel = new wxStaticText( this, wxID_ANY, wxT("Render Distance"), wxDefaultPosition, wxDefaultSize, 0 );
    renderDistanceLabel->Wrap( -1 );
    renderingGridSizer->Add( renderDistanceLabel, 0, wxALL, 5 );

    wxBoxSizer* renderDistanceSizer;
    renderDistanceSizer = new wxBoxSizer( wxHORIZONTAL );

    renderDistanceText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    renderDistanceText->SetMaxLength( 0 );
    renderDistanceSizer->Add( renderDistanceText, 0, wxRIGHT|wxLEFT, 5 );


    renderingGridSizer->Add( renderDistanceSizer, 1, wxEXPAND, 5 );


    renderingSizer->Add( renderingGridSizer, 0, wxEXPAND, 5 );


    topSizer->Add( renderingSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* hierarchySizer;
    hierarchySizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Hierarchy") ), wxVERTICAL );

    wxBoxSizer* exportedParentSizer;
    exportedParentSizer = new wxBoxSizer( wxHORIZONTAL );

    exportedParentLabel = new wxStaticText( this, wxID_ANY, wxT("Exported Parent"), wxDefaultPosition, wxDefaultSize, 0 );
    exportedParentLabel->Wrap( -1 );
    exportedParentSizer->Add( exportedParentLabel, 0, wxALL, 5 );

    exportedParentText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 180,-1 ), wxTE_READONLY );
    exportedParentText->SetMaxLength( 0 );
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

    sortChildrenLabel = new wxStaticText( this, wxID_ANY, wxT("Sort Children By"), wxDefaultPosition, wxDefaultSize, 0 );
    sortChildrenLabel->Wrap( -1 );
    childrenSizer->Add( sortChildrenLabel, 0, wxALL, 5 );

    wxArrayString sortChildrenChoiceChoices;
    sortChildrenChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, sortChildrenChoiceChoices, 0 );
    sortChildrenChoice->SetSelection( 0 );
    childrenSizer->Add( sortChildrenChoice, 0, wxRIGHT, 5 );

    childOrderLabel = new wxStaticText( this, wxID_ANY, wxT("Order"), wxDefaultPosition, wxDefaultSize, 0 );
    childOrderLabel->Wrap( -1 );
    childrenSizer->Add( childOrderLabel, 0, wxALL, 5 );

    childOrderText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    childOrderText->SetMaxLength( 0 );
    childrenSizer->Add( childOrderText, 0, wxRIGHT, 5 );


    hierarchySizer->Add( childrenSizer, 1, wxEXPAND, 5 );


    topSizer->Add( hierarchySizer, 0, wxEXPAND|wxTOP, 5 );

    noteTrackNamesSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Note Track Names (One name per line, per note track)") ), wxVERTICAL );

    noteTrackNamesText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,80 ), wxTE_DONTWRAP|wxTE_MULTILINE );
    noteTrackNamesText->SetMaxLength( 0 );
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

    flagsText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_DONTWRAP|wxTE_MULTILINE );
    flagsText->SetMaxLength( 0 );
    flagsText->SetMinSize( wxSize( -1,60 ) );

    queryFlagsSizer->Add( flagsText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    flagsSizer->Add( queryFlagsSizer, 1, wxEXPAND|wxRIGHT, 5 );


    topSizer->Add( flagsSizer, 1, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ObjectSettingsDialog_GeneralPage::~ObjectSettingsDialog_GeneralPage()
{
}

ObjectSettingsDialog_MeshPage::ObjectSettingsDialog_MeshPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* meshSettingsSizer;
    meshSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("General Settings") ), wxVERTICAL );

    wxFlexGridSizer* meshSettingsGridSizer;
    meshSettingsGridSizer = new wxFlexGridSizer( 4, 2, 0, 0 );
    meshSettingsGridSizer->SetFlexibleDirection( wxBOTH );
    meshSettingsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    meshNameLabel = new wxStaticText( this, wxID_ANY, wxT("Mesh Name"), wxDefaultPosition, wxDefaultSize, 0 );
    meshNameLabel->Wrap( -1 );
    meshSettingsGridSizer->Add( meshNameLabel, 0, wxALL, 5 );

    meshNameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), 0 );
    meshNameText->SetMaxLength( 0 );
    meshSettingsGridSizer->Add( meshNameText, 0, wxRIGHT|wxLEFT, 5 );

    primitiveTypeLabel = new wxStaticText( this, wxID_ANY, wxT("Primitive Type"), wxDefaultPosition, wxDefaultSize, 0 );
    primitiveTypeLabel->Wrap( -1 );
    meshSettingsGridSizer->Add( primitiveTypeLabel, 0, wxALL, 5 );

    wxArrayString primitiveTypeChoiceChoices;
    primitiveTypeChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 140,-1 ), primitiveTypeChoiceChoices, 0 );
    primitiveTypeChoice->SetSelection( 0 );
    meshSettingsGridSizer->Add( primitiveTypeChoice, 0, wxRIGHT|wxLEFT, 5 );

    pointSizeLabel = new wxStaticText( this, wxID_ANY, wxT("Point Size"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
    pointSizeLabel->Wrap( -1 );
    meshSettingsGridSizer->Add( pointSizeLabel, 0, wxALL, 5 );

    pointSizeSpinner = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), 0 );
    pointSizeSpinner->SetMaxLength( 0 );
    meshSettingsGridSizer->Add( pointSizeSpinner, 0, wxBOTTOM|wxLEFT|wxRIGHT, 5 );


    meshSettingsSizer->Add( meshSettingsGridSizer, 1, wxEXPAND, 5 );

    exportVertexColorsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Vertex Colors"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsSizer->Add( exportVertexColorsCheckbox, 0, wxALL, 5 );

    animateBoundingVolumesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Animated Bounding Volumes"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsSizer->Add( animateBoundingVolumesCheckbox, 0, wxALL, 5 );

    checkMeshInstanceCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Check for Mesh Instance"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsSizer->Add( checkMeshInstanceCheckbox, 0, wxALL, 5 );


    topSizer->Add( meshSettingsSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* tangentBinormalSettingsSizer;
    tangentBinormalSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Tangent/Binormal Settings") ), wxVERTICAL );

    wxFlexGridSizer* tangentBinormalSettingsGridSizer;
    tangentBinormalSettingsGridSizer = new wxFlexGridSizer( 6, 2, 0, 0 );
    tangentBinormalSettingsGridSizer->SetFlexibleDirection( wxBOTH );
    tangentBinormalSettingsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    exportTangentsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Tangents"), wxDefaultPosition, wxDefaultSize, 0 );
    tangentBinormalSettingsGridSizer->Add( exportTangentsCheckbox, 0, wxALL, 5 );

    exportBinormalsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Binormals"), wxDefaultPosition, wxDefaultSize, 0 );
    tangentBinormalSettingsGridSizer->Add( exportBinormalsCheckbox, 0, wxALL, 5 );


    tangentBinormalSettingsSizer->Add( tangentBinormalSettingsGridSizer, 1, wxEXPAND, 5 );


    topSizer->Add( tangentBinormalSettingsSizer, 0, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ObjectSettingsDialog_MeshPage::~ObjectSettingsDialog_MeshPage()
{
}

ObjectSettingsDialog_SubmeshesPage::ObjectSettingsDialog_SubmeshesPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxGridSizer* headerSizer;
    headerSizer = new wxGridSizer( 1, 2, 0, 0 );

    useCustomTextureCoordinateCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Customize"), wxDefaultPosition, wxDefaultSize, 0 );
    headerSizer->Add( useCustomTextureCoordinateCheckbox, 0, wxALL, 5 );


    topSizer->Add( headerSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* textureCoordinateSetsSizer;
    textureCoordinateSetsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Submeshes / Subentities") ), wxVERTICAL );

    wxFlexGridSizer* textureCoordinateSetsGridSizer;
    textureCoordinateSetsGridSizer = new wxFlexGridSizer( 5, 3, 0, 0 );
    textureCoordinateSetsGridSizer->AddGrowableCol( 2 );
    textureCoordinateSetsGridSizer->SetFlexibleDirection( wxBOTH );
    textureCoordinateSetsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


    textureCoordinateSetsGridSizer->Add( 0, 0, 0, wxEXPAND, 5 );

    activeTextureCoordinateSetLabel = new wxStaticText( this, wxID_ANY, wxT("Submesh/Material"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
    activeTextureCoordinateSetLabel->Wrap( -1 );
    textureCoordinateSetsGridSizer->Add( activeTextureCoordinateSetLabel, 0, wxALL, 5 );

    wxArrayString submeshTextureCoordinateSetChoiceChoices;
    submeshTextureCoordinateSetChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), submeshTextureCoordinateSetChoiceChoices, 0 );
    submeshTextureCoordinateSetChoice->SetSelection( 0 );
    submeshTextureCoordinateSetChoice->SetMinSize( wxSize( 140,-1 ) );

    textureCoordinateSetsGridSizer->Add( submeshTextureCoordinateSetChoice, 0, wxEXPAND|wxLEFT, 5 );


    textureCoordinateSetsGridSizer->Add( 0, 0, 0, wxEXPAND, 5 );

    submeshNameLabel = new wxStaticText( this, wxID_ANY, wxT("Submesh Name"), wxDefaultPosition, wxDefaultSize, 0 );
    submeshNameLabel->Wrap( -1 );
    textureCoordinateSetsGridSizer->Add( submeshNameLabel, 0, wxALL, 5 );

    submeshNameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), 0 );
    submeshNameText->SetMaxLength( 0 );
    textureCoordinateSetsGridSizer->Add( submeshNameText, 0, wxLEFT|wxRIGHT, 5 );


    textureCoordinateSetsGridSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    renderQueueLabel = new wxStaticText( this, wxID_ANY, wxT("Render Queue"), wxDefaultPosition, wxDefaultSize, 0 );
    renderQueueLabel->Wrap( -1 );
    textureCoordinateSetsGridSizer->Add( renderQueueLabel, 0, wxALL, 5 );

    wxBoxSizer* renderQueueSizer;
    renderQueueSizer = new wxBoxSizer( wxHORIZONTAL );

    renderQueueText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), 0 );
    renderQueueText->SetMaxLength( 0 );
    renderQueueSizer->Add( renderQueueText, 0, wxRIGHT|wxLEFT, 5 );

    renderQueuePriorityLabel = new wxStaticText( this, wxID_ANY, wxT("Priority"), wxDefaultPosition, wxDefaultSize, 0 );
    renderQueuePriorityLabel->Wrap( -1 );
    renderQueueSizer->Add( renderQueuePriorityLabel, 0, wxALL, 5 );

    renderQueuePriorityText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    renderQueuePriorityText->SetMaxLength( 0 );
    renderQueueSizer->Add( renderQueuePriorityText, 0, wxRIGHT|wxLEFT, 5 );


    textureCoordinateSetsGridSizer->Add( renderQueueSizer, 1, wxEXPAND, 5 );


    textureCoordinateSetsGridSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    textureCoordinateSetsLabel = new wxStaticText( this, wxID_ANY, wxT("Texture Coordinate Sets"), wxDefaultPosition, wxDefaultSize, 0 );
    textureCoordinateSetsLabel->Wrap( -1 );
    textureCoordinateSetsGridSizer->Add( textureCoordinateSetsLabel, 0, wxALL, 5 );

    wxBoxSizer* refreshClearButtonsSizer;
    refreshClearButtonsSizer = new wxBoxSizer( wxHORIZONTAL );

    refreshTextureCoordinateSetsButton = new wxButton( this, wxID_ANY, wxT("Refresh"), wxDefaultPosition, wxSize( 70,-1 ), 0 );
    refreshClearButtonsSizer->Add( refreshTextureCoordinateSetsButton, 0, wxLEFT, 5 );

    clearTextureCoordinateSetsButton = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxSize( 70,-1 ), 0 );
    refreshClearButtonsSizer->Add( clearTextureCoordinateSetsButton, 0, 0, 5 );


    textureCoordinateSetsGridSizer->Add( refreshClearButtonsSizer, 1, wxEXPAND, 5 );


    textureCoordinateSetsSizer->Add( textureCoordinateSetsGridSizer, 0, wxEXPAND, 5 );

    wxBoxSizer* textureCoordinateSetsListSizer;
    textureCoordinateSetsListSizer = new wxBoxSizer( wxHORIZONTAL );


    textureCoordinateSetsListSizer->Add( 5, 0, 0, wxEXPAND, 5 );

    textureCoordinateSetsList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_NO_SORT_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES );
    textureCoordinateSetsListSizer->Add( textureCoordinateSetsList, 1, wxBOTTOM|wxEXPAND, 5 );

    wxBoxSizer* textureCoordinateSetsButtonsSizer;
    textureCoordinateSetsButtonsSizer = new wxBoxSizer( wxVERTICAL );

    moveTextureCoordinateSetUpButton = new wxButton( this, wxID_ANY, wxT("Move Up"), wxDefaultPosition, wxDefaultSize, 0 );
    textureCoordinateSetsButtonsSizer->Add( moveTextureCoordinateSetUpButton, 0, wxLEFT, 5 );

    moveTextureCoordinateSetDownButton = new wxButton( this, wxID_ANY, wxT("Move Down"), wxDefaultPosition, wxDefaultSize, 0 );
    textureCoordinateSetsButtonsSizer->Add( moveTextureCoordinateSetDownButton, 0, wxLEFT, 5 );

    removeTextureCoordinateSetButton = new wxButton( this, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
    textureCoordinateSetsButtonsSizer->Add( removeTextureCoordinateSetButton, 0, wxLEFT|wxTOP, 5 );

    textureCoordinateSetUButton = new wxButton( this, wxID_ANY, wxT("U (1D)"), wxDefaultPosition, wxDefaultSize, 0 );
    textureCoordinateSetsButtonsSizer->Add( textureCoordinateSetUButton, 0, wxLEFT|wxTOP, 5 );

    textureCoordinateSetUVButton = new wxButton( this, wxID_ANY, wxT("UV (2D)"), wxDefaultPosition, wxDefaultSize, 0 );
    textureCoordinateSetsButtonsSizer->Add( textureCoordinateSetUVButton, 0, wxLEFT, 5 );

    textureCoordinateSetUVWButton = new wxButton( this, wxID_ANY, wxT("UVW (3D)"), wxDefaultPosition, wxDefaultSize, 0 );
    textureCoordinateSetsButtonsSizer->Add( textureCoordinateSetUVWButton, 0, wxLEFT, 5 );


    textureCoordinateSetsListSizer->Add( textureCoordinateSetsButtonsSizer, 0, wxEXPAND, 5 );


    textureCoordinateSetsSizer->Add( textureCoordinateSetsListSizer, 1, wxEXPAND, 5 );


    topSizer->Add( textureCoordinateSetsSizer, 1, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ObjectSettingsDialog_SubmeshesPage::~ObjectSettingsDialog_SubmeshesPage()
{
}

ObjectSettingsDialog_MeshLodPage::ObjectSettingsDialog_MeshLodPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* levelOfDetailSettingsSizer;
    levelOfDetailSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Level-of-Detail (LOD) Settings") ), wxVERTICAL );

    wxBoxSizer* manualLodSizer;
    manualLodSizer = new wxBoxSizer( wxHORIZONTAL );

    manualLodList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_NO_SORT_HEADER|wxLC_REPORT|wxLC_VRULES );
    manualLodSizer->Add( manualLodList, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxTOP, 5 );

    wxBoxSizer* manualLodButtonsSizer;
    manualLodButtonsSizer = new wxBoxSizer( wxVERTICAL );

    addManualLodButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxSize( 100,-1 ), 0 );
    manualLodButtonsSizer->Add( addManualLodButton, 0, wxLEFT, 5 );

    editManualLodButton = new wxButton( this, wxID_ANY, wxT("Edit..."), wxDefaultPosition, wxSize( 100,-1 ), 0 );
    manualLodButtonsSizer->Add( editManualLodButton, 0, wxLEFT, 5 );

    removeManualLodButton = new wxButton( this, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
    manualLodButtonsSizer->Add( removeManualLodButton, 0, wxLEFT, 5 );


    manualLodButtonsSizer->Add( 0, 10, 0, wxEXPAND, 5 );

    calculateManualDistancesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Auto Distances"), wxDefaultPosition, wxDefaultSize, 0 );
    calculateManualDistancesCheckbox->SetValue(true);
    manualLodButtonsSizer->Add( calculateManualDistancesCheckbox, 0, wxALL, 5 );


    manualLodSizer->Add( manualLodButtonsSizer, 0, wxEXPAND, 5 );


    levelOfDetailSettingsSizer->Add( manualLodSizer, 1, wxEXPAND, 5 );


    topSizer->Add( levelOfDetailSettingsSizer, 1, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ObjectSettingsDialog_MeshLodPage::~ObjectSettingsDialog_MeshLodPage()
{
}

ObjectSettingsDialog_EntityPage::ObjectSettingsDialog_EntityPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* generalSettingsSizer;
    generalSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("General Settings") ), wxVERTICAL );

    wxFlexGridSizer* generalSettingsGridSizer;
    generalSettingsGridSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
    generalSettingsGridSizer->SetFlexibleDirection( wxBOTH );
    generalSettingsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    exportMeshCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Mesh"), wxDefaultPosition, wxDefaultSize, 0 );
    generalSettingsGridSizer->Add( exportMeshCheckbox, 0, wxALL, 5 );


    generalSettingsGridSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    skeletonAnimationBlendingLabel = new wxStaticText( this, wxID_ANY, wxT("Skeleton Animation Blending"), wxDefaultPosition, wxDefaultSize, 0 );
    skeletonAnimationBlendingLabel->Wrap( -1 );
    generalSettingsGridSizer->Add( skeletonAnimationBlendingLabel, 0, wxALL, 5 );

    wxArrayString skeletonAnimationBlendingChoiceChoices;
    skeletonAnimationBlendingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 140,-1 ), skeletonAnimationBlendingChoiceChoices, 0 );
    skeletonAnimationBlendingChoice->SetSelection( 0 );
    generalSettingsGridSizer->Add( skeletonAnimationBlendingChoice, 0, wxRIGHT|wxLEFT, 5 );


    generalSettingsSizer->Add( generalSettingsGridSizer, 1, wxEXPAND, 5 );


    topSizer->Add( generalSettingsSizer, 0, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ObjectSettingsDialog_EntityPage::~ObjectSettingsDialog_EntityPage()
{
}

ObjectSettingsDialog_PrefabInstancePage::ObjectSettingsDialog_PrefabInstancePage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* generalSettingsSizer;
    generalSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("General Settings") ), wxVERTICAL );

    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 1, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    modelNameLabel = new wxStaticText( this, wxID_ANY, wxT("Prefab Name"), wxDefaultPosition, wxDefaultSize, 0 );
    modelNameLabel->Wrap( -1 );
    gridSizer->Add( modelNameLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    modelNameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 180,-1 ), 0 );
    modelNameText->SetMaxLength( 0 );
    gridSizer->Add( modelNameText, 0, wxRIGHT|wxLEFT, 5 );


    generalSettingsSizer->Add( gridSizer, 1, wxEXPAND, 5 );


    topSizer->Add( generalSettingsSizer, 0, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ObjectSettingsDialog_PrefabInstancePage::~ObjectSettingsDialog_PrefabInstancePage()
{
}

ObjectSettingsDialog_SkyPage::ObjectSettingsDialog_SkyPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* generalSettingsSizer;
    generalSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("General Settings") ), wxVERTICAL );

    enableCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
    generalSettingsSizer->Add( enableCheckbox, 0, wxALL, 5 );


    topSizer->Add( generalSettingsSizer, 0, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ObjectSettingsDialog_SkyPage::~ObjectSettingsDialog_SkyPage()
{
}

ObjectSettingsDialog_LightPage::ObjectSettingsDialog_LightPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* generalSettingsSizer;
    generalSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("General Settings") ), wxVERTICAL );

    wxBoxSizer* colorSizer;
    colorSizer = new wxBoxSizer( wxHORIZONTAL );

    specularColorLabel = new wxStaticText( this, wxID_ANY, wxT("Specular Color"), wxDefaultPosition, wxDefaultSize, 0 );
    specularColorLabel->Wrap( -1 );
    colorSizer->Add( specularColorLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    specularColor = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    colorSizer->Add( specularColor, 0, wxRIGHT|wxLEFT, 5 );


    generalSettingsSizer->Add( colorSizer, 1, wxEXPAND, 5 );


    topSizer->Add( generalSettingsSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* attenuationSizer;
    attenuationSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Attenuation") ), wxVERTICAL );

    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 3, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    constantAttenuationLabel = new wxStaticText( this, wxID_ANY, wxT("Constant"), wxDefaultPosition, wxDefaultSize, 0 );
    constantAttenuationLabel->Wrap( -1 );
    gridSizer->Add( constantAttenuationLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    constantAttenuationText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
    constantAttenuationText->SetMaxLength( 0 );
    gridSizer->Add( constantAttenuationText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    linearAttenuationLabel = new wxStaticText( this, wxID_ANY, wxT("Linear"), wxDefaultPosition, wxDefaultSize, 0 );
    linearAttenuationLabel->Wrap( -1 );
    gridSizer->Add( linearAttenuationLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    linearAttenuationText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
    linearAttenuationText->SetMaxLength( 0 );
    gridSizer->Add( linearAttenuationText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    quadraticAttenuationLabel = new wxStaticText( this, wxID_ANY, wxT("Quadratic"), wxDefaultPosition, wxDefaultSize, 0 );
    quadraticAttenuationLabel->Wrap( -1 );
    gridSizer->Add( quadraticAttenuationLabel, 0, wxALL, 5 );

    quadraticAttenuationText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
    quadraticAttenuationText->SetMaxLength( 0 );
    gridSizer->Add( quadraticAttenuationText, 0, wxALL, 5 );


    attenuationSizer->Add( gridSizer, 1, wxEXPAND, 5 );


    topSizer->Add( attenuationSizer, 0, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ObjectSettingsDialog_LightPage::~ObjectSettingsDialog_LightPage()
{
}

ObjectSettingsDialog_MeshAnimationsPage::ObjectSettingsDialog_MeshAnimationsPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* animationTypeSizer;
    animationTypeSizer = new wxBoxSizer( wxHORIZONTAL );

    meshAnimationTypeLabel = new wxStaticText( this, wxID_ANY, wxT("Mesh Animation Type"), wxDefaultPosition, wxDefaultSize, 0 );
    meshAnimationTypeLabel->Wrap( -1 );
    animationTypeSizer->Add( meshAnimationTypeLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxArrayString meshAnimationTypeChoiceChoices;
    meshAnimationTypeChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 140,-1 ), meshAnimationTypeChoiceChoices, 0 );
    meshAnimationTypeChoice->SetSelection( 0 );
    animationTypeSizer->Add( meshAnimationTypeChoice, 0, wxBOTTOM|wxLEFT|wxRIGHT, 5 );

    selectedMeshAnimationTypeLabel = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    selectedMeshAnimationTypeLabel->Wrap( -1 );
    animationTypeSizer->Add( selectedMeshAnimationTypeLabel, 0, wxALL, 5 );


    topSizer->Add( animationTypeSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* skeletonSettingsSizer;
    skeletonSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Skeleton Settings") ), wxVERTICAL );

    wxBoxSizer* alwaysExportSizer;
    alwaysExportSizer = new wxBoxSizer( wxHORIZONTAL );

    exportSkeletonCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Skeleton"), wxDefaultPosition, wxDefaultSize, 0 );
    alwaysExportSizer->Add( exportSkeletonCheckbox, 0, wxALL, 5 );

    exportedMeshDeformedBySkeletonCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Skeleton Deforms Exported Mesh"), wxDefaultPosition, wxDefaultSize, 0 );
    alwaysExportSizer->Add( exportedMeshDeformedBySkeletonCheckbox, 0, wxALL, 5 );

    embedSkeletonCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Embed in Mesh"), wxDefaultPosition, wxDefaultSize, 0 );
    alwaysExportSizer->Add( embedSkeletonCheckbox, 0, wxALL, 5 );

    linkSkeletonToObjectCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Link to Mesh"), wxDefaultPosition, wxDefaultSize, 0 );
    alwaysExportSizer->Add( linkSkeletonToObjectCheckbox, 0, wxALL, 5 );


    skeletonSettingsSizer->Add( alwaysExportSizer, 0, wxEXPAND, 5 );

    wxBoxSizer* referencePoseSizer;
    referencePoseSizer = new wxBoxSizer( wxHORIZONTAL );

    referencePoseLabel = new wxStaticText( this, wxID_ANY, wxT("Reference Pose"), wxDefaultPosition, wxSize( 90,-1 ), 0 );
    referencePoseLabel->Wrap( -1 );
    referencePoseSizer->Add( referencePoseLabel, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

    wxArrayString referencePoseChoiceChoices;
    referencePoseChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, referencePoseChoiceChoices, 0 );
    referencePoseChoice->SetSelection( 0 );
    referencePoseChoice->SetMinSize( wxSize( 70,-1 ) );

    referencePoseSizer->Add( referencePoseChoice, 0, wxBOTTOM|wxRIGHT, 5 );

    referencePoseTimeSpinner = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), 0 );
    referencePoseTimeSpinner->SetMaxLength( 0 );
    referencePoseSizer->Add( referencePoseTimeSpinner, 0, wxRIGHT, 5 );

    referencePoseNoteLabel = new wxStaticText( this, wxID_ANY, wxT("(Use only with Skeleton animation type)"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
    referencePoseNoteLabel->Wrap( -1 );
    referencePoseSizer->Add( referencePoseNoteLabel, 0, wxALL, 5 );


    skeletonSettingsSizer->Add( referencePoseSizer, 0, wxEXPAND, 5 );

    wxBoxSizer* skeletonNameSizer;
    skeletonNameSizer = new wxBoxSizer( wxHORIZONTAL );

    skeletonNameLabel = new wxStaticText( this, wxID_ANY, wxT("Skeleton Name"), wxDefaultPosition, wxSize( 90,-1 ), 0 );
    skeletonNameLabel->Wrap( -1 );
    skeletonNameSizer->Add( skeletonNameLabel, 0, wxALL, 5 );

    skeletonNameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), 0 );
    skeletonNameText->SetMaxLength( 0 );
    skeletonNameSizer->Add( skeletonNameText, 0, wxRIGHT, 5 );


    skeletonSettingsSizer->Add( skeletonNameSizer, 0, wxEXPAND, 5 );

    wxFlexGridSizer* animatedRootSizer;
    animatedRootSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
    animatedRootSizer->SetFlexibleDirection( wxBOTH );
    animatedRootSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    animatedRootLabel = new wxStaticText( this, wxID_ANY, wxT("Animated Root"), wxDefaultPosition, wxSize( 90,-1 ), 0 );
    animatedRootLabel->Wrap( -1 );
    animatedRootSizer->Add( animatedRootLabel, 0, wxALL, 5 );

    animatedRootText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 320,-1 ), wxTE_READONLY );
    animatedRootText->SetMaxLength( 0 );
    animatedRootSizer->Add( animatedRootText, 1, 0, 5 );


    animatedRootSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    wxBoxSizer* animatedSkeletonRootButtonsSizer;
    animatedSkeletonRootButtonsSizer = new wxBoxSizer( wxHORIZONTAL );

    animatedRootChooseBoneButton = new wxButton( this, wxID_ANY, wxT("Choose Bone..."), wxDefaultPosition, wxSize( 100,-1 ), 0 );
    animatedSkeletonRootButtonsSizer->Add( animatedRootChooseBoneButton, 0, wxRIGHT, 5 );

    animatedRootChooseObjectButton = new wxButton( this, wxID_ANY, wxT("Choose Object..."), wxDefaultPosition, wxSize( 100,-1 ), 0 );
    animatedSkeletonRootButtonsSizer->Add( animatedRootChooseObjectButton, 0, wxRIGHT, 5 );


    animatedRootSizer->Add( animatedSkeletonRootButtonsSizer, 1, wxEXPAND, 5 );


    skeletonSettingsSizer->Add( animatedRootSizer, 0, wxEXPAND, 0 );

    wxBoxSizer* skeletonAnimationSamplingSizer;
    skeletonAnimationSamplingSizer = new wxBoxSizer( wxHORIZONTAL );

    skeletonAnimationSamplingLabel = new wxStaticText( this, wxID_ANY, wxT("Skeleton Animation Sampling"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
    skeletonAnimationSamplingLabel->Wrap( -1 );
    skeletonAnimationSamplingLabel->SetMinSize( wxSize( 140,-1 ) );

    skeletonAnimationSamplingSizer->Add( skeletonAnimationSamplingLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxBoxSizer* skeletonAnimationSamplingValuesSizer;
    skeletonAnimationSamplingValuesSizer = new wxBoxSizer( wxHORIZONTAL );

    skeletonAnimationSamplingText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    skeletonAnimationSamplingText->SetMaxLength( 0 );
    skeletonAnimationSamplingValuesSizer->Add( skeletonAnimationSamplingText, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    wxArrayString skeletonAnimationSamplingChoiceChoices;
    skeletonAnimationSamplingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, skeletonAnimationSamplingChoiceChoices, 0 );
    skeletonAnimationSamplingChoice->SetSelection( 0 );
    skeletonAnimationSamplingValuesSizer->Add( skeletonAnimationSamplingChoice, 1, wxALL, 5 );


    skeletonAnimationSamplingSizer->Add( skeletonAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );


    skeletonSettingsSizer->Add( skeletonAnimationSamplingSizer, 1, wxEXPAND, 5 );

    removeBonesWithNoInfluenceCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Remove Bones With No Influence"), wxDefaultPosition, wxDefaultSize, 0 );
    skeletonSettingsSizer->Add( removeBonesWithNoInfluenceCheckBox, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


    topSizer->Add( skeletonSettingsSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* poseSettingsSizer;
    poseSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Pose Settings") ), wxVERTICAL );

    exportPosesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Poses"), wxDefaultPosition, wxDefaultSize, 0 );
    poseSettingsSizer->Add( exportPosesCheckbox, 0, wxALL, 5 );


    topSizer->Add( poseSettingsSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* morphSettingsSizer;
    morphSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Morph Settings") ), wxVERTICAL );

    wxBoxSizer* vertexAnimationSamplingSizer;
    vertexAnimationSamplingSizer = new wxBoxSizer( wxHORIZONTAL );

    vertexAnimationSamplingLabel = new wxStaticText( this, wxID_ANY, wxT("Vertex Animation Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    vertexAnimationSamplingLabel->Wrap( -1 );
    vertexAnimationSamplingLabel->SetMinSize( wxSize( 140,-1 ) );

    vertexAnimationSamplingSizer->Add( vertexAnimationSamplingLabel, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

    wxBoxSizer* vertexAnimationSamplingValuesSizer;
    vertexAnimationSamplingValuesSizer = new wxBoxSizer( wxHORIZONTAL );

    vertexAnimationSamplingText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    vertexAnimationSamplingText->SetMaxLength( 0 );
    vertexAnimationSamplingValuesSizer->Add( vertexAnimationSamplingText, 0, wxBOTTOM|wxLEFT, 5 );

    wxArrayString vertexAnimationSamplingChoiceChoices;
    vertexAnimationSamplingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, vertexAnimationSamplingChoiceChoices, 0 );
    vertexAnimationSamplingChoice->SetSelection( 0 );
    vertexAnimationSamplingValuesSizer->Add( vertexAnimationSamplingChoice, 1, wxBOTTOM|wxLEFT|wxRIGHT, 5 );


    vertexAnimationSamplingSizer->Add( vertexAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );


    morphSettingsSizer->Add( vertexAnimationSamplingSizer, 1, wxEXPAND, 0 );


    topSizer->Add( morphSettingsSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* meshAnimationsSizer;
    meshAnimationsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Mesh Animations") ), wxVERTICAL );

    wxGridSizer* meshAnimationsControlsSizer;
    meshAnimationsControlsSizer = new wxGridSizer( 1, 2, 0, 0 );

    wxBoxSizer* meshAnimationsButtonsSizer;
    meshAnimationsButtonsSizer = new wxBoxSizer( wxHORIZONTAL );

    addMeshAnimationButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxDefaultSize, 0 );
    meshAnimationsButtonsSizer->Add( addMeshAnimationButton, 0, wxLEFT, 5 );

    editMeshAnimationButton = new wxButton( this, wxID_ANY, wxT("Edit..."), wxDefaultPosition, wxDefaultSize, 0 );
    meshAnimationsButtonsSizer->Add( editMeshAnimationButton, 0, 0, 5 );

    copyMeshAnimationButton = new wxButton( this, wxID_ANY, wxT("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
    meshAnimationsButtonsSizer->Add( copyMeshAnimationButton, 0, 0, 5 );

    removeMeshAnimationButton = new wxButton( this, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
    meshAnimationsButtonsSizer->Add( removeMeshAnimationButton, 0, 0, 5 );


    meshAnimationsControlsSizer->Add( meshAnimationsButtonsSizer, 1, wxEXPAND, 5 );

    syncToBipedMixerButton = new wxButton( this, wxID_ANY, wxT("Sync to Mixer"), wxDefaultPosition, wxDefaultSize, 0 );
    meshAnimationsControlsSizer->Add( syncToBipedMixerButton, 0, wxALIGN_RIGHT|wxRIGHT|wxLEFT, 5 );


    meshAnimationsSizer->Add( meshAnimationsControlsSizer, 0, wxEXPAND, 5 );

    meshAnimationsList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_VRULES );
    meshAnimationsSizer->Add( meshAnimationsList, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    topSizer->Add( meshAnimationsSizer, 1, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ObjectSettingsDialog_MeshAnimationsPage::~ObjectSettingsDialog_MeshAnimationsPage()
{
}

ObjectSettingsDialog_NodeAnimationsPage::ObjectSettingsDialog_NodeAnimationsPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* nodeAnimationDefaultsSizer;
    nodeAnimationDefaultsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Settings") ), wxVERTICAL );

    wxFlexGridSizer* nodeAnimationsDefaultsGridSizer;
    nodeAnimationsDefaultsGridSizer = new wxFlexGridSizer( 1, 2, 0, 0 );
    nodeAnimationsDefaultsGridSizer->SetFlexibleDirection( wxBOTH );
    nodeAnimationsDefaultsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    defaultAnimationSamplingLabel = new wxStaticText( this, wxID_ANY, wxT("Animation Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    defaultAnimationSamplingLabel->Wrap( -1 );
    nodeAnimationsDefaultsGridSizer->Add( defaultAnimationSamplingLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* animationSamplingSizer;
    animationSamplingSizer = new wxBoxSizer( wxHORIZONTAL );

    defaultAnimationSamplingText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    defaultAnimationSamplingText->SetMaxLength( 0 );
    animationSamplingSizer->Add( defaultAnimationSamplingText, 0, wxLEFT, 5 );

    wxArrayString defaultAnimationSamplingChoiceChoices;
    defaultAnimationSamplingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, defaultAnimationSamplingChoiceChoices, 0 );
    defaultAnimationSamplingChoice->SetSelection( 0 );
    animationSamplingSizer->Add( defaultAnimationSamplingChoice, 1, wxRIGHT|wxLEFT, 5 );


    nodeAnimationsDefaultsGridSizer->Add( animationSamplingSizer, 1, wxEXPAND, 5 );


    nodeAnimationDefaultsSizer->Add( nodeAnimationsDefaultsGridSizer, 0, wxEXPAND, 5 );


    topSizer->Add( nodeAnimationDefaultsSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* nodeAnimationsSizer;
    nodeAnimationsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Node Animations") ), wxVERTICAL );

    wxBoxSizer* meshAnimationsButtonsSizer;
    meshAnimationsButtonsSizer = new wxBoxSizer( wxHORIZONTAL );

    addNodeAnimationButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxDefaultSize, 0 );
    meshAnimationsButtonsSizer->Add( addNodeAnimationButton, 0, wxLEFT, 5 );

    editNodeAnimationButton = new wxButton( this, wxID_ANY, wxT("Edit..."), wxDefaultPosition, wxDefaultSize, 0 );
    meshAnimationsButtonsSizer->Add( editNodeAnimationButton, 0, 0, 5 );

    copyNodeAnimationButton = new wxButton( this, wxID_ANY, wxT("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
    meshAnimationsButtonsSizer->Add( copyNodeAnimationButton, 0, 0, 5 );

    removeNodeAnimationButton = new wxButton( this, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
    meshAnimationsButtonsSizer->Add( removeNodeAnimationButton, 0, 0, 5 );


    nodeAnimationsSizer->Add( meshAnimationsButtonsSizer, 0, wxEXPAND, 5 );

    nodeAnimationsList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_VRULES );
    nodeAnimationsSizer->Add( nodeAnimationsList, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    topSizer->Add( nodeAnimationsSizer, 1, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ObjectSettingsDialog_NodeAnimationsPage::~ObjectSettingsDialog_NodeAnimationsPage()
{
}

ObjectSettingsDialog_PhysicalPage::ObjectSettingsDialog_PhysicalPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* generalSettingsSizer;
    generalSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("General Settings") ), wxVERTICAL );

    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 1, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    itemTypeLabel = new wxStaticText( this, wxID_ANY, wxT("Item Type"), wxDefaultPosition, wxDefaultSize, 0 );
    itemTypeLabel->Wrap( -1 );
    gridSizer->Add( itemTypeLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    itemTypeText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), 0 );
    itemTypeText->SetMaxLength( 0 );
    gridSizer->Add( itemTypeText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );


    generalSettingsSizer->Add( gridSizer, 1, wxEXPAND, 5 );


    topSizer->Add( generalSettingsSizer, 0, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ObjectSettingsDialog_PhysicalPage::~ObjectSettingsDialog_PhysicalPage()
{
}

SceneSettingsDialog_GeneralPage::SceneSettingsDialog_GeneralPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* generalSettingsSizer;
    generalSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("General Settings") ), wxVERTICAL );

    wxFlexGridSizer* sceneManagerSizer;
    sceneManagerSizer = new wxFlexGridSizer( 1, 2, 0, 0 );
    sceneManagerSizer->SetFlexibleDirection( wxBOTH );
    sceneManagerSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    sceneManagerLabel = new wxStaticText( this, wxID_ANY, wxT("Scene Manager"), wxDefaultPosition, wxDefaultSize, 0 );
    sceneManagerLabel->Wrap( -1 );
    sceneManagerSizer->Add( sceneManagerLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    customSceneManagerText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
    customSceneManagerText->SetMaxLength( 0 );
    sceneManagerSizer->Add( customSceneManagerText, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    generalSettingsSizer->Add( sceneManagerSizer, 0, wxEXPAND, 5 );

    wxBoxSizer* sceneScaleSizer;
    sceneScaleSizer = new wxBoxSizer( wxHORIZONTAL );

    sceneScaleLabel = new wxStaticText( this, wxID_ANY, wxT("Scale"), wxDefaultPosition, wxDefaultSize, 0 );
    sceneScaleLabel->Wrap( -1 );
    sceneScaleSizer->Add( sceneScaleLabel, 0, wxALL, 5 );

    wxArrayString sceneScaleChoiceChoices;
    sceneScaleChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, sceneScaleChoiceChoices, 0 );
    sceneScaleChoice->SetSelection( 0 );
    sceneScaleSizer->Add( sceneScaleChoice, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    divideByLabel = new wxStaticText( this, wxID_ANY, wxT("Divided By"), wxDefaultPosition, wxDefaultSize, 0 );
    divideByLabel->Wrap( -1 );
    sceneScaleSizer->Add( divideByLabel, 0, wxALL, 5 );

    dividedBySpinner = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    dividedBySpinner->SetMaxLength( 0 );
    sceneScaleSizer->Add( dividedBySpinner, 0, wxBOTTOM|wxLEFT|wxRIGHT, 5 );


    generalSettingsSizer->Add( sceneScaleSizer, 0, wxEXPAND, 5 );

    wxFlexGridSizer* generalSettingsCheckboxesSizer;
    generalSettingsCheckboxesSizer = new wxFlexGridSizer( 3, 1, 0, 0 );
    generalSettingsCheckboxesSizer->SetFlexibleDirection( wxBOTH );
    generalSettingsCheckboxesSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    exportHierarchyCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Hierarchy"), wxDefaultPosition, wxDefaultSize, 0 );
    generalSettingsCheckboxesSizer->Add( exportHierarchyCheckbox, 0, wxALL, 5 );

    exportHiddenObjectsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Hidden Objects"), wxDefaultPosition, wxDefaultSize, 0 );
    generalSettingsCheckboxesSizer->Add( exportHiddenObjectsCheckbox, 0, wxALL, 5 );


    generalSettingsSizer->Add( generalSettingsCheckboxesSizer, 1, wxEXPAND, 5 );


    topSizer->Add( generalSettingsSizer, 0, wxEXPAND|wxTOP, 5 );

    wxBoxSizer* aliasesSizer;
    aliasesSizer = new wxBoxSizer( wxHORIZONTAL );

    wxStaticBoxSizer* queryFlagAliasesSizer;
    queryFlagAliasesSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Flags") ), wxVERTICAL );

    wxGridSizer* queryFlagAliasesControlsSizer;
    queryFlagAliasesControlsSizer = new wxGridSizer( 1, 2, 0, 0 );

    addFlagButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxDefaultSize, 0 );
    queryFlagAliasesControlsSizer->Add( addFlagButton, 0, wxLEFT|wxRIGHT, 5 );


    queryFlagAliasesSizer->Add( queryFlagAliasesControlsSizer, 0, wxEXPAND, 5 );

    queryFlagAliasesText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
    queryFlagAliasesText->SetMaxLength( 0 );
    queryFlagAliasesSizer->Add( queryFlagAliasesText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    aliasesSizer->Add( queryFlagAliasesSizer, 1, wxEXPAND|wxRIGHT, 5 );


    topSizer->Add( aliasesSizer, 1, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

SceneSettingsDialog_GeneralPage::~SceneSettingsDialog_GeneralPage()
{
}

SceneSettingsDialog_MeshesPage::SceneSettingsDialog_MeshesPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* meshSettingsSizer;
    meshSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("General Settings") ), wxVERTICAL );

    wxFlexGridSizer* meshSettingsGridSizer;
    meshSettingsGridSizer = new wxFlexGridSizer( 1, 4, 0, 0 );
    meshSettingsGridSizer->SetFlexibleDirection( wxBOTH );
    meshSettingsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    exportVertexColorsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Vertex Colors"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsGridSizer->Add( exportVertexColorsCheckbox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    useExplicitVertexColorCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Explicit Color Fallback (White)"), wxDefaultPosition, wxDefaultSize, 0 );
    useExplicitVertexColorCheckbox->SetValue(true);
    meshSettingsGridSizer->Add( useExplicitVertexColorCheckbox, 0, wxALL, 5 );


    meshSettingsSizer->Add( meshSettingsGridSizer, 1, wxEXPAND, 5 );

    animateBoundingVolumesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Animated Bounding Volumes"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsSizer->Add( animateBoundingVolumesCheckbox, 0, wxALL, 5 );

    checkMeshInstancesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Check for Mesh Instances"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsSizer->Add( checkMeshInstancesCheckbox, 0, wxALL, 5 );

    alwaysCreateNewSubmeshesWhenMergingCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Always Create New Submeshes When Merging"), wxDefaultPosition, wxDefaultSize, 0 );
    meshSettingsSizer->Add( alwaysCreateNewSubmeshesWhenMergingCheckBox, 0, wxALL, 5 );


    topSizer->Add( meshSettingsSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* tangentBinormalSettingsSizer;
    tangentBinormalSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Tangent/Binormal Settings") ), wxVERTICAL );

    wxBoxSizer* tangentBinormalSizer;
    tangentBinormalSizer = new wxBoxSizer( wxHORIZONTAL );

    exportTangentsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Tangents"), wxDefaultPosition, wxDefaultSize, 0 );
    tangentBinormalSizer->Add( exportTangentsCheckbox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    exportBinormalsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Binormals"), wxDefaultPosition, wxDefaultSize, 0 );
    tangentBinormalSizer->Add( exportBinormalsCheckbox, 0, wxALL, 5 );


    tangentBinormalSettingsSizer->Add( tangentBinormalSizer, 1, wxEXPAND, 5 );

    wxFlexGridSizer* tangentSettingsGridSizer;
    tangentSettingsGridSizer = new wxFlexGridSizer( 2, 4, 0, 0 );
    tangentSettingsGridSizer->SetFlexibleDirection( wxBOTH );
    tangentSettingsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


    tangentBinormalSettingsSizer->Add( tangentSettingsGridSizer, 0, wxEXPAND, 5 );


    topSizer->Add( tangentBinormalSettingsSizer, 0, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

SceneSettingsDialog_MeshesPage::~SceneSettingsDialog_MeshesPage()
{
}

SceneSettingsDialog_MaterialsPage::SceneSettingsDialog_MaterialsPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* additionalAndIgnoreSizer;
    additionalAndIgnoreSizer = new wxBoxSizer( wxHORIZONTAL );

    wxStaticBoxSizer* additionalMaterialsSizer;
    additionalMaterialsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Additional Materials") ), wxVERTICAL );

    addAdditionalMaterialButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxDefaultSize, 0 );
    additionalMaterialsSizer->Add( addAdditionalMaterialButton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5 );

    addAdditionalMaterialsText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
    addAdditionalMaterialsText->SetMaxLength( 0 );
    additionalMaterialsSizer->Add( addAdditionalMaterialsText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    additionalAndIgnoreSizer->Add( additionalMaterialsSizer, 1, wxEXPAND|wxRIGHT|wxTOP, 5 );

    wxStaticBoxSizer* ignoreMaterialsSizer;
    ignoreMaterialsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Ignore Materials") ), wxVERTICAL );

    addIgnoreMaterialButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxDefaultSize, 0 );
    ignoreMaterialsSizer->Add( addIgnoreMaterialButton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5 );

    ignoreMaterialsText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
    ignoreMaterialsText->SetMaxLength( 0 );
    ignoreMaterialsSizer->Add( ignoreMaterialsText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    additionalAndIgnoreSizer->Add( ignoreMaterialsSizer, 1, wxEXPAND|wxTOP, 5 );


    topSizer->Add( additionalAndIgnoreSizer, 1, wxEXPAND, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

SceneSettingsDialog_MaterialsPage::~SceneSettingsDialog_MaterialsPage()
{
}

SceneSettingsDialog_BitmapsPage::SceneSettingsDialog_BitmapsPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* bitmapExportControlSizer;
    bitmapExportControlSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Bitmap Export Control") ), wxVERTICAL );

    copyBitmapsToExportDirectoryCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Copy Bitmaps to Export Directory"), wxDefaultPosition, wxDefaultSize, 0 );
    bitmapExportControlSizer->Add( copyBitmapsToExportDirectoryCheckbox, 0, wxALL, 5 );

    convertBitmapsToTexturesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Convert Bitmaps to Textures (.texture)"), wxDefaultPosition, wxDefaultSize, 0 );
    bitmapExportControlSizer->Add( convertBitmapsToTexturesCheckbox, 0, wxALL, 5 );


    topSizer->Add( bitmapExportControlSizer, 0, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

SceneSettingsDialog_BitmapsPage::~SceneSettingsDialog_BitmapsPage()
{
}

SceneSettingsDialog_EnvironmentPage::SceneSettingsDialog_EnvironmentPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxGridSizer* headerSizer;
    headerSizer = new wxGridSizer( 1, 2, 0, 0 );

    exportCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Environment Settings"), wxDefaultPosition, wxDefaultSize, 0 );
    headerSizer->Add( exportCheckbox, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );


    topSizer->Add( headerSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* colorsSizer;
    colorsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Colors") ), wxVERTICAL );

    wxFlexGridSizer* colorsGridSizer;
    colorsGridSizer = new wxFlexGridSizer( 2, 3, 0, 0 );
    colorsGridSizer->SetFlexibleDirection( wxBOTH );
    colorsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    ambientLightColorLabel = new wxStaticText( this, wxID_ANY, wxT("Ambient Light"), wxDefaultPosition, wxDefaultSize, 0 );
    ambientLightColorLabel->Wrap( -1 );
    colorsGridSizer->Add( ambientLightColorLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    ambientLightColor = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    colorsGridSizer->Add( ambientLightColor, 0, wxLEFT, 5 );

    wxArrayString ambientLightColorSyncChoiceChoices;
    ambientLightColorSyncChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ambientLightColorSyncChoiceChoices, 0 );
    ambientLightColorSyncChoice->SetSelection( 0 );
    colorsGridSizer->Add( ambientLightColorSyncChoice, 0, wxLEFT|wxRIGHT, 5 );

    backgroundColorLabel = new wxStaticText( this, wxID_ANY, wxT("Background"), wxDefaultPosition, wxDefaultSize, 0 );
    backgroundColorLabel->Wrap( -1 );
    colorsGridSizer->Add( backgroundColorLabel, 0, wxALL, 5 );

    backgroundColor = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    colorsGridSizer->Add( backgroundColor, 0, wxLEFT, 5 );

    wxArrayString backgroundColorSyncChoiceChoices;
    backgroundColorSyncChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, backgroundColorSyncChoiceChoices, 0 );
    backgroundColorSyncChoice->SetSelection( 0 );
    colorsGridSizer->Add( backgroundColorSyncChoice, 0, wxLEFT|wxRIGHT, 5 );


    colorsSizer->Add( colorsGridSizer, 1, wxEXPAND, 5 );


    topSizer->Add( colorsSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* environmentRangeSizer;
    environmentRangeSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Environment Range") ), wxVERTICAL );

    wxFlexGridSizer* environmentRangeGridSizer;
    environmentRangeGridSizer = new wxFlexGridSizer( 4, 2, 0, 0 );
    environmentRangeGridSizer->SetFlexibleDirection( wxBOTH );
    environmentRangeGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    nearDistanceLabel = new wxStaticText( this, wxID_ANY, wxT("Near Distance"), wxDefaultPosition, wxDefaultSize, 0 );
    nearDistanceLabel->Wrap( -1 );
    environmentRangeGridSizer->Add( nearDistanceLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    nearDistanceSpinner = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    nearDistanceSpinner->SetMaxLength( 0 );
    environmentRangeGridSizer->Add( nearDistanceSpinner, 0, wxRIGHT|wxLEFT, 5 );

    farLabel = new wxStaticText( this, wxID_ANY, wxT("Far"), wxDefaultPosition, wxDefaultSize, 0 );
    farLabel->Wrap( -1 );
    environmentRangeGridSizer->Add( farLabel, 0, wxRIGHT|wxLEFT, 5 );


    environmentRangeGridSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    wxBoxSizer* farDistanceLeftSizer;
    farDistanceLeftSizer = new wxBoxSizer( wxHORIZONTAL );


    farDistanceLeftSizer->Add( 20, 0, 1, wxEXPAND, 5 );

    farDistanceButton = new wxRadioButton( this, wxID_ANY, wxT("Distance"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP|wxRB_SINGLE );
    farDistanceLeftSizer->Add( farDistanceButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


    environmentRangeGridSizer->Add( farDistanceLeftSizer, 1, 0, 5 );

    wxBoxSizer* farDistanceRightSizer;
    farDistanceRightSizer = new wxBoxSizer( wxHORIZONTAL );

    farDistanceSpinner = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    farDistanceSpinner->SetMaxLength( 0 );
    farDistanceRightSizer->Add( farDistanceSpinner, 0, wxRIGHT|wxLEFT, 5 );


    environmentRangeGridSizer->Add( farDistanceRightSizer, 1, wxEXPAND, 5 );

    wxBoxSizer* farBoxObjectLeftSizer;
    farBoxObjectLeftSizer = new wxBoxSizer( wxHORIZONTAL );


    farBoxObjectLeftSizer->Add( 20, 0, 1, wxEXPAND, 5 );

    farBoxObjectButton = new wxRadioButton( this, wxID_ANY, wxT("Box Object"), wxDefaultPosition, wxDefaultSize, wxRB_SINGLE );
    farBoxObjectLeftSizer->Add( farBoxObjectButton, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );


    environmentRangeGridSizer->Add( farBoxObjectLeftSizer, 1, wxEXPAND, 5 );

    wxBoxSizer* farBoxObjectRightSizer;
    farBoxObjectRightSizer = new wxBoxSizer( wxHORIZONTAL );

    farBoxObjectText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 220,-1 ), wxTE_READONLY );
    farBoxObjectText->SetMaxLength( 0 );
    farBoxObjectRightSizer->Add( farBoxObjectText, 0, wxLEFT, 5 );

    chooseFarBoxObjectButton = new wxButton( this, wxID_ANY, wxT("Choose..."), wxDefaultPosition, wxDefaultSize, 0 );
    farBoxObjectRightSizer->Add( chooseFarBoxObjectButton, 0, wxRIGHT|wxLEFT, 5 );


    environmentRangeGridSizer->Add( farBoxObjectRightSizer, 1, wxEXPAND, 5 );


    environmentRangeSizer->Add( environmentRangeGridSizer, 1, wxEXPAND, 5 );


    topSizer->Add( environmentRangeSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* fogSizer;
    fogSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Fog") ), wxVERTICAL );

    wxFlexGridSizer* fogHorizontalSizer;
    fogHorizontalSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
    fogHorizontalSizer->SetFlexibleDirection( wxBOTH );
    fogHorizontalSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    fogModeLabel = new wxStaticText( this, wxID_ANY, wxT("Mode"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
    fogModeLabel->Wrap( -1 );
    fogHorizontalSizer->Add( fogModeLabel, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5 );

    wxBoxSizer* fogModeSizer;
    fogModeSizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString fogModeChoiceChoices;
    fogModeChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, fogModeChoiceChoices, 0 );
    fogModeChoice->SetSelection( 0 );
    fogModeSizer->Add( fogModeChoice, 0, 0, 5 );


    fogModeSizer->Add( 10, 0, 1, wxEXPAND, 5 );

    fogDensityLabel = new wxStaticText( this, wxID_ANY, wxT("Density"), wxDefaultPosition, wxDefaultSize, 0 );
    fogDensityLabel->Wrap( -1 );
    fogModeSizer->Add( fogDensityLabel, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5 );

    fogDensitySpinner = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    fogDensitySpinner->SetMaxLength( 0 );
    fogModeSizer->Add( fogDensitySpinner, 0, wxRIGHT, 5 );

    fogStartLabel = new wxStaticText( this, wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
    fogStartLabel->Wrap( -1 );
    fogModeSizer->Add( fogStartLabel, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT, 5 );

    fogStartSpinner = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    fogStartSpinner->SetMaxLength( 0 );
    fogModeSizer->Add( fogStartSpinner, 0, 0, 5 );

    fogEndLabel = new wxStaticText( this, wxID_ANY, wxT("End"), wxDefaultPosition, wxDefaultSize, 0 );
    fogEndLabel->Wrap( -1 );
    fogModeSizer->Add( fogEndLabel, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5 );

    fogEndSpinner = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    fogEndSpinner->SetMaxLength( 0 );
    fogModeSizer->Add( fogEndSpinner, 0, 0, 5 );


    fogHorizontalSizer->Add( fogModeSizer, 1, wxEXPAND, 5 );

    fogColorLabel = new wxStaticText( this, wxID_ANY, wxT("Color"), wxDefaultPosition, wxDefaultSize, 0 );
    fogColorLabel->Wrap( -1 );
    fogHorizontalSizer->Add( fogColorLabel, 0, wxALL, 5 );

    fogColor = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    fogHorizontalSizer->Add( fogColor, 0, wxRIGHT, 5 );


    fogSizer->Add( fogHorizontalSizer, 1, wxEXPAND, 5 );

    wxArrayString fogSyncChoiceChoices;
    fogSyncChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, fogSyncChoiceChoices, 0 );
    fogSyncChoice->SetSelection( 0 );
    fogSizer->Add( fogSyncChoice, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxTOP, 5 );


    topSizer->Add( fogSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* shadowsSizer;
    shadowsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Shadows") ), wxVERTICAL );

    wxFlexGridSizer* shadowsGridSizer;
    shadowsGridSizer = new wxFlexGridSizer( 3, 2, 0, 0 );
    shadowsGridSizer->SetFlexibleDirection( wxBOTH );
    shadowsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    shadowTechniqueLabel = new wxStaticText( this, wxID_ANY, wxT("Technique"), wxDefaultPosition, wxDefaultSize, 0 );
    shadowTechniqueLabel->Wrap( -1 );
    shadowsGridSizer->Add( shadowTechniqueLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer* shadowTechniqueSizer;
    shadowTechniqueSizer = new wxBoxSizer( wxHORIZONTAL );

    shadowTechniqueText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
    shadowTechniqueText->SetMaxLength( 0 );
    shadowTechniqueSizer->Add( shadowTechniqueText, 0, wxALL, 5 );


    shadowsGridSizer->Add( shadowTechniqueSizer, 1, wxEXPAND, 5 );

    shadowFarDistanceLabel = new wxStaticText( this, wxID_ANY, wxT("Far Distance"), wxDefaultPosition, wxDefaultSize, 0 );
    shadowFarDistanceLabel->Wrap( -1 );
    shadowsGridSizer->Add( shadowFarDistanceLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer* shadowFarDistanceSizer;
    shadowFarDistanceSizer = new wxBoxSizer( wxHORIZONTAL );

    shadowFarDistanceSpinner = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    shadowFarDistanceSpinner->SetMaxLength( 0 );
    shadowFarDistanceSizer->Add( shadowFarDistanceSpinner, 0, wxRIGHT|wxLEFT, 5 );


    shadowsGridSizer->Add( shadowFarDistanceSizer, 1, wxEXPAND, 5 );

    shadowColorLabel = new wxStaticText( this, wxID_ANY, wxT("Color"), wxDefaultPosition, wxDefaultSize, 0 );
    shadowColorLabel->Wrap( -1 );
    shadowsGridSizer->Add( shadowColorLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    shadowColor = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    shadowsGridSizer->Add( shadowColor, 0, wxRIGHT|wxLEFT, 5 );


    shadowsSizer->Add( shadowsGridSizer, 1, wxEXPAND, 5 );


    topSizer->Add( shadowsSizer, 0, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

SceneSettingsDialog_EnvironmentPage::~SceneSettingsDialog_EnvironmentPage()
{
}

SceneSettingsDialog_DirectoriesPage::SceneSettingsDialog_DirectoriesPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* defaultSceneFileSizer;
    defaultSceneFileSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Default File") ), wxVERTICAL );

    exportToDefaultSceneFileLabel = new wxStaticText( this, wxID_ANY, wxT("The default file is used when exporting the scene through the Finjin menu."), wxDefaultPosition, wxDefaultSize, 0 );
    exportToDefaultSceneFileLabel->Wrap( -1 );
    defaultSceneFileSizer->Add( exportToDefaultSceneFileLabel, 0, wxALL, 5 );

    exportToDefaultSceneFileCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export to Default File"), wxDefaultPosition, wxDefaultSize, 0 );
    defaultSceneFileSizer->Add( exportToDefaultSceneFileCheckbox, 0, wxALL, 5 );

    baseDirectoryLabel = new wxStaticText( this, wxID_ANY, wxT("Base Directory"), wxDefaultPosition, wxDefaultSize, 0 );
    baseDirectoryLabel->Wrap( -1 );
    defaultSceneFileSizer->Add( baseDirectoryLabel, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* baseDirectorySizer;
    baseDirectorySizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString baseDirectoryChoiceChoices;
    baseDirectoryChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, baseDirectoryChoiceChoices, 0 );
    baseDirectoryChoice->SetSelection( 0 );
    baseDirectorySizer->Add( baseDirectoryChoice, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    baseDirectoryText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 285,-1 ), wxTE_READONLY );
    baseDirectoryText->SetMaxLength( 0 );
    baseDirectorySizer->Add( baseDirectoryText, 1, wxBOTTOM, 5 );

    baseDirectoryButton = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxSize( 100,-1 ), 0 );
    baseDirectorySizer->Add( baseDirectoryButton, 0, wxBOTTOM|wxLEFT, 5 );


    defaultSceneFileSizer->Add( baseDirectorySizer, 1, wxEXPAND, 5 );

    defaultSceneFileNameLabel = new wxStaticText( this, wxID_ANY, wxT("File Name"), wxDefaultPosition, wxDefaultSize, 0 );
    defaultSceneFileNameLabel->Wrap( -1 );
    defaultSceneFileSizer->Add( defaultSceneFileNameLabel, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* defaultSceneFileNameSizer;
    defaultSceneFileNameSizer = new wxBoxSizer( wxHORIZONTAL );

    defaultSceneFileNameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 285,-1 ), 0 );
    defaultSceneFileNameText->SetMaxLength( 0 );
    defaultSceneFileNameSizer->Add( defaultSceneFileNameText, 1, wxBOTTOM|wxLEFT, 5 );

    defaultSceneFileNameButton = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxSize( 100,-1 ), 0 );
    defaultSceneFileNameSizer->Add( defaultSceneFileNameButton, 0, wxBOTTOM|wxLEFT, 5 );


    defaultSceneFileSizer->Add( defaultSceneFileNameSizer, 1, wxEXPAND, 5 );


    topSizer->Add( defaultSceneFileSizer, 0, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

SceneSettingsDialog_DirectoriesPage::~SceneSettingsDialog_DirectoriesPage()
{
}

SceneSettingsDialog_AnimationsPage::SceneSettingsDialog_AnimationsPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* meshAnimationsSizer;
    meshAnimationsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Mesh Animations") ), wxVERTICAL );

    wxGridSizer* skeletonAndVertexanimationSamplingSizer;
    skeletonAndVertexanimationSamplingSizer = new wxGridSizer( 1, 2, 0, 0 );

    wxBoxSizer* skeletonAnimationSamplingSizer;
    skeletonAnimationSamplingSizer = new wxBoxSizer( wxVERTICAL );

    skeletonAnimationSamplingLabel = new wxStaticText( this, wxID_ANY, wxT("Skeleton Animation Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    skeletonAnimationSamplingLabel->Wrap( -1 );
    skeletonAnimationSamplingSizer->Add( skeletonAnimationSamplingLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* skeletonAnimationSamplingValuesSizer;
    skeletonAnimationSamplingValuesSizer = new wxBoxSizer( wxHORIZONTAL );

    skeletonAnimationSamplingText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    skeletonAnimationSamplingText->SetMaxLength( 0 );
    skeletonAnimationSamplingValuesSizer->Add( skeletonAnimationSamplingText, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    wxArrayString skeletonAnimationSamplingChoiceChoices;
    skeletonAnimationSamplingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, skeletonAnimationSamplingChoiceChoices, 0 );
    skeletonAnimationSamplingChoice->SetSelection( 0 );
    skeletonAnimationSamplingValuesSizer->Add( skeletonAnimationSamplingChoice, 1, wxALL, 5 );


    skeletonAnimationSamplingSizer->Add( skeletonAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );


    skeletonAndVertexanimationSamplingSizer->Add( skeletonAnimationSamplingSizer, 1, wxEXPAND, 5 );

    wxBoxSizer* vertexAnimationSamplingSizer;
    vertexAnimationSamplingSizer = new wxBoxSizer( wxVERTICAL );

    vertexAnimationSamplingLabel = new wxStaticText( this, wxID_ANY, wxT("Vertex Animation Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    vertexAnimationSamplingLabel->Wrap( -1 );
    vertexAnimationSamplingSizer->Add( vertexAnimationSamplingLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* vertexAnimationSamplingValuesSizer;
    vertexAnimationSamplingValuesSizer = new wxBoxSizer( wxHORIZONTAL );

    vertexAnimationSamplingText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    vertexAnimationSamplingText->SetMaxLength( 0 );
    vertexAnimationSamplingValuesSizer->Add( vertexAnimationSamplingText, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    wxArrayString vertexAnimationSamplingChoiceChoices;
    vertexAnimationSamplingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, vertexAnimationSamplingChoiceChoices, 0 );
    vertexAnimationSamplingChoice->SetSelection( 0 );
    vertexAnimationSamplingValuesSizer->Add( vertexAnimationSamplingChoice, 1, wxALL, 5 );


    vertexAnimationSamplingSizer->Add( vertexAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );


    skeletonAndVertexanimationSamplingSizer->Add( vertexAnimationSamplingSizer, 1, wxEXPAND, 5 );


    meshAnimationsSizer->Add( skeletonAndVertexanimationSamplingSizer, 1, wxEXPAND, 5 );


    topSizer->Add( meshAnimationsSizer, 0, wxEXPAND|wxTOP, 5 );

    wxStaticBoxSizer* nodeAnimationsSizer;
    nodeAnimationsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Node Animations") ), wxVERTICAL );

    wxBoxSizer* nodeAnimationSamplingSizer;
    nodeAnimationSamplingSizer = new wxBoxSizer( wxVERTICAL );

    nodeAnimationSamplingLabel = new wxStaticText( this, wxID_ANY, wxT("Node Animation Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    nodeAnimationSamplingLabel->Wrap( -1 );
    nodeAnimationSamplingSizer->Add( nodeAnimationSamplingLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* nodeAnimationSamplingValuesSizer;
    nodeAnimationSamplingValuesSizer = new wxBoxSizer( wxHORIZONTAL );

    nodeAnimationSamplingText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    nodeAnimationSamplingText->SetMaxLength( 0 );
    nodeAnimationSamplingValuesSizer->Add( nodeAnimationSamplingText, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    wxArrayString nodeAnimationSamplingChoiceChoices;
    nodeAnimationSamplingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, nodeAnimationSamplingChoiceChoices, 0 );
    nodeAnimationSamplingChoice->SetSelection( 0 );
    nodeAnimationSamplingValuesSizer->Add( nodeAnimationSamplingChoice, 1, wxALL, 5 );


    nodeAnimationSamplingSizer->Add( nodeAnimationSamplingValuesSizer, 0, wxEXPAND, 5 );


    nodeAnimationsSizer->Add( nodeAnimationSamplingSizer, 1, wxEXPAND, 5 );


    topSizer->Add( nodeAnimationsSizer, 0, wxEXPAND|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

SceneSettingsDialog_AnimationsPage::~SceneSettingsDialog_AnimationsPage()
{
}

SceneSettingsDialog_EventsPage::SceneSettingsDialog_EventsPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    descriptionLabel = new wxStaticText( this, wxID_ANY, wxT("Events are command lines to execute at various points in the export process. Enter one command\nper line. Be sure to quote macro parameters if the expanded values contain spaces."), wxDefaultPosition, wxDefaultSize, 0 );
    descriptionLabel->Wrap( -1 );
    topSizer->Add( descriptionLabel, 0, wxALL|wxEXPAND, 5 );

    wxStaticBoxSizer* beforeExportSizer;
    beforeExportSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Before Export") ), wxVERTICAL );

    beforeExportText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,160 ), wxTE_DONTWRAP|wxTE_MULTILINE );
    beforeExportText->SetMaxLength( 0 );
    beforeExportSizer->Add( beforeExportText, 1, wxALL|wxEXPAND, 5 );


    topSizer->Add( beforeExportSizer, 1, wxEXPAND, 5 );

    wxStaticBoxSizer* afterExportSizer;
    afterExportSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("After Export") ), wxVERTICAL );

    afterExportText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,160 ), wxTE_DONTWRAP|wxTE_MULTILINE );
    afterExportText->SetMaxLength( 0 );
    afterExportSizer->Add( afterExportText, 1, wxALL|wxEXPAND, 5 );


    topSizer->Add( afterExportSizer, 1, wxEXPAND|wxTOP, 5 );

    macrosLabel = new wxStaticText( this, wxID_ANY, wxT("Macros"), wxDefaultPosition, wxDefaultSize, 0 );
    macrosLabel->Wrap( -1 );
    topSizer->Add( macrosLabel, 0, wxALL, 5 );

    macrosText = new wxTextCtrl( this, wxID_ANY, wxT("$(AppDir) - 3DS Max application directory (with trailing separator)\n$(ExportDir) - Export directory (with trailing separator)\n$(ExportFilePath) - Full path to the exported file\n$(ExportFileName) - Name of the exported file\n$(ENV_VAR) - Example of a user-defined system environment variable named ENV_VAR"), wxDefaultPosition, wxSize( -1,80 ), wxTE_MULTILINE|wxTE_NO_VSCROLL|wxTE_READONLY );
    macrosText->SetMaxLength( 0 );
    topSizer->Add( macrosText, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

SceneSettingsDialog_EventsPage::~SceneSettingsDialog_EventsPage()
{
}

UserDataSettingsPage::UserDataSettingsPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* idReferenceSizer;
    idReferenceSizer = new wxBoxSizer( wxHORIZONTAL );

    idLabel = new wxStaticText( this, wxID_ANY, wxT("ID"), wxDefaultPosition, wxDefaultSize, 0 );
    idLabel->Wrap( -1 );
    idReferenceSizer->Add( idLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    idText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 240,-1 ), 0 );
    idText->SetMaxLength( 0 );
    idReferenceSizer->Add( idText, 0, wxRIGHT, 5 );

    generateIDButton = new wxButton( this, wxID_ANY, wxT("Generate"), wxDefaultPosition, wxDefaultSize, 0 );
    idReferenceSizer->Add( generateIDButton, 0, 0, 5 );


    idReferenceSizer->Add( 0, 0, 1, 0, 5 );


    topSizer->Add( idReferenceSizer, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

    wxBoxSizer* userDataClassSizer;
    userDataClassSizer = new wxBoxSizer( wxHORIZONTAL );

    userDataClassLabel = new wxStaticText( this, wxID_ANY, wxT("User Data Class"), wxDefaultPosition, wxDefaultSize, 0 );
    userDataClassLabel->Wrap( -1 );
    userDataClassSizer->Add( userDataClassLabel, 0, wxALL, 5 );

    wxArrayString userDataClassChoiceChoices;
    userDataClassChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, userDataClassChoiceChoices, 0 );
    userDataClassChoice->SetSelection( 0 );
    userDataClassSizer->Add( userDataClassChoice, 1, 0, 5 );

    configureUserDataClassButton = new wxButton( this, wxID_ANY, wxT("Configure..."), wxDefaultPosition, wxDefaultSize, 0 );
    userDataClassSizer->Add( configureUserDataClassButton, 0, wxRIGHT|wxLEFT, 5 );


    topSizer->Add( userDataClassSizer, 0, wxEXPAND, 5 );

    userDataText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    userDataText->SetMaxLength( 0 );
    topSizer->Add( userDataText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    userDataControlsWindow = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTAB_TRAVERSAL|wxVSCROLL );
    userDataControlsWindow->SetScrollRate( 5, 5 );
    topSizer->Add( userDataControlsWindow, 1, wxEXPAND | wxALL, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

UserDataSettingsPage::~UserDataSettingsPage()
{
}

AboutDialog_AcknowledgmentsPage::AboutDialog_AcknowledgmentsPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    thirdPartyLibrariesLabel = new wxStaticText( this, wxID_ANY, wxT("3rd Party Libraries"), wxDefaultPosition, wxDefaultSize, 0 );
    thirdPartyLibrariesLabel->Wrap( -1 );
    topSizer->Add( thirdPartyLibrariesLabel, 0, wxALL, 5 );

    thirdPartyLibrariesText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_AUTO_URL|wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_NO_VSCROLL|wxTE_READONLY );
    thirdPartyLibrariesText->SetMaxLength( 0 );
    topSizer->Add( thirdPartyLibrariesText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    this->SetSizer( topSizer );
    this->Layout();

    // Connect Events
    thirdPartyLibrariesText->Connect( wxEVT_COMMAND_TEXT_URL, wxTextUrlEventHandler( AboutDialog_AcknowledgmentsPage::OnTextURL ), NULL, this );
}

AboutDialog_AcknowledgmentsPage::~AboutDialog_AcknowledgmentsPage()
{
    // Disconnect Events
    thirdPartyLibrariesText->Disconnect( wxEVT_COMMAND_TEXT_URL, wxTextUrlEventHandler( AboutDialog_AcknowledgmentsPage::OnTextURL ), NULL, this );

}

ProgressDialog::ProgressDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* gaugeSizer;
    gaugeSizer = new wxBoxSizer( wxVERTICAL );

    totalProgressLabel = new wxStaticText( this, wxID_ANY, wxT("Total progress"), wxDefaultPosition, wxDefaultSize, 0 );
    totalProgressLabel->Wrap( -1 );
    gaugeSizer->Add( totalProgressLabel, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

    totalProgressGauge = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
    gaugeSizer->Add( totalProgressGauge, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

    progressLabel = new wxStaticText( this, wxID_ANY, wxT("Current progress"), wxDefaultPosition, wxDefaultSize, 0 );
    progressLabel->Wrap( -1 );
    gaugeSizer->Add( progressLabel, 0, wxALL, 5 );

    progressGauge = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
    gaugeSizer->Add( progressGauge, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


    topSizer->Add( gaugeSizer, 0, wxEXPAND, 5 );

    progressText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,150 ), wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY );
    progressText->SetMaxLength( 0 );
    progressText->SetMinSize( wxSize( 365,150 ) );

    topSizer->Add( progressText, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ProgressDialog::~ProgressDialog()
{
}

NodeAnimationSettingsDialog::NodeAnimationSettingsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 4, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    nameLabel = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    nameLabel->Wrap( -1 );
    gridSizer->Add( nameLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer* nameSizer;
    nameSizer = new wxBoxSizer( wxHORIZONTAL );

    nameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
    nameText->SetMaxLength( 0 );
    nameSizer->Add( nameText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );


    gridSizer->Add( nameSizer, 1, wxEXPAND, 5 );

    startEndFramesLabel = new wxStaticText( this, wxID_ANY, wxT("Start/End Frames"), wxDefaultPosition, wxDefaultSize, 0 );
    startEndFramesLabel->Wrap( -1 );
    gridSizer->Add( startEndFramesLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer* framesSizer;
    framesSizer = new wxBoxSizer( wxHORIZONTAL );

    startFrameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), 0 );
    startFrameText->SetMaxLength( 0 );
    framesSizer->Add( startFrameText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    inBetweenFrameLabel = new wxStaticText( this, wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
    inBetweenFrameLabel->Wrap( -1 );
    framesSizer->Add( inBetweenFrameLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    endFrameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), 0 );
    endFrameText->SetMaxLength( 0 );
    framesSizer->Add( endFrameText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );


    gridSizer->Add( framesSizer, 1, wxEXPAND, 5 );

    timeScalingLabel = new wxStaticText( this, wxID_ANY, wxT("Time Scaling"), wxDefaultPosition, wxDefaultSize, 0 );
    timeScalingLabel->Wrap( -1 );
    gridSizer->Add( timeScalingLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer* timeScalingSizer;
    timeScalingSizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString timeScalingChoiceChoices;
    timeScalingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, timeScalingChoiceChoices, 0 );
    timeScalingChoice->SetSelection( 0 );
    timeScalingSizer->Add( timeScalingChoice, 0, wxTOP|wxLEFT, 5 );

    timeScalingText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    timeScalingText->SetMaxLength( 0 );
    timeScalingSizer->Add( timeScalingText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );


    gridSizer->Add( timeScalingSizer, 1, wxEXPAND, 5 );

    samplingLabel = new wxStaticText( this, wxID_ANY, wxT("Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    samplingLabel->Wrap( -1 );
    gridSizer->Add( samplingLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer* samplingSizer;
    samplingSizer = new wxBoxSizer( wxHORIZONTAL );

    samplingText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    samplingText->SetMaxLength( 0 );
    samplingSizer->Add( samplingText, 0, wxTOP|wxLEFT, 5 );

    wxArrayString samplingChoiceChoices;
    samplingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, samplingChoiceChoices, 0 );
    samplingChoice->SetSelection( 0 );
    samplingSizer->Add( samplingChoice, 0, wxTOP|wxRIGHT|wxLEFT, 5 );


    gridSizer->Add( samplingSizer, 1, wxEXPAND, 5 );


    topSizer->Add( gridSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxFlexGridSizer* interpolationSizer;
    interpolationSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
    interpolationSizer->SetFlexibleDirection( wxBOTH );
    interpolationSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    translationInterpolationLabel = new wxStaticText( this, wxID_ANY, wxT("Interpolation"), wxDefaultPosition, wxDefaultSize, 0 );
    translationInterpolationLabel->Wrap( -1 );
    interpolationSizer->Add( translationInterpolationLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    rotationInterpolationLabel = new wxStaticText( this, wxID_ANY, wxT("Rotation Interpolation"), wxDefaultPosition, wxDefaultSize, 0 );
    rotationInterpolationLabel->Wrap( -1 );
    interpolationSizer->Add( rotationInterpolationLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxArrayString translationInterpolationChoiceChoices;
    translationInterpolationChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, translationInterpolationChoiceChoices, 0 );
    translationInterpolationChoice->SetSelection( 0 );
    interpolationSizer->Add( translationInterpolationChoice, 1, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxArrayString rotationInterpolationChoiceChoices;
    rotationInterpolationChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, rotationInterpolationChoiceChoices, 0 );
    rotationInterpolationChoice->SetSelection( 0 );
    interpolationSizer->Add( rotationInterpolationChoice, 1, wxTOP|wxRIGHT|wxLEFT, 5 );


    topSizer->Add( interpolationSizer, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* checkBoxesSizer;
    checkBoxesSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* embedSizer;
    embedSizer = new wxBoxSizer( wxHORIZONTAL );

    embedAnimationCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Embed Animation"), wxDefaultPosition, wxDefaultSize, 0 );
    embedSizer->Add( embedAnimationCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    linkToObjectCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Link to Node"), wxDefaultPosition, wxDefaultSize, 0 );
    embedSizer->Add( linkToObjectCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );


    checkBoxesSizer->Add( embedSizer, 0, wxEXPAND, 5 );

    enabledCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Enabled"), wxDefaultPosition, wxDefaultSize, 0 );
    checkBoxesSizer->Add( enabledCheckbox, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

    loopedCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Looped"), wxDefaultPosition, wxDefaultSize, 0 );
    checkBoxesSizer->Add( loopedCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    useDefinedAnimationStartTimeCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Use Defined Animation Start Time"), wxDefaultPosition, wxDefaultSize, 0 );
    checkBoxesSizer->Add( useDefinedAnimationStartTimeCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    copyFirstAnimationKeyToLastCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Copy First Animation Key to Last"), wxDefaultPosition, wxDefaultSize, 0 );
    checkBoxesSizer->Add( copyFirstAnimationKeyToLastCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );


    topSizer->Add( checkBoxesSizer, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT|wxTOP, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

NodeAnimationSettingsDialog::~NodeAnimationSettingsDialog()
{
}

MeshAnimationSettingsDialog::MeshAnimationSettingsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 8, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    nameLabel = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    nameLabel->Wrap( -1 );
    gridSizer->Add( nameLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxBoxSizer* nameSizer;
    nameSizer = new wxBoxSizer( wxHORIZONTAL );

    nameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
    nameText->SetMaxLength( 0 );
    nameSizer->Add( nameText, 0, wxRIGHT|wxLEFT, 5 );


    gridSizer->Add( nameSizer, 1, wxEXPAND, 5 );

    trackLabel = new wxStaticText( this, wxID_ANY, wxT("Track"), wxDefaultPosition, wxDefaultSize, 0 );
    trackLabel->Wrap( -1 );
    gridSizer->Add( trackLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxBoxSizer* trackSizer;
    trackSizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString trackChoiceChoices;
    trackChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 120,-1 ), trackChoiceChoices, 0 );
    trackChoice->SetSelection( 0 );
    trackSizer->Add( trackChoice, 0, wxLEFT|wxRIGHT|wxTOP, 5 );


    gridSizer->Add( trackSizer, 1, wxEXPAND, 5 );

    startEndFramesLabel = new wxStaticText( this, wxID_ANY, wxT("Start/End Frames"), wxDefaultPosition, wxDefaultSize, 0 );
    startEndFramesLabel->Wrap( -1 );
    gridSizer->Add( startEndFramesLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxBoxSizer* framesSizer;
    framesSizer = new wxBoxSizer( wxHORIZONTAL );

    startFrameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), 0 );
    startFrameText->SetMaxLength( 0 );
    framesSizer->Add( startFrameText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    inBetweenFrameLabel = new wxStaticText( this, wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
    inBetweenFrameLabel->Wrap( -1 );
    framesSizer->Add( inBetweenFrameLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );

    endFrameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), 0 );
    endFrameText->SetMaxLength( 0 );
    framesSizer->Add( endFrameText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );


    gridSizer->Add( framesSizer, 1, wxEXPAND, 5 );

    timeScalingLabel = new wxStaticText( this, wxID_ANY, wxT("Time Scaling"), wxDefaultPosition, wxDefaultSize, 0 );
    timeScalingLabel->Wrap( -1 );
    gridSizer->Add( timeScalingLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxBoxSizer* timeScalingSizer;
    timeScalingSizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString timeScalingChoiceChoices;
    timeScalingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, timeScalingChoiceChoices, 0 );
    timeScalingChoice->SetSelection( 0 );
    timeScalingSizer->Add( timeScalingChoice, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    timeScalingText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    timeScalingText->SetMaxLength( 0 );
    timeScalingSizer->Add( timeScalingText, 0, wxTOP|wxRIGHT, 5 );


    gridSizer->Add( timeScalingSizer, 1, wxEXPAND, 5 );

    samplingLabel = new wxStaticText( this, wxID_ANY, wxT("Sampling"), wxDefaultPosition, wxDefaultSize, 0 );
    samplingLabel->Wrap( -1 );
    gridSizer->Add( samplingLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxBoxSizer* samplingSizer;
    samplingSizer = new wxBoxSizer( wxHORIZONTAL );

    samplingText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    samplingText->SetMaxLength( 0 );
    samplingSizer->Add( samplingText, 0, wxTOP|wxLEFT, 5 );

    wxArrayString samplingChoiceChoices;
    samplingChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, samplingChoiceChoices, 0 );
    samplingChoice->SetSelection( 0 );
    samplingSizer->Add( samplingChoice, 0, wxTOP|wxRIGHT|wxLEFT, 5 );


    gridSizer->Add( samplingSizer, 1, wxEXPAND, 5 );

    morphWholeObjectLabel = new wxStaticText( this, wxID_ANY, wxT("Morph Whole Object"), wxDefaultPosition, wxDefaultSize, 0 );
    morphWholeObjectLabel->Wrap( -1 );
    gridSizer->Add( morphWholeObjectLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxArrayString morphWholeObjectChoiceChoices;
    morphWholeObjectChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, morphWholeObjectChoiceChoices, 0 );
    morphWholeObjectChoice->SetSelection( 0 );
    gridSizer->Add( morphWholeObjectChoice, 0, wxLEFT|wxRIGHT|wxTOP, 5 );


    topSizer->Add( gridSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* checkboxesSizer;
    checkboxesSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* embedSizer;
    embedSizer = new wxBoxSizer( wxHORIZONTAL );

    embedAnimationCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Embed Animation"), wxDefaultPosition, wxDefaultSize, 0 );
    embedSizer->Add( embedAnimationCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    linkToObjectCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Link to Object"), wxDefaultPosition, wxDefaultSize, 0 );
    embedSizer->Add( linkToObjectCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );


    checkboxesSizer->Add( embedSizer, 0, wxEXPAND, 5 );

    copyFirstAnimationKeyToLastCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Copy First Animation Key to Last"), wxDefaultPosition, wxDefaultSize, 0 );
    checkboxesSizer->Add( copyFirstAnimationKeyToLastCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    exportAsMorphCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export as Morph"), wxDefaultPosition, wxDefaultSize, 0 );
    checkboxesSizer->Add( exportAsMorphCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* animatedRootStartFrameSizer;
    animatedRootStartFrameSizer = new wxBoxSizer( wxHORIZONTAL );

    overrideAnimatedRootStartTimeCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Override Animated Root Start Frame"), wxDefaultPosition, wxDefaultSize, 0 );
    animatedRootStartFrameSizer->Add( overrideAnimatedRootStartTimeCheckbox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    animatedRootStartTimeText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), 0 );
    animatedRootStartTimeText->SetMaxLength( 0 );
    animatedRootStartFrameSizer->Add( animatedRootStartTimeText, 0, wxRIGHT|wxLEFT, 5 );


    checkboxesSizer->Add( animatedRootStartFrameSizer, 1, wxEXPAND, 5 );


    topSizer->Add( checkboxesSizer, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

    skeletonAnimationSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Skeleton Animation") ), wxVERTICAL );

    wxGridSizer* removeBoneTranslationsSizer;
    removeBoneTranslationsSizer = new wxGridSizer( 1, 2, 0, 0 );

    removeBoneTranslationsLabel = new wxStaticText( this, wxID_ANY, wxT("Remove Bone Translations"), wxDefaultPosition, wxDefaultSize, 0 );
    removeBoneTranslationsLabel->Wrap( -1 );
    removeBoneTranslationsSizer->Add( removeBoneTranslationsLabel, 0, wxALIGN_BOTTOM|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    addRemoveTranslationsBoneButton = new wxButton( this, wxID_ANY, wxT("Add Bone..."), wxDefaultPosition, wxDefaultSize, 0 );
    removeBoneTranslationsSizer->Add( addRemoveTranslationsBoneButton, 0, wxALIGN_RIGHT|wxRIGHT|wxLEFT, 5 );


    skeletonAnimationSizer->Add( removeBoneTranslationsSizer, 0, wxEXPAND, 5 );

    removeBoneTranslationsText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,100 ), wxTE_DONTWRAP|wxTE_MULTILINE );
    removeBoneTranslationsText->SetMaxLength( 0 );
    skeletonAnimationSizer->Add( removeBoneTranslationsText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    wxGridSizer* includedExcludeBonesSizer;
    includedExcludeBonesSizer = new wxGridSizer( 1, 2, 0, 0 );

    includeExcludeBonesLabel = new wxStaticText( this, wxID_ANY, wxT("Include/Exclude Bones"), wxDefaultPosition, wxDefaultSize, 0 );
    includeExcludeBonesLabel->Wrap( -1 );
    includedExcludeBonesSizer->Add( includeExcludeBonesLabel, 0, wxALIGN_BOTTOM|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    includeExcludeBonesButton = new wxButton( this, wxID_ANY, wxT("Add Bone..."), wxDefaultPosition, wxDefaultSize, 0 );
    includedExcludeBonesSizer->Add( includeExcludeBonesButton, 0, wxALIGN_RIGHT|wxRIGHT|wxLEFT, 5 );


    skeletonAnimationSizer->Add( includedExcludeBonesSizer, 0, wxEXPAND, 5 );

    includeExcludeBonesText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,100 ), wxTE_DONTWRAP|wxTE_MULTILINE );
    includeExcludeBonesText->SetMaxLength( 0 );
    skeletonAnimationSizer->Add( includeExcludeBonesText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    topSizer->Add( skeletonAnimationSizer, 1, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

MeshAnimationSettingsDialog::~MeshAnimationSettingsDialog()
{
}

ConvertToFramesDialog::ConvertToFramesDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* sourceFileSizer;
    sourceFileSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Source File") ), wxVERTICAL );

    sourceFileDescriptionLabel = new wxStaticText( this, wxID_ANY, wxT("The video file to be converted to frames."), wxDefaultPosition, wxDefaultSize, 0 );
    sourceFileDescriptionLabel->Wrap( -1 );
    sourceFileSizer->Add( sourceFileDescriptionLabel, 0, wxALL, 5 );

    wxBoxSizer* sourceFileHorizontalSizer;
    sourceFileHorizontalSizer = new wxBoxSizer( wxHORIZONTAL );

    sourceFileText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 250,-1 ), 0 );
    sourceFileText->SetMaxLength( 0 );
    sourceFileHorizontalSizer->Add( sourceFileText, 0, wxEXPAND|wxALL, 5 );

    sourceFileButton = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
    sourceFileHorizontalSizer->Add( sourceFileButton, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );


    sourceFileSizer->Add( sourceFileHorizontalSizer, 1, wxEXPAND, 5 );


    topSizer->Add( sourceFileSizer, 0, wxEXPAND, 5 );

    wxStaticBoxSizer* destinationDirectorySizer;
    destinationDirectorySizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Destination Directory") ), wxVERTICAL );

    destinationDirectoryDescriptionLabel = new wxStaticText( this, wxID_ANY, wxT("The directory that will contain the generated frames. Leave empty to \nuse the source file directory."), wxDefaultPosition, wxDefaultSize, 0 );
    destinationDirectoryDescriptionLabel->Wrap( -1 );
    destinationDirectorySizer->Add( destinationDirectoryDescriptionLabel, 0, wxALL, 5 );

    wxBoxSizer* destinationDirectoryHorizontalSizer;
    destinationDirectoryHorizontalSizer = new wxBoxSizer( wxHORIZONTAL );

    destinationDirectoryText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 250,-1 ), 0 );
    destinationDirectoryText->SetMaxLength( 0 );
    destinationDirectoryHorizontalSizer->Add( destinationDirectoryText, 0, wxALL, 5 );

    destinationDirectoryButton = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
    destinationDirectoryHorizontalSizer->Add( destinationDirectoryButton, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );


    destinationDirectorySizer->Add( destinationDirectoryHorizontalSizer, 1, wxEXPAND, 5 );


    topSizer->Add( destinationDirectorySizer, 0, wxEXPAND, 5 );

    wxStaticBoxSizer* destinationBaseFileNameSizer;
    destinationBaseFileNameSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Destination Base File Name") ), wxVERTICAL );

    destinationBaseFileNameDescriptionLabel = new wxStaticText( this, wxID_ANY, wxT("The base file name of the generated frames. For example: 'frame.jpg'"), wxDefaultPosition, wxDefaultSize, 0 );
    destinationBaseFileNameDescriptionLabel->Wrap( -1 );
    destinationBaseFileNameSizer->Add( destinationBaseFileNameDescriptionLabel, 0, wxALL, 5 );

    destinationBaseFileNameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    destinationBaseFileNameText->SetMaxLength( 0 );
    destinationBaseFileNameSizer->Add( destinationBaseFileNameText, 0, wxEXPAND|wxALL, 5 );


    topSizer->Add( destinationBaseFileNameSizer, 0, wxEXPAND, 5 );

    wxStaticBoxSizer* sizeSizer;
    sizeSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Size") ), wxVERTICAL );

    originalSizeButton = new wxRadioButton( this, wxID_ANY, wxT("Original"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    sizeSizer->Add( originalSizeButton, 0, wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* customSizeSizer;
    customSizeSizer = new wxBoxSizer( wxHORIZONTAL );

    customSizeButton = new wxRadioButton( this, wxID_ANY, wxT("Custom"), wxDefaultPosition, wxDefaultSize, 0 );
    customSizeSizer->Add( customSizeButton, 0, wxALIGN_TOP|wxALL, 5 );

    customWidthText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
    customWidthText->SetMaxLength( 0 );
    customSizeSizer->Add( customWidthText, 0, wxRIGHT|wxLEFT, 5 );

    customSizeXLabel = new wxStaticText( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
    customSizeXLabel->Wrap( -1 );
    customSizeSizer->Add( customSizeXLabel, 0, wxALL, 5 );

    customHeightText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
    customHeightText->SetMaxLength( 0 );
    customSizeSizer->Add( customHeightText, 0, wxRIGHT|wxLEFT, 5 );


    sizeSizer->Add( customSizeSizer, 1, wxEXPAND, 5 );


    topSizer->Add( sizeSizer, 0, wxEXPAND, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ConvertToFramesDialog::~ConvertToFramesDialog()
{
}

FlagAliasDialog::FlagAliasDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    nameLabel = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    nameLabel->Wrap( -1 );
    gridSizer->Add( nameLabel, 0, wxALL, 5 );

    nameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), 0 );
    nameText->SetMaxLength( 0 );
    gridSizer->Add( nameText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    bitLabel = new wxStaticText( this, wxID_ANY, wxT("Bit"), wxDefaultPosition, wxDefaultSize, 0 );
    bitLabel->Wrap( -1 );
    gridSizer->Add( bitLabel, 0, wxALL, 5 );

    bitText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
    bitText->SetMaxLength( 0 );
    gridSizer->Add( bitText, 0, wxALL, 5 );


    topSizer->Add( gridSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_CENTER, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

FlagAliasDialog::~FlagAliasDialog()
{
}

FlagDialog::FlagDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 1, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    nameText = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    nameText->Wrap( -1 );
    gridSizer->Add( nameText, 0, wxALL, 5 );

    wxArrayString nameChoiceChoices;
    nameChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, nameChoiceChoices, 0 );
    nameChoice->SetSelection( 0 );
    gridSizer->Add( nameChoice, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    topSizer->Add( gridSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_CENTER, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

FlagDialog::~FlagDialog()
{
}

IncludeExcludeBoneDialog::IncludeExcludeBoneDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* boneSizer;
    boneSizer = new wxBoxSizer( wxHORIZONTAL );

    boneLabel = new wxStaticText( this, wxID_ANY, wxT("Bone"), wxDefaultPosition, wxDefaultSize, 0 );
    boneLabel->Wrap( -1 );
    boneSizer->Add( boneLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    boneEdit = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 160,-1 ), wxTE_READONLY );
    boneEdit->SetMaxLength( 0 );
    boneSizer->Add( boneEdit, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    boneButton = new wxButton( this, wxID_ANY, wxT("Choose..."), wxDefaultPosition, wxDefaultSize, 0 );
    boneSizer->Add( boneButton, 0, wxTOP|wxRIGHT, 5 );


    topSizer->Add( boneSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* includeExcludeSizer;
    includeExcludeSizer = new wxBoxSizer( wxHORIZONTAL );

    includeButton = new wxRadioButton( this, wxID_ANY, wxT("Include"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    includeExcludeSizer->Add( includeButton, 0, wxALL, 5 );

    excludeButton = new wxRadioButton( this, wxID_ANY, wxT("Exclude"), wxDefaultPosition, wxDefaultSize, 0 );
    includeExcludeSizer->Add( excludeButton, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );


    topSizer->Add( includeExcludeSizer, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* applyRecursivesSizer;
    applyRecursivesSizer = new wxBoxSizer( wxVERTICAL );

    applyRecursiveCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Apply to Bone and Descendants"), wxDefaultPosition, wxDefaultSize, 0 );
    applyRecursivesSizer->Add( applyRecursiveCheckbox, 0, wxALL, 5 );


    topSizer->Add( applyRecursivesSizer, 1, wxEXPAND|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

IncludeExcludeBoneDialog::~IncludeExcludeBoneDialog()
{
}

RenameDialog::RenameDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxHORIZONTAL );

    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    fromLabel = new wxStaticText( this, wxID_ANY, wxT("From"), wxDefaultPosition, wxDefaultSize, 0 );
    fromLabel->Wrap( -1 );
    gridSizer->Add( fromLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    fromText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 160,-1 ), 0 );
    fromText->SetMaxLength( 0 );
    gridSizer->Add( fromText, 0, wxALL, 5 );

    toLabel = new wxStaticText( this, wxID_ANY, wxT("To"), wxDefaultPosition, wxDefaultSize, 0 );
    toLabel->Wrap( -1 );
    gridSizer->Add( toLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    toText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 160,-1 ), 0 );
    toText->SetMaxLength( 0 );
    gridSizer->Add( toText, 0, wxALL, 5 );


    topSizer->Add( gridSizer, 1, wxEXPAND|wxALL, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxVERTICAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

RenameDialog::~RenameDialog()
{
}

AddDialog::AddDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer* addSizer;
    addSizer = new wxBoxSizer( wxVERTICAL );

    addLabel = new wxStaticText( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
    addLabel->Wrap( -1 );
    addSizer->Add( addLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    addText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 160,-1 ), 0 );
    addText->SetMaxLength( 0 );
    addSizer->Add( addText, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    topSizer->Add( addSizer, 1, wxALIGN_CENTER_VERTICAL, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxVERTICAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_CENTER_VERTICAL, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

AddDialog::~AddDialog()
{
}

RenderCubeMapDialog::RenderCubeMapDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    informationLabel = new wxStaticText( this, wxID_ANY, wxT("The cube map will be rendered from a camera at the world origin."), wxDefaultPosition, wxDefaultSize, 0 );
    informationLabel->Wrap( -1 );
    topSizer->Add( informationLabel, 0, wxALL, 5 );


    topSizer->Add( 0, 5, 0, 0, 5 );

    wxBoxSizer* cubeFaceSizeSizer;
    cubeFaceSizeSizer = new wxBoxSizer( wxHORIZONTAL );

    cubeFaceSizeLabel = new wxStaticText( this, wxID_ANY, wxT("Cube Face Width/Height"), wxDefaultPosition, wxDefaultSize, 0 );
    cubeFaceSizeLabel->Wrap( -1 );
    cubeFaceSizeSizer->Add( cubeFaceSizeLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    cubeFaceSizeEdit = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
    cubeFaceSizeEdit->SetMaxLength( 0 );
    cubeFaceSizeSizer->Add( cubeFaceSizeEdit, 0, wxRIGHT|wxLEFT, 5 );


    topSizer->Add( cubeFaceSizeSizer, 0, wxEXPAND, 5 );

    separateFilesButton = new wxRadioButton( this, wxID_ANY, wxT("Separate Files"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    topSizer->Add( separateFilesButton, 0, wxALL, 5 );

    wxFlexGridSizer* separateFilesSizer;
    separateFilesSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
    separateFilesSizer->SetFlexibleDirection( wxBOTH );
    separateFilesSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


    separateFilesSizer->Add( 20, 0, 1, wxEXPAND, 5 );

    wxStaticBoxSizer* destinationDirectorySizer;
    destinationDirectorySizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Destination Directory") ), wxVERTICAL );

    destinationDirectoryDescriptionLabel = new wxStaticText( this, wxID_ANY, wxT("The directory that will contain the generated cube map images."), wxDefaultPosition, wxDefaultSize, 0 );
    destinationDirectoryDescriptionLabel->Wrap( -1 );
    destinationDirectorySizer->Add( destinationDirectoryDescriptionLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* bSizer200;
    bSizer200 = new wxBoxSizer( wxHORIZONTAL );

    destinationDirectoryText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 250,-1 ), 0 );
    destinationDirectoryText->SetMaxLength( 0 );
    bSizer200->Add( destinationDirectoryText, 0, wxALL, 5 );

    destinationDirectoryButton = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer200->Add( destinationDirectoryButton, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );


    destinationDirectorySizer->Add( bSizer200, 1, wxEXPAND, 5 );


    separateFilesSizer->Add( destinationDirectorySizer, 1, wxEXPAND, 5 );


    separateFilesSizer->Add( 20, 0, 1, wxEXPAND, 5 );

    wxStaticBoxSizer* destinationBaseFileNameSizer;
    destinationBaseFileNameSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Destination Base File Name") ), wxVERTICAL );

    destinationBaseFileNameDescriptionLabel = new wxStaticText( this, wxID_ANY, wxT("The base file name of the generated images. For example: 'sky.jpg'"), wxDefaultPosition, wxDefaultSize, 0 );
    destinationBaseFileNameDescriptionLabel->Wrap( -1 );
    destinationBaseFileNameSizer->Add( destinationBaseFileNameDescriptionLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    destinationBaseFileNameText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 180,-1 ), 0 );
    destinationBaseFileNameText->SetMaxLength( 0 );
    destinationBaseFileNameSizer->Add( destinationBaseFileNameText, 0, wxALL, 5 );


    separateFilesSizer->Add( destinationBaseFileNameSizer, 1, wxEXPAND, 5 );


    topSizer->Add( separateFilesSizer, 0, wxEXPAND, 5 );

    ddsFileButton = new wxRadioButton( this, wxID_ANY, wxT("DDS File"), wxDefaultPosition, wxDefaultSize, 0 );
    topSizer->Add( ddsFileButton, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxFlexGridSizer* ddsFileSizer;
    ddsFileSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
    ddsFileSizer->SetFlexibleDirection( wxBOTH );
    ddsFileSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


    ddsFileSizer->Add( 20, 0, 1, wxEXPAND, 5 );

    wxBoxSizer* ddsFileNameSizer;
    ddsFileNameSizer = new wxBoxSizer( wxHORIZONTAL );

    ddsFileText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 250,-1 ), 0 );
    ddsFileText->SetMaxLength( 0 );
    ddsFileNameSizer->Add( ddsFileText, 0, wxALL, 5 );

    ddsFileButton = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
    ddsFileNameSizer->Add( ddsFileButton, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );


    ddsFileSizer->Add( ddsFileNameSizer, 1, wxEXPAND, 5 );


    topSizer->Add( ddsFileSizer, 0, wxEXPAND, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

RenderCubeMapDialog::~RenderCubeMapDialog()
{
}

RenderTerrainMapDialog::RenderTerrainMapDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    informationLabel = new wxStaticText( this, wxID_ANY, wxT("The height map will be rendered with all world objects."), wxDefaultPosition, wxDefaultSize, 0 );
    informationLabel->Wrap( -1 );
    topSizer->Add( informationLabel, 0, wxALL, 5 );


    topSizer->Add( 0, 5, 0, 0, 5 );

    wxBoxSizer* bSizer207;
    bSizer207 = new wxBoxSizer( wxHORIZONTAL );

    dimensionsLabel = new wxStaticText( this, wxID_ANY, wxT("Dimensions"), wxDefaultPosition, wxDefaultSize, 0 );
    dimensionsLabel->Wrap( -1 );
    bSizer207->Add( dimensionsLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    widthEdit = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
    widthEdit->SetMaxLength( 0 );
    bSizer207->Add( widthEdit, 0, wxRIGHT|wxLEFT, 5 );

    dimensionsXLabel = new wxStaticText( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
    dimensionsXLabel->Wrap( -1 );
    bSizer207->Add( dimensionsXLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    heightText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
    heightText->SetMaxLength( 0 );
    bSizer207->Add( heightText, 0, wxRIGHT|wxLEFT, 5 );

    squareCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Square"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer207->Add( squareCheckbox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


    topSizer->Add( bSizer207, 0, wxEXPAND, 5 );

    wxBoxSizer* rotateSizer;
    rotateSizer = new wxBoxSizer( wxHORIZONTAL );

    rotate90DegreesClockwiseCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Rotate 90 Degrees Clockwise"), wxDefaultPosition, wxDefaultSize, 0 );
    rotateSizer->Add( rotate90DegreesClockwiseCheckbox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    bytesPerPixelLabel = new wxStaticText( this, wxID_ANY, wxT("Bytes Per Pixel"), wxDefaultPosition, wxDefaultSize, 0 );
    bytesPerPixelLabel->Wrap( -1 );
    rotateSizer->Add( bytesPerPixelLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    bytesPerPixelEdit = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
    bytesPerPixelEdit->SetMaxLength( 0 );
    rotateSizer->Add( bytesPerPixelEdit, 0, wxALL, 5 );


    topSizer->Add( rotateSizer, 0, wxEXPAND, 5 );

    destinationFileLabel = new wxStaticText( this, wxID_ANY, wxT("Destination File"), wxDefaultPosition, wxDefaultSize, 0 );
    destinationFileLabel->Wrap( -1 );
    topSizer->Add( destinationFileLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* destinationFileSizer;
    destinationFileSizer = new wxBoxSizer( wxHORIZONTAL );

    destinationFileText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 280,-1 ), 0 );
    destinationFileText->SetMaxLength( 0 );
    destinationFileSizer->Add( destinationFileText, 0, wxALL, 5 );

    destinationFileButton = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
    destinationFileSizer->Add( destinationFileButton, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );


    topSizer->Add( destinationFileSizer, 0, wxEXPAND, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

RenderTerrainMapDialog::~RenderTerrainMapDialog()
{
}

TextInputDialog::TextInputDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    descriptionalLabel = new wxStaticText( this, wxID_ANY, wxT("The height map will be rendered with all world objects. The height map will be rendered with all world objects. asdfasdfasdfasdfasdf"), wxDefaultPosition, wxDefaultSize, 0 );
    descriptionalLabel->Wrap( 280 );
    topSizer->Add( descriptionalLabel, 0, wxALL, 10 );

    inputText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 280,-1 ), 0 );
    inputText->SetMaxLength( 0 );
    topSizer->Add( inputText, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

TextInputDialog::~TextInputDialog()
{
}

ManualLodDialog::ManualLodDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer* gridSizer;
    gridSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
    gridSizer->SetFlexibleDirection( wxBOTH );
    gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    objectLabel = new wxStaticText( this, wxID_ANY, wxT("Object"), wxDefaultPosition, wxDefaultSize, 0 );
    objectLabel->Wrap( -1 );
    gridSizer->Add( objectLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer* nodeSizer;
    nodeSizer = new wxBoxSizer( wxHORIZONTAL );

    objectEdit = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 160,-1 ), wxTE_READONLY );
    objectEdit->SetMaxLength( 0 );
    nodeSizer->Add( objectEdit, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    objectButton = new wxButton( this, wxID_ANY, wxT("Choose..."), wxDefaultPosition, wxDefaultSize, 0 );
    nodeSizer->Add( objectButton, 0, wxTOP|wxRIGHT, 5 );


    gridSizer->Add( nodeSizer, 1, wxEXPAND, 5 );

    distanceLabel = new wxStaticText( this, wxID_ANY, wxT("Distance"), wxDefaultPosition, wxDefaultSize, 0 );
    distanceLabel->Wrap( -1 );
    gridSizer->Add( distanceLabel, 0, wxALL, 5 );

    wxBoxSizer* distanceSizer;
    distanceSizer = new wxBoxSizer( wxHORIZONTAL );

    distanceText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    distanceText->SetMaxLength( 0 );
    distanceSizer->Add( distanceText, 0, wxTOP|wxRIGHT|wxLEFT, 5 );


    gridSizer->Add( distanceSizer, 1, wxEXPAND, 5 );


    topSizer->Add( gridSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ManualLodDialog::~ManualLodDialog()
{
}

ChooseObjectsDialog::ChooseObjectsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxSize( 380,456 ), wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* nameSizer;
    nameSizer = new wxBoxSizer( wxVERTICAL );

    nameLabel = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    nameLabel->Wrap( -1 );
    nameSizer->Add( nameLabel, 0, wxALL, 5 );

    nameTextBox = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
    nameTextBox->SetMaxLength( 0 );
    nameTextBox->SetMinSize( wxSize( 200,-1 ) );

    nameSizer->Add( nameTextBox, 1, wxEXPAND|wxBOTTOM|wxLEFT, 5 );


    topSizer->Add( nameSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* objectsListSizer;
    objectsListSizer = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer* listBoxSizer;
    listBoxSizer = new wxBoxSizer( wxVERTICAL );

    showFullNamesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Show Full Names"), wxDefaultPosition, wxDefaultSize, 0 );
    listBoxSizer->Add( showFullNamesCheckbox, 0, wxLEFT|wxTOP, 5 );

    objectsListBox = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_EXTENDED|wxLB_MULTIPLE|wxLB_NEEDED_SB|wxLB_SINGLE );
    objectsListBox->SetMinSize( wxSize( 200,250 ) );

    listBoxSizer->Add( objectsListBox, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );


    objectsListSizer->Add( listBoxSizer, 1, wxEXPAND, 5 );

    wxStaticBoxSizer* typesSizer;
    typesSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Types") ), wxVERTICAL );

    typesSizer->SetMinSize( wxSize( 110,-1 ) );
    meshesCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Meshes"), wxDefaultPosition, wxDefaultSize, 0 );
    typesSizer->Add( meshesCheckBox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    planesCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Planes"), wxDefaultPosition, wxDefaultSize, 0 );
    typesSizer->Add( planesCheckBox, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

    lightsCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Lights"), wxDefaultPosition, wxDefaultSize, 0 );
    typesSizer->Add( lightsCheckBox, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    camerasCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Cameras"), wxDefaultPosition, wxDefaultSize, 0 );
    typesSizer->Add( camerasCheckBox, 0, wxLEFT|wxRIGHT|wxTOP, 5 );

    otherCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Other"), wxDefaultPosition, wxDefaultSize, 0 );
    typesSizer->Add( otherCheckBox, 0, wxALL, 5 );


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


    topSizer->Add( buttonsSizer, 0, wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ChooseObjectsDialog::~ChooseObjectsDialog()
{
}

ChooseFilesDialog::ChooseFilesDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxSize( 380,456 ), wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* nameSizer;
    nameSizer = new wxBoxSizer( wxVERTICAL );

    nameLabel = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    nameLabel->Wrap( -1 );
    nameSizer->Add( nameLabel, 0, wxALL, 5 );

    nameTextBox = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
    nameTextBox->SetMaxLength( 0 );
    nameTextBox->SetMinSize( wxSize( 200,-1 ) );

    nameSizer->Add( nameTextBox, 1, wxBOTTOM|wxEXPAND|wxLEFT, 5 );


    topSizer->Add( nameSizer, 0, wxEXPAND|wxRIGHT|wxTOP, 5 );

    wxBoxSizer* filesListSizer;
    filesListSizer = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer* listBoxSizer;
    listBoxSizer = new wxBoxSizer( wxVERTICAL );

    filesListBox = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_EXTENDED|wxLB_MULTIPLE|wxLB_NEEDED_SB|wxLB_SINGLE );
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


    this->SetSizer( topSizer );
    this->Layout();
}

ChooseFilesDialog::~ChooseFilesDialog()
{
}

ChooseMaterialsDialog::ChooseMaterialsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxSize( 380,456 ), wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* nameSizer;
    nameSizer = new wxBoxSizer( wxVERTICAL );

    nameLabel = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    nameLabel->Wrap( -1 );
    nameSizer->Add( nameLabel, 0, wxALL, 5 );

    nameTextBox = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
    nameTextBox->SetMaxLength( 0 );
    nameTextBox->SetMinSize( wxSize( 200,-1 ) );

    nameSizer->Add( nameTextBox, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    topSizer->Add( nameSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* objectsListSizer;
    objectsListSizer = new wxBoxSizer( wxVERTICAL );

    showFullNamesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Show Full Names"), wxDefaultPosition, wxDefaultSize, 0 );
    objectsListSizer->Add( showFullNamesCheckbox, 0, wxLEFT|wxTOP, 5 );

    objectsListBox = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_EXTENDED|wxLB_MULTIPLE|wxLB_NEEDED_SB|wxLB_SINGLE );
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


    topSizer->Add( buttonsSizer, 0, wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

ChooseMaterialsDialog::~ChooseMaterialsDialog()
{
}

FinjinMaterialFileRemappingDialog::FinjinMaterialFileRemappingDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* invalidFilesSizer;
    invalidFilesSizer = new wxBoxSizer( wxVERTICAL );

    invalidFilesLabel = new wxStaticText( this, wxID_ANY, wxT("The following files are referenced by Finjin materials but could not be located."), wxDefaultPosition, wxDefaultSize, 0 );
    invalidFilesLabel->Wrap( -1 );
    invalidFilesSizer->Add( invalidFilesLabel, 0, wxALL, 5 );

    invalidFilesListBox = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_NEEDED_SB|wxLB_SINGLE );
    invalidFilesListBox->SetMinSize( wxSize( 200,250 ) );

    invalidFilesSizer->Add( invalidFilesListBox, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


    topSizer->Add( invalidFilesSizer, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* directoriesSizer;
    directoriesSizer = new wxBoxSizer( wxVERTICAL );

    directoriesLabel = new wxStaticText( this, wxID_ANY, wxT("Add one or more directories to search for the files listed above."), wxDefaultPosition, wxDefaultSize, 0 );
    directoriesLabel->Wrap( -1 );
    directoriesSizer->Add( directoriesLabel, 0, wxALL, 5 );

    directoriesText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 305,-1 ), wxTE_DONTWRAP|wxTE_MULTILINE );
    directoriesText->SetMaxLength( 0 );
    directoriesSizer->Add( directoriesText, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


    topSizer->Add( directoriesSizer, 1, wxEXPAND|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* buttonsSizer;
    buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer* addRemoveSizer;
    addRemoveSizer = new wxBoxSizer( wxHORIZONTAL );

    addButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxDefaultSize, 0 );
    addRemoveSizer->Add( addButton, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    searchButton = new wxButton( this, wxID_ANY, wxT("Search..."), wxDefaultPosition, wxDefaultSize, 0 );
    addRemoveSizer->Add( searchButton, 0, wxALL, 5 );


    buttonsSizer->Add( addRemoveSizer, 1, wxEXPAND, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxBOTTOM|wxLEFT|wxTOP, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxTOP, 5 );


    buttonsSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT|wxEXPAND, 5 );


    topSizer->Add( buttonsSizer, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

FinjinMaterialFileRemappingDialog::~FinjinMaterialFileRemappingDialog()
{
}

FinjinLogFileDialog::FinjinLogFileDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* logSizer;
    logSizer = new wxBoxSizer( wxVERTICAL );

    logText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 305,-1 ), wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY );
    logText->SetMaxLength( 0 );
    logSizer->Add( logText, 1, wxALL|wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


    topSizer->Add( logSizer, 1, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

FinjinLogFileDialog::~FinjinLogFileDialog()
{
}

FinjinInstallationDetailsDialog::FinjinInstallationDetailsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* resultsSizer;
    resultsSizer = new wxBoxSizer( wxVERTICAL );

    resultsText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 305,-1 ), wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY );
    resultsText->SetMaxLength( 0 );
    resultsSizer->Add( resultsText, 1, wxALL|wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


    topSizer->Add( resultsSizer, 1, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );


    topSizer->Add( okCancelSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


    this->SetSizer( topSizer );
    this->Layout();
}

FinjinInstallationDetailsDialog::~FinjinInstallationDetailsDialog()
{
}
