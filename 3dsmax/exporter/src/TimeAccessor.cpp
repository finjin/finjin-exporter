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


//Includes----------------------------------------------------------------------
#include "FinjinPrecompiled.hpp"
#include "TimeAccessor.hpp"
#include "Strings.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
TimeAccessor::TimeAccessor()
{
    this->ticks = 0;
}

TimeAccessor& TimeAccessor::operator = (const TimeAccessor& other)
{
    this->ticks = other.GetNativeTime();
    return *this;
}

bool TimeAccessor::operator == (const TimeAccessor& other) const
{
    return this->ticks == other.GetNativeTime();
}

bool TimeAccessor::operator != (const TimeAccessor& other) const
{
    return this->ticks != other.GetNativeTime();
}

bool TimeAccessor::operator < (const TimeAccessor& other) const
{
    return this->ticks < other.GetNativeTime();
}

bool TimeAccessor::operator > (const TimeAccessor& other) const
{
    return this->ticks > other.GetNativeTime();
}

bool TimeAccessor::operator >= (const TimeAccessor& other) const
{
    return this->ticks >= other.GetNativeTime();
}

WxTimeDuration TimeAccessor::GetDuration() const
{
    return WxTimeDuration::Seconds(TicksToSec(this->ticks), APP_TIME_RESOLUTION_UNIT);
}

double TimeAccessor::GetSeconds() const
{
    return TicksToSec(this->ticks);
}

void TimeAccessor::SetSeconds(double seconds)
{
    this->ticks = SecToTicks(seconds);
}

double TimeAccessor::GetValue() const
{
    return this->ticks;
}

void TimeAccessor::SetValue(double value)
{
    this->ticks = RoundToInt(value);
}

TimeValue TimeAccessor::GetNativeTime() const
{
    return this->ticks;
}

void TimeAccessor::SetNativeTime(TimeValue value)
{
    this->ticks = value;
}

TimeAccessor TimeAccessor::GetCurrentTime()
{
    TimeAccessor t;
    t.SetNativeTime(GetCOREInterface()->GetTime());
    return t;
}

double TimeAccessor::ConvertUIToInternalValue(double value)
{
    return value * GetTicksPerFrame();
}

double TimeAccessor::ConvertInternalToUIValue(double value)
{
    return RoundToInt(value) / GetTicksPerFrame();            
}

const wxString& TimeAccessor::GetUIUnit(bool plural)
{
    return plural ? Strings::FRAMES : Strings::FRAME;
}

bool TimeAccessor::IsUIUnitTimeBased()
{
    //Always frames
    return false;
}
