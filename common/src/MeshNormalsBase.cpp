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
#include "MeshNormalsBase.hpp"
#include "CoordinateSystemConverter.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------

//VertexNormal
VertexNormal::VertexNormal()
{
    this->smooth = 0;
    this->next = nullptr;
}

VertexNormal::VertexNormal(const FinjinVector3& n, unsigned int s)
{
    this->smooth = s;
    this->next = nullptr;
    this->accum.push_back(n);
}

VertexNormal::~VertexNormal()
{
    delete this->next;
}

void VertexNormal::Add(FinjinVector3& n, unsigned int smoothingGroups)
{
    if (this->accum.empty())
    {
        //This is an initialization
        this->smooth = smoothingGroups;
        this->accum.push_back(n);
    }
    else if ((smoothingGroups & this->smooth) == 0)
    {
        //Smoothing groups don't overlap. Add a new normal
        if (this->next != nullptr)
            this->next->Add(n, smoothingGroups);
        else
            this->next = new VertexNormal(n, smoothingGroups);
    }
    else
    {
        //Smoothing groups overlap
        auto ignoreInfluence = false;
        for (auto& acc : this->accum)
        {
            auto dot = acc.Dot(n);
            if (dot >= .98f)
            {
                ignoreInfluence = true;
                break;
            }
        }

        if (!ignoreInfluence)
        {
            this->smooth |= smoothingGroups;
            this->accum.push_back(n);
        }
    }
}

const FinjinVector3& VertexNormal::Get(unsigned int smoothingGroups) const
{
    //Retrieves a normal if the smoothing groups overlap or there is only one in the list

    auto n = this;
    for (; n->next != nullptr; n = n->next)
    {
        if (n->smooth & smoothingGroups)
            return n->norm;
    }

    return n->norm;
}

void VertexNormal::Normalize()
{
    //Normalize each normal in the list

    for (auto n = this; n != nullptr; n = n->next)
    {
        n->norm.SetZero();

        for (auto& acc : n->accum)
            n->norm += acc;

        n->norm.Normalize();
    }
}

//VertexTangent
VertexTangent::VertexTangent()
{
    this->smooth = 0;
    this->next = nullptr;
}

VertexTangent::VertexTangent(const FinjinVector4& t, unsigned int s)
{
    this->smooth = s;
    this->next = nullptr;
    this->accum.push_back(t);
}

VertexTangent::~VertexTangent()
{
    delete this->next;
}

void VertexTangent::Add(FinjinVector4& t, unsigned int smoothingGroups)
{
    if (this->accum.empty())
    {
        //This is an initialization
        this->smooth = smoothingGroups;
        this->accum.push_back(t);
    }
    else if ((smoothingGroups & this->smooth) == 0)
    {
        //Smoothing groups don't overlap. Add a new normal
        if (this->next != nullptr)
            this->next->Add(t, smoothingGroups);
        else
            this->next = new VertexTangent(t, smoothingGroups);
    }
    else
    {
        //Smoothing groups overlap
        auto ignoreInfluence = false;
        for (auto& acc : this->accum)
        {
            auto dot = ((FinjinVector3&)acc).Dot((FinjinVector3&)t);
            if (dot >= .98f)
            {
                ignoreInfluence = true;
                break;
            }
        }

        if (!ignoreInfluence)
        {
            this->smooth |= smoothingGroups;
            this->accum.push_back(t);
        }
    }
}

const FinjinVector4& VertexTangent::Get(unsigned int smoothingGroups) const
{
    //Retrieves a tangent if the smoothing groups overlap or there is only one in the list

    auto n = this;
    for (; n->next != nullptr; n = n->next)
    {
        if (n->smooth & smoothingGroups)
            return n->tangent;
    }

    return n->tangent;
}

void VertexTangent::Normalize()
{
    //Normalize each normal in the list

    FinjinVector3 tangent;

    for (auto n = this; n != nullptr; n = n->next)
    {
        tangent.Set(0, 0, 0);

        for (auto& acc : n->accum)
            tangent += (FinjinVector3&)acc;

        tangent.Normalize();

        n->tangent.Set(tangent, !n->accum.empty() ? n->accum.front().w : 1.0f);
    }
}

