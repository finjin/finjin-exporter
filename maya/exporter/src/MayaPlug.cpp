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
#include "MayaPlug.hpp"
#include "TimeChanger.hpp"
#include "StringSplitter.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
MPlug MayaPlug::FindChildPlug(MPlug& rootPlug, MObject attribute, const MString& name)
{
    MPlug result;

    if (rootPlug.isDynamic())
    {
        MString plugName;
        unsigned int count = rootPlug.numChildren();
        for (unsigned int i = 0; i < count; i++)
        {
            MPlug plug = rootPlug.child(i);
            MayaPlug::GetBaseAttributeName(plug, plugName);
            if (plugName == name)
            {
                result = plug;
                break;
            }
        }
    }
    else
        result = rootPlug.child(attribute);

    return result;
}

MPlug MayaPlug::FindChildPlug(MPlug& rootPlug, const MString& name)
{
    MPlug result;

    MString plugName;
    unsigned int count = rootPlug.numChildren();
    for (unsigned int i = 0; i < count; i++)
    {
        MPlug plug = rootPlug.child(i);
        MayaPlug::GetBaseAttributeName(plug, plugName);
        if (plugName == name)
        {
            result = plug;
            break;
        }
    }

    return result;
}

bool MayaPlug::GetBoolAttribute(const MObject& obj, const MString& attributeName, bool defaultValue, TimeAccessor time)
{
    TimeChanger timeChanger(time);

    bool value = defaultValue;

    MStatus status;
    MFnDagNode dagNode(obj, &status);
    if (status == MStatus::kSuccess)
    {
        MObject attribute = dagNode.attribute(attributeName);
        if (!attribute.isNull())
        {
            MPlug plug(obj, attribute);
            plug.getValue(value);
        }
    }
    else
    {
        MFnDependencyNode depNode(obj, &status);
        MObject attribute = depNode.attribute(attributeName);
        if (!attribute.isNull())
        {
            MPlug plug(obj, attribute);
            plug.getValue(value);
        }
    }

    return value;
}

void MayaPlug::SetBoolAttribute(const MObject& obj, const MString& attributeName, bool value, TimeAccessor time)
{
    TimeChanger timeChanger(time);

    MStatus status;
    MFnDagNode dagNode(obj, &status);
    if (status == MStatus::kSuccess)
    {
        MObject attribute = dagNode.attribute(attributeName);
        if (!attribute.isNull())
        {
            MPlug plug(obj, attribute);
            plug.setValue(value);
        }
    }
    else
    {
        MFnDependencyNode depNode(obj, &status);
        MObject attribute = depNode.attribute(attributeName);
        if (!attribute.isNull())
        {
            MPlug plug(obj, attribute);
            plug.setValue(value);
        }
    }
}

int MayaPlug::GetIntAttribute(const MObject& obj, const MString& attributeName, int defaultValue, TimeAccessor time)
{
    TimeChanger timeChanger(time);

    int value = defaultValue;

    MStatus status;
    MFnDagNode dagNode(obj, &status);
    if (status == MStatus::kSuccess)
    {
        MObject attribute = dagNode.attribute(attributeName);
        if (!attribute.isNull())
        {
            MPlug plug(obj, attribute);
            plug.getValue(value);
        }
    }
    else
    {
        MFnDependencyNode depNode(obj, &status);
        MObject attribute = depNode.attribute(attributeName);
        if (!attribute.isNull())
        {
            MPlug plug(obj, attribute);
            plug.getValue(value);
        }
    }

    return value;
}

float MayaPlug::GetFloatAttribute(const MObject& obj, const MString& attributeName, float defaultValue, TimeAccessor time)
{
    TimeChanger timeChanger(time);

    float value = defaultValue;

    MStatus status;
    MFnDagNode dagNode(obj, &status);
    if (status == MStatus::kSuccess)
    {
        MObject attribute = dagNode.attribute(attributeName);
        if (!attribute.isNull())
        {
            MPlug plug(obj, attribute);
            plug.getValue(value);
        }
    }
    else
    {
        MFnDependencyNode depNode(obj, &status);
        if (status == MStatus::kSuccess)
        {
            MObject attribute = depNode.attribute(attributeName);
            if (!attribute.isNull())
            {
                MPlug plug(obj, attribute);
                plug.getValue(value);
            }
        }
    }

    return value;
}

