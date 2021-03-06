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


//Includes----------------------------------------------------------------------
#include "FinjinVector.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class TransformAccessor;

    /**
     * Performs conversion from application coordinates to some other coordinate system.
     */
    class CoordinateSystemConverter
    {
    public:
        /** Initializes the conversion manager with the 'y up' axis. */
        CoordinateSystemConverter();

        /** Initializes the conversion manager for the specified up axis. */
        CoordinateSystemConverter(UpAxis upAxis);

        /** Initializes the conversion manager for the specified up axis. */
        void Initialize(UpAxis upAxis);

        /** Converts the specified point. */
        void ConvertPoint(MPoint& point) const;

        /** Converts the specified point. */
        void ConvertPoint(FinjinVector3& point) const;

        /** Converts the specified rotation. */
        void ConvertRotation(MQuaternion& rotation) const;

        /** Converts the specified rotation. */
        void ConvertRotation(FinjinQuaternion& rotation) const;

        /** Converts the specified matrix. */
        void ConvertMatrix(MMatrix& matrix) const;

        /** Converts the specified transformation. */
        void ConvertTransformation(MTransformationMatrix& transformation) const;

        /** Converts the specified transformation. */
        void ConvertTransformation(TransformAccessor& transformation) const;

        /**
         * Converts the specified translation mask.
         * @param mask [in/out] - The translation mask. A translation mask is vector of boolean values (0 or 1)
         * indicating which translations to remove (0 value) or keep (1 value). The mask is assumed to
         * be specified relative to 3DS Max's coordinate system
         */
        void ConvertAbsoluteComponents(FinjinVector3& mask) const;

        /** Converts the specified camera rotation. */
        void ConvertCameraRotation(FinjinVector4& q) const;

        /** Converts the specified light rotation. */
        void ConvertLightRotation(FinjinVector4& q) const;

        /** Converts the specified camera rotation. */
        void ConvertCameraRotation(MQuaternion& q) const;

        /** Converts the specified light rotation. */
        void ConvertLightRotation(MQuaternion& q) const;

        /**
         * Gets one of the axis vectors.
         * @param axis [in] - Zero-based index of the axis to retrieve:
         *   0 - Right axis
         *   1 - Forward axis
         *   2 - Up axis
         */
        const MPoint& GetAxisVector(int axis) const;

        /** Gets the coordinate system's 'right' vector. */
        const MPoint& GetRightVector() const {return this->rightVector;}

        /** Gets the coordinate system's 'forward' vector. */
        const MPoint& GetForwardVector() const {return this->forwardVector;}

        /** Gets the coordinate system's 'up' vector. */
        const MPoint& GetUpVector() const {return this->upVector;}

        /** Gets the up vector relative to the specified normal. */
        const MPoint& GetUpVector(const MPoint& normal) const;

        /** Gets the up vector relative to the specified normal. */
        FinjinVector3 GetUpVector(const FinjinVector3& normal) const;

        /**
         * Determines if the current conversion setting requires vertices, normals, etc
         * to be converted. This is provided so that callers can have some optimized
         * code paths that don't do conversion.
         */
        bool RequiresConversion() const;

        /** Gets the up axis that was used to initialize the conversion manager. */
        UpAxis GetUpAxis() const;

    private:
        MMatrix mayaToOutputMatrix;
        MMatrix mayaToOutputMatrixInverse;
        UpAxis sourceUpAxis;
        UpAxis upAxis;

        MPoint rightVector;
        MPoint forwardVector;
        MPoint upVector;
        MPoint upVectorPerpendicular;
    };

} }
