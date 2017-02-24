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
#include "ExtendedValue.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** 
     * This class provides generic access to 'extended values' stored within an object.
     * Extended values are similar to user data. The difference is that user data is 
     * intended for USERS of the Finjin exporter, while extended values are intended
     * for DEVELOPERS working with the Finjin exporter source code. Finjin does not
     * store any of its own data in extended values.
     */
    class ExtendedValueAccessor
    {
    public:
        virtual ~ExtendedValueAccessor() {}

        /** 
         * Gets all the extended values stored within the object.
         * @param values [out] - Array of values.
         * @param ids [out] - Pointer to an array of identifiers, each one corresponding to a value. 
         * If null, no identifiers are returned.
         */
        virtual void GetAllExtendedValues(std::vector<ExtendedValue>& values, std::vector<int>* ids = nullptr) = 0;

        /**
         * Gets one extended value.
         * @param id [in] - The identifier of the extended value to retrieve.
         * @param value [out] - The retrieved value.
         * @return If a value with the specified identifer is found, true is returned. Otherwise, false is returned.
         */
        virtual bool GetExtendedValue(int id, ExtendedValue& value) = 0;
        
        /**
         * Sets one extended value.
         * @param id [in] - The identifier of the extended value to set.
         * @param value [in] - The value to set.
         */
        virtual void SetExtendedValue(int id, const ExtendedValue& value) = 0;

        /** 
         * Removes the extended value with the specified identifier.
         * @param id [in] - The identifier of the extended value to remove.
         * @return If a value with the specified identifer is removed, true is returned. Otherwise, false is returned.
         */
        virtual bool RemoveExtendedValue(int id) = 0;

        /** 
         * Removes all extended values.
         */
        virtual void ClearExtendedValues() = 0;    
    };

} }


//Macros-----------------------------------------------------------------------

/** Common extended value accessor method declarations. */
#define FINJIN_EXPORTER_DECLARE_EXTENDED_VALUE_ACCESSOR_METHODS \
    void GetAllExtendedValues(std::vector<ExtendedValue>& values, std::vector<int>* ids = nullptr);\
    bool GetExtendedValue(int id, ExtendedValue& value);\
    \
    void SetExtendedValue(int id, const ExtendedValue& value);\
    bool RemoveExtendedValue(int id);\
    void ClearExtendedValues();

#define FINJIN_EXPORTER_DECLARE_EXTENDED_VALUE_ACCESSOR_METHODS_OVERRIDE \
    void GetAllExtendedValues(std::vector<ExtendedValue>& values, std::vector<int>* ids = nullptr) override;\
    bool GetExtendedValue(int id, ExtendedValue& value) override;\
    \
    void SetExtendedValue(int id, const ExtendedValue& value) override;\
    bool RemoveExtendedValue(int id) override;\
    void ClearExtendedValues() override;

/**
 * Default common extended value accessor method implementation.
 * This is useful during development, before the actual data storage code has been written.
 */
#define FINJIN_EXPORTER_IMPLEMENT_DEFAULT_EXTENDED_VALUE_ACCESSOR_METHODS(className) \
    void className::GetAllExtendedValues(std::vector<ExtendedValue>& values, std::vector<int>* ids) {}\
    bool className::GetExtendedValue(int id, ExtendedValue& value) {return false;}\
    \
    void className::SetExtendedValue(int id, const ExtendedValue& value) {}\
    bool className::RemoveExtendedValue(int id) {return true;}\
    void className::ClearExtendedValues() {}
