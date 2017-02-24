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

    /** An float-based RGBA color. */
    struct FinjinColor
    {
        FinjinColor() { Set(0, 0, 0, 1); }
        FinjinColor(float r, float g, float b, float a = 1) { Set(r, g, b, a); }
        FinjinColor(const wxColor& wxcolor) { Set(wxcolor.Red() / 255.f, wxcolor.Green() / 255.f, wxcolor.Blue() / 255.f, wxcolor.Alpha() / 255.f); }
        FinjinColor(const wxColor& wxcolor, float alpha) { Set(wxcolor.Red() / 255.f, wxcolor.Green() / 255.f, wxcolor.Blue() / 255.f, alpha); }
        FinjinColor(const FinjinVector4& v) { Set(v.x, v.y, v.z, v.w); }

        void Set(float r, float g, float b, float a = 1)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;            
        }

        FinjinColor& operator *= (float value)
        {
            this->r *= value;
            this->g *= value;
            this->b *= value;
            this->a *= value;
            return *this;
        }

        bool operator == (const FinjinColor& v) const
        {
            return this->r == v.r && this->g == v.g && this->b == v.b && this->a == v.a;
        }

        bool operator != (const FinjinColor& v) const
        {
            return this->r != v.r || this->g != v.g || this->b != v.b || this->a != v.a;
        }

        std::array<float, 3> GetRGBArray() const
        {
            std::array<float, 3> result;
            result[0] = this->r;
            result[1] = this->g;
            result[2] = this->b;
            return result;
        }

        std::array<float, 4> GetRGBAArray() const
        {
            std::array<float, 4> result;
            result[0] = this->r;
            result[1] = this->g;
            result[2] = this->b;
            result[3] = this->a;
            return result;
        }

        float operator [] (size_t i) const { return (&this->r)[i]; }
        float& operator [] (size_t i) { return (&this->r)[i]; }

        bool IsBlack() const { return this->r == 0 && this->g == 0 && this->b == 0; }

        /** Determines whether all the color components are almost zero. */
        bool AlmostZero(float tolerance = 0.0001f) const
        {
            return fabsf(this->r) <= tolerance && fabsf(this->r) <= tolerance && fabsf(this->g) <= tolerance && fabsf(this->a) <= tolerance;
        }

        /** Determines if two colors are almost equal. */
        bool AlmostEquals(const FinjinColor& v, float tolerance = 0.0001f) const
        {
            FinjinColor diff(this->r - v.r, this->g - v.g, this->b - v.b, this->a - v.a);
            return diff.AlmostZero(tolerance);
        }

        /** Limits all the color components to [0, 1]. */
        void Saturate()
        {
            Saturate(this->r);
            Saturate(this->g);
            Saturate(this->b);
            Saturate(this->a);
        }
        
        /** Creates a new color that's had its color components limited to [0, 1]. */
        FinjinColor Saturated() const
        {
            FinjinColor color = *this;
            color.Saturate();
            return color;
        }

        /** Limits the specified value to [0, 1]. */
        static void Saturate(float& value)
        {
            if (value < 0)
                value = 0;
            else if (value > 1)
                value = 1;
        }

        /** Implicitly converts this color to a wxColor. */
        operator wxColor () const
        {
            wxColor wxcolor;
            wxcolor.Set
                (
                wxColor::ChannelType(this->r * 255),
                wxColor::ChannelType(this->g * 255),
                wxColor::ChannelType(this->b * 255),
                wxColor::ChannelType(this->a * 255)
                );
            return wxcolor;
        }

        /** Implicitly converts this color to an FinjinVector4. */
        operator FinjinVector4 () const
        {
            return FinjinVector4(this->r, this->g, this->b, this->a);
        }

        float r, g, b, a;
    };

} }
