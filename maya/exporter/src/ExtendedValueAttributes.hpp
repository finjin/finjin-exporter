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

    class NodeAttributeAdder;

    /** Adds, removes, gets, and sets extended values into Maya attributes. */
    struct ExtendedValueAttributes
    {
    public:
        /** 
         * Creates the extended attributes.
         * @param adder [in] - Used to create the attributes.
         * @param attributeNamePrefix [in] - Prefix to prepend to attribute names.
         */
        void CreateExtendedValueAttributes
            (
            NodeAttributeAdder& adder, 
            const MString& attributeNamePrefix = ""
            );

        /** 
         * Creates the extended attributes and puts the created attributes into the specified list.
         * @param attributesList [out] - Receives the created attributes.
         * @param adder [in] - Used to create the attributes.
         * @param attributeNamePrefix [in] - Prefix to prepend to attribute names.
         */
        void CreateExtendedValueAttributes
            (
            std::list<MObject>& attributesList, 
            NodeAttributeAdder& adder, 
            const MString& attributeNamePrefix = ""
            );

        /**
         * Gets all the extended values.
         * @param extendedValuesPlug [in] - The plug that contains the array of extended values.
         * @param attributeNamePrefix [in] - The prefix that was used when creating the attributes.
         * @param values [out] - The collection of retrieved extended values.
         * @param ids [out] - An optional collection of extended value identifers, one for each value.
         */
        static void GetAllExtendedValues
            (
            MPlug extendedValuesPlug, 
            const MString& attributeNamePrefix, 
            std::vector<ExtendedValue>& values, 
            std::vector<int>* ids = nullptr
            );

        /**
         * Gets the extended value with the specified identifier.
         * @param extendedValuesPlug [in] - The plug that contains the array of extended values.
         * @param attributeNamePrefix [in] - The prefix that was used when creating the attributes.
         * @param id [in] - The identifier of the value to get.
         * @param value [out] - The retrieved extended value.
         * @return If the value with the specified identifier was found, true is returned. Otherwise, false is returned.
         */
        static bool GetExtendedValue
            (
            MPlug extendedValuesPlug, 
            const MString& attributeNamePrefix, 
            int id, 
            ExtendedValue& value
            );
        
        /**
         * Sets an extended value with the specified identifier.
         * If a value with the specified identifier already exists, it is overwritten.
         * Otherwise, it is added.
         * @param extendedValuesPlug [in] - The plug that contains the array of extended values.
         * @param attributeNamePrefix [in] - The prefix that was used when creating the attributes.
         * @param id [in] - The identifier of the value to set.
         * @param value [out] - The extended value to set.
         */
        static void SetExtendedValue
            (
            MPlug extendedValuesPlug, 
            const MString& attributeNamePrefix, 
            int id, 
            const ExtendedValue& value
            );

        /**
         * Removes the extended value with the specified identifier.
         * @param extendedValuesPlug [in] - The plug that contains the array of extended values.
         * @param attributeNamePrefix [in] - The prefix that was used when creating the attributes.
         * @param id [in] - The identifier of the value to remove.
         * @return If the value with the specified identifier was removed, true is returned. Otherwise, false is returned.
         */
        static bool RemoveExtendedValue
            (
            MPlug extendedValuesPlug, 
            const MString& attributeNamePrefix, 
            int id
            );

        /**
         * Removes all extended values.
         * @param extendedValuesPlug [in] - The plug that contains the array of extended values.
         * @param attributeNamePrefix [in] - The prefix that was used when creating the attributes.
         */
        static void ClearExtendedValues(MPlug extendedValuesPlug, const MString& attributeNamePrefix);

    public:
        /** The extended values attribute. */
        MObject extendedValues;

        static const MString ARRAY_ATTRIBUTE_NAME;

        static const MString ID_ATTRIBUTE_NAME;
        static const MString TYPE_ATTRIBUTE_NAME;
        static const MString FLOATS_ATTRIBUTE_NAME;
        static const MString STRINGS_ATTRIBUTE_NAME;
        static const MString OBJECTS_ATTRIBUTE_NAME;
    };

} }


//Macros-----------------------------------------------------------------------
#define IMPLEMENT_STATIC_ATTRIBUTE_EXTENDED_VALUE_ACCESSOR_METHODS_PREFIXED(className, member, attributes, attributeNamePrefix)\
    void className::GetAllExtendedValues(std::vector<ExtendedValue>& values, std::vector<int>* ids) {ExtendedValueAttributes::GetAllExtendedValues(MPlug(this->member, attributes.extendedValues), attributeNamePrefix, values, ids);}\
    bool className::GetExtendedValue(int id, ExtendedValue& value) {return ExtendedValueAttributes::GetExtendedValue(MPlug(this->member, attributes.extendedValues), attributeNamePrefix, id, value);}\
    \
    void className::SetExtendedValue(int id, const ExtendedValue& value) {ExtendedValueAttributes::SetExtendedValue(MPlug(this->member, attributes.extendedValues), attributeNamePrefix, id, value);}\
    bool className::RemoveExtendedValue(int id) {return ExtendedValueAttributes::RemoveExtendedValue(MPlug(this->member, attributes.extendedValues), attributeNamePrefix, id);}\
    void className::ClearExtendedValues() {ExtendedValueAttributes::ClearExtendedValues(MPlug(this->member, attributes.extendedValues), attributeNamePrefix);}

