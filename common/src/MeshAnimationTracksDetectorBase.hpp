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
#include "ObjectAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** 
     * This class detects the available mesh animation 'tracks' for a particular object.
     * This abstraction comes from the way 3DS Max displays objects and their modifiers in the track view:
     *    -There's the top level object, which this class considers the 'object'.
     *    -If a Skin modifier is applied, there's a 'skin' track.
     *    -If a Morpher modifier is applied, there's a 'morpher' track.
     * This class adopts a similar scheme, where the application-defined MeshAnimationTracksDetector
     * class is free to use whatever names are appropriate for the application.
     *
     * Each mesh animation setting has a track to which it is bound, which allows mesh animations
     * to be generated on export.
     *
     * When using the Finjin exporter, you can find the tracks in the mesh animation dialog
     * for a particular object.
     */
    class MeshAnimationTracksDetectorBase
    {
    public:
        /** The track type. */
        typedef int TrackType;

        virtual ~MeshAnimationTracksDetectorBase() {}

        /** Detects the available mesh animation tracks for the specified object. */
        virtual void Detect(ObjectAccessor object) = 0;

        /** Finds the index of the track with the specified internal name. */
        int FindTrackIndex(const wxString& name) const;

        /** 
         * Finds the track with the specified type. 
         * @return Returns a pointer to the track of the specified type. If such a track 
         * cannot be found, the first track is returned, which is usually the 'object' track.
         */
        MeshAnimationTrack* FindTrack(TrackType trackType);

        /** 
         * Finds the track with the specified internal name. 
         * @return Returns a pointer to the with the specified internal name. If such a track 
         * cannot be found, the first track is returned, which is usually the 'object' track.
         */
        MeshAnimationTrack* FindTrack(const wxString& name);

        /** 
         * Adds a track.
         * @param trackType [in] - The application-defined track type.
         * @param name [in] - The internal name of the track.
         * @param displayName [in] - The display name of the track.
         */
        void AddTrack(TrackType trackType, const wxString& name, const wxString& displayName);

    public:
        /** The selected track */
        MeshAnimationTrack* selectedTrack;

        /** All the tracks that are allowed, including the selected track */
        std::vector<MeshAnimationTrack> tracks;
    };

} }
