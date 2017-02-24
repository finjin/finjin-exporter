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
#include "UserDataSettingsAccessor.hpp"
#include "UserDataTypes.hpp"
#include "FinjinSceneSettingsAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class UserDataSettingsPage;

    /**
     * A modal dialog that allows user data settings to be modified for an object.
     * In most cases, the user data settings will be displayed as one tab among many
     * in a dialog. However, some object types such as the Finjin material, don't have 
     * a native settings dialog.
     */
    class UserDataSettingsDialog : public FinjinDialog
    {    
    public:
        /** Default constructor. */
        UserDataSettingsDialog();

        /** 
         * Constructor. 
         * @param parent [in] - The parent window.
         * @param objectName [in] - The name of the object that owns the user data. This is used for
         * formatting the string shown in the dialog's title bar.
         * @param userDataSettings [in] - User data settings accessor.
         * @param classUsage [in] - Indicates what type of classes to allow.
         * @param sceneSettings [in] - The scene settings.
         */
        UserDataSettingsDialog
            (
            wxWindow* parent, 
            const wxString& objectName,
            UserDataSettingsAccessor& userDataSettings, 
            UserDataUsage classUsage,
            FinjinSceneSettingsAccessor sceneSettings
            );

        bool Create
            (
            wxWindow* parent, 
            const wxString& objectName,
            UserDataSettingsAccessor& userDataSettings, 
            UserDataUsage classUsage,
            FinjinSceneSettingsAccessor sceneSettings
            );

        void OnCloseWindow(wxCloseEvent& event);    
        void OnOK(wxCommandEvent& event);
        
    protected:
        void CreateControls();

        int GetGUIData();
        void SetGUIData();

    private:
        DECLARE_EVENT_TABLE()
        DECLARE_NO_COPY_CLASS(UserDataSettingsDialog)

        UserDataSettingsAccessor* userDataSettings;
        UserDataUsage classUsage;
        FinjinSceneSettingsAccessor sceneSettings;

        UserDataSettingsPage* userDataPage;
    };

} }
