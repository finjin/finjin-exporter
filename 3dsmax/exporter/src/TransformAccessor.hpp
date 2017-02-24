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


#pragma once


//Includes---------------------------------------------------------------------
#include "CoordinateSystemConverter.hpp"
#include "MatrixAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Access to a transformation. */
    class TransformAccessor
    {
    public:
        /** Default constructor. */
        TransformAccessor();

        /** Constructor that takes a transformation. */
        TransformAccessor(const Matrix3& transform);

        /** Constructor that takes a transformation and conversion info. */
        TransformAccessor(const TransformAccessor& transform, const CoordinateSystemConverter& coordinateConverter, float scale);

        /** Sets the transformation and converts it with the conversion info. */
        void Set(const TransformAccessor& transform, const CoordinateSystemConverter& coordinateConverter, float scale);

        /** Sets the identity transformation. */
        void SetIdentity();

        /** Gets the transformation's translation. */
        FinjinVector3 GetTranslation() const;

        /** Sets the transformation's translation. */
        void SetTranslation(const FinjinVector3& v);

        /** Removes the transformation's translation. */
        void RemoveTranslation();

        /** Gets the transformation's scale. */
        FinjinVector3 GetScale() const;

        /** Sets the transformation's scale. */
        void SetScale(const FinjinVector3& s);

        /** Gets the transformation's rotation quaternion. */
        FinjinVector4 GetRotationQuaternion() const;

        /** Gets the transformation's rotation quaternion. */
        void SetRotationQuaternion(const FinjinVector4& v);

        /** Gets the transformation's rotation as an angle and axis. */
        void GetRotationAngleAxis(float& angle, FinjinVector3& axis) const;
        
        /** Gets the transformation's rotation angles. */
        FinjinVector3 GetRotationAngles() const;
        
        /** Inverts the transformation. */
        void Invert();

        /** Gets the inverted transformation. */
        TransformAccessor Inverted() const;

        /** Gets the transformation that can be used to transform normals. */
        TransformAccessor GetNormalTransform() const;

        /** Gets the transformation relative to another transformation. */
        TransformAccessor GetRelativeTo(const TransformAccessor& parentTransform) const;

        /** Creates the transformation matrices that can be used to transform points and normals. */
        void MakeTransformMatrices(MatrixAccessor& transformMatrix, MatrixAccessor& normalTransformMatrix) const;

        /** Transforms the specified point. */
        void TransformPoint(FinjinVector3& v) const;

        /** Concatenates the specified transformation with this one. */
        void Concatenate(const TransformAccessor& other);

        /** Concatenates the specified transformation with this one. */
        void Concatenate(const MatrixAccessor& other);
        
        /** Gets the transformation as a matrix. */
        const Matrix3& GetMatrix() const;

        /** Gets the camera identity transformation. */
        static TransformAccessor GetCameraIdentityTransform();

    private:
        void UpdateMatrix();

    public:
        Matrix3 transform;
        AffineParts affineParts;
    };

} }
