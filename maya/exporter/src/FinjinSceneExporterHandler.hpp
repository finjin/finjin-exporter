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
#include "ExporterHandler.hpp"
#include "FinjinSceneDocument.hpp"
#include "GenericBitmapFileName.hpp"
#include "FinjinSceneExporterContext.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Handler for exporting a Finjin scene. */
    class FinjinSceneExporterHandler : public ExporterHandler         
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(ExporterHandler)

        bool Initialize(AssetClass assetType, const wxString& fileName, bool exportSelected, FinjinSceneSettingsAccessor sceneSettings) override;

        bool RunExport() override;

        void ExportScene(ExportableObjectPtr& exportableObject, FinjinSceneDocument& scene, ExporterContext& context);

    protected:
        void ExportChildren
            (
            FinjinSceneDocument& scene, 
            FinjinSceneDocumentParentItems parentItems, 
            ExportableObjectPtr object,
            FinjinSceneExporterContext& finjinExporterContext
            );

        FinjinSceneDocumentParentItems ExportExportableObject
            (
            FinjinSceneDocument& scene, 
            FinjinSceneDocumentParentItems parentItems, 
            ExportableObjectPtr exportableObject,
            bool createNodeIfNeeded,
            FinjinSceneExporterContext& finjinExporterContext,
            bool affectsObjectProgressCounter = true
            );

        std::shared_ptr<FinjinSceneDocument_SceneNode> ExportNode
            (
            FinjinSceneDocument& scene, 
            FinjinSceneDocumentParentItems parentItems, 
            ExportableObject* exportableObject, 
            FinjinSceneExporterContext& finjinExporterContext
            );

        void ExportCamera
            (
            FinjinSceneDocument& scene, 
            FinjinSceneDocumentParentItems parentItems, 
            MObject camera, 
            ExportableObject* exportableObject,
            FinjinSceneExporterContext& finjinExporterContext
            );

        void ExportLight
            (
            FinjinSceneDocument& scene, 
            FinjinSceneDocumentParentItems parentItems, 
            MObject light, 
            ExportableObject* exportableObject,
            FinjinSceneExporterContext& finjinExporterContext
            );

        void ExportEntity
            (
            FinjinSceneDocument& scene, 
            FinjinSceneDocumentParentItems parentItems, 
            ExportableObject* exportableObject, 
            FinjinSceneExporterContext& finjinExporterContext
            );

        void ExportSky(FinjinSceneDocument& scene, ExportableObject* exportableObject, FinjinSceneExporterContext& finjinExporterContext);

        void ExportPhysical
            (
            FinjinSceneDocument& scene, 
            FinjinSceneDocumentParentItems parentItems, 
            ExportableObject* exportableObject,
            FinjinSceneExporterContext& finjinExporterContext
            );

        void ExportStaticGeometry
            (
            FinjinSceneDocument& scene, 
            ExportableObject* exportableObject,
            FinjinSceneExporterContext& finjinExporterContext
            );

        void ExportInstancedGeometry
            (
            FinjinSceneDocument& scene, 
            ExportableObject* exportableObject,
            FinjinSceneExporterContext& finjinExporterContext
            );

        void ExportStaticAndInstancedGeometryEntities
            (
            FinjinSceneDocument& scene, 
            ExportableObject* exportableObject,
            FinjinSceneExporterContext& finjinExporterContext,
            FinjinSceneDocument_Item* exportedOwnerItem,
            const TransformAccessor& ownerTransformNotConverted
            );

        void ExportBoneAttachments
            (
            FinjinSceneDocument& scene, 
            FinjinSceneDocument_Entity* exportedEntity,
            ExportableObject* exportableObject,
            GeometryState* geometryState,
            FinjinSceneExporterContext& finjinExporterContext
            );
    };

    /** Handler factory for exporting a Finjin scene. */
    class FinjinSceneExporterHandlerFactory : public ExporterHandlerFactory
    {
    public:
        int GetExtensionCount() const override
        {
            return 1;
        }

        const wxString& GetDisplayName() const override
        {
            static const wxString DISPLAY_NAME(wxT("Finjin Scene"));
            return DISPLAY_NAME;
        }

        void GetExtension(int index, wxString& extension, wxString* description) const override
        {
            switch (index)
            {
                case 0: extension = wxT(".fstd-scene"); if (description != nullptr) *description = wxT("Finjin Streaming Text Scene"); break;
                case 1: extension = wxT(".fsbd-scene"); if (description != nullptr) *description = wxT("Finjin Streaming Binary Scene"); break;
                case 2: extension = wxT(".json-scene"); if (description != nullptr) *description = wxT("Finjin Streaming JSON Scene"); break;
                case 3: extension = wxT(".cfg-scene"); if (description != nullptr) *description = wxT("Finjin Config Scene"); break;
            }
        }

        ExporterHandler* CreateHandler() override
        {
            return new FinjinSceneExporterHandler;
        }

        static FinjinSceneExporterHandlerFactory& GetInstance()
        {
            static FinjinSceneExporterHandlerFactory instance;
            return instance;
        }
    };

} }
