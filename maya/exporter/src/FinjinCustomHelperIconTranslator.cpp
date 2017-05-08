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
#include "FinjinCustomHelperIconTranslator.hpp"
#include "CustomHelperIconReaderWriter.hpp"
#include "CoordinateSystemConverter.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
FINJIN_IMPLEMENT_TRANSLATOR(FinjinCustomHelperIconTranslator, "1.0", "chi", "*.chi", "Finjin Custom Helper Icon");


//Implementation----------------------------------------------------------------
MStatus FinjinCustomHelperIconTranslatorBase::writer
    (
    const MFileObject& file,
    const MString& options,
    MPxFileTranslator::FileAccessMode mode
    )
{
    //Make sure the translator isn't being misused
    if (mode != MPxFileTranslator::kExportAccessMode &&
        mode != MPxFileTranslator::kExportActiveAccessMode)
    {
        return MS::kNotImplemented;
    }

    MStatus status = MS::kSuccess;

    MDagPath dagPath;
    bool foundMesh = false;
    MItDag dagIterator(MItDag::kBreadthFirst, MFn::kInvalid, &status);
    for (; !dagIterator.isDone(); dagIterator.next())
    {
        status = dagIterator.getPath(dagPath);
        if (status != MS::kSuccess)
            continue;

        MFnDagNode dagNode(dagPath, &status);
        if (status != MS::kSuccess || dagNode.isIntermediateObject())
            continue;

        if (dagPath.hasFn(MFn::kMesh) && dagPath.hasFn(MFn::kTransform))
            foundMesh = true;
    }

    //Handle mesh
    if (!foundMesh)
        status = MStatus::kFailure;
    else
        status = SaveIcon(dagPath, file.fullName()) ? MStatus::kSuccess : MStatus::kFailure;

    return status;
}

bool FinjinCustomHelperIconTranslatorBase::SaveIcon(MDagPath& dagPath, const MString& fileName)
{
    MStatus status;
    MFnMesh mesh(dagPath, &status);

    CustomHelperIconWriter icon;

    //Get the visible edges
    int edgeCount = mesh.numEdges();
    icon.edges.resize(edgeCount);
    int2 edgeVertexIndices;
    for (int i = 0; i < edgeCount; i++)
    {
        mesh.getEdgeVertices(i, edgeVertexIndices);

        icon.edges[i].v1 = edgeVertexIndices[0];
        icon.edges[i].v2 = edgeVertexIndices[1];
    }

    //Get mesh points
    MFloatPointArray points;
    mesh.getPoints(points, GET_POINTS_NORMALS_SPACE);

    CoordinateSystemConverter coordinateSystemConverter;
    coordinateSystemConverter.Initialize(UpAxis::Z);

    //Put the points into the icon
    icon.vertices.resize(points.length());
    for (unsigned int i = 0; i < points.length(); i++)
    {
        icon.vertices[i].Set(points[i].x, points[i].y, points[i].z);
        coordinateSystemConverter.ConvertPoint(icon.vertices[i]);
    }
    icon.Normalize();

    //Save
    return icon.Write(ApplicationStringToWxString(fileName), wxT("MVector"));
}
