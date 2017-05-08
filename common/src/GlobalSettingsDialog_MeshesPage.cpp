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
#include "GlobalSettingsDialog_MeshesPage.hpp"
#include "FinjinGlobalSettings.hpp"
#include "Strings.hpp"
#include "StringUtilities.hpp"
#include "StringSplitter.hpp"
#include "FileUtilities.hpp"
#include "PropertyValues.hpp"
#include "ApplicationAccessor.hpp"
#include "RelativeLocationPrompt.hpp"
#include "Dialogs.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "FinjinResourceManager.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
const wxString GlobalSettingsDialog_MeshesPage::TITLE(wxT("Meshes"));


//Local data--------------------------------------------------------------------
static const wxChar* SUBMESH_NAMINGS_TEXT[] = {wxT("\"submesh\""), wxT("Material Name"), wxT("Custom")};
static const wxChar* SUBMESH_NAMINGS_VALUE[] = {wxT(""), wxT("material"), wxT("custom")};
static const int SUBMESH_NAMINGS_COUNT = FINJIN_COUNT_OF(SUBMESH_NAMINGS_VALUE);
static const int SUBMESH_NAMINGS_CUSTOM_VALUE_INDEX = 2;


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(GlobalSettingsDialog_MeshesPage, SettingsPage)
    EVT_CHOICE(GlobalSettingsDialog_MeshesPage::SUBMESH_NAMING_CHOICE, GlobalSettingsDialog_MeshesPage::OnSubmeshNamingChoice)
    EVT_CHOICE(GlobalSettingsDialog_MeshesPage::BASE_DIRECTORY_CHOICE, GlobalSettingsDialog_MeshesPage::OnVertexFormatBaseDirectoryChoice)
    EVT_BUTTON(GlobalSettingsDialog_MeshesPage::BASE_DIRECTORY_BUTTON, GlobalSettingsDialog_MeshesPage::OnVertexFormatBaseDirectoryButton)
    EVT_BUTTON(GlobalSettingsDialog_MeshesPage::VERTEX_FORMAT_FILE_BUTTON, GlobalSettingsDialog_MeshesPage::OnVertexFormatFileButton)
END_EVENT_TABLE()

