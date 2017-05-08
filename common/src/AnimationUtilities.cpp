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
#include "AnimationUtilities.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "Strings.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
void AnimationUtilities::GetSampledKeyTimes(std::vector<WxTimeDuration>& sampleTimes, FinjinTimeInterval timeInterval, double sampleInterval)
{
    //Calculate the number of sample intervals contained in the animation
    auto animationLength = timeInterval.GetDuration();
    int sampleIntervalCount = animationLength.ToNanoseconds() / WxTimeDuration::Seconds(sampleInterval, APP_TIME_RESOLUTION_UNIT).ToNanoseconds();
    auto roundedAnimationLength = (double)sampleIntervalCount * sampleInterval;
    if ((animationLength.ToSecondsDouble() - roundedAnimationLength) > .001)
    {
        //The sample interval does not divide the animation length evenly, so add an extra interval
        sampleIntervalCount++;

        //Recalculate the sample interval so that the samples are evenly spaced in time
        sampleInterval = (animationLength / WxTimeDuration::Nanoseconds(sampleIntervalCount)).ToSecondsDouble();
    }

    //Calculate the key times
    int sampleCount = sampleIntervalCount + 1;
    if (sampleCount <= 2)
    {
        sampleTimes.resize(2);
        sampleTimes[0] = timeInterval.start.GetDuration();
        sampleTimes[1] = timeInterval.end.GetDuration();
    }
    else
    {
        sampleTimes.resize(sampleCount);

        //First key time
        sampleTimes[0] = timeInterval.start.GetDuration();

        //Last key time
        sampleTimes[sampleCount - 1] = timeInterval.end.GetDuration();

        //In between key times
        for (int keyIndex = 1; keyIndex < sampleCount - 1; keyIndex++)
            sampleTimes[keyIndex] = WxTimeDuration::Seconds(timeInterval.start.GetSeconds() + sampleInterval * keyIndex, APP_TIME_RESOLUTION_UNIT);
    }
}

double AnimationUtilities::GetRealTimeScale(const wxString& scaleType, double scale, FinjinTimeInterval animationInterval)
{
    double animationScale = 1;

    auto animationIntervalSeconds = animationInterval.GetDuration().ToSecondsDouble();

    if (scaleType.empty() && scale == 0)
        animationScale = 1;
    else if (scaleType.empty() ||scaleType == wxT("scale"))
        animationScale = scale;
    else if (scaleType == PropertyValues::TimeScale::LENGTH)
    {
        if (scale > 0 && animationIntervalSeconds > 0)
            animationScale = scale / animationIntervalSeconds;
    }
    else if (scaleType == PropertyValues::TimeScale::NORMALIZE)
    {
        if (animationIntervalSeconds > 0)
            animationScale = 1 / animationIntervalSeconds;
    }

    if (animationScale <= 0)
        animationScale = 1;

    return animationScale;
}

wxString AnimationUtilities::FormatSampling(SampleType sampleType, double sampleInterval)
{
    wxString sampling;
    if (sampleInterval != 0)
    {
        if (sampleType == SampleType::INTERVAL)
            sampling = wxString::Format(wxT("%.2f %s/%s"), sampleInterval, Strings::SECOND_CHARACTER_LOWER.wx_str(), Strings::SAMPLE_LOWER.wx_str());
        else if (sampleType == SampleType::RATE)
            sampling = wxString::Format(wxT("%.2f %s/%s"), 1 / sampleInterval, Strings::SAMPLES_LOWER.wx_str(), Strings::SECOND_CHARACTER_LOWER.wx_str());
    }
    return sampling;
}
