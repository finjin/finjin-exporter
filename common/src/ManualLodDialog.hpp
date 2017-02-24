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
#include "SpinnerControl.hpp"
#include "ObjectAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * A modal dialog used to add or edit manual level-of-detail meshes.
     * This dialog can be seen within the application by clicking
     * 'Add' in the 'Manual' area in the 'Mesh LOD' page of the object settings dialog.
     */
    class ManualLodDialog : public FinjinDialog
    {
    private:
        DECLARE_EVENT_TABLE()
    
    protected:
        wxStaticText* objectLabel;
        ApplicationTextCtrl* objectEdit;
        wxButton* objectButton;
        wxStaticText* distanceLabel;
        SpinnerControl* distanceSpinner;
        wxButton* okButton;
        wxButton* cancelButton;
    
    public:
        ManualLodDialog
            (
            wxWindow* parent, 
            const wxString& title, 
            ObjectAccessor baseObject,
            ObjectAccessor object = ObjectAccessor(), 
            float distance = 0,
            ObjectAccessorSet* excludedLodObjects = 0,
            bool autoDistance = false
            );
        ~ManualLodDialog();        

        enum
        {
            MESH_BUTTON
        };

        void SetGUIData();

        void OnOK(wxCommandEvent& event);
        void OnMeshButton(wxCommandEvent& event);

        /** The selected mesh object. */
        ObjectAccessor object;

        /** The selected distance. */
        float distance;

    private:
        ObjectAccessor baseObject;
        ObjectAccessorSet excludedLodObjects;
        bool autoDistance;
    };

} }
