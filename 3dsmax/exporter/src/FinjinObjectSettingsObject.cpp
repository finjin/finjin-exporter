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
#include "FinjinObjectSettingsObject.hpp"
#include "FinjinSceneSettingsObject.hpp"
#include "FinjinNodeAnimationSettingsObject.hpp"
#include "FinjinMeshAnimationSettingsObject.hpp"
#include "FinjinSubmeshSettingsObject.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "MaxUtilities.hpp"
#include "TextureCoordinateSetMappings.hpp"
#include "FinjinMaxClassID.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Local classes----------------------------------------------------------------
class FinjinObjectSettingsObjectClassDesc : public ClassDesc2
{
public:
    int IsPublic() override                      {return FALSE;}
    void* Create(BOOL loading = FALSE) override  {return new FinjinObjectSettingsObject();}
    const MCHAR* ClassName() override            {return MaxUtilities::GetStringM(IDS_OBJECT_SETTINGS_OBJECT_CLASS_NAME);}
    SClass_ID SuperClassID() override            {return REF_TARGET_CLASS_ID;}
    Class_ID ClassID() override                  {return FinjinObjectSettingsObject::GetClassClassID();}
    const MCHAR* Category() override             {return MaxUtilities::GetStringM(IDS_PLUGINS_CATEGORY);}
    const MCHAR* InternalName() override         {return _M("FinjinObjectSettingsObject");}
    HINSTANCE HInstance() override               {return wxGetInstance();}

public:
    static FinjinObjectSettingsObjectClassDesc instance;
};
FinjinObjectSettingsObjectClassDesc FinjinObjectSettingsObjectClassDesc::instance;


//Globals----------------------------------------------------------------------
enum {OBJECT_SETTINGS_PARAM_BLOCK_ID = 0};
enum {OBJECT_SETTINGS_VERSION = 1};

