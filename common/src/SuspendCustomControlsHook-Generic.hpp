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

    /**
     * Suspends the application's custom user interface styling.
     * This generic version does nothing.
     */
    class SuspendCustomControlsHook
    {
    public:
        /** Explicitly restores the custom styling. */
        void Unsuspend() {}

        /** Indicates whether the styling is suspended. */
        bool IsSuspended() const {return false;}
    };

} }

#define SUSPEND_CUSTOM_CONTROLS  //Do nothing
