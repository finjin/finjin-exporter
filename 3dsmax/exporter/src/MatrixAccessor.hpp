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
#include "FinjinVector.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Access to a matrix. */
    class MatrixAccessor
    {
    public:
        /** Default constructor. */
        MatrixAccessor();

        /** Constructor that takes a matrix. */
        MatrixAccessor(const Matrix3& matrix);

        /** Inverts the matrix in place. */
        void Invert();

        /** Gets the inverted matrix. */
        MatrixAccessor Inverted() const;

        /** Transforms the specified point. */
        void TransformPoint(FinjinVector3& v) const;

        /** Transforms the specified normal. */
        void TransformNormal(FinjinVector3& v) const;

        void GetExportMatrix44(float m44[4][4]) const;

    public:
        Matrix3 matrix;
    };

} }
