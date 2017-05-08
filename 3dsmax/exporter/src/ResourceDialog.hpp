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


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Base class for resource-based dialogs. */
    class ResourceDialog
    {
    public:
        /** Constructor. */
        ResourceDialog();

        /** Destructor. */
        virtual ~ResourceDialog();

        /**
         * Creates a new dialog and runs it as a modal dialog.
         * @param dialogID [in] - Resource identifier for the dialog.
         * @param hWndParent [in] - Handle to parent window. If null, the application
         * window is used.
         */
        virtual INT_PTR DoModal(int dialogID, HWND hWndParent = nullptr);

        /** Attaches the dialog logic to an existing dialog. */
        virtual void Attach(HWND hDlg);

        /** Detaches from the existing dialog. Should only be called if Attach() was called. */
        virtual void Detach() {}

        /** Gets the data from the GUI and stores it in the internal settings. */
        virtual bool GetGUIData();

        void GetTitle(wxString& text);
        void SetTitle(const wxString& text);

        /**
         * Checks a radio button in the dialog.
         * @param idFirstButton [in] - The identifier of the first button.
         * @param idLastButton [in] - The identifier of the last button.
         * @param idCheckButton [in] - The identifier of the checked button.
         */
        void CheckRadioButton(int idFirstButton, int idLastButton, int idCheckButton);

        /**
         * Gets the window handle of a control in the dialog.
         * @param idDlgItem [in] - Identifier of the dialog control to retrieve.
         * @return The control window handle is returned.
         */
        HWND GetDlgItem(int idDlgItem);

        /**
         * Checks the specified button.
         * @param idButton [in] - Identifier of the control in the dialog.
         * @param check [in] - The check state of the button.
         */
        void CheckDlgButton(int idButton, UINT check);

        /** Determines whether the specified button is checked. */
        bool IsDlgButtonChecked(int idButton);

        /** Gets the text from the specified control. */
        wxString GetDlgItemText(int idControl);

        /** Gets the text from the specified control. */
        void GetDlgItemText(int idControl, wxString& text);

        /** Sets the text into the specified control. */
        void SetDlgItemText(int idControl, const wxString& text);

        /** Centers the dialog in its parent. */
        void CenterWindow();

        /** Ends the dialog, returning the specified result to the caller of DoModal(). */
        void End(int result = IDOK);

    protected:
        /** The dialog procedure to be overridden in derived classes. */
        virtual INT_PTR DialogProc(UINT message, WPARAM wParam, LPARAM lParam, bool& handled) = 0;

    private:
        /** The internal dialog procedure. */
        static INT_PTR CALLBACK DialogProcCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    protected:
        HWND hDlg;

        LONG_PTR oldUserData;
        WNDPROC oldWndProc;
    };

} }
