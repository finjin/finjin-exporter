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
#include "FinjinMeshAnimationSettingsAccessor.hpp"
#include "ApplicationAccessor.hpp"
#include "NodeAttributeAdder.hpp"
#include "MayaUtilities.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
FinjinMeshAnimationSettingsAccessor::Attributes FinjinMeshAnimationSettingsAccessor::attributes;


//Implementation---------------------------------------------------------------
FinjinMeshAnimationSettingsAccessor::FinjinMeshAnimationSettingsAccessor() 
{
}

FinjinMeshAnimationSettingsAccessor::FinjinMeshAnimationSettingsAccessor(MPlug rootPlug) 
{
    this->rootPlug = rootPlug;
}

bool FinjinMeshAnimationSettingsAccessor::operator == (const FinjinMeshAnimationSettingsAccessor& other) const 
{
    return this->rootPlug == other.rootPlug;
}

wxString FinjinMeshAnimationSettingsAccessor::GetTrack()
{
    return ApplicationStringToWxString(ChildString(attributes.trackName, "MeshAnimation_TrackName"));
}

wxString FinjinMeshAnimationSettingsAccessor::GetAnimationName()
{
    return ApplicationStringToWxString(ChildString(attributes.animationName, "MeshAnimation_AnimationName"));
}

wxString FinjinMeshAnimationSettingsAccessor::GetTimeScaleType()
{
    return ApplicationStringToWxString(ChildString(attributes.timeScaleType, "MeshAnimation_TimeScaleType"));
}

double FinjinMeshAnimationSettingsAccessor::GetTimeScale()
{
    return ChildDouble(attributes.timeScale, "MeshAnimation_TimeScale");
}

bool FinjinMeshAnimationSettingsAccessor::GetCopyFirstKeyToLast()
{
    return ChildBool(attributes.copyFirstKeyToLast, "MeshAnimation_CopyFirstKeyToLast");
}

bool FinjinMeshAnimationSettingsAccessor::GetExportAsMorph()
{
    return ChildBool(attributes.exportAsMorph, "MeshAnimation_ExportAsMorph");
}

InheritedBool FinjinMeshAnimationSettingsAccessor::GetMorphWholeObject()
{
    return (InheritedBool)ChildInt(attributes.morphWholeObject, "MeshAnimation_MorphWholeObject", (int)INHERITED_USE_PARENT_BOOL);
}

bool FinjinMeshAnimationSettingsAccessor::GetOverrideAnimatedRootStartTime()
{
    return ChildBool(attributes.overrideAnimatedRootStartTime, "MeshAnimation_OverrideAnimatedRootStartTime");
}

TimeAccessor FinjinMeshAnimationSettingsAccessor::GetAnimatedRootStartTime()
{
    TimeAccessor value;
    value.SetSeconds(ChildDouble(attributes.animatedRootStartTime, "MeshAnimation_AnimatedRootStartTime"));
    return value;
}

bool FinjinMeshAnimationSettingsAccessor::GetUseAnimationStartTime()
{
    return ChildBool(attributes.useAnimationStartTime, "MeshAnimation_UseAnimationStartTime");
}

bool FinjinMeshAnimationSettingsAccessor::GetLooped()
{
    return ChildBool(attributes.looped, "MeshAnimation_Looped");
}

double FinjinMeshAnimationSettingsAccessor::GetSampleInterval()
{
    return ChildDouble(attributes.sampleInterval, "MeshAnimation_SampleInterval");
}

SampleType FinjinMeshAnimationSettingsAccessor::GetSampleType()
{
    return (SampleType)ChildInt(attributes.sampleType, "MeshAnimation_SampleType", (int)SampleType::USE_PARENT);
}

FinjinTimeInterval FinjinMeshAnimationSettingsAccessor::GetTimeInterval()
{
    FinjinTimeInterval interval;
    interval.start.SetSeconds(ChildDouble(attributes.timeStart, "MeshAnimation_TimeStart"));
    interval.end.SetSeconds(ChildDouble(attributes.timeEnd, "MeshAnimation_TimeEnd"));
    return interval;
}

bool FinjinMeshAnimationSettingsAccessor::GetEmbedAnimation()
{
    return ChildBool(attributes.embedAnimation, "MeshAnimation_EmbedAnimation");
}

bool FinjinMeshAnimationSettingsAccessor::GetLinkToMainObject()
{
    return ChildBool(attributes.linkToMainObject, "MeshAnimation_LinkToMainObject");
}

