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


#pragma once


//Includes---------------------------------------------------------------------
#include "FinjinDialog.hpp"
#include "FinjinGlobalSettings.hpp"
#include "TabControl.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** 
     * A modal dialog used to edit the global settings.
     * This dialog can be seen within the application by selecting 'Global Settings' from the Finjin menu.
     */
    class GlobalSettingsDialog : public FinjinDialog 
    {
    protected:        

    public:
        GlobalSettingsDialog(wxWindow* parent, int initialPageIndex, const wxString& initialPageTitle);
        
        bool Create(wxWindow* parent, int initialPageIndex, const wxString& initialPageTitle);

        void CreateControls(int initialPageIndex, const wxString& initialPageTitle);

        void ChangeToPage(int index, const wxString& title);

        int GetGUIData();
        void SetGUIData();
        
        void OnOK(wxCommandEvent& event);
        void OnCancel(wxCommandEvent& event);
        void OnCloseWindow(wxCloseEvent& event);
        void OnNotebookPageChanged(wxNotebookEvent& event);

    private:
        DECLARE_EVENT_TABLE()
        DECLARE_NO_COPY_CLASS(GlobalSettingsDialog)

        TabControl* tabControl;
        TabControlPages pages;

        static int lastPageSelection;
        bool initializingPages;

        FinjinGlobalSettings originalSettings;
        
    };

} }