FinjinVector2 MayaPlug::GetVector2(MPlug plug, FinjinVector2 defaultValue)
{
    FinjinVector2 result = defaultValue;

    if (!plug.isNull())
    {
        unsigned int count = std::min((unsigned int)2, plug.numChildren());
        for (unsigned int i = 0; i < count; i++)
            result[i] = plug.child(i).asFloat();
    }

    return result;
}

FinjinVector3 MayaPlug::GetVector3(MPlug plug, FinjinVector3 defaultValue)
{
    FinjinVector3 result = defaultValue;

    if (!plug.isNull())
    {
        unsigned int count = std::min((unsigned int)3, plug.numChildren());
        for (unsigned int i = 0; i < count; i++)
            result[i] = plug.child(i).asFloat();
    }

    return result;
}

FinjinVector4 MayaPlug::GetVector4(MPlug plug, FinjinVector4 defaultValue)
{
    FinjinVector4 result = defaultValue;

    if (!plug.isNull())
    {
        unsigned int count = std::min((unsigned int)4, plug.numChildren());
        for (unsigned int i = 0; i < count; i++)
            result[i] = plug.child(i).asDouble(); //Yes, asDouble() is the correct method to call
    }

    return result;
}

void MayaPlug::SetVector2(MPlug plug, const FinjinVector2& value)
{
    unsigned int count = std::min((unsigned int)2, plug.numChildren());
    for (unsigned int i = 0; i < count; i++)
        plug.child(i).setValue(value[i]);
}

void MayaPlug::SetVector3(MPlug plug, const FinjinVector3& value)
{
    unsigned int count = std::min((unsigned int)3, plug.numChildren());
    for (unsigned int i = 0; i < count; i++)
        plug.child(i).setValue(value[i]);
}

void MayaPlug::SetVector4(MPlug plug, const FinjinVector4& value)
{
    unsigned int count = std::min((unsigned int)4, plug.numChildren());
    for (unsigned int i = 0; i < count; i++)
        plug.child(i).setValue((double)value[i]); //Yes, the 'double' method is the correct one to call
}

FinjinColor MayaPlug::GetColor(MPlug plug, FinjinColor defaultValue)
{
    FinjinColor color = defaultValue;

    if (!plug.isNull())
    {
        unsigned int count = std::min((unsigned int)4, plug.numChildren());
        for (unsigned int i = 0; i < count; i++)
            color[i] = plug.child(i).asFloat();
    }

    return color;
}

void MayaPlug::SetColor(MPlug plug, const FinjinColor& value)
{
    unsigned int count = std::min((unsigned int)4, plug.numChildren());
    for (unsigned int i = 0; i < count; i++)
        plug.child(i).setValue(value[i]);
}

FinjinColor MayaPlug::GetColor(MObject object, MObject attribute)
{
    FinjinColor color;
    GetColor(object, attribute, color);
    return color;
}

void MayaPlug::SetColor(MObject object, MObject attribute, const FinjinColor& color)
{
    MFnDependencyNode depNode(object);
    MPlug colorPlug = depNode.findPlug(attribute);
    if (!colorPlug.isNull())
    {
        unsigned int count = std::min((unsigned int)4, colorPlug.numChildren());
        for (unsigned int i = 0; i < count; i++)
            colorPlug.child(i).setValue(color[i]);
    }
}

bool MayaPlug::GetColor(MObject object, MObject attribute, FinjinColor& color)
{
    MFnDependencyNode depNode(object);
    MPlug colorPlug = depNode.findPlug(attribute);
    if (!colorPlug.isNull())
    {
        unsigned int count = std::min((unsigned int)4, colorPlug.numChildren());
        for (unsigned int i = 0; i < count; i++)
            color[i] = colorPlug.child(i).asFloat();
        return true;
    }
    else
        return false;
}

bool MayaPlug::GetColor(MObject object, const MString& attributeName, FinjinColor& color)
{
    MFnDependencyNode depNode(object);
    return GetColor(object, depNode.findPlug(attributeName), color);
}

void MayaPlug::SetColor(MObject object, const MString& attributeName, const FinjinColor& color)
{
    MFnDependencyNode depNode(object);
    SetColor(object, depNode.findPlug(attributeName), color);
}

