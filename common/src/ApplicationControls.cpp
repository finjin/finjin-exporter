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
#include "ApplicationControls.hpp"
#include "ApplicationAccessor.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
#if FINJIN_EXPORTER_HOST == FINJIN_EXPORTER_HOST_XSI && defined(USE_XSI_CONTROL_STYLE)
    //XSI overrides the default handling of regular text controls, preventing them from rendering properly
    //As a result, we must turn all text controls into a rich text controls and give them a border
    const long ApplicationTextCtrl::EXTRA_TEXT_CTRL_FLAGS = wxTE_RICH2 | wxSIMPLE_BORDER;

    //Auto URLs are blue, making them very hard to see in XSI, so disable them
    const long ApplicationTextCtrl::DISALLOWED_TEXT_CTRL_FLAGS = wxTE_AUTO_URL;
#elif FINJIN_EXPORTER_HOST == FINJIN_EXPORTER_HOST_3DSMAX && MAX_VERSION_MAJOR >= MAX_VERSION_MAJOR_2010
    const long ApplicationTextCtrl::EXTRA_TEXT_CTRL_FLAGS = 0;

    //Auto URLs are blue, making them very hard to see in Max 2010 and up, so disable them
    const long ApplicationTextCtrl::DISALLOWED_TEXT_CTRL_FLAGS = wxTE_AUTO_URL;
#else
    const long ApplicationTextCtrl::EXTRA_TEXT_CTRL_FLAGS = 0;

    const long ApplicationTextCtrl::DISALLOWED_TEXT_CTRL_FLAGS = 0;
#endif


//Implementation----------------------------------------------------------------
void ApplicationControls::AdjustContainerBackgroundColor(wxWindow* window)
{
    wxColor color;
    if (ApplicationAccessor::GetWindowBackgroundColor(color))
    {
        window->SetBackgroundStyle(wxBG_STYLE_COLOUR);
        window->SetBackgroundColour(color);
    }
}

void ApplicationControls::AdjustTextColors(wxWindow* window)
{
    wxColor color;
    if (ApplicationAccessor::GetTextBackgroundColor(color))
    {
        window->SetBackgroundStyle(wxBG_STYLE_COLOUR);
        window->SetBackgroundColour(color);
    }
    if (ApplicationAccessor::GetTextForegroundColor(color))
        window->SetForegroundColour(color);
}

void ApplicationControls::AdjustStaticTextForegroundColor(wxWindow* window)
{
    wxColor color;
    if (ApplicationAccessor::GetStaticTextForegroundColor(color))
        window->SetForegroundColour(color);
}

void ApplicationControls::SetFinjinIcon(wxTopLevelWindowBase* window)
{
#if defined(__WXMSW__)
    window->SetIcon(wxIcon(wxT("IDI_FINJIN"), wxBITMAP_TYPE_ICO_RESOURCE));
#endif
}
