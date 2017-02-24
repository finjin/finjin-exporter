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
#include "TransformAccessor.hpp"
#include "TimeChanger.hpp"
#include "MayaUtilities.hpp"
#include "ApplicationAccessor.hpp"

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
    this->sourceUpAxis = ApplicationAccessor::GetUpAxis();
    this->upAxis = upAxis;

    //Determine world direction vectors and conversion matrix 
    this->rightVector = MPoint(1,0,0);
    this->mayaToOutputMatrix.setToIdentity();        
    if (this->upAxis == UpAxis::Z)
    {
        this->forwardVector = MPoint(0,1,0);
        this->upVector = MPoint(0,0,1);
        this->upVectorPerpendicular = MPoint(0,1,0);

        if (this->upAxis != this->sourceUpAxis)
        {
            //Y to Z conversion
            MTransformationMatrix rotationTransform = MTransformationMatrix::identity;
            rotationTransform.setToRotationAxis(MVector(1,0,0), -Maya::PI/2);
            this->mayaToOutputMatrix = rotationTransform.asMatrix();
        }        
    }
    else
    {
        this->forwardVector = MPoint(0,0,1);
        this->upVector = MPoint(0,1,0);        
        this->upVectorPerpendicular = MPoint(0,0,-1);

        if (this->upAxis != this->sourceUpAxis)
        {
            //Z to Y conversion
            MTransformationMatrix rotationTransform = MTransformationMatrix::identity;
            rotationTransform.setToRotationAxis(MVector(1,0,0), Maya::PI/2);
            this->mayaToOutputMatrix = rotationTransform.asMatrix();
        }            
    }

    this->mayaToOutputMatrixInverse = this->mayaToOutputMatrix;
    this->mayaToOutputMatrixInverse = this->mayaToOutputMatrixInverse.inverse();
}

void CoordinateSystemConverter::ConvertPoint(MPoint& point) const
{
#if FAST_CONVERSION
    if (this->sourceUpAxis != this->upAxis)
    {
        if (this->sourceUpAxis == UpAxis::Z)
        {
            //Z up to Y up
            float newY = point.z;
            float newZ = -point.y;
            point.y = newY;
            point.z = newZ;
        }
        else
        {
            //Y up to Z up
            float newY = -point.z;
            float newZ = point.y;
            point.y = newY;
            point.z = newZ;
        }
    }
#else
    MTransformationMatrix transform = MTransformationMatrix::identity;
    transform.setTranslation(point, SET_TRANSLATION_SPACE);
        
    MMatrix matrix = transform.asMatrix();
    ConvertMatrix(matrix);    
    
    transform = MTransformationMatrix(matrix);
    point = transform.getTranslation(GET_TRANSLATION_SPACE);
#endif
}

void CoordinateSystemConverter::ConvertPoint(FinjinVector3& point) const
{
    MPoint v(point.x, point.y, point.z);
    ConvertPoint(v);
    point.Set(v.x, v.y, v.z);
}

void CoordinateSystemConverter::ConvertRotation(MQuaternion& rotation) const
{
    MTransformationMatrix transform = MTransformationMatrix::identity;
    transform.setRotationOrientation(rotation);

    MMatrix matrix = transform.asMatrix();
    ConvertMatrix(matrix);    
    
    transform = MTransformationMatrix(matrix);
    rotation = transform.rotation();
}

void CoordinateSystemConverter::ConvertRotation(FinjinQuaternion& rotation) const
{
    MQuaternion mayaQuat(rotation.x(), rotation.y(), rotation.z(), rotation.w());
    ConvertRotation(mayaQuat);
    rotation = FinjinQuaternion(mayaQuat.w, mayaQuat.x, mayaQuat.y, mayaQuat.z);
}

void CoordinateSystemConverter::ConvertMatrix(MMatrix& matrix) const
{   
    //This is what the code in this function does: 
    //  matrix = this->mayaToOutputMatrix * matrix * this->mayaToOutputMatrixInverse;

    MMatrix result = this->mayaToOutputMatrix;
    result *= matrix;
    result *= this->mayaToOutputMatrixInverse;
    
    matrix = result;
}

void CoordinateSystemConverter::ConvertTransformation(MTransformationMatrix& transformation) const
{
    MMatrix transformationMatrix = transformation.asMatrix();
    ConvertMatrix(transformationMatrix);
    transformation = MTransformationMatrix(transformationMatrix);
}

void CoordinateSystemConverter::ConvertTransformation(TransformAccessor& transformation) const
{
    ConvertTransformation(transformation.transform);
}

void CoordinateSystemConverter::ConvertAbsoluteComponents(FinjinVector3& mask) const
{
    if (this->upAxis != this->sourceUpAxis)
    {
        //Swap Y and Z
        std::swap(mask.y, mask.z);
    }
}

void CoordinateSystemConverter::ConvertCameraRotation(FinjinVector4& q) const
{
    MQuaternion qq(q.x, q.y, q.z, q.w);
    ConvertCameraRotation(qq);
    q.Set(qq.x, qq.y, qq.z, qq.w);
}

void CoordinateSystemConverter::ConvertLightRotation(FinjinVector4& q) const
{
    MQuaternion qq(q.x, q.y, q.z, q.w);
    ConvertLightRotation(qq);
    q.Set(qq.x, qq.y, qq.z, qq.w);
}

void CoordinateSystemConverter::ConvertCameraRotation(MQuaternion& q) const
{
    if (this->upAxis != this->sourceUpAxis)
    {
        if (this->upAxis == UpAxis::Z)
        {
            //Y to Z conversion
            MQuaternion rotationQuat(Maya::PI/2, MVector(1, 0, 0));
            q = rotationQuat * q;
        }
        else
        {
            //Z to Y conversion
            MQuaternion rotationQuat(-Maya::PI/2, MVector(1, 0, 0));
            q = rotationQuat * q;
        }
    }
}

void CoordinateSystemConverter::ConvertLightRotation(MQuaternion& q) const
{
    if (this->upAxis != this->sourceUpAxis)
    {
        if (this->upAxis == UpAxis::Z)
        {
            //Y to Z conversion
            MQuaternion rotationQuat(Maya::PI/2, MVector(1, 0, 0));
            q = rotationQuat * q;
        }
        else
        {
            //Z to Y conversion
            MQuaternion rotationQuat(-Maya::PI/2, MVector(1, 0, 0));
            q = rotationQuat * q;
        }
    }
    
    //Lights need to face the opposite direction
    MQuaternion rotationQuat2(Maya::PI, MVector(1, 0, 0));
    q = rotationQuat2 * q;    
}

static double Dot(const MPoint& a, const MPoint& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

const MPoint& CoordinateSystemConverter::GetUpVector(const MPoint& normal) const
{
    if (fabs(Dot(normal, this->upVector)) > .99f)
    {
        //Normal and world up vector point in very nearly the same
        //or opposite direction. Use the vector known to be perpendicular
        //to the world up vector
        return this->upVectorPerpendicular;
    }
    else
        return this->upVector;
}

FinjinVector3 CoordinateSystemConverter::GetUpVector(const FinjinVector3& normal) const
{
    const MPoint& up = GetUpVector(MPoint(normal.x, normal.y, normal.z));
    return FinjinVector3(up.x, up.y, up.z);
}

const MPoint& CoordinateSystemConverter::GetAxisVector(int axis) const
{
    static const MPoint ZERO_VECTOR3(0,0,0);

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
    return this->upAxis != this->sourceUpAxis;
}

UpAxis CoordinateSystemConverter::GetUpAxis() const
{
    return this->upAxis;
}
