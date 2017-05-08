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

#if defined(__WXMSW__)


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Enumerates a registry key's subkeys. */
    class RegistryKeyEnumerator
    {
    public:
        /** Initializes the enumerator and starts enumeration */
        RegistryKeyEnumerator(const wxString& baseKey, HKEY hRootKey = HKEY_LOCAL_MACHINE);
        ~RegistryKeyEnumerator();

        /** Determines whether there is a key to get. */
        bool HasKey() const;

        /** Moves to the next key. */
        bool MoveNextKey();

        /** Gets the full 'path' to the current subkey. */
        wxString GetFullKey();

        /** Gets the current subkey. */
        wxString GetSubkey();

    private:
        /** Gets the data for the currently indexed subkey. */
        void GetIndexedSubkey();

    private:
        HKEY hRootKey;
        HKEY hKey;
        DWORD subkeyIndex;
        wxString baseKey;
        wxString subkey;
    };

} }

#endif
