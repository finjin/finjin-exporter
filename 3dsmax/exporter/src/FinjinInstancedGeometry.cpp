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
#include "FinjinInstancedGeometry.hpp"
#include "MaxUtilities.hpp"
#include "FileUtilities.hpp"
#include "MeshIconUtilities.hpp"
#include "SnapUtilities.hpp"
#include "MaxControlUtilities.hpp"
#include "CornerCreateMouseCallback.hpp"
#include "FinjinMaxClassID.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Local classes----------------------------------------------------------------
class FinjinInstancedGeometryClassDesc : public ClassDesc2
{
public:
    int IsPublic() override                      {return TRUE;}
    void* Create(BOOL loading = FALSE) override  {return new FinjinInstancedGeometry();}
    const MCHAR* ClassName() override            {return MaxUtilities::GetStringM(IDS_INSTANCED_GEOMETRY_CLASS_NAME);}
    SClass_ID SuperClassID() override            {return HELPER_CLASS_ID;}
    Class_ID ClassID() override                  {return FinjinInstancedGeometry::GetClassClassID();}
    const MCHAR* Category() override             {return MaxUtilities::GetStringM(IDS_PLUGINS_CATEGORY);}
    const MCHAR* InternalName() override         {return _M("FinjinInstancedGeometry");}
    HINSTANCE HInstance() override               {return wxGetInstance();}

public:
    static FinjinInstancedGeometryClassDesc instance;
};
FinjinInstancedGeometryClassDesc FinjinInstancedGeometryClassDesc::instance;

class FinjinInstancedGeometryDlgProc : public ParamMap2UserDlgProc
{
public:
    FinjinInstancedGeometryDlgProc() {}

    void DeleteThis() override {}

    void SetThing(ReferenceTarget* m) 
    {
        this->helper = (FinjinInstancedGeometry*)m;
        this->node = nullptr;
    }

    void SetNode(INode* node)
    {
        this->node = node;
    }

    INT_PTR DlgProc(TimeValue t, IParamMap2* map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
            case WM_COMMAND:
            {
                int id = LOWORD(wParam);
                int code = HIWORD(wParam);

                /*
                switch (id)
                {
                    case IDC_FLASH_CHILD_NODES_BUTTON:
                    {
                        INodeTab childNodes;
                        MaxUtilities::GetChildNodes(this->node, childNodes);
                        GetCOREInterface()->FlashNodes(&childNodes);
                        break;
                    }
                }
                */

                break;
            }
        }

        return 0;
    }

public:
    FinjinInstancedGeometry* helper;
    INode* node;

    static FinjinInstancedGeometryDlgProc instance;
};
FinjinInstancedGeometryDlgProc FinjinInstancedGeometryDlgProc::instance;

class FinjinInstancedGeometryCreateCallback : public CornerCreateMouseCallback
{
public:
    FinjinInstancedGeometryCreateCallback()
    {
        this->object = nullptr;
    }

    void SetObject(FinjinInstancedGeometry* obj) 
    {
        this->object = obj;
    }
    
protected:
    void SetWidth(float size)
    {
        this->object->SetWidth(size);
    }

    void SetLength(float size)
    {
        this->object->SetLength(size);
    }

    void SetHeight(float size)
    {
        this->object->SetHeight(size);
    }

    void SuspendSnap(bool suspend)
    {
        this->object->suspendSnap = suspend;
    }    

private:
    FinjinInstancedGeometry* object;

public:    
    static FinjinInstancedGeometryCreateCallback instance;
};
FinjinInstancedGeometryCreateCallback FinjinInstancedGeometryCreateCallback::instance;

static std::vector<Point3> DEFAULT_ICON_EDGE_VERTICES;


//Globals----------------------------------------------------------------------
enum {CUSTOM_HELPER_PARAM_BLOCK_ID = 0};
enum {CUSTOM_HELPER_VERSION = 1};
enum {MAIN_MAP, NUM_MAPS};

