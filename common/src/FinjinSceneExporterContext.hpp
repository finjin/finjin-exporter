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
#include "ExporterContext.hpp"
#include "ProgressCalculator.hpp"
#include "ExportableObject.hpp"
#include "MaterialAccessor.hpp"
#include "MaterialExporterHandler.hpp"
#include "GenericBitmapFileName.hpp"
#include "finjin/common/WxDataChunkWriter.hpp"
#include "ExportedGeometry.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class GeometryState;
    class FinjinSceneDocument;
    class FinjinSceneDocument_Item;
    class FinjinSceneDocument_MovableObject;
    class FinjinSceneDocument_RenderableMovableObject;
    class FinjinSceneDocument_Entity;

    enum class FinjinSceneExporterContextFlags
    {
        NONE = 0,

        /**
         * Indicates that materials should always be added, even if it is disabled at the scene level.
         * That does not have an effect on Finjin materials that have the 'Export' setting disabled.
         */
        ALWAYS_ADD_MATERIALS = 1 << 0
    };
    FINJIN_ENUM_BITWISE_OPERATIONS(FinjinSceneExporterContextFlags)

    /**
     * The ExporterContext used by FinjinSceneExporterHandler during the exporter of Finjin scenes.
     * It collects a lot of functionality that all versions of the Finjin exporter share.
     */
    class FinjinSceneExporterContext :
        public ExporterContext,
        public ProgressCalculator::ProgressChangeListener
    {
    public:
        /**
         * Constructor.
         * @param handler [in] - Pointer to the exporter handler that owns the context.
         * @param sceneSettings [in] - The scene settings.
         * @param sceneExportSettings [in] - Pointer to the scene export settings.
         * @param flags [in] - Initialization flags.
         */
        FinjinSceneExporterContext(ExporterHandler* handler, FinjinSceneSettingsAccessor sceneSettings, SceneExportSettings* sceneExportSettings, FinjinSceneExporterContextFlags flags);

        ~FinjinSceneExporterContext();

        /**
         * Runs the export process on the scene, calling back to the handler as necessary.
         * @param exportFileName [in] - Full path to the scene file to be exported.
         * @param exportSelected [in] - Indicates whether just the selected objects should be exported.
         * If false, the entire scene is exported.
         */
        void RunExport(const wxString& exportFileName, bool exportSelected);

        /**
         * Initializes the progress calculators associated with the export process.
         * @param rootObject [in] - The root object from which the export starts.
         */
        void StartProgress(ExportableObject* rootObject);

        /**
         * Puts the progress calculators into a finished state.
         * This is called internally.
         */
        void FinishProgress();

        /**
         * Starts the scene export.
         * This is called by the scene exporter handler.
         * @param scene [out] - The scene document which is to contain the exported scene data.
         * If null, this indicates that not scene document will be generated.
         */
        void StartSceneExport(FinjinSceneDocument* scene = nullptr);

        /**
         * Finishes the scene export by exporting materials, meshes, and anything else.
         * This is called by the scene exporter handler.
         */
        void FinishSceneExport();

        /**
         * Adds the material for later export.
         * @param material [in] - The material to add.
         * @param useShaderMeshRequirements [in] - Indicates whether the shader mesh requirements should be used.
         * @param shaderMeshRequirements [in] - Some basic information about the mesh that uses the material.
         */
        void AddMaterial(MaterialAccessor material);

        /**
         * Makes a node/object name.
         * @param object [in] - The object for which the name is generated.
         * @param useObjectName [in] - Indicates whether to use the object name. If false,
         * the node name is used. Depending on the application, these two names may be the same.
         * @return The appropriate name is returned.
         */
        wxString MakeNodeOrObjectName(ObjectAccessor object, bool useObjectName = true);

        /** Determines whether the specified object should be ignored during the export process. */
        bool ShouldIgnoreObject(ExportableObject* exportableObject, bool rebuildingObjectGraph = false) override;

        /** Determines whether the children of the specified object should be ignored during the export process. */
        bool ShouldIgnoreChildren(ExportableObject* exportableObject) override;

        /** Gets the number of exportable children from the specified object. */
        int GetExportableChildCount(ExportableObject* object);

        /**
         * Get the geometry for the specified exportable object, creating it if necessary.
         * @param exportableObject [in] - The object that contains the mesh that is to be retrieved.
         * @param submeshMaterials [out] - If specified, receives the collection of materials applied to the geometry submeshes,
         * one for each submesh.
         * @param manualLods [out] - If specified, receives the collection of manual LODs specified for the mesh.
         * @param parentGeometry [in] - If specified, points ot the geometry into which the geometry being retrieved will
         * later be merged. A null parent indicates the geometry is not going to be merged.
         * @param createGeometryStates [in] - If specified, indicates whether the GeometryState object(s) should be retrieved.
         * Normally, this will be true, but it can be set to false to query submesh materials and manual LOD information.
         * @return An existing geometry is returned if the exportableObject's geometry can be matched to an existing instance.
         * Otherwise, the returned geometry is new. If parentGeometry is non-null, the returned geometry must be deleted by
         * the caller. Otherwise, it is managed by the context.
         */
        GeometryState* GetOrCreateGeometry
            (
            ExportableObject* exportableObject,
            MaterialAccessorVector* submeshMaterials = nullptr,
            ExportedGeometryManualLods* manualLods = nullptr,
            GeometryState* parentGeometry = nullptr,
            bool createGeometryStates = true
            );

        void GetItemSettings(FinjinSceneDocument_Item* item, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings);

        void GetMovableObjectSettings
            (
            FinjinSceneDocument_MovableObject* movableObject,
            ObjectAccessor object,
            FinjinObjectSettingsAccessor objectSettings,
            TimeAccessor time,
            CoordinateSystemConverter& conversionManager,
            float scale
            );

        void GetRenderableObjectSettings
            (
            FinjinSceneDocument_RenderableMovableObject* renderableObject,
            ObjectAccessor object,
            FinjinObjectSettingsAccessor objectSettings,
            TimeAccessor time,
            CoordinateSystemConverter& conversionManager,
            float scale
            );

        /**
         * Generates node animations for the specified exportable object.
         * @param item [out] - The item that will receive the generated animations.
         * @param exportableObject [in] - The object for which node animations are generated.
         */
        void GenerateNodeAnimations(FinjinSceneDocument_Item* item, ExportableObject* exportableObject);

    protected:
        /**
         * Finds an matching existing instance for the specified geometry.
         * This is called by GetOrCreateGeometry() when searching for a geometry's original instance.
         * @param [in] - The geometry to try to match to an existing instance.
         * @return If a matching instance is found, it is returned. Otherwise, null is returned.
         */
        ExportedGeometry* FindOriginalInstance(GeometryState* geometryState);

        /** ProgressCalculator::ProgressChangeListener implementation. */
        void ProgressChanged(ProgressCalculator* progress) override;

    public:
        /** The top-level progress calculator. This corresponds to the total progress in the progress dialog. */
        ProgressCalculator sceneProgress;

        //Various progress calculators. Individually, these correspond to the subprogress in the progress dialog.
        std::shared_ptr<ProgressCalculator> objectGraphProgress;
        std::shared_ptr<ProgressCalculator> meshesProgress;
        std::shared_ptr<ProgressCalculator> materialsProgress;
        std::shared_ptr<ProgressCalculator> bitmapsProgress;

        /** The main scene document. */
        FinjinSceneDocument* scene;

        /** A list of a unique geometries generated during the export of the scene. */
        std::list<std::shared_ptr<GeometryState> > geometryStates;

        /**
         * A list of all the geometries associated with objects in the scene.
         * Some of these may be duplicates, referring multiple times to entries in geometryStates.
         */
        std::list<ExportedGeometry> exportedGeometries;

        std::list<ExportedGeometry> uniqueExportedGeometries;

        std::list<ExportedGeometry> uniqueExportedSkeletons;

        /** Flags passed to the constructor. */
        FinjinSceneExporterContextFlags flags;

        /** All the materials encountered during the export of the scene. */
        MaterialAccessorMap<std::shared_ptr<MaterialExporterHandler> > materialHandlerMap;

        WxDataChunkWriter* dataChunkWriter;
    };

} }
