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


//Includes---------------------------------------------------------------------
#include "MeshAnimationTracksDetectorBase.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class MeshAnimationTracksDetector : public MeshAnimationTracksDetectorBase
    {   
    public:
        void Detect(ObjectAccessor object);

        /** Track types. */
        enum 
        {
            OBJECT,
            MORPHER,
            SKELETON,
            TRACK_TYPE_COUNT
        };

        /** Gets the internal track name for the specified track type. */
        static const wxString& GetTrackName(TrackType trackType);

        /** Determines whether the specified track results in an object-based animation. */
        static bool IsObjectBased(TrackType trackType);

        /** Determines whether the specified track results in an skeleton-based animation. */
        static bool IsSkeletonBased(TrackType trackType);

        /** Determines whether the specified track results in an morpher-based animation. */
        static bool IsMorpherBased(TrackType trackType);

        /** Determines whether the specified track results in an object-based animation. */
        static bool IsObjectBased(const wxString& trackType);

        /** Determines whether the specified track results in an skeleton-based animation. */
        static bool IsSkeletonBased(const wxString& trackType);

        /** Determines whether the specified track results in an morpher-based animation. */
        static bool IsMorpherBased(const wxString& trackType);
    };

} }