int FinjinMeshAnimationSettingsAccessor::GetBoneTranslationMaskCount()
{
    std::vector<int> deletedIndices;
    int count = MayaPlug::GetObjectReferenceCount(Child(attributes.boneTranslationMaskObjects, "MeshAnimation_BoneTranslationMaskObjects"), &deletedIndices);
    if (!deletedIndices.empty())
        MayaPlug::RemoveAttributeElements(Child(attributes.boneTranslationMasks, "MeshAnimation_BoneTranslationMasks"), &deletedIndices);
    return count;
}

void FinjinMeshAnimationSettingsAccessor::GetBoneTranslationMask(int index, ObjectAccessor& object, FinjinVector3& mask)
{
    object = MayaPlug::GetObjectReference(Child(attributes.boneTranslationMaskObjects, "MeshAnimation_BoneTranslationMaskObjects")[index]);
    mask = MayaPlug::GetVector3(Child(attributes.boneTranslationMaskObjects, "MeshAnimation_BoneTranslationMasks")[index]);    
}

FinjinVector3 FinjinMeshAnimationSettingsAccessor::GetBoneTranslationMask(ObjectAccessor object)
{
    FinjinVector3 mask(1,1,1);

    ObjectAccessor maskObject;
    int count = GetBoneTranslationMaskCount();
    for (int i = 0; i < count; i++)
    {
        GetBoneTranslationMask(i, maskObject, mask);
        if (maskObject == object)
            break;
    }

    return mask;
}

int FinjinMeshAnimationSettingsAccessor::GetAllowExportBoneCount()
{   
    std::vector<int> deletedIndices;
    int count = MayaPlug::GetObjectReferenceCount(Child(attributes.allowExportBonesObjects, "MeshAnimation_AllowExportBones"), &deletedIndices);
    if (!deletedIndices.empty())
    {
        MayaPlug::RemoveAttributeElements(Child(attributes.allowExportBoneAllows, "MeshAnimation_AllowExportBoneAllows"), &deletedIndices);
        MayaPlug::RemoveAttributeElements(Child(attributes.allowExportBoneRecursives, "MeshAnimation_AllowExportBoneRecursives"), &deletedIndices);
    }
    return count;
}

void FinjinMeshAnimationSettingsAccessor::GetAllowExportBone(int i, ObjectAccessor& object, bool& allow, bool& recursive)
{
    MPlug allowBonesPlug = Child(attributes.allowExportBonesObjects, "MeshAnimation_AllowExportBones");
    MPlug allowsPlug = Child(attributes.allowExportBoneAllows, "MeshAnimation_AllowExportBoneAllows");
    MPlug recursivesPlug = Child(attributes.allowExportBoneRecursives, "MeshAnimation_AllowExportBoneRecursives");

    if (!allowBonesPlug.isNull() && !allowsPlug.isNull() && !recursivesPlug.isNull())
    {
        object = MayaPlug::GetObjectReference(allowBonesPlug[i]);
        allow = allowsPlug[i].asInt() ? true : false;
        recursive = recursivesPlug[i].asInt() ? true : false;
    }
    else
    {
        object = ObjectAccessor();
        allow = false;
        recursive = false;
    }
}

void FinjinMeshAnimationSettingsAccessor::SetTrack(wxString value)
{
    Child(attributes.trackName, "MeshAnimation_TrackName").setValue(WxStringToApplicationString(value));
}

void FinjinMeshAnimationSettingsAccessor::SetAnimationName(wxString value)
{
    Child(attributes.animationName, "MeshAnimation_AnimationName").setValue(WxStringToApplicationString(value));
}

void FinjinMeshAnimationSettingsAccessor::SetTimeScaleType(wxString value)
{
    Child(attributes.timeScaleType, "MeshAnimation_TimeScaleType").setValue(WxStringToApplicationString(value));
}

void FinjinMeshAnimationSettingsAccessor::SetTimeScale(double value)
{
    Child(attributes.timeScale, "MeshAnimation_TimeScale").setValue(value);
}

void FinjinMeshAnimationSettingsAccessor::SetCopyFirstKeyToLast(bool value)
{
    Child(attributes.copyFirstKeyToLast, "MeshAnimation_CopyFirstKeyToLast").setValue(value);
}

void FinjinMeshAnimationSettingsAccessor::SetExportAsMorph(bool value)
{
    Child(attributes.exportAsMorph, "MeshAnimation_ExportAsMorph").setValue(value);
}

void FinjinMeshAnimationSettingsAccessor::SetMorphWholeObject(InheritedBool value)
{
    Child(attributes.morphWholeObject, "MeshAnimation_MorphWholeObject").setValue((int)value);
}

