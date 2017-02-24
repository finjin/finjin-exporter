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
#include "FileVersionInfo.hpp"


//Implementation---------------------------------------------------------------
FileVersionInfo::FileVersionInfo()
{
    Reset();
}

FileVersionInfo::~FileVersionInfo()
{
}

BOOL FileVersionInfo::GetTranslationId(void* dataBytes, UINT blockSize, WORD langId, DWORD &id, BOOL primaryEnough)
{
    WORD* dataWords;

    for (dataWords = (WORD*)dataBytes; (BYTE*)dataWords < ((BYTE*)dataBytes)+blockSize; dataWords+=2)
    {
        if (*dataWords == langId)
        {
            id = *((DWORD*)dataWords);
            return TRUE;
        }
    }

    if (!primaryEnough)
        return FALSE;

    for (dataWords = (WORD*)dataBytes; (BYTE*)dataWords < ((BYTE*)dataBytes)+blockSize; dataWords+=2)
    {
        if (((*dataWords)&0x00FF) == (langId&0x00FF))
        {
            id = *((DWORD*)dataWords);
            return TRUE;
        }
    }

    return FALSE;
}

bool FileVersionInfo::Create(HMODULE hModule)
{
    TCHAR path[_MAX_PATH + 1];
    GetModuleFileName(hModule, path, _MAX_PATH);    
    return Create(path);
}

bool FileVersionInfo::Create(const TCHAR* fileName)
{
    Reset();

    auto result = false;

    DWORD handle;
    DWORD fileVersionInfoSize = GetFileVersionInfoSize(fileName, &handle);
    if (fileVersionInfoSize > 0)
    {
        BYTE* dataBytes = new BYTE[fileVersionInfoSize];    
        if (GetFileVersionInfo(fileName, handle, fileVersionInfoSize, dataBytes))
        {
            result = true;

            //Get default information
            void* infoPointer;
            UINT infoLength;
            if (VerQueryValue(dataBytes, wxT("\\"), &infoPointer, &infoLength))
            {
                if (infoLength == sizeof(this->fileInfo))
                    memcpy(&this->fileInfo, infoPointer, infoLength);
            }

            //Find best matching language and codepage
            VerQueryValue(dataBytes, wxT("\\VarFileInfo\\Translation"), &infoPointer, &infoLength);
            
            DWORD langCode = 0;
            if (!GetTranslationId(infoPointer, infoLength, GetUserDefaultLangID(), langCode, FALSE))
            {
                if (!GetTranslationId(infoPointer, infoLength, GetUserDefaultLangID(), langCode, TRUE))
                {
                    if (!GetTranslationId(infoPointer, infoLength, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), langCode, TRUE))
                    {
                        if (!GetTranslationId(infoPointer, infoLength, MAKELANGID(LANG_ENGLISH, SUBLANG_NEUTRAL), langCode, TRUE))
                            // use the first one we can get
                            langCode = *((DWORD*)infoPointer);
                    }
                }
            }
            

            TCHAR subBlockChars[500];
            _stprintf(subBlockChars, wxT("\\StringFileInfo\\%04X%04X\\"), langCode&0x0000FFFF, (langCode&0xFFFF0000)>>16);
            wxString subBlock = subBlockChars;
            
            //Get string table
            if (VerQueryValue(dataBytes, (TCHAR*)(subBlock+wxT("CompanyName")).wx_str(), &infoPointer, &infoLength))
                this->companyName = (TCHAR*)infoPointer;
            if (VerQueryValue(dataBytes, (TCHAR*)(subBlock+wxT("FileDescription")).wx_str(), &infoPointer, &infoLength))
                this->fileDescription = (TCHAR*)infoPointer;
            if (VerQueryValue(dataBytes, (TCHAR*)(subBlock+wxT("FileVersion")).wx_str(), &infoPointer, &infoLength))
                this->fileVersion = (TCHAR*)infoPointer;
            if (VerQueryValue(dataBytes, (TCHAR*)(subBlock+wxT("InternalName")).wx_str(), &infoPointer, &infoLength))
                this->internalName = (TCHAR*)infoPointer;
            if (VerQueryValue(dataBytes, (TCHAR*)(subBlock+wxT("LegalCopyright")).wx_str(), &infoPointer, &infoLength))
                this->legalCopyright = (TCHAR*)infoPointer;
            if (VerQueryValue(dataBytes, (TCHAR*)(subBlock+wxT("OriginalFileName")).wx_str(), &infoPointer, &infoLength))
                this->originalFileName = (TCHAR*)infoPointer;
            if (VerQueryValue(dataBytes, (TCHAR*)(subBlock+wxT("ProductName")).wx_str(), &infoPointer, &infoLength))
                this->productName = (TCHAR*)infoPointer;
            if (VerQueryValue(dataBytes, (TCHAR*)(subBlock+wxT("ProductVersion")).wx_str(), &infoPointer, &infoLength))
                this->productVersion = (TCHAR*)infoPointer;
            if (VerQueryValue(dataBytes, (TCHAR*)(subBlock+wxT("Comments")).wx_str(), &infoPointer, &infoLength))
                this->comments = (TCHAR*)infoPointer;
            if (VerQueryValue(dataBytes, (TCHAR*)(subBlock+wxT("LegalTrademarks")).wx_str(), &infoPointer, &infoLength))
                this->legalTrademarks = (TCHAR*)infoPointer;
            if (VerQueryValue(dataBytes, (TCHAR*)(subBlock+wxT("PrivateBuild")).wx_str(), &infoPointer, &infoLength))
                this->privateBuild = (TCHAR*)infoPointer;
            if (VerQueryValue(dataBytes, (TCHAR*)(subBlock+wxT("SpecialBuild")).wx_str(), &infoPointer, &infoLength))
                this->specialBuild = (TCHAR*)infoPointer;
        }

        delete [] dataBytes;
    }

    return result;
}

