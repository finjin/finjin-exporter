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
#include "GlobalSettingsDialog_GeneralPage.hpp"
#include "FinjinGlobalSettings.hpp"
#include "Strings.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "ApplicationAccessor.hpp"
#include "RelativeLocationPrompt.hpp"
#include "Dialogs.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
const wxString GlobalSettingsDialog_GeneralPage::TITLE(wxT("General"));


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(GlobalSettingsDialog_GeneralPage, SettingsPage)
    EVT_RADIOBUTTON(GlobalSettingsDialog_GeneralPage::FINJIN_VIEWER_RADIO_BUTTON, GlobalSettingsDialog_GeneralPage::OnFinjinViewerRadioButton)    
END_EVENT_TABLE()

GlobalSettingsDialog_GeneralPage::GlobalSettingsDialog_GeneralPage(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxStaticBoxSizer* exportFormatSizer;
    exportFormatSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("File Format") ), wxVERTICAL );
    
    wxBoxSizer* textBlobFormatHorizontalSizer;
    textBlobFormatHorizontalSizer = new wxBoxSizer( wxHORIZONTAL );
    
    textBlobFormatLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Text File Blob Format"), wxDefaultPosition, wxSize( 140,-1 ), 0 );
    textBlobFormatLabel->Wrap( -1 );
    textBlobFormatHorizontalSizer->Add( textBlobFormatLabel, 0, wxALL, 5 );
    
    wxArrayString textBlobFormatChoiceChoices;
    textBlobFormatChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, textBlobFormatChoiceChoices, 0 );
    textBlobFormatChoice->SetSelection( 0 );
    textBlobFormatChoice->SetMinSize( wxSize( 80,-1 ) );
    
    textBlobFormatHorizontalSizer->Add( textBlobFormatChoice, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    blobTextFormatDefaultLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
    blobTextFormatDefaultLabel->Wrap( -1 );
    textBlobFormatHorizontalSizer->Add( blobTextFormatDefaultLabel, 0, wxALL, 5 );
    
    
    exportFormatSizer->Add( textBlobFormatHorizontalSizer, 0, wxEXPAND, 5 );
    
    wxBoxSizer* maxBytesPerLineSizer;
    maxBytesPerLineSizer = new wxBoxSizer( wxHORIZONTAL );
    
    maxBytesPerLineLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Maximum Bytes Per Line"), wxDefaultPosition, wxSize( 140,-1 ), 0 );
    maxBytesPerLineLabel->Wrap( -1 );
    maxBytesPerLineSizer->Add( maxBytesPerLineLabel, 0, wxALL, 5 );
    
    maxBytesPerLineSpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(70, -1), 0, FINJIN_EXPORTER_MIN_BYTES_PER_LINE, FINJIN_EXPORTER_MAX_BYTES_PER_LINE, SpinnerControl::INT_UNIT);
    maxBytesPerLineSizer->Add( maxBytesPerLineSpinner, 0, wxBOTTOM|wxLEFT|wxRIGHT, 5 );
    
    
    exportFormatSizer->Add( maxBytesPerLineSizer, 0, wxEXPAND, 5 );
    
    wxBoxSizer* alwaysExportSizer;
    alwaysExportSizer = new wxBoxSizer( wxHORIZONTAL );
    
    alwaysExportLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Additional Export Formats"), wxDefaultPosition, wxSize( 140,-1 ), 0 );
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
    
    embedLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Embed Files"), wxDefaultPosition, wxSize( 140,-1 ), 0 );
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

    topSizer->Add( exportFormatSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* headerExportSizer;
    headerExportSizer = new wxBoxSizer( wxHORIZONTAL );
    
    exportLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Export"), wxDefaultPosition, wxSize( 140,-1 ), 0 );
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
    headerExportSizer->Add( exportTimestampCheckbox, 0, wxALL, 5 );

    exportExporterVersionCheckbox = new wxCheckBox(this, wxID_ANY, wxT("Exporter Version"), wxDefaultPosition, wxDefaultSize, 0);
    exportExporterVersionCheckbox->SetValue(true);
    headerExportSizer->Add(exportExporterVersionCheckbox, 0, wxALL, 5);

    exportFormatSizer->Add( headerExportSizer, 1, wxEXPAND, 5 );

    wxStaticBoxSizer* exportDirectoryTopSizer;
    exportDirectoryTopSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Export Directory") ), wxVERTICAL );
    
    exportDirectoryDescriptionLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("The export directory contains all of the mesh, material and other asset directories that are generated\nwhen a scene is exported. By default it is the same as the exported scene directory, but you may\nadd a path relative to that directory."), wxDefaultPosition, wxDefaultSize, 0 );
    exportDirectoryDescriptionLabel->Wrap( -1 );
    exportDirectoryTopSizer->Add( exportDirectoryDescriptionLabel, 0, wxALL|wxEXPAND, 5 );
    
    wxBoxSizer* exportDirectorySizer;
    exportDirectorySizer = new wxBoxSizer( wxHORIZONTAL );
    
    exportDirectoryLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Directory"), wxDefaultPosition, wxDefaultSize, 0 );
    exportDirectoryLabel->Wrap( -1 );
    exportDirectorySizer->Add( exportDirectoryLabel, 0, wxALL, 5 );
    
    exportDirectoryText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 440,-1 ), 0 );
    exportDirectoryText->SetMaxLength( 0 ); 
    exportDirectorySizer->Add( exportDirectoryText, 0, wxLEFT|wxRIGHT, 5 );
    
    
    exportDirectoryTopSizer->Add( exportDirectorySizer, 1, wxEXPAND, 5 );
    
    
    topSizer->Add( exportDirectoryTopSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
    
    wxStaticBoxSizer* externalViewerSettingsSizer;
    externalViewerSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("External Viewer") ), wxVERTICAL );
    
    finjinViewerButton = new wxRadioButton( this, wxID_ANY, wxT("Finjin Viewer"), wxDefaultPosition, wxDefaultSize, wxRB_SINGLE );
    externalViewerSettingsSizer->Add( finjinViewerButton, 0, wxALL, 5 );
    
    
    externalViewerSettingsSizer->Add( 0, 4, 1, wxEXPAND, 5 );
    
    runExternalViewerAfterExportCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Run After Export"), wxDefaultPosition, wxDefaultSize, 0 );
    externalViewerSettingsSizer->Add( runExternalViewerAfterExportCheckbox, 0, wxALL, 5 );
    
    
    topSizer->Add( externalViewerSettingsSizer, 0, wxALL|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
    
    this->finjinViewerButton->SetId(FINJIN_VIEWER_RADIO_BUTTON);
    
    SetSizer(topSizer);
    Layout();
}