static ParamBlockDesc2 FinjinObjectSettingsObjectParamBlock
    (
    OBJECT_SETTINGS_PARAM_BLOCK_ID, _M("Parameters"),  0, &FinjinObjectSettingsObjectClassDesc::instance, P_VERSION | P_AUTO_CONSTRUCT, 
    
    //Version
    OBJECT_SETTINGS_VERSION,
    
    //Reference number
    FinjinObjectSettingsObject::PARAM_BLOCK_REF, 
    
    //Parameters

    USER_DATA_PARAMETER_BLOCK_PARAMETERS(FinjinObjectSettingsObject),

    EXTENDED_VALUE_PARAMETER_BLOCK_PARAMETERS(FinjinObjectSettingsObject),

    FinjinObjectSettingsObject::PB_TYPE, _M("Type"), TYPE_STRING, 0, IDS_TYPE,
        p_end,
    FinjinObjectSettingsObject::PB_VISIBILITY, _M("Visibility"), TYPE_STRING, 0, IDS_VISIBILITY,
        p_end,
    FinjinObjectSettingsObject::PB_VISIBILITY_AFFECT_OBJECT_ONLY, _M("VisibilityAffectObjectOnly"), TYPE_BOOL, 0, IDS_VISIBILITY_AFFECT_OBJECT_ONLY,
        p_end,
    FinjinObjectSettingsObject::PB_TARGET_TYPE, _M("TargetType"), TYPE_STRING, 0, IDS_TARGET_TYPE,
        p_end,
    FinjinObjectSettingsObject::PB_IS_PREFAB, _M("IsPrefab"), TYPE_BOOL, 0, IDS_IS_PREFAB,
        p_end,
    FinjinObjectSettingsObject::PB_IS_PREFAB_INSTANCE, _M("IsPrefabInstance"), TYPE_BOOL, 0, IDS_IS_PREFAB_INSTANCE,
        p_end,
    FinjinObjectSettingsObject::PB_PREFAB_NAME, _M("PrefabName"), TYPE_STRING, 0, IDS_PREFAB_NAME,
        p_end,
    FinjinObjectSettingsObject::PB_NODE_ANIMATION_SAMPLE_INTERVAL, _M("NodeAnimationSampleInterval"), TYPE_FLOAT, 0, IDS_NODE_ANIMATION_SAMPLE_INTERVAL,
        p_end,
    FinjinObjectSettingsObject::PB_NODE_ANIMATION_SAMPLE_TYPE, _M("NodeAnimationSampleType"), TYPE_INT, 0, IDS_NODE_ANIMATION_SAMPLE_TYPE,
        p_end,
    FinjinObjectSettingsObject::PB_NOTE_TRACK_NAMES, _M("NoteTrackNames"), TYPE_STRING_TAB, 0, P_VARIABLE_SIZE, IDS_NOTE_TRACK_NAMES,
        p_end,
    FinjinObjectSettingsObject::PB_RENDER_QUEUE_NAME, _M("RenderQueueName"), TYPE_STRING, 0, IDS_RENDER_QUEUE_NAME,
        p_end,
    FinjinObjectSettingsObject::PB_RENDER_QUEUE_PRIORITY, _M("RenderQueuePriority"), TYPE_INT, 0, IDS_RENDER_QUEUE_PRIORITY,
        p_end,
    FinjinObjectSettingsObject::PB_EXPORT_FLAGS, _M("ExportFlags"), TYPE_BOOL, 0, IDS_EXPORT_FLAGS,
        p_default, TRUE,
        p_end,        
    FinjinObjectSettingsObject::PB_FLAG_NAMES, _M("FlagNames"), TYPE_STRING_TAB, 0, P_VARIABLE_SIZE, IDS_FLAG_NAMES,
        p_end,
    
    FinjinObjectSettingsObject::PB_EXPORT_MESH, _M("ExportMesh"), TYPE_BOOL, 0, IDS_EXPORT_MESH,
        p_default, TRUE,
        p_end,
    FinjinObjectSettingsObject::PB_MESH_NAME, _M("MeshName"), TYPE_STRING, 0, IDS_MESH_NAME,
        p_end,
    FinjinObjectSettingsObject::PB_ANIMATE_BOUNDING_VOLUMES, _M("AnimateBoundingVolumes"), TYPE_BOOL, 0, IDS_ANIMATE_BOUNDING_VOLUMES,
        p_end,
    FinjinObjectSettingsObject::PB_TEXTURE_COORDINATE_TYPE, _M("TextureCoordinateType"), TYPE_STRING, 0, IDS_TEXTURE_COORDINATE_TYPE,
        p_end,        
    
    FinjinObjectSettingsObject::PB_SKELETON_NAME, _M("SkeletonName"), TYPE_STRING, 0, IDS_SKELETON_NAME,
        p_end,
    FinjinObjectSettingsObject::PB_MESH_ANIMATION_TYPE, _M("MeshAnimationType"), TYPE_STRING, 0, IDS_MESH_ANIMATION_TYPE,
        p_end,
    FinjinObjectSettingsObject::PB_SKELETON_ANIMATION_SAMPLE_INTERVAL, _M("SkeletonAnimationSampleInterval"), TYPE_FLOAT, 0, IDS_SKELETON_ANIMATION_SAMPLE_INTERVAL,
        p_end,
    FinjinObjectSettingsObject::PB_SKELETON_ANIMATION_SAMPLE_TYPE, _M("SkeletonAnimationSampleType"), TYPE_INT, 0, IDS_SKELETON_ANIMATION_SAMPLE_TYPE,
        p_end,
    FinjinObjectSettingsObject::PB_VERTEX_ANIMATION_SAMPLE_INTERVAL, _M("VertexAnimationSampleInterval"), TYPE_FLOAT, 0, IDS_VERTEX_ANIMATION_SAMPLE_INTERVAL,
        p_end,
    FinjinObjectSettingsObject::PB_VERTEX_ANIMATION_SAMPLE_TYPE, _M("VertexAnimationSampleType"), TYPE_INT, 0, IDS_VERTEX_ANIMATION_SAMPLE_TYPE,
        p_end,
    FinjinObjectSettingsObject::PB_ANIMATED_ROOT, _M("AnimatedRoot"), TYPE_INODE, 0, IDS_ANIMATED_ROOT,
        p_end,
    
    FinjinObjectSettingsObject::PB_PRIMITIVE_TYPE, _M("PrimitiveType"), TYPE_STRING, 0, IDS_PRIMITIVE_TYPE,
        p_end,
    FinjinObjectSettingsObject::PB_EXPORT_VERTEX_COLORS, _M("ExportVertexColors"), TYPE_BOOL, 0, IDS_EXPORT_VERTEX_COLORS,
        p_end,
    FinjinObjectSettingsObject::PB_EXPORT_TANGENTS, _M("ExportTangents"), TYPE_BOOL, 0, IDS_EXPORT_TANGENTS,
        p_end,
    FinjinObjectSettingsObject::PB_EXPORT_BINORMALS, _M("ExportBinormals"), TYPE_BOOL, 0, IDS_EXPORT_BINORMALS,
        p_end,
    FinjinObjectSettingsObject::PB_CHECK_MESH_INSTANCE, _M("CheckMeshInstance"), TYPE_BOOL, 0, IDS_CHECK_MESH_INSTANCE,
        p_end,
    FinjinObjectSettingsObject::PB_USE_CUSTOM_SUBMESHES, _M("UseCustomSubmeshes"), TYPE_BOOL, 0, IDS_USE_CUSTOM_SUBMESHES,    
        p_end,
    FinjinObjectSettingsObject::PB_SUBMESHES_SETTINGS, _M("SubmeshSettings"), TYPE_REFTARG_TAB, 0, P_VARIABLE_SIZE, IDS_SUBMESHES_SETTINGS,
        p_end,    

    FinjinObjectSettingsObject::PB_ITEM_TYPE, _M("ItemType"), TYPE_STRING, 0, IDS_ITEM_TYPE,
        p_end,
    
    FinjinObjectSettingsObject::PB_ENABLE_SKY, _M("EnableSky"), TYPE_BOOL, 0, IDS_ENABLE_SKY,
        p_default, TRUE,
        p_end,
    
    FinjinObjectSettingsObject::PB_POINT_SIZE, _M("PointSize"), TYPE_FLOAT, 0, IDS_POINT_SIZE,
        p_default, 10.0f,
        p_end,
    
    FinjinObjectSettingsObject::PB_IGNORE_CHILDREN, _M("IgnoreChildren"), TYPE_BOOL, 0, IDS_IGNORE_CHILDREN,
        p_end,
    FinjinObjectSettingsObject::PB_CHILD_SORTING, _M("ChildSorting"), TYPE_STRING, 0, IDS_CHILD_SORTING,
        p_end,
    FinjinObjectSettingsObject::PB_CHILD_ORDER, _M("ChildOrder"), TYPE_INT, 0, IDS_CHILD_ORDER,
        p_end,

    FinjinObjectSettingsObject::PB_NODE_ANIMATIONS, _M("NodeAnimations"), TYPE_REFTARG_TAB, 0, P_VARIABLE_SIZE, IDS_NODE_ANIMATIONS,
        p_end,
    FinjinObjectSettingsObject::PB_MESH_ANIMATIONS, _M("MeshAnimations"), TYPE_REFTARG_TAB, 0, P_VARIABLE_SIZE, IDS_MESH_ANIMATIONS,
        p_end,
    FinjinObjectSettingsObject::PB_EXPORT_SKELETON, _M("ExportSkeleton"), TYPE_BOOL, 0, IDS_EXPORT_SKELETON,
        p_default, TRUE,
        p_end,        
    FinjinObjectSettingsObject::PB_EXPORTED_MESH_DEFORMED_BY_SKELETON, _M("ExportedMeshDeformedBySkeleton"), TYPE_BOOL, 0, IDS_EXPORTED_MESH_DEFORMED_BY_SKELETON,
        p_default, TRUE,
        p_end,
    FinjinObjectSettingsObject::PB_SKELETON_REFERENCE_POSE_TYPE, _M("SkeletonReferencePoseType"), TYPE_STRING, 0, IDS_SKELETON_REFERENCE_POSE_TYPE,
        p_end,        
    FinjinObjectSettingsObject::PB_SKELETON_REFERENCE_POSE_TIME, _M("SkeletonReferencePoseTime"), TYPE_TIMEVALUE, 0, IDS_SKELETON_REFERENCE_POSE_TIME,
        p_end,    
    FinjinObjectSettingsObject::PB_EXPORT_POSES, _M("ExportPoses"), TYPE_BOOL, 0, IDS_EXPORT_POSES,
        p_default, TRUE,
        p_end,
    FinjinObjectSettingsObject::PB_REMOVE_BONES_WITH_NO_VERTEX_INFLUENCE, _M("RemoveBonesWithNoVertexInfluence"), TYPE_BOOL, 0, IDS_REMOVE_BONES_WITH_NO_VERTEX_INFLUENCE,
        p_end,
    FinjinObjectSettingsObject::PB_EXPORT_MESH_ANIMATIONS_TO_SEPARATE_FILE, _M("ExportMeshAnimationsToSeparateFile"), TYPE_BOOL, 0, IDS_EXPORT_MESH_ANIMATIONS_TO_SEPARATE_FILE,
        p_end,
    FinjinObjectSettingsObject::PB_LINK_MESH_ANIMATIONS_TO_MAIN_OBJECT, _M("LinkMeshAnimationsToMainObject"), TYPE_BOOL, 0, IDS_LINK_MESH_ANIMATIONS_TO_MAIN_OBJECT,
        p_end,

    FinjinObjectSettingsObject::PB_LOD_TYPE, _M("LODType"), TYPE_INT, 0, IDS_LOD_TYPE,
        p_end,
    FinjinObjectSettingsObject::PB_LOD_NODES, _M("LODNodes"), TYPE_INODE_TAB, 0, P_VARIABLE_SIZE, IDS_LOD_NODES,
        p_end,
    FinjinObjectSettingsObject::PB_LOD_DISTANCES, _M("LODDistances"), TYPE_FLOAT_TAB, 0, P_VARIABLE_SIZE, IDS_LOD_DISTANCES,
        p_end,
    FinjinObjectSettingsObject::PB_CALCULATE_MANUAL_LOD_DISTANCES, _M("CalculateManualLODDistances"), TYPE_BOOL, 0, IDS_CALCULATE_MANUAL_LOD_DISTANCES,
        p_end,
    
    FinjinObjectSettingsObject::PB_RENDERING_DISTANCE, _M("RenderingDistance"), TYPE_FLOAT, 0, IDS_RENDERING_DISTANCE,
        p_end,        

    FinjinObjectSettingsObject::PB_EMBED_SKELETON_IN_MESH, _M("EmbedSkeletonInMesh"), TYPE_BOOL, 0, IDS_EMBED_SKELETON_IN_MESH,
        p_default, TRUE,
        p_end,    
    FinjinObjectSettingsObject::PB_LINK_SKELETON_TO_MESH, _M("LinkSkeletonToMesh"), TYPE_BOOL, 0, IDS_LINK_SKELETON_TO_MESH,
        p_default, TRUE,
        p_end,    
    
    FinjinObjectSettingsObject::PB_EXPORTED_PARENT, _M("ExportedParent"), TYPE_INODE, 0, IDS_EXPORTED_PARENT,
        p_end,    
    FinjinObjectSettingsObject::PB_IS_ATTACHED_OBJECT, _M("IsAttachedObject"), TYPE_BOOL, 0, IDS_IS_ATTACHED_OBJECT,
        p_end,    

    FinjinObjectSettingsObject::PB_USE_ROOT_AS_EXPORTED_PARENT, _M("UseRootAsExportedParent"), TYPE_BOOL, 0, IDS_USE_ROOT_AS_EXPORTED_PARENT,
        p_end,

    p_end
    );


