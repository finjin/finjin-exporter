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
#include "FinjinMeshAnimationSettingsObject.hpp"
#include "FinjinSceneSettingsObject.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "MaxUtilities.hpp"
#include "FinjinMaxClassID.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Local classes----------------------------------------------------------------
class FinjinMeshAnimationSettingsObjectClassDesc : public ClassDesc2
{
public:
    int IsPublic() override                      {return FALSE;}
    void* Create(BOOL loading = FALSE) override  {return new FinjinMeshAnimationSettingsObject();}
    const MCHAR* ClassName() override            {return MaxUtilities::GetStringM(IDS_MESH_ANIMATION_SETTINGS_OBJECT_CLASS_NAME);}
    SClass_ID SuperClassID() override            {return REF_TARGET_CLASS_ID;}
    Class_ID ClassID() override                  {return FinjinMeshAnimationSettingsObject::GetClassClassID();}
    const MCHAR* Category() override             {return MaxUtilities::GetStringM(IDS_PLUGINS_CATEGORY);}
    const MCHAR* InternalName() override         {return _M("FinjinMeshAnimationSettingsObject");}
    HINSTANCE HInstance() override               {return wxGetInstance();}

public:
    static FinjinMeshAnimationSettingsObjectClassDesc instance;
};
FinjinMeshAnimationSettingsObjectClassDesc FinjinMeshAnimationSettingsObjectClassDesc::instance;


//Globals----------------------------------------------------------------------
enum {OBJECT_SETTINGS_PARAM_BLOCK_ID = 0};
enum {OBJECT_SETTINGS_VERSION = 1};

static ParamBlockDesc2 FinjinMeshAnimationSettingsObjectParamBlock
    (
    OBJECT_SETTINGS_PARAM_BLOCK_ID, _M("Parameters"),  0, &FinjinMeshAnimationSettingsObjectClassDesc::instance, P_VERSION | P_AUTO_CONSTRUCT, 
    
    //Version
    OBJECT_SETTINGS_VERSION,
    
    //Reference number
    FinjinMeshAnimationSettingsObject::PARAM_BLOCK_REF, 
    
    //Parameters    
    FinjinMeshAnimationSettingsObject::PB_TRACK, _M("TrackName"), TYPE_STRING, P_COMPUTED_NAME, IDS_TRACK_NAME,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_ANIMATION_NAME, _M("AnimationName"), TYPE_STRING, 0, IDS_ANIMATION_NAME,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_TIME_START, _M("TimeStart"), TYPE_TIMEVALUE, 0, IDS_TIME_START,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_TIME_END, _M("TimeEnd"), TYPE_TIMEVALUE, 0, IDS_TIME_END,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_TIME_SCALE_TYPE, _M("TimeScaleType"), TYPE_STRING, 0, IDS_TIME_SCALE_TYPE,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_TIME_SCALE, _M("TimeScale"), TYPE_FLOAT, 0, IDS_TIME_SCALE,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_COPY_FIRST_KEY_TO_LAST, _M("CopyFirstKeyToLast"), TYPE_BOOL, 0, IDS_COPY_FIRST_KEY_TO_LAST,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_EXPORT_AS_MORPH, _M("ExportAsMorph"), TYPE_BOOL, 0, IDS_EXPORT_AS_MOPRH,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_MORPH_WHOLE_OBJECT, _M("MorphWholeObject"), TYPE_INT, 0, IDS_MORPH_WHOLE_OBJECT,
        p_default, (int)INHERITED_USE_PARENT_BOOL,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_OVERRIDE_ANIMATED_ROOT_START_TIME, _M("OverrideAnimatedRootStartTime"), TYPE_BOOL, 0, IDS_OVERRIDE_ANIMATED_ROOT_START_TIME,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_ANIMATED_ROOT_START_TIME, _M("AnimatedRootStartTime"), TYPE_TIMEVALUE, 0, IDS_ANIMATED_ROOT_START_TIME,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_USE_ANIMATION_START_TIME, _M("UseAnimationStartTime"), TYPE_BOOL, 0, IDS_USE_ANIMATION_START_TIME,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_LOOPED, _M("Looped"), TYPE_BOOL, 0, IDS_LOOPED,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_SAMPLE_INTERVAL, _M("SampleInterval"), TYPE_FLOAT, 0, IDS_SAMPLE_INTERVAL,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_SAMPLE_TYPE, _M("SampleType"), TYPE_INT, 0, IDS_SAMPLE_TYPE,
        p_default, (int)SampleType::USE_PARENT,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_LINK_TO_MAIN_OBJECT, _M("LinkToMainObject"), TYPE_BOOL, 0, IDS_LINK_TO_MAIN_OBJECT,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_EMBED_ANIMATION, _M("EmbedAnimation"), TYPE_BOOL, 0, IDS_EMBED_ANIMATION,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_BONE_TRANSLATION_MASK_NODES, _M("BoneTranslationMaskNodes"), TYPE_INODE_TAB, 0, P_VARIABLE_SIZE, IDS_BONE_TRANSLATION_MASK_NODES,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_BONE_TRANSLATION_MASKS, _M("BoneTranslationMasks"), TYPE_POINT3_TAB, 0, P_VARIABLE_SIZE, IDS_BONE_TRANSLATION_MASKS,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_ALLOW_EXPORT_BONE_NODES, _M("AllowExportBones"), TYPE_INODE_TAB, 0, P_VARIABLE_SIZE, IDS_ALLOW_EXPORT_BONES,
        p_end,
    FinjinMeshAnimationSettingsObject::PB_ALLOW_EXPORT_BONE_ALLOWS, _M("AllowExportBoneAllows"), TYPE_BOOL_TAB, 0, P_VARIABLE_SIZE, IDS_ALLOW_EXPORT_BONE_ALLOWS,
        p_end,    
    FinjinMeshAnimationSettingsObject::PB_ALLOW_EXPORT_BONE_RECURSIVES, _M("AllowExportBoneRecursives"), TYPE_BOOL_TAB, 0, P_VARIABLE_SIZE, IDS_ALLOW_EXPORT_BONE_RECURSIVES,
        p_end,
    p_end
    );