int MayaPlug::FindLogicalIndexForAppend(MPlug plug)
{
    //Get the logical indices in use
    MIntArray logicalIndices;
    plug.getExistingArrayAttributeIndices(logicalIndices);

    //Use the highest index plus one
    int logicalParameterIndex = -1;
    for (unsigned int i = 0; i < logicalIndices.length(); i++)
        logicalParameterIndex = std::max(logicalParameterIndex, logicalIndices[i]);
    logicalParameterIndex++;

    return logicalParameterIndex;
}

void MayaPlug::AddAttributeElement(const wxString& attributeName, int logicalParameterIndex, const wxString& subattributeName)
{
    //Create the new element by accessing it
    wxString command;
    if (!subattributeName.empty())
        command = wxString::Format(wxT("getAttr(\"%s[%d].%s\")"), attributeName.wx_str(), logicalParameterIndex, subattributeName.wx_str());
    else
        command = wxString::Format(wxT("getAttr(\"%s[%d]\")"), attributeName.wx_str(), logicalParameterIndex);
    MGlobal::executeCommand(WxStringToApplicationString(command), DISPLAY_MAYA_COMMAND);
}

void MayaPlug::AddAttributeElement(const wxString& nodeName, const wxString& attributeName, int logicalParameterIndex, const wxString& subattributeName)
{
    //Create the new element by accessing it
    wxString command;
    if (!subattributeName.empty())
        command = wxString::Format(wxT("getAttr(\"%s.%s[%d].%s\")"), nodeName.wx_str(), attributeName.wx_str(), logicalParameterIndex, subattributeName.wx_str());
    else
        command = wxString::Format(wxT("getAttr(\"%s.%s[%d]\")"), nodeName.wx_str(), attributeName.wx_str(), logicalParameterIndex);
    MGlobal::executeCommand(WxStringToApplicationString(command), DISPLAY_MAYA_COMMAND);
}

int MayaPlug::AddAttributeElement(MPlug plug, const wxString& subattributeName)
{
    int parameterIndex = plug.evaluateNumElements();
    int logicalParameterIndex = FindLogicalIndexForAppend(plug);

    auto attributeName = GetFullAttributeName(plug);
    AddAttributeElement(attributeName, logicalParameterIndex, subattributeName);

    return parameterIndex;
}

int MayaPlug::AddAttributeElement(MObject obj, MObject attribute, const wxString& subattributeName)
{
    return AddAttributeElement(MPlug(obj, attribute), subattributeName);
}

int MayaPlug::AddAttributeElementValue(MPlug plug, int value)
{
    int parameterIndex = AddAttributeElement(plug);
    plug[parameterIndex].setValue(value);
    return parameterIndex;
}

int MayaPlug::AddAttributeElementValue(MPlug plug, float value)
{
    int parameterIndex = AddAttributeElement(plug);
    plug[parameterIndex].setValue(value);
    return parameterIndex;
}

int MayaPlug::AddAttributeElementValue(MPlug plug, const wxString& value)
{
    int parameterIndex = AddAttributeElement(plug);
    plug[parameterIndex].setValue(WxStringToApplicationString(value));
    return parameterIndex;
}

int MayaPlug::AddAttributeElementValue(MPlug plug, const FinjinVector3& value)
{
    int parameterIndex = AddAttributeElement(plug);
    MPlug addedPlug = plug[parameterIndex];
    unsigned int count = std::min((unsigned int)3, addedPlug.numChildren());
    for (unsigned int i = 0; i < count; i++)
        addedPlug.child(i).setValue(value[i]);
    return parameterIndex;
}

int MayaPlug::AddAttributeElementValue(MPlug plug, const FinjinVector4& value)
{
    int parameterIndex = AddAttributeElement(plug);
    MPlug addedPlug = plug[parameterIndex];
    unsigned int count = std::min((unsigned int)4, addedPlug.numChildren());
    for (unsigned int i = 0; i < count; i++)
        addedPlug.child(i).setValue((double)value[i]);
    return parameterIndex;
}

int MayaPlug::AddAttributeElementValue(MObject obj, MObject attribute, int value)
{
    int parameterIndex = AddAttributeElement(obj, attribute);

    MPlug(obj, attribute)[parameterIndex].setValue(value);

    return parameterIndex;
}

