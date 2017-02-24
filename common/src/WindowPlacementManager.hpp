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


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Manages the saving and restoring of window placements (position and size). */
    class WindowPlacementManager
    {
    public:
        /** Gets a window's placement and saves it so that it can be restored later. */
        static void SavePlacement(wxTopLevelWindow* window);

        /** Restores a previously saved placement, setting it into the window. */
        static bool RestorePlacement(wxTopLevelWindow* window);

        /** Indicates whether there is a saved placement for a window with the specified name. */
        static bool HasSavedPlacement(const wxString& name);

    private:
        struct Placement
        {
            wxRect rect;
            bool maximized;
        };

        static bool GetSavedPlacement(const wxString& name, Placement& placement);

    private:
        typedef std::unordered_map<wxString, Placement> Placements;
        static Placements placements;
    };

} }
