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


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Various math utility functions. */
    class MaxMathUtilities
    {
    public:
        /** Returns the average of the point's 3 components. */
        static float Average(const Point3& xyz);

        /** Returns half the average size of the dimensions of box. */
        static float GetBoxHalfSize(const Box3& box);

        /** Creates a plane from the specified triangle points. */
        static Point4 CreatePlane(const Point3& p0, const Point3& p1, const Point3& p2);

        /** Creates a normal from the specified triangle points. */
        static Point3 CreateNormal(const Point3& p0, const Point3& p1, const Point3& p2);

        /** Makes all of the point's components positive. */
        static void Abs(Point3& p);

        /** Interpolates the two points using the 't' parameter. */
        static Point3 Lerp(const Point3& p0, const Point3& p1, float t);

        /** Swaps two rows in the specified matrix. */
        static void SwapRow(Matrix3& m, int row1, int row2);

        /**
         * Creates the transformation that can be used to transform normals.
         * @param matrix [in] - The matrix that can be used to transform points.
         */
        static Matrix3 CreateNormalTM(const Matrix3& matrix);

        static Matrix3 GetObjectOffsetTM(INode* node, TimeValue time = 0);

        /**
         * Gets the direction vectors from a transform matrix.
         * @param rotation [in] - The transform matrix.
         * @param rightDirection [out] - The local right direction.
         * @param upDirection [out] - The local up direction.
         * @param forwardDirection [out] - The local forward direction.
         */
        static void GetDirections
            (
            const Matrix3& rotation,
            Point3& rightDirection,
            Point3& upDirection,
            Point3& forwardDirection
            );
    };

} }
