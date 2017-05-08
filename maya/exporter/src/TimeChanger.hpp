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
#include "TimeAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * Instances of this class change the animation time, then changes it back to its original value.
     * This is needed because Maya has no way to evaluate attributes at various times
     * without changing the time. This kind of functionality is essential during export.
     *
     * This class also provides a way of shutting off the time changing capability,
     * which allows a lot of common code to operate without special cases. This is needed
     * by the real-time scene viewports, since they only operate on the current time.
     *
     * By default, time changing is disabled, and is typically enabled with a call to
     * to ApplicationAccessor::EnableExport().
     */
    class TimeChanger
    {
    public:
        /** If time changing is enabled, Maya's current animation time is changed to the specified time. */
        TimeChanger(MTime newTime, bool restoreTime = true);

        /** If time changing is enabled, Maya's current animation time is changed to the specified time. */
        TimeChanger(TimeAccessor newTime, bool restoreTime = true);

        /** If the time was changed, it is restored. */
        ~TimeChanger();

        /** Indicates whether time changing is enabled. */
        static bool IsEnabled();

        /** Enables/disables time changing. */
        static void Enable(bool enable = true);

        /** Disables time changing. */
        static void Disable();

    private:
        static bool enabled;
        MTime oldTime;

        /** Indicates whether the caller wants to restore the time later. */
        bool restoreTime;

        /** Indicates whether the system is enabled so that the time can be restored later. */
        bool enabledOnCreation;
    };


} }
