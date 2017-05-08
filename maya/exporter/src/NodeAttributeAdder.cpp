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
#include "NodeAttributeAdder.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
static const int ENUM_MIN = SHRT_MIN;
static const int ENUM_MAX = SHRT_MAX;


//Implementation----------------------------------------------------------------
NodeAttributeAdder::NodeAttributeAdder()
{
    this->add = true;
    this->connectable = true;
    this->hidden = false;
    this->readable = true;
    this->writable = true;
    this->storable = true;
    this->keyable = false;
}

void NodeAttributeAdder::MakeInput(bool keyable)
{
    this->readable = true;
    this->writable = true;
    this->storable = true;
    this->keyable = keyable;
}

void NodeAttributeAdder::MakeOutput()
{
    this->readable = true;
    this->writable = false;
    this->storable = false;
    this->keyable = false;
}

void NodeAttributeAdder::SetUp(MFnAttribute& attr)
{
    attr.setConnectable(this->connectable);
    attr.setHidden(this->hidden);
    attr.setReadable(this->readable);
    attr.setWritable(this->writable);
}

void NodeAttributeAdder::AddAttribute(MObject& attr)
{
    if (this->add)
    {
        if (!this->object.isNull())
        {
            if (this->object.hasFn(MFn::kDependencyNode))
            {
                MFnDependencyNode depNode(this->object);
                depNode.addAttribute(attr);
            }
        }
        else
            MPxNode::addAttribute(attr);
    }
}

