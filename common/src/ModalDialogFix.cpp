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
#include "ModalDialogFix.hpp"

using namespace Finjin::Exporter;


#if USING_MODAL_DIALOG_FIX

//Static initialization---------------------------------------------------------
ModalDialogFix::DialogStack ModalDialogFix::dialogs;


//Implementation----------------------------------------------------------------
ModalDialogFix::ModalDialogFix(wxWindow* window) : dialog(window)
{
    //Step 1: An instance of this class is created just after the modal dialog is declared
    //        This only needs to be done for non-system dialogs

    dialogs.push_back(dialog);
}

ModalDialogFix::ModalDialogFix(wxDialog* window) : dialog(window)
{
    //Step 1: An instance of this class is created just after the modal dialog is declared
    //        This only needs to be done for non-system dialogs

    dialogs.push_back(dialog);
}

ModalDialogFix::~ModalDialogFix()
{
    //Step 6: This is called automatically when the ModalDialogFix instance goes out of scope
    wxString msg = wxString::Format(wxT("ModalDialogFix::EndingModalDialog(): %s"), this->dialog.GetName().wx_str());
}

bool ModalDialogFix::IsActive()
{
    return !dialogs.empty();
}

void ModalDialogFix::StartingDialogCreate(wxWindow* dialog)
{
    //Step 2: FinjinDialog::Create() calls this function before actually creating the dialog

    //Pause the renderer when a dialog is about to be created
    wxString msg = wxString::Format(wxT("ModalDialogFix::StartingDialogCreate(): %s"), dialog->GetName().wx_str());
}

void ModalDialogFix::FinishedDialogCreate(wxWindow* dialog)
{
    //Step 3: FinjinDialog::Create() calls this function after actually creating the dialog

    //Unpause the renderer after the dialog is created
    wxString msg = wxString::Format(wxT("ModalDialogFix::StartingDialogCreate(): %s"), dialog->GetName().wx_str());
}

void ModalDialogFix::StartingModalDialog(wxWindow* dialog)
{
    //Step 4: FinjinDialog::ShowModal() calls this before showing the dialog modally
}

void ModalDialogFix::EndingModalDialog(wxWindow* dialog)
{
    //Step 5: FinjinDialog::EndModal() calls this after showing the dialog modally

    //If the dialog was created using a ModalDialogFix instance, it will be at the end of the list
    if (!dialogs.empty() && dialogs.back().window == dialog)
    {
        dialogs.pop_back();
        wxString msg = wxString::Format(wxT("ModalDialogFix::EndingModalDialog(): %s"), dialog->GetName().wx_str());
    }
}


#endif
