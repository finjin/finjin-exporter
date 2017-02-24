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
#include "MeshBone.hpp"
#include "PRSKey.hpp"
#include "WeightedVertex.hpp"
#include "SkeletonReferencePose.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    using namespace Finjin::Common;

    class GeometryStateBase;

    typedef std::shared_ptr<MeshBone> MeshBonePtr;
    typedef std::vector<MeshBonePtr> MeshBones;

    /**
     * Base class that collects the data and methods common to all the
     * application-specific MeshSkeleton classes. A skeleton is a construct that 
     * maintains a hierarchical relationship of bones that deform a mesh.
     */
    class MeshSkeletonBase
    {
    public:
        /**
         * Constructor.
         * @param geometryState [in] - The geometry that owns this mesh skeleton. This can be null
         * if the mesh skeleton is only being used to obtain the bones and/or auxiliary objects.
         */
        MeshSkeletonBase(GeometryStateBase* geometryState);

        /** Destructor. */
        virtual ~MeshSkeletonBase();

        /** Indicates whether the skeleton is valid. */
        virtual bool IsValid() const = 0;

        /** Clears all members. */
        void Clear();

        /** Changes bone parents as necessary according to scene/object settings. */
        void UpdateHierarchy();

        /**
         * Gets all the bone objects.
         * @param boneObjects [out] - A collection that receives bone objects.
         */
        void GetBones(ObjectAccessorSet& boneObjects);

        /** 
         * Gets the bone information for the specified bone.
         * @param object [in] - The object to be matched to bone information.
         * @return If the bone could be found, a pointer to the bone information is returned. Otherwise, null is returned.
         */
        MeshBonePtr FindBone(ObjectAccessor object);

        MeshBonePtr FindBone(MeshBoneBase* meshBone);
        
        int GetBoneIndex(MeshBoneBase* meshBone) const;

        int GetChildBoneCount(MeshBoneBase* meshBone) const;

        /** 
         * Gets the index of the specified bone. 
         * @param object [in] - The object to be matched to bone information.
         * @return If the bone could be found, the zero-based index is returned. Otherwise, -1 is returned.
         */
        int GetBoneIndex(ObjectAccessor object) const;

        MeshBone* GetBone(ObjectAccessor object);

        /** Determines whether this skeleton contains the specified object as a bone. */
        bool HasBone(ObjectAccessor object) const;

        /** Determines whether the specified object has an ancestor that is a bone in this skeleton. */
        bool HasBoneAncestor(ObjectAccessor object) const;

        /** Determines whether the specified object has a descendant that is a bone in this skeleton. */
        bool HasBoneDescendant(ObjectAccessor object) const;

        /**
         * Adds a bone to the internal collection.
         * @param bone [in] - The bone to add.
         */
        void AddBone(MeshBonePtr bone);

        /** Determines whether the specified bone influences a vertex. */
        bool BoneHasVertexInfluence(ObjectAccessor object) const;

        /** Removes bones that have no vertex assignments. */
        void RemoveBonesWithNoVertexInfluence();

        /** Call this when removing the specified bone. */
        void RemovingBone(MeshBone* bone);

        /** Gets the non-bone objects that are attached to the specified bone. */
        void GetBoneAttachedObjects(MeshBone* bone, ObjectAccessorVector& attachedObjects) const;

        /** Determines whether the skeleton has two matching weighted vertex assignments at the specified indices. */
        bool WeightedVerticesHasSameBones(int a, int b) const;
        
        /**
         * Merges the specified skeleton with this one.
         * @param mergeSkeleton [in] - The skeleton whose data is merged with this one. After the merge
         * is complete, Clear() is called on this parameter.
         */
        void Merge(MeshSkeletonBase& mergeSkeleton);

        /** Transforms the skeleton to world space. */
        void TransformToWorldSpace();

        /** 
         * Finishes the creation of the skeleton data. 
         * This is called by GeometryStateBase::FinishCreate().
         */
        void FinishCreate();

        /** Contains all the animation keys for a single animation for a bone. */
        struct BoneAnimation
        { 
            BoneAnimation()
            {
                this->bone = nullptr;
            }

            /** Gets the length of the animation in seconds. */
            WxTimeDuration GetDuration() const
            {
                auto length = WxTimeDuration::Zero();
                if (this->keys.size() > 0)
                    length = this->keys[this->keys.size() - 1].time - this->keys[0].time;
                return length;
            }

            MeshBone* bone;
            std::vector<PRSKey> keys;
        };
        typedef std::shared_ptr<BoneAnimation> BoneAnimationPtr;
        
        /** All the keys, for all bones, of a single animation. */
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
                for (size_t i = 0; i < this->boneAnimations.size(); i++)
                    length = std::max(length, this->boneAnimations[i]->GetDuration());                    
                return length;
            }

            /** The name of the aniamtion. */
            wxString name;

            /** Indicates whether this animation should embedded. */
            bool embedAnimation;

            /** Indicates whether this animation, if exported to a separate file, should be linked back to the main skeleton. */
            bool linkToMainObject;

            /** All animations for all bones. */
            std::vector<BoneAnimationPtr> boneAnimations;
        };
        typedef std::shared_ptr<Animation> AnimationPtr;

        /** All the animations for all bones. */    
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

    protected:
        /** 
         * Removes excess vertex bone assignments.
         * @param maxAssignments [in] - The maximum number of bones that can be assigned to a vertex.
         */
        void RemoveExcessAssignments(size_t maxAssignments = FINJIN_MAX_BLEND_WEIGHTS);
        
    public:
        /** The geometry that owns this mesh skeleton. This can be null. */
        GeometryStateBase* geometryState;

        /** Defines where the skeleton reference pose should come from. */
        SkeletonReferencePose referencePose;

        /** The initial skin transformation. */
        TransformAccessor initialSkinTransform;

        /** All the bones. */
        MeshBones bones;

        /** Control objects that are attached to bones but not considered part of the skeleton. */
        ObjectAccessorSet auxiliaryBoneObjects;

        /** The vertex bone weights. */
        std::vector<WeightedVertex> weightedVertices;
    };

} }
