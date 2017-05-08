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
#include "MatrixAccessor.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
MatrixAccessor::MatrixAccessor() : matrix(TRUE)
{
}

MatrixAccessor::MatrixAccessor(const Matrix3& matrix)
{
    this->matrix = matrix;
}

void MatrixAccessor::Invert()
{
    this->matrix.Invert();
}

MatrixAccessor MatrixAccessor::Inverted() const
{
    return Inverse(this->matrix);
}

void MatrixAccessor::TransformPoint(FinjinVector3& v) const
{
    Point3 maxVector(v.x, v.y, v.z);
    maxVector = this->matrix * maxVector;
    v.Set(maxVector.x, maxVector.y, maxVector.z);
}

void MatrixAccessor::TransformNormal(FinjinVector3& v) const
{
    Point3 maxVector(v.x, v.y, v.z);
    maxVector = this->matrix * maxVector;
    v.Set(maxVector.x, maxVector.y, maxVector.z);
    v.Normalize();
}

void MatrixAccessor::GetExportMatrix44(float m44[4][4]) const
{
    for (int i = 0; i < 4; i++)
    {
        auto row = this->matrix.GetRow(i);
        memcpy(&m44[i], &row, sizeof(float) * 3);
        m44[i][3] = (i < 3) ? 0 : 1;
    }
}
