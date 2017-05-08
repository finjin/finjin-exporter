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
#include "SceneExportSettings.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "ProgressCalculator.hpp"
#include "MaterialExporterHandler.hpp"
#include "finjin/common/WxDataChunkWriter.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * The Exporter class is the entry point for performing an export of a material.
     * When exporting, it selects the appropriate exporter handler factory, creates a handler, and calls export on that handler.
     * Before exporting, during the plugin's initialization, at least one exporter handler must be registered by calling MaterialExporter::RegisterHandlerFactory()
     */
    class MaterialExporter : public ProgressCalculator::ProgressChangeListener
    {
    public:
        MaterialExporter();

        /** Sets the progress calculator to use during the export. */
        void SetProgressCalculator(std::shared_ptr<ProgressCalculator> progress);
        void SetBitmapsProgressCalculator(std::shared_ptr<ProgressCalculator> progress);

        /**
         * Exports a collection of materials.
         * @param materials [in] - The materials to export.
         * @param sceneSettings [in] - The scene settings.
         * @param sceneExportSettings [in] - The scene export settings. The particular setting of
         * interest is sceneExportSettings.materialFileName, which is the file to which all the materials are saved.
         * @param flags [in] - The export flags.
         */
        bool Export
            (
            WxDataChunkWriter& writer,
            const MaterialAccessorMap<std::shared_ptr<MaterialExporterHandler> >& materials,
            FinjinSceneSettingsAccessor sceneSettings,
            const SceneExportSettings& sceneExportSettings
            );

        void ExportBitmaps
            (
            WxDataChunkWriter& writer,
            FinjinSceneSettingsAccessor sceneSettings,
            const SceneExportSettings& sceneExportSettings,
            WxError& error
            );

        /**
         * Prompts the user for a material file name, then exports all materials in the scene.
         */
        static void ExportSceneMaterialsWithDialog();

        /**
         * Prompts the user for a material file name, then exports all selected materials.
         */
        static void ExportSelectedMaterialsWithDialog();

        /**
         * Prompts the user for a material file name, then exports all specified materials to that file.
         * @param materials [in] - The materials to export.
         * @param exportFlags [in] - The export flags.
         */
        static void ExportMaterialsWithDialog(const MaterialAccessorVector& materials);

        /** Gets all of the bitmap file names that were encountered during the export process. */
        const GenericBitmapFileNameSet& GetBitmapFileNames() const;

        /** Registers an exporter handler factory. */
        static void RegisterHandlerFactory(MaterialExporterHandlerFactory& factory);

        /** Removes all registered exporter handler factories. */
        static void ClearHandlerFactories();

        /** Gets the exporter handler factory for the specified material. */
        static MaterialExporterHandlerFactory* GetFactoryForMaterial(MaterialAccessor material);

    protected:
        /** Called when one of the WithDialog() functions are called. */
        void InitializeWithDialog();

        /** ProgressCalculator::ProgressChangeListener implementation. */
        void ProgressChanged(ProgressCalculator* progress) override;

    private:
        typedef std::list<MaterialExporterHandlerFactory*> FactoryList;

        /** The registered exporter handler factories */
        static FactoryList factories;

        GenericBitmapFileNameSet bitmapFileNames;

        /** Pointer to the active progress calculator. May be null. */
        std::shared_ptr<ProgressCalculator> materialsProgress;
        std::shared_ptr<ProgressCalculator> bitmapsProgress;

        /** Used when one of the WithDialog() functions are called. */
        ProgressCalculator withDialogProgress;
    };

} }
