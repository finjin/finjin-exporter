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

    /** Manipulates the 'auto keyframe' Maya setting. */
    class AutoKeyState
    {
    public:
        AutoKeyState()
        {
            int result;
            MGlobal::executeCommand("autoKeyframe -query -state", result);
            this->enabled = this->originalState = result ? true : false;
        }

        /** Indicates whether auto keyframing is enabled. */
        bool IsEnabled() const
        {
            return this->enabled;
        }

        /** Enables/disables auto keyframing. */
        void Enable(bool enable = true)
        {
            if (enable != this->enabled)
            {
                this->enabled = enable;
                if (this->enabled)
                    MGlobal::executeCommand("autoKeyframe -state true");
                else
                    MGlobal::executeCommand("autoKeyframe -state false");
            }
        }

        /** Disables auto keyframing. */
        void Disable()
        {
            Enable(false);
        }

        /** Restores auto keyframing. */
        void Restore()
        {
            Enable(this->originalState);
        }

    private:
        bool originalState;
        bool enabled;
    };

} }
