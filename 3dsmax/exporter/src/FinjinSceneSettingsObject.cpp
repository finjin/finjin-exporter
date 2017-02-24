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
#include "FinjinSceneSettingsObject.hpp"
#include "FinjinObjectSettingsObject.hpp"
#include "FinjinMeshAnimationSettingsObject.hpp"
#include "FinjinNodeAnimationSettingsObject.hpp"
#include "MaxUtilities.hpp"
#include "StringUtilities.hpp"
#include "FileUtilities.hpp"
#include "MaxMaterialUtilities.hpp"
#include "MaxMathUtilities.hpp"
#include "PropertyValues.hpp"
#include "BaseDirectory.hpp"
#include "FinjinMaxClassID.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Local classes----------------------------------------------------------------
class FinjinSceneSettingsObjectClassDesc : public ClassDesc2
{
public:
    int IsPublic() override                      {return FALSE;}
    void* Create(BOOL loading = FALSE) override  {return new FinjinSceneSettingsObject();}
    const MCHAR* ClassName() override            {return MaxUtilities::GetStringM(IDS_SCENE_SETTINGS_OBJECT_CLASS_NAME);}
    SClass_ID SuperClassID() override            {return REF_TARGET_CLASS_ID;}
    Class_ID ClassID() override                  {return FinjinSceneSettingsObject::GetClassClassID();}
    const MCHAR* Category() override             {return MaxUtilities::GetStringM(IDS_PLUGINS_CATEGORY);}
    const MCHAR* InternalName() override         {return _M("FinjinSceneSettingsObject");}
    HINSTANCE HInstance() override               {return wxGetInstance();}

public:
    static FinjinSceneSettingsObjectClassDesc instance;
};
FinjinSceneSettingsObjectClassDesc FinjinSceneSettingsObjectClassDesc::instance;


//Globals----------------------------------------------------------------------
enum {SCENE_SETTINGS_PARAM_BLOCK_ID = 0};
enum {SCENE_SETTINGS_VERSION = 1};