//Implementation---------------------------------------------------------------
FinjinObjectSettingsObject::FinjinObjectSettingsObject()
{
    this->pblock = nullptr;
    FinjinObjectSettingsObjectClassDesc::instance.MakeAutoParamBlocks(this);
}

FinjinObjectSettingsObject::~FinjinObjectSettingsObject()
{    
}

CreateMouseCallBack* FinjinObjectSettingsObject::GetCreateMouseCallBack()
{
    return nullptr;
}

ObjectState FinjinObjectSettingsObject::Eval(TimeValue time)
{
    return ObjectState(this);
}

void FinjinObjectSettingsObject::InitNodeName(MSTR& s)
{
    s = _M("FinjinObjectSettings");
}

Class_ID FinjinObjectSettingsObject::ClassID() 
{
    return GetClassClassID();
}        

Class_ID FinjinObjectSettingsObject::GetClassClassID()
{
    return FinjinMaxClassID::ObjectSettings;
}

SClass_ID FinjinObjectSettingsObject::SuperClassID() 
{ 
    return REF_TARGET_CLASS_ID;
}

void FinjinObjectSettingsObject::GetClassName(MSTR& s)
{
    s = MaxUtilities::GetStringM(IDS_OBJECT_SETTINGS_OBJECT_CLASS_NAME);
}

