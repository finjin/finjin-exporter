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
#include "FileUtilities.hpp"
#if !defined(FINJIN_EXPORTER_INSTALLER)
    #include "FinjinGlobalSettings.hpp"    
    #include "finjin/common/WxByteBuffer.hpp"
#endif

using namespace Finjin::Exporter;


//Local functions--------------------------------------------------------------
static bool EndsWithNoCase(const wxString& s1, const wxString& s2)
{
    auto s1Length = s1.length();
    auto s2Length = s2.length();
    if (s2Length > s1Length)
        return false;
    else if (s2Length == s1Length)
        return s1.IsSameAs(s2, false);
    else
    {
        wxString end = s1.Mid(s1Length - s2Length - 1);
        return end.IsSameAs(s2, false);
    }
}


//Implementation---------------------------------------------------------------
void FileUtilities::UnifySeparators(wxString& path)
{
    path.Replace(wxT("\\"), wxT("/"));
}

void FileUtilities::RemoveLeadingSeparators(wxString& path)
{
    while (path.StartsWith(wxT("\\")) || path.StartsWith(wxT("/")))
        path.Remove(0, 1);
}

void FileUtilities::WriteFileTo(wxFile& inFile, wxFile& outFile)
{
    uint8_t tempBuffer[1024];
    size_t readCount = 0;
    while ((readCount = inFile.Read(tempBuffer, sizeof(tempBuffer))) > 0)
        outFile.Write(tempBuffer, readCount);
}

bool FileUtilities::IsSeparator(wxChar c)
{
    return c == wxT('\\') || c == wxT('/');
}

bool FileUtilities::EndsWithSeparator(const wxString& path)
{
    return !path.empty() && IsSeparator(path[path.length() - 1]);
}

void FileUtilities::EnsureTrailingPathSeparator(wxString& path, wxChar separatorChar)
{
    if (!path.empty())
    {
        auto lastChar = path[path.length() - 1];
        if (!IsSeparator(lastChar))
        {
            //Figure out which separator character to use
            auto bestSeparatorChar = separatorChar;
            for (size_t i = 0; i < path.size(); i++)
            {
                if (IsSeparator(path[i]))
                {
                    bestSeparatorChar = path[i];
                    break;
                }
            }

            //Append the character
            path += bestSeparatorChar;
        }
    }        
    else
        path = separatorChar;
}

void FileUtilities::EnsureNoTrailingPathSeparator(wxString& path)
{
    if (!path.empty())
    {
        auto lastIndex = path.length() - 1;
        if (IsSeparator(path[lastIndex]))
            path.Remove(lastIndex);
    }    
}

wxString FileUtilities::WithTrailingPathSeparator(const wxString& path, wxChar separatorChar)
{
    if (EndsWithSeparator(path))
        return path;
    else
    {
        auto newPath = path;
        EnsureTrailingPathSeparator(newPath, separatorChar);
        return newPath;
    }
}

wxString FileUtilities::WithoutTrailingPathSeparator(const wxString& path)
{
    if (!EndsWithSeparator(path))
        return path;
    else
    {
        auto newPath = path;
        EnsureNoTrailingPathSeparator(newPath);
        return newPath;
    }
}

wxString FileUtilities::JoinPath(const wxString& path1, const wxString& path2)
{
    //Do nothing if path2 is empty
    if (path2.empty())
        return path1;

    //If path2 is an absolute path, it supercedes path1
    if (wxIsAbsolutePath(path2))
        return path2;

    //Path2 is not absolute
    wxString joined(path1);
    EnsureTrailingPathSeparator(joined);
    
    //Skip leading separators in path2
    size_t charIndex = 0;
    while (charIndex < path2.length() && IsSeparator(path2[charIndex]))
        charIndex++;

    //If not at the end of path2, append it
    if (charIndex < path2.length())
        joined += path2.substr(charIndex);

    //Remove relative components
    joined = MakeFullPath(joined);
    
    return joined;
}

wxString FileUtilities::MakeFullPath(const wxString& path)
{
    return path;
    /*wxFileName pathFileName(path);
    pathFileName.Normalize();
    return pathFileName.GetFullPath();*/
}

wxString FileUtilities::PrettyPath(const wxString& path)
{
    wxFileName filename(path);
    return filename.GetFullPath();
}

wxString FileUtilities::GetFileName(const wxString& path, bool includeExtension)
{
    wxFileName pathFileName(path);
    if (includeExtension)
        return pathFileName.GetFullName();
    else
    {
        auto result = pathFileName.GetName();
        auto foundAt = result.rfind(wxT('.'));
        while (foundAt != wxString::npos)
        {
            result = result.substr(0, foundAt);
            foundAt = result.rfind(wxT('.'));
        }
        return result;
    }
}

wxString FileUtilities::GetFileNameNoExtension(const wxString& path)
{
    return GetFileName(path, false);
}

int FileUtilities::GetFileNameOffset(const wxString& path)
{
    auto fileName = GetFileName(path);
    if (fileName.empty())
        return 0;
    else
        return (int)path.length() - (int)fileName.length();
}

