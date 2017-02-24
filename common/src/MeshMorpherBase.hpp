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
#include "MeshMorphTarget.hpp"
#include "MatrixAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    using namespace Finjin::Common;

    class GeometryStateBase;
    class GeometryStateSubmesh;

    typedef std::shared_ptr<MeshMorphTarget> MeshMorphTargetPtr;
    typedef std::list<MeshMorphTargetPtr> MeshMorphTargets;

    /**
     * Base class that collects the data and methods common to all the
     * application-specific MeshMorpher classes. A morpher is a construct that 
     * morphs/blends a mesh among many different meshes using blending factors. The name 
     * 'morpher' is adopted from the 3DS Max Morpher modifier. Maya and XSI have similar
     * functionality.
     */
    class MeshMorpherBase
    {
    public:
        /**
         * Constructor.
         * @param geometryState [in] - The geometry that owns this mesh morpher. This can be null
         * if the mesh morpher is only being used to obtain the references to the morph targets.
         */
        MeshMorpherBase(GeometryStateBase* geometryState);

        /** Destructor. */
        virtual ~MeshMorpherBase();

        /** Indicates whether the morpher is valid. */
        virtual bool IsValid() const = 0;

        /** Clears all members. */
        void Clear();

        /**
         * Gets the names of all morph targets.
         * @param names [out] - A collection that receives the morph target names.
         */
        void GetMorphTargetNames(std::set<wxString>& names);

        /**
         * Adds a morph target to the internal collection.
         * @param morphTarget [in] - The morph target to add.
         */
        void AddMorphTarget(MeshMorphTargetPtr morphTarget);

        /**
         * Merges the specified morpher with this one.
         * @param mergeMorpher [in] - The morpher whose data is merged with this one. After the merge
         * is complete, Clear() is called on this parameter.
         * @param transformMatrix [in] - The transformation matrix used to offset the morph target points.
         * @param normalTransformMatrix [in] - The transformation matrix used to offset the morph target normals.
         */
        void Merge(MeshMorpherBase& mergeMorpher, const MatrixAccessor& transformMatrix, const MatrixAccessor& normalTransformMatrix);

        /** 
         * Finishes the creation of the morpher data. 
         * This is called by GeometryStateBase::FinishCreate().
         */
        void FinishCreate();

        /**
         * Transforms all the morph target points using specified transformation matrix.
         * @param transformMatrix [in] - Transforms morph target points.
         * @param normalTransformMatrix [in] - Transforms morph target normals.
         */
        void TransformVertices(const MatrixAccessor& transformMatrix, const MatrixAccessor& normalTransformMatrix);

        /**
         * In the 3D application, whole meshes make up the morph target influence. In Finjin,
         * meshes are segmented into submeshes. Accordingly, morph targets are segmented into
         * subtargets along with their influences. This struct manages one such target and its influence.
         */
        struct MorphSubtargetInfluence
        {
            MorphSubtargetInfluence() 
            {
                this->subtarget = nullptr; 
                this->influence = 0;
            }

            MorphSubtargetInfluence(MeshMorphTarget::Subtarget* subtarget, float influence)
            {
                this->subtarget = subtarget;
                this->influence = influence;
            }

            /** The morph subtarget. */
            MeshMorphTarget::Subtarget* subtarget;

            /** The influence, from 0 to 1. */
            float influence;
        };
        typedef std::vector<MorphSubtargetInfluence> MorphSubtargetInfluences;

        /** Contains all the influences in a single animation key for a submesh. */
        struct Key
        {
            Key()
            {
                this->time = WxTimeDuration::Zero();
            }

            WxTimeDuration time;
            MorphSubtargetInfluences influences;
        };

        /** Contains all the animation keys for a single animation for a submesh. */
        struct Subanimation
        {
            Subanimation()
            {
                this->submesh = nullptr;
            }

            /** Gets the length of the animation in seconds. */
            WxTimeDuration GetDuration() const
            {
                auto length = WxTimeDuration::Zero();
                if (this->keys.size() > 1)
                    length = this->keys[this->keys.size() - 1].time - this->keys[0].time;
                return length;
            }

            /** The submesh that the keys operate on. */
            GeometryStateSubmesh* submesh;

            /** All the keys for the submesh. */
            std::vector<Key> keys;
        };
        typedef std::shared_ptr<Subanimation> SubanimationPtr;
        
        /** All the keys, for all submeshes, of a single animation. */
        struct Animation
        {
            Animation()
            {
                this->embedAnimation = false;
                this->linkToMainObject = false;
            }

            /** Gets the length of the animation in seconds. */
            WxTimeDuration GetDuration() const
            {
                auto length = WxTimeDuration::Zero();
                for (size_t i = 0; i < this->subanimations.size(); i++)
                    length = std::max(length, this->subanimations[i]->GetDuration());                    
                return length;
            }

            /** The name of the aniamtion. */
            wxString name;

            bool embedAnimation;
            bool linkToMainObject;

            /** All animations for all submeshes. */
            std::vector<SubanimationPtr> subanimations;
        };
        typedef std::shared_ptr<Animation> AnimationPtr;

        /** A single morph target influence. */
        struct MorphTargetInfluence
        {
            MorphTargetInfluence() 
            {
                this->target = nullptr; 
                this->influence = 0;
            }

            /** The morph target. */
            MeshMorphTarget* target;

            /** The influence, from 0 to 1. */
            float influence;            
        };
        typedef std::vector<MorphTargetInfluence> MorphTargetInfluences;
         
        /** 
         * Samples the morph target influences at the specified time.
         * @param targetInfluences [out] - Receives all the sampled morph targets.
         * @param sampleTime [in] - The time at which the sampling occurs.
         * @param forceAllTargets [in] - Indicates whether all morph targets should be placed into
         * the output targetInfluences collection. If false, only morph targets with non-zero influences
         * will be returned.
         */
        void SampleMorphTargetInfluences
            (
            MorphTargetInfluences& targetInfluences, 
            TimeAccessor sampleTime, 
            bool forceAllTargets = false
            );

        /** All the animations for all submeshes. */        
        struct Animations
        {
            bool empty() const {return this->animations.empty();}
            void clear() {this->animations.clear();}
            size_t size() const {return this->animations.size();}
            void resize(size_t newSize) {this->animations.resize(newSize);}
            AnimationPtr& operator [] (size_t i) {return this->animations[i];}
            const AnimationPtr& operator [] (size_t i) const {return this->animations[i];}

            /** Determines whether the other collection of animations contains the same animations as this one. */
            bool operator == (const Animations& other) const
            {
                if (size() != other.size())
                    return false;

                for (size_t i = 0; i < other.size(); i++)
                {
                    if (!HasAnimation(other[i]->name))
                        return false;
                }

                return true;
            }

            /** Determines whether the other collection of animations contains different animations as this one. */
            bool operator != (const Animations& other) const
            {
                return !(operator == (other));
            }

            /** Determines whether an animation with the specified name exists. */
            bool HasAnimation(const wxString& name) const
            {
                for (size_t i = 0; i < this->animations.size(); i++)
                {
                    if (this->animations[i]->name == name)
                        return true;
                }
                return false;
            }

            std::vector<AnimationPtr> animations;
        };

    public:
        /** The geometry that owns this mesh morpher. This can be null. */
        GeometryStateBase* geometryState;

        /** All the morph targets. */
        MeshMorphTargets morphTargets;        
    };

} }
