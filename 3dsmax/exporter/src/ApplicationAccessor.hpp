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
#include "ApplicationAccessorBase.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Access to application-level (3DS Max, Maya, etc) functionality. */
    class ApplicationAccessor : public ApplicationAccessorBase
    {
    public:
        /** Gets the application's currently configured up axis. */
        static UpAxis GetUpAxis();

        /** Gets the global animation range. */
        static FinjinTimeInterval GetAnimationRange();

        /**
         * Gets the application's main window.
         * @param modal [in] - Indicates whether the window will be used as the parent of a modal dialog.
         */
        static WXWidget GetMainWindow(bool modal = true);

        /** Registers a modeless dialog with the application. */
        static void RegisterModelessDialog(wxWindow* dialog);

        /** Unregisters a previously registered modeless dialog. */
        static void UnregisterModelessDialog(wxWindow* dialog);

        /** Gets a string indicating the version of the application. */
        static wxString GetApplicationVersion();

        /** Gets the Finjin home directory. */
        static wxString GetFinjinHomeDirectory(bool writable);

        /** Gets the application root directory. */
        static wxString GetApplicationDirectory();

        /** Gets the application's help directory. */
        static wxString GetHelpDirectory();

        /** Gets the application's current project directory, if there is one. */
        static wxString GetCurrentProjectDirectory();

        /** Sets the application's current project directory, returning true if successful. */
        static bool SetCurrentProjectDirectory(wxString directory);

        /** Gets the application's online or local HTML help location. */
        static wxString GetApplicationHelpLocation();

        /**
         * Gets the background color of the application's windows.
         * @param color [out] - The application's window background color.
         * @return If the application has a custom background color, true is returned.
         * Otherwise, false is returned.
         */
        static bool GetWindowBackgroundColor(wxColor& color);

        /**
         * Gets the background color of the application's text controls.
         * @param color [out] - The application's text control background color.
         * @return If the application has a custom text control background color, true is returned.
         * Otherwise, false is returned.
         */
        static bool GetTextBackgroundColor(wxColor& color);

        /**
         * Gets the foreground color of the application's text controls.
         * This is the color of the text entered into text boxes.
         * @param color [out] - The application's text color.
         * @return If the application has a custom text color, true is returned.
         * Otherwise, false is returned.
         */
        static bool GetTextForegroundColor(wxColor& color);

        /**
         * Gets the foreground color of the application's static text controls.
         * @param color [out] - The application's static text color.
         * @return If the application has a custom static text color, true is returned.
         * Otherwise, false is returned.
         */
        static bool GetStaticTextForegroundColor(wxColor& color);

        /** Gets the scaling factor that converts from internal units to user interface units. */
        static float GetInternalToUIScale();

        /** Gets the scaling factor that converts from internal units to meters. */
        static float GetMasterScaleMeters();

        /**
         * Gets the scene scaling factor for the specified unit.
         * @param scaleUnit [in/out] - A Finjin-specific name of a unit. This will be
         * set to an empty string if it is not recognized.
         * @return The scale corresponding to the specified unit is returned. If the unit
         * was not recognized, 1 is returned.
         */
        static float GetScale(wxString& scaleUnit);

        /** Gets the default unit name. */
        static wxString GetDefaultUnitType(bool abbreviate = false);

        /** Converts the internal unit value to a displayable string. */
        static wxString FormatWorldValue(float value);

        /** Writes a message to the application's equivalent of a log window. */
        static void LogMessage(const wxChar* format, ...);

        /** Writes a message to the application's equivalent of a log window. */
        static void LogMessage(const wxString& message);

        /** Indicates whether the application supports projects. */
        static bool SupportsProjects();

        /** Indicates whether the application supports nested materials. */
        static bool SupportsNestedMaterials();

        /** Indicates whether the application supports note tracks on objects. */
        static bool SupportsNoteTracks();

        /** Gets the author of the currently loaded scene. */
        static wxString GetSceneAuthor();
        static wxString GetLastAuthor();

        /** Gets the environment ambient color at the specified time. */
        static FinjinColor GetAmbientLightColor(TimeAccessor time = TimeAccessor());

        /** Sets the environment ambient color at the specified time. */
        static bool SetAmbientLightColor(FinjinColor value, TimeAccessor time = TimeAccessor());

        /** Gets the environment background color at the specified time. */
        static FinjinColor GetBackgroundColor(TimeAccessor time = TimeAccessor());

        /** Sets the environment background color at the specified time. */
        static bool SetBackgroundColor(FinjinColor value, TimeAccessor time = TimeAccessor());

        /** Gets the environment linear fog color at the specified time. */
        static bool GetLinearFog(float& start, float& end, FinjinColor& color, float range, TimeAccessor time = TimeAccessor());

        /** Sets the environment linear fog color at the specified time. */
        static bool SetLinearFog(float start, float end, FinjinColor color, float range, TimeAccessor time = TimeAccessor());

        /** Enables/disables the application's shortcut keys. */
        static void EnableShortcutKeys(bool enable = true);

        /** Indicates whether the application supports full (long) names. */
        static bool UsesFullNames();

        /** Determines whether the application is running as some type of slave, typically on a network. */
        static bool InQuietMode();

    public:
        /** The application's full name. */
        static const wxString APPLICATION_LONG_NAME;

        /** The application's short name. This may be the same as the long name. */
        static const wxString APPLICATION_SHORT_NAME;

        /** The application's name for the window that lets you select materials. */
        static const wxString MATERIAL_SELECTOR_WINDOW_NAME;

        /** The application's name for bones. */
        static const wxString BONE_NAME;

        /** The application's name for a texture coordinate set. */
        static const wxString TEXTURE_COORDINATE_SET_NAME;
    };

} }