RefTargetHandle FinjinObjectSettingsObject::Clone(RemapDir& remap)
{
    FinjinObjectSettingsObject* newHelper = new FinjinObjectSettingsObject();

    //Copy everything
    newHelper->ReplaceReference(PARAM_BLOCK_REF, remap.CloneRef(this->pblock));
        
    BaseClone(this, newHelper, remap);
    
    return newHelper;
}

RefResult FinjinObjectSettingsObject::NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate)
{
    switch (message)
    {
        case REFMSG_CONTAINER_ELEMENT_NULLED:
        {
            int tabIndex;
            
            auto lastNotifyID = this->pblock->LastNotifyParamID(tabIndex);
            switch (lastNotifyID)
            {
                EXTENDED_VALUE_NOTIFY_NULLED_CASE(lastNotifyID, tabIndex)
                case PB_NODE_ANIMATIONS:
                case PB_MESH_ANIMATIONS: 
                {
                    this->pblock->Delete(lastNotifyID, tabIndex, 1);
                    break;
                }
                case PB_LOD_NODES:
                {
                    this->pblock->Delete(PB_LOD_NODES, tabIndex, 1);
                    this->pblock->Delete(PB_LOD_DISTANCES, tabIndex, 1);
                }
            }

            break;
        }        
    }
    return REF_SUCCEED;
}

int FinjinObjectSettingsObject::NumSubs() 
{ 
    return NUM_REFS; 
}

MSTR FinjinObjectSettingsObject::SubAnimName(int i) 
{
    return MaxUtilities::GetString(IDS_PARAMETERS); 
}                

Animatable* FinjinObjectSettingsObject::SubAnim(int i) 
{
    return this->pblock; 
}

int FinjinObjectSettingsObject::NumRefs() 
{ 
    return NUM_REFS; 
}

RefTargetHandle FinjinObjectSettingsObject::GetReference(int i) 
{
    return this->pblock; 
}

void FinjinObjectSettingsObject::SetReference(int i, RefTargetHandle rtarg) 
{ 
    this->pblock = (IParamBlock2*)rtarg; 
}

ClassDesc* FinjinObjectSettingsObject::GetClassDesc()
{
    return &FinjinObjectSettingsObjectClassDesc::instance;
}

void FinjinObjectSettingsObject::CopyFrom(FinjinObjectSettingsObject* other)
{
    ReplaceReference(PARAM_BLOCK_REF, other->pblock->Clone(DefaultRemapDir()));    
}

FINJIN_EXPORTER_IMPLEMENT_USER_DATA_OBJECT_METHODS(FinjinObjectSettingsObject)

FINJIN_EXPORTER_IMPLEMENT_EXTENDED_VALUE_OBJECT_METHODS(FinjinObjectSettingsObject)

wxString FinjinObjectSettingsObject::GetType()
{
    return this->pblock->GetStr(PB_TYPE);
}

wxString FinjinObjectSettingsObject::GetVisibility()
{
    return this->pblock->GetStr(PB_VISIBILITY);
}

bool FinjinObjectSettingsObject::GetVisibilityAffectObjectOnly()
{
    return this->pblock->GetInt(PB_VISIBILITY_AFFECT_OBJECT_ONLY) ? true : false;
}

bool FinjinObjectSettingsObject::GetIsPrefab()
{
    return this->pblock->GetInt(PB_IS_PREFAB) ? true : false;
}

bool FinjinObjectSettingsObject::GetIsPrefabInstance()
{
    return this->pblock->GetInt(PB_IS_PREFAB_INSTANCE) ? true : false;
}

wxString FinjinObjectSettingsObject::GetPrefabName()
{
    return this->pblock->GetStr(PB_PREFAB_NAME);
}

double FinjinObjectSettingsObject::GetNodeAnimationSampleInterval()
{
    return this->pblock->GetFloat(PB_NODE_ANIMATION_SAMPLE_INTERVAL);
}

SampleType FinjinObjectSettingsObject::GetNodeAnimationSampleType()
{
    return (SampleType)this->pblock->GetInt(PB_NODE_ANIMATION_SAMPLE_TYPE);
}

int FinjinObjectSettingsObject::GetNoteTrackNameCount()
{
    return this->pblock->Count(PB_NOTE_TRACK_NAMES);
}

wxString FinjinObjectSettingsObject::GetNoteTrackName(int index)
{
    return this->pblock->GetStr(PB_NOTE_TRACK_NAMES, 0, index);
}

void FinjinObjectSettingsObject::ClearNoteTrackNames()
{
    this->pblock->ZeroCount(PB_NOTE_TRACK_NAMES);
}

wxString FinjinObjectSettingsObject::GetRenderQueue()
{
    return this->pblock->GetStr(PB_RENDER_QUEUE_NAME);
}

