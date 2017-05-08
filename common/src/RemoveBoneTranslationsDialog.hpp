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
     * A modal dialog used to add bones that should have some part of their translation removed.
     */
    class RemoveBoneTranslationsDialog : public FinjinDialog
    {
    private:
        DECLARE_EVENT_TABLE()

    protected:
        wxStaticText* boneLabel;
        ApplicationTextCtrl* boneEdit;
        wxButton* boneButton;
        wxStaticText* removeTranslationsLabel;
        wxCheckBox* removeXTranslationCheckbox;
        wxCheckBox* removeYTranslationCheckbox;
        wxCheckBox* removeZTranslationCheckbox;
        wxButton* okButton;
        wxButton* cancelButton;

    public:
        /**
         * Constructor.
         * @param parent [in] - The parent window.
         * @param meshBones [in] - A collection of bones associated with the mesh whose settings
         * are being modified.
         */
        RemoveBoneTranslationsDialog
            (
            wxWindow* parent,
            const ObjectAccessorSet& meshBones
            );

        ~RemoveBoneTranslationsDialog();

        enum
        {
            BONE_BUTTON
        };

        void OnOK(wxCommandEvent& event);
        void OnBoneButton(wxCommandEvent& event);

        ObjectAccessorSet meshBones;

        /** The bone being added. */
        ObjectAccessor boneObject;

        /** A mask indicating which translations to keep and remove. 1 = keep, 0 = remove. */
        FinjinVector3 mask;
    };

} }
