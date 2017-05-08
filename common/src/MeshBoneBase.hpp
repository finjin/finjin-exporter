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
#include "TransformAccessor.hpp"
#include "ObjectAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Base mesh bone class. */
    class MeshBoneBase
    {
    public:
        MeshBoneBase()
        {
            this->initialized = false;
            this->isExplicit = false;
            //this->skinHasPivotOffset = false;
            this->parent = nullptr;
        }

        virtual ~MeshBoneBase() {}

        /** Indicates whether this bone is a root. */
        bool IsRoot() const
        {
            return this->parent == nullptr;
        }

        /**
         * Sets the bone's initial state.
         * This should be called after manually setting the initialWorldTransform member.
         * @param skinTransform [in] - The initial skin transform.
         */
        void SetInitialState(const TransformAccessor& skinTransform)
        {
            this->initialRelativeTransform = this->initialWorldTransform.GetRelativeTo(IsRoot() ? skinTransform : this->parent->initialWorldTransform);

            this->initialized = true;
        }

        /** Gets the bone's world transformation at the specified time. */
        virtual TransformAccessor GetNodeTransformation(TimeAccessor time)
        {
            return this->object.GetNodeTransformation(time);
        }

    public:
        /** Indicates whether SetInitialState() has been called. */
        bool initialized;

        /**
         * Indicates whether the bone was explicitly defined.
         * If false, the bone was found implicitly.
         */
        bool isExplicit;

        /** Indicates whether the bone's skin has an extra pivot offset. */
        //bool skinHasPivotOffset; //This may not be needed any more

        /** The bone object. */
        ObjectAccessor object;

        /** The parent bone. */
        MeshBoneBase* parent;

        /** The bone's initial world transformation. */
        TransformAccessor initialWorldTransform;

        /** The bones initial world transformation, relative to its parent. */
        TransformAccessor initialRelativeTransform;
    };

} }
