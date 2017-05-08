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


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Manipulates the 'auto keyframe' 3DS Max setting. */
    class AutoKeyState
    {
    public:
        AutoKeyState()
        {
            this->enabled = GetSetKeySuspended() ? true : false;
        }

        /** Indicates whether auto keyframing is enabled. */
        bool IsEnabled() const
        {
            return GetSetKeySuspended() ? false : true;
        }

        /** Enables/disables auto keyframing. */
        void Enable(bool enable = true)
        {
            if (enable)
                ResumeSetKeyMode();
            else
                SuspendSetKeyMode();
        }

        /** Disables auto keyframing. */
        void Disable()
        {
            Enable(false);
        }

        /** Restores auto keyframing. */
        void Restore()
        {
            Enable(this->enabled);
        }

    private:
        bool enabled;
    };

} }
