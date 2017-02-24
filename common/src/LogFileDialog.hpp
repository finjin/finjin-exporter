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


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class LogFileDialog : public FinjinDialog
    {
    private:
        DECLARE_EVENT_TABLE()
    
    protected:
        ApplicationTextCtrl* logText;
        wxButton* originalFileButton;
        wxButton* okButton;
    
    public:
        /** The method used to monitor the log file. */
        enum class MonitorMethod
        {
            NONE,

            /** Use a monitoring thread to watch for file changes. */
            MONITOR_FILE
        };

        /**
         * Constructor.
         * @param parent [in] - The parent window.
         * @param fileName [in] - Full path to the log file.
         * @param monitorMethod [in] - The method used to monitor the file.
         */
        LogFileDialog(wxWindow* parent, const wxString& fileName, MonitorMethod monitorMethod);

        ~LogFileDialog();        

        void OnOK(wxCommandEvent& event);
        void OnCloseWindow(wxCloseEvent& event);

        /** 
         * Shows the dialog modelessly.
         * @param show [in] - If true, the dialog is shown. Otherwise, the dialog is hidden.
         * @return Indicates whether the dialog was shown.
         */
        bool Show(bool show = true);

    private:
        void ReloadFile(bool force = false);

    private:
        wxString fileName;
        
        MonitorMethod monitorMethod;

    #if defined(__WXMSW__)
        static DWORD WINAPI FileMonitorThreadProc(void* param);

        HANDLE exitMonitorThreadEvent;
        HANDLE fileMonitorThread;
    #endif
    };

} }
