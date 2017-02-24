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
#include "CoordinateSystemConverter.hpp"
#include "MaxMathUtilities.hpp"
#include "TransformAccessor.hpp"

using namespace Finjin::Exporter;


//Macros-----------------------------------------------------------------------
#define FAST_CONVERSION 1 //1 = special-case point conversion. 0 = generic


//Implementation---------------------------------------------------------------
CoordinateSystemConverter::CoordinateSystemConverter()
{
    Initialize(UpAxis::Y);
}

CoordinateSystemConverter::CoordinateSystemConverter(UpAxis upAxis)
{
    Initialize(upAxis);
}

void CoordinateSystemConverter::Initialize(UpAxis upAxis)
{
    this->upAxis = upAxis;

    //Determine world direction vectors and conversion matrix 
    this->rightVector = Point3(1,0,0);
    if (this->upAxis == UpAxis::Z)
    {
        this->forwardVector = Point3(0,1,0);
        this->upVector = Point3(0,0,1);
        this->upVectorPerpendicular = Point3(0,1,0);

        this->maxToOutputMatrix.IdentityMatrix();        
    }
    else
    {
        this->forwardVector = Point3(0,0,1);
        this->upVector = Point3(0,1,0);        
        this->upVectorPerpendicular = Point3(0,0,-1);

        this->maxToOutputMatrix = RotateXMatrix(Max::PI/2);
    }

    this->maxToOutputMatrixInverse = Inverse(this->maxToOutputMatrix);
}

void CoordinateSystemConverter::ConvertPoint(Point3& point) const
{
#if FAST_CONVERSION
    if (this->upAxis == UpAxis::Y)
    {
        //Z up to Y up
        float newY = point.z;
        float newZ = -point.y;
        point.y = newY;
        point.z = newZ;
    }
#else
    Matrix3 matrix = TransMatrix(point);
    ConvertMatrix(matrix);
    point = matrix.GetTrans();
#endif
}

void CoordinateSystemConverter::ConvertPoint(FinjinVector3& point) const
{
    ConvertPoint((Point3&)point);
}

void CoordinateSystemConverter::ConvertMatrix(Matrix3& matrix) const
{   
    matrix = this->maxToOutputMatrix * matrix * this->maxToOutputMatrixInverse;
}

void CoordinateSystemConverter::ConvertTransformation(TransformAccessor& transformation) const
{
    if (RequiresConversion())
    {
        Matrix3 m = transformation.GetMatrix();
        ConvertMatrix(m);
        transformation = m;
    }
}

void CoordinateSystemConverter::ConvertAbsoluteComponents(FinjinVector3& mask) const
{
    if (this->upAxis == UpAxis::Y)
        std::swap(mask.y, mask.z);
}

void CoordinateSystemConverter::ConvertCameraRotation(FinjinVector4& q) const
{
    Quat qq(q.x, q.y, q.z, q.w);
    qq.Invert();
    ConvertCameraRotation(qq);
    qq.Invert();
    q.Set(qq.x, qq.y, qq.z, qq.w);
}

void CoordinateSystemConverter::ConvertLightRotation(FinjinVector4& q) const
{
    Quat qq(q.x, q.y, q.z, q.w);
    qq.Invert();
    ConvertLightRotation(qq);
    qq.Invert();
    q.Set(qq.x, qq.y, qq.z, qq.w);
}

void CoordinateSystemConverter::ConvertCameraRotation(Quat& q) const
{
    if (this->upAxis == UpAxis::Y)
        q = Quat(AngAxis(1, 0, 0, Max::PI/2)) * q;
}

void CoordinateSystemConverter::ConvertLightRotation(Quat& q) const
{
    //Rotation needs to be adjusted so that it always looks down its z axis
    if (this->upAxis == UpAxis::Y)
        q = Quat(AngAxis(1, 0, 0, Max::PI/2)) * q;

    //Lights need to face the opposite direction
    q = Quat(AngAxis(1, 0, 0, Max::PI)) * q;
}

const Point3& CoordinateSystemConverter::GetUpVector(const Point3& normal) const
{
    if (fabs(DotProd(normal, this->upVector)) > .99f)
    {
        //Normal and world up vector point in very nearly the same
        //or opposite direction. Use the vector known to be perpendicular
        //to the world up vector
        return this->upVectorPerpendicular;
    }
    else
        return this->upVector;
}

const Point3& CoordinateSystemConverter::GetAxisVector(int axis) const
{
    static const Point3 ZERO_VECTOR3(0,0,0);

    switch (axis)
    {
        case 0: return this->rightVector;
        case 1: return this->forwardVector;
        case 2: return this->upVector;
        default : return ZERO_VECTOR3;
    }
}

bool CoordinateSystemConverter::RequiresConversion() const
{
    return this->upAxis != UpAxis::Z;
}

UpAxis CoordinateSystemConverter::GetUpAxis() const
{
    return this->upAxis;
}
