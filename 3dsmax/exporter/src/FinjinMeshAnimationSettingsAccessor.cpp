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
#include "FinjinMeshAnimationSettingsAccessor.hpp"
#include "FinjinMeshAnimationSettingsObject.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
bool FinjinMeshAnimationSettingsAccessor::operator == (const FinjinMeshAnimationSettingsAccessor& other) const
{
    return this->object == other.object;
}

wxString FinjinMeshAnimationSettingsAccessor::GetTrack()
{
    return this->object->GetTrack();
}

wxString FinjinMeshAnimationSettingsAccessor::GetAnimationName()
{
    return this->object->GetAnimationName();
}

wxString FinjinMeshAnimationSettingsAccessor::GetTimeScaleType()
{
    return this->object->GetTimeScaleType();
}

double FinjinMeshAnimationSettingsAccessor::GetTimeScale()
{
    return this->object->GetTimeScale();
}

bool FinjinMeshAnimationSettingsAccessor::GetCopyFirstKeyToLast()
{
    return this->object->GetCopyFirstKeyToLast();
}

bool FinjinMeshAnimationSettingsAccessor::GetExportAsMorph()
{
    return this->object->GetExportAsMorph();
}

InheritedBool FinjinMeshAnimationSettingsAccessor::GetMorphWholeObject()
{
    return this->object->GetMorphWholeObject();
}

bool FinjinMeshAnimationSettingsAccessor::GetOverrideAnimatedRootStartTime()
{
    return this->object->GetOverrideAnimatedRootStartTime();
}

TimeAccessor FinjinMeshAnimationSettingsAccessor::GetAnimatedRootStartTime()
{
    TimeAccessor value;
    value.SetNativeTime(this->object->GetAnimatedRootStartTime());
    return value;
}

bool FinjinMeshAnimationSettingsAccessor::GetUseAnimationStartTime()
{
    return this->object->GetUseAnimationStartTime();
}

bool FinjinMeshAnimationSettingsAccessor::GetLooped()
{
    return this->object->GetLooped();
}

double FinjinMeshAnimationSettingsAccessor::GetSampleInterval()
{
    return this->object->GetSampleInterval();
}

SampleType FinjinMeshAnimationSettingsAccessor::GetSampleType()
{
    return this->object->GetSampleType();
}

FinjinTimeInterval FinjinMeshAnimationSettingsAccessor::GetTimeInterval()
{
    Interval maxInterval = this->object->GetTimeInterval();

    FinjinTimeInterval interval;
    interval.start.SetNativeTime(maxInterval.Start());
    interval.end.SetNativeTime(maxInterval.End());
    return interval;
}

bool FinjinMeshAnimationSettingsAccessor::GetEmbedAnimation()
{
    return this->object->GetEmbedAnimation();
}

bool FinjinMeshAnimationSettingsAccessor::GetLinkToMainObject()
{
    return this->object->GetLinkToMainObject();
}

int FinjinMeshAnimationSettingsAccessor::GetBoneTranslationMaskCount()
{
    return this->object->GetBoneTranslationMaskCount();
}

void FinjinMeshAnimationSettingsAccessor::GetBoneTranslationMask(int index, ObjectAccessor& object, FinjinVector3& mask)
{
    Point3 p3;
    this->object->GetBoneTranslationMask(index, object.node, p3);
    mask = FinjinVector3(p3.x, p3.y, p3.z);
}

FinjinVector3 FinjinMeshAnimationSettingsAccessor::GetBoneTranslationMask(ObjectAccessor object)
{
    Point3 p3 = this->object->GetBoneTranslationMask(object.node);
    return FinjinVector3(p3.x, p3.y, p3.z);
}

int FinjinMeshAnimationSettingsAccessor::GetAllowExportBoneCount()
{
    return this->object->GetAllowExportBoneCount();
}

void FinjinMeshAnimationSettingsAccessor::GetAllowExportBone(int index, ObjectAccessor& object, bool& allow, bool& recursive)
{
    this->object->GetAllowExportBone(index, object.node, allow, recursive);
}

void FinjinMeshAnimationSettingsAccessor::SetTrack(wxString value)
{
    this->object->SetTrack(value);
}

void FinjinMeshAnimationSettingsAccessor::SetAnimationName(wxString value)
{
    this->object->SetAnimationName(value);
}

void FinjinMeshAnimationSettingsAccessor::SetTimeScaleType(wxString value)
{
    this->object->SetTimeScaleType(value);
}

void FinjinMeshAnimationSettingsAccessor::SetTimeScale(double value)
{
    this->object->SetTimeScale(value);
}

void FinjinMeshAnimationSettingsAccessor::SetCopyFirstKeyToLast(bool value)
{
    this->object->SetCopyFirstKeyToLast(value);
}

void FinjinMeshAnimationSettingsAccessor::SetExportAsMorph(bool value)
{
    this->object->SetExportAsMorph(value);
}

void FinjinMeshAnimationSettingsAccessor::SetMorphWholeObject(InheritedBool value)
{
    this->object->SetMorphWholeObject(value);
}

void FinjinMeshAnimationSettingsAccessor::SetOverrideAnimatedRootStartTime(bool value)
{
    this->object->SetOverrideAnimatedRootStartTime(value);
}

void FinjinMeshAnimationSettingsAccessor::SetAnimatedRootStartTime(TimeAccessor value)
{
    this->object->SetAnimatedRootStartTime(value.GetNativeTime());
}

void FinjinMeshAnimationSettingsAccessor::SetUseAnimationStartTime(bool value)
{
    this->object->SetUseAnimationStartTime(value);
}

void FinjinMeshAnimationSettingsAccessor::SetLooped(bool value)
{
    this->object->SetLooped(value);
}

void FinjinMeshAnimationSettingsAccessor::SetSampleInterval(double value)
{
    this->object->SetSampleInterval(value);
}

void FinjinMeshAnimationSettingsAccessor::SetSampleType(SampleType value)
{
    this->object->SetSampleType(value);
}

void FinjinMeshAnimationSettingsAccessor::SetTimeInterval(FinjinTimeInterval interval)
{
    Interval maxInterval;
    maxInterval.SetStart(interval.start.GetNativeTime());
    maxInterval.SetEnd(interval.end.GetNativeTime());
    this->object->SetTimeInterval(maxInterval);
}

void FinjinMeshAnimationSettingsAccessor::SetEmbedAnimation(bool value)
{
    this->object->SetEmbedAnimation(value);
}

void FinjinMeshAnimationSettingsAccessor::SetLinkToMainObject(bool value)
{
    this->object->SetLinkToMainObject(value);
}

void FinjinMeshAnimationSettingsAccessor::ClearBoneTranslationMasks()
{
    this->object->ClearBoneTranslationMasks();
}

void FinjinMeshAnimationSettingsAccessor::AddBoneTranslationMask(ObjectAccessor object, FinjinVector3 mask)
{
    this->object->AddBoneTranslationMask(object.node, Point3(mask.x, mask.y, mask.z));
}

void FinjinMeshAnimationSettingsAccessor::ClearAllowExportBones()
{
    this->object->ClearAllowExportBones();
}

void FinjinMeshAnimationSettingsAccessor::AddAllowExportBone(ObjectAccessor object, bool allow, bool recursive)
{
    this->object->AddAllowExportBone(object.node, allow, recursive);
}
