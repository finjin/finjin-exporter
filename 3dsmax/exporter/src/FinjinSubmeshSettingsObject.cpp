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
#include "FinjinSubmeshSettingsObject.hpp"
#include "MaxUtilities.hpp"
#include "FinjinMaxClassID.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Local types-------------------------------------------------------------------
class FinjinSubmeshSettingsObjectClassDesc : public ClassDesc2
{
public:
    int IsPublic() override                      {return FALSE;}
    void* Create(BOOL loading = FALSE) override  {return new FinjinSubmeshSettingsObject();}
    const MCHAR* ClassName() override            {return MaxUtilities::GetStringM(IDS_SUBMESH_SETTINGS_OBJECT_CLASS_NAME);}
    SClass_ID SuperClassID() override            {return REF_TARGET_CLASS_ID;}
    Class_ID ClassID() override                  {return FinjinSubmeshSettingsObject::GetClassClassID();}
    const MCHAR* Category() override             {return MaxUtilities::GetStringM(IDS_PLUGINS_CATEGORY);}
    const MCHAR* InternalName() override         {return _M("FinjinSubmeshSettingsObject");}
    HINSTANCE HInstance() override               {return wxGetInstance();}

public:
    static FinjinSubmeshSettingsObjectClassDesc instance;
};
FinjinSubmeshSettingsObjectClassDesc FinjinSubmeshSettingsObjectClassDesc::instance;


//Globals-----------------------------------------------------------------------
enum {SUBMESH_SETTINGS_PARAM_BLOCK_ID = 0};
enum {SUBMESH_SETTINGS_VERSION = 1};

static ParamBlockDesc2 FinjinSubmeshSettingsObjectParamBlock
    (
    SUBMESH_SETTINGS_PARAM_BLOCK_ID, _M("Parameters"),  0, &FinjinSubmeshSettingsObjectClassDesc::instance, P_VERSION | P_AUTO_CONSTRUCT,

    //Version
    SUBMESH_SETTINGS_VERSION,

    //Reference number
    FinjinSubmeshSettingsObject::PARAM_BLOCK_REF,

    //Parameters
    FinjinSubmeshSettingsObject::PB_SUBMESH_NAME, _M("SubmeshName"), TYPE_STRING, 0, IDS_SUBMESH_NAME,
        p_end,
    FinjinSubmeshSettingsObject::PB_MATERIAL, _M("Material"), TYPE_MTL, 0, IDS_MATERIAL,
        p_end,
    FinjinSubmeshSettingsObject::PB_TEXTURE_COORDINATE_SET_SOURCES, _M("TextureCoordinateSetSources"), TYPE_INT_TAB, 0, P_VARIABLE_SIZE, IDS_TEXTURE_COORDINATE_SET_SOURCES,
        p_end,
    FinjinSubmeshSettingsObject::PB_TEXTURE_COORDINATE_SET_DESTINATIONS, _M("TextureCoordinateSetDestinations"), TYPE_INT_TAB, 0, P_VARIABLE_SIZE, IDS_TEXTURE_COORDINATE_SET_DESTINATIONS,
        p_end,
    FinjinSubmeshSettingsObject::PB_TEXTURE_COORDINATE_SET_TYPES, _M("TextureCoordinateSetTypes"), TYPE_STRING_TAB, 0, P_VARIABLE_SIZE, IDS_TEXTURE_COORDINATE_SET_TYPES,
        p_end,
    FinjinSubmeshSettingsObject::PB_TEXTURE_ALIASES, _M("TextureAliases"), TYPE_STRING_TAB, 0, P_VARIABLE_SIZE, IDS_TEXTURE_ALIASES,
        p_end,
    FinjinSubmeshSettingsObject::PB_TEXTURE_NAMES, _M("TextureNames"), TYPE_STRING_TAB, 0, P_VARIABLE_SIZE, IDS_TEXTURE_NAMES,
        p_end,
    FinjinSubmeshSettingsObject::PB_RENDER_QUEUE_NAME, _M("RenderQueueName"), TYPE_STRING, 0, IDS_RENDER_QUEUE_NAME,
        p_end,
    FinjinSubmeshSettingsObject::PB_RENDER_QUEUE_ADD, _M("RenderQueueAdd"), TYPE_INT, 0, IDS_RENDER_QUEUE_ADD,
        p_end,
    FinjinSubmeshSettingsObject::PB_RENDER_QUEUE_PRIORITY, _M("RenderQueuePriority"), TYPE_INT, 0, IDS_RENDER_QUEUE_PRIORITY,
        p_end,
    p_end
    );


