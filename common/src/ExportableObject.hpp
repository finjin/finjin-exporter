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
#include "ObjectAccessor.hpp"
#include "FinjinObjectSettingsAccessor.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "ObjectExportSettings.hpp"
#include "ExportableObjectBase.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class ExporterContext;
    class SceneExportSettings;
    
    /** 
     * This class is used in the construction of a scene graph that is appropriate for export, which is
     * easier to deal with than the native application's scene graph.
     */
    class ExportableObject
    {
    public:
        ExportableObject();
        virtual ~ExportableObject();

        /** 
         * Constructor for scene root object. This constructor recursively creates the scene graph.
         * @param object [in] - The scene root object.
         * @param exporterContext [in/out] - The context in use.
         */
        virtual void Initialize(ObjectAccessor object, ExporterContext& exporterContext);

        /** 
         * Equivalent to calling InitializeStart(), if that method were public. 
         * This method is useful when exportable objects need to be created outside of the normal
         * way, such as what occurs with XSI when dealing with the light and camera interest objects.
         */
        void Initialize
            (
            ExportableObject* parent,
            ObjectAccessor object, 
            FinjinObjectSettingsAccessor objectSettings,
            ExporterContext& context
            );

        /** Creates a new instance of the ExportableObject class. This should be overridden by derived classes. */
        virtual ExportableObject* NewInstance();

        TransformAccessor GetObjectOffsetTransformation(TimeAccessor time);

        TransformAccessor GetConvertedObjectOffsetTransformation(TimeAccessor time, CoordinateSystemConverter& conversionManager, float scale);

        TransformAccessor GetNodeTransformation(TimeAccessor time);

        /**
         * Gets the world transform for the object at the specified time.
         * @param time [in] - The time at which the world transformation is retrieved.
         * @return The object's world transformation is returned.
         */
        TransformAccessor GetFullWorldTransformation(TimeAccessor time);

        /**
         * Gets the local transform (relative to the parent) for the object at the specified time.
         * @param time [in] - The time at which the local transformation is retrieved.
         * @param parentTransform [in] - If specified, points to an alternate parent transform. If not specified,
         * the object's parent transform will be used.
         * @return The object's local transformation is returned.
         */
        TransformAccessor GetLocalNodeTransformation(TimeAccessor time, const TransformAccessor* parentTransform = nullptr);

        /**
         * Gets the world transform for the object at the specified time, taking into account any
         * special transformations required due to the object type.
         * @param time [in] - The time at which the world transformation is retrieved.
         * @param conversionManager [in] - The converter used to adjust for the world coordinate space.
         * @param scale [in] - Translation/scale scaling factor.
         * @return The object's world transformation is returned.
         */
        TransformAccessor GetConvertedNodeTransformation(TimeAccessor time, CoordinateSystemConverter& conversionManager, float scale, FinjinVector4* objectRotation = nullptr);

        /**
         * Gets the local transform (relative to the parent) for the object at the specified time, taking into account any
         * special transformations required due to the object type.
         * @param time [in] - The time at which the local transformation is retrieved.
         * @param parentTransform [in] - If specified, points to an alternate parent transform. If not specified,
         * the object's parent transform will be used.
         * @param conversionManager [in] - The converter used to adjust for the world coordinate space.
         * @param scale [in] - Translation/scale scaling factor.
         * @return The object's local transformation is returned.
         */
        TransformAccessor GetConvertedLocalNodeTransformation(TimeAccessor time, CoordinateSystemConverter& conversionManager, float scale, FinjinVector4* objectRotation = nullptr);

        /** 
         * Resolves the object's dependencies.
         * @param context [in] - The context in use.
         */
        virtual void ResolveDependencies(ExporterContext& context);

        /** 
         * Gets the closest object when moving up the hierarchy with a type that indicates it is a container. 
         * @return The closest object with a type indicating it is a container is returned. If this object
         * is a container, it is returned.
         */
        virtual ExportableObject* GetBestContainer();

        /** Determines if this object is consumed by its parent. */
        bool IsConsumedByParent();

        /** 
         * Removes this object from its parent.
         * Note that if this is called as a result of ResolveDependencies() being called, the removal from the parent
         * will be deferred until the dependency resolution is complete.
         */
        void RemoveFromParent();

        /** Adds the child object to the most appropriate collection, possibly owned by an ancestor object. */
        void AddBestChild(ExportableObjectPtr child, ExporterContext& context);

        /** Adds the child object to this object. */
        void AddChild(ExportableObjectPtr child);

        /** 
         * Removes the specified child from this object. 
         * This should not be called during dependency resolution. 
         */
        void RemoveChild(ExportableObject* child);

        /** 
         * Removes the all the children from this object. 
         * This should not be called during dependency resolution. 
         */
        void RemoveChildren();

        /** Gets the shared pointer for this object. */
        ExportableObjectPtr GetSharedPtr();

        /** Gets the shared pointer for the specified child. */
        ExportableObjectPtr GetChildPtr(ExportableObject* child);

        /** 
         * Gets the number of children.
         * @param recursive [in] - Determines whether the count should be determined recursively.
         * @return The number of children is returned.
         */
        int GetChildCount(bool recursive = true) const;

        /**
         * Gets the exportable dependency object for the specified object.
         * @param dependentObject [in] - The dependent object.
         * @return The corresponding exportable object is returned.
         */
        ExportableObjectPtr GetDependentExportableObject(ObjectAccessor dependentObject);

        /** 
         * Adds a dependency.
         * @param object [in] - The dependent object.
         * @param tag [in] - A string tag for the object.
         * @return If the dependent object is valid and hasn't already been added, true is returned. Otherwise,
         * false is returned.
         */
        bool AddDependency(ObjectAccessor object, const wxString& tag = wxEmptyString);

        /** Determines if the specified object is a dependent object. */
        bool HasDependency(ObjectAccessor object) const;

        /** Callback passed to IterationChildren. */
        class IterateCallback
        {
        public:
            virtual ~IterateCallback() {}

            /** A value returned by HandleObject(). */
            enum Result
            {
                /** Indicates that the iteration should continue. */
                CONTINUE_ITERATION,

                /** 
                 * Indicates that the iteration should skip the object. 
                 * This is only useful when recursively iterating over the children. 
                 */
                SKIP_ITERATION,

                /** Indicates that the iteration should stop. */
                END_ITERATION
            };

            /**
             * Called for every child encountered during the iteration.
             * @param exportableObject [in] - The child.
             * @return A value indicating the result of the iteration is returned.
             */
            virtual Result HandleObject(ExportableObjectPtr exportableObject) = 0;
        };
        
        /** 
         * Iterates of the child objects, calling the specified callback at each step.
         * @param callback [in] - The callback to call.
         * @param recursive [in] - Indicates whether the child iteration occurs recursively.
         * @return The result of the iteration is returned.
         */
        IterateCallback::Result IterateChildren(IterateCallback& callback, bool recursive = true);

    private:
        /** 
         * Initializes non-root objects. This recursively creates the scene graph.
         * @param parent [in] - The parent of the object.
         * @param object [in] - The object.
         * @param objectSettings [in] - The settings for the object.
         * @param exporterContext [in/out] - The context in use.
         */
        virtual void InitializeStart
            (
            ExportableObject* parent,
            ObjectAccessor object, 
            FinjinObjectSettingsAccessor objectSettings,
            ExporterContext& exporterContext
            );

        /**
         * Finishes the initialization for non-root objects. Recursively finishes initialization.
         * @param exporterContext [in/out] - The context in use.
         */
        virtual void InitializeEnd(ExporterContext& context);

    public:
        /** The number of times the object has been encountered during export, typically during ExportExportableObject() */
        int exportCount;

        /** Pointer to the root. */
        ExportableObject* root;

        /** Pointer to the parent. */
        ExportableObject* parent;

        std::unordered_map<ExportableObject*, ObjectAccessor> changeParent;

        /** The actual object. */
        ObjectAccessor object;

        /** Object settings for the object. */
        FinjinObjectSettingsAccessor objectSettings;

        /** Export settings for the object. */
        ObjectExportSettings objectExportSettings;

        /** The detected object type. */
        DetectableObjectType* detectedObjectType;
     
        /** Children of this object. Matches the application's notion of a child. */
        std::list<ExportableObjectPtr> initializedChildObjects;

        /** Children of this object. May differ slightly from the application's notion of a child. */
        std::list<ExportableObjectPtr> childObjects;

        /** Objects that are exported as siblings of the parent object. */
        std::list<ExportableObjectPtr> siblingObjects;

        /**
         * Objects that are merged into the parent object, such as when submesh objects
         * are merged into a parent mesh.
         */
        std::list<ExportableObjectPtr> mergedObjects;

        struct Dependency
        {
            Dependency() {}
            Dependency(ObjectAccessor object) {this->object = object;}
            Dependency(ObjectAccessor object, wxString tag) {this->object = object; this->tag = tag;}

            operator ObjectAccessor () {return object;}

            bool operator == (ObjectAccessor other) const {return this->object == other;}

            ObjectAccessor object;
            wxString tag;
        };

        /** A list is used for dependent objects so that order is maintained. */
        typedef std::list<Dependency> DependentObjects;

        /** 
         * A list of objects that this object dependent on.
         * Objects added to this list will be mapped, if possible, to exportable objects and
         * entered into the 'dependentObjectMap' member.
         */
        DependentObjects dependentObjects;

        /** Maps dependent objects to their corresponding exportable objects. */
        ExportableObjectMap dependentObjectMap;

    private:
        bool startingInitialize;
        bool resolvingDependencies;

        std::list<ExportableObject*> objectsForRemovalFromParent;
    };

} }