void FinjinMeshAnimationSettingsAccessor::SetOverrideAnimatedRootStartTime(bool value)
{
    Child(attributes.overrideAnimatedRootStartTime, "MeshAnimation_OverrideAnimatedRootStartTime").setValue(value);    
}

void FinjinMeshAnimationSettingsAccessor::SetAnimatedRootStartTime(TimeAccessor value)
{
    Child(attributes.animatedRootStartTime, "MeshAnimation_AnimatedRootStartTime").setValue((double)value.GetSeconds());
}

void FinjinMeshAnimationSettingsAccessor::SetUseAnimationStartTime(bool value)
{
    Child(attributes.useAnimationStartTime, "MeshAnimation_UseAnimationStartTime").setValue(value);
}

void FinjinMeshAnimationSettingsAccessor::SetLooped(bool value)
{
    Child(attributes.looped, "MeshAnimation_Looped").setValue(value);
}

void FinjinMeshAnimationSettingsAccessor::SetSampleInterval(double value)
{
    Child(attributes.sampleInterval, "MeshAnimation_SampleInterval").setValue(value);
}

void FinjinMeshAnimationSettingsAccessor::SetSampleType(SampleType value)
{
    Child(attributes.sampleType, "MeshAnimation_SampleType").setValue((int)value);
}

void FinjinMeshAnimationSettingsAccessor::SetTimeInterval(FinjinTimeInterval interval)
{
    Child(attributes.timeStart, "MeshAnimation_TimeStart").setValue((double)interval.start.GetSeconds());
    Child(attributes.timeEnd, "MeshAnimation_TimeEnd").setValue((double)interval.end.GetSeconds());
}

void FinjinMeshAnimationSettingsAccessor::SetEmbedAnimation(bool value)
{
    Child(attributes.embedAnimation, "MeshAnimation_EmbedAnimation").setValue(value);
}

void FinjinMeshAnimationSettingsAccessor::SetLinkToMainObject(bool value)
{
    Child(attributes.linkToMainObject, "MeshAnimation_LinkToMainObject").setValue(value);
}

void FinjinMeshAnimationSettingsAccessor::ClearBoneTranslationMasks()
{
    MayaPlug::RemoveAttributeElements(Child(attributes.boneTranslationMaskObjects, "MeshAnimation_BoneTranslationMaskObjects"));
    MayaPlug::RemoveAttributeElements(Child(attributes.boneTranslationMasks, "MeshAnimation_BoneTranslationMasks"));    
}

void FinjinMeshAnimationSettingsAccessor::AddBoneTranslationMask(ObjectAccessor object, FinjinVector3 mask)
{
    MayaPlug::AddObjectReference(Child(attributes.boneTranslationMaskObjects, "MeshAnimation_BoneTranslationMaskObjects"), object.obj);
    MayaPlug::AddAttributeElementValue(Child(attributes.boneTranslationMasks, "MeshAnimation_BoneTranslationMasks"), mask);    
}

void FinjinMeshAnimationSettingsAccessor::ClearAllowExportBones()
{
    MayaPlug::RemoveAttributeElements(Child(attributes.allowExportBonesObjects, "MeshAnimation_AllowExportBones"));
    MayaPlug::RemoveAttributeElements(Child(attributes.allowExportBoneAllows, "MeshAnimation_AllowExportBoneAllows"));
    MayaPlug::RemoveAttributeElements(Child(attributes.allowExportBoneRecursives, "MeshAnimation_AllowExportBoneRecursives"));
}

void FinjinMeshAnimationSettingsAccessor::AddAllowExportBone(ObjectAccessor object, bool allow, bool recursive)
{
    MayaPlug::AddObjectReference(Child(attributes.allowExportBonesObjects, "MeshAnimation_AllowExportBones"), object.obj);
    MayaPlug::AddAttributeElementValue(Child(attributes.allowExportBoneAllows, "MeshAnimation_AllowExportBoneAllows"), (int)allow);
    MayaPlug::AddAttributeElementValue(Child(attributes.allowExportBoneRecursives, "MeshAnimation_AllowExportBoneRecursives"), (int)recursive);
}

