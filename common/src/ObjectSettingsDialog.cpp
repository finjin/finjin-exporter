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
#include "ObjectSettingsDialog.hpp"
#include "WindowPlacementManager.hpp"
#include "ApplicationControls.hpp"
#include "DetectableObjectType.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "Strings.hpp"
#include "ObjectSettingsDialog_GeneralPage.hpp"
#include "ApplicationAccessor.hpp"

#if defined(__WXMAC__)
    #define DIALOG_WIDTH 910
    #define DIALOG_HEIGHT 760
#else
    #define DIALOG_WIDTH 610
    #define DIALOG_HEIGHT 660
#endif

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
wxString ObjectSettingsDialog::lastPageSelection;


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(ObjectSettingsDialog, FinjinDialog)
    EVT_CLOSE(ObjectSettingsDialog::OnCloseWindow)        
    EVT_BUTTON(wxID_OK, ObjectSettingsDialog::OnOK)
    EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, ObjectSettingsDialog::OnNotebookPageChanged)
END_EVENT_TABLE()

ObjectSettingsDialog::ObjectSettingsDialog()
{       
}

ObjectSettingsDialog::ObjectSettingsDialog(wxWindow* parent, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, FinjinSceneSettingsAccessor sceneSettings)
{
    Create(parent, object, objectSettings, sceneSettings);
}

bool ObjectSettingsDialog::Create(wxWindow* parent, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, FinjinSceneSettingsAccessor sceneSettings)
{
    this->object = object;
    this->objectSettings = objectSettings;
    this->sceneSettings = sceneSettings;

    this->defaultObjectType.Detect(this->object, 0, true, this->objectSettings);        
    this->objectType.Detect(this->object, this->objectSettings);    
    
    auto title = wxString::Format(Strings::OBJECT_SETTINGS_DIALOG_TITLE_FORMAT, object.GetLocalName().wx_str());
    
    if (!FinjinDialog::Create(parent, wxID_ANY, title, wxDefaultPosition, wxSize(DIALOG_WIDTH, DIALOG_HEIGHT), wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU, wxT("FinjinObjectSettingsDialog")))
        return false;

    CreateControls();

    return true;
}

void ObjectSettingsDialog::CreateControls()
{
    wxButton* closeButton;

    SetSizeHints(wxDefaultSize, wxDefaultSize);
    
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* tabPagesSizer = nullptr;    
    int tabControlProportion = 1;        

    this->initializingPages = true;
    tabControl = new TabControl( this, wxID_ANY, wxDefaultPosition, DEFAULT_TAB_CONTROL_SIZE, 0);
    ApplicationControls::AdjustContainerBackgroundColor(tabControl);
    this->pages.SetTabControl(this->tabControl, tabPagesSizer);
    this->pages.AddPage(new ObjectSettingsDialog_GeneralPage(this, this->pages.GetTabParentWindow(), this->object, this->objectSettings), ObjectSettingsDialog_GeneralPage::TITLE);
    this->basicSettingsPageCount = this->pages.GetPageCount();
    this->initializingPages = false;

    topSizer->Add( tabControl, tabControlProportion, wxEXPAND | wxALL | TAB_CONTROL_SIZER_FLAGS, 5 );

    if (tabPagesSizer != nullptr)
        topSizer->Add( tabPagesSizer, 1, wxEXPAND | wxDOWN | wxRIGHT | wxLEFT, 5);
    
    wxGridSizer* bottomSizer;
    bottomSizer = new wxGridSizer( 1, 1, 0, 0 );
    
    closeButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    bottomSizer->Add( closeButton, 0, wxALIGN_RIGHT|wxALL, 5 );
    
    topSizer->Add( bottomSizer, 0, wxEXPAND, 5 );

    SetGUIData();
    
    ChangeToDefaultPage();
    
    SetSizer(topSizer);
    
    if (!WindowPlacementManager::RestorePlacement(this))
    {
        Layout();
        Centre(wxBOTH);
    }

    SetMinSize(GetSize());
    SetMaxSize(wxGetDisplaySize());
}

void ObjectSettingsDialog::ChangeToDefaultPage()
{
    int index = 0;
    if (!lastPageSelection.empty())
    {
        for (size_t i = 0; i < this->pages.GetPageCount(); i++)
        {
            if (lastPageSelection == this->pages.GetPageText(i))
            {
                index = (int)i;
                break;
            }
        }
    }
        
    //Set initial page
    this->pages.ChangeSelection(index);
}

void ObjectSettingsDialog::OnCloseWindow(wxCloseEvent& event)
{
    WindowPlacementManager::SavePlacement(this);
    EndModal(wxID_OK);
}

void ObjectSettingsDialog::OnOK(wxCommandEvent& event)
{
    WindowPlacementManager::SavePlacement(this);
    int badPage = GetGUIData();
    if (badPage == -1)
    {
        MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
    }
}

void ObjectSettingsDialog::OnNotebookPageChanged(wxNotebookEvent& event)
{
    this->pages.OnPageChanged();
    Layout();
    if (!this->initializingPages)
        lastPageSelection = this->pages.GetPageText(this->pages.GetSelection());    
}

void ObjectSettingsDialog::SelectedObjectType(DetectableObjectType* type)
{
    //Get data for current pages and delete the pages
    while (this->pages.GetPageCount() > this->basicSettingsPageCount)
    {
        auto page = static_cast<SettingsPage*>(this->pages.GetPage(this->basicSettingsPageCount));
        page->GetGUIData();
        this->pages.DeletePage((int)this->basicSettingsPageCount);
    }
    
    //Create the new pages
    SUSPEND_CUSTOM_CONTROLS;
    type->CreateSettingsPages(this->pages, this->object, this->objectSettings, this->sceneSettings);
    
    //Set data into new pages
    for (size_t pageIndex = this->basicSettingsPageCount; pageIndex < this->pages.GetPageCount(); pageIndex++)
    {
        auto page = static_cast<SettingsPage*>(this->pages.GetPage(pageIndex));
        page->SetGUIData();
    }
 
    Refresh();
}

int ObjectSettingsDialog::GetGUIData()
{
    for (size_t pageIndex = 0; pageIndex < this->pages.GetPageCount(); pageIndex++)
    {
        auto page = static_cast<SettingsPage*>(this->pages.GetPage(pageIndex));
        if (!page->GetGUIData())
            return (int)pageIndex;
    }

    return -1;
}

void ObjectSettingsDialog::SetGUIData()
{
    //Set data on all pages
    for (size_t pageIndex = 0; pageIndex < this->pages.GetPageCount(); pageIndex++)
    {
        auto page = static_cast<SettingsPage*>(this->pages.GetPage(pageIndex));
        page->SetGUIData();
    }

    //Add pages for current type
    auto generalPage = static_cast<ObjectSettingsDialog_GeneralPage*>(this->pages.GetPage(0));
    SelectedObjectType(generalPage->GetSelectedObjectType());
}
