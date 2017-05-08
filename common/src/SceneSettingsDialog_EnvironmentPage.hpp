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
#include "SpinnerControl.hpp"
#include "FinjinSceneSettingsAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Environment page for the scene settings dialog. */
    class SceneSettingsDialog_EnvironmentPage : public SettingsPage
    {
    private:
        DECLARE_EVENT_TABLE()

    protected:
        wxCheckBox* exportCheckbox;

        wxStaticText* ambientLightColorLabel;
        Finjin::Exporter::ColorPickerControl* ambientLightColor;
        ApplicationChoiceCtrl* ambientLightColorSyncChoice;
        wxStaticText* backgroundColorLabel;
        Finjin::Exporter::ColorPickerControl* backgroundColor;
        ApplicationChoiceCtrl* backgroundColorSyncChoice;
        wxStaticText* nearDistanceLabel;
        SpinnerControl* nearDistanceSpinner;
        wxStaticText* farLabel;
        wxRadioButton* farDistanceButton;
        SpinnerControl* farDistanceSpinner;
        wxRadioButton* farBoxObjectButton;
        ApplicationTextCtrl* farBoxObjectText;
        wxButton* chooseFarBoxObjectButton;
        wxStaticText* fogModeLabel;
        ApplicationChoiceCtrl* fogModeChoice;
        wxStaticText* fogDensityLabel;
        SpinnerControl* fogDensitySpinner;
        wxStaticText* fogStartLabel;
        SpinnerControl* fogStartSpinner;
        wxStaticText* fogEndLabel;
        SpinnerControl* fogEndSpinner;
        wxStaticText* fogColorLabel;
        Finjin::Exporter::ColorPickerControl* fogColor;
        ApplicationChoiceCtrl* fogSyncChoice;
        wxStaticText* shadowTechniqueLabel;
        ApplicationTextCtrl* shadowTechniqueText;
        wxStaticText* shadowFarDistanceLabel;
        SpinnerControl* shadowFarDistanceSpinner;
        wxStaticText* shadowColorLabel;
        Finjin::Exporter::ColorPickerControl* shadowColor;

        FinjinSceneSettingsAccessor sceneSettings;

        ObjectAccessor environmentFarObject;

    public:
        SceneSettingsDialog_EnvironmentPage( wxWindow* parent, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 485,614 ), long style = wxTAB_TRAVERSAL );
        ~SceneSettingsDialog_EnvironmentPage();

        bool GetGUIData() override;
        bool SetGUIData() override;

        enum
        {
            AMBIENT_LIGHT_COLOR,
            AMBIENT_LIGHT_COLOR_SYNC_CHOICE,
            BACKGROUND_COLOR,
            BACKGROUND_COLOR_SYNC_CHOICE,
            NEAR_DISTANCE_SPINNER,
            FAR_DISTANCE_RADIO_BUTTON,
            FAR_DISTANCE_SPINNER,
            FAR_BOX_OBJECT_RADIO_BUTTON,
            FAR_BOX_OBJECT_BUTTON,
            FOG_MODE_CHOICE,
            FOG_SYNC_CHOICE,
            FOG_START_SPINNER,
            FOG_END_SPINNER,
            FOG_DENSITY_SPINNER,
            FOG_COLOR,
            SHADOW_COLOR,
            SHADOW_FAR_DISTANCE_SPINNER
        };

        void OnBackgroundColorChanged(wxColourPickerEvent& event);
        void OnBackgroundColorSyncChoice(wxCommandEvent& event);
        void OnAmbientLightColorChanged(wxColourPickerEvent& event);
        void OnAmbientLightColorSyncChoice(wxCommandEvent& event);
        void OnNearDistanceSpinner(SpinnerControlEvent& event);
        void OnFarDistanceRadioButton(wxCommandEvent& event);
        void OnFarBoxObjectRadioButton(wxCommandEvent& event);
        void OnFarDistanceSpinner(SpinnerControlEvent& event);
        void OnFarBoxObjectButton(wxCommandEvent& event);
        void OnFogModeChoice(wxCommandEvent& event);
        void OnFogSyncChoice(wxCommandEvent& event);
        void OnFogStartSpinner(SpinnerControlEvent& event);
        void OnFogEndSpinner(SpinnerControlEvent& event);
        void OnFogDensitySpinner(SpinnerControlEvent& event);
        void OnFogColorChanged(wxColourPickerEvent& event);
        void OnShadowColorChanged(wxColourPickerEvent& event);
        void OnShadowFarDistanceSpinner(SpinnerControlEvent& event);

        static const wxString TITLE;

    private:
        void UpdateAmbientLightColorControls(bool syncChanged = false);
        void UpdateBackgroundColorControls(bool syncChanged = false);
        void UpdateFogModeControls(bool syncChanged = false);

        ObjectAccessor GetEnvironmentObject();
    };

} }