MPlug FinjinMeshAnimationSettingsAccessor::Child(MObject attribute, const MString& name)
{
    MPlug result;

    if (this->rootPlug.isDynamic())    
    {
        MString plugName;
        unsigned int count = this->rootPlug.numChildren();
        for (unsigned int i = 0; i < count; i++)
        {
            MPlug plug = this->rootPlug.child(i);
            MayaPlug::GetBaseAttributeName(plug, plugName);
            if (plugName == name)
            {
                result = plug;
                break;
            }
        }
    }
    else
        result = this->rootPlug.child(attribute);
    
    return result;
}

int FinjinMeshAnimationSettingsAccessor::ChildInt(MObject attribute, const MString& name, int defaultValue)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? defaultValue : plug.asInt();
}

bool FinjinMeshAnimationSettingsAccessor::ChildBool(MObject attribute, const MString& name, bool defaultValue)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? defaultValue : plug.asBool();
}

float FinjinMeshAnimationSettingsAccessor::ChildFloat(MObject attribute, const MString& name, float defaultValue)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? defaultValue : plug.asFloat();
}

double FinjinMeshAnimationSettingsAccessor::ChildDouble(MObject attribute, const MString& name, double defaultValue)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? defaultValue : plug.asDouble();
}

MString FinjinMeshAnimationSettingsAccessor::ChildString(MObject attribute, const MString& name)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? "" : plug.asString();
}

int FinjinMeshAnimationSettingsAccessor::ChildNumElements(MObject attribute, const MString& name)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? 0 : plug.evaluateNumElements();
}

MObject FinjinMeshAnimationSettingsAccessor::CreateSettingsArrayAttribute(NodeAttributeAdder& adder, const MString& name)
{
    std::list<MObject> attributesList;

    attributesList.push_back(attributes.updateCounter = adder.AddInt("MeshAnimation_UpdateCounter"));

    attributesList.push_back(attributes.trackName = adder.AddString("MeshAnimation_TrackName"));
    attributesList.push_back(attributes.animationName = adder.AddString("MeshAnimation_AnimationName"));
    attributesList.push_back(attributes.timeStart = adder.AddDouble("MeshAnimation_TimeStart"));
    attributesList.push_back(attributes.timeEnd = adder.AddDouble("MeshAnimation_TimeEnd"));
    attributesList.push_back(attributes.timeScaleType = adder.AddString("MeshAnimation_TimeScaleType"));
    attributesList.push_back(attributes.timeScale = adder.AddDouble("MeshAnimation_TimeScale"));
    attributesList.push_back(attributes.copyFirstKeyToLast = adder.AddBool("MeshAnimation_CopyFirstKeyToLast"));
    attributesList.push_back(attributes.exportAsMorph = adder.AddBool("MeshAnimation_ExportAsMorph"));
    attributesList.push_back(attributes.morphWholeObject = adder.AddInt("MeshAnimation_MorphWholeObject", (int)INHERITED_USE_PARENT_BOOL));
    attributesList.push_back(attributes.overrideAnimatedRootStartTime = adder.AddBool("MeshAnimation_OverrideAnimatedRootStartTime"));
    attributesList.push_back(attributes.animatedRootStartTime = adder.AddDouble("MeshAnimation_AnimatedRootStartTime"));
    attributesList.push_back(attributes.useAnimationStartTime = adder.AddBool("MeshAnimation_UseAnimationStartTime"));
    attributesList.push_back(attributes.looped = adder.AddBool("MeshAnimation_Looped"));
    attributesList.push_back(attributes.sampleInterval = adder.AddDouble("MeshAnimation_SampleInterval"));
    attributesList.push_back(attributes.sampleType = adder.AddInt("MeshAnimation_SampleType", (int)SampleType::USE_PARENT));
    attributesList.push_back(attributes.embedAnimation = adder.AddBool("MeshAnimation_EmbedAnimation"));
    attributesList.push_back(attributes.linkToMainObject = adder.AddBool("MeshAnimation_LinkToMainObject"));
    
    attributesList.push_back(attributes.boneTranslationMaskObjects = adder.AddStringArray("MeshAnimation_BoneTranslationMaskObjects"));
    attributesList.push_back(attributes.boneTranslationMasks = adder.AddFloat3Array("MeshAnimation_BoneTranslationMasks"));
    
    attributesList.push_back(attributes.allowExportBonesObjects = adder.AddStringArray("MeshAnimation_AllowExportBonesObjects"));
    attributesList.push_back(attributes.allowExportBoneAllows = adder.AddIntArray("MeshAnimation_AllowExportBoneAllows"));
    attributesList.push_back(attributes.allowExportBoneRecursives = adder.AddIntArray("MeshAnimation_AllowExportBoneRecursives"));
 
    return adder.AddCompoundArray(name, attributesList);
}
