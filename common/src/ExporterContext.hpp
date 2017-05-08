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
#include "ExportableObjectBase.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "finjin/common/WxStreamingFileFormat.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class ExporterHandler;
    class SceneExportSettings;
    class GeometryState;

    /** A bone and all its attached objects. */
    struct BoneAndAttachments
    {
        /** The bone. */
        ExportableObjectPtr bone;

        /** The attached objects. */
        std::list<ExportableObjectPtr> attachedObjects;
    };
    typedef std::vector<BoneAndAttachments> AllBonesWithAttachments;

    /**
     *
     */
    class ExporterContext
    {
    public:
        /**
         * Constructor.
         * @param handler [in] - Pointer to the exporter handler in use.
         * @param sceneSettings [in] - The scene settings in use.
         * @param sceneExportSettings [in] - The scene export settings in use.
         */
        ExporterContext(ExporterHandler* handler, FinjinSceneSettingsAccessor sceneSettings, SceneExportSettings* sceneExportSettings);
        virtual ~ExporterContext();

        /** Gets the export handler in use. */
        ExporterHandler* GetHandler() {return this->handler;}

        /** Gets the scene settings in use. */
        FinjinSceneSettingsAccessor GetSceneSettings() {return this->sceneSettings;}

        /** Gets the scene export settings in use. */
        SceneExportSettings* GetSceneExportSettings() {return this->sceneExportSettings;}

        /** Gets a reference to all the objects in the scene. */
        ExportableObjectMap& GetAllObjects();

        /**
         * Ensures the specified object has a unique name.
         * @param object [in/out] - The object whose name is checked for uniqueness. If the object's default name is
         * not unique, it is given a new name.
         */
        void EnsureUniqueExportableObjectName(ObjectAccessor& object);

        /**
         * Associates an object and its exportable object.
         * @param object [in] - The application object.
         * @param exportableObject [in] - The exportable object.
         */
        void AddExportableObject(ObjectAccessor object, ExportableObjectPtr exportableObject);

        /** Gets the exportable object for the specified object. */
        ExportableObjectPtr FindExportableObject(ObjectAccessor object);

        /** Determines if there is an exportable object for the specified object. */
        bool HasExportableObject(ObjectAccessor object) const;

        /** Adds an object to be ignored. */
        void AddIgnoreObject(ObjectAccessor object);

        /** Determines if the specified object has been flagged to be ignored. */
        bool HasIgnoreObject(ObjectAccessor object) const;

        virtual bool ShouldIgnoreObject(ExportableObject* exportableObject, bool rebuildingObjectGraph = false);
        virtual bool ShouldIgnoreChildren(ExportableObject* exportableObject);

        /** Adds all the dependencies for an exportable object, assuming it's a mesh of some sort. */
        void AddMeshDependencies(ExportableObject* exportableObject);

        /** Resolves all the dependencies for an exportable object, assuming it's a mesh of some sort. */
        void ResolveMeshDependencies(ExportableObject* exportableObject);

        /**
         * Gets all the bone attachments for an object, assuming it has a skeleton.
         * @param allBonesWithAttachments [out] - Contains all the bones and their attached objects.
         * @param exportableObject [in] - The exportable object.
         * @param geometryState [in] - The geometry for the exportable object.
         */
        void GetBoneAttachments(AllBonesWithAttachments& allBonesWithAttachments, ExportableObject* exportableObject, GeometryState* geometryState);

        /**
         * Replaces the object children with the root-most selected objects. Also,
         * the entire scene graph hierarchy is modified so that it only includes the selected
         * objects and whatever child objects may exist along the path between selected objects.
         * NOTE: This should only be called on the root object.
         * @param exportableObject [in/out] - The exportable object whose children will be modified.
         */
        void ReplaceChildrenWithSelectedObjects(ExportableObject* exportableObject);

    protected:
        ExporterHandler* handler;
        FinjinSceneSettingsAccessor sceneSettings;
        SceneExportSettings* sceneExportSettings;
        ExportableObjectMap allObjects;
        ObjectAccessorSet ignoreObjects;

        /** All the object names, used when determining name uniqueness. */
        std::set<wxString> objectNames;

        /** All the node names, used when determining name uniqueness. */
        std::set<wxString> nodeNames;
    };

} }
