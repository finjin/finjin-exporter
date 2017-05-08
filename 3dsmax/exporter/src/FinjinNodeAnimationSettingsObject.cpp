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
#include "MaxUtilities.hpp"
#include "FinjinNodeAnimationSettingsObject.hpp"
#include "FinjinSceneSettingsObject.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "FinjinMaxClassID.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Local types-------------------------------------------------------------------
class FinjinNodeAnimationSettingsObjectClassDesc : public ClassDesc2
{
public:
    int IsPublic() override                      {return FALSE;}
    void* Create(BOOL loading = FALSE) override  {return new FinjinNodeAnimationSettingsObject();}
    const MCHAR* ClassName() override            {return MaxUtilities::GetStringM(IDS_NODE_ANIMATION_SETTINGS_OBJECT_CLASS_NAME);}
    SClass_ID SuperClassID() override            {return REF_TARGET_CLASS_ID;}
    Class_ID ClassID() override                  {return FinjinNodeAnimationSettingsObject::GetClassClassID();}
    const MCHAR* Category() override             {return MaxUtilities::GetStringM(IDS_PLUGINS_CATEGORY);}
    const MCHAR* InternalName() override         {return _M("FinjinNodeAnimationSettingsObject");}
    HINSTANCE HInstance() override               {return wxGetInstance();}

public:
    static FinjinNodeAnimationSettingsObjectClassDesc instance;
};
FinjinNodeAnimationSettingsObjectClassDesc FinjinNodeAnimationSettingsObjectClassDesc::instance;


//Globals-----------------------------------------------------------------------
enum {OBJECT_SETTINGS_PARAM_BLOCK_ID = 0};
enum {OBJECT_SETTINGS_VERSION = 1};

static ParamBlockDesc2 FinjinNodeAnimationSettingsObjectParamBlock
    (
    OBJECT_SETTINGS_PARAM_BLOCK_ID, _M("Parameters"),  0, &FinjinNodeAnimationSettingsObjectClassDesc::instance, P_VERSION | P_AUTO_CONSTRUCT,

    //Version
    OBJECT_SETTINGS_VERSION,

    //Reference number
    FinjinNodeAnimationSettingsObject::PARAM_BLOCK_REF,

    //Parameters
    FinjinNodeAnimationSettingsObject::PB_ANIMATION_NAME, _M("AnimationName"), TYPE_STRING, 0, IDS_ANIMATION_NAME,
        p_end,
    FinjinNodeAnimationSettingsObject::PB_TIME_START, _M("TimeStart"), TYPE_TIMEVALUE, 0, IDS_TIME_START,
        p_end,
    FinjinNodeAnimationSettingsObject::PB_TIME_END, _M("TimeEnd"), TYPE_TIMEVALUE, 0, IDS_TIME_END,
        p_end,
    FinjinNodeAnimationSettingsObject::PB_TIME_SCALE_TYPE, _M("TimeScaleType"), TYPE_STRING, 0, IDS_TIME_SCALE_TYPE,
        p_end,
    FinjinNodeAnimationSettingsObject::PB_TIME_SCALE, _M("TimeScale"), TYPE_FLOAT, 0, IDS_TIME_SCALE,
        p_default, 1.0f,
        p_end,
    FinjinNodeAnimationSettingsObject::PB_COPY_FIRST_KEY_TO_LAST, _M("CopyFirstKeyToLast"), TYPE_BOOL, 0, IDS_COPY_FIRST_KEY_TO_LAST,
        p_end,
    FinjinNodeAnimationSettingsObject::PB_USE_ANIMATION_START_TIME, _M("UseAnimationStartTime"), TYPE_BOOL, 0, IDS_USE_ANIMATION_START_TIME,
        p_end,
    FinjinNodeAnimationSettingsObject::PB_LINK_TO_MAIN_OBJECT, _M("LinkToMainObject"), TYPE_BOOL, 0, IDS_LINK_TO_MAIN_OBJECT,
        p_end,
    FinjinNodeAnimationSettingsObject::PB_EMBED_ANIMATION, _M("EmbedAnimation"), TYPE_BOOL, 0, IDS_EMBED_ANIMATION,
        p_end,
    FinjinNodeAnimationSettingsObject::PB_ENABLED, _M("Enabled"), TYPE_BOOL, 0, IDS_ENABLED,
        p_end,
    FinjinNodeAnimationSettingsObject::PB_LOOPED, _M("Looped"), TYPE_BOOL, 0, IDS_LOOPED,
        p_end,
    FinjinNodeAnimationSettingsObject::PB_SAMPLE_INTERVAL, _M("SampleInterval"), TYPE_FLOAT, 0, IDS_SAMPLE_INTERVAL,
        p_end,
    FinjinNodeAnimationSettingsObject::PB_SAMPLE_TYPE, _M("SampleType"), TYPE_INT, 0, IDS_SAMPLE_TYPE,
        p_default, (int)SampleType::USE_PARENT,
        p_end,
    FinjinNodeAnimationSettingsObject::PB_TRANSLATION_INTERPOLATION_TYPE, _M("TranslationInterpolationType"), TYPE_STRING, 0, IDS_TRANSLATION_INTERPOLATION_TYPE,
        p_end,
    FinjinNodeAnimationSettingsObject::PB_ROTATION_INTERPOLATION_TYPE, _M("RotationInterpolationType"), TYPE_STRING, 0, IDS_ROTATION_INTERPOLATION_TYPE,
        p_end,
    p_end
    );


