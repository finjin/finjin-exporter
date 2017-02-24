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
#include "FinjinObjectSettingsAccessor.hpp"
#include "ObjectTypeDetector.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "TabControl.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class DetectableObjectType;

    /** 
     * The object settings dialog.
     * This dialog can be seen within the application by selecting 
     * 'Object Settings' from the Finjin menu.
     */
    class ObjectSettingsDialog : public FinjinDialog
    {    
    public:
        /** Default constructor. */
        ObjectSettingsDialog();

        /** 
         * Constructor.
         * @param parent [in] - Parent window.
         * @param object [in] - The object whose settings are being edited.
         * @param objectSettings [in] - The settings for the specified object.
         * @param sceneSettings [in] - The scene settings.
         */
        ObjectSettingsDialog
            (
            wxWindow* parent, 
            ObjectAccessor object, 
            FinjinObjectSettingsAccessor objectSettings, 
            FinjinSceneSettingsAccessor sceneSettings
            );

        bool Create
            (
            wxWindow* parent, 
            ObjectAccessor object, 
            FinjinObjectSettingsAccessor objectSettings, 
            FinjinSceneSettingsAccessor sceneSettings
            );

        void OnCloseWindow(wxCloseEvent& event);    
        void OnOK(wxCommandEvent& event);
        void OnNotebookPageChanged(wxNotebookEvent& event);
        
        void SelectedObjectType(DetectableObjectType* type);    
        
    protected:
        void CreateControls();

        void ChangeToDefaultPage();

        int GetGUIData();
        void SetGUIData();

    private:
        DECLARE_EVENT_TABLE()
        DECLARE_NO_COPY_CLASS(ObjectSettingsDialog)

        TabControl* tabControl;
        TabControlPages pages;
        size_t basicSettingsPageCount;

        ObjectAccessor object;
        FinjinObjectSettingsAccessor objectSettings;

        static wxString lastPageSelection;
        bool initializingPages;
        
    public:
        FinjinSceneSettingsAccessor sceneSettings;
        ObjectTypeDetector defaultObjectType;
        ObjectTypeDetector objectType;        
    };

} }
