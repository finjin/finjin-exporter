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
#include "ExtendedValueAttributes.hpp"
#include "NodeAttributeAdder.hpp"
#include "MayaPlug.hpp"
#include "Logger.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
const MString ExtendedValueAttributes::ARRAY_ATTRIBUTE_NAME = "ExtendedValues";

const MString ExtendedValueAttributes::ID_ATTRIBUTE_NAME = "ExtendedValueID";
const MString ExtendedValueAttributes::TYPE_ATTRIBUTE_NAME = "ExtendedValueAttributesType";
const MString ExtendedValueAttributes::FLOATS_ATTRIBUTE_NAME = "ExtendedValueAttributesFloats";
const MString ExtendedValueAttributes::STRINGS_ATTRIBUTE_NAME = "ExtendedValueAttributesStrings";
const MString ExtendedValueAttributes::OBJECTS_ATTRIBUTE_NAME = "ExtendedValueAttributesObjects";


//Local functions---------------------------------------------------------------
static void GetExtendedPlugValue
    (
    MPlug& extendedValuePlug,
    const MString& attributeNamePrefix,
    ExtendedValue& value,
    int* id = 0
    )
{
    if (id != nullptr)
        *id = MayaPlug::FindChildPlug(extendedValuePlug, attributeNamePrefix + ExtendedValueAttributes::ID_ATTRIBUTE_NAME).asInt();

    wxString typeString = ApplicationStringToWxString(MayaPlug::FindChildPlug(extendedValuePlug, attributeNamePrefix + ExtendedValueAttributes::TYPE_ATTRIBUTE_NAME).asString());
    ExtendedValue::Type type = ExtendedValue::ParseType(typeString);

    switch (type)
    {
        case ExtendedValue::EXTENDED_INT:
        case ExtendedValue::EXTENDED_FLOAT:
        {
            MPlug floatsPlug = MayaPlug::FindChildPlug(extendedValuePlug, attributeNamePrefix + ExtendedValueAttributes::FLOATS_ATTRIBUTE_NAME);

            std::vector<float> floats;
            floats.resize(floatsPlug.evaluateNumElements());
            for (size_t i = 0; i < floats.size(); i++)
                floats[i] = floatsPlug[(unsigned int)i].asFloat();
            value.Set(floats);

            break;
        }
        case ExtendedValue::EXTENDED_COLOR:
        {
            MPlug floatsPlug = MayaPlug::FindChildPlug(extendedValuePlug, attributeNamePrefix + ExtendedValueAttributes::FLOATS_ATTRIBUTE_NAME);

            std::vector<FinjinColor> colors;
            colors.resize(floatsPlug.evaluateNumElements() / 4);
            size_t floatIndex = 0;
            for (size_t i = 0; i < colors.size(); i++)
            {
                colors[i].r = floatsPlug[(unsigned int)floatIndex++].asFloat();
                colors[i].g = floatsPlug[(unsigned int)floatIndex++].asFloat();
                colors[i].b = floatsPlug[(unsigned int)floatIndex++].asFloat();
                colors[i].a = floatsPlug[(unsigned int)floatIndex++].asFloat();
            }
            value.Set(colors);

            break;
        }
        case ExtendedValue::EXTENDED_STRING:
        {
            MPlug stringsPlug = MayaPlug::FindChildPlug(extendedValuePlug, attributeNamePrefix + ExtendedValueAttributes::STRINGS_ATTRIBUTE_NAME);

            std::vector<wxString> strings;
            strings.resize(stringsPlug.evaluateNumElements());
            for (size_t i = 0; i < strings.size(); i++)
                strings[i] = ApplicationStringToWxString(stringsPlug[(unsigned int)i].asString());
            value.Set(strings);

            break;
        }
        case ExtendedValue::EXTENDED_OBJECT:
        case ExtendedValue::EXTENDED_MATERIAL:
        {
            MPlug objectsPlug = MayaPlug::FindChildPlug(extendedValuePlug, attributeNamePrefix + ExtendedValueAttributes::OBJECTS_ATTRIBUTE_NAME);

            std::vector<MObject> objects;
            objects.resize(MayaPlug::GetObjectReferenceCount(objectsPlug));
            for (size_t i = 0; i < objects.size(); i++)
            {
                MPlug objectPlug = objectsPlug[(unsigned int)i];
                objects[i] = MayaPlug::GetObjectReference(objectPlug);
            }

            if (type == ExtendedValue::EXTENDED_OBJECT)
            {
                std::vector<ObjectAccessor> accessors;
                accessors.resize(objects.size());
                for (size_t i = 0; i < objects.size(); i++)
                    accessors[i] = objects[i];
                value.Set(accessors);
            }
            else
            {
                std::vector<MaterialAccessor> accessors;
                accessors.resize(objects.size());
                for (size_t i = 0; i < objects.size(); i++)
                    accessors[i] = objects[i];
                value.Set(accessors);
            }

            break;
        }
        default: value.SetNone(); break;
    }
}

