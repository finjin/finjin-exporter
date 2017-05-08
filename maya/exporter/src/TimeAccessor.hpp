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

    /** Access to a time value. */
    class TimeAccessor
    {
    public:
        /** Default constructor. */
        TimeAccessor();

        /** Assigns the time to this accessor. */
        TimeAccessor& operator = (const TimeAccessor& other);

        /** Determines whether this time equals the other time. */
        bool operator == (const TimeAccessor& other) const;

        /** Determines whether this time doesn't equal the other time. */
        bool operator != (const TimeAccessor& other) const;

        /** Determines whether this time is less than the other time. */
        bool operator < (const TimeAccessor& other) const;

        /** Determines whether this time is greater than the other time. */
        bool operator > (const TimeAccessor& other) const;

        /** Determines whether this time is greater than or equal to the other time. */
        bool operator >= (const TimeAccessor& other) const;

        WxTimeDuration GetDuration() const;

        /** Gets the time as seconds. */
        double GetSeconds() const;

        /** Sets the time as seconds. */
        void SetSeconds(double seconds);

        /** Gets the time native value. */
        double GetValue() const;

        /** Sets the time native value. */
        void SetValue(double value);

        /** Gets the time native value. */
        MTime GetNativeTime() const;

        /** Sets the time native value. */
        void SetNativeTime(MTime time);

        /** Gets the current time in the application. */
        static TimeAccessor GetCurrentTime();

        /** Converts the value specified in user interface units to an internal unit value. */
        static double ConvertUIToInternalValue(double value);

        /** Converts the value specified in internal units to user interface unit value. */
        static double ConvertInternalToUIValue(double value);

        /** Gets the user interface unit name. */
        static const wxString& GetUIUnit(bool plural = true);

        /** Determines whether the user interface unit is time-based. If it isn't time-based, it's frame-based. */
        static bool IsUIUnitTimeBased();

    private:
        MTime t;
    };

} }