int FinjinObjectSettingsObject::GetRenderPriority()
{
    return this->pblock->GetInt(PB_RENDER_QUEUE_PRIORITY);
}

bool FinjinObjectSettingsObject::GetExportMesh()
{
    return this->pblock->GetInt(PB_EXPORT_MESH) ? true : false;
}

wxString FinjinObjectSettingsObject::GetMeshName()
{
    return this->pblock->GetStr(PB_MESH_NAME);
}

bool FinjinObjectSettingsObject::GetAnimateBoundingVolumes()
{
    return (InheritedBool)this->pblock->GetInt(PB_ANIMATE_BOUNDING_VOLUMES) ? true : false;
}

wxString FinjinObjectSettingsObject::GetSkeletonName()
{
    return this->pblock->GetStr(PB_SKELETON_NAME);
}

wxString FinjinObjectSettingsObject::GetMeshAnimationType()
{
    return this->pblock->GetStr(PB_MESH_ANIMATION_TYPE);
}

double FinjinObjectSettingsObject::GetSkeletonAnimationSampleInterval()
{
    return this->pblock->GetFloat(PB_SKELETON_ANIMATION_SAMPLE_INTERVAL);
}

SampleType FinjinObjectSettingsObject::GetSkeletonAnimationSampleType()
{
    return (SampleType)this->pblock->GetInt(PB_SKELETON_ANIMATION_SAMPLE_TYPE);
}

double FinjinObjectSettingsObject::GetVertexAnimationSampleInterval()
{
    return this->pblock->GetFloat(PB_VERTEX_ANIMATION_SAMPLE_INTERVAL);
}

INode* FinjinObjectSettingsObject::GetAnimatedRoot()
{
    return this->pblock->GetINode(PB_ANIMATED_ROOT);
}

SampleType FinjinObjectSettingsObject::GetVertexAnimationSampleType()
{
    return (SampleType)this->pblock->GetInt(PB_VERTEX_ANIMATION_SAMPLE_TYPE);
}

wxString FinjinObjectSettingsObject::GetPrimitiveType()
{
    return this->pblock->GetStr(PB_PRIMITIVE_TYPE);
}

bool FinjinObjectSettingsObject::GetExportVertexColors()
{
    return this->pblock->GetInt(PB_EXPORT_VERTEX_COLORS) ? true : false;
}

bool FinjinObjectSettingsObject::GetExportTangents()
{
    return this->pblock->GetInt(PB_EXPORT_TANGENTS) ? true : false;
}

bool FinjinObjectSettingsObject::GetExportBinormals()
{
    return this->pblock->GetInt(PB_EXPORT_BINORMALS) ? true : false;
}

wxString FinjinObjectSettingsObject::GetItemType()
{
    return this->pblock->GetStr(PB_ITEM_TYPE);
}

bool FinjinObjectSettingsObject::GetEnableSky()
{
    return this->pblock->GetInt(PB_ENABLE_SKY) ? true : false;
}

bool FinjinObjectSettingsObject::GetEmbedSkeletonInMesh()
{
    return this->pblock->GetInt(PB_EMBED_SKELETON_IN_MESH) ? true : false;
}

bool FinjinObjectSettingsObject::GetLinkSkeletonToMesh()
{
    return this->pblock->GetInt(PB_LINK_SKELETON_TO_MESH) ? true : false;
}

float FinjinObjectSettingsObject::GetPointSize()
{
    return this->pblock->GetFloat(PB_POINT_SIZE);
}

bool FinjinObjectSettingsObject::GetIgnoreChildren()
{
    return this->pblock->GetInt(PB_IGNORE_CHILDREN) ? true : false;
}

wxString FinjinObjectSettingsObject::GetChildSorting()
{
    return this->pblock->GetStr(PB_CHILD_SORTING);
}

int FinjinObjectSettingsObject::GetChildOrder()
{
    return this->pblock->GetInt(PB_CHILD_ORDER);
}

int FinjinObjectSettingsObject::GetNodeAnimationCount()
{
    return this->pblock->Count(PB_NODE_ANIMATIONS);
}

FinjinNodeAnimationSettingsObject* FinjinObjectSettingsObject::GetNodeAnimation(int index)
{
    return (FinjinNodeAnimationSettingsObject*)this->pblock->GetReferenceTarget(PB_NODE_ANIMATIONS, 0, index);
}

void FinjinObjectSettingsObject::GetNodeAnimationNames(std::vector<wxString>& names)
{
    int count = GetNodeAnimationCount();
    names.resize(count);
    for (int i = 0; i < count; i++)
        names[i] = GetNodeAnimation(i)->GetAnimationName();
}

int FinjinObjectSettingsObject::GetMeshAnimationCount()
{
    return this->pblock->Count(PB_MESH_ANIMATIONS);
}

FinjinMeshAnimationSettingsObject* FinjinObjectSettingsObject::GetMeshAnimation(int index)
{
    return (FinjinMeshAnimationSettingsObject*)this->pblock->GetReferenceTarget(PB_MESH_ANIMATIONS, 0, index);
}

FinjinMeshAnimationSettingsObject* FinjinObjectSettingsObject::GetMeshAnimation(const wxString& name, int* foundAt)
{
    int count = GetMeshAnimationCount();
    for (int i = 0; i < count; i++)
    {
        FinjinMeshAnimationSettingsObject* anim = GetMeshAnimation(i);
        if (anim->GetAnimationName() == name)
        {
            if (foundAt != nullptr)
                *foundAt = i;
            return anim;
        }
    }
    return nullptr;
}

