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


//Classes----------------------------------------------------------------------
class FileVersionInfo
{
public:
    FileVersionInfo();
    virtual ~FileVersionInfo();

public:
    bool Create(HMODULE hModule = nullptr);
    bool Create(const TCHAR* fileName);

    WORD GetFileVersion(int index) const;
    WORD GetProductVersion(int index) const;
    DWORD GetFileFlagsMask() const;
    DWORD GetFileFlags() const;
    DWORD GetFileOs() const;
    DWORD GetFileType() const;
    DWORD GetFileSubtype() const;
    FILETIME GetFileDate() const;

    const wxString& GetCompanyName() const;
    const wxString& GetFileDescription() const;
    const wxString& GetFileVersion() const;
    const wxString& GetInternalName() const;
    const wxString& GetLegalCopyright() const;
    const wxString& GetOriginalFileName() const;
    const wxString& GetProductName() const;
    const wxString& GetProductVersion() const;
    const wxString& GetComments() const;
    const wxString& GetLegalTrademarks() const;
    const wxString& GetPrivateBuild() const;
    const wxString& GetSpecialBuild() const;

protected:
    virtual void Reset();
    BOOL GetTranslationId(void* data, UINT blockSize, WORD langId, DWORD &id, BOOL primaryEnough);

private:
    VS_FIXEDFILEINFO fileInfo;

    wxString companyName;
    wxString fileDescription;
    wxString fileVersion;
    wxString internalName;
    wxString legalCopyright;
    wxString originalFileName;
    wxString productName;
    wxString productVersion;
    wxString comments;
    wxString legalTrademarks;
    wxString privateBuild;
    wxString specialBuild;
};
