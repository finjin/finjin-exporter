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
#include "FinjinNodeAnimationSettingsAccessor.hpp"
#include "FinjinNodeAnimationSettingsObject.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
bool FinjinNodeAnimationSettingsAccessor::operator == (const FinjinNodeAnimationSettingsAccessor& other) const
{
    return this->object == other.object;
}

wxString FinjinNodeAnimationSettingsAccessor::GetAnimationName()
{
    return this->object->GetAnimationName();
}

wxString FinjinNodeAnimationSettingsAccessor::GetTimeScaleType()
{
    return this->object->GetTimeScaleType();
}

double FinjinNodeAnimationSettingsAccessor::GetTimeScale()
{
    return this->object->GetTimeScale();
}

bool FinjinNodeAnimationSettingsAccessor::GetCopyFirstKeyToLast()
{
    return this->object->GetCopyFirstKeyToLast();
}

bool FinjinNodeAnimationSettingsAccessor::GetUseAnimationStartTime()
{
    return this->object->GetUseAnimationStartTime();
}

bool FinjinNodeAnimationSettingsAccessor::GetEmbedAnimation()
{
    return this->object->GetEmbedAnimation();
}

bool FinjinNodeAnimationSettingsAccessor::GetLinkToMainObject()
{
    return this->object->GetLinkToMainObject();
}

bool FinjinNodeAnimationSettingsAccessor::GetEnabled()
{
    return this->object->GetEnabled();
}

bool FinjinNodeAnimationSettingsAccessor::GetLooped()
{
    return this->object->GetLooped();
}

double FinjinNodeAnimationSettingsAccessor::GetSampleInterval()
{
    return this->object->GetSampleInterval();
}

SampleType FinjinNodeAnimationSettingsAccessor::GetSampleType()
{
    return this->object->GetSampleType();
}

FinjinTimeInterval FinjinNodeAnimationSettingsAccessor::GetTimeInterval()
{
    Interval maxInterval = this->object->GetTimeInterval();

    FinjinTimeInterval interval;
    interval.start.SetNativeTime(maxInterval.Start());
    interval.end.SetNativeTime(maxInterval.End());
    return interval;
}

wxString FinjinNodeAnimationSettingsAccessor::GetTranslationInterpolationType()
{
    return this->object->GetTranslationInterpolationType();
}

wxString FinjinNodeAnimationSettingsAccessor::GetRotationInterpolationType()
{
    return this->object->GetRotationInterpolationType();
}

void FinjinNodeAnimationSettingsAccessor::SetAnimationName(wxString value)
{
    this->object->SetAnimationName(value);    
}

void FinjinNodeAnimationSettingsAccessor::SetTimeScaleType(wxString value)
{
    this->object->SetTimeScaleType(value);    
}

void FinjinNodeAnimationSettingsAccessor::SetTimeScale(double value)
{
    this->object->SetTimeScale(value);
}

void FinjinNodeAnimationSettingsAccessor::SetCopyFirstKeyToLast(bool value)
{
    this->object->SetCopyFirstKeyToLast(value);
}

void FinjinNodeAnimationSettingsAccessor::SetUseAnimationStartTime(bool value)
{
    this->object->SetUseAnimationStartTime(value);
}

void FinjinNodeAnimationSettingsAccessor::SetEmbedAnimation(bool value)
{
    this->object->SetEmbedAnimation(value);
}

void FinjinNodeAnimationSettingsAccessor::SetLinkToMainObject(bool value)
{
    this->object->SetLinkToMainObject(value);
}

void FinjinNodeAnimationSettingsAccessor::SetEnabled(bool value)
{
    this->object->SetEnabled(value);
}

void FinjinNodeAnimationSettingsAccessor::SetLooped(bool value)
{
    this->object->SetLooped(value);
}

void FinjinNodeAnimationSettingsAccessor::SetSampleInterval(double value)
{
    this->object->SetSampleInterval(value);
}

void FinjinNodeAnimationSettingsAccessor::SetSampleType(SampleType value)
{
    this->object->SetSampleType(value);
}

void FinjinNodeAnimationSettingsAccessor::SetTimeInterval(FinjinTimeInterval interval)
{
    Interval maxInterval;
    maxInterval.SetStart(interval.start.GetNativeTime());
    maxInterval.SetEnd(interval.end.GetNativeTime());
    this->object->SetTimeInterval(maxInterval);
}

void FinjinNodeAnimationSettingsAccessor::SetTranslationInterpolationType(wxString value)
{
    this->object->SetTranslationInterpolationType(value);    
}

void FinjinNodeAnimationSettingsAccessor::SetRotationInterpolationType(wxString value)
{
    this->object->SetRotationInterpolationType(value);    
}