//Implementation---------------------------------------------------------------
FinjinMeshAnimationSettingsObject::FinjinMeshAnimationSettingsObject()
{
    this->pblock = nullptr;
    FinjinMeshAnimationSettingsObjectClassDesc::instance.MakeAutoParamBlocks(this);    
}

FinjinMeshAnimationSettingsObject::~FinjinMeshAnimationSettingsObject()
{   
}

CreateMouseCallBack* FinjinMeshAnimationSettingsObject::GetCreateMouseCallBack()
{
    return nullptr;
}

ObjectState FinjinMeshAnimationSettingsObject::Eval(TimeValue time)
{
    return ObjectState(this);
}

void FinjinMeshAnimationSettingsObject::InitNodeName(MSTR& s)
{
    s = _M("FinjinMeshAnimationSettings");
}

Class_ID FinjinMeshAnimationSettingsObject::ClassID() 
{
    return GetClassClassID();
}        

Class_ID FinjinMeshAnimationSettingsObject::GetClassClassID()
{
    return FinjinMaxClassID::MeshAnimationSettings;
}

SClass_ID FinjinMeshAnimationSettingsObject::SuperClassID() 
{ 
    return REF_TARGET_CLASS_ID;
}

void FinjinMeshAnimationSettingsObject::GetClassName(MSTR& s)
{
    s = MaxUtilities::GetStringM(IDS_MESH_ANIMATION_SETTINGS_OBJECT_CLASS_NAME);
}

RefTargetHandle FinjinMeshAnimationSettingsObject::Clone(RemapDir& remap)
{
    FinjinMeshAnimationSettingsObject* newHelper = new FinjinMeshAnimationSettingsObject();

    //Copy everything
    newHelper->ReplaceReference(PARAM_BLOCK_REF, remap.CloneRef(this->pblock));
        
    BaseClone(this, newHelper, remap);
    
    return newHelper;
}