static ParamBlockDesc2 FinjinSceneSettingsObjectParamBlock
    (
    SCENE_SETTINGS_PARAM_BLOCK_ID, _M("Parameters"),  0, &FinjinSceneSettingsObjectClassDesc::instance, P_VERSION | P_AUTO_CONSTRUCT, 
    
    //Version
    SCENE_SETTINGS_VERSION,
    
    //Reference number
    FinjinSceneSettingsObject::PARAM_BLOCK_REF, 
    
    //Parameters

    USER_DATA_PARAMETER_BLOCK_PARAMETERS(FinjinSceneSettingsObject),

    EXTENDED_VALUE_PARAMETER_BLOCK_PARAMETERS(FinjinSceneSettingsObject),

    FinjinSceneSettingsObject::PB_NODES, _M("Nodes"), TYPE_INODE_TAB, 0, P_VARIABLE_SIZE, IDS_NODES,
        p_end,    
    FinjinSceneSettingsObject::PB_OBJECTS, _M("Objects"), TYPE_REFTARG_TAB, 0, P_VARIABLE_SIZE, IDS_OBJECTS,
        p_end,

    FinjinSceneSettingsObject::PB_SCENE_MANAGER, _M("SceneManager"), TYPE_STRING, 0, IDS_SCENE_MANAGER,
        p_end,
    FinjinSceneSettingsObject::PB_EXPORT_ENVIRONMENT, _M("ExportEnvironment"), TYPE_BOOL, 0, IDS_EXPORT_ENVIRONMENT,
        p_default, TRUE,
        p_end,
    FinjinSceneSettingsObject::PB_AMBIENT_LIGHT_COLOR, _M("AmbientLightColor"), TYPE_RGBA, 0, IDS_AMBIENT_LIGHT_COLOR,
        p_default, Color(WIN_RGB(85, 85, 85)),
        p_end,
    FinjinSceneSettingsObject::PB_AMBIENT_LIGHT_COLOR_SYNC, _M("AmbientLightColorSync"), TYPE_INT, 0, IDS_AMBIENT_LIGHT_COLOR_SYNC,
        p_default, (int)ValueSourceSync::SEPARATE,
        p_end,
    FinjinSceneSettingsObject::PB_BACKGROUND_COLOR, _M("BackgroundColor"), TYPE_RGBA, 0, IDS_BACKGROUND_COLOR,
        p_end,
    FinjinSceneSettingsObject::PB_BACKGROUND_COLOR_SYNC, _M("BackgroundColorSync"), TYPE_INT, 0, IDS_BACKGROUND_COLOR_SYNC,
        p_default, (int)ValueSourceSync::SEPARATE,
        p_end,
    FinjinSceneSettingsObject::PB_ENVIRONMENT_NEAR, _M("EnvironmentNear"), TYPE_FLOAT, 0, IDS_ENVIRONMENT_NEAR,
        p_end,
    FinjinSceneSettingsObject::PB_ENVIRONMENT_FAR, _M("EnvironmentFar"), TYPE_FLOAT, 0, IDS_ENVIRONMENT_FAR,
        p_default, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE,
        p_end,
    FinjinSceneSettingsObject::PB_ENVIRONMENT_FAR_OBJECT, _M("EnvironmentFarObject"), TYPE_INODE, 0, IDS_ENVIRONMENT_FAR_OBJECT,
        p_end,
    FinjinSceneSettingsObject::PB_FOG_SYNC, _M("FogSync"), TYPE_INT, 0, IDS_FOG_SYNC,
        p_default, (int)ValueSourceSync::SEPARATE,
        p_end,
    FinjinSceneSettingsObject::PB_FOG_MODE, _M("FogMode"), TYPE_STRING, 0, IDS_FOG_MODE,
        p_end,
    FinjinSceneSettingsObject::PB_FOG_DENSITY, _M("FogDensity"), TYPE_FLOAT, 0, IDS_FOG_DENSITY,
        p_end,
    FinjinSceneSettingsObject::PB_FOG_START, _M("FogStart"), TYPE_FLOAT, 0, IDS_FOG_START,
        p_default, 0.0f,
        p_end,
    FinjinSceneSettingsObject::PB_FOG_END, _M("FogEnd"), TYPE_FLOAT, 0, IDS_FOG_END,
        p_default, 1.0f,
        p_end,
    FinjinSceneSettingsObject::PB_FOG_COLOR, _M("FogColor"), TYPE_RGBA, 0, IDS_FOG_COLOR,
        p_end,
    
    FinjinSceneSettingsObject::PB_SHADOW_TECHNIQUE, _M("ShadowTechnique"), TYPE_STRING, 0, IDS_SHADOW_TECHNIQUE,
        p_end,
    FinjinSceneSettingsObject::PB_SHADOW_COLOR, _M("ShadowColor"), TYPE_RGBA, 0, IDS_SHADOW_COLOR,
        p_end,
    FinjinSceneSettingsObject::PB_SHADOW_FAR_DISTANCE, _M("ShadowFarDistance"), TYPE_FLOAT, 0, IDS_SHADOW_FAR_DISTANCE,
        p_end,
    
    FinjinSceneSettingsObject::PB_SCENE_SCALE_UNIT, _M("SceneScaleUnit"), TYPE_STRING, 0, IDS_SCENE_SCALE_UNIT,
        p_end,
    FinjinSceneSettingsObject::PB_SCENE_SCALE_DIVIDE, _M("SceneScaleDivide"), TYPE_FLOAT, 0, IDS_SCENE_SCALE_DIVIDE,
        p_default, 1.0f,
        p_end,

    FinjinSceneSettingsObject::PB_EXPORT_HIERARCHY, _M("ExportHierarchy"), TYPE_BOOL, 0, IDS_EXPORT_HIERARCHY,
        p_default, TRUE,
        p_end,
    FinjinSceneSettingsObject::PB_IGNORE_HIDDEN_LAYERS, _M("IgnoreHiddenLayers"), TYPE_BOOL, 0, IDS_IGNORE_HIDDEN_LAYERS,
        p_end,
    FinjinSceneSettingsObject::PB_IGNORE_HIDDEN_OBJECTS, _M("IgnoreHiddenObjects"), TYPE_BOOL, 0, IDS_IGNORE_HIDDEN_OBJECTS,
        p_default, TRUE,
        p_end,
        
    FinjinSceneSettingsObject::PB_NODE_ANIMATION_SAMPLE_INTERVAL, _M("NodeAnimationSampleInterval"), TYPE_FLOAT, 0, IDS_NODE_ANIMATION_SAMPLE_INTERVAL,
        p_end,
    FinjinSceneSettingsObject::PB_NODE_ANIMATION_SAMPLE_TYPE, _M("NodeAnimationSampleType"), TYPE_INT, 0, IDS_NODE_ANIMATION_SAMPLE_TYPE,
        p_end,
    FinjinSceneSettingsObject::PB_CONFIGURATION_TARGET_NAME, _M("ConfigurationTargetName"), TYPE_STRING, 0, IDS_CONFIGURATION_TARGET_NAME,
        p_end,
    FinjinSceneSettingsObject::PB_SKELETON_ANIMATION_SAMPLE_INTERVAL, _M("SkeletonAnimationSampleInterval"), TYPE_FLOAT, 0, IDS_SKELETON_ANIMATION_SAMPLE_INTERVAL,
        p_end,
    FinjinSceneSettingsObject::PB_SKELETON_ANIMATION_SAMPLE_TYPE, _M("SkeletonAnimationSampleType"), TYPE_INT, 0, IDS_SKELETON_ANIMATION_SAMPLE_TYPE,
        p_end,
    FinjinSceneSettingsObject::PB_VERTEX_ANIMATION_SAMPLE_INTERVAL, _M("VertexAnimationSampleInterval"), TYPE_FLOAT, 0, IDS_VERTEX_ANIMATION_SAMPLE_INTERVAL,
        p_end,
    FinjinSceneSettingsObject::PB_VERTEX_ANIMATION_SAMPLE_TYPE, _M("VertexAnimationSampleType"), TYPE_INT, 0, IDS_VERTEX_ANIMATION_SAMPLE_TYPE,
        p_end,
    
    FinjinSceneSettingsObject::PB_EXPORT_MESHES, _M("ExportMeshes"), TYPE_BOOL, 0, IDS_EXPORT_MESHES,
        p_default, TRUE,
        p_end,
    FinjinSceneSettingsObject::PB_DISALLOWED_MESHES, _M("DisallowedMeshes"), TYPE_INODE_TAB, 0, P_VARIABLE_SIZE, IDS_DISALLOWED_MESHES,
        p_end,
    FinjinSceneSettingsObject::PB_EXPORT_VERTEX_COLORS, _M("ExportVertexColors"), TYPE_BOOL, 0, IDS_EXPORT_VERTEX_COLORS,
        p_default, TRUE,
        p_end,
    FinjinSceneSettingsObject::PB_SUBMESH_NAMING, _M("SubmeshNaming"), TYPE_STRING, 0, IDS_SUBMESH_NAMING,
        p_end,
    FinjinSceneSettingsObject::PB_SUBMESH_CUSTOM_NAME, _M("SubmeshCustomName"), TYPE_STRING, 0, IDS_SUBMESH_CUSTOM_NAME,
        p_end,
    FinjinSceneSettingsObject::PB_ALWAYS_CREATE_NEW_SUBMESHES_WHEN_MERGING, _M("AlwaysCreateNewSubmeshesWhenMerging"), TYPE_BOOL, 0, IDS_ALWAYS_CREATE_NEW_SUBMESHES_WHEN_MERGING,
        p_default, TRUE,
        p_end,        
    FinjinSceneSettingsObject::PB_CONVERT_BITMAPS_TO_TEXTURES, _M("ConvertTexturesToBitmaps"), TYPE_BOOL, 0, IDS_GENERATE_NORMALS,
        p_default, FALSE,
        p_end,
    FinjinSceneSettingsObject::PB_EXPORT_TANGENTS, _M("ExportTangents"), TYPE_BOOL, 0, IDS_EXPORT_TANGENTS,
        p_default, TRUE,
        p_end,
    FinjinSceneSettingsObject::PB_EXPORT_BINORMALS, _M("ExportBinormals"), TYPE_BOOL, 0, IDS_GENERATE_NORMALS,
        p_end,
    FinjinSceneSettingsObject::PB_ANIMATE_BOUNDING_VOLUMES, _M("AnimateBoundingVolumes"), TYPE_BOOL, 0, IDS_ANIMATE_BOUNDING_VOLUMES,
        p_end,
    FinjinSceneSettingsObject::PB_CHECK_MESH_INSTANCES, _M("CheckMeshInstances"), TYPE_BOOL, 0, IDS_CHECK_MESH_INSTANCES,
        p_end,
    FinjinSceneSettingsObject::PB_VERTEX_COLOR_FORMAT, _M("VertexColorFormat"), TYPE_STRING, 0, IDS_VERTEX_COLOR_FORMAT,
        p_end,
    
    FinjinSceneSettingsObject::PB_EXPORT_MATERIALS, _M("ExportMaterials"), TYPE_BOOL, 0, IDS_EXPORT_MATERIALS,
        p_default, TRUE,
        p_end,
    FinjinSceneSettingsObject::PB_ADDITIONAL_MATERIALS, _M("AdditionalMaterials"), TYPE_MTL_TAB, 0, P_VARIABLE_SIZE, IDS_ADDITIONAL_MATERIALS,
        p_end,
    FinjinSceneSettingsObject::PB_DISALLOWED_MATERIALS, _M("DisallowedMaterials"), TYPE_MTL_TAB, 0, P_VARIABLE_SIZE, IDS_DISALLOWED_MATERIALS,
        p_end,
    FinjinSceneSettingsObject::PB_NO_MATERIAL_NAME, _M("NoMaterialName"), TYPE_STRING, 0, IDS_NO_MATERIAL_NAME,
        p_end,

    FinjinSceneSettingsObject::PB_RENDER_SYSTEM, _M("RenderSystem"), TYPE_STRING, 0, IDS_RENDER_SYSTEM,
        p_end,

    FinjinSceneSettingsObject::PB_RENAME_BITMAPS_FROM, _M("RenameBitmapsFrom"), TYPE_STRING_TAB, 0, P_VARIABLE_SIZE, IDS_RENAME_BITMAPS_FROM,
        p_end,
    FinjinSceneSettingsObject::PB_RENAME_BITMAPS_TO, _M("RenameBitmapsTo"), TYPE_STRING_TAB, 0, P_VARIABLE_SIZE, IDS_RENAME_BITMAPS_TO,
        p_end,

    FinjinSceneSettingsObject::PB_RENAME_BITMAP_EXTENSIONS_FROM, _M("RenameBitmapExtensionsFrom"), TYPE_STRING_TAB, 0, P_VARIABLE_SIZE, IDS_RENAME_BITMAP_EXTENSIONS_FROM,
        p_end,
    FinjinSceneSettingsObject::PB_RENAME_BITMAP_EXTENSIONS_TO, _M("RenameBitmapExtensionsTo"), TYPE_STRING_TAB, 0, P_VARIABLE_SIZE, IDS_RENAME_BITMAP_EXTENSIONS_TO,
        p_end,

    FinjinSceneSettingsObject::PB_BEFORE_EXPORT_COMMANDS, _M("BeforeExportCommands"), TYPE_STRING_TAB, 0, P_VARIABLE_SIZE, IDS_BEFORE_EXPORT_COMMANDS,
        p_end,
    FinjinSceneSettingsObject::PB_AFTER_EXPORT_COMMANDS, _M("AfterExportCommands"), TYPE_STRING_TAB, 0, P_VARIABLE_SIZE, IDS_AFTER_EXPORT_COMMANDS,
        p_end,
        
    FinjinSceneSettingsObject::PB_EXPORT_FLAGS, _M("ExportFlags"), TYPE_BOOL, 0, IDS_EXPORT_FLAGS,
        p_default, TRUE,
        p_end,
    FinjinSceneSettingsObject::PB_FLAG_NAMES, _M("FlagNames"), TYPE_STRING_TAB, 0, P_VARIABLE_SIZE, IDS_FLAG_NAMES,
        p_end,
    
    FinjinSceneSettingsObject::PB_CREATE_EXTRA_MATERIALS, _M("CreateExtraMaterials"), TYPE_BOOL, 0, IDS_CREATE_EXTRA_MATERIALS,
        p_end,
    FinjinSceneSettingsObject::PB_INCLUDE_PARENT_MATERIAL_NAME, _M("IncludeParentMaterialName"), TYPE_BOOL, 0, IDS_INCLUDE_PARENT_MATERIAL_NAME,
        p_end,

    FinjinSceneSettingsObject::PB_USE_EXPLICIT_VERTEX_COLOR, _M("ExplicitVertexColor"), TYPE_BOOL, 0, IDS_EXPLICIT_VERTEX_COLOR,
        p_end,
            
    p_end
    );
    

