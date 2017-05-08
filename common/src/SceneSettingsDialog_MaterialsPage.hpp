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
#include "FinjinSceneSettingsAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Materials page for the scene settings dialog. */
    class SceneSettingsDialog_MaterialsPage : public SettingsPage
    {
    private:
        DECLARE_EVENT_TABLE()

    protected:
        wxButton* addAdditionalMaterialButton;
        ApplicationTextCtrl* addAdditionalMaterialsText;
        wxButton* addIgnoreMaterialButton;
        ApplicationTextCtrl* ignoreMaterialsText;

        FinjinSceneSettingsAccessor sceneSettings;

        std::map<wxString, MaterialAccessor> disallowedMaterials;
        std::map<wxString, MaterialAccessor> additionalMaterials;

    public:
        SceneSettingsDialog_MaterialsPage( wxWindow* parent, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 501,561 ), long style = wxTAB_TRAVERSAL );
        ~SceneSettingsDialog_MaterialsPage();

        bool GetGUIData() override;
        bool SetGUIData() override;

        enum
        {
            ADDITIONAL_MATERIAL_BUTTON,
            IGNORE_MATERIAL_BUTTON
        };

        void OnAdditionalMaterialButton(wxCommandEvent& event);
        void OnIgnoreMaterialButton(wxCommandEvent& event);

        static const wxString TITLE;
    };

} }