RefResult FinjinMeshAnimationSettingsObject::NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate)
{
    int tabIndex = -1;
    int lastNotifyID = 0;
    if (this->pblock != nullptr)
        lastNotifyID = this->pblock->LastNotifyParamID(tabIndex);

    switch (message) 
    {
        case REFMSG_TARGET_DELETED:
        {   
            if (lastNotifyID == PB_BONE_TRANSLATION_MASK_NODES)
            {
                this->pblock->Delete(PB_BONE_TRANSLATION_MASK_NODES, tabIndex, 1);
                this->pblock->Delete(PB_BONE_TRANSLATION_MASKS, tabIndex, 1);
            }
            else if (lastNotifyID == PB_ALLOW_EXPORT_BONE_NODES)
            {
                this->pblock->Delete(PB_ALLOW_EXPORT_BONE_NODES, tabIndex, 1);
                this->pblock->Delete(PB_ALLOW_EXPORT_BONE_ALLOWS, tabIndex, 1);
                this->pblock->Delete(PB_ALLOW_EXPORT_BONE_RECURSIVES, tabIndex, 1);
            }

            break;
        }
    }

    return REF_SUCCEED;
}

int FinjinMeshAnimationSettingsObject::NumSubs() 
{ 
    return NUM_REFS; 
}

MSTR FinjinMeshAnimationSettingsObject::SubAnimName(int i) 
{
    return MaxUtilities::GetString(IDS_PARAMETERS); 
}                

Animatable* FinjinMeshAnimationSettingsObject::SubAnim(int i) 
{
    return this->pblock; 
}

int FinjinMeshAnimationSettingsObject::NumRefs() 
{ 
    return NUM_REFS; 
}

RefTargetHandle FinjinMeshAnimationSettingsObject::GetReference(int i) 
{
    return this->pblock; 
}

void FinjinMeshAnimationSettingsObject::SetReference(int i, RefTargetHandle rtarg) 
{ 
    this->pblock = (IParamBlock2*)rtarg; 
}

ClassDesc* FinjinMeshAnimationSettingsObject::GetClassDesc()
{
    return &FinjinMeshAnimationSettingsObjectClassDesc::instance;
}

wxString FinjinMeshAnimationSettingsObject::GetTrack()
{
    wxString track = this->pblock->GetStr(PB_TRACK);
    if (track == wxT("object"))
        track.clear();
    return track;
}

wxString FinjinMeshAnimationSettingsObject::GetAnimationName()
{
    return this->pblock->GetStr(PB_ANIMATION_NAME);
}

wxString FinjinMeshAnimationSettingsObject::GetTimeScaleType()
{
    return this->pblock->GetStr(PB_TIME_SCALE_TYPE);
}

double FinjinMeshAnimationSettingsObject::GetTimeScale()
{
    return this->pblock->GetFloat(PB_TIME_SCALE);
}

bool FinjinMeshAnimationSettingsObject::GetCopyFirstKeyToLast()
{
    return this->pblock->GetInt(PB_COPY_FIRST_KEY_TO_LAST) ? true : false;
}

bool FinjinMeshAnimationSettingsObject::GetExportAsMorph()
{
    return this->pblock->GetInt(PB_EXPORT_AS_MORPH) ? true : false;
}

bool FinjinMeshAnimationSettingsObject::GetUseAnimationStartTime()
{
    return this->pblock->GetInt(PB_USE_ANIMATION_START_TIME) ? true : false;
}

bool FinjinMeshAnimationSettingsObject::GetLooped()
{
    return this->pblock->GetInt(PB_LOOPED) ? true : false;
}

double FinjinMeshAnimationSettingsObject::GetSampleInterval()
{
    return this->pblock->GetFloat(PB_SAMPLE_INTERVAL);
}

SampleType FinjinMeshAnimationSettingsObject::GetSampleType()
{
    return (SampleType)this->pblock->GetInt(PB_SAMPLE_TYPE);
}

