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

    /** A collection of math functions. */
    class MathUtilities
    {
    public:
        /** Determines if the specified value is a power of two. */
        static bool IsPowerOfTwo(unsigned int value);

        /** Gets the next largest power of two for the specified value. */
        static unsigned int NextLargestPowerOfTwo(unsigned int value);

        /** Gets the next smallest power of two for the specified value. */
        static unsigned int NextSmallestPowerOfTwo(unsigned int value);

        /** Converts the horizontal field of view to a vertical one. */
        static float HorizontalToVerticalFOV(float fov, float aspectRatio = 1.333f);

        /** Determiens if the specified value is almost one. */
        static bool AlmostOne(float v, float tolerance = 0.0001f);

        /** Determiens if the specified value is almost zero. */
        static bool AlmostZero(float v, float tolerance = 0.0001f);

        /** Creates a plane from three points. */
        static FinjinVector4 CreatePlane(const FinjinVector3& p0, const FinjinVector3& p1, const FinjinVector3& p2);

        /** Creates a normal from three points. */
        static FinjinVector3 CreateNormal(const FinjinVector3& p0, const FinjinVector3& p1, const FinjinVector3& p2);

        /**
         * Gets the quaternion relative to another one.
         * @param q [in] - The source quaternion.
         * @param parent [in] - The 'parent' quaternion.
         * @return The quaternion q relative to parent is returned.
         */
        static FinjinVector4 QuaternionRelativeTo(const FinjinVector4& q, const FinjinVector4& parent);
    };

} }
