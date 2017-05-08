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
#include "ObjectAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * A modal dialog used to edit the global settings.
     * This dialog can be seen within the application by clicking
     * 'Add Bone' from the mesh animation dialog, accessible through the object settings dialog.
     */
    class IncludeExcludeBoneDialog : public FinjinDialog
    {
    private:
        DECLARE_EVENT_TABLE()

    protected:
        wxStaticText* boneLabel;
        ApplicationTextCtrl* boneEdit;
        wxButton* boneButton;
        wxRadioButton* includeButton;
        wxRadioButton* excludeButton;
        wxCheckBox* applyRecursiveCheckbox;
        wxButton* okButton;
        wxButton* cancelButton;

    public:
        IncludeExcludeBoneDialog(wxWindow* parent, const ObjectAccessorSet& meshBones);
        ~IncludeExcludeBoneDialog();

        enum
        {
            BONE_BUTTON
        };

        void OnOK(wxCommandEvent& event);
        void OnBoneButton(wxCommandEvent& event);

        /** The selected bone. */
        ObjectAccessor boneObject;

        /** Indicates whether the bone is being included. If false, the bone is excluded. */
        bool include;

        /** Indicates whether the bone and all its descendants are being affected. */
        bool isTree;

    private:
        ObjectAccessorSet meshBones;
    };

} }
