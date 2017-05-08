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


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Various system-level utility functions. */
    class SystemUtilities
    {
    public:
#if defined(__WXMSW__)
        /**
         * Gets the string value for the specified registry key.
         * @param key [in] - The key.
         * @param valueName [in] - The name of the value.
         * @param hRootKey [in] - The root key.
         * @return The string value is returned.
         */
        static wxString GetKeyStringValue
            (
            const wxString& key,
            const wxString& valueName,
            HKEY hRootKey = HKEY_LOCAL_MACHINE
            );

        /**
         * Finds the subkey with the specified name.
         * @param baseKey [in] - The key whose subkeys should be searched.
         * @param subkey [in] - The string to search for in subkeys. The search is performed
         * without regard to case sensitivity.
         * @param hRootKey [in] - The root key.
         */
        static wxString FindSubkey
            (
            const wxString& baseKey,
            const wxString& subkey,
            HKEY hRootKey = HKEY_LOCAL_MACHINE
            );

        /**
         * Finds the subkey that starts with the specified string.
         * @param baseKey [in] - The key whose subkeys should be searched.
         * @param startingWith [in] - The string to search for in subkeys. The search is performed
         * without regard to case sensitivity.
         * @param hRootKey [in] - The root key.
         */
        static wxString FindSubkeyStartingWith
            (
            const wxString& baseKey,
            const wxString& startingWith,
            HKEY hRootKey = HKEY_LOCAL_MACHINE
            );
#endif

        /**
         * Shows a particular resource (text file, web page, and so on) in the most appropriate way.
         * @param resourceName [in] - The name of the resource.
         * @param resourceExtension [in] - The typical file extension for the resource, without the leading '.'
         */
        static void ShowResource(const wxString& resourceName, const wxString& resourceExtension);

        /** Shows the specified text file in a system-appropriate way. */
        static void ShowTextFile(const wxString& fileName);

        /** Shows the specified web URL in a system-appropriate way. */
        static void ShowWebPage(const wxString& url);
    };

} }
