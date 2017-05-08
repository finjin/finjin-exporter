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


//Macros------------------------------------------------------------------------
#define USING_MODAL_DIALOG_FIX 0 //Enable it for all versions of Finjin so that it is tested regularly


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

#if USING_MODAL_DIALOG_FIX
    /**
     * There's a problem in XSI and that causes the user interface to lock up when
     * the renderer is running and a top-level dialog (one with the application window
     * as its parent) is shown then closed.
     *
     * This class helps work around that problem by shutting the renderer down when such a
     * dialog is created or destroyed.
     *
     * Note: This bug does not appear to be an issue with wxWidgets, since it also appears
     * when using non-wxWidgets user interface classes with the renderer active.
     */
    class ModalDialogFix
    {
    public:
        ModalDialogFix(wxWindow* dialog);
        ModalDialogFix(wxDialog* dialog);
        ~ModalDialogFix();

        static bool IsRequiredByApplication() {return true;}
        static bool IsActive();
        static void StartingDialogCreate(wxWindow* dialog);
        static void FinishedDialogCreate(wxWindow* dialog);
        static void StartingModalDialog(wxWindow* dialog);
        static void EndingModalDialog(wxWindow* dialog);

    private:
        struct DialogWindow
        {
            wxWindow* window;
            wxDialog* dialog;

            DialogWindow(wxWindow* window) {this->window = window; this->dialog = nullptr;}
            DialogWindow(wxDialog* dialog) {this->window = nullptr; this->dialog = dialog;}

            wxString GetName() const {return this->window ? this->window->GetName() : this->dialog->GetName();}
        };

        DialogWindow dialog;

        typedef std::list<DialogWindow> DialogStack;
        static DialogStack dialogs;
    };
#else
    /** This 'fix' does nothing. */
    class ModalDialogFix
    {
    public:
        ModalDialogFix(wxWindow* dialog) {}
        ModalDialogFix(wxDialog* dialog) {}

        static bool IsRequiredByApplication() {return false;}
        static bool IsActive() {return false;}
        static void StartingDialogCreate(wxWindow* dialog) {}
        static void FinishedDialogCreate(wxWindow* dialog) {}
        static void StartingModalDialog(wxWindow* dialog) {}
        static void EndingModalDialog(wxWindow* dialog) {}
    };
#endif

} }