//Implementation----------------------------------------------------------------
FinjinNodeAnimationSettingsObject::FinjinNodeAnimationSettingsObject()
{
    this->pblock = nullptr;
    FinjinNodeAnimationSettingsObjectClassDesc::instance.MakeAutoParamBlocks(this);
}

FinjinNodeAnimationSettingsObject::~FinjinNodeAnimationSettingsObject()
{
}

CreateMouseCallBack* FinjinNodeAnimationSettingsObject::GetCreateMouseCallBack()
{
    return nullptr;
}

ObjectState FinjinNodeAnimationSettingsObject::Eval(TimeValue time)
{
    return ObjectState(this);
}

void FinjinNodeAnimationSettingsObject::InitNodeName(MSTR& s)
{
    s = _M("FinjinNodeAnimationSettings");
}

Class_ID FinjinNodeAnimationSettingsObject::ClassID()
{
    return GetClassClassID();
}

Class_ID FinjinNodeAnimationSettingsObject::GetClassClassID()
{
    return FinjinMaxClassID::NodeAnimationSettings;
}

SClass_ID FinjinNodeAnimationSettingsObject::SuperClassID()
{
    return REF_TARGET_CLASS_ID;
}

void FinjinNodeAnimationSettingsObject::GetClassName(MSTR& s)
{
    s = MaxUtilities::GetStringM(IDS_NODE_ANIMATION_SETTINGS_OBJECT_CLASS_NAME);
}

RefTargetHandle FinjinNodeAnimationSettingsObject::Clone(RemapDir& remap)
{
    FinjinNodeAnimationSettingsObject* newHelper = new FinjinNodeAnimationSettingsObject();

    //Copy everything
    newHelper->ReplaceReference(PARAM_BLOCK_REF, remap.CloneRef(this->pblock));

    BaseClone(this, newHelper, remap);

    return newHelper;
}

RefResult FinjinNodeAnimationSettingsObject::NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate)
{
    return REF_SUCCEED;
}

int FinjinNodeAnimationSettingsObject::NumSubs()
{
    return NUM_REFS;
}

MSTR FinjinNodeAnimationSettingsObject::SubAnimName(int i)
{
    return MaxUtilities::GetString(IDS_PARAMETERS);
}

Animatable* FinjinNodeAnimationSettingsObject::SubAnim(int i)
{
    return this->pblock;
}

int FinjinNodeAnimationSettingsObject::NumRefs()
{
    return NUM_REFS;
}

RefTargetHandle FinjinNodeAnimationSettingsObject::GetReference(int i)
{
    return this->pblock;
}

void FinjinNodeAnimationSettingsObject::SetReference(int i, RefTargetHandle rtarg)
{
    this->pblock = (IParamBlock2*)rtarg;
}

ClassDesc* FinjinNodeAnimationSettingsObject::GetClassDesc()
{
    return &FinjinNodeAnimationSettingsObjectClassDesc::instance;
}

wxString FinjinNodeAnimationSettingsObject::GetAnimationName()
{
    return this->pblock->GetStr(PB_ANIMATION_NAME);
}

wxString FinjinNodeAnimationSettingsObject::GetTimeScaleType()
{
    return this->pblock->GetStr(PB_TIME_SCALE_TYPE);
}

double FinjinNodeAnimationSettingsObject::GetTimeScale()
{
    return this->pblock->GetFloat(PB_TIME_SCALE);
}

bool FinjinNodeAnimationSettingsObject::GetCopyFirstKeyToLast()
{
    return this->pblock->GetInt(PB_COPY_FIRST_KEY_TO_LAST) ? true : false;
}