bool FileUtilities::HasExtension(const wxString& path)
{
    return GetExtensionOffset(path, true) >= 0;
}

int FileUtilities::GetExtensionOffset(const wxString& path, bool includeDot)
{
    auto extension = GetExtension(path, includeDot);
    if (extension.empty())
        return -1;
    else
        return (int)path.length() - (int)extension.length();
}

wxString FileUtilities::GetExtension(const wxString& path, bool includeDot)
{
    wxFileName pathFileName(path);
    auto ext = pathFileName.GetExt();
    return (includeDot && !ext.empty()) ? (wxT(".") + ext) : ext;
}

wxString FileUtilities::RemoveExtension(const wxString& path)
{
    auto extension = GetExtension(path, true);
    if (!extension.empty())
        return path.substr(0, path.length() - extension.length());
    else
        return path;
}

wxString FileUtilities::RemoveFileName(const wxString& path)
{
    for (size_t i = path.length(); i != (size_t)-1; i--)
    {
        if (IsSeparator(path[i]))
            return path.substr(0, i);
    }

    return path;
}

void FileUtilities::MakeExtensionLowercase(wxString& path)
{
    auto extLowercase = GetExtension(path, true);
    extLowercase.MakeLower();

    path = RemoveExtension(path) + extLowercase;
}

wxString FileUtilities::GetDirectoryPath(const wxString& path)
{
    wxFileName pathFileName(path);    
    return pathFileName.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
}

bool FileUtilities::CreateDirectoryRecursive(const wxString& path)
{
    if (wxDirExists(path))
        return true;
    else
        return wxFileName::Mkdir(path, 0777, wxPATH_MKDIR_FULL);
}

bool FileUtilities::CreateFileDirectoryRecursive(const wxString& path)
{
    auto directory = GetDirectoryPath(path);
    if (directory.empty())
        return false;
    else
        return CreateDirectoryRecursive(directory);
}

wxString FileUtilities::CreateExportDirectory(const wxString& defaultExportPath, const wxString& subdirectory)
{
    wxString outputPath;

    if (subdirectory.empty())
        outputPath = defaultExportPath;
    else
        outputPath = JoinPath(defaultExportPath, subdirectory);

    CreateDirectoryRecursive(outputPath);

    return outputPath;
}

bool FileUtilities::CopyDirectory(const wxString& sourceDirectory, const wxString& destinationDirectory)
{
    //Create the destination directory if necessary
    CreateDirectoryRecursive(destinationDirectory);
                    
    //Copy files first
    wxArrayString files;                    
    wxDir::GetAllFiles(sourceDirectory, &files, wxEmptyString, wxDIR_FILES);
    for (size_t fileIndex = 0; fileIndex < files.size(); fileIndex++)
    {
        wxString destinationFileName = destinationDirectory + wxFileName(files[fileIndex]).GetFullName();
        if (!CopyFileEnsureReplaceable(files[fileIndex], destinationFileName))
            return false;
    }

    //Recurse into subdirectories
    wxArrayString subdirs;
    GetSubdirectories(sourceDirectory, subdirs, false);
    wxString subdirectory;
    for (size_t dirIndex = 0; dirIndex < subdirs.size(); dirIndex++)
    {
        if (!CopyDirectory(sourceDirectory + subdirs[dirIndex], destinationDirectory + subdirs[dirIndex]))
            return false;
    }

    return true;
}


bool FileUtilities::FileOrDirectoryExists(const wxString& path)
{
    return wxFileExists(path) || wxDirExists(path);
}

void FileUtilities::GetSubdirectories(const wxString& directory, wxArrayString& subdirs, bool full, bool includeTrailing)
{
    class Traverser : public wxDirTraverser
    {
    public:
        Traverser(wxArrayString& sub, const wxString& dir, bool full, bool includeTrailing) : subdirs(sub), directory(dir) 
        {
            this->full = full;
            this->includeTrailing = includeTrailing;
        }
        wxDirTraverseResult OnFile(const wxString& WXUNUSED(fileName)) override {return wxDIR_CONTINUE;}
        wxDirTraverseResult OnDir(const wxString& foundDirectory) override
        {
            wxString name;
            if (this->full)
                name = foundDirectory; 
            else
                name = foundDirectory.Mid(directory.size());
            if (this->includeTrailing)
                name += wxT("/");
            this->subdirs.push_back(name);
            return wxDIR_IGNORE;
        }

    private:
        wxArrayString& subdirs;
        const wxString& directory;
        bool full;
        bool includeTrailing;
    };

    Traverser traverser(subdirs, directory, full, includeTrailing);
    wxDir dir(directory);
    dir.Traverse(traverser);
}