//Implementation---------------------------------------------------------------
FinjinSceneSettingsObject::FinjinSceneSettingsObject()
{
    this->pblock = nullptr;
    FinjinSceneSettingsObjectClassDesc::instance.MakeAutoParamBlocks(this);
}

FinjinSceneSettingsObject::~FinjinSceneSettingsObject()
{       
}

CreateMouseCallBack* FinjinSceneSettingsObject::GetCreateMouseCallBack()
{
    return nullptr;
}

ObjectState FinjinSceneSettingsObject::Eval(TimeValue time)
{
    return ObjectState(this);
}

void FinjinSceneSettingsObject::InitNodeName(MSTR& s)
{
    s = _M("FinjinSceneSettings");
}

Class_ID FinjinSceneSettingsObject::ClassID() 
{
    return GetClassClassID();
}        

Class_ID FinjinSceneSettingsObject::GetClassClassID()
{
    return FinjinMaxClassID::SceneSettings;
}

SClass_ID FinjinSceneSettingsObject::SuperClassID() 
{ 
    return REF_TARGET_CLASS_ID;
}

void FinjinSceneSettingsObject::GetClassName(MSTR& s)
{
    s = MaxUtilities::GetStringM(IDS_SCENE_SETTINGS_OBJECT_CLASS_NAME);
}

RefTargetHandle FinjinSceneSettingsObject::Clone(RemapDir& remap)
{
    auto newHelper = new FinjinSceneSettingsObject();

    //Copy everything
    newHelper->ReplaceReference(PARAM_BLOCK_REF, remap.CloneRef(this->pblock));
        
    BaseClone(this, newHelper, remap);
    
    return newHelper;
}

