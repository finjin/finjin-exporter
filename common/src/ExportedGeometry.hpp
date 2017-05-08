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
#include "ExportableObject.hpp"
#include "MaterialAccessor.hpp"
#include "AssetReference.hpp"
#include "finjin/common/WxDataChunkWriter.hpp"
#include "SceneExportSettings.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class GeometryState;

    struct ExportedGeometryManualLod
    {
        ExportableObject* exportableObject;
        GeometryState* geometryState;
        MaterialAccessorVector submeshMaterials;
    };
    typedef std::vector<ExportedGeometryManualLod> ExportedGeometryManualLods;

    /**
     * A GeometryState that has been created and is ready for export.
     */
    struct ExportedGeometry
    {
        ExportedGeometry
            (
            ExportableObject* exportableObject,
            GeometryState* geometryState,
            ExportedGeometryManualLods* manualLods,
            FinjinSceneSettingsAccessor sceneSettings,
            SceneExportSettings* sceneExportSettings
            )
        {
            this->exportableObject = exportableObject;
            this->geometryState = geometryState;
            if (manualLods != nullptr)
                this->manualLods = *manualLods;

            this->sceneSettings = sceneSettings;
            this->sceneExportSettings = sceneExportSettings;

            this->exportSkeleton = exportableObject->objectSettings.GetExportSkeleton();
            this->embedSkeleton = exportableObject->objectSettings.GetEmbedSkeletonInMesh();
            this->linkSkeletonToMesh = exportableObject->objectSettings.GetLinkSkeletonToMesh();
            this->exportPoses = exportableObject->objectSettings.GetExportPoses();
            this->exportAnimations = true;
            this->allowNoMaterialAssignment = true;
        }

        void Write(WxDataChunkWriter& chunkWriter, WxError& error);

        ExportableObject* exportableObject;
        GeometryState* geometryState;
        ExportedGeometryManualLods manualLods;

        FinjinSceneSettingsAccessor sceneSettings;
        SceneExportSettings* sceneExportSettings;

        bool exportSkeleton;
        bool embedSkeleton;
        bool linkSkeletonToMesh;
        bool exportPoses;
        bool exportAnimations;
        std::vector<wxString> alternateSubmeshMaterialNames;
        bool allowNoMaterialAssignment;
    };

} }
