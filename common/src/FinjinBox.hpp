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

    /** A 3D bounding box. */
    struct FinjinBox3
    {
        /** Default constructor. */
        FinjinBox3()
        {
        }

        /** Constructor that takes a box size and center. */
        FinjinBox3(float size, FinjinVector3 center = FinjinVector3(0,0,0))
        {
            SetSize(size, center);
        }

        void Reset()
        {
            this->min.SetZero();
            this->max.SetZero();
        }

        /** Determines if the box is valid. */
        bool IsValid() const
        {
            return !(this->min.IsZero() && this->max.IsZero());
        }

        /** Determines if the box is empty, within some tolerance. */
        bool IsEmpty(float tolerance = 0.0001f) const
        {
            return (this->max - this->min).AlmostZero(tolerance);
        }

        bool operator == (const FinjinBox3& other) const
        {
            return
                this->min == other.min &&
                this->max == other.max
                ;
        }

        bool operator != (const FinjinBox3& other) const
        {
            return !(operator == (other));
        }

        /** Determines if the box is equal to another box, within some tolerance. */
        bool AlmostEquals(const FinjinBox3& v, float tolerance = 0.0001f) const
        {
            return
                (this->min - v.min).AlmostZero(tolerance) &&
                (this->max - v.max).AlmostZero(tolerance);
        }

        /** Gets the box's dimensions, halved. */
        FinjinVector3 GetHalfSize() const
        {
            FinjinVector3 size = GetSize();
            size /= 2;
            return size;
        }

        /** Gets the box's dimensions. */
        FinjinVector3 GetSize() const
        {
            return FinjinVector3(this->max.x - this->min.x, this->max.y - this->min.y, this->max.z - this->min.z);
        }

        /**
         * Sets the box's size.
         * @param size [in] - The size of the box, from one face to the opposite face.
         * @param center [in] - The center of the box.
         */
        void SetSize(float size, FinjinVector3 center = FinjinVector3(0,0,0))
        {
            float halfSize = size/2;
            this->min.Set(halfSize - center.x, halfSize - center.y, halfSize - center.z);
            this->max.Set(halfSize + center.x, halfSize + center.y, halfSize + center.z);
        }

        /** Gets the center of the box. */
        FinjinVector3 GetCenter() const
        {
            return FinjinVector3((this->max.x + this->min.x)/2, (this->max.y + this->min.y)/2, (this->max.z + this->min.z)/2);
        }

        /** Modifies the box to include the specified point. */
        void IncludePoint(const FinjinVector3& point)
        {
            if (point.x < this->min.x)
                this->min.x = point.x;
            else if (point.x > this->max.x)
                this->max.x = point.x;

            if (point.y < this->min.y)
                this->min.y = point.y;
            else if (point.y > this->max.y)
                this->max.y = point.y;

            if (point.z < this->min.z)
                this->min.z = point.z;
            else if (point.z > this->max.z)
                this->max.z = point.z;
        }

        /**
         * Modifies the box to include the specified points.
         * @param points [in] - An array of points.
         * @param count [in] - The number of points in the array.
         * @param initialize [in] - Indicates whether this is being called to initialize the box.
         * If true, the existing box settings are ignored and replaced with a new box that
         * bounds just the specified points.
         */
        void IncludePoints(const FinjinVector3* points, int count, bool initialize = false)
        {
            int startIndex = 0;
            if (initialize)
            {
                if (count > 0)
                {
                    this->min = this->max = points[0];
                    startIndex++;
                }
                else
                    Reset();
            }

            for (int i = startIndex; i < count; i++)
                IncludePoint(points[i]);
        }

        /** Modifies this box to include the specified box's points. */
        void IncludeBox(const FinjinBox3& box)
        {
            IncludePoint(box.min);
            IncludePoint(box.max);
        }

        /**
         * Gets the corners of the box as points.
         * @param points [out] - An array of points. This array must have at least 8 elements.
         */
        void GetCornerPoints(FinjinVector3* points) const
        {
            points[0].Set(min.x, min.y, min.z);
            points[1].Set(max.x, min.y, min.z);
            points[2].Set(max.x, max.y, min.z);
            points[3].Set(min.x, max.y, min.z);
            points[4].Set(min.x, min.y, max.z);
            points[5].Set(max.x, min.y, max.z);
            points[6].Set(max.x, max.y, max.z);
            points[7].Set(min.x, max.y, max.z);
        }

        /**
         * Gets the corners of the box as points.
         * @param points [out] - An array of points. This array must have at least 6 elements.
         */
        void GetSpherePoints(FinjinVector3* points) const
        {
            points[0].Set(min.x, 0, 0);
            points[1].Set(max.x, 0, 0);
            points[2].Set(0, min.y, 0);
            points[3].Set(0, max.y, 0);
            points[4].Set(0, 0, min.z);
            points[5].Set(0, 0, max.z);

            FinjinVector3 center = GetCenter();
            for (int i = 0; i < 6; i++)
                points[i] += center;
        }

        FinjinVector3 min;
        FinjinVector3 max;
    };

} }
