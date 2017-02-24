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
#include "SimpleMeshInfo.hpp"
#include "MathUtilities.hpp"
#include "ApplicationAccessor.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
bool SimpleMeshInfo::CanHandle(ObjectAccessor object, TimeAccessor time)
{
    auto maxObject = object.node->EvalWorldState(time.GetNativeTime()).obj;
    return 
        maxObject!= nullptr && 
        maxObject->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0));
}

bool SimpleMeshInfo::Create(ObjectAccessor object, TimeAccessor time, TransformAccessor* transformation)
{
    CoordinateSystemConverter nullConverter(ApplicationAccessor::GetUpAxis());
    return Create(object, nullConverter, 1, time, transformation);
}

bool SimpleMeshInfo::Create
    (
    ObjectAccessor object, 
    const CoordinateSystemConverter& coordinateConverter,
    float scale,
    TimeAccessor time,
    TransformAccessor* transformation
    )
{
    if (!CanHandle(object, time))
        return false;

    ResetBounds();

    //Evaluate mesh
    auto maxObject = object.node->EvalWorldState(time.GetNativeTime()).obj;
    auto triObject = static_cast<TriObject*>(maxObject->ConvertToType(time.GetNativeTime(), Class_ID(TRIOBJ_CLASS_ID, 0)));
    auto& mesh = triObject->GetMesh();
    
    FinjinVector3 rawPlaneVertices[3];
    FinjinVector3 planeVertices[3];
    for (int faceIndex = 0; faceIndex < mesh.numFaces; faceIndex++)
    {
        auto& face = mesh.faces[faceIndex];

        for (int i = 0; i < 3; i++)
        {
            //Position
            auto& rawPosition = mesh.verts[face.v[i]];
            FinjinVector3 rawVertex(rawPosition.x, rawPosition.y, rawPosition.y);
            
            //Convert position
            Point3 maxPosition(rawPosition);
            coordinateConverter.ConvertPoint(maxPosition);
            FinjinVector3 vertex(maxPosition.x * scale, maxPosition.y * scale, maxPosition.z * scale);
            if (transformation != nullptr)
                transformation->TransformPoint(vertex);                

            //Update bounds
            UpdateBounds(faceIndex == 0 && i == 0, rawVertex, vertex);            

            //Hold onto plane vertices
            if (faceIndex == 0)
            {
                rawPlaneVertices[i] = rawVertex;
                planeVertices[i] = vertex;
            }
        }
    }
    FinishBounds();
    InitializePlanes(rawPlaneVertices, planeVertices);
    
    //Clean up
    if (triObject != maxObject)
        triObject->DeleteThis();

    return !this->rawBoundingBox.IsEmpty();
}
