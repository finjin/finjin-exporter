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

    /** Animations page for the scene settings dialog. */
    class SceneSettingsDialog_AnimationsPage : public SettingsPage
    {
    private:
        DECLARE_EVENT_TABLE()

    protected:
        wxStaticText* skeletonAnimationSamplingLabel;
        ApplicationTextCtrl* skeletonAnimationSamplingText;
        ApplicationChoiceCtrl* skeletonAnimationSamplingChoice;
        wxStaticText* vertexAnimationSamplingLabel;
        ApplicationTextCtrl* vertexAnimationSamplingText;
        ApplicationChoiceCtrl* vertexAnimationSamplingChoice;
        wxStaticText* nodeAnimationSamplingLabel;
        ApplicationTextCtrl* nodeAnimationSamplingText;
        ApplicationChoiceCtrl* nodeAnimationSamplingChoice;

        FinjinSceneSettingsAccessor sceneSettings;

    public:
        SceneSettingsDialog_AnimationsPage( wxWindow* parent, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
        ~SceneSettingsDialog_AnimationsPage();

        void OnSkeletonAnimationSamplingChoice(wxCommandEvent& event);
        void OnVertexAnimationSamplingChoice(wxCommandEvent& event);
        void OnNodeAnimationSamplingChoice(wxCommandEvent& event);

        void UpdateSkeletonAnimationSamplingTypeEdit();
        void UpdateVertexAnimationSamplingTypeEdit();
        void UpdateNodeAnimationSamplingTypeEdit();

        enum
        {
            SKELETON_ANIMATION_SAMPLING_CHOICE = 1 << 0,
            VERTEX_ANIMATION_SAMPLING_CHOICE = 1 << 1,
            NODE_ANIMATION_SAMPLING_CHOICE = 1 << 2
        };

        bool GetGUIData() override;
        bool SetGUIData() override;

        static const wxString TITLE;

    private:
        bool settingGuiData;
    };

} }