//Implementation----------------------------------------------------------------
FinjinSubmeshSettingsObject::FinjinSubmeshSettingsObject()
{
    this->pblock = nullptr;
    FinjinSubmeshSettingsObjectClassDesc::instance.MakeAutoParamBlocks(this);
}

FinjinSubmeshSettingsObject::~FinjinSubmeshSettingsObject()
{
}

CreateMouseCallBack* FinjinSubmeshSettingsObject::GetCreateMouseCallBack()
{
    return nullptr;
}

ObjectState FinjinSubmeshSettingsObject::Eval(TimeValue time)
{
    return ObjectState(this);
}

void FinjinSubmeshSettingsObject::InitNodeName(MSTR& s)
{
    s = _M("FinjinSubmeshSettings");
}

Class_ID FinjinSubmeshSettingsObject::ClassID()
{
    return GetClassClassID();
}

Class_ID FinjinSubmeshSettingsObject::GetClassClassID()
{
    return FinjinMaxClassID::SubmeshSettings;
}

SClass_ID FinjinSubmeshSettingsObject::SuperClassID()
{
    return REF_TARGET_CLASS_ID;
}

void FinjinSubmeshSettingsObject::GetClassName(MSTR& s)
{
    s = MaxUtilities::GetStringM(IDS_SUBMESH_SETTINGS_OBJECT_CLASS_NAME);
}

RefTargetHandle FinjinSubmeshSettingsObject::Clone(RemapDir& remap)
{
    FinjinSubmeshSettingsObject* newHelper = new FinjinSubmeshSettingsObject();

    //Copy everything
    newHelper->ReplaceReference(PARAM_BLOCK_REF, remap.CloneRef(this->pblock));

    BaseClone(this, newHelper, remap);

    return newHelper;
}

int FinjinSubmeshSettingsObject::NumSubs()
{
    return NUM_REFS;
}

MSTR FinjinSubmeshSettingsObject::SubAnimName(int i)
{
    return MaxUtilities::GetString(IDS_PARAMETERS);
}

Animatable* FinjinSubmeshSettingsObject::SubAnim(int i)
{
    return this->pblock;
}

RefResult FinjinSubmeshSettingsObject::NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate)
{
    return REF_SUCCEED;
}

int FinjinSubmeshSettingsObject::NumRefs()
{
    return NUM_REFS;
}

RefTargetHandle FinjinSubmeshSettingsObject::GetReference(int i)
{
    return this->pblock;
}

void FinjinSubmeshSettingsObject::SetReference(int i, RefTargetHandle rtarg)
{
    this->pblock = (IParamBlock2*)rtarg;
}

ClassDesc* FinjinSubmeshSettingsObject::GetClassDesc()
{
    return &FinjinSubmeshSettingsObjectClassDesc::instance;
}

