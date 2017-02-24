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
#include "FinjinTimeInterval.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Extracts the animation clips from a Biped's mixer. */
    class BipedMixerAnimations
    {
    public:
        /** 
         * Gets all the animations from the mixer. 
         * @param object [in] - The object from which the animations are retrieved.
         * @return If there was at least one animation in the mixer, true is returned.
         * Otherwise, false is returned.
         */
        bool Initialize(ObjectAccessor object);
        
        /** Gets the number of animations read from the mixer. */
        size_t size() const {return this->animations.size();}

        /** A single animation clip. */
        struct Animation
        {
            wxString name;
            FinjinTimeInterval interval;
        };

        /** Gets the animation at the specified index. */
        Animation& operator [] (size_t index) {return this->animations[index];}

        /** Gets the animation at the specified index. */
        const Animation& operator [] (size_t index) const {return this->animations[index];}

    public:
        /** The animation clips extracted from the mixer. */
        std::vector<Animation> animations;

        bool bipedAttachedWithPhysique;
        Modifier* skinningModifier;
    };

} }
