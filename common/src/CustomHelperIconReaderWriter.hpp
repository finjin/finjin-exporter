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
#include "FinjinVector.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Writes an icon file that is used with the Finjin custom helper object. */
    class CustomHelperIconWriter
    {
    public:
        /** Determines if the icon data is valid. */
        bool IsValid() const;

        /** Normalizes the vertex data by ensuring scaling all vertices down by the length of the 'longest' vertex. */
        void Normalize();

        /** 
         * Writes the icon data to the specified file.
         * @param fileName [in] - Full path to the file name to write the icon data to. If this name starts with
         * an '_' character, the data is written as a C-style array of vectors. Otherwise, it is written as XML data.
         * @param vector3Type [in] - If the data is being written as a C-style array, this is the type of the array.
         * @return If the file was written, true is returned. Otherwise, false is returned.
         */
        bool Write(const wxString& fileName, const wxString& vector3Type = wxT("Vector3"));    

    public:
        struct Edge
        {
            int v1, v2;
        };
        /** 
         * All the visible edges of the icon. 
         * Use of this array is optional. If empty, the number of vertices should be even, with each pair
         * of vertices containing a visibile edge.
         */
        std::vector<Edge> edges;

        /** 
         * If edges is empty, this contains all the pairs of visible edges.
         * If edges is non-empty, this contains all the vertices references by the edges.
         */
        std::vector<FinjinVector3> vertices;
    };

    /** Reads an icon file that is used with the Finjin custom helper object. */
    class CustomHelperIconReader
    {
    public:
        /**
         * Reads the icon data from the specified file.
         * @param fileName [in] - Full path to the file to read.
         * @return If the file was read and the data contained within it was valid, true is returned. Otherwise,
         * false is returned.
         */
        bool Read(const wxString& fileName);

    public:
        /** All the pairs of visible edges. */
        std::vector<FinjinVector3> vertices;
    };

} }
