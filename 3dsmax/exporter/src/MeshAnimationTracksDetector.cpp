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
#include "MeshAnimationTracksDetector.hpp"
#include "MaxUtilities.hpp"
#include "PropertyValues.hpp"
#include "Strings.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
void MeshAnimationTracksDetector::Detect(ObjectAccessor object)
{
    struct TrackInfo
    {
        bool isValid;
        wxString name;
        wxString displayName;
    };

    //Get all the usable track locations
    auto skinModifier = MaxUtilities::FindSkinModifier(object.node);
    auto physiqueModifier = MaxUtilities::FindPhysiqueModifier(object.node);
    auto morpherModifier = MaxUtilities::FindMorpherModifier(object.node);    
    TrackInfo tracks[TRACK_TYPE_COUNT] = 
        {
        {true, wxEmptyString, Strings::OBJECT},
        {morpherModifier != nullptr, PropertyValues::MeshAnimationTrack::MORPHER, Strings::MORPHER},
        {physiqueModifier != nullptr, PropertyValues::MeshAnimationTrack::PHYSIQUE, Strings::PHYSIQUE},
        {skinModifier != nullptr, PropertyValues::MeshAnimationTrack::SKIN, Strings::SKIN}
        };
    for (TrackType i = OBJECT; i < TRACK_TYPE_COUNT; i++)
    {
        if (tracks[i].isValid)
            AddTrack(i, tracks[i].name, tracks[i].displayName);
    }
}

const wxString& MeshAnimationTracksDetector::GetTrackName(TrackType trackType)
{
    static const wxString names[] =
        {
        wxEmptyString,
        PropertyValues::MeshAnimationTrack::MORPHER,
        PropertyValues::MeshAnimationTrack::PHYSIQUE,
        PropertyValues::MeshAnimationTrack::SKIN
        };
    return names[trackType];
}

bool MeshAnimationTracksDetector::IsObjectBased(TrackType trackType)
{
    return trackType == OBJECT;
}

bool MeshAnimationTracksDetector::IsSkeletonBased(TrackType trackType)
{
    return trackType == PHYSIQUE || trackType == SKIN;
}

bool MeshAnimationTracksDetector::IsMorpherBased(TrackType trackType)
{
    return trackType == MORPHER;
}

bool MeshAnimationTracksDetector::IsObjectBased(const wxString& trackType)
{
    return trackType.empty();
}

bool MeshAnimationTracksDetector::IsSkeletonBased(const wxString& trackType)
{
    return 
        trackType == PropertyValues::MeshAnimationTrack::SKIN || 
        trackType == PropertyValues::MeshAnimationTrack::PHYSIQUE;
}

bool MeshAnimationTracksDetector::IsMorpherBased(const wxString& trackType)
{
    return trackType == PropertyValues::MeshAnimationTrack::MORPHER;
}