MObject NodeAttributeAdder::AddInt(const MString& name, int defaultValue)
{
    MFnNumericAttribute numericAttribute;
    MObject attr = numericAttribute.create(name, name, MFnNumericData::kInt, defaultValue);
    SetUp(numericAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddInt(const MString& name, int defaultValue, int minValue, int maxValue)
{
    MFnNumericAttribute numericAttribute;
    MObject attr = numericAttribute.create(name, name, MFnNumericData::kInt, defaultValue);
    SetUp(numericAttribute);
    numericAttribute.setMin(minValue);
    numericAttribute.setMax(maxValue);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddIntArray(const MString& name)
{
    MFnNumericAttribute numericAttribute;
    MObject attr = numericAttribute.create(name, name, MFnNumericData::kInt, std::numeric_limits<int>::max());
    SetUp(numericAttribute);
    numericAttribute.setArray(true);
    numericAttribute.setUsesArrayDataBuilder(true);
    numericAttribute.setDisconnectBehavior(MFnAttribute::kDelete);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddEmptyEnum(const MString& name)
{
    return AddEnum(name, ENUM_MIN, 2, "Min", ENUM_MIN, "Max", ENUM_MAX);
}

MObject NodeAttributeAdder::AddEnum(const MString& name, short defaultValue, int pairCount, ...)
{
    MFnEnumAttribute enumAttribute;
    MObject attr = enumAttribute.create(name, name, defaultValue);

    //Add fields
    va_list args;
    va_start(args, pairCount);
    for (int i = 0; i < pairCount; i++)
    {
        char* fieldName = va_arg(args, char*);
        int fieldValue = va_arg(args, int);
        enumAttribute.addField(fieldName, fieldValue);
    }
    va_end(args);

    SetUp(enumAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddEmptyEnumArray(const MString& name)
{
    return AddEnumArray(name, ENUM_MIN, 2, "Min", ENUM_MIN, "Max", ENUM_MAX);
}

MObject NodeAttributeAdder::AddEnumArray(const MString& name, short defaultValue, int pairCount, ...)
{
    MFnEnumAttribute enumAttribute;
    MObject attr = enumAttribute.create(name, name, defaultValue);

    //Add fields
    va_list args;
    va_start(args, pairCount);
    for (int i = 0; i < pairCount; i++)
    {
        char* fieldName = va_arg(args, char*);
        int fieldValue = va_arg(args, int);
        enumAttribute.addField(fieldName, fieldValue);
    }
    va_end(args);

    SetUp(enumAttribute);
    enumAttribute.setArray(true);
    enumAttribute.setUsesArrayDataBuilder(true);
    enumAttribute.setDisconnectBehavior(MFnAttribute::kDelete);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddDistance(const MString& name, MDistance defaultValue)
{
    MFnUnitAttribute unitAttribute;
    MObject attr = unitAttribute.create(name, name, MFnUnitAttribute::kDistance);
    unitAttribute.setDefault(defaultValue);
    unitAttribute.setMin(MDistance(0.0, defaultValue.unit()));
    SetUp(unitAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddDistance(const MString& name, MDistance defaultValue, double maxValue)
{
    MFnUnitAttribute unitAttribute;
    MObject attr = unitAttribute.create(name, name, MFnUnitAttribute::kDistance);
    unitAttribute.setDefault(defaultValue);
    unitAttribute.setMin(MDistance(0.0, defaultValue.unit()));
    unitAttribute.setSoftMax(MDistance(maxValue, defaultValue.unit()));
    SetUp(unitAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddAlpha(const MString& name, double defaultValue)
{
    MFnNumericAttribute numericAttribute;
    MObject attr = numericAttribute.create(name, name, MFnNumericData::kFloat, defaultValue);
    SetUp(numericAttribute);
    numericAttribute.setMin(0);
    numericAttribute.setMax(1);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddDouble(const MString& name, double defaultValue)
{
    MFnNumericAttribute numericAttribute;
    MObject attr = numericAttribute.create(name, name, MFnNumericData::kDouble, defaultValue);
    SetUp(numericAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddFloat(const MString& name, double defaultValue, double minValue, double maxValue)
{
    MFnNumericAttribute numericAttribute;
    MObject attr = numericAttribute.create(name, name, MFnNumericData::kFloat, defaultValue);
    SetUp(numericAttribute);
    numericAttribute.setMin(minValue);
    numericAttribute.setMax(maxValue);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddFloatArray(const MString& name)
{
    MFnNumericAttribute numericAttribute;
    MObject attr = numericAttribute.create(name, name, MFnNumericData::kFloat, std::numeric_limits<float>::max());
    SetUp(numericAttribute);
    numericAttribute.setArray(true);
    numericAttribute.setUsesArrayDataBuilder(true);
    numericAttribute.setDisconnectBehavior(MFnAttribute::kDelete);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddBool(const MString& name, bool defaultValue)
{
    MFnNumericAttribute numericAttribute;
    MObject attr = numericAttribute.create(name, name, MFnNumericData::kBoolean, defaultValue);
    SetUp(numericAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddColor(const MString& name, MColor defaultValue)
{
    MFnNumericAttribute numericAttribute;
    MObject attr = numericAttribute.createColor(name, name);
    numericAttribute.setDefault(defaultValue.r, defaultValue.g, defaultValue.b);
    SetUp(numericAttribute);
    numericAttribute.setUsedAsColor(true);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddFloat2(const MString& name, MPoint defaultValue)
{
    MFnNumericAttribute numericAttribute;
    MObject attr = numericAttribute.create(name, name, MFnNumericData::k2Float);
    numericAttribute.setDefault(defaultValue.x, defaultValue.y);
    SetUp(numericAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddFloat3(const MString& name, MPoint defaultValue)
{
    MFnNumericAttribute numericAttribute;
    MObject attr = numericAttribute.create(name, name, MFnNumericData::k3Float);
    numericAttribute.setDefault(defaultValue.x, defaultValue.y, defaultValue.z);
    SetUp(numericAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddFloat3Array(const MString& name)
{
    MFnNumericAttribute numericAttribute;
    MObject attr = numericAttribute.create(name, name, MFnNumericData::k3Float);
    numericAttribute.setDefault(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    SetUp(numericAttribute);
    numericAttribute.setArray(true);
    numericAttribute.setUsesArrayDataBuilder(true);
    numericAttribute.setDisconnectBehavior(MFnAttribute::kDelete);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddDouble4(const MString& name, MPoint defaultValue)
{
    MFnNumericAttribute numericAttribute;
    MObject attr = numericAttribute.create(name, name, MFnNumericData::k4Double);
    numericAttribute.setDefault(defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w);
    SetUp(numericAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddDouble4Array(const MString& name)
{
    MFnNumericAttribute numericAttribute;
    MObject attr = numericAttribute.create(name, name, MFnNumericData::k4Double);
    numericAttribute.setDefault(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    SetUp(numericAttribute);
    numericAttribute.setArray(true);
    numericAttribute.setUsesArrayDataBuilder(true);
    numericAttribute.setDisconnectBehavior(MFnAttribute::kDelete);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddMatrix(const MString& name)
{
    MFnMatrixAttribute matrixAttribute;
    MObject attr = matrixAttribute.create(name, name);
    SetUp(matrixAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddMatrixArray(const MString& name)
{
    static const float defaultMatrixValues[4][4] =
        {
        {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()},
        {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()},
        {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()},
        {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()}
        };

    MFnMatrixAttribute matrixAttribute;
    MObject attr = matrixAttribute.create(name, name);
    MMatrix defaultMatrix(defaultMatrixValues);
    matrixAttribute.setDefault(defaultMatrix);
    SetUp(matrixAttribute);
    matrixAttribute.setArray(true);
    matrixAttribute.setUsesArrayDataBuilder(true);
    matrixAttribute.setDisconnectBehavior(MFnAttribute::kDelete);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddString(const MString& name, const MString& defaultValue)
{
    MFnTypedAttribute typedAttribute;
    MFnStringData stringData;
    MObject attr = typedAttribute.create(name, name, MFnData::kString, stringData.create(defaultValue));
    SetUp(typedAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddStringArray(const MString& name)
{
    MFnTypedAttribute typedAttribute;
    MObject attr = typedAttribute.create(name, name, MFnData::kString);
    typedAttribute.setDefault(MObject::kNullObj);
    SetUp(typedAttribute);
    typedAttribute.setArray(true);
    typedAttribute.setUsesArrayDataBuilder(true);
    typedAttribute.setDisconnectBehavior(MFnAttribute::kDelete);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddTypedObject(const MString& name, const MTypeId& typeID)
{
    MFnTypedAttribute typedAttribute;
    MObject attr = typedAttribute.create(name, name, typeID);
    SetUp(typedAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddObjectReference(const MString& name)
{
    MFnMessageAttribute messageAttribute;
    MObject attr = messageAttribute.create(name, name);
    SetUp(messageAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddObjectReferenceArray(const MString& name)
{
    MFnMessageAttribute messageAttribute;
    MObject attr = messageAttribute.create(name, name);
    SetUp(messageAttribute);
    messageAttribute.setArray(true);
    messageAttribute.setUsesArrayDataBuilder(true);
    messageAttribute.setDisconnectBehavior(MFnAttribute::kDelete);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddCompound(const MString& name, int attributeCount, const MObject* attributes)
{
    MFnCompoundAttribute compoundAttribute;
    MObject attr = compoundAttribute.create(name, name);

    for (int i = 0; i < attributeCount; i++)
        compoundAttribute.addChild(attributes[i]);

    SetUp(compoundAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddCompound(const MString& name, const std::list<MObject>& attributes)
{
    //FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("NodeAttributeAdder::AddCompound()"));

    MFnCompoundAttribute compoundAttribute;
    MObject attr = compoundAttribute.create(name, name);

    for (std::list<MObject>::const_iterator i = attributes.begin(); i != attributes.end(); ++i)
        compoundAttribute.addChild(*i);

    SetUp(compoundAttribute);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddCompoundArray(const MString& name, int attributeCount, const MObject* attributes)
{
    MFnCompoundAttribute compoundAttribute;
    MObject attr = compoundAttribute.create(name, name);

    for (int i = 0; i < attributeCount; i++)
        compoundAttribute.addChild(attributes[i]);

    SetUp(compoundAttribute);
    compoundAttribute.setArray(true);
    compoundAttribute.setUsesArrayDataBuilder(true);
    compoundAttribute.setDisconnectBehavior(MFnAttribute::kDelete);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddCompoundArray(const MString& name, const std::list<MObject>& attributes)
{
    MFnCompoundAttribute compoundAttribute;
    MObject attr = compoundAttribute.create(name, name);

    for (std::list<MObject>::const_iterator i = attributes.begin(); i != attributes.end(); ++i)
        compoundAttribute.addChild(*i);

    SetUp(compoundAttribute);
    compoundAttribute.setArray(true);
    compoundAttribute.setUsesArrayDataBuilder(true);
    compoundAttribute.setDisconnectBehavior(MFnAttribute::kDelete);
    AddAttribute(attr);

    return attr;
}

MObject NodeAttributeAdder::AddPixelFormat(const MString& name, short defaultValue)
{
    return AddEnum
        (
        name,
        defaultValue,
        33,
        "Default", 0,
        "L8", 1,
        "L16", 2,
        "A8", 3,
        "A4L4", 4,
        "BYTE_LA", 5,
        "R5G6B5", 6,
        "B5G6R5", 7,
        "R3G3B2", 8,
        "A4R4G4B4", 9,
        "A1R5G5B5", 10,
        "R8G8B8", 11,
        "B8G8R8", 12,
        "A8R8G8B8", 13,
        "A8B8G8R8", 14,
        "B8G8R8A8", 15,
        "R8G8B8A8", 16,
        "X8R8G8B8", 17,
        "X8B8G8R8", 18,
        "A2R10G10B10", 19,
        "A2B10G10R10", 20,
        "FLOAT16_R", 21,
        "FLOAT16_RGB", 22,
        "FLOAT16_RGBA", 23,
        "FLOAT32_R", 24,
        "FLOAT32_RGB", 25,
        "FLOAT32_RGBA", 26,
        "FLOAT16_GR", 27,
        "FLOAT32_GR", 28,
        "DEPTH", 29,
        "SHORT_RGBA", 30,
        "SHORT_GR", 31,
        "SHORT_RGB", 32
        );
}
