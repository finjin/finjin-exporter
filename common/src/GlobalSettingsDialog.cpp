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
#include "GlobalSettingsDialog.hpp"
#include "WindowPlacementManager.hpp"
#include "ApplicationControls.hpp"
#include "FinjinGlobalSettings.hpp"
#include "Strings.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "ApplicationAccessor.hpp"
#include "RelativeLocationPrompt.hpp"
#include "Dialogs.hpp"
#include "GlobalSettingsDialog_GeneralPage.hpp"
#include "GlobalSettingsDialog_ScenePage.hpp"
#include "GlobalSettingsDialog_AnimationsPage.hpp"
#include "GlobalSettingsDialog_MeshesPage.hpp"
#include "GlobalSettingsDialog_MaterialsPage.hpp"
#include "GlobalSettingsDialog_UserDataClassPage.hpp"
#include "GlobalSettingsDialog_FlagsPage.hpp"

#if defined(__WXMAC__)
    #define DIALOG_WIDTH 860
    #define DIALOG_HEIGHT 710
#else
    #define DIALOG_WIDTH 615
    #define DIALOG_HEIGHT 610
#endif

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
int GlobalSettingsDialog::lastPageSelection = 0;


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(GlobalSettingsDialog, FinjinDialog)
    EVT_BUTTON(wxID_OK, GlobalSettingsDialog::OnOK)    
    EVT_BUTTON(wxID_CANCEL, GlobalSettingsDialog::OnCancel)
    EVT_CLOSE(GlobalSettingsDialog::OnCloseWindow)    
    EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, GlobalSettingsDialog::OnNotebookPageChanged)
END_EVENT_TABLE()

GlobalSettingsDialog::GlobalSettingsDialog(wxWindow* parent, int initialPageIndex, const wxString& initialPageTitle)
{
    Create(parent, initialPageIndex, initialPageTitle);
}

bool GlobalSettingsDialog::Create(wxWindow* parent, int initialPageIndex, const wxString& initialPageTitle)
{
    if (!FinjinDialog::Create(parent, wxID_ANY, wxT("Finjin Global Settings"), wxDefaultPosition, wxSize(DIALOG_WIDTH, DIALOG_HEIGHT), wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU, wxT("FinjinGlobalSettingsDialog")))
        return false;

    CreateControls(initialPageIndex, initialPageTitle);

    return true;
}

void GlobalSettingsDialog::CreateControls(int initialPageIndex, const wxString& initialPageTitle)
{
    wxButton* closeButton;

    SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );
    
    wxBoxSizer* tabPagesSizer = nullptr;
    int tabControlProportion = 1;        
    
    this->initializingPages = true;
    tabControl = new TabControl( this, wxID_ANY, wxDefaultPosition, DEFAULT_TAB_CONTROL_SIZE, 0);
    ApplicationControls::AdjustContainerBackgroundColor(tabControl);

    //Add pages to the control
    this->pages.SetTabControl(this->tabControl, tabPagesSizer);
    this->pages.AddPage(new GlobalSettingsDialog_GeneralPage(this->pages.GetTabParentWindow()), GlobalSettingsDialog_GeneralPage::TITLE);
    this->pages.AddPage(new GlobalSettingsDialog_ScenePage(this->pages.GetTabParentWindow()), GlobalSettingsDialog_ScenePage::TITLE);
    this->pages.AddPage(new GlobalSettingsDialog_UserDataClassPage(this->pages.GetTabParentWindow()), GlobalSettingsDialog_UserDataClassPage::TITLE);
    this->pages.AddPage(new GlobalSettingsDialog_AnimationsPage(this->pages.GetTabParentWindow()), GlobalSettingsDialog_AnimationsPage::TITLE);
    this->pages.AddPage(new GlobalSettingsDialog_MeshesPage(this->pages.GetTabParentWindow()), GlobalSettingsDialog_MeshesPage::TITLE);
    this->pages.AddPage(new GlobalSettingsDialog_MaterialsPage(this->pages.GetTabParentWindow()), GlobalSettingsDialog_MaterialsPage::TITLE);    
    this->pages.AddPage(new GlobalSettingsDialog_FlagsPage(this->pages.GetTabParentWindow()), GlobalSettingsDialog_FlagsPage::TITLE);
    this->initializingPages = false;

    //Select the initial page
    ChangeToPage(initialPageIndex, initialPageTitle);
        
    //Finish adding tab control(s) to the dialog
    topSizer->Add( tabControl, tabControlProportion, wxEXPAND | wxALL | TAB_CONTROL_SIZER_FLAGS, 5 );
    if (tabPagesSizer != nullptr)
        topSizer->Add( tabPagesSizer, 1, wxEXPAND | wxDOWN | wxRIGHT | wxLEFT, 5);
    
    wxGridSizer* bottomSizer;
    bottomSizer = new wxGridSizer( 1, 1, 0, 0 );
    
    closeButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    bottomSizer->Add( closeButton, 0, wxALIGN_RIGHT|wxDOWN | wxRIGHT | wxLEFT, 5 );

    /*auto cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    bottomSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );*/    
    
    topSizer->Add( bottomSizer, 0, wxEXPAND, 5 );
    
    this->originalSettings = FinjinGlobalSettings::GetInstance();

    SetGUIData();

    SetSizer(topSizer);

    if (!WindowPlacementManager::RestorePlacement(this))
    {
        Layout();
        Centre(wxBOTH);
    }

    SetMinSize(GetSize());
    SetMaxSize(wxGetDisplaySize());
}

