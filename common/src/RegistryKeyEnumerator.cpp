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


//Includes----------------------------------------------------------------------
#include "FinjinPrecompiled.hpp"

#if defined(__WXMSW__)

#include "RegistryKeyEnumerator.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
RegistryKeyEnumerator::RegistryKeyEnumerator(const wxString& baseKey, HKEY hRootKey)
{
    this->hRootKey = hRootKey;
    this->hKey = nullptr;
    this->subkeyIndex = 0;
    this->baseKey = baseKey;

    //Open the registry key
    auto result = RegOpenKeyEx(this->hRootKey, this->baseKey.wx_str(), 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &this->hKey);
    if (result == ERROR_SUCCESS)
        GetIndexedSubkey();
}

RegistryKeyEnumerator::~RegistryKeyEnumerator()
{
    if (this->hKey != nullptr)
        RegCloseKey(this->hKey);
}

bool RegistryKeyEnumerator::HasKey() const
{
    return !this->subkey.empty();
}

bool RegistryKeyEnumerator::MoveNextKey()
{
    this->subkeyIndex++;
    GetIndexedSubkey();
    return HasKey();
}

wxString RegistryKeyEnumerator::GetFullKey()
{
    return HasKey() ? (this->baseKey + wxT("\\") + this->subkey) : wxEmptyString;
}

wxString RegistryKeyEnumerator::GetSubkey()
{
    return this->subkey;
}

void RegistryKeyEnumerator::GetIndexedSubkey()
{
    const int MAX_SUBKEY_LENGTH = 1024;
    TCHAR subkeyName[MAX_SUBKEY_LENGTH];

    //Get the subkey at the current index
    long result = RegEnumKey(this->hKey, this->subkeyIndex, subkeyName, MAX_SUBKEY_LENGTH);
    this->subkey = (result == ERROR_SUCCESS) ? subkeyName : wxEmptyString;
}

#endif