void FinjinSubmeshSettingsObject::GetValue(SubmeshSettings& value)
{
    value.submeshName = ApplicationStringToWxString(this->pblock->GetStr(PB_SUBMESH_NAME));
    value.material = this->pblock->GetMtl(PB_MATERIAL);

    value.textureCoordinateSetMappings.resize(this->pblock->Count(PB_TEXTURE_COORDINATE_SET_SOURCES));
    for (size_t i = 0; i < value.textureCoordinateSetMappings.size(); i++)
    {
        value.textureCoordinateSetMappings[i].source.number = this->pblock->GetInt(PB_TEXTURE_COORDINATE_SET_SOURCES, 0, (int)i);
        value.textureCoordinateSetMappings[i].destinationIndex = this->pblock->GetInt(PB_TEXTURE_COORDINATE_SET_DESTINATIONS, 0, (int)i);
        value.textureCoordinateSetMappings[i].SetTextureCoordinateFlagsFromString(this->pblock->GetStr(PB_TEXTURE_COORDINATE_SET_TYPES, 0, (int)i));
    }

    value.textureNameAliases.resize(this->pblock->Count(PB_TEXTURE_ALIASES));
    for (size_t i = 0; i < value.textureNameAliases.size(); i++)
    {
        value.textureNameAliases[i].alias = this->pblock->GetStr(PB_TEXTURE_ALIASES, 0, (int)i);
        value.textureNameAliases[i].textureName = this->pblock->GetStr(PB_TEXTURE_NAMES, 0, (int)i);
    }

    value.renderQueueName = ApplicationStringToWxString(this->pblock->GetStr(PB_RENDER_QUEUE_NAME));
    value.renderPriority = this->pblock->GetInt(PB_RENDER_QUEUE_PRIORITY);
}

void FinjinSubmeshSettingsObject::SetValue(const SubmeshSettings& value)
{
    this->pblock->SetValue(PB_SUBMESH_NAME, 0, WxStringToApplicationStringM(value.submeshName));
    this->pblock->SetValue(PB_MATERIAL, 0, value.material.mtl);

    this->pblock->ZeroCount(PB_TEXTURE_COORDINATE_SET_SOURCES);
    this->pblock->ZeroCount(PB_TEXTURE_COORDINATE_SET_DESTINATIONS);
    this->pblock->ZeroCount(PB_TEXTURE_COORDINATE_SET_TYPES);
    TSTR type;
    for (size_t i = 0; i < value.textureCoordinateSetMappings.size(); i++)
    {
        int numbers[1] = {(int)value.textureCoordinateSetMappings[i].source.number};
        this->pblock->Append(PB_TEXTURE_COORDINATE_SET_SOURCES, 1, numbers);

        int destinations[1] = {(int)value.textureCoordinateSetMappings[i].destinationIndex};
        this->pblock->Append(PB_TEXTURE_COORDINATE_SET_DESTINATIONS, 1, destinations);

        type = WxStringToApplicationString(value.textureCoordinateSetMappings[i].GetTextureCoordinateFlagsString());
        const MCHAR* types[1] = {type.data()};
        this->pblock->Append(PB_TEXTURE_COORDINATE_SET_TYPES, 1, types);
    }

    this->pblock->ZeroCount(PB_TEXTURE_ALIASES);
    this->pblock->ZeroCount(PB_TEXTURE_NAMES);
    MSTR textureAlias;
    MSTR textureName;
    for (size_t i = 0; i < value.textureNameAliases.size(); i++)
    {
        textureAlias = WxStringToApplicationStringM(value.textureNameAliases[i].alias);
        const MCHAR* textureAliases[1] = {textureAlias.data()};
        this->pblock->Append(PB_TEXTURE_ALIASES, 1, textureAliases);

        textureName = WxStringToApplicationStringM(value.textureNameAliases[i].textureName);
        const MCHAR* textureNames[1] = {textureName.data()};
        this->pblock->Append(PB_TEXTURE_NAMES, 1, textureNames);
    }

    this->pblock->SetValue(PB_RENDER_QUEUE_NAME, 0, WxStringToApplicationStringM(value.renderQueueName));
    this->pblock->SetValue(PB_RENDER_QUEUE_PRIORITY, 0, value.renderPriority);
}
