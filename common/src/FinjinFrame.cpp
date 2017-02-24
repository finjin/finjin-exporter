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
#include "FinjinFrame.hpp"
#include "ApplicationControls.hpp"
#include "ModalDialogFix.hpp"
#include "wxAdoptedWindow.hpp"
#include "wxApplicationWindow.hpp"
#include "Dialogs.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
FinjinFrame::FinjinFrame()
{   
}

FinjinFrame::FinjinFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
{
    Create(parent, id, title, pos, size, style, name);    
}

FinjinFrame::~FinjinFrame()
{    
}

bool FinjinFrame::Create(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
{
    SetName(name);
    ModalDialogFix::StartingDialogCreate(this);
    
    bool result = wxFrame::Create(parent, id, title, pos, size, style, name);    
    if (result)
        FinishCreation(style);

    ModalDialogFix::FinishedDialogCreate(this);

    return result;
}

bool FinjinFrame::Destroy()
{
    wxWindow* parent = GetParent();
    if (parent != nullptr && parent != Dialogs::GetNullParent())
    {
        if (dynamic_cast<wxAdoptedWindow*>(parent) != nullptr || dynamic_cast<wxApplicationWindow*>(parent) != nullptr)
        {
            Reparent(Dialogs::GetNullParent());
            parent->Destroy();
        }
    }
    
    return wxFrame::Destroy();
}

void FinjinFrame::FinishCreation(long style)
{
    ApplicationControls::AdjustContainerBackgroundColor(this);

    const long NEEDS_ICON_STYLE = wxSYSTEM_MENU | wxCAPTION;
    if ((style & NEEDS_ICON_STYLE) == NEEDS_ICON_STYLE)
        ApplicationControls::SetFinjinIcon(this);
}