Interval FinjinMeshAnimationSettingsObject::GetTimeInterval()
{
    return Interval(this->pblock->GetTimeValue(PB_TIME_START), this->pblock->GetTimeValue(PB_TIME_END));        
}

int FinjinMeshAnimationSettingsObject::GetBoneTranslationMaskCount()
{
    return this->pblock->Count(PB_BONE_TRANSLATION_MASK_NODES);
}

void FinjinMeshAnimationSettingsObject::GetBoneTranslationMask(int index, INode*& node, Point3& mask)
{
    node = this->pblock->GetINode(PB_BONE_TRANSLATION_MASK_NODES, 0, index);
    mask = this->pblock->GetPoint3(PB_BONE_TRANSLATION_MASKS, 0, index);
}

Point3 FinjinMeshAnimationSettingsObject::GetBoneTranslationMask(INode* node)
{
    Point3 mask(1,1,1);

    int count = GetBoneTranslationMaskCount();
    for (int i = 0; i < count; i++)
    {
        if (this->pblock->GetINode(PB_BONE_TRANSLATION_MASK_NODES, 0, i) == node)
        {
            mask = this->pblock->GetPoint3(PB_BONE_TRANSLATION_MASKS, 0, i);
            break;
        }
    }

    return mask;
}

void FinjinMeshAnimationSettingsObject::ClearBoneTranslationMasks()
{
    this->pblock->ZeroCount(PB_BONE_TRANSLATION_MASK_NODES);
    this->pblock->ZeroCount(PB_BONE_TRANSLATION_MASKS);
}

int FinjinMeshAnimationSettingsObject::GetAllowExportBoneCount()
{
    return this->pblock->Count(PB_ALLOW_EXPORT_BONE_NODES);
}

void FinjinMeshAnimationSettingsObject::GetAllowExportBone(int index, INode*& node, bool& allow, bool& recursive)
{
    node = this->pblock->GetINode(PB_ALLOW_EXPORT_BONE_NODES, 0, index);
    allow = this->pblock->GetInt(PB_ALLOW_EXPORT_BONE_ALLOWS, 0, index) ? true : false;
    recursive = this->pblock->GetInt(PB_ALLOW_EXPORT_BONE_RECURSIVES, 0, index) ? true : false;
}

void FinjinMeshAnimationSettingsObject::ClearAllowExportBones()
{
    this->pblock->ZeroCount(PB_ALLOW_EXPORT_BONE_NODES);
    this->pblock->ZeroCount(PB_ALLOW_EXPORT_BONE_ALLOWS);
    this->pblock->ZeroCount(PB_ALLOW_EXPORT_BONE_RECURSIVES);
}

InheritedBool FinjinMeshAnimationSettingsObject::GetMorphWholeObject()
{
    return (InheritedBool)this->pblock->GetInt(PB_MORPH_WHOLE_OBJECT);
}

bool FinjinMeshAnimationSettingsObject::GetOverrideAnimatedRootStartTime()
{
    return this->pblock->GetInt(PB_OVERRIDE_ANIMATED_ROOT_START_TIME) ? true : false;
}

TimeValue FinjinMeshAnimationSettingsObject::GetAnimatedRootStartTime()
{
    return this->pblock->GetTimeValue(PB_ANIMATED_ROOT_START_TIME);
}

bool FinjinMeshAnimationSettingsObject::GetEmbedAnimation()
{
    return this->pblock->GetInt(PB_EMBED_ANIMATION) ? true : false;
}

bool FinjinMeshAnimationSettingsObject::GetLinkToMainObject()
{
    return this->pblock->GetInt(PB_LINK_TO_MAIN_OBJECT) ? true : false;
}

void FinjinMeshAnimationSettingsObject::SetTrack(wxString value)
{
    this->pblock->SetValue(PB_TRACK, 0, WxStringToApplicationStringM(value));
}

