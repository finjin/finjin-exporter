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
#include "FinjinNodeAnimationSettingsAccessor.hpp"
#include "NodeAttributeAdder.hpp"
#include "MayaUtilities.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
FinjinNodeAnimationSettingsAccessor::Attributes FinjinNodeAnimationSettingsAccessor::attributes;


//Implementation---------------------------------------------------------------
FinjinNodeAnimationSettingsAccessor::FinjinNodeAnimationSettingsAccessor() 
{    
}

FinjinNodeAnimationSettingsAccessor::FinjinNodeAnimationSettingsAccessor(MPlug rootPlug) 
{
    this->rootPlug = rootPlug;
}

bool FinjinNodeAnimationSettingsAccessor::operator == (const FinjinNodeAnimationSettingsAccessor& other) const 
{
    return this->rootPlug == other.rootPlug;
}

wxString FinjinNodeAnimationSettingsAccessor::GetAnimationName()
{
    return ApplicationStringToWxString(ChildString(attributes.animationName, "NodeAnimation_AnimationName"));
}

wxString FinjinNodeAnimationSettingsAccessor::GetTimeScaleType()
{
    return ApplicationStringToWxString(ChildString(attributes.timeScaleType, "NodeAnimation_TimeScaleType"));
}

double FinjinNodeAnimationSettingsAccessor::GetTimeScale()
{
    return ChildDouble(attributes.timeScale, "NodeAnimation_TimeScale");
}

bool FinjinNodeAnimationSettingsAccessor::GetCopyFirstKeyToLast()
{
    return ChildBool(attributes.copyFirstKeyToLast, "NodeAnimation_CopyFirstKeyToLast");
}

bool FinjinNodeAnimationSettingsAccessor::GetUseAnimationStartTime()
{
    return ChildBool(attributes.useAnimationStartTime, "NodeAnimation_UseAnimationStartTime");
}

bool FinjinNodeAnimationSettingsAccessor::GetEmbedAnimation()
{
    return ChildBool(attributes.embedAnimation, "NodeAnimation_EmbedAnimation");
}

bool FinjinNodeAnimationSettingsAccessor::GetLinkToMainObject()
{
    return ChildBool(attributes.linkToMainObject, "NodeAnimation_LinkToMainObject");
}

bool FinjinNodeAnimationSettingsAccessor::GetEnabled()
{
    return ChildBool(attributes.enabled, "NodeAnimation_Enabled");
}

bool FinjinNodeAnimationSettingsAccessor::GetLooped()
{
    return ChildBool(attributes.looped, "NodeAnimation_Looped");
}

double FinjinNodeAnimationSettingsAccessor::GetSampleInterval()
{
    return ChildDouble(attributes.sampleInterval, "NodeAnimation_SampleInterval");
}

SampleType FinjinNodeAnimationSettingsAccessor::GetSampleType()
{
    return (SampleType)ChildInt(attributes.sampleType, "NodeAnimation_SampleType", (int)SampleType::USE_PARENT);
}

FinjinTimeInterval FinjinNodeAnimationSettingsAccessor::GetTimeInterval()
{
    FinjinTimeInterval interval;
    interval.start.SetSeconds(ChildDouble(attributes.timeStart, "NodeAnimation_TimeStart"));
    interval.end.SetSeconds(ChildDouble(attributes.timeEnd, "NodeAnimation_TimeEnd"));
    return interval;
}

wxString FinjinNodeAnimationSettingsAccessor::GetTranslationInterpolationType()
{
    return ApplicationStringToWxString(ChildString(attributes.translationInterpolationType, "NodeAnimation_TranslationInterpolationType"));
}

wxString FinjinNodeAnimationSettingsAccessor::GetRotationInterpolationType()
{
    return ApplicationStringToWxString(ChildString(attributes.rotationInterpolationType, "NodeAnimation_RotationInterpolationType"));
}

void FinjinNodeAnimationSettingsAccessor::SetAnimationName(wxString value)
{
    Child(attributes.animationName, "NodeAnimation_AnimationName").setValue(WxStringToApplicationString(value));
}

void FinjinNodeAnimationSettingsAccessor::SetTimeScaleType(wxString value)
{
    Child(attributes.timeScaleType, "NodeAnimation_TimeScaleType").setValue(WxStringToApplicationString(value));
}

void FinjinNodeAnimationSettingsAccessor::SetTimeScale(double value)
{
    Child(attributes.timeScale, "NodeAnimation_TimeScale").setValue(value);
}

void FinjinNodeAnimationSettingsAccessor::SetCopyFirstKeyToLast(bool value)
{
    Child(attributes.copyFirstKeyToLast, "NodeAnimation_CopyFirstKeyToLast").setValue(value);
}

void FinjinNodeAnimationSettingsAccessor::SetUseAnimationStartTime(bool value)
{
    Child(attributes.useAnimationStartTime, "NodeAnimation_UseAnimationStartTime ").setValue(value);
}

void FinjinNodeAnimationSettingsAccessor::SetEmbedAnimation(bool value)
{
    Child(attributes.embedAnimation, "NodeAnimation_EmbedAnimation").setValue(value);
}

