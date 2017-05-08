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
#include "FinjinMeshAnimationSettingsAccessor.hpp"
#include "FinjinNodeAnimationSettingsAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * This class provides simplified and uniform access to animation settings managed
     * by FinjinMeshAnimationSettingsAccessor and FinjinNodeAnimationSettingsAccessor.
     */
    class AnimationExportSettings
    {
    public:
        /**
         * Constructor.
         * @param settings [in] - The settings accessor.
         * @param objectSampleInterval [in] - The object-level default sample interval.
         * @param sceneSampleInterval [in] - The scene-level default sample interval.
         * @param globalSampleInterval [in] - The global-level default sample interval.
         */
        template <class T>
        AnimationExportSettings(T settings, double objectSampleInterval, double sceneSampleInterval, double globalSampleInterval)
        {
            this->name = settings.GetAnimationName();
            this->timeInterval = settings.GetTimeInterval();
            this->timeScale = settings.GetRealTimeScale();
            this->embedAnimation = settings.GetEmbedAnimation();
            this->linkToMainObject = settings.GetLinkToMainObject();
            this->copyFirstKeyToLast = settings.GetCopyFirstKeyToLast();
            this->sampleInterval = settings.GetSampleInterval();
            if (this->sampleInterval == 0)
            {
                this->sampleInterval = objectSampleInterval;
                if (this->sampleInterval == 0)
                {
                    this->sampleInterval = sceneSampleInterval;
                    if (this->sampleInterval == 0)
                    {
                        this->sampleInterval = globalSampleInterval;
                        if (this->sampleInterval == 0)
                            this->sampleInterval = .25f;
                    }
                }
            }
            this->useStartTime = settings.GetUseAnimationStartTime();
        }

        /**
         * Gets the sample time at specified index.
         * @param sampleTimes [in] - Collection of sample times.
         * @param index [in] - The index of the sample being retrieved.
         * @param The appropriate sample time is returned.
         */
        TimeAccessor GetSampleTime(const std::vector<WxTimeDuration>& sampleTimes, size_t index) const
        {
            TimeAccessor t;
            if (this->copyFirstKeyToLast && index == sampleTimes.size() - 1)
                t.SetSeconds(sampleTimes[0].ToSecondsDouble());
            else
                t.SetSeconds(sampleTimes[index].ToSecondsDouble());
            return t;
        }

        /**
         * Gets the output key time at specified index.
         * @param sampleTimes [in] - Collection of sample times.
         * @param index [in] - The index of the output key time being retrieved.
         * @param The appropriate output key time is returned.
         */
        WxTimeDuration GetOutputKeyTime(const std::vector<WxTimeDuration>& sampleTimes, size_t index) const
        {
            auto t = (sampleTimes[index] - sampleTimes[0]).ToSecondsDouble() * this->timeScale;
            if (this->useStartTime)
            {
                //Use the original start time
                t += sampleTimes[0].ToSecondsDouble();
            }
            return WxTimeDuration::Seconds(t, APP_TIME_RESOLUTION_UNIT);
        }

    public:
        /** The name of the animation. */
        wxString name;

        /** The time interval over which the animation is sampled. */
        FinjinTimeInterval timeInterval;

        /** Time scaling for output key times. */
        double timeScale;

        bool embedAnimation;
        bool linkToMainObject;

        /** Indicates whether the first sample key is copied to the last. */
        bool copyFirstKeyToLast;

        /** The amount of time between samples. */
        double sampleInterval;

        /** Indicates whether output key times should use the real start time. */
        bool useStartTime;
    };

} }