#define IMPLEMENT_STATIC_ATTRIBUTE_EXTENDED_VALUE_ACCESSOR_METHODS(className, member, attributes)\
    IMPLEMENT_STATIC_ATTRIBUTE_EXTENDED_VALUE_ACCESSOR_METHODS_PREFIXED(className, member, attributes, "")

#define IMPLEMENT_DYNAMIC_ATTRIBUTE_EXTENDED_VALUE_ACCESSOR_METHODS_PREFIXED(className, attributeNamePrefix)\
    void className::GetAllExtendedValues(std::vector<ExtendedValue>& values, std::vector<int>* ids) {ExtendedValueAttributes::GetAllExtendedValues(Child(attributeNamePrefix + ExtendedValueAttributes::ARRAY_ATTRIBUTE_NAME), attributeNamePrefix, values, ids);}\
    bool className::GetExtendedValue(int id, ExtendedValue& value) {return ExtendedValueAttributes::GetExtendedValue(Child(attributeNamePrefix + ExtendedValueAttributes::ARRAY_ATTRIBUTE_NAME), attributeNamePrefix, id, value);}\
    \
    void className::SetExtendedValue(int id, const ExtendedValue& value) {ExtendedValueAttributes::SetExtendedValue(Child(attributeNamePrefix + ExtendedValueAttributes::ARRAY_ATTRIBUTE_NAME), attributeNamePrefix, id, value);}\
    bool className::RemoveExtendedValue(int id) {return ExtendedValueAttributes::RemoveExtendedValue(Child(attributeNamePrefix + ExtendedValueAttributes::ARRAY_ATTRIBUTE_NAME), attributeNamePrefix, id);}\
    void className::ClearExtendedValues() {ExtendedValueAttributes::ClearExtendedValues(Child(attributeNamePrefix + ExtendedValueAttributes::ARRAY_ATTRIBUTE_NAME), attributeNamePrefix);}

#define IMPLEMENT_DYNAMIC_ATTRIBUTE_EXTENDED_VALUE_ACCESSOR_METHODS(className)\
    IMPLEMENT_DYNAMIC_ATTRIBUTE_EXTENDED_VALUE_ACCESSOR_METHODS_PREFIXED(className, "")

#define IMPLEMENT_DYNAMIC_OR_STATIC_ATTRIBUTE_EXTENDED_VALUE_ACCESSOR_METHODS_PREFIXED(className, attributeNamePrefix, attributes)\
    void className::GetAllExtendedValues(std::vector<ExtendedValue>& values, std::vector<int>* ids) {ExtendedValueAttributes::GetAllExtendedValues(Child(attributes.extendedValues, attributeNamePrefix + ExtendedValueAttributes::ARRAY_ATTRIBUTE_NAME), attributeNamePrefix, values, ids);}\
    bool className::GetExtendedValue(int id, ExtendedValue& value) {return ExtendedValueAttributes::GetExtendedValue(Child(attributes.extendedValues, attributeNamePrefix + ExtendedValueAttributes::ARRAY_ATTRIBUTE_NAME), attributeNamePrefix, id, value);}\
    \
    void className::SetExtendedValue(int id, const ExtendedValue& value) {ExtendedValueAttributes::SetExtendedValue(Child(attributes.extendedValues, attributeNamePrefix + ExtendedValueAttributes::ARRAY_ATTRIBUTE_NAME), attributeNamePrefix, id, value);}\
    bool className::RemoveExtendedValue(int id) {return ExtendedValueAttributes::RemoveExtendedValue(Child(attributes.extendedValues, attributeNamePrefix + ExtendedValueAttributes::ARRAY_ATTRIBUTE_NAME), attributeNamePrefix, id);}\
    void className::ClearExtendedValues() {ExtendedValueAttributes::ClearExtendedValues(Child(attributes.extendedValues, attributeNamePrefix + ExtendedValueAttributes::ARRAY_ATTRIBUTE_NAME), attributeNamePrefix);}

#define IMPLEMENT_DYNAMIC_OR_STATIC_ATTRIBUTE_EXTENDED_VALUE_ACCESSOR_METHODS(className, attributes)\
    IMPLEMENT_DYNAMIC_OR_STATIC_ATTRIBUTE_EXTENDED_VALUE_ACCESSOR_METHODS_PREFIXED(className, "", attributes)