int MayaPlug::AddAttributeElementValue(MObject obj, MObject attribute, const wxString& value)
{
    int parameterIndex = AddAttributeElement(obj, attribute);

    MPlug(obj, attribute)[parameterIndex].setValue(WxStringToApplicationString(value));

    return parameterIndex;
}

int MayaPlug::AddAttributeElementValue(MObject obj, MObject attribute, float value)
{
    int parameterIndex = AddAttributeElement(obj, attribute);

    MPlug(obj, attribute)[parameterIndex].setValue(value);

    return parameterIndex;
}

void MayaPlug::RemoveAttributeElement(const wxString& attributeName, int logicalParameterIndex)
{
    wxString command;
    if (attributeName.Last() == wxT(']'))
        command = wxString::Format(wxT("removeMultiInstance(\"-break\", true, \"%s\")"), attributeName.wx_str());
    else
        command = wxString::Format(wxT("removeMultiInstance(\"-break\", true, \"%s[%d]\")"), attributeName.wx_str(), logicalParameterIndex);
    MGlobal::executeCommand(WxStringToApplicationString(command), DISPLAY_MAYA_COMMAND);
}

void MayaPlug::RemoveAttributeElement(const wxString& nodeName, const wxString& attributeName, int logicalParameterIndex)
{
    wxString command;
    if (attributeName.Last() == wxT(']'))
        command = wxString::Format(wxT("removeMultiInstance(\"-break\", true, \"%s.%s\")"), nodeName.wx_str(), attributeName.wx_str());
    else
        command = wxString::Format(wxT("removeMultiInstance(\"-break\", true, \"%s.%s[%d]\")"), nodeName.wx_str(), attributeName.wx_str(), logicalParameterIndex);
    MGlobal::executeCommand(WxStringToApplicationString(command), DISPLAY_MAYA_COMMAND);
}

void MayaPlug::RemoveAttributeElement(MPlug plug)
{
    if (!plug.isNull())
    {
        wxString attributeName = GetFullAttributeName(plug);
        RemoveAttributeElement(attributeName, plug.logicalIndex());
    }
}

void MayaPlug::RemoveAttributeElements(MPlug plug, const std::vector<int>* indices)
{
    if (!plug.isNull())
    {
        wxString attributeName = GetFullAttributeName(plug);

        if (indices != nullptr)
        {
            if (!indices->empty())
            {
                if (indices->size() == 1 || (*indices)[0] <= indices->back())
                {
                    //Indices are in ascending order. Remove in descending (backwards)
                    for (int i = (int)indices->size() - 1; i >= 0; i--)
                        RemoveAttributeElement(attributeName, plug[(*indices)[i]].logicalIndex());
                }
                else
                {
                    //Indices are in descending order, the normal case
                    for (size_t i = 0; i < indices->size(); i++)
                        RemoveAttributeElement(attributeName, plug[(*indices)[i]].logicalIndex());
                }
            }
        }
        else
        {
            //Remove all
            int elementCount = (int)plug.evaluateNumElements();
            for (int i = elementCount - 1; i >= 0; i--)
                RemoveAttributeElement(attributeName, plug[i].logicalIndex());
        }
    }
}

void MayaPlug::RemoveAttributeElements(MObject obj, MObject attribute, const std::vector<int>* indices)
{
    RemoveAttributeElements(MPlug(obj, attribute), indices);
}

MObject MayaPlug::GetObjectReference(MPlug plug)
{
    MObject objectReference;

    if (!plug.isNull())
    {
        MPlugArray inputs;
        plug.connectedTo(inputs, true, false);
        if (inputs.length() > 0)
            objectReference = inputs[0].node();
    }

    return objectReference;
}

MObject MayaPlug::GetObjectReference(MObject object, MObject& attribute)
{
    MPlug plug(object, attribute);
    return GetObjectReference(plug);
}

MObject MayaPlug::GetObjectReference(MObject object, MObject& attribute, int i)
{
    MPlug arrayPlug(object, attribute);
    MPlug objectPlug = arrayPlug[i];
    return GetObjectReference(objectPlug);
}