GlobalSettingsDialog_MeshesPage::GlobalSettingsDialog_MeshesPage(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* meshNamingSizer;
    meshNamingSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Meshes") ), wxVERTICAL );

    wxFlexGridSizer* meshNamingGridSizer;
    meshNamingGridSizer = new wxFlexGridSizer( 1, 2, 0, 0 );
    meshNamingGridSizer->SetFlexibleDirection( wxBOTH );
    meshNamingGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    submeshNamingLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Submesh Naming"), wxDefaultPosition, wxDefaultSize, 0 );
    submeshNamingLabel->Wrap( -1 );
    meshNamingGridSizer->Add( submeshNamingLabel, 0, wxALL, 5 );

    wxBoxSizer* submeshNamingSizer;
    submeshNamingSizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString submeshNamingChoiceChoices;
    submeshNamingChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), submeshNamingChoiceChoices, 0 );
    submeshNamingChoice->SetSelection( 0 );
    submeshNamingSizer->Add( submeshNamingChoice, 0, wxLEFT|wxRIGHT, 5 );

    submeshNamingCustomText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    submeshNamingCustomText->SetMaxLength( 0 );
    submeshNamingSizer->Add( submeshNamingCustomText, 0, 0, 5 );


    meshNamingGridSizer->Add( submeshNamingSizer, 1, wxEXPAND, 5 );


    meshNamingSizer->Add( meshNamingGridSizer, 0, wxEXPAND, 5 );

    topSizer->Add( meshNamingSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

    wxStaticBoxSizer* vertexFileLocationsTopSizer;
    vertexFileLocationsTopSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Vertex Format File") ), wxVERTICAL );

    baseDirectoryLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Base Directory"), wxDefaultPosition, wxDefaultSize, 0 );
    baseDirectoryLabel->Wrap( -1 );
    vertexFileLocationsTopSizer->Add( baseDirectoryLabel, 0, wxALL, 5 );

    wxBoxSizer* baseDirectorySizer;
    baseDirectorySizer = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString baseDirectoryChoiceChoices;
    baseDirectoryChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, baseDirectoryChoiceChoices, 0 );
    baseDirectoryChoice->SetSelection( 0 );
    baseDirectorySizer->Add( baseDirectoryChoice, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    baseDirectoryText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 285,-1 ), wxTE_READONLY );
    baseDirectoryText->SetMaxLength( 0 );
    baseDirectorySizer->Add( baseDirectoryText, 1, wxBOTTOM, 5 );

    baseDirectoryButton = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxSize( 100,-1 ), 0 );
    baseDirectorySizer->Add( baseDirectoryButton, 0, wxBOTTOM|wxLEFT, 5 );


    vertexFileLocationsTopSizer->Add( baseDirectorySizer, 0, wxEXPAND, 5 );

    wxBoxSizer* vertexFormatFileSizer;
    vertexFormatFileSizer = new wxBoxSizer( wxHORIZONTAL );

    vertexFormatFileLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("File"), wxDefaultPosition, wxDefaultSize, 0 );
    vertexFormatFileLabel->Wrap( -1 );
    vertexFormatFileSizer->Add( vertexFormatFileLabel, 0, wxALL, 5 );

    vertexFormatFileText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 285,-1 ), 0 );
    vertexFormatFileText->SetMaxLength( 0 );
    vertexFormatFileSizer->Add( vertexFormatFileText, 1, wxBOTTOM|wxLEFT, 5 );

    vertexFormatFileButton = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxSize( 100,-1 ), 0 );
    vertexFormatFileSizer->Add( vertexFormatFileButton, 0, wxBOTTOM|wxLEFT, 5 );


    vertexFileLocationsTopSizer->Add( vertexFormatFileSizer, 0, wxEXPAND, 5 );


    topSizer->Add( vertexFileLocationsTopSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    this->submeshNamingChoice->SetId(SUBMESH_NAMING_CHOICE);
    this->baseDirectoryChoice->SetId(BASE_DIRECTORY_CHOICE);
    this->baseDirectoryButton->SetId(BASE_DIRECTORY_BUTTON);
    this->vertexFormatFileButton->SetId(VERTEX_FORMAT_FILE_BUTTON);

    SetSizer(topSizer);
    Layout();
}

GlobalSettingsDialog_MeshesPage::~GlobalSettingsDialog_MeshesPage()
{
}

bool GlobalSettingsDialog_MeshesPage::GetGUIData()
{
    auto& settings = FinjinGlobalSettings::GetInstance();

    //Submesh naming
    settings.submeshNaming = (wxChar*)this->submeshNamingChoice->GetClientData(this->submeshNamingChoice->GetSelection());

    //Submesh naming custom name
    settings.submeshCustomName = this->submeshNamingCustomText->GetValue();

    //Base directory choice/edit
    BaseDirectory baseDirectory(this->baseDirectoryChoice, this->baseDirectoryText);
    baseDirectory.GetGUIData(settings.baseVertexFormatDirectoryType, settings.baseVertexFormatDirectory);

    //Vertex format
    settings.vertexFormatPath = this->vertexFormatFileText->GetValue();

    //If the locations are valid, then hold onto them
    if (FinjinResourceManager::GetInstance().ValidateVertexFormatLocation())
    {
        //Notify that the locations have changed
        FinjinResourceManager::GetInstance().UpdateVertexFormats();
        return true;
    }
    else
        return false;
}