void FinjinObjectSettingsObject::GetMeshAnimationNames(std::vector<wxString>& names)
{
    int count = GetMeshAnimationCount();
    names.resize(count);
    for (int i = 0; i < count; i++)
        names[i] = GetMeshAnimation(i)->GetAnimationName();
}

bool FinjinObjectSettingsObject::GetExportFlags()
{
    return this->pblock->GetInt(PB_EXPORT_FLAGS) ? true : false;
}

int FinjinObjectSettingsObject::GetFlagCount()
{
    return this->pblock->Count(PB_FLAG_NAMES);
}

void FinjinObjectSettingsObject::GetFlag(int index, wxString& name)
{
    name = this->pblock->GetStr(PB_FLAG_NAMES, 0, index);    
}

void FinjinObjectSettingsObject::GetFlagNames(std::set<wxString>& names)
{
    wxString name;
    int count = GetFlagCount();
    for (int i = 0; i < count; i++)
    {
        name = this->pblock->GetStr(PB_FLAG_NAMES, 0, i);
        names.insert(name);
    }
}

void FinjinObjectSettingsObject::SetExportFlags(bool value)
{
    this->pblock->SetValue(PB_EXPORT_FLAGS, 0, (int)value);    
}

void FinjinObjectSettingsObject::ClearFlags()
{
    this->pblock->ZeroCount(PB_FLAG_NAMES);    
}

int FinjinObjectSettingsObject::GetManualLODCount()
{
    return this->pblock->Count(PB_LOD_NODES);
}

void FinjinObjectSettingsObject::GetManualLOD(int index, INode*& node, float& distance)
{
    node = this->pblock->GetINode(PB_LOD_NODES, 0, index);
    distance = this->pblock->GetFloat(PB_LOD_DISTANCES, 0, index);
}

INode* FinjinObjectSettingsObject::GetManuaLODNode(int index)
{
    return this->pblock->GetINode(PB_LOD_NODES, 0, index);
}

void FinjinObjectSettingsObject::RemoveManualLOD(int index)
{
    this->pblock->Delete(PB_LOD_NODES, index, 1);
    this->pblock->Delete(PB_LOD_DISTANCES, index, 1);
}

bool FinjinObjectSettingsObject::GetCalculateManualLodDistances()
{
    return this->pblock->GetInt(PB_CALCULATE_MANUAL_LOD_DISTANCES) ? true : false;
}

float FinjinObjectSettingsObject::GetRenderingDistance()
{
    return this->pblock->GetFloat(PB_RENDERING_DISTANCE);
}

bool FinjinObjectSettingsObject::GetCheckMeshInstance()
{
    return this->pblock->GetInt(PB_CHECK_MESH_INSTANCE) ? true : false;
}

bool FinjinObjectSettingsObject::GetUseCustomSubmeshes()
{
    return this->pblock->GetInt(PB_USE_CUSTOM_SUBMESHES) ? true : false;
}

void FinjinObjectSettingsObject::GetSubmeshesSettings(SubmeshesSettings& value)
{
    value.resize(this->pblock->Count(PB_SUBMESHES_SETTINGS));
    for (int i = 0; i < (int)value.size(); i++)
    {
        FinjinSubmeshSettingsObject* settings = 
            (FinjinSubmeshSettingsObject*)this->pblock->GetReferenceTarget(PB_SUBMESHES_SETTINGS, 0, i);
        settings->GetValue(value[i]);
    }
}

bool FinjinObjectSettingsObject::GetExportSkeleton()
{
    return this->pblock->GetInt(PB_EXPORT_SKELETON) ? true : false;
}

bool FinjinObjectSettingsObject::GetExportedMeshDeformedBySkeleton()
{
    return this->pblock->GetInt(PB_EXPORTED_MESH_DEFORMED_BY_SKELETON) ? true : false;
}

wxString FinjinObjectSettingsObject::GetSkeletonReferencePoseType()
{
    return this->pblock->GetStr(PB_SKELETON_REFERENCE_POSE_TYPE);
}

TimeValue FinjinObjectSettingsObject::GetSkeletonReferencePoseTime()
{
    return this->pblock->GetTimeValue(PB_SKELETON_REFERENCE_POSE_TIME);
}

bool FinjinObjectSettingsObject::GetExportPoses()
{
    return this->pblock->GetInt(PB_EXPORT_POSES) ? true : false;
}

bool FinjinObjectSettingsObject::GetRemoveBonesWithNoVertexInfluence()
{
    return this->pblock->GetInt(PB_REMOVE_BONES_WITH_NO_VERTEX_INFLUENCE) ? true : false;
}

bool FinjinObjectSettingsObject::GetAttachedObject()
{
    return this->pblock->GetInt(PB_IS_ATTACHED_OBJECT) ? true : false;
}

INode* FinjinObjectSettingsObject::GetExportedParent()
{
    return this->pblock->GetINode(PB_EXPORTED_PARENT);
}

bool FinjinObjectSettingsObject::GetUseRootAsExportedParent()
{
    return this->pblock->GetInt(PB_USE_ROOT_AS_EXPORTED_PARENT) ? true : false;
}

void FinjinObjectSettingsObject::SetType(wxString value)
{
    this->pblock->SetValue(PB_TYPE, 0, WxStringToApplicationStringM(value));
}

void FinjinObjectSettingsObject::SetVisibility(wxString value)
{
    this->pblock->SetValue(PB_VISIBILITY, 0, WxStringToApplicationStringM(value));
}

