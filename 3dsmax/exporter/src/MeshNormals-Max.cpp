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
#include "MeshNormals-Max.hpp"
#include "MaxUtilities.hpp"
#include "MaxControlUtilities.hpp"
#include <IEditNormals.h>

#define ALL_SMOOTHING_GROUPS (DWORD)-1

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
MeshNormals::MeshNormals()
{
}

MeshNormals::MeshNormals(Mesh& mesh, const CoordinateSystemConverter& coordinateConverter, PrimitiveType meshPrimitiveType)
{
    Create(mesh, coordinateConverter, meshPrimitiveType);
}

void MeshNormals::Create(Mesh& mesh, const CoordinateSystemConverter& coordinateConverter, PrimitiveType meshPrimitiveType)
{
    Destroy();

    //Initialize arrays
    this->faceNormals.resize(mesh.numFaces);
    this->faceSmoothingGroups.resize(mesh.numFaces);
    this->vertexNormals.resize(mesh.numVerts);

    MeshMap* textureCoordinateMap = nullptr;
    if (meshPrimitiveType == PrimitiveType::TRIANGLES)
    {
        for (int mapNumber = -NUM_HIDDENMAPS; mapNumber < mesh.numMaps; mapNumber++)
        {
            auto& map = mesh.Map(mapNumber);
            if (map.IsUsed() && mapNumber >= 0)
            {
                textureCoordinateMap = &map;

                this->faceTangents.resize(mesh.numFaces);
                this->vertexTangents.resize(mesh.numVerts);

                break;
            }
        }
    }

    //Compute face and vertex normals/tangents
    for (int faceIndex = 0; faceIndex < mesh.numFaces; faceIndex++)
    {
        //Face smoothing groups
        auto faceSmoothingGroup = (meshPrimitiveType == PrimitiveType::TRIANGLES) ? mesh.faces[faceIndex].smGroup : ALL_SMOOTHING_GROUPS;
        this->faceSmoothingGroups[faceIndex] = faceSmoothingGroup;

        //Face normal
        auto& v1 = mesh.verts[mesh.faces[faceIndex].v[0]];
        auto& v2 = mesh.verts[mesh.faces[faceIndex].v[1]];
        auto& v3 = mesh.verts[mesh.faces[faceIndex].v[2]];
        auto normal = Normalize((v2-v1) ^ (v3-v2));
        this->faceNormals[faceIndex].Set(normal.x, normal.y, normal.z);

        //Add the face normal to all the vertex normals
        for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
            this->vertexNormals[mesh.faces[faceIndex].v[vertexIndex]].Add(this->faceNormals[faceIndex], faceSmoothingGroup);

        if (textureCoordinateMap != nullptr)
        {
            auto& w1 = textureCoordinateMap->tv[textureCoordinateMap->tf[faceIndex].t[0]];
            auto& w2 = textureCoordinateMap->tv[textureCoordinateMap->tf[faceIndex].t[1]];
            auto& w3 = textureCoordinateMap->tv[textureCoordinateMap->tf[faceIndex].t[2]];

            auto x1 = v2.x - v1.x;
            auto x2 = v3.x - v1.x;
            auto y1 = v2.y - v1.y;
            auto y2 = v3.y - v1.y;
            auto z1 = v2.z - v1.z;
            auto z2 = v3.z - v1.z;

            auto s1 = w2.x - w1.x;
            auto s2 = w3.x - w1.x;
            auto t1 = w2.y - w1.y;
            auto t2 = w3.y - w1.y;

            auto r = 1.0 / (s1 * t2 - s2 * t1);

            Point3 tan1((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
            Point3 tan2((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

            auto tangent = (tan1 - normal * DotProd(normal, tan1)).Normalize();

            auto handedness = DotProd(CrossProd(normal, tan1), tan2) < 0.0 ? -1.0f : 1.0f;

            this->faceTangents[faceIndex].Set(tangent.x, tangent.y, tangent.z, handedness);

            for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
                this->vertexTangents[mesh.faces[faceIndex].v[vertexIndex]].Add(this->faceTangents[faceIndex], faceSmoothingGroup);
        }
    }

    //Normalize vertex normals
    for (size_t vertexIndex = 0; vertexIndex < this->vertexNormals.size(); vertexIndex++)
        this->vertexNormals[vertexIndex].Normalize();
    for (size_t vertexIndex = 0; vertexIndex < this->vertexTangents.size(); vertexIndex++)
        this->vertexTangents[vertexIndex].Normalize();

    //Get the specified normals
    auto specifiedNormals = mesh.GetSpecifiedNormals();
    if (specifiedNormals != nullptr)
    {
        int normalFaceCount = specifiedNormals->GetNumFaces();
        this->normalFaces.resize(normalFaceCount);

        for (int faceIndex = 0; faceIndex < normalFaceCount; faceIndex++)
        {
            int degree = 3;
            this->normalFaces[faceIndex].SetDegree(degree);

            for (int cornerIndex = 0; cornerIndex < degree; cornerIndex++)
            {
                int normalID = specifiedNormals->GetNormalIndex(faceIndex, cornerIndex);
                if (specifiedNormals->GetNormalExplicit(normalID))
                {
                    Point3 maxNormal = specifiedNormals->GetNormal(faceIndex, cornerIndex);
                    FinjinVector3 normal(maxNormal.x, maxNormal.y, maxNormal.z);
                    this->normalFaces[faceIndex].Set(cornerIndex, normalID, normal);
                }
            }
        }
    }

    //Convert to the specified coordinate system
    Convert(coordinateConverter);
}
