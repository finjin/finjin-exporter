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


//Includes----------------------------------------------------------------------
#include "FinjinPrecompiled.hpp"
#include "RemoveBoneTranslationsDialog.hpp"
#include "StringUtilities.hpp"
#include "Dialogs.hpp"
#include "Strings.hpp"
#include "WindowPlacementManager.hpp"
#include "ApplicationControls.hpp"
#include "ChooseObjectsPrompt.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(RemoveBoneTranslationsDialog, FinjinDialog)
    EVT_BUTTON(wxID_OK, RemoveBoneTranslationsDialog::OnOK)
    EVT_BUTTON(RemoveBoneTranslationsDialog::BONE_BUTTON, RemoveBoneTranslationsDialog::OnBoneButton)
END_EVENT_TABLE()

RemoveBoneTranslationsDialog::RemoveBoneTranslationsDialog(wxWindow* parent, const ObjectAccessorSet& meshBones) :
    FinjinDialog(parent, wxID_ANY, wxT("Remove Bone Translations"), wxDefaultPosition, wxSize( 301,125 ), wxDEFAULT_DIALOG_STYLE, wxT("RemoveBoneTranslationsDialog"))
{
    this->meshBones = meshBones;

    SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* boneSizer;
    boneSizer = new wxBoxSizer( wxHORIZONTAL );

    boneLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Bone"), wxDefaultPosition, wxDefaultSize, 0 );
    boneLabel->Wrap( -1 );
    boneSizer->Add( boneLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    boneEdit = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 160,-1 ), wxTE_READONLY );

    boneSizer->Add( boneEdit, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    boneButton = new wxButton( this, wxID_ANY, wxT("Choose..."), wxDefaultPosition, wxDefaultSize, 0 );
    boneSizer->Add( boneButton, 0, wxTOP|wxRIGHT, 5 );

    topSizer->Add( boneSizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* includeExcludeSizer;
    includeExcludeSizer = new wxBoxSizer( wxHORIZONTAL );

    removeTranslationsLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Remove Translations"), wxDefaultPosition, wxDefaultSize, 0 );
    removeTranslationsLabel->Wrap( -1 );
    includeExcludeSizer->Add( removeTranslationsLabel, 0, wxALL, 5 );

    removeXTranslationCheckbox = new wxCheckBox( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );

    includeExcludeSizer->Add( removeXTranslationCheckbox, 0, wxALL, 5 );

    removeYTranslationCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );

    includeExcludeSizer->Add( removeYTranslationCheckbox, 0, wxALL, 5 );

    removeZTranslationCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );

    includeExcludeSizer->Add( removeZTranslationCheckbox, 0, wxALL, 5 );

    topSizer->Add( includeExcludeSizer, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );

    okButton = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );

    cancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( cancelButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 5 );

    topSizer->Add( okCancelSizer, 0, wxALIGN_RIGHT|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    SetSizer(topSizer);

    this->boneButton->SetId(BONE_BUTTON);

    if (!WindowPlacementManager::RestorePlacement(this))
    {
        Layout();
        Centre(wxBOTH);
    }
}

RemoveBoneTranslationsDialog::~RemoveBoneTranslationsDialog()
{
}

void RemoveBoneTranslationsDialog::OnOK(wxCommandEvent& event)
{
    WindowPlacementManager::SavePlacement(this);

    this->mask = FinjinVector3(1,1,1);
    if (this->removeXTranslationCheckbox->GetValue())
        this->mask.x = 0;
    if (this->removeYTranslationCheckbox->GetValue())
        this->mask.y = 0;
    if (this->removeZTranslationCheckbox->GetValue())
        this->mask.z = 0;

    if (!this->boneObject.IsValid())
        Dialogs::ShowMessage(this, Strings::BONE_REQUIRED, Strings::DATA_ENTRY_ERROR, wxINFORMATION_DIALOG_FLAGS);
    else if (this->mask == FinjinVector3(1,1,1))
        Dialogs::ShowMessage(this, Strings::REMOVE_TRANSLATION_COMPONENT_REQUIRED, Strings::DATA_ENTRY_ERROR, wxINFORMATION_DIALOG_FLAGS);
    else
        MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
}

void RemoveBoneTranslationsDialog::OnBoneButton(wxCommandEvent& event)
{
    ChooseObjectsPrompt choose(this, HowMany::CHOOSE_ONE, ObjectFilterType::ALL, &this->meshBones);
    if (choose.ShowPrompt())
    {
        this->boneObject = choose.GetObject();
        this->boneEdit->SetValue(this->boneObject.GetFullName());
    }
}