RefResult FinjinSceneSettingsObject::NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate)
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
                case PB_DISALLOWED_MESHES:
                case PB_ADDITIONAL_MATERIALS: 
                case PB_DISALLOWED_MATERIALS: 
                {
                    this->pblock->Delete(lastNotifyID, tabIndex, 1);
                    break;
                }
            }

            break;
        }        
    }
    return REF_SUCCEED;
}

int FinjinSceneSettingsObject::NumSubs() 
{ 
    return NUM_REFS; 
}

MSTR FinjinSceneSettingsObject::SubAnimName(int i) 
{
    return MaxUtilities::GetString(IDS_PARAMETERS); 
}                

Animatable* FinjinSceneSettingsObject::SubAnim(int i) 
{
    return this->pblock; 
}

int FinjinSceneSettingsObject::NumRefs() 
{ 
    return NUM_REFS; 
}

RefTargetHandle FinjinSceneSettingsObject::GetReference(int i) 
{
    return this->pblock; 
}

void FinjinSceneSettingsObject::SetReference(int i, RefTargetHandle rtarg) 
{ 
    this->pblock = (IParamBlock2*)rtarg; 
}

ClassDesc* FinjinSceneSettingsObject::GetClassDesc()
{
    return &FinjinSceneSettingsObjectClassDesc::instance;
}

FINJIN_EXPORTER_IMPLEMENT_USER_DATA_OBJECT_METHODS(FinjinSceneSettingsObject)

FINJIN_EXPORTER_IMPLEMENT_EXTENDED_VALUE_OBJECT_METHODS(FinjinSceneSettingsObject)

wxString FinjinSceneSettingsObject::GetSceneManager()
{
    return this->pblock->GetStr(PB_SCENE_MANAGER);
}

float FinjinSceneSettingsObject::GetEnvironmentNear()
{
    return this->pblock->GetFloat(PB_ENVIRONMENT_NEAR);
}

bool FinjinSceneSettingsObject::GetExportEnvironment()
{
    return this->pblock->GetInt(PB_EXPORT_ENVIRONMENT) ? true : false;
}

Color FinjinSceneSettingsObject::GetAmbientLightColor()
{
    return this->pblock->GetColor(PB_AMBIENT_LIGHT_COLOR);
}

ValueSourceSync FinjinSceneSettingsObject::GetAmbientLightColorSync()
{
    return (ValueSourceSync)this->pblock->GetInt(PB_AMBIENT_LIGHT_COLOR_SYNC);
}

Color FinjinSceneSettingsObject::GetBackgroundColor()
{
    return this->pblock->GetColor(PB_BACKGROUND_COLOR);
}

ValueSourceSync FinjinSceneSettingsObject::GetBackgroundColorSync()
{
    return (ValueSourceSync)this->pblock->GetInt(PB_BACKGROUND_COLOR_SYNC);
}

float FinjinSceneSettingsObject::GetEnvironmentFar()
{
    return this->pblock->GetFloat(PB_ENVIRONMENT_FAR);
}

INode* FinjinSceneSettingsObject::GetEnvironmentFarObject()
{
    return this->pblock->GetINode(PB_ENVIRONMENT_FAR_OBJECT);
}

ValueSourceSync FinjinSceneSettingsObject::GetFogSync()
{
    return (ValueSourceSync)this->pblock->GetInt(PB_FOG_SYNC);
}

wxString FinjinSceneSettingsObject::GetFogMode()
{
    return this->pblock->GetStr(PB_FOG_MODE);
}

float FinjinSceneSettingsObject::GetFogDensity()
{
    return this->pblock->GetFloat(PB_FOG_DENSITY);
}

float FinjinSceneSettingsObject::GetFogStart()
{
    return this->pblock->GetFloat(PB_FOG_START);
}

float FinjinSceneSettingsObject::GetFogEnd()
{
    return this->pblock->GetFloat(PB_FOG_END);
}

Color FinjinSceneSettingsObject::GetFogColor()
{
    return this->pblock->GetColor(PB_FOG_COLOR);
}

wxString FinjinSceneSettingsObject::GetShadowTechnique()
{
    return this->pblock->GetStr(PB_SHADOW_TECHNIQUE);
}

Color FinjinSceneSettingsObject::GetShadowColor()
{
    return this->pblock->GetColor(PB_SHADOW_COLOR);
}

float FinjinSceneSettingsObject::GetShadowFarDistance()
{
    return this->pblock->GetFloat(PB_SHADOW_FAR_DISTANCE);
}

wxString FinjinSceneSettingsObject::GetSceneScaleUnit()
{
    return this->pblock->GetStr(PB_SCENE_SCALE_UNIT);
}

float FinjinSceneSettingsObject::GetSceneScaleDivide()
{
    return this->pblock->GetFloat(PB_SCENE_SCALE_DIVIDE);
}

bool FinjinSceneSettingsObject::GetExportHierarchy()
{
    return this->pblock->GetInt(PB_EXPORT_HIERARCHY) ? true : false;
}

bool FinjinSceneSettingsObject::GetIgnoreHiddenObjects()
{
    return this->pblock->GetInt(PB_IGNORE_HIDDEN_OBJECTS) ? true : false;
}

double FinjinSceneSettingsObject::GetNodeAnimationSampleInterval()
{
    return this->pblock->GetFloat(PB_NODE_ANIMATION_SAMPLE_INTERVAL);
}

SampleType FinjinSceneSettingsObject::GetNodeAnimationSampleType()
{
    return (SampleType)this->pblock->GetInt(PB_NODE_ANIMATION_SAMPLE_TYPE);
}

wxString FinjinSceneSettingsObject::GetConfigurationTargetName()
{
    return this->pblock->GetStr(PB_CONFIGURATION_TARGET_NAME);
}

double FinjinSceneSettingsObject::GetSkeletonAnimationSampleInterval()
{
    return this->pblock->GetFloat(PB_SKELETON_ANIMATION_SAMPLE_INTERVAL);
}

SampleType FinjinSceneSettingsObject::GetSkeletonAnimationSampleType()
{
    return (SampleType)this->pblock->GetInt(PB_SKELETON_ANIMATION_SAMPLE_TYPE);
}

double FinjinSceneSettingsObject::GetVertexAnimationSampleInterval()
{
    return this->pblock->GetFloat(PB_VERTEX_ANIMATION_SAMPLE_INTERVAL);
}

SampleType FinjinSceneSettingsObject::GetVertexAnimationSampleType()
{
    return (SampleType)this->pblock->GetInt(PB_VERTEX_ANIMATION_SAMPLE_TYPE);
}

