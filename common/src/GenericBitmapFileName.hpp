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
namespace Finjin { namespace Exporter {

    /**
     * Manages a bitmap file name used with an Finjin material, which may turn into multiple names.
     */
    class GenericBitmapFileName
    {
    public:
        /** The type of file the bitmap file name represents. */
        enum Type
        {
            /** A standard bitmap file name. The file name is used directly. */
            STANDARD,

            /** An animated bitmap file name. It has multiple frames. */
            ANIMATED,

            /** A cubic texture file name. It has six files. */
            CUBIC
        };
        
        /** Default constructor. */
        GenericBitmapFileName();

        /**
         * Constructor.
         * @param fileName [in] - The file name.
         * @param type [in] - The file name type.
         */
        GenericBitmapFileName(const wxString& fileName, Type type = STANDARD);

        /** 
         * Constructs the file name assuming the ANIMATED type. 
         * @param fileName [in] - The file name.
         * @param frameCount [in] - The number of frames.
         */
        GenericBitmapFileName(const wxString& fileName, int frameCount);

        /** Gets the number of file names this bitmap file name maps to. */
        int GetAllFileNamesCount() const;

        /** Gets all the file names this bitmap file name maps to. */
        void GetAllFileNames(std::vector<wxString>& fileNames) const;

        /** Allows a GenericBitmapFileName to be inserted into a std::set. */
        bool operator < (const GenericBitmapFileName& fileName) const;

    public:
        wxString fileName;
        Type type;
        int frameCount;
    };

    typedef std::set<GenericBitmapFileName> GenericBitmapFileNameSet;

} }
