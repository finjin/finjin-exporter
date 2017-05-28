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
#include "VertexList.hpp"

#define PERFORM_FULL_VERTEX_COMPARISON 0

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
Vertex::Vertex()
{
    this->originalCornerIndex = -1;
    this->originalFaceVertexIndex = -1;
    this->originalFaceIndex = -1;
    this->smoothingGroups = 0;

    this->pointSize = 0;

    this->colors[0].Set(1,1,1);

    for (auto& item : this->uvwMap)
        item.first = -1;
}

void Vertex::SetPosition(float x, float y, float z)
{
    this->position.Set(x, y, z);
}

void Vertex::SetNormal(float x, float y, float z)
{
    this->normal.Set(x, y, z);
}

void Vertex::SetColor(float r, float g, float b, float a)
{
    auto& color = this->colors[0];
    color.Set(r, g, b, a);
}

void Vertex::AddTexCoord(int mapIndex, const FinjinVector3& uvw)
{
    if (mapIndex >= 0 && mapIndex < this->uvwMap.size())
    {
        this->uvwMap[mapIndex].first = mapIndex;
        this->uvwMap[mapIndex].second = uvw;
    }
}

void Vertex::AddTexCoord(int mapIndex, float u, float v, float w)
{
    if (mapIndex >= 0 && mapIndex < this->uvwMap.size())
    {
        this->uvwMap[mapIndex].first = mapIndex;
        this->uvwMap[mapIndex].second = FinjinVector3(u, v, w);
    }
}

const FinjinVector3& Vertex::GetTextureCoordinate(int mapIndex) const
{
    static const FinjinVector3 NULL_VECTOR(0, 0, 0);

    if (mapIndex >= 0 && mapIndex < this->uvwMap.size() && this->uvwMap[mapIndex].first >= 0)
        return this->uvwMap[mapIndex].second;
    else
        return NULL_VECTOR;
}

bool Vertex::AlmostEquals(const Vertex& other, float tolerance) const
{
    if (this->pointSize != other.pointSize)
    {
        //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Point size check failed"));
        return false;
    }

    if (!this->position.AlmostEquals(other.position, tolerance))
    {
        //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Position check failed"));
        return false;
    }

    if (!this->normal.AlmostEquals(other.normal, tolerance))
    {
        //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Normal check failed"));
        return false;
    }

    if (!this->colors[0].AlmostEquals(other.colors[0]))
    {
        //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Color check failed"));
        return false;
    }

    if (!other.HasSameTexCoords(this->uvwMap))
    {
        //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Texture coordinate set check failed"));
        return false;
    }

    return true;
}

VertexList::VertexList()
{
}

void VertexList::Reserve(size_t count)
{
    this->items.reserve(count);
}

size_t VertexList::Add(Vertex& v, bool* isNew, bool fastAdd)
{
    size_t index = 0;

    auto hash = HashBytes(&v.pointSize, sizeof(v) - offsetof(Vertex, pointSize));

    if (fastAdd)
    {
        //Add the vertex without checking for duplicates
        index = this->items.size();
        this->items.push_back(v);

        if (isNew != nullptr)
            *isNew = true;
    }
    else
    {
        //Check for duplicates
        auto foundIndex = this->hashToIndex.find(hash);
        if (foundIndex != this->hashToIndex.end())
        {
            index = foundIndex->second;
            if (isNew != nullptr)
                *isNew = false;
        }
        else
        {
            index = this->items.size();
            this->items.push_back(v);

            this->hashToIndex[hash] = index;

            if (isNew != nullptr)
                *isNew = true;
        }
    }

    return index;
}

void VertexList::FastAppend(const VertexList& other)
{
    for (auto& v : other)
        this->items.push_back(v);
}

void VertexList::Clear()
{
    this->items.clear();
    this->hashToIndex.clear();
}

bool VertexList::IsAlmostSame(const VertexList& other, size_t stride, float tolerance) const
{
    if (size() != other.size())
        return false;

    if (stride < 1)
        stride = 1;

#if PERFORM_FULL_VERTEX_COMPARISON
    for (auto thisVertex = begin(), otherVertex = other.begin(); thisVertex != end();)
    {
        if (!thisVertex->AlmostEquals(*otherVertex, tolerance))
        {
            size_t vertexIndex = thisVertex - begin();
            //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Vertex %d check failed"), vertexIndex);
            return false;
        }

        for (size_t i = 0; i < stride; i++)
        {
            if (++thisVertex == end())
                break;
            ++otherVertex;
        }
    }
#else
    for (auto thisVertex = this->hashToIndex.begin(), otherVertex = other.hashToIndex.begin(); thisVertex != this->hashToIndex.end();)
    {
        if (thisVertex->first != otherVertex->first)
        {
            //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Vertex check failed"));
            return false;
        }

        for (size_t i = 0; i < stride; i++)
        {
            if (++thisVertex == this->hashToIndex.end())
                break;
            ++otherVertex;
        }
    }
#endif

    return true;
}

const Vertex& VertexList::operator [] (size_t i) const
{
    static const Vertex NULL_VERTEX;
    return (i < this->items.size()) ? this->items[i] : NULL_VERTEX;
}
