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


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Node animations page for the object settings dialog. */
    class ObjectSettingsDialog_NodeAnimationsPage : public SettingsPage
    {
    private:
        DECLARE_EVENT_TABLE()

    protected:
        wxStaticText* defaultAnimationSamplingLabel;
        ApplicationTextCtrl* defaultAnimationSamplingText;
        ApplicationChoiceCtrl* defaultAnimationSamplingChoice;
        wxButton* addNodeAnimationButton;
        wxButton* removeNodeAnimationButton;
        wxButton* editNodeAnimationButton;
        wxButton* copyNodeAnimationButton;
        ApplicationListCtrl* nodeAnimationsList;

        ObjectAccessor object;
        FinjinObjectSettingsAccessor objectSettings;
        FinjinSceneSettingsAccessor sceneSettings;
        std::vector<FinjinNodeAnimationSettingsAccessor> nodeAnimations;

    public:
        ObjectSettingsDialog_NodeAnimationsPage(wxWindow* parent, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,515 ), long style = wxTAB_TRAVERSAL );
        ~ObjectSettingsDialog_NodeAnimationsPage();

        enum
        {
            ANIMATION_SAMPLING_TEXT = 1 << 0,
            ANIMATION_SAMPLING_CHOICE = 1 << 1,
            TRANSLATION_INTERPOLATION_CHOICE = 1 << 2,
            ROTATION_INTERPOLATION_CHOICE = 1 << 3,
            ADD_ANIMATION_BUTTON = 1 << 4,
            EDIT_ANIMATION_BUTTON = 1 << 5,
            COPY_ANIMATION_BUTTON = 1 << 6,
            REMOVE_ANIMATION_BUTTON = 1 << 7,
            NODE_ANIMATION_LIST = 1 << 8,

            ALL_IDS = 0xffffffff
        };

        bool GetGUIData(int ids);
        bool GetGUIData() override;
        bool SetGUIData() override;

        void UpdateAnimationSamplingTypeEdit();
        void OnEditAnimation();
        void UpdateNodeAnimationListButtons();
        void UpdateNodeAnimationListGUIData(int selectedAnimationIndex = -1);
        void UpdateNodeAnimationGUIData(int index);
        void SortNodeAnimationsList();
        static int wxCALLBACK CompareAnimationNames(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);
        static int wxCALLBACK CompareAnimationFrames(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);
        static int wxCALLBACK CompareAnimationLength(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);
        static int wxCALLBACK CompareAnimationSampling(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);
        static int wxCALLBACK CompareAnimationLooped(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);

        wxString GetAnimationSamplingText(FinjinNodeAnimationSettingsAccessor& anim);
        wxString GetAnimationLoopedText(FinjinNodeAnimationSettingsAccessor& anim);

        void OnAnimationSamplingTextChanged(wxCommandEvent& event);
        void OnAnimationSamplingChoice(wxCommandEvent& event);
        void OnAddAnimationButton(wxCommandEvent& event);
        void OnEditAnimationButton(wxCommandEvent& event);
        void OnCopyAnimationButton(wxCommandEvent& event);
        void OnRemoveAnimationButton(wxCommandEvent& event);
        void OnNodeAnimationListActivate(wxListEvent& event);
        void OnNodeAnimationListColumnClick(wxListEvent& event);
        void OnNodeAnimationListSelectionChange(wxListEvent& event);

        static const wxString TITLE;

    private:
        bool settingGuiData;
        int sortColumn;
        int invertSorting;
    };

} }
