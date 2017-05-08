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


//Includes----------------------------------------------------------------------
#include "FinjinVector.hpp"
#include "FinjinColor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** A single vertex in a mesh. */
    class Vertex
    {
    public:
        Vertex();

        void SetPosition(float x, float y, float z);
        void SetNormal(float x, float y, float z);
        void SetColor(float r, float g, float b, float a = 1.0f);
        void AddTexCoord(int mapIndex, const FinjinVector3& uvw);
        void AddTexCoord(int mapIndex, float u, float v, float w = 0.0f);

        bool AlmostEquals(const Vertex& other, float tolerance = 0.0001f) const;

        const FinjinVector3& GetTextureCoordinate(int mapIndex) const;

    private:
        bool HasSameTexCoords(const std::array<std::pair<int, FinjinVector3>, FINJIN_EXPORTER_MAX_TEXTURE_COORD_SETS>& uvwMap) const
        {
            for (auto thisMappedVertex = this->uvwMap.begin(), otherMappedVertex = uvwMap.begin();
                thisMappedVertex != this->uvwMap.end();
                ++thisMappedVertex, ++otherMappedVertex)
            {
                if (thisMappedVertex->first != otherMappedVertex->first)
                {
                    //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Map index check failed"));
                    return false;
                }

                if (thisMappedVertex->second != otherMappedVertex->second)
                {
                    //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Map coord check failed"));
                    return false;
                }
            }

            return true;
        }

    public:
        //Members that are not part of the hashing
        int originalCornerIndex;
        int originalFaceVertexIndex;
        int originalFaceIndex;
        unsigned int smoothingGroups;

        //Members that are part of the hashing
        float pointSize;
        FinjinVector3 position;
        FinjinVector3 normal;
        FinjinVector4 tangent;
        FinjinVector3 binormal;
        std::array<FinjinColor, FINJIN_EXPORTER_MAX_COLOR_SETS> colors;
        std::array<std::pair<int, FinjinVector3>, FINJIN_EXPORTER_MAX_TEXTURE_COORD_SETS> uvwMap;
    };

    /** A collection of vertices in a mesh. */
    class VertexList
    {
    private:
        typedef std::vector<Vertex> Vertices;

    public:
        VertexList();

        typedef Vertices::iterator iterator;
        typedef Vertices::const_iterator const_iterator;
        typedef Vertices::reverse_iterator reverse_iterator;
        typedef Vertices::const_reverse_iterator const_reverse_iterator;

        void Reserve(size_t count);

        /**
         * Adds a vertex to the list.
         * @param v [in] - The vertex to add.
         * @param isNew [out] - Pointer to a boolean that receives a value indicating whether the
         * vertex was actually added to the list.
         * @param fastAdd [in] - Indicates whether the list should be searched for a vertex that
         * matches the one passed in.
         * @return The index into the list for the inserted element is returned.
         */
        size_t Add(Vertex& v, bool* isNew = nullptr, bool fastAdd = false);

        void FastAppend(const VertexList& other);

        bool IsEmpty() const { return this->items.empty(); }
        size_t GetSize() const { return this->items.size(); }
        const Vertex& GetFront() { return this->items.front(); }

        void Clear();

        size_t size() const {return GetSize();}

        /**
         * Determines if two vertex lists are 'almost' the same by checking every couple of vertices.
         * @param other [in] - The other vertex list to compare to.
         * @param stride [in] - How many vertices to advance between vertex comparisons. This value should be
         * 1 or greater.
         * @return If the two vertex lists are almost the same true is returned. Otherwise, false is returned.
         */
        bool IsAlmostSame(const VertexList& other, size_t stride, float tolerance = 0.0001f) const;

        iterator begin() {return this->items.begin();}
        iterator end() {return this->items.end();}
        const_iterator begin() const {return this->items.begin();}
        const_iterator end() const {return this->items.end();}

        reverse_iterator rbegin() {return this->items.rbegin();}
        reverse_iterator rend() {return this->items.rend();}
        const_reverse_iterator rbegin() const {return this->items.rbegin();}
        const_reverse_iterator rend() const {return this->items.rend();}

        const Vertex& operator [] (size_t i) const;

    private:
        Vertices items;
        std::unordered_map<size_t, size_t> hashToIndex;
    };

} }
