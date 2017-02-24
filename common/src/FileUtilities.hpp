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


//Includes---------------------------------------------------------------------


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Common {
    class WxByteBuffer;
} }

namespace Finjin { namespace Exporter {

    using namespace Finjin::Common;

    /** Various file-related utility functions. */
    class FileUtilities
    {
    public:
        static void UnifySeparators(wxString& path);
        static void RemoveLeadingSeparators(wxString& path);

        static void WriteFileTo(wxFile& inFile, wxFile& outFile);

        /** Determines if the specified character is a path separator. */
        static bool IsSeparator(wxChar c);

        /** Determines if the specified string ends with a path separator character. */
        static bool EndsWithSeparator(const wxString& path);

        /** Ensures that the specified path ends with the specified separator character. */
        static void EnsureTrailingPathSeparator(wxString& path, wxChar separatorChar = wxFileName::GetPathSeparator());

        /** Ensures that the specified path doesn't end with a separator character. */
        static void EnsureNoTrailingPathSeparator(wxString& path);

        /** The same as EnsureTrailingPathSeparator(), but returns a new string instead of modifying input. */
        static wxString WithTrailingPathSeparator(const wxString& path, wxChar separatorChar = wxFileName::GetPathSeparator());

        /** The same as EnsureNoTrailingPathSeparator(), but returns a new string instead of modifying input. */
        static wxString WithoutTrailingPathSeparator(const wxString& path);

        /** Joins two paths and returns the joined result. */
        static wxString JoinPath(const wxString& path1, const wxString& path2);

        /** Removes all the relative components from the path. */
        static wxString MakeFullPath(const wxString& path);

        /** Ensures slashes look nice for printing. */
        static wxString PrettyPath(const wxString& path);

        /** Parses the base file name from the specified path. */
        static wxString GetFileName(const wxString& path, bool includeExtension = true);

        /** Parses the base file name from the specified path. The file name contains no extension. */
        static wxString GetFileNameNoExtension(const wxString& path);

        /** Gets the index of the character where the base file name starts in the specified path. */
        static int GetFileNameOffset(const wxString& path);

        static bool HasExtension(const wxString& path);

        /** Gets the index of the character where the file extension (not including the dot) starts in the specified path. */
        static int GetExtensionOffset(const wxString& path, bool includeDot);

        /** Gets the file name's extension. */
        static wxString GetExtension(const wxString& path, bool includeDot);

        /** Returns the specified path without the file extension, if any. */
        static wxString RemoveExtension(const wxString& path);

        static wxString RemoveFileName(const wxString& path);

        /** Modifies the file path's extension, making it lower case. */
        static void MakeExtensionLowercase(wxString& path);

        /** 
         * Extracts the drive and directories from the file name. 
         * @return The directory path, with a trailing path separator.
         */
        static wxString GetDirectoryPath(const wxString& path);

        /** Creates all the directories and subdirectories specified by the directory path. */
        static bool CreateDirectoryRecursive(const wxString& path);

        /** Creates all the directories and subdirectories containing the file path. */
        static bool CreateFileDirectoryRecursive(const wxString& path);

        /**
         * Copies the contents of the source directory into the destination.
         * @param sourceDirectory [in] - The source directory. Should be terminated with a path separator.
         * @param destinationDirectory [in] - The destination directory. Should be terminated with a path separator.
         * @return Returns true if the directory was successfully copied, false otherwise.
         */
        static bool CopyDirectory(const wxString& sourceDirectory, const wxString& destinationDirectory);

        /**
         * Creates an export directory.
         * @param outputPath [out] - The resultant output path.
         * @param defaultExportPath [in] - The default export path. This path is assumed to exist 
         * on the computer.
         * @param subdirectory [in] - A subdirectory, which may be either absolute or 
         * relative to the defaultExportPath. If this is empty, the the default export path
         * is returned. If the subdirectory is not empty, it is joined with the default export path, 
         * and the resultant directory is created recursively.
         */
        static wxString CreateExportDirectory(const wxString& defaultExportPath, const wxString& subdirectory);

        /** 
         * Gets all the subdirectories of the specified directory
         * @param directory [in] - The directory whose subdirectories are queried. This should include the trailing separator
         * @param subdirs [out] - The subdirectories
         * @param full [in] - Indicates whether full file paths are returned
         * @param includeTrailing [in] - Indicates whether a trailing separator should be appended to the subdirectories
         */
        static void GetSubdirectories(const wxString& directory, wxArrayString& subdirs, bool full = true, bool includeTrailing = true);

