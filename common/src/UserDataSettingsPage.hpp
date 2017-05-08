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


//Includes----------------------------------------------------------------------
#include "SettingsPage.hpp"
#include "UserDataSettingsAccessor.hpp"
#include "UserDataTypes.hpp"
#include "UserDataClassControls.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "FinjinResourceManager.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * A settings page that manages the display of controls used to edit user data.
     */
    class UserDataSettingsPage :
        public SettingsPage,
        public FinjinResourceManager::UserDataTypesChangedListener
    {
    private:
        DECLARE_EVENT_TABLE()

    protected:
        wxStaticText* idLabel;
        ApplicationTextCtrl* idText;
        wxButton* generateIDButton;

        wxStaticText* userDataClassLabel;
        ApplicationChoiceCtrl* userDataClassChoice;
        wxButton* configureUserDataClassButton;
        ApplicationTextCtrl* userDataText;
        wxScrolledWindow* userDataControlsWindow;

        UserDataSettingsAccessor& settings;
        UserDataUsage classUsage;
        UserDataClassControls classControls;
        wxString lastSelectedClassName;
        std::vector<wxString> comboBoxClassNames;
        FinjinSceneSettingsAccessor sceneSettings;

    public:
        /**
         * Constructor.
         * @param parent [in] - The parent window.
         * @param userDataSettings [in] - User data settings accessor.
         * @param classUsage [in] - Indicates what type of classes to allow.
         * @param sceneSettings [in] - The scene settings.
         */
        UserDataSettingsPage
            (
            wxWindow* parent,
            UserDataSettingsAccessor& userDataSettings,
            UserDataUsage classUsage,
            FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor()
            );

        ~UserDataSettingsPage();

        bool GetGUIData() override;
        bool SetGUIData() override;

        void OnClassChoice(wxCommandEvent& event);
        void OnGenerateIDButton(wxCommandEvent& event);
        void OnConfigureButton(wxCommandEvent& event);
        void OnCheckBox(wxCommandEvent& event);

        enum
        {
            CLASS_CHOICE,
            GENERATE_ID_BUTTON,
            CONFIGURE_BUTTON
        };

        static const wxString TITLE;

    private:
        void FlushDataGUIToText();
        void UpdateDataGUIVisibilityAndLayout();
        int FillClassComboBox(const wxString& selectedClass);
        void InitializeClassControls(int classIndex);
        void UserDataTypesChanging() override;
        void UserDataTypesChanged() override;
    };

} }
