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
#include "MeshMorpherBase.hpp"
#include "AutoKeyState.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Extracts morph targets from the blend shapes. */
    class MeshMorpher : public MeshMorpherBase
    {
    public:
        MeshMorpher(GeometryStateBase* geometryState);
        ~MeshMorpher();
        
        void Clear();
        
        /** 
         * Initializes the morpher data.
         * @param object [in] - The object that may contain blend shapes.
         * @param coordinateConverter [in] - The coordinate converter used to convert points.
         * @param scale [in] - The scale applied to points.
         */
        bool Initialize
            (
            MObject object, 
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

    private:
        /** Attempts to find the blend shape defoermer. */
        bool FindMeshBlendShapeDeformer(MFnMesh& mesh);
        
        bool HasMorphTarget(ObjectAccessor target) const;

        void StartMorphTargets
            (
            std::vector<MDagModifier*>& weightConnectionDagModifiers,
            MIntArray& weightIndexList,
            std::vector<float>& originalWeights
            );
        
        void FinishMorphTargets
            (
            const std::vector<MDagModifier*>& weightConnectionDagModifiers,
            const MIntArray& indexList,
            const std::vector<float>& originalWeights            
            );

    public:
        /**
         * It's necessary to define the deformer in this way so that the entire MeshMorpher class
         * can be copied
         */
        std::shared_ptr<MFnBlendShapeDeformer> blendShapeDeformer;
        
    private:
        /** The original envelope value. Needed for restoring. */
        float originalEnvelope;

        /** The original auto key state. Needed for restoring. */
        AutoKeyState autoKeyState;
    };

} }
