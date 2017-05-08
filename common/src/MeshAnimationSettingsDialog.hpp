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
#include "FinjinDialog.hpp"
#include "FinjinMeshAnimationSettingsAccessor.hpp"
#include "SpinnerControl.hpp"
#include "AllowExportBone.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * A modal dialog used to add or edit a mesh animation.
     * This dialog can be seen within the application by clicking
     * 'Add' on the 'Mesh Animations' page of the object settings dialog.
     */
    class MeshAnimationSettingsDialog : public FinjinDialog
    {
    private:
        DECLARE_EVENT_TABLE()

    protected:
        wxStaticText* trackLabel;
        ApplicationChoiceCtrl* trackChoice;
        wxStaticText* morphWholeObjectLabel;
        ApplicationChoiceCtrl* morphWholeObjectChoice;
        wxStaticText* nameLabel;
        ApplicationTextCtrl* nameText;
        wxStaticText* startEndFramesLabel;
        SpinnerControl* startFrameSpinner;
        wxStaticText* inBetweenFrameLabel;
        SpinnerControl* endFrameSpinner;
        wxStaticText* timeScalingLabel;
        ApplicationChoiceCtrl* timeScalingChoice;
        SpinnerControl* timeScalingSpinner;
        wxStaticText* samplingLabel;
        ApplicationTextCtrl* samplingText;
        ApplicationChoiceCtrl* samplingChoice;
        wxCheckBox* overrideAnimatedRootStartTimeCheckbox;
        SpinnerControl* animatedRootStartTimeSpinner;
        wxStaticBoxSizer* skeletonAnimationSizer;
        wxCheckBox* copyFirstAnimationKeyToLastCheckbox;
        wxCheckBox* exportAsMorphCheckbox;
        wxCheckBox* embedAnimationCheckbox;
        wxCheckBox* linkToObjectCheckbox;
        wxStaticText* removeBoneTranslationsLabel;
        wxButton* addRemoveTranslationsBoneButton;
        ApplicationTextCtrl* removeBoneTranslationsText;
        wxStaticText* includeExcludeBonesLabel;
        wxButton* includeExcludeBonesButton;
        ApplicationTextCtrl* includeExcludeBonesText;
        wxButton* okButton;
        wxButton* cancelButton;

    public:
        /** The settings that are modified by the dialog. */
        struct Settings
        {
            /** Default constructor. */
            Settings();

            /** Constructor that copies the settings from the specified mesh animation settings object. */
            Settings(FinjinMeshAnimationSettingsAccessor& anim) {SetSettings(anim);}

            /** Gets the local settings and puts them into the specified animation settings. */
            void GetSettings(FinjinMeshAnimationSettingsAccessor& anim);

            /** Gets the specifed animation settings and puts them into the local settings. */
            void SetSettings(FinjinMeshAnimationSettingsAccessor& anim);

            wxString trackName;
            FinjinTimeInterval interval;
            wxString animationName;
            wxString scaleType;
            double scale;
            bool copyFirstKeyToLast;
            bool exportAsMorph;
            bool overrideAnimatedRootStartTime;
            TimeAccessor animatedRootStartTime;
            InheritedBool morphWholeObject;
            bool useAnimationStartTime;
            bool embedAnimation;
            bool linkToObject;
            bool looped;
            double sampleInterval;
            SampleType sampleType;
            wxString translationInterpolationType;
            wxString rotationInterpolationType;
            //InheritedBool exportToSeparateFile;
            //InheritedBool linkToMainSkeleton;
            std::vector<BoneTranslationMask> boneTranslationMasks;
            std::vector<AllowExportBone> allowExportBones;
        };

        /**
         * Constructor.
         * @param parent [in] - The parent window.
         * @param title [in] - The dialog title.
         * @param meshBones [in] - A collection of bones associated with the mesh to which the
         * mesh animation is being created.
         * @param allAnimationNames [in] - A collection of all the existing animation names. This
         * is to prevent duplicate names.
         * @param selectedTrack [in] - Index of the selected track.
         * @param settings [in] - The settings to edit.
         */
        MeshAnimationSettingsDialog
            (
            wxWindow* parent,
            const wxString& title,
            const ObjectAccessorSet& meshBones,
            const std::vector<wxString>& allAnimationNames,
            const std::vector<MeshAnimationTrack>& tracks,
            int selectedTrack,
            Settings settings = Settings()
            );
        ~MeshAnimationSettingsDialog();

        enum
        {
            TRACK_CHOICE,
            TIME_SCALE_CHOICE,
            SAMPLING_TYPE_CHOICE,
            OVERRIDE_ANIMATED_ROOT_START_TIME_CHECKBOX,
            ADD_REMOVE_BONE_TRANSLATION_BUTTON,
            ADD_INCLUDE_EXCLUDE_BONE_BUTTON,
            EMBED_ANIMATION_CHECKBOX
        };

        void GetGUIData();
        void SetGUIData();

        void UpdateTrackControls(bool layout = true);
        void UpdateTimeScaleEdit(bool layout = true);
        void UpdateSamplingTypeEdit(bool layout = true);
        void UpdateEmbedControls(bool layout = true);
        void UpdateOverrideAnimatedRootStartTimeControls(bool layout = true);

        void OnOK(wxCommandEvent& event);
        void OnTrackChoice(wxCommandEvent& event);
        void OnTimeScaleChoice(wxCommandEvent& event);
        void OnSamplingTypeChoice(wxCommandEvent& event);
        void OnOverrideAnimatedRootStartTimeCheckbox(wxCommandEvent& event);
        void OnRemoveBoneTranslationButton(wxCommandEvent& event);
        void OnAddIncludeExcludeBoneButton(wxCommandEvent& event);
        void OnEmbedCheckbox(wxCommandEvent& event);

    public:
        ObjectAccessorSet meshBones;
        std::vector<wxString> allAnimationNames;
        std::vector<MeshAnimationTrack> tracks;
        int selectedTrack;
        Settings settings;
    };

} }
