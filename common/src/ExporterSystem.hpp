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
#include "ObjectAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * Provides top-level access to functions that control large parts of the Finjin system.
     * These functions are typically called by the Finjin plugin entry points.
     */
    class ExporterSystem
    {
    public:
        /** Determines if Finjin has been initialized. */
        static bool IsInitialized();

        /** Initializes Finjin. */
        static void Initialize();

        /** Shuts down Finjin. */
        static void Shutdown();

        /** Gets the full path to the help file. */
        static wxString GetHelpFileName();

        /** Shows the help file. */
        static void ShowHelp();

        /** Called before a new scene is opened. */
        static void OnBeforeOpenScene();

        /** Called after a new scene is opened. */
        static void OnAfterOpenScene();

        /** Called when the scene is reset. */
        static void OnSceneReset();

        /** Called before a new scene is merged. */
        static void OnBeforeMerge();

        /** Called after a new scene is merged. */
        static void OnAfterMerge();

        /**
         * Enables/disables detailed logging.
         * @param enable [in] - Indicates whether to enable or disable detailed logging.
         * @param saveSetting [in] - Indicates whether a save should be performed on the global
         * settings, which is where the detailed logging state is kept.
         */
        static void EnableDetailedLogging(bool enable, bool saveSetting = true);

        static void RunExternalViewerWithLastExportedFile();
        static void RunExternalViewer(const wxString& fileName = wxEmptyString);

        static void SyncSubmeshSettings(const std::vector<ObjectAccessor>& objects);

    private:
        static void RefreshResources();
    };

} }
