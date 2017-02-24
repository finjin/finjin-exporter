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
#include "FinjinSceneSettingsAccessor.hpp"
#include "TabControl.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** 
     * The object settings dialog.
     * This dialog can be seen within the application by selecting 
     * 'Scene Settings' from the Finjin menu.
     */
    class SceneSettingsDialog : public FinjinDialog
    {    
    public:
        /** Default constructor. */
        SceneSettingsDialog();

        /** 
         * Constructor.
         * @param parent [in] - Parent window.
         * @param sceneSettings [in] - The scene settings.
         * @param initialPageIndex [in] - Index of the initial page to show. -1 indicates that 
         * initialPageTitle should be used. If that's empty, then the last selected page is used.
         * @param initialPageTitle [in] - The title of the initial page to show. This is used if
         * initialPageIndex is -1. If this is empty, then the last selected page is used.
         */
        SceneSettingsDialog
            (
            wxWindow* parent, 
            FinjinSceneSettingsAccessor sceneSettings, 
            int initialPageIndex = -1, 
            const wxString& initialPageTitle = wxEmptyString
            );

        bool Create
            (
            wxWindow* parent, 
            FinjinSceneSettingsAccessor sceneSettings, 
            int initialPageIndex = -1, 
            const wxString& initialPageTitle = wxEmptyString
            );

        void OnCloseWindow(wxCloseEvent& event);    
        void OnOK(wxCommandEvent& event);
        void OnNotebookPageChanged(wxNotebookEvent& event);

    protected:
        void CreateControls(int initialPageIndex = 0, const wxString& initialPageTitle = wxEmptyString);

        void ChangeToPage(int index, const wxString& title);

        int GetGUIData();
        void SetGUIData();

    private:
        DECLARE_EVENT_TABLE()
        DECLARE_NO_COPY_CLASS(SceneSettingsDialog)

        TabControl* tabControl;
        TabControlPages pages;

        static int lastPageSelection;
        bool initializingPages;

        FinjinSceneSettingsAccessor sceneSettings;
    };

} }
