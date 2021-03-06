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
#include "PhysicalObjectExportSettings.hpp"
#include "GeometryState.hpp"
#include "MayaUtilities.hpp"
#include "FinjinSceneExporterContext.hpp"
#include "SceneExportSettings.hpp"
#include "SimpleMeshInfo.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
PhysicalObjectExportSettings::PhysicalObjectExportSettings(FinjinSceneDocument* scene) : FinjinSceneDocument_Physical(scene)
{
}

void PhysicalObjectExportSettings::Initialize(ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, FinjinSceneExporterContext& finjinExporterContext)
{
    finjinExporterContext.GetMovableObjectSettings(this, object, objectSettings, finjinExporterContext.GetSceneExportSettings()->time, finjinExporterContext.GetSceneExportSettings()->conversionManager, finjinExporterContext.GetSceneExportSettings()->scale);

    //Item type
    this->type = objectSettings.GetItemType();

    //Bounding volume
    FinjinIVector2 unusedSegmentCount;
    FinjinVector3 size;
    GeometryState geometryState;
    if (MayaUtilities::IsPlane(object.obj))
    {
        //Plane
        SimpleMeshInfo simpleMeshInfo;
        simpleMeshInfo.Create(object, finjinExporterContext.GetSceneExportSettings()->conversionManager, finjinExporterContext.GetSceneExportSettings()->scale, finjinExporterContext.GetSceneExportSettings()->time);

        //Type
        this->shape.type = Shape::PLANE;

        //Radius
        this->shape.plane = simpleMeshInfo.GetPlane();
    }
    else if (MayaUtilities::GetSphereRadiusAndSegmentCount(this->shape.radius, unusedSegmentCount, object.obj))
    {
        //Sphere bounding volume

        //Type
        this->shape.type = Shape::SPHERE;

        //Radius
        this->shape.radius *= finjinExporterContext.GetSceneExportSettings()->scale;
    }
    else if (MayaUtilities::GetBoxSize(size, object.obj))
    {
        //Box bounding volume

        //Type
        this->shape.type = Shape::BOX;

        //Size
        finjinExporterContext.GetSceneExportSettings()->conversionManager.ConvertAbsoluteComponents(size);
        size *= finjinExporterContext.GetSceneExportSettings()->scale;
        this->shape.size.Set(size.x, size.y, size.z);
    }
    else if (MayaUtilities::GetCylinderRadiusAndLength(this->shape.radius, this->shape.size.x, object.obj))
    {
        //Cylinder bounding volume

        //Type
        this->shape.type = Shape::CYLINDER;

        //Length
        this->shape.size.x *= finjinExporterContext.GetSceneExportSettings()->scale;

        //Radius
        this->shape.radius *= finjinExporterContext.GetSceneExportSettings()->scale;
    }
    else if
        (
        geometryState.Create(wxEmptyString, object, finjinExporterContext.GetSceneExportSettings()->conversionManager, finjinExporterContext.GetSceneExportSettings()->scale, finjinExporterContext.GetSceneExportSettings()->time, GeometryStateFlags::FAST_VERTICES) &&
        geometryState.submeshes.front()->geometryType == GeometryStateSubmesh::MESH
        )
    {
        //Mesh bounding volume

        //Use object offset
        geometryState.TransformVertices(this->objectOffsetTransform);

        //Type
        this->shape.type = Shape::MESH;

        //Faces
        auto submesh = geometryState.submeshes.front().get();
        size_t faceCount = submesh->indices.size() / 3;
        this->shape.meshFaces.resize(faceCount);

        auto vertexIterator = submesh->vertexList.begin();
        for (size_t faceIndex = 0; faceIndex < faceCount; faceIndex++)
        {
            for (size_t vertexIndex = 0; vertexIndex < 3; vertexIndex++)
            {
                this->shape.meshFaces[faceIndex].vertex[vertexIndex] = vertexIterator->position;
                ++vertexIterator;
            }
        }
    }
}
