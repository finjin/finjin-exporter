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
#include "FinjinSceneSettingsAccessor.hpp"
#include "ExporterHandler.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    enum class ExporterFlags
    {
        NONE = 0,

        NO_STATUS_DIALOGS = 1 << 0
    };
    FINJIN_ENUM_BITWISE_OPERATIONS(ExporterFlags)

    /**
     * The Exporter class is the entry point for performing an export of the scene managed by the application.
     * When exporting, it selects the appropriate exporter handler factory, creates a handler, and calls
     * export on that handler.
     * Before exporting, during the plugin's initialization, at least one exporter handler
     * must be registered by calling Exporter::RegisterHandlerFactory()
     */
    class Exporter
    {
    public:
        /**
         * Performs a scene export using the specified file name
         * @param fileName [in] - The name of the file to which the scene is saved. If empty,
         * the scene's 'default scene file' setting is used. If that's empty, then the export fails
         * and false is returned. In other words, a 'Save As' dialog never appears
         * @param selected [in] - Indicates whether just the selected scene objects should be exported.
         * If false, the entire scene is exported
         * @param factory [in] - The export handler factory used to export the scene. If null, a factory
         * will be determined based on the file name's extension
         * @param allowStatusDialogs [in] - Indicates whether status dialogs, the progress dialog in
         * particular, are displayed during the export.
         * @return If the export was successful, true is returned. Otherwise, false is returned
         */
        static bool ExportWithFileName
            (
            const wxString& fileName,
            bool selected,
            ExporterHandlerFactory* factory = nullptr,
            bool allowStatusDialogs = true
            );

        /**
         * Performs a scene export using either the scene's 'default scene file' or one retrieved from
         * a 'Save As' dialog
         * @param selected [in] - Indicates whether just the selected scene objects should be exported.
         * If false, the entire scene is exported
         * @param forceSaveDialog [in] - Indicates whether a 'Save As' dialog is forced to be shown.
         * Normally, the scene's 'default scene file' setting is used as the file name, with the 'Save As'
         * dialog being shown only if this setting is empty. When forceSaveDialog is true, the 'default scene
         * file' setting is ignored and a 'Save As' dialog is used to get the file name
         * @param factory [in] - The export handler factory used to export the scene. If null, a factory
         * will be determined based on the file name's extension
         * @return If the export was successful, true is returned. Otherwise, false is returned
         */
        static bool ExportWithDialog
            (
            bool selected,
            bool forceSaveDialog = false,
            ExporterHandlerFactory* factory = nullptr
            );

        /** Registers an exporter handler factory */
        static void RegisterHandlerFactory(ExporterHandlerFactory& factory);

        /** Removes all registered exporter handler factories */
        static void ClearHandlerFactories();

        /**
         * Gets a file filter string, composed from all registered handler factories, that can be used
         * in save/load file dialogs
         */
        static const wxString& GetFileFilter();

        /**
         * Gets the last file name used for an export.
         */
        static const wxString& GetLastFileName() {return lastInputFileName;}

    protected:
        /**
         * Runs an export of the scene
         * @param fileName [in] - The name of the file that is created during the export
         * @param selected [in] - Indicates whether only the selected objects in the scene should be exported
         * @param sceneSettings [in] - Scene settings
         * @return If the scene export was successful, true is returned. Otherwise, false is returned.
         */
        bool Export
            (
            const wxString& fileName,
            bool selected,
            FinjinSceneSettingsAccessor sceneSettings,
            ExporterFlags exportFlags = ExporterFlags::NONE,
            ExporterHandlerFactory* factory = nullptr
            );

        /**
         * Gets the factory that handles a particular type of scene export, as indicated by the file extension
         * @param fileName [in] - The file name for which a factory is retrieved. The file's extension is used in
         * determining the factory
         * @param extensionIndex [out] - The index of the extension, as determined by the retrieved factory
         * @return A pointer to the found exporter handler factory is returned. If no factories matched the
         * input file's extension, the first registered factory is returned
         */
        static ExporterHandlerFactory* GetFactoryForFileName(const wxString& fileName, int* extensionIndex);

        /**
         * Gets the zero-based index of the file filter string that matches the extension contained in the specified file name.
         * @param fileName [in] - The file name for which a filter string index is retrieved.
         * @return A zero-based index of the matching file filter string is returned. If no factories matched the
         * input file's extension, -1 is returned.
         */
        static int GetFilterIndexForFileName(const wxString& fileName);

        /**
         * Runs the appropriate external viewer, if any, for the exported scene.
         * Called after the export is finished.
         */
        static void RunExternalViewerAfterExport();

    private:
        typedef std::list<ExporterHandlerFactory*> FactoryList;

        /** The registered exporter handler factories */
        static FactoryList factories;

        /** The current file filter string that can be used in save/load file dialogs */
        static wxString fileFilter;

        /** The last file name that was input by the caller */
        static wxString lastInputFileName;
    };

} }