bool FinjinSceneSettingsObject::GetExportMeshes()
{
    return this->pblock->GetInt(PB_EXPORT_MESHES) ? true : false;
}

int FinjinSceneSettingsObject::GetDisallowedMeshCount()
{
    return this->pblock->Count(PB_DISALLOWED_MESHES);
}

INode* FinjinSceneSettingsObject::GetDisallowedMesh(int index)
{
    return this->pblock->GetINode(PB_DISALLOWED_MESHES, 0, index);
}

void FinjinSceneSettingsObject::ClearDisallowedMeshes()
{
    this->pblock->ZeroCount(PB_DISALLOWED_MESHES);
}

bool FinjinSceneSettingsObject::GetExportVertexColors()
{
    return this->pblock->GetInt(PB_EXPORT_VERTEX_COLORS) ? true : false;
}

wxString FinjinSceneSettingsObject::GetSubmeshNaming()
{
    return this->pblock->GetStr(PB_SUBMESH_NAMING);
}

wxString FinjinSceneSettingsObject::GetSubmeshCustomName()
{    
    return this->pblock->GetStr(PB_SUBMESH_CUSTOM_NAME);
}

bool FinjinSceneSettingsObject::GetConvertBitmapsToTextures()
{
    return this->pblock->GetInt(PB_CONVERT_BITMAPS_TO_TEXTURES) ? true : false;
}

bool FinjinSceneSettingsObject::GetExportTangents()
{
    return this->pblock->GetInt(PB_EXPORT_TANGENTS) ? true : false;
}

bool FinjinSceneSettingsObject::GetExportBinormals()
{
    return this->pblock->GetInt(PB_EXPORT_BINORMALS) ? true : false;
}

bool FinjinSceneSettingsObject::GetAnimateBoundingVolumes()
{
    return this->pblock->GetInt(PB_ANIMATE_BOUNDING_VOLUMES) ? true : false;
}

bool FinjinSceneSettingsObject::GetCheckMeshInstances()
{
    return this->pblock->GetInt(PB_CHECK_MESH_INSTANCES) ? true : false;
}

bool FinjinSceneSettingsObject::GetAlwaysCreateNewSubmeshesWhenMerging()
{
    return this->pblock->GetInt(PB_ALWAYS_CREATE_NEW_SUBMESHES_WHEN_MERGING) ? true : false;    
}

wxString FinjinSceneSettingsObject::GetVertexColorFormat()
{
    return this->pblock->GetStr(PB_VERTEX_COLOR_FORMAT);
}

bool FinjinSceneSettingsObject::GetUseExplicitVertexColor()
{
    return this->pblock->GetInt(PB_USE_EXPLICIT_VERTEX_COLOR) ? true : false;
}

bool FinjinSceneSettingsObject::GetExportMaterials()
{
    return this->pblock->GetInt(PB_EXPORT_MATERIALS) ? true : false;
}

int FinjinSceneSettingsObject::GetAdditionalMaterialCount()
{
    return this->pblock->Count(PB_ADDITIONAL_MATERIALS);
}

Mtl* FinjinSceneSettingsObject::GetAdditionalMaterial(int index)
{
    return this->pblock->GetMtl(PB_ADDITIONAL_MATERIALS, 0, index);
}

void FinjinSceneSettingsObject::ClearAdditionalMaterials()
{
    this->pblock->ZeroCount(PB_ADDITIONAL_MATERIALS);
}

int FinjinSceneSettingsObject::GetDisallowedMaterialCount()
{
    return this->pblock->Count(PB_DISALLOWED_MATERIALS);
}

Mtl* FinjinSceneSettingsObject::GetDisallowedMaterial(int index)
{
    return this->pblock->GetMtl(PB_DISALLOWED_MATERIALS, 0, index);
}

void FinjinSceneSettingsObject::ClearDisallowedMaterials()
{
    this->pblock->ZeroCount(PB_DISALLOWED_MATERIALS);
}

bool FinjinSceneSettingsObject::GetCreateExtraMaterials()
{
    return this->pblock->GetInt(PB_CREATE_EXTRA_MATERIALS) ? true : false;
}

bool FinjinSceneSettingsObject::GetIncludeParentMaterialName()
{
    return this->pblock->GetInt(PB_INCLUDE_PARENT_MATERIAL_NAME) ? true : false;
}

wxString FinjinSceneSettingsObject::GetNoMaterialName()
{
    return this->pblock->GetStr(PB_NO_MATERIAL_NAME);
}

wxString FinjinSceneSettingsObject::GetRenderSystem()
{
    return this->pblock->GetStr(PB_RENDER_SYSTEM);
}

int FinjinSceneSettingsObject::GetRenameBitmapCount()
{
    return this->pblock->Count(PB_RENAME_BITMAPS_FROM);
}

void FinjinSceneSettingsObject::GetRenameBitmap(int index, wxString& from, wxString& to)
{
    from = this->pblock->GetStr(PB_RENAME_BITMAPS_FROM, 0, index);
    to = this->pblock->GetStr(PB_RENAME_BITMAPS_TO, 0, index);
}

void FinjinSceneSettingsObject::ClearRenameBitmaps()
{
    this->pblock->ZeroCount(PB_RENAME_BITMAPS_FROM);
    this->pblock->ZeroCount(PB_RENAME_BITMAPS_TO);
}

int FinjinSceneSettingsObject::GetRenameBitmapExtensionCount()
{
    return this->pblock->Count(PB_RENAME_BITMAP_EXTENSIONS_FROM);
}

void FinjinSceneSettingsObject::GetRenameBitmapExtension(int index, wxString& from, wxString& to)
{
    from = this->pblock->GetStr(PB_RENAME_BITMAP_EXTENSIONS_FROM, 0, index);
    to = this->pblock->GetStr(PB_RENAME_BITMAP_EXTENSIONS_TO, 0, index);
}

void FinjinSceneSettingsObject::ClearRenameBitmapExtensions()
{
    this->pblock->ZeroCount(PB_RENAME_BITMAP_EXTENSIONS_FROM);
    this->pblock->ZeroCount(PB_RENAME_BITMAP_EXTENSIONS_TO);
}

bool FinjinSceneSettingsObject::GetExportFlags()
{
    return this->pblock->GetInt(PB_EXPORT_FLAGS) ? true : false;
}

