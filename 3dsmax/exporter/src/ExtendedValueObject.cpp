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
#include "ExtendedValueObject.hpp"
#include "ExtendedValue.hpp"
#include "MaxUtilities.hpp"
#include "FinjinMaxClassID.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Local classes----------------------------------------------------------------
class ExtendedValueObjectClassDesc : public ClassDesc2
{
public:
    int IsPublic() override                      {return FALSE;}
    void* Create(BOOL loading = FALSE) override  {return new ExtendedValueObject();}
    const MCHAR* ClassName() override            {return MaxUtilities::GetStringM(IDS_EXTENDED_VALUE_SETTINGS_OBJECT_CLASS_NAME);}
    SClass_ID SuperClassID() override            {return REF_TARGET_CLASS_ID;}
    Class_ID ClassID() override                  {return ExtendedValueObject::GetClassClassID();}
    const MCHAR* Category() override             {return MaxUtilities::GetStringM(IDS_PLUGINS_CATEGORY);}
    const MCHAR* InternalName() override         {return _M("ExtendedValueObject");}
    HINSTANCE HInstance() override               {return wxGetInstance();}

public:
    static ExtendedValueObjectClassDesc instance;
};
ExtendedValueObjectClassDesc ExtendedValueObjectClassDesc::instance;


//Globals----------------------------------------------------------------------
enum {EXTENDED_VALUE_SETTINGS_PARAM_BLOCK_ID = 0};
enum {EXTENDED_VALUE_SETTINGS_VERSION = 1};

static ParamBlockDesc2 ExtendedValueObjectParamBlock
    (
    EXTENDED_VALUE_SETTINGS_PARAM_BLOCK_ID, _M("Parameters"),  0, &ExtendedValueObjectClassDesc::instance, P_VERSION | P_AUTO_CONSTRUCT, 
    
    //Version
    EXTENDED_VALUE_SETTINGS_VERSION,
    
    //Reference number
    ExtendedValueObject::PARAM_BLOCK_REF, 
    
    //Parameters
    ExtendedValueObject::PB_ID, _M("ID"), TYPE_INT, 0, IDS_ID,
        p_end,
    ExtendedValueObject::PB_TYPE, _M("Type"), TYPE_STRING, 0, IDS_TYPE,
        p_end,
    ExtendedValueObject::PB_FLOATS, _M("Floats"), TYPE_FLOAT_TAB, 0, P_VARIABLE_SIZE, IDS_FLOATS,
        p_end,
    ExtendedValueObject::PB_STRINGS, _M("Strings"), TYPE_STRING_TAB, 0, P_VARIABLE_SIZE, IDS_STRINGS,
        p_end,
    ExtendedValueObject::PB_NODES, _M("Nodes"), TYPE_INODE_TAB, 0, P_VARIABLE_SIZE, IDS_NODES,
        p_end,
    ExtendedValueObject::PB_MATERIALS, _M("Materials"), TYPE_MTL_TAB, 0, P_VARIABLE_SIZE, IDS_MATERIALS,
        p_end,

    p_end
    );


//Implementation---------------------------------------------------------------
ExtendedValueObject::ExtendedValueObject()
{
    this->pblock = nullptr;
    ExtendedValueObjectClassDesc::instance.MakeAutoParamBlocks(this);
}

ExtendedValueObject::~ExtendedValueObject()
{    
}

CreateMouseCallBack* ExtendedValueObject::GetCreateMouseCallBack()
{
    return nullptr;
}

ObjectState ExtendedValueObject::Eval(TimeValue time)
{
    return ObjectState(this);
}

void ExtendedValueObject::InitNodeName(MSTR& s)
{
    s = _M("FinjinExtendedValue");
}

Class_ID ExtendedValueObject::ClassID() 
{
    return GetClassClassID();
}        

Class_ID ExtendedValueObject::GetClassClassID()
{
    return FinjinMaxClassID::ExtendedValueObject;
}

SClass_ID ExtendedValueObject::SuperClassID() 
{ 
    return REF_TARGET_CLASS_ID;
}

void ExtendedValueObject::GetClassName(MSTR& s)
{
    s = MaxUtilities::GetStringM(IDS_EXTENDED_VALUE_SETTINGS_OBJECT_CLASS_NAME);
}

RefTargetHandle ExtendedValueObject::Clone(RemapDir& remap)
{
    ExtendedValueObject* newHelper = new ExtendedValueObject();

    //Copy everything
    newHelper->ReplaceReference(PARAM_BLOCK_REF, remap.CloneRef(this->pblock));
        
    BaseClone(this, newHelper, remap);
    
    return newHelper;
}

int ExtendedValueObject::NumSubs() 
{ 
    return NUM_REFS; 
}

MSTR ExtendedValueObject::SubAnimName(int i) 
{
    return MaxUtilities::GetString(IDS_PARAMETERS); 
}                

Animatable* ExtendedValueObject::SubAnim(int i) 
{
    return this->pblock; 
}

RefResult ExtendedValueObject::NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate)
{
    return REF_SUCCEED;
}

int ExtendedValueObject::NumRefs() 
{ 
    return NUM_REFS; 
}

RefTargetHandle ExtendedValueObject::GetReference(int i) 
{
    return this->pblock; 
}