void FinjinObjectSettingsObject::SetVisibilityAffectObjectOnly(bool value)
{
    this->pblock->SetValue(PB_VISIBILITY_AFFECT_OBJECT_ONLY, 0, (int)value);
}

void FinjinObjectSettingsObject::SetIsPrefab(bool value)
{
    this->pblock->SetValue(PB_IS_PREFAB, 0, (int)value);
}

void FinjinObjectSettingsObject::SetIsPrefabInstance(bool value)
{
    this->pblock->SetValue(PB_IS_PREFAB_INSTANCE, 0, (int)value);
}

void FinjinObjectSettingsObject::SetPrefabName(wxString value)
{
    this->pblock->SetValue(PB_PREFAB_NAME, 0, WxStringToApplicationStringM(value));
}

void FinjinObjectSettingsObject::SetNodeAnimationSampleInterval(double value)
{
    this->pblock->SetValue(PB_NODE_ANIMATION_SAMPLE_INTERVAL, 0, (float)value);
}

void FinjinObjectSettingsObject::SetNodeAnimationSampleType(SampleType value)
{
    this->pblock->SetValue(PB_NODE_ANIMATION_SAMPLE_TYPE, 0, (int)value);
}

void FinjinObjectSettingsObject::AddNoteTrackName(wxString value)
{
    MSTR maxValue = WxStringToApplicationStringM(value);
    const MCHAR* names[1] = {maxValue};
    this->pblock->Append(PB_NOTE_TRACK_NAMES, 1, names);
}

void FinjinObjectSettingsObject::SetRenderQueue(wxString value)
{
    this->pblock->SetValue(PB_RENDER_QUEUE_NAME, 0, WxStringToApplicationStringM(value));
}

void FinjinObjectSettingsObject::SetRenderPriority(int value)
{
    this->pblock->SetValue(PB_RENDER_QUEUE_PRIORITY, 0, value);
}

void FinjinObjectSettingsObject::SetExportMesh(bool value)
{
    this->pblock->SetValue(PB_EXPORT_MESH, 0, (int)value);
}

void FinjinObjectSettingsObject::SetMeshName(wxString value)
{
    this->pblock->SetValue(PB_MESH_NAME, 0, WxStringToApplicationStringM(value));
}

void FinjinObjectSettingsObject::SetAnimateBoundingVolumes(bool value)
{
    this->pblock->SetValue(PB_ANIMATE_BOUNDING_VOLUMES, 0, (int)value);
}

void FinjinObjectSettingsObject::SetSkeletonName(wxString value)
{
    this->pblock->SetValue(PB_SKELETON_NAME, 0, WxStringToApplicationStringM(value));
}

void FinjinObjectSettingsObject::SetMeshAnimationType(wxString value)
{
    this->pblock->SetValue(PB_MESH_ANIMATION_TYPE, 0, WxStringToApplicationStringM(value));
}

void FinjinObjectSettingsObject::SetSkeletonAnimationSampleInterval(double value)
{
    this->pblock->SetValue(PB_SKELETON_ANIMATION_SAMPLE_INTERVAL, 0, (float)value);
}

void FinjinObjectSettingsObject::SetSkeletonAnimationSampleType(SampleType value)
{
    this->pblock->SetValue(PB_SKELETON_ANIMATION_SAMPLE_TYPE, 0, (int)value);
}

void FinjinObjectSettingsObject::SetVertexAnimationSampleInterval(double value)
{
    this->pblock->SetValue(PB_VERTEX_ANIMATION_SAMPLE_INTERVAL, 0, (float)value);
}

void FinjinObjectSettingsObject::SetVertexAnimationSampleType(SampleType value)
{
    this->pblock->SetValue(PB_VERTEX_ANIMATION_SAMPLE_TYPE, 0, (int)value);
}

void FinjinObjectSettingsObject::SetAnimatedRoot(INode* value)
{
    this->pblock->SetValue(PB_ANIMATED_ROOT, 0, value);
}

void FinjinObjectSettingsObject::SetPrimitiveType(wxString value)
{
    this->pblock->SetValue(PB_PRIMITIVE_TYPE, 0, WxStringToApplicationStringM(value));
}

void FinjinObjectSettingsObject::SetExportVertexColors(bool value)
{
    this->pblock->SetValue(PB_EXPORT_VERTEX_COLORS, 0, (int)value);
}

void FinjinObjectSettingsObject::SetExportTangents(bool value)
{
    this->pblock->SetValue(PB_EXPORT_TANGENTS, 0, (int)value);
}

void FinjinObjectSettingsObject::SetExportBinormals(bool value)
{
    this->pblock->SetValue(PB_EXPORT_BINORMALS, 0, (int)value);
}

void FinjinObjectSettingsObject::SetItemType(wxString value)
{
    this->pblock->SetValue(PB_ITEM_TYPE, 0, WxStringToApplicationStringM(value));
}
    
void FinjinObjectSettingsObject::SetEnableSky(bool value)
{
    this->pblock->SetValue(PB_ENABLE_SKY, 0, (int)value);
}

void FinjinObjectSettingsObject::SetEmbedSkeletonInMesh(bool value)
{
    this->pblock->SetValue(PB_EMBED_SKELETON_IN_MESH, 0, (int)value);
}

void FinjinObjectSettingsObject::SetLinkSkeletonToMesh(bool value)
{
    this->pblock->SetValue(PB_LINK_SKELETON_TO_MESH, 0, (int)value);
}

void FinjinObjectSettingsObject::SetPointSize(float value)
{
    this->pblock->SetValue(PB_POINT_SIZE, 0, value);
}