GlobalSettingsDialog_GeneralPage::~GlobalSettingsDialog_GeneralPage()
{
}

bool GlobalSettingsDialog_GeneralPage::GetGUIData()
{
    auto& settings = FinjinGlobalSettings::GetInstance();
    
    //Text blob format
    int textBlobFormatSelection = this->textBlobFormatChoice->GetSelection();
    if (textBlobFormatSelection >= 0)
        settings.blobTextFormat = (WxDataChunkBlobTextFormat)textBlobFormatSelection;

    //Max bytes per line
    settings.maxBytesPerLine = maxBytesPerLineSpinner->GetIntValue();

    //"Additional export" files
    settings.additionalExportJsonFiles = this->alwaysExportJsonCheckbox->GetValue();
    settings.additionalExportTextFiles = this->alwaysExportTextCheckbox->GetValue();
    settings.additionalExportBinaryFiles = this->alwaysExportBinaryCheckbox->GetValue();
    settings.additionalExportConfigFiles = this->alwaysExportConfigCheckbox->GetValue();

    //Export
    settings.exportApplicationName = this->exportApplicationNameCheckbox->GetValue();    
    settings.exportAuthor = this->exportAuthorCheckbox->GetValue();    
    settings.exportExportTime = this->exportTimestampCheckbox->GetValue();
    settings.exportExporterVersion = this->exportExporterVersionCheckbox->GetValue();

    //"Embed" files
    settings.embedMeshes = this->embedMeshesCheckbox->GetValue();
    settings.embedMaterials = this->embedMaterialsCheckbox->GetValue();
    settings.embedTextures = this->embedTexturesCheckbox->GetValue();
    settings.embedPrefabs = this->embedPrefabsCheckbox->GetValue();
    
    //Export directory
    settings.exportDirectory = this->exportDirectoryText->GetValue();

    //External viewer
    //if (this->finjinViewerButton->GetValue())
        settings.externalViewer = FinjinGlobalSettings::FINJIN_VIEWER;
    
    //Run external viewer after export
    settings.runExternalViewerAfterExport = this->runExternalViewerAfterExportCheckbox->GetValue();

    return true;
}

bool GlobalSettingsDialog_GeneralPage::SetGUIData()
{
    auto& settings = FinjinGlobalSettings::GetInstance();

    //Text blob format
    this->textBlobFormatChoice->Freeze();
    this->textBlobFormatChoice->Append(wxT("Byte Array"));
    this->textBlobFormatChoice->Append(wxT("Base64"));
    this->textBlobFormatChoice->SetSelection((int)settings.blobTextFormat);
    this->textBlobFormatChoice->Thaw();

    //Max bytes per line
    maxBytesPerLineSpinner->SetValue(settings.maxBytesPerLine);

    //"Additional export" files
    this->alwaysExportJsonCheckbox->SetValue(settings.additionalExportJsonFiles);
    this->alwaysExportTextCheckbox->SetValue(settings.additionalExportTextFiles);
    this->alwaysExportBinaryCheckbox->SetValue(settings.additionalExportBinaryFiles);
    this->alwaysExportConfigCheckbox->SetValue(settings.additionalExportConfigFiles);

    //Export
    this->exportApplicationNameCheckbox->SetValue(settings.exportApplicationName);    
    this->exportAuthorCheckbox->SetValue(settings.exportAuthor);    
    this->exportTimestampCheckbox->SetValue(settings.exportExportTime);
    this->exportExporterVersionCheckbox->SetValue(settings.exportExporterVersion);

    //"Embed" files
    this->embedMeshesCheckbox->SetValue(settings.embedMeshes);
    this->embedMaterialsCheckbox->SetValue(settings.embedMaterials);
    this->embedTexturesCheckbox->SetValue(settings.embedTextures);
    this->embedPrefabsCheckbox->SetValue(settings.embedPrefabs);

    //Export directory
    this->exportDirectoryText->SetValue(settings.exportDirectory);
    
    //External viewer
    this->finjinViewerButton->SetValue(settings.externalViewer == FinjinGlobalSettings::FINJIN_VIEWER);
    
    //Run external viewer after export
    this->runExternalViewerAfterExportCheckbox->SetValue(settings.runExternalViewerAfterExport);

    return true;
}

void GlobalSettingsDialog_GeneralPage::OnFinjinViewerRadioButton(wxCommandEvent& event)
{
    //this->finjinViewerButton->SetValue(false);
}
