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
#include "FinjinVector.hpp"
#include "MatrixAccessor.hpp"
#include "ObjectAccessor.hpp"
#include "TimeAccessor.hpp"
#include "MeshNormals.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class GeometryStateSubmesh;

    /**
     * A single target in a MeshMorpher.
     *
     * This is collects the data and methods common to all the application-specific 
     * MeshMorphTarget classes.
     */
    class MeshMorphTargetBase
    {
    public:
        virtual ~MeshMorphTargetBase() {}

        /** 
         * Gets the morph target's influence at the specified time. 
         * @param t [in] - The time at which to sample the target influence.
         * @return The influence, a value in [0, 1], is returned.
         */
        virtual float GetInfluence(TimeAccessor t) = 0;

        /**
         * Transforms all the morph target points using specified transformation matrix.
         * @param transformMatrix [in] - Transforms morph target points.
         * @param normalTransformMatrix [in] - Transforms morph target normals.
         */
        void Transform(const MatrixAccessor& transformMatrix, const MatrixAccessor& normalTransformMatrix)
        {
            //Transform creation points
            for (size_t pointIndex = 0; pointIndex < this->creationPositions.size(); pointIndex++)
                transformMatrix.TransformPoint(this->creationPositions[pointIndex]);
            
            //Transform subtarget points
            for (size_t subtargetIndex = 0; subtargetIndex < this->subtargets.size(); subtargetIndex++)
            {
                for (size_t offsetIndex = 0; offsetIndex < this->subtargets[subtargetIndex]->offsets.size(); offsetIndex++)
                {
                    this->subtargets[subtargetIndex]->offsets[offsetIndex].Transform(transformMatrix, normalTransformMatrix);                    
                }
            }
        }

    public:
        /** 
         * The morph target object. 
         * This may not be a valid object, since 3DS Max allows the object to be deleted. 
         */
        ObjectAccessor object;

        /** The name of the morph target. */
        wxString name;

        /** The morph target's object-space points. */
        std::vector<FinjinVector3> creationPositions;

        /** The morph target's object-space normals. */
        MeshNormals creationNormals;

        /** 
         * Contains all the information necessary to generate an offset for a morph target point. 
         * The reason two object space points are maintained - and not just a single offset - is that
         * keeping the object space points allows the points to be transformed later, such as when
         * meshes are merged.
         */
        struct Offset
        {
            Offset() {this->index = 0;}

            Offset
                (
                size_t index, 
                const FinjinVector3& targetPosition, const FinjinVector3& targetNormal, const FinjinVector4& targetTangent, 
                const FinjinVector3& meshPosition, const FinjinVector3& meshNormal, const FinjinVector4& meshTangent
                )
            {
                this->index = index;
                
                this->targetPosition = targetPosition;
                this->targetNormal = targetNormal;
                this->targetTangent = targetTangent;
                
                this->meshPosition = meshPosition;
                this->meshNormal = meshNormal;
                this->meshTangent = meshTangent;

                CalculateOffsets();
            }

            void Transform(const MatrixAccessor& transformMatrix, const MatrixAccessor& normalTransformMatrix)
            {
                transformMatrix.TransformPoint(this->targetPosition);
                normalTransformMatrix.TransformNormal(this->targetNormal);
                transformMatrix.TransformPoint(this->meshPosition);                    

                CalculateOffsets();                
            }
            
            /** Index of the affected vertex in the submesh. */
            size_t index;

            FinjinVector3 positionOffset;
            FinjinVector3 normalOffset;
            FinjinVector4 tangentOffset;

        private:
            void CalculateOffsets()
            {
                this->positionOffset = this->targetPosition - this->meshPosition;
                
                this->normalOffset = this->targetNormal - this->meshNormal;
                this->normalOffset.Normalize();

                FinjinVector3 tangentOffset3(this->targetTangent.x - this->meshTangent.x, this->targetTangent.y - this->meshTangent.y, this->targetTangent.z - this->meshTangent.z);
                tangentOffset3.Normalize();
                this->tangentOffset.Set(tangentOffset3.x, tangentOffset3.y, tangentOffset3.z, this->meshTangent.w); //Handedness in the original tangent takes precedence
            }

            //The morph targets
            FinjinVector3 targetPosition;
            FinjinVector3 targetNormal;
            FinjinVector4 targetTangent;

            //The original mesh components
            FinjinVector3 meshPosition;
            FinjinVector3 meshNormal;
            FinjinVector4 meshTangent;
        };

        /** A collection of offsets for a submesh. */
        struct Subtarget
        {      
            Subtarget()
            {
                this->submesh = nullptr;
                this->index = 0;
            }

            /** The submesh this subtarget corresponds to. */
            GeometryStateSubmesh* submesh;

            /** Index of the subtarget. Used during export only. */
            int index;

            /** The morpher offsets. */
            std::vector<Offset> offsets;
        };
        typedef std::shared_ptr<Subtarget> SubtargetPtr;
        std::vector<SubtargetPtr> subtargets;
    };

} }
