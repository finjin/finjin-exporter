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
#include "LogFileDialog.hpp"
#include "WindowPlacementManager.hpp"
#include "ApplicationControls.hpp"
#include "ApplicationAccessor.hpp"
#include "FileUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(LogFileDialog, FinjinDialog)
    EVT_BUTTON(wxID_OK, LogFileDialog::OnOK)    
    EVT_CLOSE(LogFileDialog::OnCloseWindow)        
END_EVENT_TABLE()

LogFileDialog::LogFileDialog(wxWindow* parent, const wxString& fileName, MonitorMethod monitorMethod) :
    FinjinDialog(parent, wxID_ANY, wxT("Finjin Log"), wxDefaultPosition, wxSize( 600,500 ), wxCAPTION | wxSYSTEM_MENU | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT | wxFRAME_NO_TASKBAR, wxT("LogFileDialog"))
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);

    this->fileName = fileName;
    
    this->monitorMethod = monitorMethod;
    
#if defined(__WXMSW__)
    this->exitMonitorThreadEvent = nullptr;
    this->fileMonitorThread = nullptr;
#endif

    SetSizeHints(wxDefaultSize, wxDefaultSize);
    
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* logSizer;
    logSizer = new wxBoxSizer(wxVERTICAL);
    
    logText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 305,-1 ), wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY );
    logSizer->Add( logText, 1, wxALL|wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
    
    topSizer->Add( logSizer, 1, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
    
    wxBoxSizer* okCancelSizer;
    okCancelSizer = new wxBoxSizer( wxHORIZONTAL );
    
    okButton = new wxButton( this, wxID_OK, wxT("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    okCancelSizer->Add( okButton, 0, wxALL, 5 );
    
    topSizer->Add( okCancelSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    SetSizer(topSizer);

    ReloadFile(true);
    
    if (!WindowPlacementManager::RestorePlacement(this))
    {
        Layout();
        Centre(wxBOTH);
    }

    if (this->monitorMethod == MonitorMethod::MONITOR_FILE)
    {
    #if defined(__WXMSW__)
        this->exitMonitorThreadEvent = CreateEvent(0, true, false, wxT("ExitMonitorThreadEvent"));
        this->fileMonitorThread = CreateThread(0, 0, FileMonitorThreadProc, this, 0, 0);
        SetThreadPriority(this->fileMonitorThread, THREAD_PRIORITY_LOWEST);
    #else
        #pragma error Not implemented
    #endif
    }
}

LogFileDialog::~LogFileDialog()
{    
#if defined(__WXMSW__)
    if (this->fileMonitorThread != nullptr)
    {
        SetEvent(this->exitMonitorThreadEvent);
        WaitForSingleObject(this->fileMonitorThread, 1000);

        CloseHandle(this->exitMonitorThreadEvent);
        CloseHandle(this->fileMonitorThread);
    }
#endif
}

void LogFileDialog::OnOK(wxCommandEvent& event)
{
    WindowPlacementManager::SavePlacement(this);
    if (IsModal())
        MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
    else
        Hide();
}

void LogFileDialog::OnCloseWindow(wxCloseEvent& event)
{
    WindowPlacementManager::SavePlacement(this);
    if (IsModal())
        MODAL_DIALOG_ON_CLOSE(event, wxID_OK)
    else
        Hide();
}

bool LogFileDialog::Show(bool show)
{
    bool wasVisible = IsVisible();
    bool result = FinjinDialog::Show(show);

    //Reload file if dialog is now visible after being hidden, and a file is being monitored
    if (show && !wasVisible)
        ReloadFile(this->monitorMethod == MonitorMethod::NONE);
    
    return result;
}

void LogFileDialog::ReloadFile(bool force)
{
    if (force || IsVisible())
    {
        //Load the file
        if (wxFileExists(this->fileName))
            this->logText->LoadFile(this->fileName);

        //Move cursor to the end position so that the window scrolls all the way down
        this->logText->ShowPosition(this->logText->GetLastPosition());    
    }
}

#if defined(__WXMSW__)
DWORD WINAPI LogFileDialog::FileMonitorThreadProc(void* param)
{
    LogFileDialog* dialog = (LogFileDialog*)param;

    wxString directory = wxFileName(dialog->fileName).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    HANDLE changeNotificationHandle = FindFirstChangeNotification(directory.wx_str(), false, FILE_NOTIFY_CHANGE_SIZE);

    HANDLE waitObjects[2];
    waitObjects[0] = changeNotificationHandle;
    waitObjects[1] = dialog->exitMonitorThreadEvent;

    bool loop = true;
    while (loop)
    {
        if (WaitForMultipleObjects(2, waitObjects, false, INFINITE) == WAIT_OBJECT_0)
        {
            //Change notification received
            dialog->ReloadFile();
            FindNextChangeNotification(changeNotificationHandle);
        }
        else
        {
            //Exit thread notification received
            loop = false;
        }
    }

    FindCloseChangeNotification(changeNotificationHandle);

    return 0;
}
#endif