static ParamBlockDesc2 FinjinInstancedGeometryParamBlock
    (
    CUSTOM_HELPER_PARAM_BLOCK_ID, _M("Parameters"),  0, &FinjinInstancedGeometryClassDesc::instance, P_VERSION | P_AUTO_CONSTRUCT | P_AUTO_UI | P_MULTIMAP, 
    
    //Version
    CUSTOM_HELPER_VERSION,
    
    //Reference number
    FinjinInstancedGeometry::PARAM_BLOCK_REF, 
    
    //Rollout
    NUM_MAPS,
    MAIN_MAP, IDD_INSTANCED_GEOMETRY_ROLLUP, IDS_PARAMETERS, 0, 0, &FinjinInstancedGeometryDlgProc::instance,
    
    FinjinInstancedGeometry::PB_WIDTH, _M("Width"), TYPE_FLOAT, P_RESET_DEFAULT, IDS_WIDTH,
        p_default, 1.0f,
        p_range, 0.05f, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE,
        p_ui, MAIN_MAP, TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_WIDTH_EDIT, IDC_WIDTH_SPIN, .1f,
        p_end,    
    FinjinInstancedGeometry::PB_LENGTH, _M("Length"), TYPE_FLOAT, P_RESET_DEFAULT, IDS_LENGTH,
        p_default, 1.0f,
        p_range, 0.05f, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE,
        p_ui, MAIN_MAP, TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_LENGTH_EDIT, IDC_LENGTH_SPIN, .1f,
        p_end,
    FinjinInstancedGeometry::PB_HEIGHT, _M("Height"), TYPE_FLOAT, P_RESET_DEFAULT, IDS_HEIGHT,
        p_default, 1.0f,
        p_range, 0.05f, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE,
        p_ui, MAIN_MAP, TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_HEIGHT_EDIT, IDC_HEIGHT_SPIN, .1f,
        p_end,
    FinjinInstancedGeometry::PB_CAST_SHADOWS, _M("CastShadows"), TYPE_BOOL, P_RESET_DEFAULT, IDS_CAST_SHADOWS,
        p_default, TRUE,
        p_ui, MAIN_MAP, TYPE_SINGLECHEKBOX, IDC_CAST_SHADOWS_SCHECK,
        p_end,
    FinjinInstancedGeometry::PB_RENDERING_DISTANCE_UNUSED, _M("RenderingDistance"), TYPE_FLOAT, P_RESET_DEFAULT, IDS_RENDERING_DISTANCE,
        p_default, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE,
        p_end,
    FinjinInstancedGeometry::PB_BATCH_COUNT, _M("BatchCount"), TYPE_INT, P_RESET_DEFAULT, IDS_BATCH_COUNT,
        p_default, 1,
        p_range, 1, 100,
        p_ui, MAIN_MAP, TYPE_SPINNER, EDITTYPE_INT, IDC_BATCH_COUNT_EDIT, IDC_BATCH_COUNT_SPIN, 1.0f,
        p_end,
    p_end
    );


//Implementation---------------------------------------------------------------
FinjinInstancedGeometry::FinjinInstancedGeometry()
{
    this->pblock = nullptr;
    this->suspendSnap = false;
    FinjinInstancedGeometryClassDesc::instance.MakeAutoParamBlocks(this);

    if (DEFAULT_ICON_EDGE_VERTICES.empty())
        MeshIconUtilities::MakeBoxEdges(DEFAULT_ICON_EDGE_VERTICES, 1, 1, 1, nullptr);
}

FinjinInstancedGeometry::~FinjinInstancedGeometry()
{
}

int FinjinInstancedGeometry::HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2* p, ViewExp* vpt)
{
    FinjinInstancedGeometryDlgProc::instance.SetNode(inode);

    return MeshIconUtilities::HitTestCustomIcon(vpt, GetEdgeVertices(), GetSizeMatrix(t), GetAlwaysFaceView(), t, inode, type, crossing, flags, p);
}

void FinjinInstancedGeometry::Snap(TimeValue t, INode* inode, SnapInfo* snap, IPoint2* p, ViewExp* vpt)
{
    FinjinInstancedGeometryDlgProc::instance.SetNode(inode);

    if (this->suspendSnap)
        return;

    SnapUtilities::DefaultSnap(t, inode, snap, p, vpt, GetSizeMatrix(t));
}

int FinjinInstancedGeometry::Display(TimeValue t, INode* inode, ViewExp* vpt, int flags)
{
    FinjinInstancedGeometryDlgProc::instance.SetNode(inode);

    //Get viewport graphics window
    auto gw = vpt->getGW();    

    //Get transform
    Matrix3 tm = GetSizeMatrix(t) * inode->GetObjectTM(t);
    if (GetAlwaysFaceView())
        tm = MeshIconUtilities::MakeBillboardTM(tm, vpt);
    
    //Draw the icon
    auto& edgeVertices = GetEdgeVertices();
    MeshIconUtilities::DrawCustomIcon(vpt, &edgeVertices[0], edgeVertices.size(), nullptr, &tm, inode->Selected(), inode->IsFrozen(), inode->GetWireColor());
    
    return 0;
}

CreateMouseCallBack* FinjinInstancedGeometry::GetCreateMouseCallBack()
{
    FinjinInstancedGeometryCreateCallback::instance.SetObject(this);
    return &FinjinInstancedGeometryCreateCallback::instance;
}

void FinjinInstancedGeometry::BeginEditParams(IObjParam* ip, ULONG flags, Animatable* prev)
{
    this->ip = ip;

    FinjinInstancedGeometryDlgProc::instance.SetThing(this);

    FinjinInstancedGeometryClassDesc::instance.BeginEditParams(ip, this, flags, prev);
}

void FinjinInstancedGeometry::EndEditParams(IObjParam* ip, ULONG flags, Animatable* next)
{
    FinjinInstancedGeometryClassDesc::instance.EndEditParams(ip, this, flags, next);

    this->ip = nullptr;
}

ObjectState FinjinInstancedGeometry::Eval(TimeValue time)
{
    return ObjectState(this);
}