bool FinjinNodeAnimationSettingsObject::GetUseAnimationStartTime()
{
    return this->pblock->GetInt(PB_USE_ANIMATION_START_TIME) ? true : false;
}

bool FinjinNodeAnimationSettingsObject::GetEmbedAnimation()
{
    return this->pblock->GetInt(PB_EMBED_ANIMATION) ? true : false;
}

bool FinjinNodeAnimationSettingsObject::GetLinkToMainObject()
{
    return this->pblock->GetInt(PB_LINK_TO_MAIN_OBJECT) ? true : false;
}

bool FinjinNodeAnimationSettingsObject::GetEnabled()
{
    return this->pblock->GetInt(PB_ENABLED) ? true : false;
}

bool FinjinNodeAnimationSettingsObject::GetLooped()
{
    return this->pblock->GetInt(PB_LOOPED) ? true : false;
}

double FinjinNodeAnimationSettingsObject::GetSampleInterval()
{
    return this->pblock->GetFloat(PB_SAMPLE_INTERVAL);
}

SampleType FinjinNodeAnimationSettingsObject::GetSampleType()
{
    return (SampleType)this->pblock->GetInt(PB_SAMPLE_TYPE);
}

wxString FinjinNodeAnimationSettingsObject::GetTranslationInterpolationType()
{
    return this->pblock->GetStr(PB_TRANSLATION_INTERPOLATION_TYPE);
}

wxString FinjinNodeAnimationSettingsObject::GetRotationInterpolationType()
{
    return this->pblock->GetStr(PB_ROTATION_INTERPOLATION_TYPE);
}

Interval FinjinNodeAnimationSettingsObject::GetTimeInterval()
{
    return Interval(this->pblock->GetTimeValue(PB_TIME_START), this->pblock->GetTimeValue(PB_TIME_END));
}

void FinjinNodeAnimationSettingsObject::SetAnimationName(wxString value)
{
    this->pblock->SetValue(PB_ANIMATION_NAME, 0, WxStringToApplicationStringM(value));
}

void FinjinNodeAnimationSettingsObject::SetTimeScaleType(wxString value)
{
    this->pblock->SetValue(PB_TIME_SCALE_TYPE, 0, WxStringToApplicationStringM(value));
}

void FinjinNodeAnimationSettingsObject::SetTimeScale(double value)
{
    this->pblock->SetValue(PB_TIME_SCALE, 0, (float)value);
}

void FinjinNodeAnimationSettingsObject::SetCopyFirstKeyToLast(bool value)
{
    this->pblock->SetValue(PB_COPY_FIRST_KEY_TO_LAST, 0, value);
}

void FinjinNodeAnimationSettingsObject::SetUseAnimationStartTime(bool value)
{
    this->pblock->SetValue(PB_USE_ANIMATION_START_TIME, 0, value);
}

void FinjinNodeAnimationSettingsObject::SetEmbedAnimation(bool value)
{
    this->pblock->SetValue(PB_EMBED_ANIMATION, 0, (int)value);
}

void FinjinNodeAnimationSettingsObject::SetLinkToMainObject(bool value)
{
    this->pblock->SetValue(PB_LINK_TO_MAIN_OBJECT, 0, (int)value);
}

void FinjinNodeAnimationSettingsObject::SetEnabled(bool value)
{
    this->pblock->SetValue(PB_ENABLED, 0, value);
}

void FinjinNodeAnimationSettingsObject::SetLooped(bool value)
{
    this->pblock->SetValue(PB_LOOPED, 0, value);
}

void FinjinNodeAnimationSettingsObject::SetSampleInterval(double value)
{
    this->pblock->SetValue(PB_SAMPLE_INTERVAL, 0, (float)value);
}

void FinjinNodeAnimationSettingsObject::SetSampleType(SampleType value)
{
    this->pblock->SetValue(PB_SAMPLE_TYPE, 0, (int)value);
}

void FinjinNodeAnimationSettingsObject::SetTimeInterval(Interval interval)
{
    this->pblock->SetValue(PB_TIME_START, 0, interval.Start());
    this->pblock->SetValue(PB_TIME_END, 0, interval.End());
}

void FinjinNodeAnimationSettingsObject::SetTranslationInterpolationType(wxString value)
{
    this->pblock->SetValue(PB_TRANSLATION_INTERPOLATION_TYPE, 0, WxStringToApplicationStringM(value));
}

void FinjinNodeAnimationSettingsObject::SetRotationInterpolationType(wxString value)
{
    this->pblock->SetValue(PB_ROTATION_INTERPOLATION_TYPE, 0, WxStringToApplicationStringM(value));
}
