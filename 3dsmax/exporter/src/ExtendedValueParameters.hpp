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
#include "ExtendedValueAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Adds, removes, gets, and sets extended values into a 3DS Max parameter block. */
    class ExtendedValueParameters
    {
    public:
        /**
         * Gets all the extended values.
         * @param pblock [in] - The parameter block that contains the extended value objects.
         * @param paramID [in] - The parameter identifer that contains the table (array) of extended value objects.
         * @param values [out] - The collection of retrieved extended values.
         * @param ids [out] - An optional collection of extended value identifers, one for each value.
         */
        static void GetAllExtendedValues
            (
            IParamBlock2* pblock,
            int paramID,
            std::vector<ExtendedValue>& values,
            std::vector<int>* ids = nullptr
            );

        /**
         * Gets the extended value with the specified identifier.
         * @param pblock [in] - The parameter block that contains the extended value objects.
         * @param paramID [in] - The parameter identifer that contains the table (array) of extended value objects.
         * @param id [in] - The identifier of the value to get.
         * @param value [out] - The retrieved extended value.
         * @return If the value with the specified identifier was found, true is returned. Otherwise, false is returned.
         */
        static bool GetExtendedValue
            (
            IParamBlock2* pblock,
            int paramID,
            int id,
            ExtendedValue& value
            );

        /**
         * Sets an extended value with the specified identifier.
         * @param pblock [in] - The parameter block that contains the extended value objects.
         * @param paramID [in] - The parameter identifer that contains the table (array) of extended value objects.
         * @param id [in] - The identifier of the value to set.
         * @param value [out] - The extended value to set.
         */
        static void SetExtendedValue
            (
            IParamBlock2* pblock,
            int paramID,
            int id,
            const ExtendedValue& value
            );

        /**
         * Removes the extended value with the specified identifier.
         * @param pblock [in] - The parameter block that contains the extended value objects.
         * @param paramID [in] - The parameter identifer that contains the table (array) of extended value objects.
         * @param id [in] - The identifier of the value to remove.
         * @return If the value with the specified identifier was removed, true is returned. Otherwise, false is returned.
         */
        static bool RemoveExtendedValue
            (
            IParamBlock2* pblock,
            int paramID,
            int id
            );

        /**
         * Removes all extended values.
         * @param pblock [in] - The parameter block that contains the extended value objects.
         * @param paramID [in] - The parameter identifer that contains the table (array) of extended value objects.
         */
        static void ClearExtendedValues
            (
            IParamBlock2* pblock,
            int paramID
            );
    };

} }


//Macros------------------------------------------------------------------------

/** This macro should NEVER be modified. If more parameters are needed, create a new macro */
#define EXTENDED_VALUE_PARAM_BLOCK_IDS PB_EXTENDED_VALUES

#define EXTENDED_VALUE_PARAMETER_BLOCK_PARAMETERS(className)\
    className::PB_EXTENDED_VALUES, _T("ExtendedValues"), TYPE_REFTARG_TAB, 0, P_VARIABLE_SIZE, IDS_EXTENDED_VALUES,\
        p_end

#define EXTENDED_VALUE_NOTIFY_NULLED_CASE(paramID, index)\
    case PB_EXTENDED_VALUES: this->pblock->Delete(paramID, index, 1); break;

#define DECLARE_EXTENDED_VALUE_OBJECT_METHODS FINJIN_EXPORTER_DECLARE_EXTENDED_VALUE_ACCESSOR_METHODS

#define FINJIN_EXPORTER_IMPLEMENT_EXTENDED_VALUE_OBJECT_METHODS(className)\
    void className::GetAllExtendedValues(std::vector<ExtendedValue>& values, std::vector<int>* ids) {ExtendedValueParameters::GetAllExtendedValues(this->pblock, PB_EXTENDED_VALUES, values, ids);}\
    bool className::GetExtendedValue(int id, ExtendedValue& value) {return ExtendedValueParameters::GetExtendedValue(this->pblock, PB_EXTENDED_VALUES, id, value);}\
    \
    void className::SetExtendedValue(int id, const ExtendedValue& value) {ExtendedValueParameters::SetExtendedValue(this->pblock, PB_EXTENDED_VALUES, id, value);}\
    bool className::RemoveExtendedValue(int id) {return ExtendedValueParameters::RemoveExtendedValue(this->pblock, PB_EXTENDED_VALUES, id);}\
    void className::ClearExtendedValues() {ExtendedValueParameters::ClearExtendedValues(this->pblock, PB_EXTENDED_VALUES);}

#define FINJIN_EXPORTER_IMPLEMENT_EXTENDED_VALUE_ACCESSOR_METHODS(className, member)\
    void className::GetAllExtendedValues(std::vector<ExtendedValue>& values, std::vector<int>* ids) {this->member->GetAllExtendedValues(values, ids);}\
    bool className::GetExtendedValue(int id, ExtendedValue& value) {return this->member->GetExtendedValue(id, value);}\
    \
    void className::SetExtendedValue(int id, const ExtendedValue& value) {this->member->SetExtendedValue(id, value);}\
    bool className::RemoveExtendedValue(int id) {return this->member->RemoveExtendedValue(id);}\
    void className::ClearExtendedValues() {this->member->ClearExtendedValues();}