int FinjinSceneSettingsObject::GetFlagCount()
{
    return this->pblock->Count(PB_FLAG_NAMES);
}

void FinjinSceneSettingsObject::GetFlag(int index, wxString& name)
{
    name = this->pblock->GetStr(PB_FLAG_NAMES, 0, index);    
}

wxString FinjinSceneSettingsObject::GetQueryFlagName(int index)
{
    return this->pblock->GetStr(PB_FLAG_NAMES, 0, index);
}

void FinjinSceneSettingsObject::GetFlagNames(std::set<wxString>& names)
{
    wxString name;
    int count = GetFlagCount();
    for (int i = 0; i < count; i++)
    {
        name = this->pblock->GetStr(PB_FLAG_NAMES, 0, i);
        names.insert(name);
    }
}

void FinjinSceneSettingsObject::ClearFlags()
{
    this->pblock->ZeroCount(PB_FLAG_NAMES);    
}

void FinjinSceneSettingsObject::SetSceneManager(wxString value)
{
    this->pblock->SetValue(PB_SCENE_MANAGER, 0, WxStringToApplicationStringM(value));
}

void FinjinSceneSettingsObject::SetEnvironmentNear(float value)
{
    this->pblock->SetValue(PB_ENVIRONMENT_NEAR, 0, value);
}

void FinjinSceneSettingsObject::SetExportEnvironment(bool value)
{
    this->pblock->SetValue(PB_EXPORT_ENVIRONMENT, 0, (int)value);
}

void FinjinSceneSettingsObject::SetAmbientLightColor(Color value)
{
    this->pblock->SetValue(PB_AMBIENT_LIGHT_COLOR, 0, value);
}

void FinjinSceneSettingsObject::SetAmbientLightColorSync(ValueSourceSync value)
{
    this->pblock->SetValue(PB_AMBIENT_LIGHT_COLOR_SYNC, 0, (int)value);
}

void FinjinSceneSettingsObject::SetBackgroundColor(Color value)
{
    this->pblock->SetValue(PB_BACKGROUND_COLOR, 0, value);    
}

void FinjinSceneSettingsObject::SetBackgroundColorSync(ValueSourceSync value)
{
    this->pblock->SetValue(PB_BACKGROUND_COLOR_SYNC, 0, (int)value);
}

void FinjinSceneSettingsObject::SetEnvironmentFar(float value)
{
    this->pblock->SetValue(PB_ENVIRONMENT_FAR, 0, value);
}

void FinjinSceneSettingsObject::SetEnvironmentFarObject(INode* value)
{
    this->pblock->SetValue(PB_ENVIRONMENT_FAR_OBJECT, 0, value);
}

void FinjinSceneSettingsObject::SetFogSync(ValueSourceSync value)
{
    this->pblock->SetValue(PB_FOG_SYNC, 0, (int)value);
}

void FinjinSceneSettingsObject::SetFogMode(wxString value)
{
    this->pblock->SetValue(PB_FOG_MODE, 0, WxStringToApplicationStringM(value));
}

void FinjinSceneSettingsObject::SetFogDensity(float value)
{
    this->pblock->SetValue(PB_FOG_DENSITY, 0, value);
}

void FinjinSceneSettingsObject::SetFogStart(float value)
{
    this->pblock->SetValue(PB_FOG_START, 0, value);
}

void FinjinSceneSettingsObject::SetFogEnd(float value)
{
    this->pblock->SetValue(PB_FOG_END, 0, value);
}

void FinjinSceneSettingsObject::SetFogColor(Color value)
{
    this->pblock->SetValue(PB_FOG_COLOR, 0, value);
}

void FinjinSceneSettingsObject::SetShadowTechnique(wxString value)
{
    this->pblock->SetValue(PB_SHADOW_TECHNIQUE, 0, WxStringToApplicationStringM(value));
}

void FinjinSceneSettingsObject::SetShadowColor(Color value)
{
    this->pblock->SetValue(PB_SHADOW_COLOR, 0, value);
}

void FinjinSceneSettingsObject::SetShadowFarDistance(float value)
{
    this->pblock->SetValue(PB_SHADOW_FAR_DISTANCE, 0, value);
}

void FinjinSceneSettingsObject::SetSceneScaleUnit(wxString value)
{
    this->pblock->SetValue(PB_SCENE_SCALE_UNIT, 0, WxStringToApplicationStringM(value));
}

void FinjinSceneSettingsObject::SetSceneScaleDivide(float value)
{
    this->pblock->SetValue(PB_SCENE_SCALE_DIVIDE, 0, value);
}

void FinjinSceneSettingsObject::SetExportHierarchy(bool value)
{
    this->pblock->SetValue(PB_EXPORT_HIERARCHY, 0, (int)value);
}

void FinjinSceneSettingsObject::SetIgnoreHiddenObjects(bool value)
{
    this->pblock->SetValue(PB_IGNORE_HIDDEN_OBJECTS, 0, (int)value);
}

void FinjinSceneSettingsObject::SetNodeAnimationSampleInterval(double value)
{
    this->pblock->SetValue(PB_NODE_ANIMATION_SAMPLE_INTERVAL, 0, (float)value);
}

void FinjinSceneSettingsObject::SetNodeAnimationSampleType(SampleType value)
{
    this->pblock->SetValue(PB_NODE_ANIMATION_SAMPLE_TYPE, 0, (int)value);
}

void FinjinSceneSettingsObject::SetConfigurationTargetName(wxString value)
{
    this->pblock->SetValue(PB_CONFIGURATION_TARGET_NAME, 0, WxStringToApplicationStringM(value));    
}

void FinjinSceneSettingsObject::SetSkeletonAnimationSampleInterval(double value)
{
    this->pblock->SetValue(PB_SKELETON_ANIMATION_SAMPLE_INTERVAL, 0, (float)value);
}

void FinjinSceneSettingsObject::SetSkeletonAnimationSampleType(SampleType value)
{
    this->pblock->SetValue(PB_SKELETON_ANIMATION_SAMPLE_TYPE, 0, (int)value);
}

void FinjinSceneSettingsObject::SetVertexAnimationSampleInterval(double value)
{
    this->pblock->SetValue(PB_VERTEX_ANIMATION_SAMPLE_INTERVAL, 0, (float)value);
}

