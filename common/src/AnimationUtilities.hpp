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
#include "FinjinMeshAnimationSettingsAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    using namespace Finjin::Common;

    /**
     * Utility functions for animation data.
     */
    class AnimationUtilities
    {
    public:
        /**
         * Generates time values between the specified interval.
         * @param sampleTimes [out] - The sampled times.
         * @param timeInterval [in] - The start/end time over which times are sampled.
         * @param sampleInterval [in] - The amount of time, in seconds, between samples. This value
         * will be adjusted so that all the time values are evenly spaced.
         */
        static void GetSampledKeyTimes(std::vector<WxTimeDuration>& sampleTimes, FinjinTimeInterval timeInterval, double sampleInterval);

        /**
         * Gets the time scale associated for the specified parameters.
         * @param scaleType [in] - The type of time scaling. Will be one of PropertyValues::TimeScale,
         * or an empty string. This value typically comes directly from a mesh or node animation settings object.
         * @param scale [in] - An additional scaling value. The way it is interpreted depends on the scaleType.
         * @param animationInterval [in] - A time interval in consideration. Depending on scaleType, this may
         * not be used.
         * @return A time scale is returned.
         */
        static double GetRealTimeScale(const wxString& scaleType, double scale, FinjinTimeInterval animationInterval);

        /**
         * Formats the sample interval according to the specified sample type.
         * @param sampleType [in] - The format of the formatted sampling.
         * @param sampleInterval [in] - The sample interval, in seconds.
         * @return A string formatted with the sample interval is returned. As an example,
         * something like "3 samples/s" or ".3 s/sample" will be returned.
         */
        static wxString FormatSampling(SampleType sampleType, double sampleInterval);

        /**
         * Determines if the specified collection has a particular animation.
         * @param animations [in] - A collection of animations.
         * @param animation [in] - An animation to look for.
         * @return If the animation could be found, true is returned. Otherwise, false is returned.
         */
        template <class T>
        static bool HasAnimation(const std::vector<T>& animations, const T& animation)
        {
            auto animationName = const_cast<T&>(animation).GetAnimationName();

            for (size_t i = 0; i < animations.size(); i++)
            {
                auto& a = const_cast<T&>(animations[i]);
                if (a.GetAnimationName() != animationName ||
                    a.GetTimeInterval() != const_cast<T&>(animation).GetTimeInterval())
                {
                    return false;
                }
            }

            return true;
        }

        /** Determines if the specified collections contain the same animations. */
        template <class T>
        static bool AnimationsSame(const std::vector<T>& a, const std::vector<T>& b)
        {
            if (a.size() != b.size())
                return false;

            for (size_t i = 0; i < a.size(); i++)
            {
                if (!HasAnimation(b, a[i]))
                    return false;
            }

            return true;
        }
    };

} }
