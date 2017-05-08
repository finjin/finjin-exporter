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
#include "FinjinMeshAnimationSettingsAccessorBase.hpp"
#include "AnimationUtilities.hpp"
#include "FinjinObjectSettingsAccessorBase.hpp"
#include "FinjinSceneSettingsAccessorBase.hpp"
#include "FinjinGlobalSettings.hpp"
#include "MeshAnimationTracksDetector.hpp"
#include "MeshAnimationType.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
double FinjinMeshAnimationSettingsAccessorBase::GetRealTimeScale()
{
    return AnimationUtilities::GetRealTimeScale(GetTimeScaleType(), GetTimeScale(), GetTimeInterval());
}

bool FinjinMeshAnimationSettingsAccessorBase::GetGenerateCombinedMorphsValue(bool objectValue)
{
    return FromInheritedBool(GetMorphWholeObject(), objectValue);
}

bool FinjinMeshAnimationSettingsAccessorBase::IsBoneAllowed(ObjectAccessor bone)
{
    auto isBoneAllowed = true;

    ObjectAccessor object;
    bool allow;
    bool recursive;

    int count = GetAllowExportBoneCount();
    for (int i = 0; i < count; i++)
    {
        GetAllowExportBone(i, object, allow, recursive);

        if (recursive)
        {
            if (object == bone || object.HasDescendant(bone))
                isBoneAllowed = allow;
        }
        else
        {
            if (object == bone)
                isBoneAllowed = allow;
        }
    }

    return isBoneAllowed;
}

void FinjinMeshAnimationSettingsAccessorBase::GetSampleTypeAndIntervalValues
    (
    SampleType& sampleType,
    double& sampleInterval,
    FinjinObjectSettingsAccessorBase& objectSettings,
    FinjinSceneSettingsAccessorBase& sceneSettings,
    MeshAnimationType& meshAnimationType,
    MeshAnimationTracksDetectorBase& tracksDetector
    )
{
    auto& globalSettings = FinjinGlobalSettings::GetInstance();

    sampleType = GetSampleType();
    sampleInterval = GetSampleInterval();
    if (sampleType == SampleType::USE_PARENT || sampleInterval == 0)
    {
        auto track = tracksDetector.FindTrack(GetTrack());
        if (MeshAnimationTracksDetector::IsSkeletonBased(track->location) && meshAnimationType.GeneratesSkeleton() && !GetExportAsMorph())
        {
            sampleType = objectSettings.GetSkeletonAnimationSampleType();
            sampleInterval = objectSettings.GetSkeletonAnimationSampleInterval();

            if (sampleType == SampleType::USE_PARENT || sampleInterval == 0)
            {
                sampleType = sceneSettings.GetSkeletonAnimationSampleType();
                sampleInterval = sceneSettings.GetSkeletonAnimationSampleInterval();

                if (sampleType == SampleType::USE_PARENT || sampleInterval == 0)
                {
                    sampleType = globalSettings.skeletonAnimationSampleType;
                    sampleInterval = globalSettings.skeletonAnimationSampleInterval;
                }
            }
        }
        else
        {
            sampleType = objectSettings.GetVertexAnimationSampleType();
            sampleInterval = objectSettings.GetVertexAnimationSampleInterval();

            if (sampleType == SampleType::USE_PARENT || sampleInterval == 0)
            {
                sampleType = sceneSettings.GetVertexAnimationSampleType();
                sampleInterval = sceneSettings.GetVertexAnimationSampleInterval();

                if (sampleType == SampleType::USE_PARENT || sampleInterval == 0)
                {
                    sampleType = globalSettings.vertexAnimationSampleType;
                    sampleInterval = globalSettings.vertexAnimationSampleInterval;
                }
            }
        }
    }
}

void FinjinMeshAnimationSettingsAccessorBase::CopyFrom(FinjinMeshAnimationSettingsAccessorBase& from)
{
    SetTrack(from.GetTrack());
    SetAnimationName(from.GetAnimationName());
    SetTimeScaleType(from.GetTimeScaleType());
    SetTimeScale(from.GetTimeScale());
    SetCopyFirstKeyToLast(from.GetCopyFirstKeyToLast());
    SetExportAsMorph(from.GetExportAsMorph());
    SetMorphWholeObject(from.GetMorphWholeObject());
    SetUseAnimationStartTime(from.GetUseAnimationStartTime());
    SetLooped(from.GetLooped());
    SetSampleInterval(from.GetSampleInterval());
    SetSampleType(from.GetSampleType());
    SetTimeInterval(from.GetTimeInterval());
    SetEmbedAnimation(from.GetEmbedAnimation());
    SetLinkToMainObject(from.GetLinkToMainObject());

    ClearAllowExportBones();
    int allowExportBoneCount = from.GetAllowExportBoneCount();
    for (int i = 0; i < allowExportBoneCount; i++)
    {
        ObjectAccessor object;
        bool allow;
        bool recursive;
        from.GetAllowExportBone(i, object, allow, recursive);
        AddAllowExportBone(object, allow, recursive);
    }
}