void FinjinSceneSettingsObject::SetVertexAnimationSampleType(SampleType value)
{
    this->pblock->SetValue(PB_VERTEX_ANIMATION_SAMPLE_TYPE, 0, (int)value);
}

void FinjinSceneSettingsObject::SetExportMeshes(bool value)
{
    this->pblock->SetValue(PB_EXPORT_MESHES, 0, (int)value);
}

void FinjinSceneSettingsObject::AddDisallowedMesh(INode* value)
{
    INode* values[1] = {value};
    this->pblock->Append(PB_DISALLOWED_MESHES, 1, values);
}

void FinjinSceneSettingsObject::SetSubmeshNaming(wxString value)
{
    this->pblock->SetValue(PB_SUBMESH_NAMING, 0, WxStringToApplicationStringM(value));
}

void FinjinSceneSettingsObject::SetSubmeshCustomName(wxString value)
{    
    this->pblock->SetValue(PB_SUBMESH_CUSTOM_NAME, 0, WxStringToApplicationStringM(value));
}

void FinjinSceneSettingsObject::SetExportVertexColors(bool value)
{
    this->pblock->SetValue(PB_EXPORT_VERTEX_COLORS, 0, (int)value);
}

void FinjinSceneSettingsObject::SetConvertBitmapsToTextures(bool value)
{
    this->pblock->SetValue(PB_CONVERT_BITMAPS_TO_TEXTURES, 0, (int)value);
}

void FinjinSceneSettingsObject::SetExportTangents(bool value)
{
    this->pblock->SetValue(PB_EXPORT_TANGENTS, 0, (int)value);
}

void FinjinSceneSettingsObject::SetExportBinormals(bool value)
{
    this->pblock->SetValue(PB_EXPORT_BINORMALS, 0, (int)value);
}

void FinjinSceneSettingsObject::SetAnimateBoundingVolumes(bool value)
{
    this->pblock->SetValue(PB_ANIMATE_BOUNDING_VOLUMES, 0, (int)value);
}

void FinjinSceneSettingsObject::SetCheckMeshInstances(bool value)
{
    this->pblock->SetValue(PB_CHECK_MESH_INSTANCES, 0, (int)value);
}

void FinjinSceneSettingsObject::SetAlwaysCreateNewSubmeshesWhenMerging(bool value)
{
    this->pblock->SetValue(PB_ALWAYS_CREATE_NEW_SUBMESHES_WHEN_MERGING, 0, (int)value);    
}

void FinjinSceneSettingsObject::SetVertexColorFormat(wxString value)
{
    this->pblock->SetValue(PB_VERTEX_COLOR_FORMAT, 0, WxStringToApplicationStringM(value));
}

void FinjinSceneSettingsObject::SetUseExplicitVertexColor(bool value)
{
    this->pblock->SetValue(PB_USE_EXPLICIT_VERTEX_COLOR, 0, value);
}

void FinjinSceneSettingsObject::SetExportMaterials(bool value)
{
    this->pblock->SetValue(PB_EXPORT_MATERIALS, 0, (int)value);
}
    
void FinjinSceneSettingsObject::AddAdditionalMaterial(Mtl* value)
{
    Mtl* mtls[1] = {value};
    this->pblock->Append(PB_ADDITIONAL_MATERIALS, 1, mtls);
}

void FinjinSceneSettingsObject::AddDisallowedMaterial(Mtl* value)
{
    Mtl* mtls[1] = {value};
    this->pblock->Append(PB_DISALLOWED_MATERIALS, 1, mtls);
}

void FinjinSceneSettingsObject::SetCreateExtraMaterials(bool value)
{
    this->pblock->SetValue(PB_CREATE_EXTRA_MATERIALS, 0, (int)value);
}

void FinjinSceneSettingsObject::SetIncludeParentMaterialName(bool value)
{
    this->pblock->SetValue(PB_INCLUDE_PARENT_MATERIAL_NAME, 0, (int)value);
}

void FinjinSceneSettingsObject::SetNoMaterialName(wxString value)
{
    this->pblock->SetValue(PB_NO_MATERIAL_NAME, 0, WxStringToApplicationStringM(value));
}

void FinjinSceneSettingsObject::SetRenderSystem(wxString value)
{
    this->pblock->SetValue(PB_RENDER_SYSTEM, 0, WxStringToApplicationStringM(value));
}

void FinjinSceneSettingsObject::AddRenameBitmap(wxString from, wxString to)
{
    auto maxFrom = WxStringToApplicationStringM(from);
    const MCHAR* froms[1] = {maxFrom};
    this->pblock->Append(PB_RENAME_BITMAPS_FROM, 1, froms);

    auto maxTo = WxStringToApplicationStringM(to);
    const MCHAR* tos[1] = {maxTo};
    this->pblock->Append(PB_RENAME_BITMAPS_TO, 1, tos);
}

void FinjinSceneSettingsObject::AddRenameBitmapExtension(wxString from, wxString to)
{
    auto maxFrom = WxStringToApplicationStringM(from);
    const MCHAR* froms[1] = {maxFrom};
    this->pblock->Append(PB_RENAME_BITMAP_EXTENSIONS_FROM, 1, froms);

    auto maxTo = WxStringToApplicationStringM(to);
    const MCHAR* tos[1] = {maxTo};
    this->pblock->Append(PB_RENAME_BITMAP_EXTENSIONS_TO, 1, tos);
}

void FinjinSceneSettingsObject::SetExportFlags(bool value)
{
    this->pblock->SetValue(PB_EXPORT_FLAGS, 0, (int)value);
}

void FinjinSceneSettingsObject::AddFlag(wxString name)
{
    auto maxName = WxStringToApplicationStringM(name);
    const MCHAR* names[1] = {maxName};
    this->pblock->Append(PB_FLAG_NAMES, 1, names);
}

FinjinSceneSettingsObject* FinjinSceneSettingsObject::GetSceneSettingsByIndex(int index, INode** settingsNode)
{
    //Check starting the last index and moving up to the end of the root node's children
    INode* rootNode = GetCOREInterface()->GetRootNode();
    int numChildNodes = rootNode->NumberOfChildren();
    for (int i = 0; i < numChildNodes; i++)
    {
        auto childNode = rootNode->GetChildNode(i);
        auto object = childNode->GetObjectRef();
        if (object->ClassID() == GetClassClassID())
        {
            //Found scene settings
            if (index-- == 0)
            {
                if (settingsNode != nullptr)
                    *settingsNode = childNode;
                return (FinjinSceneSettingsObject*)object;
            }
        }
    }

    return nullptr;
}