void ExtendedValueObject::SetReference(int i, RefTargetHandle rtarg) 
{ 
    this->pblock = (IParamBlock2*)rtarg; 
}

ClassDesc* ExtendedValueObject::GetClassDesc()
{
    return &ExtendedValueObjectClassDesc::instance;
}

int ExtendedValueObject::GetID()
{
    return this->pblock->GetInt(PB_ID);
}

void ExtendedValueObject::SetID(int id)
{
    this->pblock->SetValue(PB_ID, 0, id);
}

void ExtendedValueObject::GetValue(ExtendedValue& value)
{
    wxString typeString = ApplicationStringToWxString(this->pblock->GetStr(PB_TYPE));
    ExtendedValue::Type type = ExtendedValue::ParseType(typeString);                
    switch (type)
    {
        case ExtendedValue::EXTENDED_INT:
        case ExtendedValue::EXTENDED_FLOAT:
        {
            std::vector<float> floats;
            floats.resize(this->pblock->Count(PB_FLOATS));
            for (size_t i = 0; i < floats.size(); i++)
                floats[i] = this->pblock->GetFloat(PB_FLOATS, 0, (int)i);
            value.Set(floats);
            
            break;
        }
        case ExtendedValue::EXTENDED_COLOR:
        {
            std::vector<FinjinColor> colors;
            colors.resize(this->pblock->Count(PB_FLOATS) / 4);
            int floatIndex = 0;
            for (size_t i = 0; i < colors.size(); i++)
            {
                colors[i].r = this->pblock->GetFloat(PB_FLOATS, 0, floatIndex++);
                colors[i].g = this->pblock->GetFloat(PB_FLOATS, 0, floatIndex++);
                colors[i].b = this->pblock->GetFloat(PB_FLOATS, 0, floatIndex++);
                colors[i].a = this->pblock->GetFloat(PB_FLOATS, 0, floatIndex++);
            }
            value.Set(colors);
            
            break;
        }
        case ExtendedValue::EXTENDED_STRING:
        {
            std::vector<wxString> strings;
            strings.resize(this->pblock->Count(PB_STRINGS));
            for (size_t i = 0; i < strings.size(); i++)
                strings[i] = ApplicationStringToWxString(this->pblock->GetStr(PB_STRINGS, 0, (int)i));
            value.Set(strings);

            break;
        }
        case ExtendedValue::EXTENDED_OBJECT: 
        {
            std::vector<ObjectAccessor> objects;
            objects.resize(this->pblock->Count(PB_NODES));
            for (size_t i = 0; i < objects.size(); i++)
                objects[i] = this->pblock->GetINode(PB_NODES, 0, (int)i);
            value.Set(objects);

            break;
        }
        case ExtendedValue::EXTENDED_MATERIAL:
        {
            std::vector<MaterialAccessor> materials;
            materials.resize(this->pblock->Count(PB_MATERIALS));
            for (size_t i = 0; i < materials.size(); i++)
                materials[i] = this->pblock->GetMtl(PB_MATERIALS, 0, (int)i);
            value.Set(materials);

            break;
        }
        default: value.SetNone(); break;
    }    
}

void ExtendedValueObject::SetValue(const ExtendedValue& value)
{
    this->pblock->SetValue(PB_TYPE, 0, WxStringToApplicationStringM(value.GetTypeString()));

    switch (value.GetType())
    {
        case ExtendedValue::EXTENDED_INT:
        case ExtendedValue::EXTENDED_FLOAT:
        {
            this->pblock->ZeroCount(PB_FLOATS);

            std::vector<float> floats;
            value.Get(floats);
            if (!floats.empty())
                this->pblock->Append(PB_FLOATS, (int)floats.size(), &floats[0]);
            
            break;
        }
        case ExtendedValue::EXTENDED_COLOR:
        {
            this->pblock->ZeroCount(PB_FLOATS);

            std::vector<FinjinColor> colors;
            value.Get(colors);
            for (size_t i = 0; i < colors.size(); i++)
                this->pblock->Append(PB_FLOATS, 4, &colors[i].r);
            
            break;
        }
        case ExtendedValue::EXTENDED_STRING:
        {
            this->pblock->ZeroCount(PB_STRINGS);

            std::vector<wxString> strings;
            value.Get(strings);
            MSTR maxString;
            for (size_t i = 0; i < strings.size(); i++)
            {
                maxString = WxStringToApplicationStringM(strings[i]);
                const MCHAR* ss[1] = {maxString};
                this->pblock->Append(PB_STRINGS, 1, ss);
            }

            break;
        }
        case ExtendedValue::EXTENDED_OBJECT: 
        {
            this->pblock->ZeroCount(PB_NODES);

            std::vector<ObjectAccessor> objects;
            value.Get(objects);
            for (size_t i = 0; i < objects.size(); i++)
            {
                INode* nodes[1] = {objects[i].node};
                this->pblock->Append(PB_NODES, 1, nodes);
            }

            break;
        }
        case ExtendedValue::EXTENDED_MATERIAL:
        {
            this->pblock->ZeroCount(PB_MATERIALS);

            std::vector<MaterialAccessor> materials;
            value.Get(materials);
            for (size_t i = 0; i < materials.size(); i++)
            {
                Mtl* mtls[1] = {materials[i].mtl};
                this->pblock->Append(PB_MATERIALS, 1, mtls);
            }

            break;
        }        
    }    
}
