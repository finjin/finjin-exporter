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
#include "MeshAnimationTypeDetector.hpp"
#include "MeshAnimationTracksDetector.hpp"
#include "ObjectSettingsDialog_MeshAnimationsPage_MixerControl.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Mesh animations page for the object settings dialog. */
    class ObjectSettingsDialog_MeshAnimationsPage : public SettingsPage
    {
    private:
        DECLARE_EVENT_TABLE()

    protected:
        wxStaticText* meshAnimationTypeLabel;
        ApplicationChoiceCtrl* meshAnimationTypeChoice;
        wxStaticText* selectedMeshAnimationTypeLabel;
        wxStaticText* skeletonNameLabel;
        ApplicationTextCtrl* skeletonNameText;
        wxStaticText* exportSkeletonLabel;
        wxCheckBox* exportSkeletonCheckbox;
        wxCheckBox* exportedMeshDeformedBySkeletonCheckbox;
        wxCheckBox* embedSkeletonCheckbox;
        wxCheckBox* linkSkeletonToObjectCheckbox;
        wxStaticText* referencePoseLabel;
        ApplicationChoiceCtrl* referencePoseChoice;
        SpinnerControl* referencePoseTimeSpinner;
        wxStaticText* referencePoseNoteLabel;
        wxCheckBox* exportPosesCheckbox;
        wxCheckBox* removeBonesWithNoInfluenceCheckBox;
        wxStaticText* skeletonAnimationSamplingLabel;
        ApplicationTextCtrl* skeletonAnimationSamplingText;
        ApplicationChoiceCtrl* skeletonAnimationSamplingChoice;
        wxStaticText* vertexAnimationSamplingLabel;
        ApplicationTextCtrl* vertexAnimationSamplingText;
        ApplicationChoiceCtrl* vertexAnimationSamplingChoice;
        wxStaticText* animatedRootLabel;
        ApplicationTextCtrl* animatedRootText;
        wxButton* animatedRootChooseBoneButton;
        wxButton* animatedRootChooseObjectButton;
        wxButton* addMeshAnimationButton;
        wxButton* removeMeshAnimationButton;
        wxButton* editMeshAnimationButton;
        wxButton* copyMeshAnimationButton;
        wxButton* syncToBipedMixerButton;
        ApplicationListCtrl* meshAnimationsList;

        ObjectAccessor object;
        FinjinObjectSettingsAccessor objectSettings;
        FinjinSceneSettingsAccessor sceneSettings;
        std::vector<FinjinMeshAnimationSettingsAccessor> meshAnimations;
        MeshAnimationTypeDetector meshAnimationType;
        MeshAnimationTypeDetector defaultMeshAnimationType;
        MeshAnimationTracksDetector trackDetector;

        ObjectSettingsDialog_MeshAnimationsPage_MixerControl mixerControl;

    public:
        ObjectSettingsDialog_MeshAnimationsPage(wxWindow* parent, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,515 ), long style = wxTAB_TRAVERSAL );
        ~ObjectSettingsDialog_MeshAnimationsPage();

        enum
        {
            MESH_ANIMATION_TYPE_CHOICE = 1 << 0,
            EXPORT_SKELETON_CHECKBOX = 1 << 1,
            EMBED_SKELETON_CHECKBOX = 1 << 2,
            SKELETON_ANIMATION_SAMPLING_TEXT = 1 << 3,
            SKELETON_ANIMATION_SAMPLING_CHOICE = 1 << 4,
            VERTEX_ANIMATION_SAMPLING_TEXT = 1 << 5,
            VERTEX_ANIMATION_SAMPLING_CHOICE = 1 << 6,
            ANIMATED_ROOT_TEXT = 1 << 7,
            ANIMATED_ROOT_CHOOSE_BONE_BUTTON = 1 << 8,
            ANIMATED_ROOT_CHOOSE_OBJECT_BUTTON = 1 << 9,
            EXPORT_TO_SEPARATE_FILE_CHECKBOX = 1 << 10,
            ADD_ANIMATION_BUTTON = 1 << 11,
            EDIT_ANIMATION_BUTTON = 1 << 12,
            COPY_ANIMATION_BUTTON = 1 << 13,
            REMOVE_ANIMATION_BUTTON = 1 << 14,
            SYNC_TO_BIPED_MIXER_BUTTON = 1 << 15,
            MESH_ANIMATION_LIST = 1 << 16,
            REFERENCE_POSE_CHOICE = 1 << 17,

            ALL_IDS = 0xffffffff
        };

        bool GetGUIData(int ids);
        bool GetGUIData() override;
        bool SetGUIData() override;

        void UpdateMeshAnimationTypeDefaultText();
        void UpdateExportSkeletonControls();
        void UpdateEmbedSkeletonControls();
        void UpdateReferencePoseControls();
        void UpdateSkeletonAnimationSamplingTypeEdit();
        void UpdateVertexAnimationSamplingTypeEdit();
        void UpdateAnimatedSkeletonRootEdit();
        void OnEditAnimation();
        void UpdateMeshAnimationListButtons();
        void UpdateMeshAnimationListGUIData(int selectedAnimationIndex = -1);
        void UpdateMeshAnimationGUIData(int index);
        void SortMeshAnimationsList();
        static int wxCALLBACK CompareAnimationNames(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);
        static int wxCALLBACK CompareAnimationTrackNames(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);
        static int wxCALLBACK CompareAnimationFrames(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);
        static int wxCALLBACK CompareAnimationLength(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);
        static int wxCALLBACK CompareAnimationSampling(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);

        wxString GetAnimationTrackText(FinjinMeshAnimationSettingsAccessor& anim);
        wxString GetAnimationSamplingText(FinjinMeshAnimationSettingsAccessor& anim);

        void OnMeshAnimationTypeChoice(wxCommandEvent& event);
        void OnExportSkeletonCheckBox(wxCommandEvent& event);
        void OnEmbedSkeletonInMeshCheckBox(wxCommandEvent& event);
        void OnReferencePoseChoice(wxCommandEvent& event);
        void OnSkeletonAnimationSamplingTextChanged(wxCommandEvent& event);
        void OnSkeletonAnimationSamplingChoice(wxCommandEvent& event);
        void OnVertexAnimationSamplingTextChanged(wxCommandEvent& event);
        void OnAnimatedRootChooseBoneButton(wxCommandEvent& event);
        void OnAnimatedRootChooseObjectButton(wxCommandEvent& event);
        void OnVertexAnimationSamplingChoice(wxCommandEvent& event);
        void OnAddAnimationButton(wxCommandEvent& event);
        void OnEditAnimationButton(wxCommandEvent& event);
        void OnCopyAnimationButton(wxCommandEvent& event);
        void OnRemoveAnimationButton(wxCommandEvent& event);
        void OnSyncToBipedMixerButton(wxCommandEvent& event);
        void OnMeshAnimationListActivate(wxListEvent& event);
        void OnMeshAnimationListColumnClick(wxListEvent& event);
        void OnMeshAnimationListSelectionChange(wxListEvent& event);

        static const wxString TITLE;

    private:
        ObjectAccessorSet meshBones;
        ObjectAccessor animatedSkeletonRootObject;
        bool settingGuiData;
        int sortColumn;
        int invertSorting;
    };

} }
