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
#include "FinjinColor.hpp"
#include "TimeAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Utility functions for dealing with Maya plugs. */
    class MayaPlug
    {
    public:
        static MPlug FindChildPlug(MPlug& rootPlug, MObject attribute, const MString& name);
        static MPlug FindChildPlug(MPlug& rootPlug, const MString& name);

        static bool GetBoolAttribute(const MObject& obj, const MString& attributeName, bool defaultValue = false, TimeAccessor time = TimeAccessor());
        static void SetBoolAttribute(const MObject& obj, const MString& attributeName, bool value, TimeAccessor time = TimeAccessor());

        static int GetIntAttribute(const MObject& obj, const MString& attributeName, int defaultValue = 0, TimeAccessor time = TimeAccessor());
        static float GetFloatAttribute(const MObject& obj, const MString& attributeName, float defaultValue = 0, TimeAccessor time = TimeAccessor());

        static FinjinVector2 GetVector2(MPlug plug, FinjinVector2 defaultValue = FinjinVector2(0,0));
        static FinjinVector3 GetVector3(MPlug plug, FinjinVector3 defaultValue = FinjinVector3(0,0,0));
        static FinjinVector4 GetVector4(MPlug plug, FinjinVector4 defaultValue = FinjinVector4(0,0,0,0));

        static void SetVector2(MPlug plug, const FinjinVector2& value);
        static void SetVector3(MPlug plug, const FinjinVector3& value);
        static void SetVector4(MPlug plug, const FinjinVector4& value);

        static FinjinColor GetColor(MPlug plug, FinjinColor defaultValue = FinjinColor(0,0,0));
        static void SetColor(MPlug plug, const FinjinColor& value);

        static FinjinColor GetColor(MObject object, MObject attribute);
        static void SetColor(MObject object, MObject attribute, const FinjinColor& color);
        static bool GetColor(MObject object, MObject attribute, FinjinColor& color);
        static bool GetColor(MObject object, const MString& attributeName, FinjinColor& color);
        static void SetColor(MObject object, const MString& attributeName, const FinjinColor& color);

        static int FindLogicalIndexForAppend(MPlug plug);

        static int AddAttributeElement(MPlug plug, const wxString& subattributeName = wxEmptyString);
        static int AddAttributeElement(MObject obj, MObject attribute, const wxString& subattributeName = wxEmptyString);

        static int AddAttributeElementValue(MPlug plug, int value);
        static int AddAttributeElementValue(MPlug plug, float value);
        static int AddAttributeElementValue(MPlug plug, const wxString& value);
        static int AddAttributeElementValue(MPlug plug, const FinjinVector3& value);
        static int AddAttributeElementValue(MPlug plug, const FinjinVector4& value);

        static int AddAttributeElementValue(MObject obj, MObject attribute, int value);
        static int AddAttributeElementValue(MObject obj, MObject attribute, float value);
        static int AddAttributeElementValue(MObject obj, MObject attribute, const wxString& value);

        static void RemoveAttributeElement(const wxString& attributeName, int logicalParameterIndex);
        static void RemoveAttributeElement(const wxString& nodeName, const wxString& attributeName, int logicalParameterIndex);
        static void RemoveAttributeElement(MPlug plug);

        static void RemoveAttributeElements(MPlug plug, const std::vector<int>* indices = nullptr);
        static void RemoveAttributeElements(MObject obj, MObject attribute, const std::vector<int>* indices = nullptr);

        static MObject GetObjectReference(MPlug plug);
        static MObject GetObjectReference(MObject object, MObject& attribute);
        static MObject GetObjectReference(MObject object, MObject& attribute, int i);
        static void SetObjectReference(MPlug plug, MObject& objectReference);
        static void SetObjectReference(MObject object, MObject& attribute, MObject& objectReference);
        static int AddObjectReference(MPlug plug, MObject& objectReference);
        static int AddObjectReference(MObject object, MObject& attribute, MObject& objectReference);
        static int GetObjectReferenceCount(MPlug plug, std::vector<int>* deletedIndices = nullptr);
        static int GetObjectReferenceCount(MObject object, MObject& attribute, std::vector<int>* deletedIndices = nullptr);

        static wxString GetBaseAttributeName(MPlug& plug);
        static void GetBaseAttributeName(MPlug& plug, MString& name);

        static wxString GetFullAttributeName(MPlug& plug);
        static void GetFullAttributeName(MPlug& plug, MString& name);

    private:
        static void AddAttributeElement(const wxString& attributeName, int logicalParameterIndex, const wxString& subattributeName = wxEmptyString);
        static void AddAttributeElement(const wxString& nodeName, const wxString& attributeName, int logicalParameterIndex, const wxString& subattributeName = wxEmptyString);
    };

} }
