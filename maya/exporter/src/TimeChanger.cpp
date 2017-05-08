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
#include "TimeChanger.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
bool TimeChanger::enabled = false;


//Implementation----------------------------------------------------------------
TimeChanger::TimeChanger(MTime newTime, bool restoreTime)
{
    this->restoreTime = restoreTime;
    this->enabledOnCreation = IsEnabled();
    if (this->enabledOnCreation)
    {
        this->oldTime = MAnimControl::currentTime();
        this->enabledOnCreation = true;
        MAnimControl::setCurrentTime(newTime);
    }
}

TimeChanger::TimeChanger(TimeAccessor newTime, bool restoreTime)
{
    this->restoreTime = restoreTime;
    this->enabledOnCreation = IsEnabled();
    if (this->enabledOnCreation)
    {
        this->oldTime = MAnimControl::currentTime();
        MAnimControl::setCurrentTime(newTime.GetNativeTime());
    }
}

TimeChanger::~TimeChanger()
{
    if (this->enabledOnCreation && this->restoreTime)
        MAnimControl::setCurrentTime(this->oldTime);
}

bool TimeChanger::IsEnabled()
{
    return enabled;
}

void TimeChanger::Enable(bool enable)
{
    enabled = enable;
}

void TimeChanger::Disable()
{
    Enable(false);
}