static void SetExtendedPlugValue(MPlug& extendedValuePlug, const MString& attributeNamePrefix, const ExtendedValue& value, int id)
{
    MayaPlug::FindChildPlug(extendedValuePlug, attributeNamePrefix + ExtendedValueAttributes::ID_ATTRIBUTE_NAME).setValue(id);

    MString typeString = WxStringToApplicationString(value.GetTypeString());
    MayaPlug::FindChildPlug(extendedValuePlug, attributeNamePrefix + ExtendedValueAttributes::TYPE_ATTRIBUTE_NAME).setValue(typeString);

    //Clear out old values
    MPlug floatsPlug = MayaPlug::FindChildPlug(extendedValuePlug, attributeNamePrefix + ExtendedValueAttributes::FLOATS_ATTRIBUTE_NAME);
    MayaPlug::RemoveAttributeElements(floatsPlug);

    MPlug stringsPlug = MayaPlug::FindChildPlug(extendedValuePlug, attributeNamePrefix + ExtendedValueAttributes::STRINGS_ATTRIBUTE_NAME);
    MayaPlug::RemoveAttributeElements(stringsPlug);

    MPlug objectsPlug = MayaPlug::FindChildPlug(extendedValuePlug, attributeNamePrefix + ExtendedValueAttributes::OBJECTS_ATTRIBUTE_NAME);
    MayaPlug::RemoveAttributeElements(objectsPlug);

    switch (value.GetType())
    {
        case ExtendedValue::EXTENDED_INT:
        case ExtendedValue::EXTENDED_FLOAT:
        {
            std::vector<int> floats;
            value.Get(floats);
            for (size_t i = 0; i < floats.size(); i++)
                MayaPlug::AddAttributeElementValue(floatsPlug, floats[i]);

            break;
        }
        case ExtendedValue::EXTENDED_COLOR:
        {
            std::vector<FinjinColor> colors;
            value.Get(colors);
            for (size_t i = 0; i < colors.size(); i++)
            {
                MayaPlug::AddAttributeElementValue(floatsPlug, colors[i].r);
                MayaPlug::AddAttributeElementValue(floatsPlug, colors[i].g);
                MayaPlug::AddAttributeElementValue(floatsPlug, colors[i].b);
                MayaPlug::AddAttributeElementValue(floatsPlug, colors[i].a);
            }

            break;
        }
        case ExtendedValue::EXTENDED_STRING:
        {
            std::vector<wxString> strings;
            value.Get(strings);

            for (size_t i = 0; i < strings.size(); i++)
                MayaPlug::AddAttributeElementValue(stringsPlug, strings[i]);

            break;
        }
        case ExtendedValue::EXTENDED_OBJECT:
        case ExtendedValue::EXTENDED_MATERIAL:
        {
            if (value.GetType() == ExtendedValue::EXTENDED_OBJECT)
            {
                std::vector<ObjectAccessor> accessors;
                value.Get(accessors);
                for (size_t i = 0; i < accessors.size(); i++)
                    MayaPlug::AddObjectReference(objectsPlug, accessors[i].obj);
            }
            else
            {
                std::vector<MaterialAccessor> accessors;
                value.Get(accessors);
                for (size_t i = 0; i < accessors.size(); i++)
                    MayaPlug::AddObjectReference(objectsPlug, accessors[i].obj);
            }

            break;
        }
        default: break;
    }
}

static bool FindExtendedValue(MPlug& value, MPlug& extendedValuesPlug, const MString& attributeNamePrefix, int id)
{
    value = MPlug();
    size_t count = extendedValuesPlug.evaluateNumElements();
    for (size_t valueIndex = 0; valueIndex < count && value.isNull(); valueIndex++)
    {
        MPlug extendedValuePlug = extendedValuesPlug[(unsigned int)valueIndex];

        if (id == MayaPlug::FindChildPlug(extendedValuePlug, attributeNamePrefix + ExtendedValueAttributes::ID_ATTRIBUTE_NAME).asInt())
            value = extendedValuePlug;
    }

    return !value.isNull();
}