void FileUtilities::EnsureExtension(wxString& fileName, const wxString& extension)
{
    if (extension.empty())
        return;

    //Ensure the extension starts with a dot
    wxString extensionWithDot;
    if (extension[0] != wxT('.'))
    {
        extensionWithDot = wxT(".");
        extensionWithDot += extension;
    }
    else
        extensionWithDot = extension;

    if (!fileName.EndsWith(extensionWithDot))
    {
        //The extensions don't match

        if (EndsWithNoCase(fileName, extensionWithDot))
        {
            //The extensions are the same, but their case sensitivity differs

            //Replace the extension
            wxString newFileName = fileName.SubString(0, fileName.length() - extensionWithDot.length());
            newFileName += extensionWithDot;
            fileName = newFileName;
        }
        else
        {
            //Extensions aren't the same. Just append the new extension
            fileName += extensionWithDot;
        }
    }
}

wxString FileUtilities::CreateFileName(const wxString& name, const wxString& extension)
{
    wxString fileName = name;
    EnsureExtension(fileName, extension);
    return fileName;
}

void FileUtilities::MakeForwardSlashes(wxString& path)
{
    wxFileName pathFileName(path, wxPATH_UNIX);
    path = pathFileName.GetFullPath();
}

void FileUtilities::MakeNativeSlashes(wxString& text)
{
    auto separator = wxFileName::GetPathSeparator();
    auto size = text.length();
    for (size_t i = 0; i < size; i++)
    {
        if (IsSeparator(text[i]))
            text[i] = separator;
    }
}

bool FileUtilities::ReadTextFile(const wxString& fileName, wxString& text)
{
    wxFile file(fileName);
    if (file.IsOpened())
    {
        auto fileLength = file.Length();
        if (fileLength > 0)
        {
            std::vector<char> bytes;
            bytes.resize(fileLength);
            file.Read(&bytes[0], bytes.size());
            text = wxString::FromUTF8(&bytes[0], bytes.size());
        }
        else
            text.clear();
    }

    return file.IsOpened();
}

bool FileUtilities::WriteTextFile(const wxString& fileName, const wxString& text)
{
    wxFile file(fileName, wxFile::write);
    if (file.IsOpened() && !text.empty())
    {
        auto textBuffer = text.ToUTF8();
        file.Write(textBuffer.data(), textBuffer.length());
    }

    return file.IsOpened();
}

#if !defined(FINJIN_EXPORTER_INSTALLER)
bool FileUtilities::ReadBinaryFile(const wxString& fileName, WxByteBuffer& bytes)
{
    bytes.clear();

    if (!wxFileExists(fileName))
        return false;

    wxFile file;
    if (file.Open(fileName))
    {
        bytes.resize(file.Length());
        file.Read(bytes.data(), bytes.size());
    }

    return file.IsOpened();
}
#endif

bool FileUtilities::ReadBinaryFile(const wxString& fileName, std::vector<uint8_t>& bytes)
{
    bytes.clear();

    if (!wxFileExists(fileName))
        return false;

    wxFile file;
    if (file.Open(fileName))
    {
        bytes.resize(file.Length());
        file.Read(bytes.data(), bytes.size());
    }

    return file.IsOpened();
}

bool FileUtilities::EnsureFileReplaceable(const wxString& fileName)
{
    auto result = false;

#if defined(__WXMSW__)
    auto attributes = GetFileAttributesW(fileName.wchar_str());
    if (attributes != INVALID_FILE_ATTRIBUTES)        
    {
        if (attributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            //File is a directory
        }
        else if (attributes & FILE_ATTRIBUTE_READONLY)
        {
            //File has read only attribute. Remove it
            attributes &= ~FILE_ATTRIBUTE_READONLY;
            result = SetFileAttributesW(fileName.wchar_str(), attributes) != FALSE;
        }
        else
        {
            //File doesn't have read only attribute
            result = true;
        }
    }
    else
    {
        //File doesn't exist
        result = true;
    }
#elif defined(__WXMAC__)
    //TODO: Implement this?
    result = true;
#else
    int chmodResult = chmod(fileName.wx_str(), S_IREAD | S_IWRITE);
    if (chmodResult == 0 || chmodResult == ENOENT)
    {
        //Permissions were changed or the file didn't exist
        result = true;
    }
#endif

    return result;
}

bool FileUtilities::CopyFileEnsureReplaceable(const wxString& sourceFileName, const wxString& destinationFileName)
{
    if (EnsureFileReplaceable(destinationFileName))
        return wxCopyFile(sourceFileName, destinationFileName);
    else
        return false;
}

bool FileUtilities::DeleteDirectory(const wxString& path)
{
    //First remove all files in this directory
    wxArrayString files;
    wxDir::GetAllFiles(path, &files, wxEmptyString, wxDIR_FILES | wxDIR_HIDDEN);
    for (size_t i = 0; i < files.size(); i++)
    {
        if (!wxRemoveFile(files[i]))
            return false;
    }

    //Call DeleteDirectory on all subdirectories
    auto pathSlash = path;
    EnsureTrailingPathSeparator(pathSlash);
    wxArrayString subdirs;
    GetSubdirectories(pathSlash, subdirs);
    for (size_t i = 0; i < subdirs.size(); i++)
    {
        if (!DeleteDirectory(subdirs[i]))
            return false;
    }

    //Delete the specified directory now that it's empty
    return wxRmdir(path);
}
