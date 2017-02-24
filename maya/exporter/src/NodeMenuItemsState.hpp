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

    /** 
     * Manipulates Maya Node menu. 
     * This is used when exporting skeletons, since IK constraints and other things
     * need to be disabled in order to put the skeleton back into its bind pose.
     */
    class NodeMenuItemsState
    {
    public:
        NodeMenuItemsState()
        {
            this->enabled = true;
        }

        /** Indicates whether the menu items are enabled. */
        bool IsEnabled() const
        {
            return this->enabled;
        }

        /** Enables/disables the menu items. */
        void Enable(bool enable = true)
        {
            if (enable != this->enabled)
            {
                this->enabled = enable;
                if (this->enabled)
                    MGlobal::executeCommand("doEnableNodeItems true all", DISPLAY_MAYA_COMMAND);
                else
                    MGlobal::executeCommand("doEnableNodeItems false all", DISPLAY_MAYA_COMMAND);
            }
        }

        /** Disables the menu items. */
        void Disable() 
        {
            Enable(false);
        }

        /** Restores the menu state. */
        void Restore()
        {
            Enable(true);
        }

    private:
        bool enabled;
    };

} }
