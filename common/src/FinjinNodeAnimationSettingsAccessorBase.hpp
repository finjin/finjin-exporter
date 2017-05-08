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
#include "FinjinTimeInterval.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class FinjinObjectSettingsAccessorBase;
    class FinjinSceneSettingsAccessorBase;

    /**
     * FinjinNodeAnimationSettingsAccessorBase and the application-specific FinjinNodeAnimationSettingsAccessor
     * classes provide generic access to an Finjin node animation settings object.
     */
    class FinjinNodeAnimationSettingsAccessorBase
    {
    public:
        virtual ~FinjinNodeAnimationSettingsAccessorBase() {}

        virtual wxString GetAnimationName() = 0;
        virtual wxString GetTimeScaleType() = 0;
        virtual double GetTimeScale() = 0;
        virtual bool GetCopyFirstKeyToLast() = 0;
        virtual bool GetUseAnimationStartTime() = 0;
        virtual bool GetEmbedAnimation() = 0;
        virtual bool GetLinkToMainObject() = 0;
        virtual bool GetEnabled() = 0;
        virtual bool GetLooped() = 0;
        virtual double GetSampleInterval() = 0;
        virtual SampleType GetSampleType() = 0;
        virtual FinjinTimeInterval GetTimeInterval() = 0;
        virtual wxString GetTranslationInterpolationType() = 0;
        virtual wxString GetRotationInterpolationType() = 0;

        virtual void SetAnimationName(wxString value) = 0;
        virtual void SetTimeScaleType(wxString value) = 0;
        virtual void SetTimeScale(double value) = 0;
        virtual void SetCopyFirstKeyToLast(bool value) = 0;
        virtual void SetUseAnimationStartTime(bool value) = 0;
        virtual void SetEmbedAnimation(bool value) = 0;
        virtual void SetLinkToMainObject(bool value) = 0;
        virtual void SetEnabled(bool value) = 0;
        virtual void SetLooped(bool value) = 0;
        virtual void SetSampleInterval(double value) = 0;
        virtual void SetSampleType(SampleType value) = 0;
        virtual void SetTimeInterval(FinjinTimeInterval interval) = 0;
        virtual void SetTranslationInterpolationType(wxString value) = 0;
        virtual void SetRotationInterpolationType(wxString value) = 0;

        /** Gets the time scale factor. */
        double GetRealTimeScale();

        /**
         * Gets the animation's sample type and sample interval, taking into account the object settings
         * and global settings if necessary.
         * @param sampleType [out] - The retrieved sample type.
         * @param sampleInterval [out] - The retrieved sample interval.
         * @param objectSettings [in] - The object settings.
         * @param sceneSettings [in] - The scene settings.
         */
        void GetSampleTypeAndIntervalValues
            (
            SampleType& sampleType,
            double& sampleInterval,
            FinjinObjectSettingsAccessorBase& objectSettings,
            FinjinSceneSettingsAccessorBase& sceneSettings
            );

        /**
         * Copies all the settings from the specified source to this animation.
         */
        virtual void CopyFrom(FinjinNodeAnimationSettingsAccessorBase& from);
    };

} }
