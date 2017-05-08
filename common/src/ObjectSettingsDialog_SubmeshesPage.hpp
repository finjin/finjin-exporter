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
#include "FinjinObjectSettingsAccessor.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "TextureCoordinateSetMappings.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Mesh page for the object settings dialog. */
    class ObjectSettingsDialog_SubmeshesPage : public SettingsPage
    {
    private:
        DECLARE_EVENT_TABLE()

    protected:
        wxCheckBox* useCustomTextureCoordinateCheckbox;
        wxStaticText* activeTextureCoordinateSetLabel;
        ApplicationChoiceCtrl* submeshTextureCoordinateSetChoice;
        wxStaticText* textureCoordinateSetsLabel;
        wxButton* refreshTextureCoordinateSetsButton;
        wxButton* clearTextureCoordinateSetsButton;
        wxStaticText* submeshNameLabel;
        ApplicationTextCtrl* submeshNameText;
        wxStaticText* renderQueueLabel;
        wxTextCtrl* renderQueueText;
        wxStaticText* renderQueuePriorityLabel;
        SpinnerControl* renderQueuePrioritySpinner;
        ApplicationListCtrl* textureCoordinateSetsList;
        wxButton* moveTextureCoordinateSetUpButton;
        wxButton* moveTextureCoordinateSetDownButton;
        wxButton* removeTextureCoordinateSetButton;
        wxButton* textureCoordinateSetUButton;
        wxButton* textureCoordinateSetUVButton;
        wxButton* textureCoordinateSetUVWButton;

        ObjectAccessor object;
        FinjinObjectSettingsAccessor objectSettings;
        SubmeshesSettings detectedSubmeshesTextureCoordinateSets;
        SubmeshesSettings submeshes;

    public:
        ObjectSettingsDialog_SubmeshesPage(wxWindow* parent, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 494,573 ), long style = wxTAB_TRAVERSAL );
        ~ObjectSettingsDialog_SubmeshesPage();

        enum
        {
            CUSTOMIZE_SUBMESHES_CHECKBOX,
            SUBMESH_TEXTURE_COORDINATE_SETS_CHOICE,
            REFRESH_TEXTURE_COORDINATE_SETS_BUTTON,
            CLEAR_TEXTURE_COORDINATE_SETS_BUTTON,
            SUBMESH_NAME_TEXT,
            RENDER_QUEUE_TEXT,
            RENDER_QUEUE_PRIORITY_SPINNER,
            MOVE_TEXTURE_COORDINATE_SET_UP_BUTTON,
            MOVE_TEXTURE_COORDINATE_SET_DOWN_BUTTON,
            REMOVE_TEXTURE_COORDINATE_SET_DOWN_BUTTON,
            TEXTURE_COORDINATE_SET_U_BUTTON,
            TEXTURE_COORDINATE_SET_UV_BUTTON,
            TEXTURE_COORDINATE_SET_UVW_BUTTON
        };

        bool GetGUIData() override;
        bool SetGUIData() override;

        void OnCustomizeSubmeshesCheckBox(wxCommandEvent& event);
        void OnSubmeshTextureCoordinateSetsChoice(wxCommandEvent& event);
        void OnRefreshTextureCoordinateSetsButton(wxCommandEvent& event);
        void OnClearTextureCoordinateSetsButton(wxCommandEvent& event);
        void OnSubmeshNameText(wxCommandEvent& event);
        void OnRenderQueueText(wxCommandEvent& event);
        void OnRenderQueuePriorityValueChanged(SpinnerControlEvent& event);
        void OnMoveTextureCoordinateSetUpButton(wxCommandEvent& event);
        void OnMoveTextureCoordinateSetDownButton(wxCommandEvent& event);
        void OnRemoveTextureCoordinateSetButton(wxCommandEvent& event);
        void OnTextureCoordinateSetUButton(wxCommandEvent& event);
        void OnTextureCoordinateSetUVButton(wxCommandEvent& event);
        void OnTextureCoordinateSetUVWButton(wxCommandEvent& event);

        void UpdateSubmeshControls();
        void MoveTextureCoordinateSet(int direction);
        void SetTextureCoordinateType(TextureCoordinateFlags flags);
        void UpdateTextureCoordinateSetGUIData(int i);

        static const wxString TITLE;
    };

} }
