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
#include "GenericBitmapFileName.hpp"
#include "SceneExportSettings.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "MaterialAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * Base class for handlers that export a material.
     */
    class MaterialExporterHandler
    {
    public:
        virtual ~MaterialExporterHandler() {Shutdown();}

        /**
         * Initializes the handler.
         * @param material [in] - The material to be exporter.
         * @param sceneSettings [in] - The scene settings.
         * @param sceneExportSettings [in] - The scene export settings.
         * @param exportFlags [in] - Export flags.
         * @param materialName [in] - The name of the material. If the REAL_TIME flag is
         * specified, this is not required.
         */
        void Initialize
            (
            const MaterialAccessor& material,
            FinjinSceneSettingsAccessor sceneSettings,
            const SceneExportSettings& sceneExportSettings
            );

        /** Cleans up the handler. */
        void Shutdown() {}

        /**
         * Indicates whether the handler, based on the material settings, needs a separate file.
         * @return If the material or handler settings require that the material be exported to
         * its own file, true is returned. Otherwise, false is returned, indicating that the
         * material can be exported into a file with other materials.
         */
        virtual bool NeedsSeparateFile() {return false;}

        MaterialAccessor& GetMaterial() {return this->material;}
        MaterialToString& GetMaterialDependencies() {return this->materialDependencies;}

        /** Gets the scene settings passed to Initialize(). */
        FinjinSceneSettingsAccessor GetSceneSettings() {return this->sceneSettings;}

        /** Gets all the bitmap file names used by the material. */
        GenericBitmapFileNameSet& GetBitmapFileNames() {return this->bitmapFileNames;}

        /** Gets all the bitmap file names used by the material. */
        void GetBitmapFileNames(GenericBitmapFileNameSet& bitmapFileNames) const;

        /** "Pre-exports", calculating its requirements. */
        virtual void CalculateRequirements() {}

        /** Runs the export, writing the results to the specified writer. */
        virtual void Write(WxDataChunkWriter& writer, WxError& error) = 0;

        /** Adds a single bitmap file name to the internal bitmap file names. */
        void AddBitmapFileName(const wxString& fileName);

        /** Adds an animated bitmap file name to the internal bitmap file names. */
        void AddAnimatedBitmapFileName(const wxString& fileName, int frameCount);

        /** Adds a cubic texture bitmap file name to the internal bitmap file names. */
        void AddCubicTextureBitmapFileBaseName(const wxString& fileName);

    protected:
        MaterialAccessor material;
        FinjinSceneSettingsAccessor sceneSettings;
        const SceneExportSettings* sceneExportSettings;
        GenericBitmapFileNameSet bitmapFileNames;
        MaterialToString materialDependencies;
    };

    /** A factory is responsible for creating a handler most appropriate for exporting a material. */
    class MaterialExporterHandlerFactory
    {
    public:
        virtual ~MaterialExporterHandlerFactory() {}

        /** Indicates whether the factory can handle the specified material. */
        virtual bool CanHandle(MaterialAccessor material) const = 0;

        /** Creates a handler for a material previously passed to CanHandle(). */
        virtual MaterialExporterHandler* CreateHandler() = 0;
    };

} }