void FinjinObjectSettingsObject::SetIgnoreChildren(bool value)
{
    this->pblock->SetValue(PB_IGNORE_CHILDREN, 0, (int)value);
}

void FinjinObjectSettingsObject::SetChildSorting(wxString value)
{
    this->pblock->SetValue(PB_CHILD_SORTING, 0, WxStringToApplicationStringM(value));
}

void FinjinObjectSettingsObject::SetChildOrder(int value)
{
    this->pblock->SetValue(PB_CHILD_ORDER, 0, value);
}

void FinjinObjectSettingsObject::AddNodeAnimation(FinjinNodeAnimationSettingsObject* value)
{
    ReferenceTarget* values[1] = {value};
    this->pblock->Append(PB_NODE_ANIMATIONS, 1, values);
}

void FinjinObjectSettingsObject::InsertNodeAnimation(FinjinNodeAnimationSettingsObject* value, int index)
{
    ReferenceTarget* values[1] = {value};
    this->pblock->Insert(PB_NODE_ANIMATIONS, index, 1, values);
}

void FinjinObjectSettingsObject::AddMeshAnimation(FinjinMeshAnimationSettingsObject* value)
{
    ReferenceTarget* values[1] = {value};
    this->pblock->Append(PB_MESH_ANIMATIONS, 1, values);
}

void FinjinObjectSettingsObject::InsertMeshAnimation(FinjinMeshAnimationSettingsObject* value, int index)
{
    ReferenceTarget* values[1] = {value};
    this->pblock->Insert(PB_MESH_ANIMATIONS, index, 1, values);
}

void FinjinObjectSettingsObject::AddFlag(wxString name)
{
    MSTR maxValue = WxStringToApplicationStringM(name);
    const MCHAR* names[1] = {maxValue};
    this->pblock->Append(PB_FLAG_NAMES, 1, names);
}

void FinjinObjectSettingsObject::AddManualLOD(INode* node, float distance)
{
    INode* nodes[1] = {node};
    this->pblock->Append(PB_LOD_NODES, 1, nodes);

    this->pblock->Append(PB_LOD_DISTANCES, 1, &distance);
}

void FinjinObjectSettingsObject::SetManualLOD(int index, INode* node, float distance)
{
    this->pblock->SetValue(PB_LOD_NODES, 0, node, index);
    this->pblock->SetValue(PB_LOD_DISTANCES, 0, distance, index);
}

void FinjinObjectSettingsObject::SetCalculateManualLodDistances(bool value)
{
    this->pblock->SetValue(PB_CALCULATE_MANUAL_LOD_DISTANCES, 0, (int)value);
}

void FinjinObjectSettingsObject::SetRenderingDistance(float value)
{
    this->pblock->SetValue(PB_RENDERING_DISTANCE, 0, value);
}

void FinjinObjectSettingsObject::SetCheckMeshInstance(bool value)
{
    this->pblock->SetValue(PB_CHECK_MESH_INSTANCE, 0, (int)value);
}

void FinjinObjectSettingsObject::SetUseCustomSubmeshes(bool value)
{
    this->pblock->SetValue(PB_USE_CUSTOM_SUBMESHES, 0, (int)value);
}

void FinjinObjectSettingsObject::SetSubmeshesSettings(const SubmeshesSettings& value)
{
    this->pblock->ZeroCount(PB_SUBMESHES_SETTINGS);
    for (size_t i = 0; i < value.size(); i++)
    {
        FinjinSubmeshSettingsObject* settings = new FinjinSubmeshSettingsObject;
        settings->SetValue(value[i]);
        
        ReferenceTarget* values[1] = {settings};
        this->pblock->Append(PB_SUBMESHES_SETTINGS, 1, values);
    }
}

void FinjinObjectSettingsObject::SetExportSkeleton(bool value)
{
    this->pblock->SetValue(PB_EXPORT_SKELETON, 0, (int)value);
}

void FinjinObjectSettingsObject::SetExportedMeshDeformedBySkeleton(bool value)
{
    this->pblock->SetValue(PB_EXPORTED_MESH_DEFORMED_BY_SKELETON, 0, (int)value);
}

void FinjinObjectSettingsObject::SetSkeletonReferencePoseType(wxString value)
{
    this->pblock->SetValue(PB_SKELETON_REFERENCE_POSE_TYPE, 0, WxStringToApplicationStringM(value));
}

void FinjinObjectSettingsObject::SetSkeletonReferencePoseTime(TimeValue value)
{
    this->pblock->SetValue(PB_SKELETON_REFERENCE_POSE_TIME, 0, value);
}

void FinjinObjectSettingsObject::SetExportPoses(bool value)
{
    this->pblock->SetValue(PB_EXPORT_POSES, 0, (int)value);
}

void FinjinObjectSettingsObject::SetRemoveBonesWithNoVertexInfluence(bool value)
{
    this->pblock->SetValue(PB_REMOVE_BONES_WITH_NO_VERTEX_INFLUENCE, 0, (int)value);
}

void FinjinObjectSettingsObject::SetAttachedObject(bool value)
{
    this->pblock->SetValue(PB_IS_ATTACHED_OBJECT, 0, (int)value);
}

void FinjinObjectSettingsObject::SetExportedParent(INode* value)
{
    this->pblock->SetValue(PB_EXPORTED_PARENT, 0, value);
}

void FinjinObjectSettingsObject::SetUseRootAsExportedParent(bool value)
{
    this->pblock->SetValue(PB_USE_ROOT_AS_EXPORTED_PARENT, 0, value);
}
