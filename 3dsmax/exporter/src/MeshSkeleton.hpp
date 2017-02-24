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
#include "MeshSkeletonBase.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class MeshSkeleton : public MeshSkeletonBase
    {
    public:
        MeshSkeleton(GeometryStateBase* geometryState);

        void Clear();

        /** 
         * Initializes the skeleton data.
         * @param maxNode [in] - The object that may contain a Skin or Physique modifier.
         * @param coordinateConverter [in] - The coordinate converter used to convert transformations.
         * @param scale [in] - The scale applied to transformations.
         */
        bool Initialize
            (
            INode* maxNode, 
            const CoordinateSystemConverter& coordinateConverter, 
            float scale,
            const SkeletonReferencePose* referencePose
            );

        /** Determines whether there are skeleton bones. */
        bool IsValid() const override;

        /** 
         * Enables/disables the skeleton. 
         * Enabling it means that it has an effect on the target mesh object.
         * Disabling it means that the target mesh data appears as though there is no skeleton.
         */
        void Enable(bool enable);

        /** 
         * Disables the skeleton. 
         * Disabling it means that the target mesh data appears as though there is no skeleton.
         */
        void Disable() {Enable(false);}

        /** Returns the skeleton to its original enabled/disabled state. */
        void Restore();

        /** Determines whether the specified object has skeleton data. */
        static bool HasSkeleton(ObjectAccessor object);
        
    private:
        bool InitializeBone(MeshBone* bone, const CoordinateSystemConverter& coordinateConverter, float scale);        

    public:
        MaxModifier physiqueModifier;
        MaxModifier skinModifier;        
        ObjectAccessor meshObject;
    };

} }
