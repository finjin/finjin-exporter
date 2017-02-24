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
#include "MeshAnimationTracksDetectorBase.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
int MeshAnimationTracksDetectorBase::FindTrackIndex(const wxString& name) const
{
    int index = 0; //The default, 0, will always be "object"
    for (size_t i = 0; i < this->tracks.size(); i++)
    {
        if (this->tracks[i].name == name)
        {
            index = (int)i;
            break;
        }
    }
    return index;
}

MeshAnimationTrack* MeshAnimationTracksDetectorBase::FindTrack(TrackType trackType)
{
    for (size_t i = 0; i < this->tracks.size(); i++)
    {
        if (this->tracks[i].location == trackType)
            return &this->tracks[i];
    }
    
    //The default will always be "object"
    return &this->tracks[0];
}

MeshAnimationTrack* MeshAnimationTracksDetectorBase::FindTrack(const wxString& name)
{
    for (size_t i = 0; i < this->tracks.size(); i++)
    {
        if (this->tracks[i].name == name)
            return &this->tracks[i];
    }

    //The default will always be "object"
    return &this->tracks[0];
}

void MeshAnimationTracksDetectorBase::AddTrack(TrackType trackType, const wxString& name, const wxString& displayName)
{
    MeshAnimationTrack track;
    track.location = trackType;
    track.name = name;
    track.displayName = displayName;
    this->tracks.push_back(track);
}
