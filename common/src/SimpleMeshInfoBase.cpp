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
#include "SimpleMeshInfoBase.hpp"
#include "MathUtilities.hpp"
#include "ApplicationAccessor.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
SimpleMeshInfoBase::SimpleMeshInfoBase()
{
    ResetBounds();
}

bool SimpleMeshInfoBase::Create(ObjectAccessor object, TimeAccessor time, TransformAccessor* transformation)
{
    CoordinateSystemConverter nullConverter(ApplicationAccessor::GetUpAxis());
    return Create(object, nullConverter, 1, time, transformation);
}

float SimpleMeshInfoBase::GetRawBoundingRadius() const
{
    return this->rawBoundingRadius;
}

const FinjinBox3& SimpleMeshInfoBase::GetRawBoundingBox() const
{
    return this->rawBoundingBox;
}

const FinjinVector4& SimpleMeshInfoBase::GetRawPlane() const
{
    return this->rawPlane;
}

float SimpleMeshInfoBase::GetBoundingRadius() const
{
    return this->boundingRadius;
}

const FinjinBox3& SimpleMeshInfoBase::GetBoundingBox() const
{
    return this->boundingBox;
}

const FinjinVector4& SimpleMeshInfoBase::GetPlane() const
{
    return this->plane;
}

void SimpleMeshInfoBase::ResetBounds()
{
    this->rawBoundingRadius = 0;
    this->rawBoundingBox.Reset();
    this->boundingRadius = 0;
    this->boundingBox.Reset();
}

void SimpleMeshInfoBase::UpdateBounds(bool isFirst, const FinjinVector3& rawVertex, const FinjinVector3& vertex)
{
    if (isFirst)
    {
        this->rawBoundingRadius = rawVertex.LengthSquared();
        this->rawBoundingBox.min = this->rawBoundingBox.max = rawVertex;

        this->boundingRadius = vertex.LengthSquared();
        this->boundingBox.min = this->boundingBox.max = vertex;
    }
    else
    {
        this->rawBoundingRadius = std::max(this->rawBoundingRadius, rawVertex.LengthSquared());
        this->rawBoundingBox.IncludePoint(rawVertex);

        this->boundingRadius = std::max(this->boundingRadius, vertex.LengthSquared());
        this->boundingBox.IncludePoint(vertex);
    }
}

void SimpleMeshInfoBase::FinishBounds()
{
    this->rawBoundingRadius = sqrtf(this->rawBoundingRadius);
    this->boundingRadius = sqrtf(this->boundingRadius);
}

void SimpleMeshInfoBase::InitializePlanes(const FinjinVector3* rawPlaneVertices, const FinjinVector3* planeVertices)
{
    this->rawPlane = MathUtilities::CreatePlane(rawPlaneVertices[0], rawPlaneVertices[1], rawPlaneVertices[2]);
    this->plane = MathUtilities::CreatePlane(planeVertices[0], planeVertices[1], planeVertices[2]);
}