void FinjinInstancedGeometry::InitNodeName(MSTR& s)
{
    s = _M("InstancedGeometry");
}

void FinjinInstancedGeometry::GetWorldBoundBox(TimeValue t, INode* inode, ViewExp* vpt, Box3& box)
{
    FinjinInstancedGeometryDlgProc::instance.SetNode(inode);

    Matrix3 tm = GetSizeMatrix(t) * inode->GetObjectTM(t);
    if (GetAlwaysFaceView())
        tm = MeshIconUtilities::MakeBillboardTM(tm, vpt);

    auto& edgeVertices = GetEdgeVertices();
    box = MeshIconUtilities::GetCustomIconBox(&edgeVertices[0], edgeVertices.size(), &tm);
    box.EnlargeBy(.01f);
}

void FinjinInstancedGeometry::GetLocalBoundBox(TimeValue t, INode* inode, ViewExp* vpt, Box3& box)
{
    FinjinInstancedGeometryDlgProc::instance.SetNode(inode);

    Matrix3 tm = GetSizeMatrix(t);
    auto& edgeVertices = GetEdgeVertices();
    box = MeshIconUtilities::GetCustomIconBox(&edgeVertices[0], edgeVertices.size(), &tm);    
    box.EnlargeBy(.01f);
}

BOOL FinjinInstancedGeometry::GetAlwaysFaceView()
{
    return FALSE;
}

float FinjinInstancedGeometry::GetWidth(TimeValue t)
{
    return this->pblock->GetFloat(PB_WIDTH, t);
}

void FinjinInstancedGeometry::SetWidth(float value, TimeValue t)
{
    this->pblock->SetValue(PB_WIDTH, t, value);
}

float FinjinInstancedGeometry::GetLength(TimeValue t)
{
    return this->pblock->GetFloat(PB_LENGTH, t);
}

void FinjinInstancedGeometry::SetLength(float value, TimeValue t)
{
    this->pblock->SetValue(PB_LENGTH, t, value);
}

float FinjinInstancedGeometry::GetHeight(TimeValue t)
{
    return this->pblock->GetFloat(PB_HEIGHT, t);
}

void FinjinInstancedGeometry::SetHeight(float value, TimeValue t)
{
    this->pblock->SetValue(PB_HEIGHT, t, value);
}

bool FinjinInstancedGeometry::GetCastShadows(TimeValue t)
{
    return this->pblock->GetInt(PB_CAST_SHADOWS) ? true : false;
}

int FinjinInstancedGeometry::GetBatchCount(TimeValue t)
{
    return this->pblock->GetInt(PB_BATCH_COUNT, t);
}

Point3 FinjinInstancedGeometry::GetSize(TimeValue t)
{
    return Point3(GetWidth(t), GetLength(t), GetHeight(t));
}

Matrix3 FinjinInstancedGeometry::GetSizeMatrix(TimeValue t)
{
    return ScaleMatrix(GetSize(t));
}

std::vector<Point3>& FinjinInstancedGeometry::GetEdgeVertices()
{
    return DEFAULT_ICON_EDGE_VERTICES;
}

Class_ID FinjinInstancedGeometry::ClassID() 
{
    return GetClassClassID();
}        

Class_ID FinjinInstancedGeometry::GetClassClassID()
{
    return FinjinMaxClassID::InstancedGeometry;
}

SClass_ID FinjinInstancedGeometry::SuperClassID() 
{ 
    return HELPER_CLASS_ID; 
}

void FinjinInstancedGeometry::GetClassName(MSTR& s)
{
    s = MaxUtilities::GetStringM(IDS_INSTANCED_GEOMETRY_CLASS_NAME);
}

RefTargetHandle FinjinInstancedGeometry::Clone(RemapDir& remap)
{
    FinjinInstancedGeometry* newHelper = new FinjinInstancedGeometry();

    //Copy everything
    newHelper->ReplaceReference(PARAM_BLOCK_REF, remap.CloneRef(this->pblock));
        
    BaseClone(this, newHelper, remap);
    
    return newHelper;
}

RefResult FinjinInstancedGeometry::NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate)
{
    return REF_SUCCEED;
}

int FinjinInstancedGeometry::NumSubs() 
{ 
    return NUM_REFS; 
}

MSTR FinjinInstancedGeometry::SubAnimName(int i) 
{
    return MaxUtilities::GetString(IDS_PARAMETERS); 
}                

Animatable* FinjinInstancedGeometry::SubAnim(int i) 
{
    return this->pblock; 
}

int FinjinInstancedGeometry::NumRefs() 
{ 
    return NUM_REFS; 
}

RefTargetHandle FinjinInstancedGeometry::GetReference(int i) 
{
    return this->pblock; 
}

void FinjinInstancedGeometry::SetReference(int i, RefTargetHandle rtarg) 
{ 
    this->pblock = (IParamBlock2*)rtarg; 
}

ClassDesc* FinjinInstancedGeometry::GetClassDesc()
{
    return &FinjinInstancedGeometryClassDesc::instance;
}
