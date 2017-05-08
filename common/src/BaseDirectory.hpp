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
#include "ApplicationControls.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * A base directory is used when determining the relative location of other directories.
     * This class provides a lot of common functionality when dealing with base directories,
     * including setting values into user interface controls.
     */
    class BaseDirectory
    {
    public:
        /** The base directory type. */
        enum Type
        {
            APPLICATION,

            CUSTOM,

            /** Indicates the current project directory should be used. */
            PROJECT
        };

        /** Default constructor. */
        BaseDirectory();

        /**
         * Constructor that takes all parameters.
         * @param type [in] - The base directory type.
         * @param customDirectory [in] - The custom directory. This is only used if nonempty and the type is CUSTOM.
         */
        BaseDirectory(Type type, const wxString& customDirectory);

        /**
         * Constructor that assumes the type is CUSTOM.
         * @param customDirectory [in] - The custom directory. This is only used as a custom directory if nonempty.
         */
        BaseDirectory(const wxString& customDirectory);

        /**
         * Constructor that gets its values from user interface controls.
         * @param baseDirectoryTypeChoice [in] - A wxChoice control that was previously initialized
         * with a call to SetGUIData().
         * @param baseDirectoryText [in] - A wxTextCtrl control that was previously initialized
         * with a call to SetGUIData().
         */
        BaseDirectory(wxChoice* baseDirectoryTypeChoice, ApplicationTextCtrl* baseDirectoryText);

        /** Gets the custom directory. This may be empty. */
        const wxString& GetCustomDirectory() const;

        /** Gets the actual base resource directory, whether it be a project, custom, or application directory. */
        const wxString& GetBaseDirectory() const;

        /** Gets the base resource directory type supported by the application. */
        Type GetSupportedType() const;

        void GetGUIData(Type& type, wxString& directory);

        /**
         * Sets the base directory info into the specified user interface controls.
         * @param baseDirectoryTypeChoice [in] - A wxChoice control that is filled with the valid choices
         * and has the relevant choice selected.
         * @param baseDirectoryText [in] - A wxTextCtrl control that is filled with the relevant directory.
         */
        void SetGUIData(wxChoice* baseDirectoryTypeChoice, ApplicationTextCtrl* baseDirectoryText);

        static int GetBaseDirectoryTypeChoice(wxChoice* baseDirectoryTypeChoice);

        static wxString ToString(Type type);
        static Type ParseType(const wxString& s);

        /**
         * The values set into the wxChoice control with a call to SetClientData().
         */
        static const int APPLICATION_DIRECTORY_CHOICE;
        static const int PROJECT_DIRECTORY_CHOICE;
        static const int CUSTOM_DIRECTORY_CHOICE;

    private:
        void Init();

    private:
        Type type;
        wxString customDirectory;
        wxString baseDirectory;
    };

} }