FinjinSceneSettingsObject* FinjinSceneSettingsObject::GetSceneSettings(bool createIfNecessary, bool* newInstance)
{
    //The last known index of the node containing the scene settings object 
    //This is used so that the entire collection of root nodes needn't be checked when 
    //retrieving the one and only scene settings object
    static int lastIndex = 0;

    if (newInstance != nullptr)
        *newInstance = false;
            
    //Try to get the existing object

    //Check starting the last index and moving up to the end of the root node's children
    auto rootNode = GetCOREInterface()->GetRootNode();
    int numChildNodes = rootNode->NumberOfChildren();
    for (int i = lastIndex; i < numChildNodes; i++)
    {
        auto object = rootNode->GetChildNode(i)->GetObjectRef();
        if (object->ClassID() == GetClassClassID())
        {
            //Found the scene settings
            lastIndex = i;
            return (FinjinSceneSettingsObject*)object;
        }
    }

    //If we're here, the settings weren't found. See if the settings were made a child of some other object
    auto maybeSceneSettingsNode = GetCOREInterface()->GetINodeByName(_M("FinjinSceneSettings01"));
    if (maybeSceneSettingsNode != nullptr)
    {
        auto object = maybeSceneSettingsNode->GetObjectRef();
        if (object->ClassID() == GetClassClassID())
        {
            //Found the scene settings

            //Detach from parent and return
            maybeSceneSettingsNode->Detach(0);
            return (FinjinSceneSettingsObject*)object;
        }
    }

    //If we're here, the settings weren't found. Check from the beginning to the last index
    lastIndex = std::min(lastIndex, numChildNodes);
    for (int i = 0; i < lastIndex; i++)
    {
        auto object = rootNode->GetChildNode(i)->GetObjectRef();
        if (object->ClassID() == GetClassClassID())
        {
            //Found the scene settings
            lastIndex = i;
            return (FinjinSceneSettingsObject*)object;
        }
    }

    //If we're here, no settings were found
    FinjinSceneSettingsObject* sceneSettingsObject = nullptr;
    if (createIfNecessary)
    {
        //There's no scene settings object. Create it and upgrade the current scene to use it
        sceneSettingsObject = new FinjinSceneSettingsObject;
        auto node = GetCOREInterface()->CreateObjectNode(sceneSettingsObject);
        node->Hide(TRUE);
        if (newInstance != nullptr)
            *newInstance = true;

        //The node goes to the end of the root node's child node collection
        lastIndex = numChildNodes;
    }
    return sceneSettingsObject;
}

void FinjinSceneSettingsObject::MergeObjectSettings(FinjinSceneSettingsObject* settings)
{
    int objectCount = std::min(settings->pblock->Count(PB_NODES), settings->pblock->Count(PB_OBJECTS));
    for (int i = 0; i < objectCount; i++)
    {
        auto objectSettings = (FinjinObjectSettingsObject*)settings->pblock->GetReferenceTarget(PB_OBJECTS, 0, i);
        auto node = (INode*)settings->pblock->GetReferenceTarget(PB_NODES, 0, i);

        //Copy settings
        if (objectSettings != nullptr && node != nullptr)
            GetObjectSettings(node)->CopyFrom(objectSettings);
    }        
}

void FinjinSceneSettingsObject::CleanObjectSettings()
{
    int objectCount = std::min(this->pblock->Count(PB_NODES), this->pblock->Count(PB_OBJECTS));
    for (int i = 0; i < objectCount;)
    {
        auto objectSettings = (FinjinObjectSettingsObject*)this->pblock->GetReferenceTarget(PB_OBJECTS, 0, i);
        auto node = (INode*)this->pblock->GetReferenceTarget(PB_NODES, 0, i);

        if (objectSettings != nullptr && node == nullptr)
        {
            //There are object settings, but no object. Remove the settings and the slot that held the object reference
            this->pblock->Delete(PB_NODES, i, 1);
            this->pblock->Delete(PB_OBJECTS, i, 1);
            objectCount--;
        }
        else
            i++;
    }
}

FinjinObjectSettingsObject* FinjinSceneSettingsObject::GetObjectSettings(INode* node, bool createIfNecessary, bool* newInstance)
{
    if (newInstance != nullptr)
        *newInstance = false;
    
    FinjinObjectSettingsObject* objectSettingsObject = nullptr;

    int objectCount = std::min(this->pblock->Count(PB_NODES), this->pblock->Count(PB_OBJECTS));
    for (int i = 0; i < objectCount; i++)
    {
        auto objectSettings = (FinjinObjectSettingsObject*)this->pblock->GetReferenceTarget(PB_OBJECTS, 0, i);

        if (objectSettings != nullptr && this->pblock->GetINode(PB_NODES, 0, i) == node)
        {
            //Found object settings
            objectSettingsObject = objectSettings;
            break;
        }        
    }        

    if (objectSettingsObject == nullptr && createIfNecessary)
    {
        //There's no object settings object. Create it and upgrade the node to use it
        objectSettingsObject = new FinjinObjectSettingsObject;
        
        INode* nodes[1] = {node};
        this->pblock->Append(PB_NODES, 1, nodes);
        
        ReferenceTarget* objectSettingsObjects[1] = {objectSettingsObject};
        this->pblock->Append(PB_OBJECTS, 1, objectSettingsObjects);

        if (newInstance != nullptr)
            *newInstance = true;
    }

    return objectSettingsObject;
}

void FinjinSceneSettingsObject::RemoveObjectSettings(INode* node)
{
    int objectCount = std::min(this->pblock->Count(PB_NODES), this->pblock->Count(PB_OBJECTS));
    for (int i = 0; i < objectCount; i++)
    {
        auto objectSettings = (FinjinObjectSettingsObject*)this->pblock->GetReferenceTarget(PB_OBJECTS, 0, i);

        if (objectSettings != nullptr && this->pblock->GetINode(PB_NODES, 0, i) == node)
        {
            this->pblock->Delete(PB_NODES, i, 1);
            this->pblock->Delete(PB_OBJECTS, i, 1);
            break;
        }
    }
}