//Implementation----------------------------------------------------------------
void ExtendedValueAttributes::CreateExtendedValueAttributes
    (
    NodeAttributeAdder& adder,
    const MString& attributeNamePrefix
    )
{
    struct Attributes
    {
        MObject extendedValueID;
        MObject extendedValueType;
        MObject extendedValueFloats;
        MObject extendedValueStrings;
        MObject extendedValueObjects;
    };
    Attributes attributes;
    std::list<MObject> attributesList;

    bool oldAdd = adder.add;
    adder.add = false;
    bool oldHidden = adder.hidden;
    adder.hidden = true;

    attributesList.push_back(attributes.extendedValueID = adder.AddString(attributeNamePrefix + ID_ATTRIBUTE_NAME));
    attributesList.push_back(attributes.extendedValueType = adder.AddString(attributeNamePrefix + TYPE_ATTRIBUTE_NAME));
    attributesList.push_back(attributes.extendedValueFloats = adder.AddFloatArray(attributeNamePrefix + FLOATS_ATTRIBUTE_NAME));
    attributesList.push_back(attributes.extendedValueStrings = adder.AddStringArray(attributeNamePrefix + STRINGS_ATTRIBUTE_NAME));
    attributesList.push_back(attributes.extendedValueObjects = adder.AddObjectReferenceArray(attributeNamePrefix + OBJECTS_ATTRIBUTE_NAME));

    adder.add = oldAdd;

    this->extendedValues = adder.AddCompoundArray(attributeNamePrefix + ARRAY_ATTRIBUTE_NAME, attributesList);

    adder.hidden = oldHidden;
}

void ExtendedValueAttributes::CreateExtendedValueAttributes
    (
    std::list<MObject>& attributesList,
    NodeAttributeAdder& adder,
    const MString& attributeNamePrefix)
{
    CreateExtendedValueAttributes(adder, attributeNamePrefix);
    attributesList.push_back(this->extendedValues);
}

void ExtendedValueAttributes::GetAllExtendedValues
    (
    MPlug extendedValuesPlug,
    const MString& attributeNamePrefix,
    std::vector<ExtendedValue>& values,
    std::vector<int>* ids
    )
{
    values.resize(extendedValuesPlug.evaluateNumElements());
    if (ids != nullptr)
        ids->resize(values.size());
    for (size_t i = 0; i < values.size(); i++)
    {
        MPlug extendedValuePlug = extendedValuesPlug[(unsigned int)i];
        GetExtendedPlugValue(extendedValuePlug, attributeNamePrefix, values[i], ids ? &(*ids)[i] : nullptr);
    }
}

bool ExtendedValueAttributes::GetExtendedValue
    (
    MPlug extendedValuesPlug,
    const MString& attributeNamePrefix,
    int id,
    ExtendedValue& value
    )
{
    MPlug valuePlug;
    if (FindExtendedValue(valuePlug, extendedValuesPlug, attributeNamePrefix, id))
        GetExtendedPlugValue(valuePlug, attributeNamePrefix, value);

    return !valuePlug.isNull();
}

void ExtendedValueAttributes::SetExtendedValue
    (
    MPlug extendedValuesPlug,
    const MString& attributeNamePrefix,
    int id,
    const ExtendedValue& value
    )
{
    MPlug valuePlug;
    if (!FindExtendedValue(valuePlug, extendedValuesPlug, attributeNamePrefix, id))
    {
        int addedIndex = MayaPlug::AddAttributeElement(extendedValuesPlug, ApplicationStringToWxString(attributeNamePrefix + ID_ATTRIBUTE_NAME));
        valuePlug = extendedValuesPlug[addedIndex];
    }
    SetExtendedPlugValue(valuePlug, attributeNamePrefix, value, id);
}

bool ExtendedValueAttributes::RemoveExtendedValue
    (
    MPlug extendedValuesPlug,
    const MString& attributeNamePrefix,
    int id
    )
{
    MPlug valuePlug;
    if (FindExtendedValue(valuePlug, extendedValuesPlug, attributeNamePrefix, id))
        MayaPlug::RemoveAttributeElement(valuePlug);

    return !valuePlug.isNull();
}

void ExtendedValueAttributes::ClearExtendedValues(MPlug extendedValuesPlug, const MString& attributeNamePrefix)
{
    MayaPlug::RemoveAttributeElements(extendedValuesPlug);
}
