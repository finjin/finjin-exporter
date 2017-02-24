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
#include "SettingsPage.hpp"
#include "SpinnerControl.hpp"
#include "FinjinObjectSettingsAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Sky box page for the object settings dialog. */
    class ObjectSettingsDialog_SkyPage : public SettingsPage
    {
    private:

    protected:
        wxCheckBox* enableCheckbox;
        
        FinjinObjectSettingsAccessor objectSettings;

    public:
        ObjectSettingsDialog_SkyPage(wxWindow* parent, FinjinObjectSettingsAccessor objectSettings, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 494,573 ), long style = wxTAB_TRAVERSAL );
        ~ObjectSettingsDialog_SkyPage();

        bool GetGUIData() override;
        bool SetGUIData() override;    

        static const wxString TITLE;
    };

} }
