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
#include "MathUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
bool MathUtilities::IsPowerOfTwo(unsigned int value)
{
    return (value & (value - 1)) == 0;
}

unsigned int MathUtilities::NextLargestPowerOfTwo(unsigned int value)
{
    value |= (value >> 1);
    value |= (value >> 2);
    value |= (value >> 4);
    value |= (value >> 8);
    value |= (value >> 16);
    return value + 1;
}

unsigned int MathUtilities::NextSmallestPowerOfTwo(unsigned int value)
{    
    if (!IsPowerOfTwo(value))
    {
        //Not a power of 2. Round value up to the next power of 2
        value = NextLargestPowerOfTwo(value);
    }
 
    //The value is a power of 2. Shift downward to get the next smallest power of 2
    return value >> 1;        
}

float MathUtilities::HorizontalToVerticalFOV(float fov, float aspectRatio)
{
    return 2 * atanf(tanf(fov / 2) / aspectRatio);
}

bool MathUtilities::AlmostOne(float v, float tolerance)
{
    return AlmostZero(v - 1, tolerance);
}

bool MathUtilities::AlmostZero(float v, float tolerance)
{
    return fabs(v) < tolerance;
}

FinjinVector4 MathUtilities::CreatePlane(const FinjinVector3& p0, const FinjinVector3& p1, const FinjinVector3& p2)
{
    auto normal = CreateNormal(p0, p1, p2);
    return FinjinVector4(normal.x, normal.y, normal.z, -normal.Dot(p0));
}

FinjinVector3 MathUtilities::CreateNormal(const FinjinVector3& p0, const FinjinVector3& p1, const FinjinVector3& p2)
{
    auto v0 = p1 - p0;
    auto v1 = p2 - p1;

    auto result = v0.Cross(v1);
    result.Normalize();
    return result;
}

FinjinVector4 MathUtilities::QuaternionRelativeTo(const FinjinVector4& q, const FinjinVector4& parent)
{
    FinjinQuaternion mathQ(q.w, q.x, q.y, q.z);
    FinjinQuaternion mathParentQ(parent.w, parent.x, parent.y, parent.z);
    auto result = mathParentQ.inverse() * mathQ;
    return FinjinVector4(result.x(), result.y(), result.z(), result.w());
}
