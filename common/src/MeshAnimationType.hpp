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


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    enum class MeshAnimationTypes
    {
        /** The "default" type is only used by dialogs and is never detected as an actual type */
        DEFAULT,

        POSE_ONLY,
        MORPH_ONLY,
        COMBINED_MORPH_ONLY,
        SKELETON_ONLY,
        SKELETON_AND_POSE,
        SKELETON_AND_MORPH,
        
        COUNT
    };

    struct MeshAnimationType
    {
        MeshAnimationType() {this->type = MeshAnimationTypes::DEFAULT;}

        bool UsesObjectTrack() const {return this->type == MeshAnimationTypes::MORPH_ONLY || this->type == MeshAnimationTypes::COMBINED_MORPH_ONLY || this->type == MeshAnimationTypes::SKELETON_AND_MORPH;}
        bool UsesMorpherTrack() const {return this->type == MeshAnimationTypes::POSE_ONLY || this->type == MeshAnimationTypes::SKELETON_AND_POSE;}
        bool UsesSkeletonTrack() const {return this->type == MeshAnimationTypes::SKELETON_ONLY || this->type == MeshAnimationTypes::SKELETON_AND_POSE || this->type == MeshAnimationTypes::SKELETON_AND_MORPH;}

        bool GeneratesMorphsExclusively() const {return this->type == MeshAnimationTypes::MORPH_ONLY || this->type == MeshAnimationTypes::COMBINED_MORPH_ONLY;}
        bool GeneratesCombinedMorphs() const {return this->type == MeshAnimationTypes::COMBINED_MORPH_ONLY;}
        bool GeneratesMorphs() const {return UsesObjectTrack();}
        bool GeneratesPoses() const {return UsesMorpherTrack();}        
        bool GeneratesSkeleton() const {return UsesSkeletonTrack();}

        MeshAnimationTypes type;
        wxString name;
        wxString displayName;        
    };

} }
