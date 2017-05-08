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
#include "MaxMathUtilities.hpp"
#include "MaxUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
TransformAccessor::TransformAccessor() : transform(TRUE)
{
    decomp_affine(this->transform, &this->affineParts);
}

TransformAccessor::TransformAccessor(const Matrix3& transform)
{
    this->transform = transform;
    decomp_affine(this->transform, &this->affineParts);
}

TransformAccessor::TransformAccessor(const TransformAccessor& transform, const CoordinateSystemConverter& coordinateConverter, float scale)
{
    Set(transform, coordinateConverter, scale);
}

void TransformAccessor::Set(const TransformAccessor& transform, const CoordinateSystemConverter& coordinateConverter, float scale)
{
    this->transform = transform.transform;
    coordinateConverter.ConvertMatrix(this->transform);
    Point3 translation = this->transform.GetTrans();
    translation *= scale;
    this->transform.SetTrans(translation);

    decomp_affine(this->transform, &this->affineParts);
}

void TransformAccessor::SetIdentity()
{
    this->transform.IdentityMatrix();
    decomp_affine(this->transform, &this->affineParts);
}

FinjinVector3 TransformAccessor::GetTranslation() const
{
    auto& v = this->affineParts.t;
    return FinjinVector3(v.x, v.y, v.z);
}

void TransformAccessor::SetTranslation(const FinjinVector3& v)
{
    this->transform.SetTrans(Point3(v.x, v.y, v.z));
    decomp_affine(this->transform, &this->affineParts);
}

void TransformAccessor::RemoveTranslation()
{
    this->transform.SetTrans(Point3(0, 0, 0));
    decomp_affine(this->transform, &this->affineParts);
}

FinjinVector3 TransformAccessor::GetScale() const
{
    auto& v = this->affineParts.k;
    return FinjinVector3(v.x, v.y, v.z);
}

void TransformAccessor::SetScale(const FinjinVector3& s)
{
    this->transform.SetScale(Point3(s.x, s.y, s.z));
    decomp_affine(this->transform, &this->affineParts);
}

FinjinVector4 TransformAccessor::GetRotationQuaternion() const
{
    auto v = this->affineParts.q;
    v.Invert();
    return FinjinVector4(v.x, v.y, v.z, v.w);
}

void TransformAccessor::SetRotationQuaternion(const FinjinVector4& v)
{
    this->affineParts.q = Quat(v.x, v.y, v.z, v.w);
    this->affineParts.q.Invert();

    UpdateMatrix();
}

void TransformAccessor::GetRotationAngleAxis(float& angle, FinjinVector3& axis) const
{
    auto v = this->affineParts.q;
    v.Invert();
    AngAxis angleAxis(v);

    angle = angleAxis.angle;
    axis.Set(angleAxis.axis.x, angleAxis.axis.y, angleAxis.axis.z);
}

FinjinVector3 TransformAccessor::GetRotationAngles() const
{
    auto v = this->affineParts.q;
    v.Invert();

    FinjinVector3 angles;
    v.GetEuler(&angles.x, &angles.y, &angles.z);
    return angles;
}

void TransformAccessor::Invert()
{
    this->transform.Invert();
    decomp_affine(this->transform, &this->affineParts);
}

TransformAccessor TransformAccessor::Inverted() const
{
    Matrix3 newTransform = this->transform;
    newTransform.Invert();
    return TransformAccessor(newTransform);
}

TransformAccessor TransformAccessor::GetNormalTransform() const
{
    return MaxMathUtilities::CreateNormalTM(this->transform);
}

TransformAccessor TransformAccessor::GetRelativeTo(const TransformAccessor& parentTransform) const
{
    return this->transform * Inverse(parentTransform.transform);
}

void TransformAccessor::MakeTransformMatrices(MatrixAccessor& transformMatrix, MatrixAccessor& normalTransformMatrix) const
{
    //Get standard transformation matrix
    transformMatrix.matrix = this->transform;

    //Get normal transformation matrix
    normalTransformMatrix.matrix = MaxMathUtilities::CreateNormalTM(this->transform);
}

void TransformAccessor::TransformPoint(FinjinVector3& v) const
{
    Point3 maxVector(v.x, v.y, v.z);
    maxVector = this->transform * maxVector;
    v.Set(maxVector.x, maxVector.y, maxVector.z);
}

void TransformAccessor::Concatenate(const TransformAccessor& other)
{
    this->transform *= other.transform;
    decomp_affine(this->transform, &this->affineParts);
}

void TransformAccessor::Concatenate(const MatrixAccessor& other)
{
    this->transform *= other.matrix;
    decomp_affine(this->transform, &this->affineParts);
}

const Matrix3& TransformAccessor::GetMatrix() const
{
    return this->transform;
}

TransformAccessor TransformAccessor::GetCameraIdentityTransform()
{
    Matrix3 m;
    MaxUtilities::GetCameraIdentityMatrix(m);
    return m;
}

void TransformAccessor::UpdateMatrix()
{
    //T F R U K U'
    Matrix3 T = TransMatrix(this->affineParts.t);
    Matrix3 F = ScaleMatrix(Point3(this->affineParts.f, this->affineParts.f, this->affineParts.f));
    Matrix3 R; this->affineParts.q.MakeMatrix(R);
    Matrix3 U; this->affineParts.u.MakeMatrix(U);
    Matrix3 K = ScaleMatrix(this->affineParts.k);
    Matrix3 Uprime = Inverse(U);
    this->transform = T * F * R * U * K * Uprime;
}