void FinjinNodeAnimationSettingsAccessor::SetLinkToMainObject(bool value)
{
    Child(attributes.linkToMainObject, "NodeAnimation_LinkToMainObject").setValue(value);
}

void FinjinNodeAnimationSettingsAccessor::SetEnabled(bool value)
{
    Child(attributes.enabled, "NodeAnimation_Enabled").setValue(value);
}

void FinjinNodeAnimationSettingsAccessor::SetLooped(bool value)
{
    Child(attributes.looped, "NodeAnimation_Looped").setValue(value);
}

void FinjinNodeAnimationSettingsAccessor::SetSampleInterval(double value)
{
    Child(attributes.sampleInterval, "NodeAnimation_SampleInterval").setValue(value);
}

void FinjinNodeAnimationSettingsAccessor::SetSampleType(SampleType value)
{
    Child(attributes.sampleType, "NodeAnimation_SampleType").setValue((int)value);
}

void FinjinNodeAnimationSettingsAccessor::SetTimeInterval(FinjinTimeInterval interval)
{
    Child(attributes.timeStart, "NodeAnimation_TimeStart").setValue((double)interval.start.GetSeconds());
    Child(attributes.timeEnd, "NodeAnimation_TimeEnd").setValue((double)interval.end.GetSeconds());
}

void FinjinNodeAnimationSettingsAccessor::SetTranslationInterpolationType(wxString value)
{
    Child(attributes.translationInterpolationType, "NodeAnimation_TranslationInterpolationType").setValue(WxStringToApplicationString(value));
}

void FinjinNodeAnimationSettingsAccessor::SetRotationInterpolationType(wxString value)
{
    Child(attributes.rotationInterpolationType, "NodeAnimation_RotationInterpolationType").setValue(WxStringToApplicationString(value));
}

MPlug FinjinNodeAnimationSettingsAccessor::Child(MObject attribute, const MString& name)
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

int FinjinNodeAnimationSettingsAccessor::ChildInt(MObject attribute, const MString& name, int defaultValue)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? defaultValue : plug.asInt();
}

bool FinjinNodeAnimationSettingsAccessor::ChildBool(MObject attribute, const MString& name, bool defaultValue)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? defaultValue : plug.asBool();
}

float FinjinNodeAnimationSettingsAccessor::ChildFloat(MObject attribute, const MString& name, float defaultValue)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? defaultValue : plug.asFloat();
}

double FinjinNodeAnimationSettingsAccessor::ChildDouble(MObject attribute, const MString& name, double defaultValue)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? defaultValue : plug.asDouble();
}

MString FinjinNodeAnimationSettingsAccessor::ChildString(MObject attribute, const MString& name)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? "" : plug.asString();
}

int FinjinNodeAnimationSettingsAccessor::ChildNumElements(MObject attribute, const MString& name)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? 0 : plug.evaluateNumElements();
}

MObject FinjinNodeAnimationSettingsAccessor::CreateSettingsArrayAttribute(NodeAttributeAdder& adder, const MString& name)
{
    std::list<MObject> attributesList;

    attributesList.push_back(attributes.updateCounter = adder.AddInt("NodeAnimation_UpdateCounter"));

    attributesList.push_back(attributes.animationName = adder.AddString("NodeAnimation_AnimationName"));
    attributesList.push_back(attributes.timeStart = adder.AddDouble("NodeAnimation_TimeStart"));
    attributesList.push_back(attributes.timeEnd = adder.AddDouble("NodeAnimation_TimeEnd"));
    attributesList.push_back(attributes.timeScaleType = adder.AddString("NodeAnimation_TimeScaleType"));
    attributesList.push_back(attributes.timeScale = adder.AddDouble("NodeAnimation_TimeScale"));
    attributesList.push_back(attributes.copyFirstKeyToLast = adder.AddBool("NodeAnimation_CopyFirstKeyToLast"));
    attributesList.push_back(attributes.useAnimationStartTime = adder.AddBool("NodeAnimation_UseAnimationStartTime"));
    attributesList.push_back(attributes.embedAnimation = adder.AddBool("NodeAnimation_EmbedAnimation"));
    attributesList.push_back(attributes.linkToMainObject = adder.AddBool("NodeAnimation_LinkToMainObject"));
    attributesList.push_back(attributes.enabled = adder.AddBool("NodeAnimation_Enabled"));
    attributesList.push_back(attributes.looped = adder.AddBool("NodeAnimation_Looped"));
    attributesList.push_back(attributes.sampleInterval = adder.AddDouble("NodeAnimation_SampleInterval"));
    attributesList.push_back(attributes.sampleType = adder.AddInt("NodeAnimation_SampleType", (int)SampleType::USE_PARENT));
    attributesList.push_back(attributes.translationInterpolationType = adder.AddString("NodeAnimation_TranslationInterpolationType"));
    attributesList.push_back(attributes.rotationInterpolationType = adder.AddString("NodeAnimation_RotationInterpolationType"));
 
    return adder.AddCompoundArray(name, attributesList);
}