//MeshNormalsBase
void MeshNormalsBase::Destroy()
{
    this->vertexNormals.clear();
    this->vertexTangents.clear();
    this->faceNormals.clear();
    this->faceTangents.clear();
    this->faceSmoothingGroups.clear();
    this->normalFaces.clear();
}

FinjinVector3 MeshNormalsBase::GetNormal(int cornerIndex, int vertexIndex, int faceIndex, unsigned int smoothingGroups)
{
    FinjinVector3 result(0,0,0);

    if (!this->normalFaces.empty() &&
        cornerIndex < (int)this->normalFaces[faceIndex].GetDegree() &&
        this->normalFaces[faceIndex].v[cornerIndex] >= 0)
    {
        //Get explicitly defined normal based on face index and corner index
        result = this->normalFaces[faceIndex].normals[cornerIndex];
    }
    else if (smoothingGroups == 0 && !this->faceNormals.empty())
    {
        //Get normal based on face
        result = this->faceNormals[faceIndex];
    }
    else if (!this->vertexNormals.empty())
    {
        //Get normal based on face index, vertex index, and smoothing group
        if (this->faceSmoothingGroups[faceIndex] != 0)
            result = this->vertexNormals[vertexIndex].Get(smoothingGroups);
        else
            result = this->faceNormals[faceIndex];
    }

    return result;
}

FinjinVector4 MeshNormalsBase::GetTangent(int cornerIndex, int vertexIndex, int faceIndex, unsigned int smoothingGroups)
{
    FinjinVector4 result(0,0,0,0);

    if (!this->tangentFaces.empty() &&
        cornerIndex < (int)this->tangentFaces[faceIndex].GetDegree() &&
        this->tangentFaces[faceIndex].v[cornerIndex] >= 0)
    {
        //Get explicitly defined tangent based on face index and corner index
        result = this->tangentFaces[faceIndex].tangents[cornerIndex];
    }
    else if (smoothingGroups == 0 && !this->faceTangents.empty())
    {
        //Get tangent based on face
        result = this->faceTangents[faceIndex];
    }
    else if (!this->vertexTangents.empty())
    {
        //Get tangent based on face index, vertex index, and smoothing group
        if (this->faceSmoothingGroups[faceIndex] != 0)
            result = this->vertexTangents[vertexIndex].Get(smoothingGroups);
        else
            result = this->faceTangents[faceIndex];
    }

    return result;
}

void MeshNormalsBase::Convert(const CoordinateSystemConverter& coordinateConverter)
{
    if (coordinateConverter.RequiresConversion())
    {
        for (size_t i = 0; i < this->vertexNormals.size(); i++)
        {
            for (auto vnormal = &this->vertexNormals[i]; vnormal != nullptr; vnormal = vnormal->next)
                coordinateConverter.ConvertPoint(vnormal->norm);
        }

        for (size_t i = 0; i < this->faceNormals.size(); i++)
            coordinateConverter.ConvertPoint(this->faceNormals[i]);

        for (size_t i = 0; i < this->normalFaces.size(); i++)
        {
            for (size_t j = 0; j < this->normalFaces[i].normals.size(); j++)
                coordinateConverter.ConvertPoint(this->normalFaces[i].normals[j]);
        }


        for (size_t i = 0; i < this->vertexTangents.size(); i++)
        {
            for (auto vnormal = &this->vertexTangents[i]; vnormal != nullptr; vnormal = vnormal->next)
                coordinateConverter.ConvertPoint((FinjinVector3&)vnormal->tangent);
        }

        for (size_t i = 0; i < this->faceTangents.size(); i++)
            coordinateConverter.ConvertPoint((FinjinVector3&)this->faceTangents[i]);

        for (size_t i = 0; i < this->tangentFaces.size(); i++)
        {
            for (size_t j = 0; j < this->tangentFaces[i].tangents.size(); j++)
                coordinateConverter.ConvertPoint((FinjinVector3&)this->tangentFaces[i].tangents[j]);
        }
    }
}
