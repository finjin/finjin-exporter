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
    
    /** Provides information about a detectable type. */
    class DetectableObjectTypeDescriptor
    {
    public:
        virtual ~DetectableObjectTypeDescriptor() {}

        /** Gets the name used as the value to be stored in the scene file. */
        virtual const wxString& GetTypeName() const = 0;

        /** Gets the name to display. */
        virtual const wxString& GetDisplayName() const = 0;

        /**
         * Indicates whether the object type is universal.
         * A universal type is a type that can be applied to any object, such as "ignore".
         */
        virtual bool IsUniversal() const {return false;}

        /** Indicates whether the object can be attached (made a child) of a node. */
        virtual bool CanAttachToNode() const {return false;}

        /**
         * Indicates whether the object type is some type of sky.
         */
        virtual bool IsSky() const {return false;}

        /**
         * Indicates whether the object type is some type of camera.
         */
        virtual bool IsCamera() const {return false;}

        /**
         * Indicates whether the object type is some type of light.
         */
        virtual bool IsLight() const {return false;}

        /**
         * Indicates whether the object type is some type of movable object.
         * A movable object is one that corresponds to the FinjinObsolete::MovableObject type, or one
         * of its subclasses.
         */
        virtual bool IsMovableObject() const {return false;}

        virtual bool CanBePrefab() const {return true;}

        /**
         * Indicates whether the object type uses the pivot point to offset the exported object
         * One such type is the entity/mesh type, which uses the pivot when exporting the mesh.
         */
        //virtual bool UsesPivotAsObjectOffset() const {return false;}

        /**
         * Indicates whether the object type is for a merged type of object.
         * A merged object is one that must be merged with another, typically the parent. 
         * One such type is the submesh type, which is a mesh that is merged with its parent mesh.
         */
        virtual bool IsMerged() const {return false;}

        /**
         * Indicates whether the object type is capable of containing other objects.
         * The most obvious example of such an object would be a scene root.
         */
        virtual bool IsContainer() const {return false;}

        /**
         * Indicates whether the object uses all of its children when exported. 
         * This is the case for some objects, like terrains (since they can't have children).
         * Returning true for this is equivalent to (and more efficient than) always returning true 
         * for DetectableObjectType::IsConsumedChild().
         */
        virtual bool ConsumesAllChildren() const {return false;}
    };

} }