void MayaPlug::SetObjectReference(MPlug plug, MObject& objectReference)
{
    MString attributeName;
    GetFullAttributeName(plug, attributeName);

    MObject currentObjectReference = GetObjectReference(plug);

    if (objectReference.isNull())
    {
        if (!currentObjectReference.isNull())
        {
            MFnDagNode dagNode(currentObjectReference);
            MFnDependencyNode depNode(currentObjectReference);
            MString fullPathName = dagNode.fullPathName();
            if (fullPathName.length() == 0)
                fullPathName = depNode.name();

            MString command = "disconnectAttr \"";
            command += fullPathName;
            command += ".message\" \"";
            command += attributeName;
            command += "\"";
            MGlobal::executeCommand(command, DISPLAY_MAYA_COMMAND);
        }
    }
    else
    {
        if (objectReference != currentObjectReference)
        {
            MFnDagNode dagNode(objectReference);
            MFnDependencyNode depNode(objectReference);
            MString fullPathName = dagNode.fullPathName();
            if (fullPathName.length() == 0)
                fullPathName = depNode.name();

            MString command = "connectAttr -force \"";
            command += fullPathName;
            command += ".message\" \"";
            command += attributeName;
            command += "\"";
            MGlobal::executeCommand(command, DISPLAY_MAYA_COMMAND);
        }
    }
}

void MayaPlug::SetObjectReference(MObject object, MObject& attribute, MObject& objectReference)
{
    MPlug plug(object, attribute);
    SetObjectReference(plug, objectReference);
}

int MayaPlug::AddObjectReference(MPlug plug, MObject& objectReference)
{
    int parameterIndex = -1;

    if (!objectReference.isNull())
    {
        auto attributeName = GetFullAttributeName(plug);
        parameterIndex = plug.evaluateNumElements();
        int logicalParameterIndex = FindLogicalIndexForAppend(plug);

        MFnDependencyNode depNode(objectReference);
        wxString objectReferenceName = ApplicationStringToWxString(depNode.name());

        auto command = wxString::Format(wxT("connectAttr -force \"%s.message\" \"%s[%d]\""), objectReferenceName.wx_str(), attributeName.wx_str(), logicalParameterIndex);
        MGlobal::executeCommand(WxStringToApplicationString(command), DISPLAY_MAYA_COMMAND);
    }

    return parameterIndex;
}

int MayaPlug::AddObjectReference(MObject object, MObject& attribute, MObject& objectReference)
{
    MPlug plug(object, attribute);
    return AddObjectReference(plug, objectReference);
}

int MayaPlug::GetObjectReferenceCount(MPlug plug, std::vector<int>* deletedIndices)
{
    unsigned int count = 0;
    if (!plug.isNull())
    {
        count = plug.evaluateNumElements();
        for (int i = (int)count - 1; i >= 0; i--)
        {
            MPlug objectReferencePlug = plug[i];
            MPlugArray inputs;
            objectReferencePlug.connectedTo(inputs, true, false);
            if (inputs.length() == 0)
            {
                RemoveAttributeElement(objectReferencePlug);

                count--;

                if (deletedIndices != nullptr)
                    deletedIndices->push_back(i);
            }
        }
    }
    return (int)count;
}

int MayaPlug::GetObjectReferenceCount(MObject object, MObject& attribute, std::vector<int>* deletedIndices)
{
    MPlug plug(object, attribute);
    return GetObjectReferenceCount(plug, deletedIndices);
}

wxString MayaPlug::GetBaseAttributeName(MPlug& plug)
{
    wxString result;

    auto partialName = ApplicationStringToWxString(plug.partialName());
    StringSplitter parts(partialName, wxT('.'));
    if (parts.size() > 0)
        result = parts[parts.size() - 1];
    else
        result = partialName;

    return result;
}

void MayaPlug::GetBaseAttributeName(MPlug& plug, MString& name)
{
    auto partialName = ApplicationStringToWxString(plug.partialName());
    StringSplitter parts(partialName, wxT('.'));
    if (parts.size() > 0)
        name = WxStringToApplicationString(parts[parts.size() - 1]);
    else
        name = WxStringToApplicationString(partialName);
}

wxString MayaPlug::GetFullAttributeName(MPlug& plug)
{
    MString name;
    GetFullAttributeName(plug, name);
    return ApplicationStringToWxString(name);
}

void MayaPlug::GetFullAttributeName(MPlug& plug, MString& name)
{
    name = plug.partialName(true, false, false, false, true, true);
}
