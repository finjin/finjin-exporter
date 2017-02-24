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
#include "MaxModifier.hpp"
#include "MeshMorpherBase.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Extracts morph targets from the morpher modifier. */
    class MeshMorpher : public MeshMorpherBase
    {
    public:
        MeshMorpher(GeometryStateBase* geometryState);

        void Clear();

        /** 
         * Initializes the morpher data.
         * @param maxNode [in] - The object that may contain a morpher modifier.
         * @param coordinateConverter [in] - The coordinate converter used to convert points.
         * @param scale [in] - The scale applied to points.
         */
        bool Initialize
            (
            INode* maxNode, 
            const CoordinateSystemConverter& coordinateConverter, 
            float scale,
            PrimitiveType meshPrimitiveType
            );

        /** Determines whether there are any morph targets. */
        bool IsValid() const;

        /** 
         * Enables/disables the morpher. 
         * Enabling it means that it has an effect on the target mesh object.
         * Disabling it means that the target mesh data appears as though there is no morpher.
         */
        void Enable(bool enable = true);

        /** 
         * Disables the morpher. 
         * Disabling it means that the target mesh data appears as though there is no morpher.
         */
        void Disable() {Enable(false);}

        /** Returns the morpher to its original enabled/disabled state. */
        void Restore();

        /** Determines whether the specified object has morpher data. */
        static bool HasMorpher(ObjectAccessor object);

    public:
        /** The 3DS Max morpher modifier. */
        MaxModifier morpherModifier;
    };

} }
