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
#include "ObjectAccessorBase.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Access to an object. */
    class ObjectAccessor : public ObjectAccessorBase
    {
    public:
        /** Default constructor. */
        ObjectAccessor();

        /** Constructor that takes an object. */
        ObjectAccessor(INode* node);

        /** Constructor that takes an object. */
        ObjectAccessor(const ObjectAccessor& object);

        /** Accesses the native object. */
        operator INode* ();

        /** Determines whether this object equals the other object. */
        bool operator == (const ObjectAccessor other) const;

        /** Determines whether this object doesn't equal the other object. */
        bool operator != (const ObjectAccessor other) const;

        /** Determines whether this object is less than the other object. */
        bool operator < (const ObjectAccessor other) const;

        /**
         * Gets the object's local name.
         * @param useObjectName [in] - Indicates whether the object name is used.
         * If false, the node name is used.
         */
        wxString GetLocalName(bool useObjectName = true) const;

        /**
         * Gets the object's full name.
         * @param useObjectName [in] - Indicates whether the object name is used.
         * If false, the node name is used.
         */
        wxString GetFullName(bool useObjectName = true) const;

        /** Gets the object's parent. */
        ObjectAccessor GetParent();

        /** Gets the object's children. */
        void GetChildren(std::vector<ObjectAccessor>& children);

        /** Determines whether this object contains the other object as a descendant. */
        bool HasDescendant(ObjectAccessor other);

        /** Determines whether the object is valid. */
        bool IsValid() const;

        /** Determines whether the object is the scene root. */
        bool IsRoot() const;

        /** Determines whether the object is visible in the viewports. */
        bool IsVisible() const;

        /** Determines whether the object casts shadows. */
        bool GetCastShadows() const;

        /** Sets the object's shadow casting setting. */
        void SetCastShadows(bool value);

        /** Determines whether the object receives shadows. */
        bool GetReceiveShadows() const;

        /** Determines whether the object is a camera. */
        bool IsCamera() const;

        /** Determines whether the object is a light. */
        bool IsLight() const;

        /** Determines whether the object is an ambient light. */
        bool IsAmbientLight() const;

        /** Gets the object's light color at the specified time, assuming it's a light. */
        FinjinColor GetLightColor(TimeAccessor time);

        /** Gets the material assigned to the object. */
        Mtl* GetMaterial();

        /** Gets the default primitive type for the object, assuming it has some type of geometry. */
        PrimitiveType GetDefaultPrimitiveType();

        /** Gets the object offset transformation at the specified time. */
        TransformAccessor GetObjectOffsetTransformation(TimeAccessor time);

        TransformAccessor GetNodeTransformation(TimeAccessor time);

        TransformAccessor GetFullWorldTransformation(TimeAccessor time);

        /**
         * Gets the object's world transformation at the specified time.
         * @param time [in] - The time at which the world transformation is retrieved.
         * @param addPivotOffset [in] - Indicates whether the pivot offset should be added
         * to the world transformation.
         */
        //TransformAccessor GetWorldTransformation(TimeAccessor time, bool addPivotOffset);

        /** Gets the note tracks for the object. */
        void GetNoteTracks(std::vector<FinjinNoteTrack>& noteTracks);

        /** Gets the object that this object is looking at. */
        ObjectAccessor GetLookAtObject();

        /** Gets the scene root object. */
        static ObjectAccessor GetRootObject();

        /** Gets all the descendants beneath this object. */
        template <class T>
        void GetAllObjects(T& objects)
        {
            std::vector<ObjectAccessor> childObjects;
            GetChildren(childObjects);

            for (size_t childIndex = 0; childIndex < childObjects.size(); childIndex++)
            {
                if (!IsSettingsObject(childObjects[childIndex]))
                {
                    objects.push_back(childObjects[childIndex]);
                    childObjects[childIndex].GetAllObjects(objects);
                }
            }
        }

        /** Gets an object by name. */
        static ObjectAccessor GetObjectByName(const wxString& name);

        /** Gets all the selected objects. */
        static void GetSelectedObjects(std::vector<ObjectAccessor>& selectedObjects);

        /** Sets all the selected objects. */
        static void SetSelectedObjects(const std::vector<ObjectAccessor>& selectedObjects);

        /** Clears the current object selection set. */
        static void ClearSelectedObjects();

        /** Determines whether the specified object is some kind of bone. */
        static bool IsBone(ObjectAccessor object);

        /**
         * Determines whether the specified object is used exclusively for settings.
         * Such objects are typically hidden from the user and ignored during the export process.
         */
        static bool IsSettingsObject(ObjectAccessor object);

    public:
        INode* node;
    };

} }


//Includes----------------------------------------------------------------------
#include "ObjectAccessorExtras.hpp"
