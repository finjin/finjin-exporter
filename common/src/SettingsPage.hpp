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


//Includes----------------------------------------------------------------------
#include "ApplicationControls.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Base class for a settings page in the Scene and Object Settings dialogs */
    class SettingsPage : public wxPanel
    {
    public:
        SettingsPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL ) :
            wxPanel(parent, id, pos, size, style)
        {
            ApplicationControls::AdjustContainerBackgroundColor(this);
        }

        virtual bool GetGUIData() {return true;}
        virtual bool SetGUIData() {return true;}
    };

} }
