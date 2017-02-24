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
#include "MaxMathUtilities.hpp"

using namespace Finjin::Exporter;


//Local functions--------------------------------------------------------------
/** 
 * Gets the transpose of the specified matrix. 
 * Due to the fact that 3DS Max uses 4x3 matrices, the returned matrix 
 * does not contain translation components.
 */
static Matrix3 Transpose(const Matrix3& matrix)
{
    Matrix3 transpose;
    Point4 row;

    for (int rowIndex = 0; rowIndex < 3; rowIndex++)
    {
        row = Point4(matrix.GetRow(rowIndex), 0); 
        transpose.SetColumn(rowIndex, row);
    }
        
    return transpose;
}

/** 
 * Gets the inverse-transpose of the specified matrix. 
 * Due to the fact that 3DS Max uses 4x3 matrices, the returned matrix 
 * does not contain translation components.
 */
static Matrix3 InverseTranspose(const Matrix3& worldMatrix)
{
    Matrix3 inverse = Inverse(worldMatrix);
    return Transpose(inverse);
}


//Implementation---------------------------------------------------------------
float MaxMathUtilities::Average(const Point3& xyz)
{
    return (xyz.x + xyz.y + xyz.z) / 3;
}

float MaxMathUtilities::GetBoxHalfSize(const Box3& box)
{
    return Average(box.Width()) / 2;
}

Point4 MaxMathUtilities::CreatePlane(const Point3& p0, const Point3& p1, const Point3& p2)
{
    Point3 normal = CreateNormal(p0, p1, p2);

    return Point4(normal.x, normal.y, normal.z, -DotProd(normal, p0));
}

Point3 MaxMathUtilities::CreateNormal(const Point3& p0, const Point3& p1, const Point3& p2)
{
    Point3 v0 = p1 - p0;
    Point3 v1 = p2 - p1;

    Point3 result = CrossProd(v0, v1);
    result = Normalize(result);
    return result;
}

void MaxMathUtilities::Abs(Point3& p)
{
    p.x = fabs(p.x);
    p.y = fabs(p.y);
    p.z = fabs(p.z);
}

Point3 MaxMathUtilities::Lerp(const Point3& p0, const Point3& p1, float t)
{
    return p0 + (p1 - p0) * t;
}

void MaxMathUtilities::SwapRow(Matrix3& m, int row1, int row2)
{
    Point3 oldRow1 = m.GetRow(row1);
    m.SetRow(row1, m.GetRow(row2));
    m.SetRow(row2, oldRow1);
}

Matrix3 MaxMathUtilities::CreateNormalTM(const Matrix3& matrix)
{
    Matrix3 normalTM = matrix;
    normalTM.NoTrans();
    normalTM = InverseTranspose(normalTM);
    return normalTM;
}

Matrix3 MaxMathUtilities::GetObjectOffsetTM(INode* node, TimeValue t)
{
    Matrix3 objectTM;
    /*if (node->GetObjTMAfterWSM(t).IsIdentity()) 
        objectTM = Inverse(node->GetObjTMBeforeWSM(t)); 
    else*/
        objectTM = node->GetObjectTM(t); 
    
    Matrix3 objectOffsetTM = objectTM * Inverse(node->GetNodeTM(t));
    return objectOffsetTM;
}

void MaxMathUtilities::GetDirections
    (
    const Matrix3& rotation, 
    Point3& rightDirection, 
    Point3& upDirection,
    Point3& forwardDirection
    )
{
    //Get appropriate row vectors
    rightDirection = rotation.GetRow(0);
    upDirection = rotation.GetRow(1);
    forwardDirection = -rotation.GetRow(2);
}
