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
#include "wxApplicationWindow.hpp"
#include "ApplicationAccessor.hpp"
#if defined(__WXMAC__)
    #include "MacUtilities.hpp"
#endif

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
wxApplicationWindow::wxApplicationWindow(bool modal)
{
    this->modal = modal;
    this->subclassed = false;
    
    //Adopt the application window
    WXWidget mainWindow = ApplicationAccessor::GetMainWindow(this->modal);
#if defined(__WXMSW__)
    SetHWND(mainWindow);
#elif defined(__WXMAC__)
    if (mainWindow != nullptr && MacUtilities::GetViewWindow(mainWindow) != nullptr)
    {
        SubclassWin((NSWindow*)MacUtilities::GetViewWindow(mainWindow));
        this->subclassed = true;
    }
    else
    {
        Create(0, wxID_ANY, wxEmptyString);
        Hide();
    }
#endif
    
    //Disable the window if a modal window is to be launched
    if (this->modal)
        Enable(false);
    
    this->destroyed = false;
}

wxApplicationWindow::~wxApplicationWindow()
{
    DestroyCommon();
}

bool wxApplicationWindow::Destroy()
{
    DestroyCommon();
    
    return wxApplicationWindowBase::Destroy();
}

void wxApplicationWindow::DestroyCommon()
{
    if (this->destroyed)
        return;
    
    this->destroyed = true;
    
#if defined(__WXMSW__)
    //Enable window if a modal window was launched
    if (this->modal)
    {
        //For some reason, a window other than the application window is typically activated.
        //Make sure the application window is active
        SetActiveWindow((HWND)ApplicationAccessor::GetMainWindow(this->modal));
        
        Enable(true);
    }
    
    //Un-adopt the application window so it isn't destroyed
    SetHWND(0);
#elif defined(__WXMAC__)
    //Enable window if a modal window was launched
    if (this->modal)
        Enable(true);
    
    //Un-adopt the application window so it isn't destroyed
    if (this->subclassed)
    {
        UnsubclassWin();
        this->subclassed = false;
    }
#endif
}
