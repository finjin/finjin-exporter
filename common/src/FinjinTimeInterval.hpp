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
#include "TimeAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Two time values that define a range of time. */
    struct FinjinTimeInterval
    {
        /** Default constructor. */
        FinjinTimeInterval()
        {
        }

        /** Constructor that takes a start and end time. */
        FinjinTimeInterval(TimeAccessor start, TimeAccessor end)
        {
            this->start = start;
            this->end = end;
        }

        WxTimeDuration GetDuration() const
        {
            return this->end.GetDuration() - this->start.GetDuration();
        }

        /** Determines if the time interval is empty. */
        bool IsEmpty() const
        {
            return this->end == this->start;
        }

        /** Determines if this time interval is equal to the other. */
        bool operator == (const FinjinTimeInterval& other) const
        {
            return this->start == other.start && this->end == other.end;
        }

        /** Determines if this time interval is not equal to the other. */
        bool operator != (const FinjinTimeInterval& other) const
        {
            return this->start != other.start || this->end != other.end;
        }

        /** Compares this time interval to another. */
        int CompareTo(const FinjinTimeInterval& other) const
        {
            if (this->start < other.start)
                return -1;
            if (this->start > other.start)
                return 1;

            //If we're here, both intervals start at the same time
            if (this->end < other.end)
                return -1;
            if (this->end > other.end)
                return 1;

            //Same
            return 0;
        }

        /** Converts the time interval to a user-interface aware string. */
        wxString ToString() const
        {
            if (TimeAccessor::IsUIUnitTimeBased())
            {
                //Time-based interval
                return wxString::Format
                    (
                    wxT("%.2f-%.2f"), 
                    (float)TimeAccessor::ConvertInternalToUIValue(this->start.GetValue()), 
                    (float)TimeAccessor::ConvertInternalToUIValue(this->end.GetValue())
                    );
            }
            else    
            {
                //Frame-based interval
                return wxString::Format
                    (
                    wxT("%d-%d"), 
                    RoundToInt(TimeAccessor::ConvertInternalToUIValue(this->start.GetValue())), 
                    RoundToInt(TimeAccessor::ConvertInternalToUIValue(this->end.GetValue()))
                    );
            }
        }

        TimeAccessor start;
        TimeAccessor end;
    };

} }