bool GlobalSettingsDialog_MeshesPage::SetGUIData()
{
    auto& settings = FinjinGlobalSettings::GetInstance();

    //Submesh naming
    this->submeshNamingChoice->Freeze();
    for (int i = 0; i < SUBMESH_NAMINGS_COUNT; i++)
    {
        this->submeshNamingChoice->Append(SUBMESH_NAMINGS_TEXT[i]);
        this->submeshNamingChoice->SetClientData(i, (wxChar*)SUBMESH_NAMINGS_VALUE[i]);
        if (settings.submeshNaming == SUBMESH_NAMINGS_VALUE[i])
            this->submeshNamingChoice->SetSelection(i);
    }
    this->submeshNamingChoice->Thaw();
    if (this->submeshNamingChoice->GetSelection() < 0)
        this->submeshNamingChoice->SetSelection(0);

    //Submesh naming custom name
    this->submeshNamingCustomText->SetValue(settings.submeshCustomName);

    //Vertex format file base directory
    BaseDirectory baseDirectory(settings.baseVertexFormatDirectoryType, settings.baseVertexFormatDirectory);
    baseDirectory.SetGUIData(this->baseDirectoryChoice, this->baseDirectoryText);

    //Vertex format
    this->vertexFormatFileText->SetValue(settings.vertexFormatPath);

    this->currentVertexFormatCustomDirectory = settings.baseVertexFormatDirectory;

    UpdateSubmeshNamingControls(false);
    UpdateVertexFormatBaseDirectoryControls(false);
    Layout();

    return true;
}

void GlobalSettingsDialog_MeshesPage::UpdateSubmeshNamingControls(bool layout)
{
    auto show = this->submeshNamingChoice->GetSelection() == SUBMESH_NAMINGS_CUSTOM_VALUE_INDEX;
    this->submeshNamingCustomText->Show(show);
    if (layout)
        Layout();
}

void GlobalSettingsDialog_MeshesPage::UpdateVertexFormatBaseDirectoryControls(bool layout)
{
    int baseDirectoryChoice = BaseDirectory::GetBaseDirectoryTypeChoice(this->baseDirectoryChoice);

    //Base directory text
    if (baseDirectoryChoice == BaseDirectory::CUSTOM_DIRECTORY_CHOICE)
    {
        this->baseDirectoryText->SetValue(this->currentVertexFormatCustomDirectory);
    }
    else
    {
        BaseDirectory baseDirectory(this->baseDirectoryChoice, this->baseDirectoryText);
        this->baseDirectoryText->SetValue(baseDirectory.GetBaseDirectory());
    }

    //Base directory button
    this->baseDirectoryButton->Show(baseDirectoryChoice == BaseDirectory::CUSTOM_DIRECTORY_CHOICE);
    if (layout)
        Layout();
}

void GlobalSettingsDialog_MeshesPage::OnSubmeshNamingChoice(wxCommandEvent& event)
{
    UpdateSubmeshNamingControls();
}

void GlobalSettingsDialog_MeshesPage::OnVertexFormatBaseDirectoryChoice(wxCommandEvent& event)
{
    UpdateVertexFormatBaseDirectoryControls();
}

void GlobalSettingsDialog_MeshesPage::OnVertexFormatBaseDirectoryButton(wxCommandEvent& event)
{
    SUSPEND_CUSTOM_CONTROLS;
    wxDirDialog dialog(this, Strings::CHOOSE_DIRECTORY, wxEmptyString);
    if (dialog.ShowModal() == wxID_OK)
    {
        int baseDirectoryChoice = BaseDirectory::GetBaseDirectoryTypeChoice(this->baseDirectoryChoice);
        if (baseDirectoryChoice == BaseDirectory::CUSTOM_DIRECTORY_CHOICE)
        {
            this->currentVertexFormatCustomDirectory = dialog.GetPath();
            FileUtilities::EnsureTrailingPathSeparator(this->currentVertexFormatCustomDirectory);
            this->baseDirectoryText->SetValue(this->currentVertexFormatCustomDirectory);
        }
        else if (baseDirectoryChoice == BaseDirectory::PROJECT_DIRECTORY_CHOICE)
        {
            auto directory = dialog.GetPath();
            FileUtilities::EnsureTrailingPathSeparator(directory);
            this->baseDirectoryText->SetValue(directory);
        }
    }
}

void GlobalSettingsDialog_MeshesPage::OnVertexFormatFileButton(wxCommandEvent& event)
{
    BaseDirectory baseDirectory(this->baseDirectoryChoice, this->baseDirectoryText);
    RelativeLocationPrompt prompt(this, baseDirectory.GetBaseDirectory(), Strings::CHOOSE_FILE, Strings::CFG_FILE_FILTER);
    auto path = prompt.ShowPrompt();
    if (!path.empty())
    {
        //Set into text box
        this->vertexFormatFileText->SetValue(path);
    }
}
