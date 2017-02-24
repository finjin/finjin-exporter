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
#include "FinjinNodeAnimationSettingsAccessorBase.hpp"
#include "AnimationUtilities.hpp"
#include "FinjinObjectSettingsAccessorBase.hpp"
#include "FinjinSceneSettingsAccessorBase.hpp"
#include "FinjinGlobalSettings.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
double FinjinNodeAnimationSettingsAccessorBase::GetRealTimeScale()
{
    return AnimationUtilities::GetRealTimeScale(GetTimeScaleType(), GetTimeScale(), GetTimeInterval());    
}

void FinjinNodeAnimationSettingsAccessorBase::GetSampleTypeAndIntervalValues
    (
    SampleType& sampleType, 
    double& sampleInterval, 
    FinjinObjectSettingsAccessorBase& objectSettings,
    FinjinSceneSettingsAccessorBase& sceneSettings
    )
{
    sampleType = GetSampleType();
    sampleInterval = GetSampleInterval();
    if (sampleType == SampleType::USE_PARENT || sampleInterval == 0)
    {
        sampleType = objectSettings.GetNodeAnimationSampleType();
        sampleInterval = objectSettings.GetNodeAnimationSampleInterval();

        if (sampleType == SampleType::USE_PARENT || sampleInterval == 0)
        {
            sampleType = sceneSettings.GetNodeAnimationSampleType();
            sampleInterval = sceneSettings.GetNodeAnimationSampleInterval();

            if (sampleType == SampleType::USE_PARENT || sampleInterval == 0)
            {
                auto& globalSettings = FinjinGlobalSettings::GetInstance();
                sampleType = globalSettings.nodeAnimationSampleType;
                sampleInterval = globalSettings.nodeAnimationSampleInterval;
            }
        }        
    }
}

void FinjinNodeAnimationSettingsAccessorBase::CopyFrom(FinjinNodeAnimationSettingsAccessorBase& from)
{
    SetAnimationName(from.GetAnimationName());
    SetTimeScaleType(from.GetTimeScaleType());
    SetTimeScale(from.GetTimeScale());
    SetCopyFirstKeyToLast(from.GetCopyFirstKeyToLast());
    SetUseAnimationStartTime(from.GetUseAnimationStartTime());
    SetEmbedAnimation(GetEmbedAnimation());
    SetLinkToMainObject(GetLinkToMainObject());
    SetEnabled(from.GetEnabled());
    SetLooped(from.GetLooped());
    SetSampleInterval(from.GetSampleInterval());
    SetSampleType(from.GetSampleType());
    SetTimeInterval(from.GetTimeInterval());
    SetTranslationInterpolationType(from.GetTranslationInterpolationType());
    SetRotationInterpolationType(from.GetRotationInterpolationType());
}
