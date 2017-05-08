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
#include "MayaUtilities.hpp"
#include "ApplicationAccessor.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
bool SimpleMeshInfo::CanHandle(ObjectAccessor object, TimeAccessor time)
{
    if (object.obj.hasFn(MFn::kMesh) || object.obj.hasFn(MFn::kNurbsSurface))
    {
        MFnDagNode dagNode(object.obj);
        return !dagNode.isIntermediateObject() && !MayaUtilities::IsEmptyMeshOrCurve(object.obj);
    }
    else
        return false;
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

    FinjinVector3 rawPlaneVertices[3];
    FinjinVector3 planeVertices[3];

    //Get the mesh, converting from a NURBs surface if necessary
    MFnMesh mesh;
    MFnDagNode tesselatedMeshTransformNode;
    if (object.obj.hasFn(MFn::kNurbsSurface))
    {
        MFnNurbsSurface nurbsSurface(object.obj);
        MObject tesselatedMeshTransform = nurbsSurface.tesselate();
        tesselatedMeshTransformNode.setObject(tesselatedMeshTransform);
        mesh.setObject(tesselatedMeshTransformNode.child(0));
    }
    else if (object.obj.hasFn(MFn::kMesh))
        mesh.setObject(object.obj);

    int totalTriangleIndex = 0;
    MItMeshPolygon faceIter(mesh.object());
    for (unsigned int faceIndex = 0; !faceIter.isDone(); faceIter.next(), faceIndex++)
    {
        MIntArray polyIndices;
        faceIter.getVertices(polyIndices);

        //Get triangles for each face
        int triangleCount = 0;
        faceIter.numTriangles(triangleCount);
        for (int triangleIndex = 0; triangleIndex < triangleCount; triangleIndex++, totalTriangleIndex++)
        {
            //Get the triangle
            MPointArray triPoints;
            MIntArray faceVertexIndices;
            faceIter.getTriangle(triangleIndex, triPoints, faceVertexIndices);

            //Iterate over triangle's vertices
            for (unsigned int i = 0; i < 3; i++)
            {
                //Position
                MPoint rawPosition(triPoints[i]);
                FinjinVector3 rawVertex(rawPosition.x, rawPosition.y, rawPosition.z);

                MPoint mayaPosition(rawPosition);
                coordinateConverter.ConvertPoint(mayaPosition);
                FinjinVector3 vertex(mayaPosition.x * scale, mayaPosition.y * scale, mayaPosition.z * scale);
                if (transformation != nullptr)
                    transformation->TransformPoint(vertex);

                //Update bounds
                UpdateBounds(faceIndex == 0 && triangleIndex == 0 && i == 0, rawVertex, vertex);

                //Hold onto plane vertices
                if (totalTriangleIndex == 0)
                {
                    rawPlaneVertices[i] = rawVertex;
                    planeVertices[i] = vertex;
                }
            }
        }
    }

    //Delete the tesselated mesh if necessary
    if (!tesselatedMeshTransformNode.object().isNull())
        MayaUtilities::DeleteObject(tesselatedMeshTransformNode);

    FinishBounds();
    InitializePlanes(rawPlaneVertices, planeVertices);

    return !this->rawBoundingBox.IsEmpty();
}
