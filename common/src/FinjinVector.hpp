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


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** A 2-vector. */
    struct FinjinVector2
    {
        FinjinVector2()
        {
            Zero();
        }

        explicit FinjinVector2(float value)
        {
            this->x = value;
            this->y = value;
        }

        FinjinVector2(float x, float y)
        {
            this->x = x;
            this->y = y;
        }

        void Set(float x, float y)
        {
            this->x = x;
            this->y = y;
        }

        void Zero()
        {
            this->x = this->y = 0;
        }

        bool IsZero() const
        {
            return this->x == 0 && this->y == 0;
        }

        bool AlmostZero(float tolerance = 0.0001f) const
        {
            return fabsf(this->x) <= tolerance && fabsf(this->y) <= tolerance;
        }

        bool AlmostEquals(const FinjinVector2& v, float tolerance = 0.0001f) const
        {
            return (*this - v).AlmostZero(tolerance);
        }

        float GetX() const {return this->x;}
        float GetY() const {return this->y;}

        float Average() const
        {
            return (this->x + this->y) / 2;
        }

        FinjinVector2 operator - (const FinjinVector2& v) const
        {
            return FinjinVector2(this->x - v.x, this->y - v.y);
        }

        bool operator == (const FinjinVector2& v) const
        {
            return this->x == v.x && this->y == v.y;
        }

        std::array<float, 2> GetArray() const
        {
            std::array<float, 2> result;
            result[0] = x;
            result[1] = y;            
            return result;
        }

        float operator [] (size_t i) const
        {
            return (&this->x)[i];
        }

        float& operator [] (size_t i)
        {
            return (&this->x)[i];
        }

        float x,y;
    };

    /** A 3-vector. */
    struct FinjinVector3
    {
        static const FinjinVector3 UNIT_X;
        static const FinjinVector3 UNIT_Y;
        static const FinjinVector3 UNIT_Z;
        static const FinjinVector3 ZERO;

        FinjinVector3()
        {
            Zero();
        }

        explicit FinjinVector3(float value)
        {
            this->x = value;
            this->y = value;
            this->z = value;
        }

        FinjinVector3(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        void Set(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        void Zero()
        {
            this->x = this->y = this->z = 0;
        }
        
        bool IsZero() const
        {
            return this->x == 0 && this->y == 0 && this->z == 0;
        }

        bool IsOne() const
        {
            return this->x == 1 && this->y == 1 && this->z == 1;
        }

        bool AlmostZero(float tolerance = 0.0001f) const
        {
            return fabsf(this->x) <= tolerance && fabsf(this->y) <= tolerance && fabsf(this->z) <= tolerance;
        }

        bool AlmostEquals(const FinjinVector3& v, float tolerance = 0.0001f) const
        {
            return (*this - v).AlmostZero(tolerance);
        }

        float GetX() const {return this->x;}
        float GetY() const {return this->y;}
        float GetZ() const {return this->z;}

        float Average() const
        {
            return (this->x + this->y + this->z) / 3;
        }

        FinjinVector3& operator *= (float value)
        {
            this->x *= value;
            this->y *= value;
            this->z *= value;
            return *this;
        }

        FinjinVector3& operator *= (const FinjinVector3& v)
        {
            this->x *= v.x;
            this->y *= v.y;
            this->z *= v.z;
            return *this;
        }

        FinjinVector3 operator * (float v) const
        {
            return FinjinVector3(this->x * v, this->y * v, this->z * v);
        }

        FinjinVector3 operator * (const FinjinVector3& v) const
        {
            return FinjinVector3(this->x * v.x, this->y * v.y, this->z * v.z);
        }

        FinjinVector3 operator / (const FinjinVector3& v) const
        {
            return FinjinVector3(this->x / v.x, this->y / v.y, this->z / v.z);
        }
        
        FinjinVector3 operator / (float value) const
        {
            return FinjinVector3(this->x / value, this->y / value, this->z / value);
        }

        FinjinVector3& operator /= (float value)
        {
            this->x /= value;
            this->y /= value;
            this->z /= value;
            return *this;
        }

        FinjinVector3 operator + (const FinjinVector3& v) const
        {
            return FinjinVector3(this->x + v.x, this->y + v.y, this->z + v.z);
        }

        void operator += (const FinjinVector3& v)
        {
            this->x += v.x;
            this->y += v.y;
            this->z += v.z;
        }

        FinjinVector3 operator - () const
        {
            return FinjinVector3(-this->x, -this->y, -this->z);
        }

        FinjinVector3 operator - (const FinjinVector3& v) const
        {
            return FinjinVector3(this->x - v.x, this->y - v.y, this->z - v.z);
        }

        void operator -= (const FinjinVector3& v)
        {
            this->x -= v.x;
            this->y -= v.y;
            this->z -= v.z;
        }

        bool operator == (const FinjinVector3& v) const
        {
            return this->x == v.x && this->y == v.y && this->z == v.z;
        }

        bool operator != (const FinjinVector3& v) const
        {
            return this->x != v.x || this->y != v.y || this->z != v.z;
        }

        float Dot(const FinjinVector3& v) const
        {
            return this->x * v.x + this->y * v.y + this->z * v.z;
        }

        FinjinVector3 Cross(const FinjinVector3& v) const
        {
            FinjinVector3 result;
            result.x = this->y * v.z - this->z * v.y;
            result.y = this->z * v.x - this->x * v.z;
            result.z = this->x * v.y - this->y * v.x;
            return result;
        }

        void Normalize()
        {
            float length = Length();
            if (length > 0)
            {
                float ooLength = 1.0f / length;
                this->x *= ooLength;
                this->y *= ooLength;
                this->z *= ooLength;
            }
        }

        std::array<float, 3> GetArray() const
        {
            std::array<float, 3> result;
            result[0] = x;
            result[1] = y;
            result[2] = z;
            return result;
        }

        float operator [] (size_t i) const
        {
            return (&this->x)[i];
        }

        float& operator [] (size_t i)
        {
            return (&this->x)[i];
        }

        float LengthSquared() const
        {
            return Dot(*this);
        }

        float Length() const
        {
            return sqrtf(LengthSquared());
        }

        float GetMaxComponent() const
        {
            return std::max(std::max(this->x, this->y), this->z);
        }

        float GetMinComponent() const
        {
            return std::min(std::min(this->x, this->y), this->z);
        }

        static int GetLongest(const FinjinVector3* points, int count, float* length = nullptr)
        {
            int longestIndex = -1;
            float longestLengthSquared = -1;

            for (int i = 0; i < count; i++)
            {
                float lengthSquared = points[i].LengthSquared();
                if (lengthSquared > 0 && lengthSquared > longestLengthSquared)
                {
                    longestIndex = i;
                    longestLengthSquared = lengthSquared;
                }
            }

            if (length != nullptr)
            {
                if (longestLengthSquared > 0)
                    *length = sqrtf(longestLengthSquared);
                else
                    *length = 0;
            }

            return longestIndex;
        }

        static const FinjinVector3& GetUpVector(UpAxis upAxis)
        {
            static const FinjinVector3 Y_AXIS_VECTOR(0, 1, 0);
            static const FinjinVector3 Z_AXIS_VECTOR(0, 0, 1);
            return (upAxis == UpAxis::Y) ? Y_AXIS_VECTOR : Z_AXIS_VECTOR;
        }

        float x,y,z;
    };

    /** A 4-vector. */
    struct FinjinVector4
    {
        FinjinVector4()
        {
            Zero();
        }

        explicit FinjinVector4(float value)
        {
            this->x = value;
            this->y = value;
            this->z = value;
            this->w = value;
        }

        FinjinVector4(float x, float y, float z, float w)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        FinjinVector4(const FinjinVector3& v, float w)
        {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
            this->w = w;
        }

        void Set(float x, float y, float z, float w)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        void Set(const FinjinVector3& v, float w)
        {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
            this->w = w;
        }

        void Zero()
        {
            this->x = this->y = this->z = this->w = 0;
        }

        bool IsZero() const
        {
            return this->x == 0 && this->y == 0 && this->z == 0 && this->w == 0;
        }

        bool AlmostZero(float tolerance = 0.0001f) const
        {
            return fabsf(this->x) <= tolerance && fabsf(this->y) <= tolerance && fabsf(this->z) <= tolerance && fabsf(this->w) <= tolerance;
        }

        bool AlmostEquals(const FinjinVector4& v, float tolerance = 0.0001f) const
        {
            return (*this - v).AlmostZero(tolerance);
        }

        float GetX() const {return this->x;}
        float GetY() const {return this->y;}
        float GetZ() const {return this->z;}
        float GetW() const {return this->w;}

        float Average() const
        {
            return (this->x + this->y + this->z + this->w) / 4;
        }

        FinjinVector4 operator - () const
        {
            return FinjinVector4(-this->x, -this->y, -this->z, -this->w);
        }

        FinjinVector4 operator - (const FinjinVector4& v) const
        {
            return FinjinVector4(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
        }

        bool operator == (const FinjinVector4& v) const
        {
            return this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w;
        }

        std::array<float, 4> GetArray() const
        {
            std::array<float, 4> result;
            result[0] = x;
            result[1] = y;
            result[2] = z;
            result[3] = w;
            return result;
        }

        std::array<float, 3> GetArray3() const
        {
            std::array<float, 3> result;
            result[0] = x;
            result[1] = y;
            result[2] = z;
            return result;
        }

        float operator [] (size_t i) const
        {
            return (&this->x)[i];
        }

        float& operator [] (size_t i)
        {
            return (&this->x)[i];
        }

        float x, y, z, w;
    };

    struct FinjinIVector2
    {
        FinjinIVector2()
        {
            this->x = this->y = 0;
        }

        void Set(int x, int y)
        {
            this->x = x;
            this->y = y;
        }

        bool operator == (const FinjinIVector2& v) const
        {
            return this->x == v.x && this->y == v.y;
        }

        int operator [] (size_t i) const
        {
            return (&this->x)[i];
        }

        int& operator [] (size_t i)
        {
            return (&this->x)[i];
        }

        int x,y;
    };

    typedef MathQuaternion FinjinQuaternion;
    typedef MathMatrix44 FinjinMatrix4;

    struct SampledMeshPoint
    {
        FinjinVector3 position;
        FinjinVector3 normal;
        FinjinVector4 tangent;
    };

    inline float RadiansToDegrees(float rads)
    {
        return rads / 0.01745329252f;
    }

    inline float DegreesToRadians(float degs)
    {
        return degs * 0.01745329252f;
    }

} }
