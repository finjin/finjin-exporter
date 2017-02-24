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
#include "MeshNormals-Maya.hpp"
#include "MayaUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
MeshNormals::MeshNormals()
{
}

MeshNormals::MeshNormals(MFnMesh& mesh, const CoordinateSystemConverter& coordinateConverter)
{
    Create(mesh, coordinateConverter);
}

void MeshNormals::Create(MFnMesh& mesh, const CoordinateSystemConverter& coordinateConverter)
{
    Destroy();

    auto foundUV = false;
    MString uvSetName;
    MStringArray uvSetNames;
    mesh.getUVSetNames(uvSetNames);
    if (uvSetNames.length() > 0)
    {
        uvSetName = uvSetNames[0];
        foundUV = true;
    }
    
    MItMeshPolygon faceIter(mesh.object());
    this->normalFaces.resize(faceIter.count());
    if (foundUV)
        this->tangentFaces.resize(this->normalFaces.size());
    
    //Get all normals and tangents
    MFloatVectorArray normals;
    mesh.getNormals(normals, GET_POINTS_NORMALS_SPACE);
    MFloatVectorArray tangents;
    mesh.getTangents(tangents, GET_POINTS_NORMALS_SPACE);

    for (unsigned int faceIndex = 0; !faceIter.isDone(); faceIter.next(), faceIndex++)
    {
        MVectorArray normalArray;
        faceIter.getNormals(normalArray, GET_POINTS_NORMALS_SPACE);

        MIntArray faceIndices;
        faceIter.getVertices(faceIndices);

        this->normalFaces[faceIndex].SetDegree(normalArray.length());
        if (foundUV)
            this->tangentFaces[faceIndex].SetDegree(normalArray.length());
        for (unsigned int cornerIndex = 0; cornerIndex < normalArray.length(); cornerIndex++)
        {
            FinjinVector3 normal(normalArray[cornerIndex].x, normalArray[cornerIndex].y, normalArray[cornerIndex].z);
            coordinateConverter.ConvertPoint(normal);
            this->normalFaces[faceIndex].Set(cornerIndex, faceIter.normalIndex(cornerIndex), normal);
            
            if (foundUV)
            {
                auto tangentIndex = faceIter.tangentIndex(cornerIndex);
                FinjinVector4 tangent(tangents[tangentIndex].x, tangents[tangentIndex].y, tangents[tangentIndex].z, 1);
                //auto tangentID = mesh.getTangentId(faceIndex, faceIndices[cornerIndex]);
                //tangent.w = mesh.isRightHandedTangent(tangentID, &uvSetName) ? 1.0f : -1.0f;
                coordinateConverter.ConvertPoint((FinjinVector3&)tangent);
                this->tangentFaces[faceIndex].Set(cornerIndex, tangentIndex, tangent);
            }
        }
    }
}