void FinjinMeshAnimationSettingsObject::SetAnimationName(wxString value)
{
    this->pblock->SetValue(PB_ANIMATION_NAME, 0, WxStringToApplicationStringM(value));
}

void FinjinMeshAnimationSettingsObject::SetTimeScaleType(wxString value)
{
    this->pblock->SetValue(PB_TIME_SCALE_TYPE, 0, WxStringToApplicationStringM(value));
}

void FinjinMeshAnimationSettingsObject::SetTimeScale(double value)
{
    this->pblock->SetValue(PB_TIME_SCALE, 0, (float)value);
}

void FinjinMeshAnimationSettingsObject::SetCopyFirstKeyToLast(bool value)
{
    this->pblock->SetValue(PB_COPY_FIRST_KEY_TO_LAST, 0, value);
}

void FinjinMeshAnimationSettingsObject::SetExportAsMorph(bool value)
{
    this->pblock->SetValue(PB_EXPORT_AS_MORPH, 0, value);
}

void FinjinMeshAnimationSettingsObject::SetUseAnimationStartTime(bool value)
{
    this->pblock->SetValue(PB_USE_ANIMATION_START_TIME, 0, value);
}

void FinjinMeshAnimationSettingsObject::SetLooped(bool value)
{
    this->pblock->SetValue(PB_LOOPED, 0, value);
}

void FinjinMeshAnimationSettingsObject::SetSampleInterval(double value)
{
    this->pblock->SetValue(PB_SAMPLE_INTERVAL, 0, (float)value);
}

void FinjinMeshAnimationSettingsObject::SetSampleType(SampleType value)
{
    this->pblock->SetValue(PB_SAMPLE_TYPE, 0, (int)value);
}

void FinjinMeshAnimationSettingsObject::SetTimeInterval(Interval interval)
{
    this->pblock->SetValue(PB_TIME_START, 0, interval.Start());
    this->pblock->SetValue(PB_TIME_END, 0, interval.End());
}

void FinjinMeshAnimationSettingsObject::AddBoneTranslationMask(INode* node, Point3 mask)
{
    INode* nodes[1] = {node};
    this->pblock->Append(PB_BONE_TRANSLATION_MASK_NODES, 1, nodes);

    Point3* masks[1] = {&mask};
    this->pblock->Append(PB_BONE_TRANSLATION_MASKS, 1, masks);
}

void FinjinMeshAnimationSettingsObject::AddAllowExportBone(INode* node, bool allow, bool recursive)
{
    INode* nodes[1] = {node};
    this->pblock->Append(PB_ALLOW_EXPORT_BONE_NODES, 1, nodes);

    BOOL allows[1] = {(int)allow};
    this->pblock->Append(PB_ALLOW_EXPORT_BONE_ALLOWS, 1, allows);

    BOOL recursives[1] = {(int)recursive};
    this->pblock->Append(PB_ALLOW_EXPORT_BONE_RECURSIVES, 1, recursives);
}

void FinjinMeshAnimationSettingsObject::SetMorphWholeObject(InheritedBool value)
{
    this->pblock->SetValue(PB_MORPH_WHOLE_OBJECT, 0, (int)value);
}

void FinjinMeshAnimationSettingsObject::SetOverrideAnimatedRootStartTime(bool value)
{
    this->pblock->SetValue(PB_OVERRIDE_ANIMATED_ROOT_START_TIME, 0, (int)value);
}

void FinjinMeshAnimationSettingsObject::SetAnimatedRootStartTime(TimeValue value)
{
    this->pblock->SetValue(PB_ANIMATED_ROOT_START_TIME, 0, value);
}

void FinjinMeshAnimationSettingsObject::SetEmbedAnimation(bool value)
{
    this->pblock->SetValue(PB_EMBED_ANIMATION, 0, (int)value);
}

void FinjinMeshAnimationSettingsObject::SetLinkToMainObject(bool value)
{
    this->pblock->SetValue(PB_LINK_TO_MAIN_OBJECT, 0, (int)value);
}