void GlobalSettingsDialog::ChangeToPage(int index, const wxString& title)
{
    if (this->pages.GetPageCount() > 0)
    {
        if (index < 0 && !title.empty())
        {
            for (size_t i = 0; i < this->pages.GetPageCount(); i++)
            {
                if (this->pages.GetPageText(i) == title)
                {
                    index = (int)i;
                    break;
                }
            }
        }
        if (index < 0)
            index = lastPageSelection;
        index = std::max(0, index);
    
        //Set initial page
        this->pages.ChangeSelection(index);
    }
}

int GlobalSettingsDialog::GetGUIData()
{
    for (size_t pageIndex = 0; pageIndex < this->pages.GetPageCount(); pageIndex++)
    {
        auto page = static_cast<SettingsPage*>(this->pages.GetPage(pageIndex));
        if (!page->GetGUIData())
            return (int)pageIndex;
    }

    auto& settings = FinjinGlobalSettings::GetInstance();
    settings.Save();

    return -1;
}

void GlobalSettingsDialog::SetGUIData()
{
    for (size_t pageIndex = 0; pageIndex < this->pages.GetPageCount(); pageIndex++)
    {
        auto page = static_cast<SettingsPage*>(this->pages.GetPage(pageIndex));
        page->SetGUIData();
    }
}

void GlobalSettingsDialog::OnOK(wxCommandEvent& event)
{
    WindowPlacementManager::SavePlacement(this);
    int badPage = GetGUIData();
    if (badPage == -1)
    {
        FinjinGlobalSettings::GetInstance().Save();        
        
        MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
    }
}

void GlobalSettingsDialog::OnCancel(wxCommandEvent& event)
{
    WindowPlacementManager::SavePlacement(this);

    FinjinGlobalSettings::GetInstance() = this->originalSettings;

    MODAL_DIALOG_ON_CLOSE(event, wxID_CANCEL);
}

void GlobalSettingsDialog::OnCloseWindow(wxCloseEvent& event)
{
    WindowPlacementManager::SavePlacement(this);
    
    FinjinGlobalSettings::GetInstance() = this->originalSettings;

    MODAL_DIALOG_ON_CLOSE(event, wxID_CANCEL);
}

void GlobalSettingsDialog::OnNotebookPageChanged(wxNotebookEvent& event)
{
    this->pages.OnPageChanged();
    Layout();
    if (!this->initializingPages)
        lastPageSelection = this->pages.GetSelection();
}
