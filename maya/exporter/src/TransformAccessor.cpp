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
#include "TransformAccessor.hpp"
#include "MayaUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
TransformAccessor::TransformAccessor()
{
    this->transform = MTransformationMatrix::identity;
}

TransformAccessor::TransformAccessor(const MTransformationMatrix& transform)
{
    this->transform = transform;
}

TransformAccessor::TransformAccessor(const TransformAccessor& transform, const CoordinateSystemConverter& coordinateConverter, float scale)
{
    Set(transform, coordinateConverter, scale);
}

void TransformAccessor::Set(const TransformAccessor& transform, const CoordinateSystemConverter& coordinateConverter, float scale)
{
    this->transform = transform.transform;
    coordinateConverter.ConvertTransformation(this->transform);

    MVector translation = this->transform.translation(GET_TRANSLATION_SPACE);
    translation *= scale;
    this->transform.setTranslation(translation, SET_TRANSLATION_SPACE);
}

void TransformAccessor::SetIdentity()
{
    this->transform = MTransformationMatrix::identity;
}

FinjinVector3 TransformAccessor::GetTranslation() const
{
    MVector v = this->transform.translation(GET_TRANSLATION_SPACE);
    return FinjinVector3(v.x, v.y, v.z);
}

void TransformAccessor::SetTranslation(const FinjinVector3& v)
{
    this->transform.setTranslation(MVector(v.x, v.y, v.z), SET_TRANSLATION_SPACE);
}

void TransformAccessor::RemoveTranslation()
{
    this->transform.setTranslation(MVector(0, 0, 0), SET_TRANSLATION_SPACE);
}

FinjinVector3 TransformAccessor::GetScale() const
{
    double v[3];
    this->transform.getScale(v, GET_SCALE_SPACE);
    return FinjinVector3(v[0], v[1], v[2]);
}

void TransformAccessor::SetScale(const FinjinVector3& s)
{
    double v[3] = {s.x, s.y, s.z};
    this->transform.setScale(v, SET_SCALE_SPACE);
}

FinjinVector4 TransformAccessor::GetRotationQuaternion() const
{
    MQuaternion v = this->transform.rotation();
    return FinjinVector4(v.x, v.y, v.z, v.w);
}

void TransformAccessor::SetRotationQuaternion(const FinjinVector4& v)
{
    this->transform.setRotationQuaternion(v.x, v.y, v.z, v.w);
}

void TransformAccessor::GetRotationAngleAxis(float& angle, FinjinVector3& axis) const
{
    MQuaternion mayaQuat = this->transform.rotation();
    MVector mayaAxis;
    double mayaAngle;
    mayaQuat.getAxisAngle(mayaAxis, mayaAngle);
    angle = mayaAngle;
    axis.Set(mayaAxis.x, mayaAxis.y, mayaAxis.z);
}

FinjinVector3 TransformAccessor::GetRotationAngles() const
{
    MEulerRotation eulerRotation = this->transform.eulerRotation();
    return FinjinVector3(eulerRotation.x, eulerRotation.y, eulerRotation.z);
}

void TransformAccessor::Invert()
{
    this->transform = this->transform.asMatrixInverse();
}

TransformAccessor TransformAccessor::Inverted() const
{
    return MTransformationMatrix(this->transform.asMatrixInverse());
}

TransformAccessor TransformAccessor::GetNormalTransform() const
{
    //Get transform with translations removed
    MTransformationMatrix newTransform = this->transform;
    newTransform.setTranslation(MVector(0, 0, 0), SET_TRANSLATION_SPACE);

    //Get inverse transpose
    MMatrix m = newTransform.asMatrix();
    m = MayaUtilities::InverseTranspose(m);
    newTransform = MTransformationMatrix(m);

    //Done
    return newTransform;
}

TransformAccessor TransformAccessor::GetRelativeTo(const TransformAccessor& parentTransform) const
{
    return MTransformationMatrix(this->transform.asMatrix() * parentTransform.transform.asMatrixInverse());
}

void TransformAccessor::MakeTransformMatrices(MatrixAccessor& transformMatrix, MatrixAccessor& normalTransformMatrix) const
{
    //Get standard transformation matrix
    transformMatrix.matrix = this->transform.asMatrix();

    //Get normal transformation matrix
    MTransformationMatrix normalTransform(this->transform);
    normalTransform.setTranslation(MVector(0, 0, 0), SET_TRANSLATION_SPACE);
    normalTransformMatrix.matrix = normalTransform.asMatrixInverse().transpose();
}

void TransformAccessor::TransformPoint(FinjinVector3& v) const
{
    MPoint mayaVector(v.x, v.y, v.z);
    mayaVector *= this->transform.asMatrix();
    v.Set(mayaVector.x, mayaVector.y, mayaVector.z);
}

void TransformAccessor::Concatenate(const TransformAccessor& other)
{
    MMatrix result = this->transform.asMatrix() * other.transform.asMatrix();
    this->transform = MTransformationMatrix(result);
}

void TransformAccessor::Concatenate(const MatrixAccessor& other)
{
    MMatrix result = this->transform.asMatrix() * other.matrix;
    this->transform = MTransformationMatrix(result);
}

MatrixAccessor TransformAccessor::GetMatrix() const
{
    return this->transform.asMatrix();
}

TransformAccessor TransformAccessor::GetCameraIdentityTransform()
{
    return MTransformationMatrix::identity;
}
