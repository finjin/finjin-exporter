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


//Classes----------------------------------------------------------------------
TimeAccessor::TimeAccessor()
{
    this->t.setUnit(MTime::kSeconds);
    this->t.setValue(0);
}

TimeAccessor& TimeAccessor::operator = (const TimeAccessor& other)
{
    this->t.setUnit(MTime::kSeconds);
    this->t.setValue(other.GetSeconds());
    return *this;
}

bool TimeAccessor::operator == (const TimeAccessor& other) const
{
    return this->t == other.t;
}

bool TimeAccessor::operator != (const TimeAccessor& other) const
{
    return this->t != other.t;
}

bool TimeAccessor::operator < (const TimeAccessor& other) const
{
    return this->t < other.t;
}

bool TimeAccessor::operator > (const TimeAccessor& other) const
{
    return this->t > other.t;
}

bool TimeAccessor::operator >= (const TimeAccessor& other) const
{
    return this->t >= other.t;
}

WxTimeDuration TimeAccessor::GetDuration() const
{
    return WxTimeDuration::Milliseconds(this->t.as(MTime::kMilliseconds));
}

double TimeAccessor::GetSeconds() const
{
    return this->t.as(MTime::kSeconds);
}

void TimeAccessor::SetSeconds(double seconds)
{
    this->t.setUnit(MTime::kSeconds);
    this->t.setValue(seconds);
}

double TimeAccessor::GetValue() const
{
    return this->t.as(MTime::kSeconds);
}

void TimeAccessor::SetValue(double value)
{
    this->t.setUnit(MTime::kSeconds);
    this->t.setValue(value);
}

MTime TimeAccessor::GetNativeTime() const
{
    return this->t;
}

void TimeAccessor::SetNativeTime(MTime time)
{
    this->t = time;
}

TimeAccessor TimeAccessor::GetCurrentTime()
{
    TimeAccessor t;
    t.t.setUnit(MTime::kSeconds);
    t.t.setValue(MAnimControl::currentTime().as(MTime::kSeconds));
    return t;
}

double TimeAccessor::ConvertUIToInternalValue(double value)
{
    MTime uiTime;
    uiTime.setUnit(MTime::uiUnit());
    uiTime.setValue(value);
    return uiTime.as(MTime::kSeconds);
}

double TimeAccessor::ConvertInternalToUIValue(double value)
{
    MTime internalTime;
    internalTime.setUnit(MTime::kSeconds);
    internalTime.setValue(value);
    return internalTime.as(MTime::uiUnit());
}

const wxString& TimeAccessor::GetUIUnit(bool plural)
{
    if (IsUIUnitTimeBased())
        return Strings::TIME;
    else
        return plural ? Strings::FRAMES : Strings::FRAME;
}

bool TimeAccessor::IsUIUnitTimeBased()
{
    switch (MTime::uiUnit())
    {
        case MTime::kHours:
        case MTime::kMinutes:
        case MTime::kSeconds:
        case MTime::kMilliseconds: return true;
        default: return false;
    }
}