        /** Determines if the specified path points to an existing file or directory. */
        static bool FileOrDirectoryExists(const wxString& path);

        /**
         * Finds all the directories containing the specified files.
         * @param rootDirectory [in] - The start location for the search. This directory is used
         * when checking file names.
         * @param filenames [in/out] - The base file names to find. As files are found, they are removed.
         * @param directories [out] - The directories containing the found file names.
         */
        template <typename FileSet, typename DirectorySet>
        static bool FindDirectoriesContainingFiles
            (
            const wxString& rootDirectory, 
            FileSet& filenames, 
            DirectorySet& directories
            )
        {
            if (!filenames.empty())
            {
                //Try to find all the files in the specified directory
                wxDir dir(rootDirectory);
                for (auto filename = filenames.begin(); filename != filenames.end();)
                {
                    if (dir.HasFiles(*filename))
                    {
                        directories.insert(rootDirectory);

                        auto next = filename;
                        ++next;
                
                        filenames.erase(filename);
                
                        filename = next;
                    }
                    else
                        ++filename;
                }

                //Iterate over subdirectories if there are any files left
                if (!filenames.empty())
                {
                    wxArrayString subdirs;
                    GetSubdirectories(rootDirectory, subdirs);
                    for (int i = 0; i < (int)subdirs.GetCount(); i++)
                        FindDirectoriesContainingFiles(subdirs[i], filenames, directories);
                }
            }

            return !directories.empty();
        }


        /** 
         * Ensures the specified file name has the specified extension. 
         * @param fileName [in/out] - The file name to modify. If it has an extension, that extension is removed
         * and replaced with the specified extension.
         * @param extension [in] - The new file extension. This can be specified with or without a dot.
         */
        static void EnsureExtension(wxString& fileName, const wxString& extension);

        /** The same as EnsureExtension(), except a new string is returned instead of modifying the input one. */
        static wxString CreateFileName(const wxString& name, const wxString& extension);

        /** Ensures that all the separator characters in the specified path use forward slashes. */
        static void MakeForwardSlashes(wxString& text);

        /** Ensures that all the separator characters in the specified path use native slashes. */
        static void MakeNativeSlashes(wxString& text);

        /** 
         * Reads the specified text file into a string.
         * @param fileName [in] - The text file to load.
         * @param text [out] - The string that receives the text from the file.
         * @return If the file was read, true is returned. Otherwise, false is returned.
         */
        static bool ReadTextFile(const wxString& fileName, wxString& text);

        /** 
         * Writes the specified text to a file.
         * @param fileName [in] - The text file to write.
         * @param text [in] - The string that is written to the file.
         * @return If the file was written, true is returned. Otherwise, false is returned.
         */
        static bool WriteTextFile(const wxString& fileName, const wxString& text);
        
        static bool ReadBinaryFile(const wxString& fileName, WxByteBuffer& bytes);

        /**
         * Reads a binary file.
         * @param fileName [in] - The binary file to load.
         * @param byteCount [out] - The bytes read from the file.
         * @return If the file existed, true is returned. Otherwise, false is returned.
         */
        static bool ReadBinaryFile(const wxString& fileName, std::vector<uint8_t>& bytes);
        
        /**
         * Ensures that the specified file is replaceable by removing its read-only property.
         * @param fileName [in] - The file to modify.
         * @return If the file is replaceable (either it doesn't exist or it is writable), true is returned. 
         * Otherwise, false is returned.
         */
        static bool EnsureFileReplaceable(const wxString& fileName);

        /**
         * Copies a file, ensuring that the destination file, if present, is replaceable.
         * @param sourceFileName [in] - The source file to copy.
         * @param destinationFileName [in] - The destination file path.
         * @return If the file was copied, true is returned. Otherwise, false is returned.
         */
        static bool CopyFileEnsureReplaceable(const wxString& sourceFileName, const wxString& destinationFileName);

        /**
         * Deletes a directory and all its subdirectories.
         * @param path [in] - The directory to delete.
         * @param If the directory and all subdirectories were deleted, true is returned. Otherwise,
         * false is returned.
         */
        static bool DeleteDirectory(const wxString& path);

    };

} }
