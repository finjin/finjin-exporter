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
#include "FinjinNodeAnimationSettingsAccessor.hpp"
#include "SpinnerControl.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * A modal dialog used to add or edit a node animation.
     * This dialog can be seen within the application by clicking
     * 'Add' on the 'Node Animations' page of the object settings dialog.
     */
    class NodeAnimationSettingsDialog : public FinjinDialog
    {
    private:
        DECLARE_EVENT_TABLE()
    
    protected:
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
        wxStaticText* translationInterpolationLabel;
        wxStaticText* rotationInterpolationLabel;
        ApplicationChoiceCtrl* translationInterpolationChoice;
        ApplicationChoiceCtrl* rotationInterpolationChoice;
        wxCheckBox* embedAnimationCheckbox;
        wxCheckBox* linkToObjectCheckbox;
        wxCheckBox* enabledCheckbox;
        wxCheckBox* loopedCheckbox;
        wxCheckBox* useDefinedAnimationStartTimeCheckbox;
        wxCheckBox* copyFirstAnimationKeyToLastCheckbox;
        wxButton* okButton;
        wxButton* cancelButton;
    
    public:
        /** The settings that are modified by the dialog. */
        struct Settings
        {
            /** Default constructor. */
            Settings();

            /** Constructor that copies the settings from the specified node animation settings object. */
            Settings(FinjinNodeAnimationSettingsAccessor& anim) {SetSettings(anim);}

            /** Gets the local settings and puts them into the specified animation settings. */
            void GetSettings(FinjinNodeAnimationSettingsAccessor& anim);

            /** Gets the specifed animation settings and puts them into the local settings. */
            void SetSettings(FinjinNodeAnimationSettingsAccessor& anim);

            FinjinTimeInterval interval;
            wxString animationName;
            wxString scaleType;
            double scale;
            bool copyFirstKeyToLast;
            bool useAnimationStartTime;
            bool embedAnimation;
            bool linkToObject;
            bool enabled;
            bool looped;
            double sampleInterval;
            SampleType sampleType;        
            wxString translationInterpolationType;
            wxString rotationInterpolationType;
        };

        /**
         * Constructor.
         * @param parent [in] - The parent window.
         * @param title [in] - The dialog title.
         * @param allAnimationNames [in] - A collection of all the existing animation names. This
         * is to prevent duplicate names.
         * @param settings [in] - The settings to edit.
         */
        NodeAnimationSettingsDialog
            (
            wxWindow* parent, 
            const wxString& title, 
            const std::vector<wxString>& allAnimationNames,
            Settings settings = Settings()
            );
        ~NodeAnimationSettingsDialog();        

        enum
        {
            TIME_SCALE_CHOICE,
            SAMPLING_TYPE_CHOICE,
            EMBED_ANIMATION_CHECKBOX
        };

        void GetGUIData();
        void SetGUIData();

        void UpdateTimeScaleEdit(bool layout = false);
        void UpdateSamplingTypeEdit(bool layout = false);
        void UpdateEmbedControls(bool layout = false);

        void OnOK(wxCommandEvent& event);
        void OnTimeScaleChoice(wxCommandEvent& event);
        void OnSamplingTypeChoice(wxCommandEvent& event);
        void OnEmbedCheckbox(wxCommandEvent& event);

    public:
        std::vector<wxString> allAnimationNames;
        Settings settings;
    };

} }
