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
#include "FinjinTimeInterval.hpp"
#include "FinjinColor.hpp"
#include "ObjectAccessor.hpp"
#include "AssetClass.hpp"
#include "finjin/common/WxDataChunkWriter.hpp"
#include "finjin/common/WxChunkName.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * This class provides a lot of default information and functionality related to the application.
     * Depending on which application (Max/Maya/XSI) this code is being compiled with, some functions will be overridden
     * by a derived ApplicationAccessor class. As such, you should never call directly into ApplicationAccessorBase.
     * Instead, always call through ApplicationAccessor.
     */
    class ApplicationAccessorBase
    {
    public:
        static wxString GetUserHomeApplicationDirectory();

        /**
         * Registers the Finjin types that are used to convert from the application types.
         */
        static void RegisterTypes();

        /**
         * Gets the scene author string. This is a setting that is possibly defined by the user in the application.
         * @return The scene author string is returned. If there is no author setting, the empty string is returned
         */
        static wxString GetSceneAuthor();

        static wxString GetLastAuthor();

        /** Indicates whether the application supports projects. */
        static bool SupportsProjects();

        /**
         * Indicates whether the application allows objects to have no material assigned.
         * Some applications always apply a default material, others simply have a 'null' applied.
         * @return If 'null' material assignments are allowed for objects, true is returned. Otherwise,
         * false is returned.
         */
        static bool SupportsNoMaterialAssignment();

        /**
         * Indicates whether the application structures things in a way that requires Finjin to create extra materials.
         * For example, in Maya the "double sided" setting is in the mesh, but Finjin puts this property in a material, so
         * it needs to introduce two versions of the material: one with the double sided property enabled, the other with it disabled.
         */
        static bool NeedsExtraMaterials();

        /**
         * Indicates whether the application supports 3D texture coordinates.
         */
        static bool Supports3DTextureCoordinates();

        /**
         * Indicates whether the application supports note tracks.
         */
        static bool SupportsNoteTracks();

        /**
         * Indicates whether the application provides notifications for material structure and color changes.
         * This is mainly intended for use with XSI, which does not support either.
         */
        static bool SupportsMaterialStructureAndColorChangeNotifications();

        /**
         * Gets the color to be used by static text controls as the foreground (text) color.
         * @param color [out] - The static text color.
         * @return If the color was retrieved, true is returned. Otherwise, false is returned.
         */
        static bool GetStaticTextForegroundColor(wxColor& color);

        /**
         * Gets the scaling factor that converts from the application's internal units to user interface units.
         */
        static float GetInternalToUIScale();

        /**
         * Formats the specified value as a string.
         * @param value [in] - The value, in world units.
         * @return The value formatted in world units is returned. This string may contain non-numeric characters.
         */
        static wxString FormatWorldValue(float value);

        /**
         * Registers a modeless dialog with the application.
         * @param window [in] - The dialog to register.
         */
        static void RegisterModelessDialog(wxWindow* window);

        /**
         * Unregisters a modeless dialog from the application.
         * The dialog should have been previous registered with a call to RegisterModelessDialog().
         * @param window [in] - The dialog to register.
         */
        static void UnregisterModelessDialog(wxWindow* window);

        /**
         * Indicates whether the application supports 'full' names, which contain an objects ancestor hierarchy.
         * @return If the application supports 'full' names, true is returned. Otherwise, false is returned.
         */
        static bool UsesFullNames();

        /** Gets the application's online or local HTML help index.html. */
        static wxString GetApplicationHelpLocation();

        /** Enables/disables the application's viewport renderer. */
        static void EnableShortcutKeys(bool enable = true);

        /**
         * Enables/disables functionality associated with exporting scene data.
         * @param enable [in] - Indicates whether to enable or disable export functionality.
         */
        static void EnableExport(bool enable = true);

        /** Determines whether exporting is enabled. */
        static bool IsExportEnabled();

        /** Determines whether the application is running as some type of slave, typically on a network. */
        static bool InQuietMode();

    protected:
        static bool exportEnabled;
    };

} }
