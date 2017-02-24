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
#include "SettingsPage.hpp"
#include "SpinnerControl.hpp"
#include "FinjinObjectSettingsAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class ObjectSettingsDialog;
    class DetectableObjectType;

    /** General page for the object settings dialog. */
    class ObjectSettingsDialog_GeneralPage : public SettingsPage
    {
    private:
        DECLARE_EVENT_TABLE()

    protected:
        wxStaticText* typeLabel;
        ApplicationChoiceCtrl* typeChoice;
        wxStaticText* defaultTypeLabel;
        wxStaticText* visibilityLabel;
        ApplicationChoiceCtrl* visibilityChoice;
        wxCheckBox* visibilityAffectObjectOnlyCheckbox;
        wxStaticText* renderQueueLabel;
        wxTextCtrl* renderQueueText;
        wxStaticText* renderQueuePriorityLabel;
        SpinnerControl* renderQueuePrioritySpinner;
        wxStaticText* renderDistanceLabel;
        SpinnerControl* renderDistanceSpinner;
        wxStaticText* exportedParentLabel;
        ApplicationTextCtrl* exportedParentText;
        wxButton* exportedParentButton;
        wxCheckBox* useRootAsExportedParentCheckbox;
        wxCheckBox* isAttachedObjectCheckbox;
        wxButton* editCustomParameterButton;
        wxCheckBox* ignoreChildrenCheckbox;
        wxStaticText* sortChildrenLabel;
        ApplicationChoiceCtrl* sortChildrenChoice;
        wxStaticText* childOrderLabel;
        SpinnerControl* childOrderSpinner;
        wxCheckBox* isPrefabCheckbox;
        wxCheckBox* isPrefabInstanceCheckbox;
        wxStaticBoxSizer* noteTrackNamesSizer;
        ApplicationTextCtrl* noteTrackNamesText;
        wxButton* addFlagButton;
        ApplicationTextCtrl* flagsText;
        
        ObjectAccessor object;
        FinjinObjectSettingsAccessor objectSettings;
        ObjectSettingsDialog* dialog;
        ObjectAccessor exportedParentObject;

    public:
        ObjectSettingsDialog_GeneralPage(ObjectSettingsDialog* dialog, wxWindow* parent, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 491,671 ), long style = wxTAB_TRAVERSAL);
        ~ObjectSettingsDialog_GeneralPage();

        enum
        {
            TYPE_CHOICE,
            VISIBILITY_CHOICE,
            EXPORTED_PARENT_BUTTON,
            USE_ROOT_AS_EXPORTED_PARENT_CHECKBOX,
            ADD_FLAG_BUTTON
        };

        bool GetGUIData() override;
        bool SetGUIData() override;    

        void OnTypeChoice(wxCommandEvent& event);
        void OnVisibilityChoice(wxCommandEvent& event);
        void OnExportedParentButton(wxCommandEvent& event);
        void UseRootAsExportedParent(wxCommandEvent& event);
        void OnAddFlagButton(wxCommandEvent& event);
        
        DetectableObjectType* UpdateObjectTypeDefaultText();
        void UpdateVisibilityControls();        
        void UpdateExportedParentControls();
        void SortCustomParameterList();
                
        DetectableObjectType* GetSelectedObjectType();

        static const wxString TITLE;
    };

} }
