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
#include "ObjectFilterType.hpp"
#include "FilteredSceneObjects.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** 
     * A dialog that displays objects and allows the user to select them
     * This is needed in Maya and XSI since they don't have such a dialog.
     */
    class ChooseObjectsDialog : public FinjinDialog
    {
    private:
        DECLARE_EVENT_TABLE()
    
    protected:
        wxStaticText* nameLabel;
        ApplicationTextCtrl* nameTextBox;
        wxCheckBox* showFullNamesCheckbox;
        wxListBox* objectsListBox;
        wxCheckBox* meshesCheckBox;
        wxCheckBox* planesCheckBox;
        wxCheckBox* lightsCheckBox;
        wxCheckBox* camerasCheckBox;
        wxCheckBox* otherCheckBox;
        wxButton* allButton;
        wxButton* noneButton;
        wxButton* invertButton;
        wxButton* okButton;
        wxButton* cancelButton;
    
    public:
        ChooseObjectsDialog
            (
            wxWindow* parent, 
            bool singleSelection = true, 
            ObjectFilterType types = ObjectFilterType::ALL, 
            const ObjectAccessorSet* chooseObjects = nullptr,
            const ObjectAccessorSet* excludeObjects = nullptr,
            wxWindowID id = wxID_ANY, 
            const wxString& title = wxEmptyString, 
            const wxPoint& pos = wxDefaultPosition
            );
        ~ChooseObjectsDialog();        

        enum
        {
            NAME_TEXT,
            SHOW_FULL_NAMES_CHECKBOX,
            OBJECTS_LIST,
            SELECT_ALL_BUTTON,
            SELECT_NONE_BUTTON,
            SELECT_INVERT_BUTTON,
            MESHES_CHECKBOX,
            PLANES_CHECKBOX,
            LIGHTS_CHECKBOX,
            CAMERAS_CHECKBOX,
            OTHER_CHECKBOX
        };

        void OnOK(wxCommandEvent& event);
        void OnNameTextChanged(wxCommandEvent& event);
        void OnShowFullNamesCheckBox(wxCommandEvent& event);
        void OnObjectsListDClick(wxCommandEvent& event);
        void OnSelectAllButton(wxCommandEvent& event);
        void OnSelectNoneButton(wxCommandEvent& event);
        void OnSelectInvertButton(wxCommandEvent& event);
        void OnMeshesCheckBox(wxCommandEvent& event);
        void OnPlanesCheckBox(wxCommandEvent& event);
        void OnLightsCheckBox(wxCommandEvent& event);
        void OnCamerasCheckBox(wxCommandEvent& event);
        void OnOtherCheckBox(wxCommandEvent& event);

        void SetGUIData();

    private:
        void FilterObjects(bool saveSelection = false);
        void HandleOK();

    public:
        ObjectAccessorVector chosenObjects;

    private:
        FilteredSceneObjects filteredSceneObjects;        
    };

} }
