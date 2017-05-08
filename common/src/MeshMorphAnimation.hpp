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


//Includes----------------------------------------------------------------------


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    using namespace Finjin::Common;

    /**
     * Contains a single morph animation.
     * A morph animation is composed of a sequence of keys, which are snapshots of a mesh.
     */
    struct MeshMorphAnimation
    {
        MeshMorphAnimation()
        {
            this->embedAnimation = false;
            this->linkToMainObject = false;
        }

        WxTimeDuration GetDuration() const
        {
            auto length = WxTimeDuration::Zero();
            for (size_t i = 0; i < this->subanimations.size(); i++)
                length = std::max(length, this->subanimations[i]->GetDuration());
            return length;
        }

        /** A single animation key. */
        struct Key
        {
            Key()
            {
                this->time = WxTimeDuration::Zero();
            }

            WxTimeDuration time;
            std::vector<SampledMeshPoint> points;
        };

        /**
         * Since meshes are eventually broken down into submeshes, its necessary
         * to also break down the morph mesh snapshot into submeshes.
         * Each morph animation will have as many subanimations are there are submeshes in the original mesh.
         */
        struct Subanimation
        {
            Subanimation()
            {
                this->submesh = nullptr;
            }

            WxTimeDuration GetDuration() const
            {
                auto length = WxTimeDuration::Zero();
                if (this->keys.size() > 1)
                    length = this->keys[this->keys.size() - 1].time - this->keys[0].time;
                return length;
            }

            /** The submesh that this subanimation corresponds to. */
            GeometryStateSubmesh* submesh;

            /**
             * The animation keys.
             * Each subanimation for an animation will have the same number of keys.
             */
            std::vector<Key> keys;
        };
        typedef std::shared_ptr<Subanimation> SubanimationPtr;

        /** The name of the animation. */
        wxString name;

        bool embedAnimation;
        bool linkToMainObject;

        /** The subanimations. */
        std::vector<SubanimationPtr> subanimations;
    };
    typedef std::shared_ptr<MeshMorphAnimation> MeshMorphAnimationPtr;

    /** A collection of mesh morph animations */
    struct MeshMorphAnimations
    {
        bool empty() const {return this->animations.empty();}
        void clear() {this->animations.clear();}
        size_t size() const {return this->animations.size();}
        void resize(size_t newSize) {this->animations.resize(newSize);}
        MeshMorphAnimationPtr& operator [] (size_t i) {return this->animations[i];}
        const MeshMorphAnimationPtr& operator [] (size_t i) const {return this->animations[i];}

        /** Determines if this collection has the same animations as another collection. */
        bool operator == (const MeshMorphAnimations& other) const
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

        /** Determines if this collection does NOT have the same animations as another collection. */
        bool operator != (const MeshMorphAnimations& other) const
        {
            return !(operator == (other));
        }

        /** Determines if this collection has an animation with the specified name. */
        bool HasAnimation(const wxString& name) const
        {
            for (size_t i = 0; i < this->animations.size(); i++)
            {
                if (this->animations[i]->name == name)
                    return true;
            }
            return false;
        }

        /** Transforms all the points of all morph animations using the specified matrix. */
        void TransformVertices(const MatrixAccessor& transformMatrix, const MatrixAccessor& normalTransformMatrix)
        {
            for (size_t animationIndex = 0; animationIndex < this->animations.size(); animationIndex++)
            {
                auto& animation = this->animations[animationIndex];
                for (size_t subanimationIndex = 0 ; subanimationIndex < animation->subanimations.size(); subanimationIndex++)
                {
                    auto& subanimation = animation->subanimations[subanimationIndex];
                    for (size_t keyIndex = 0; keyIndex < subanimation->keys.size(); keyIndex++)
                    {
                        auto& key = subanimation->keys[keyIndex];
                        for (size_t pointIndex = 0; pointIndex < key.points.size(); pointIndex++)
                        {
                            transformMatrix.TransformPoint(key.points[pointIndex].position);
                            normalTransformMatrix.TransformNormal(key.points[pointIndex].normal);
                        }
                    }
                }
            }
        }

        /** The actual collection of animations. */
        std::vector<MeshMorphAnimationPtr> animations;
    };

} }
