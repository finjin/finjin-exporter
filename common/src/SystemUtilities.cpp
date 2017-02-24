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
#include "SystemUtilities.hpp"
#include "RegistryKeyEnumerator.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
#if defined(__WXMSW__)
wxString SystemUtilities::GetKeyStringValue(const wxString& key, const wxString& valueName, HKEY hRootKey)
{
    wxString value;

    //Open the registry key
    TCHAR valueChars[1024];
    HKEY hKey = nullptr;
    DWORD dataType;
    DWORD dataSize;
    auto result = RegOpenKeyEx(hRootKey, key.wx_str(), 0, KEY_READ, &hKey);
    if (result == ERROR_SUCCESS)
    {
        //Query the value
        dataSize = sizeof(valueChars);
        result = RegQueryValueEx(hKey, valueName.wx_str(), nullptr, &dataType, (BYTE*)valueChars, &dataSize);
        if (result == ERROR_SUCCESS)
        {
            //Success
            value = valueChars;
        }
        
        //Close key
        RegCloseKey(hKey);
    }

    return value;
}

wxString SystemUtilities::FindSubkey(const wxString& baseKey, const wxString& desiredSubkey, HKEY hRootKey)
{
    //The returned subkey
    wxString subkey;

    //Enumerate keys
    RegistryKeyEnumerator keyEnumerator(baseKey, hRootKey);
    if (keyEnumerator.HasKey())
    {
        do
        {
            auto subkeyName = keyEnumerator.GetSubkey();
            
            //Key opened. See if it matches desiredSubkey
            if (_tcsicmp(subkeyName.wx_str(), desiredSubkey.wx_str()) == 0)
            {
                //The subkey has been found
                subkey = subkeyName;
            }
        }while (keyEnumerator.MoveNextKey() && subkey.empty());
    }

    return subkey;
}

wxString SystemUtilities::FindSubkeyStartingWith(const wxString& baseKey, const wxString& startingWith, HKEY hRootKey)
{
    wxString startingWithLower = startingWith.Lower();

    //The returned subkey
    wxString subkey;

    //Enumerate keys
    RegistryKeyEnumerator keyEnumerator(baseKey, hRootKey);
    if (keyEnumerator.HasKey())
    {
        do
        {
            wxString subkeyName = keyEnumerator.GetSubkey();
            wxString subkeyNameLower = subkeyName.Lower();
                        
            //Key opened. See if it has 'startingWith' at the beginning of its name
            if (_tcsstr(subkeyNameLower.wx_str(), startingWithLower.wx_str()) == subkeyNameLower.wx_str())
            {
                //The subkey has been found
                subkey = subkeyName;
            }
        }while (keyEnumerator.MoveNextKey() && subkey.empty());
    }

    return subkey;
}

#endif

void SystemUtilities::ShowResource(const wxString& resourceName, const wxString& resourceExtension)
{
    auto fileType = wxTheMimeTypesManager->GetFileTypeFromExtension(resourceExtension);
    if (fileType != nullptr)
    {
        auto command = fileType->GetOpenCommand(resourceName);
        if (!command.empty())
            wxExecute(command);
    }
}

void SystemUtilities::ShowTextFile(const wxString& fileName)
{
    ShowResource(fileName, wxT("txt"));
}

void SystemUtilities::ShowWebPage(const wxString& url)
{
    ShowResource(url, wxT("html"));
}
