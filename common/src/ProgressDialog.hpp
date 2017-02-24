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
#include "Strings.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * The progress dialog displays the progress of a long running task, such as a scene export.
     * It achieves this by being displayed modelessly, but freezing the main user interface so that
     * it appears to be modal. Then the process runs, updating the progress dialog, and eventually
     * notifies the dialog when it's finished.
     *
     * The progress dialog tracks two statuses.
     * 1) The total progress - This is the status of the entire task.
     * 2) The subtask progress - This is the status of some smaller task within the main task.
     *
     * The dialog receives log messages by adding itself as a listener to Finjin::Logger. So, 
     * anything logged with Logger will be picked up by the progress dialog.
     *
     * There can only be one progress dialog in existance at any time. This dialog can be retreved globally
     * by calling ProgressDialog::GetInstance().
     */
    class ProgressDialog : 
        public FinjinDialog, 
        public LogListener
    {
    private:
    
    protected:
        wxStaticText* totalProgressLabel;
        wxGauge* totalProgressGauge;
        wxStaticText* progressLabel;
        wxGauge* progressGauge;
        ApplicationTextCtrl* progressText;
        wxButton* okButton;

        wxString totalProgressText;

        static ProgressDialog* instance;
    
    public:
        DECLARE_EVENT_TABLE()
        DECLARE_DYNAMIC_CLASS(ProgressDialog)    

        ProgressDialog
            (
            const wxString& title = Strings::PROGRESS, 
            LogMessageType logLevel = INFO_LOG_MESSAGE, 
            const wxString& totalProgressText = Strings::TOTAL_PROGRESS
            );
        ~ProgressDialog();

        /** 
         * Displays the dialog modally.
         * This will most likely never be called, but it's useful to have modal functionality for testing.
         * @return wxID_OK will always be returned.
         */
        int ShowModal();
        
        /** 
         * Finishes the task for which the progress dialog was originally created.
         * @param totalProgressLabel [in] - The text to be displayed in the total progress label.
         */
        void FinishedTask(const wxString& totalProgressLabel);
        
        /** 
         * Sets the total progress label.
         * @param text [in] - The text to be displayed in the total progress label.
         */
        void SetTotalProgressLabel(const wxString& text);

        /** 
         * Sets the sub-progress label.
         * @param text [in] - The text to be displayed in the sub-progress label.
         */
        void SetSubprogressLabel(const wxString& text);

        /** 
         * Updates the progress.
         * @param totalProgress [in] - The new total progress. This will be between 0 and 1, inclusive.
         * @param subprogress [in] - The new sub-progress. This will be between 0 and 1, inclusive.
         */
        void UpdateProgress(float totalProgress, float subprogress);

        /**
         * Shows or hides the 'Close' button. This is to be called when the task has completed.
         * @param show [in] - Indicates whether the button should be shown or hidden.
         */
        void ShowCloseButton(bool show);

        /**
         * Shows or hides the sub-progress controls.
         * @param show [in] - Indicates whether the sub-progress controls should be shown or hidden.
         */
        void ShowSubprogress(bool show);

        /** LogListener implementation. */
        void LogMessage(LogMessageType type, const wxString& message);

        /** Gets the one and only progress dialog instance. */
        static ProgressDialog* GetInstance();

        void OnOK(wxCommandEvent& event);
    };

} }