WORD FileVersionInfo::GetFileVersion(int index) const
{
    if (index == 0)
        return (WORD)(this->fileInfo.dwFileVersionLS & 0x0000FFFF);
    else if (index == 1)
        return (WORD)((this->fileInfo.dwFileVersionLS & 0xFFFF0000) >> 16);
    else if (index == 2)
        return (WORD)(this->fileInfo.dwFileVersionMS & 0x0000FFFF);
    else if (index == 3)
        return (WORD)((this->fileInfo.dwFileVersionMS & 0xFFFF0000) >> 16);
    else
        return 0;
}

WORD FileVersionInfo::GetProductVersion(int index) const
{
    if (index == 0)
        return (WORD)(this->fileInfo.dwProductVersionLS & 0x0000FFFF);
    else if (index == 1)
        return (WORD)((this->fileInfo.dwProductVersionLS & 0xFFFF0000) >> 16);
    else if (index == 2)
        return (WORD)(this->fileInfo.dwProductVersionMS & 0x0000FFFF);
    else if (index == 3)
        return (WORD)((this->fileInfo.dwProductVersionMS & 0xFFFF0000) >> 16);
    else
        return 0;
}

DWORD FileVersionInfo::GetFileFlagsMask() const
{
    return this->fileInfo.dwFileFlagsMask;
}

DWORD FileVersionInfo::GetFileFlags() const
{
    return this->fileInfo.dwFileFlags;
}

DWORD FileVersionInfo::GetFileOs() const
{
    return this->fileInfo.dwFileOS;
}

DWORD FileVersionInfo::GetFileType() const
{
    return this->fileInfo.dwFileType;
}

DWORD FileVersionInfo::GetFileSubtype() const
{
    return this->fileInfo.dwFileSubtype;
}

FILETIME FileVersionInfo::GetFileDate() const
{
    FILETIME ft;
    ft.dwLowDateTime = this->fileInfo.dwFileDateLS;
    ft.dwHighDateTime = this->fileInfo.dwFileDateMS;
    return ft;
}

const wxString& FileVersionInfo::GetCompanyName() const
{
    return this->companyName;
}

const wxString& FileVersionInfo::GetFileDescription() const
{
    return this->fileDescription;
}

const wxString& FileVersionInfo::GetFileVersion() const
{
    return this->fileVersion;
}

const wxString& FileVersionInfo::GetInternalName() const
{
    return this->internalName;
}

const wxString& FileVersionInfo::GetLegalCopyright() const
{
    return this->legalCopyright;
}

const wxString& FileVersionInfo::GetOriginalFileName() const
{
    return this->originalFileName;
}

const wxString& FileVersionInfo::GetProductName() const
{
    return this->productName;
}

const wxString& FileVersionInfo::GetProductVersion() const
{
    return this->productVersion;
}

const wxString& FileVersionInfo::GetComments() const
{
    return this->comments;
}

const wxString& FileVersionInfo::GetLegalTrademarks() const
{
    return this->legalTrademarks;
}

const wxString& FileVersionInfo::GetPrivateBuild() const
{
    return this->privateBuild;
}

const wxString& FileVersionInfo::GetSpecialBuild() const
{
    return this->specialBuild;
}

void FileVersionInfo::Reset()
{
    ZeroMemory(&this->fileInfo, sizeof(this->fileInfo));
    this->companyName.clear();
    this->fileDescription.clear();
    this->fileVersion.clear();
    this->internalName.clear();
    this->legalCopyright.clear();
    this->originalFileName.clear();
    this->productName.clear();
    this->productVersion.clear();
    this->comments.clear();
    this->legalTrademarks.clear();
    this->privateBuild.clear();
    this->specialBuild.clear();
}
