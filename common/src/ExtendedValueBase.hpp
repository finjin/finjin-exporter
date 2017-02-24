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
#include "ObjectAccessor.hpp"
#include "MaterialAccessor.hpp"
#include "FinjinColor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * A single extended value, which can actually contain an array of values.
     * 
     * @see ExtendedValueAccessor
     */
    class ExtendedValueBase
    {
    public:
        /** The type of data stored in the extended value. */
        enum Type
        {
            /** No value. */
            EXTENDED_NONE,

            /* An integer or array of integers. */
            EXTENDED_INT,

            /* An float or array of floats. */
            EXTENDED_FLOAT,

            /** A color or array of colors. Colors are stored as floats. */
            EXTENDED_COLOR,

            /** A string or array of strings. */
            EXTENDED_STRING,

            /** An object or array of objects. */
            EXTENDED_OBJECT,

            /** A material or array of materials. */
            EXTENDED_MATERIAL
        };

        ExtendedValueBase();
        virtual ~ExtendedValueBase() {}

        /** Gets the extended value's type. */
        Type GetType() const;
        
        /** Gets the type formatted as a string. */
        wxString GetTypeString() const;

        /** Parses the specified type string */
        static Type ParseType(const wxString& text);

        /** Gets the number of values stored in the extended value. */
        int GetCount() const;

        //Getter methods-----------------------------------
        void Get(int& value) const;
        void Get(int* values, int& count) const;
        void Get(std::vector<int>& values) const;
        void Get(float& value) const;
        void Get(float* values, int& count) const;
        void Get(std::vector<float>& values) const;
        void Get(FinjinColor& value) const;
        void Get(FinjinColor* values, int& count) const;
        void Get(std::vector<FinjinColor>& values) const;
        void Get(wxString& value) const;
        void Get(wxString* values, int& count) const;
        void Get(std::vector<wxString>& values) const;
        void Get(ObjectAccessor& value) const;
        void Get(ObjectAccessor* values, int& count) const;
        void Get(std::vector<ObjectAccessor>& values) const;
        void Get(MaterialAccessor& value) const;
        void Get(MaterialAccessor* values, int& count) const;
        void Get(std::vector<MaterialAccessor>& values) const;

        //Setter methods-----------------------------------
        void SetNone();
        void Set(int value);
        void Set(const int* values, int count);
        void Set(const std::vector<int>& values);
        void Set(float value);
        void Set(const float* values, int count);
        void Set(const std::vector<float>& values);
        void Set(const FinjinColor& value);
        void Set(const FinjinColor* values, int count);
        void Set(const std::vector<FinjinColor>& values);
        void Set(const wxString& value);
        void Set(const wxString* values, int count);
        void Set(const std::vector<wxString>& values);
        void Set(ObjectAccessor value);
        void Set(const ObjectAccessor* values, int count);
        void Set(const std::vector<ObjectAccessor>& values);
        void Set(MaterialAccessor value);
        void Set(const MaterialAccessor* values, int count);
        void Set(const std::vector<MaterialAccessor>& values);

        /** 
         * Converts the extended value to a string. This is useful for debugging.
         * @param addPrefix [in] - Indicates whether string containing the type and value count should be prepended.
         * @return A formatted value string is returned.
         */
        wxString ToString(bool addPrefix = true) const;

    protected:
        /** Removes any unnecessary data. */
        void Clean();

    protected:
        Type type;
        int count;

        std::vector<float> floats;
        std::vector<wxString> strings;        
        std::vector<ObjectAccessor> objects;
        std::vector<MaterialAccessor> materials;
    };

} }
