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
#include "FinjinStaticGeometry.hpp"
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
class FinjinStaticGeometryClassDesc : public ClassDesc2
{
public:
    int IsPublic() override                      {return TRUE;}
    void* Create(BOOL loading = FALSE) override  {return new FinjinStaticGeometry();}
    const MCHAR* ClassName() override            {return MaxUtilities::GetStringM(IDS_STATIC_GEOMETRY_CLASS_NAME);}
    SClass_ID SuperClassID() override            {return HELPER_CLASS_ID;}
    Class_ID ClassID() override                  {return FinjinStaticGeometry::GetClassClassID();}
    const MCHAR* Category() override             {return MaxUtilities::GetStringM(IDS_PLUGINS_CATEGORY);}
    const MCHAR* InternalName() override         {return _M("FinjinStaticGeometry");}
    HINSTANCE HInstance() override               {return wxGetInstance();}

public:
    static FinjinStaticGeometryClassDesc instance;
};
FinjinStaticGeometryClassDesc FinjinStaticGeometryClassDesc::instance;

class FinjinStaticGeometryDlgProc : public ParamMap2UserDlgProc
{
public:
    FinjinStaticGeometryDlgProc() {}

    void DeleteThis() override {}

    void SetThing(ReferenceTarget* m) 
    {
        this->helper = (FinjinStaticGeometry*)m;
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
    FinjinStaticGeometry* helper;
    INode* node;

    static FinjinStaticGeometryDlgProc instance;
};
FinjinStaticGeometryDlgProc FinjinStaticGeometryDlgProc::instance;

class FinjinStaticGeometryCreateCallback : public CornerCreateMouseCallback
{
public:
    FinjinStaticGeometryCreateCallback()
    {
        this->object = nullptr;
    }

    void SetObject(FinjinStaticGeometry* obj) 
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
    FinjinStaticGeometry* object;

public:    
    static FinjinStaticGeometryCreateCallback instance;
};
FinjinStaticGeometryCreateCallback FinjinStaticGeometryCreateCallback::instance;

static std::vector<Point3> DEFAULT_ICON_EDGE_VERTICES;


//Globals----------------------------------------------------------------------
enum {CUSTOM_HELPER_PARAM_BLOCK_ID = 0};
enum {CUSTOM_HELPER_VERSION = 1};
enum {MAIN_MAP, NUM_MAPS};

static ParamBlockDesc2 FinjinStaticGeometryParamBlock
    (
    CUSTOM_HELPER_PARAM_BLOCK_ID, _M("Parameters"),  0, &FinjinStaticGeometryClassDesc::instance, P_VERSION | P_AUTO_CONSTRUCT | P_AUTO_UI | P_MULTIMAP, 
    
    //Version
    CUSTOM_HELPER_VERSION,
    
    //Reference number
    FinjinStaticGeometry::PARAM_BLOCK_REF, 
    
    //Rollout
    NUM_MAPS,
    MAIN_MAP, IDD_STATIC_GEOMETRY_ROLLUP, IDS_PARAMETERS, 0, 0, &FinjinStaticGeometryDlgProc::instance,
    
    FinjinStaticGeometry::PB_WIDTH, _M("Width"), TYPE_FLOAT, P_RESET_DEFAULT, IDS_WIDTH,
        p_default, 1.0f,
        p_range, 0.05f, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE,
        p_ui, MAIN_MAP, TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_WIDTH_EDIT, IDC_WIDTH_SPIN, .1f,
        p_end,    
    FinjinStaticGeometry::PB_LENGTH, _M("Length"), TYPE_FLOAT, P_RESET_DEFAULT, IDS_LENGTH,
        p_default, 1.0f,
        p_range, 0.05f, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE,
        p_ui, MAIN_MAP, TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_LENGTH_EDIT, IDC_LENGTH_SPIN, .1f,
        p_end,
    FinjinStaticGeometry::PB_HEIGHT, _M("Height"), TYPE_FLOAT, P_RESET_DEFAULT, IDS_HEIGHT,
        p_default, 1.0f,
        p_range, 0.05f, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE,
        p_ui, MAIN_MAP, TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_HEIGHT_EDIT, IDC_HEIGHT_SPIN, .1f,
        p_end,
    FinjinStaticGeometry::PB_CAST_SHADOWS, _M("CastShadows"), TYPE_BOOL, P_RESET_DEFAULT, IDS_CAST_SHADOWS,
        p_default, TRUE,
        p_ui, MAIN_MAP, TYPE_SINGLECHEKBOX, IDC_CAST_SHADOWS_SCHECK,
        p_end,
    FinjinStaticGeometry::PB_RENDERING_DISTANCE_UNUSED, _M("RenderingDistance"), TYPE_FLOAT, P_RESET_DEFAULT, IDS_RENDERING_DISTANCE,
        p_default, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE,
        p_end,
    p_end
    );


//Implementation---------------------------------------------------------------
FinjinStaticGeometry::FinjinStaticGeometry()
{
    this->pblock = nullptr;
    this->suspendSnap = false;
    FinjinStaticGeometryClassDesc::instance.MakeAutoParamBlocks(this);

    if (DEFAULT_ICON_EDGE_VERTICES.empty())
        MeshIconUtilities::MakeBoxEdges(DEFAULT_ICON_EDGE_VERTICES, 1, 1, 1, nullptr);
}

FinjinStaticGeometry::~FinjinStaticGeometry()
{
}

int FinjinStaticGeometry::HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2* p, ViewExp* vpt)
{
    FinjinStaticGeometryDlgProc::instance.SetNode(inode);

    return MeshIconUtilities::HitTestCustomIcon(vpt, GetEdgeVertices(), GetSizeMatrix(t), GetAlwaysFaceView(), t, inode, type, crossing, flags, p);
}

void FinjinStaticGeometry::Snap(TimeValue t, INode* inode, SnapInfo* snap, IPoint2* p, ViewExp* vpt)
{
    FinjinStaticGeometryDlgProc::instance.SetNode(inode);

    if (this->suspendSnap)
        return;

    SnapUtilities::DefaultSnap(t, inode, snap, p, vpt, GetSizeMatrix(t));
}

int FinjinStaticGeometry::Display(TimeValue t, INode* inode, ViewExp* vpt, int flags)
{
    FinjinStaticGeometryDlgProc::instance.SetNode(inode);

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

CreateMouseCallBack* FinjinStaticGeometry::GetCreateMouseCallBack()
{
    FinjinStaticGeometryCreateCallback::instance.SetObject(this);
    return &FinjinStaticGeometryCreateCallback::instance;
}

void FinjinStaticGeometry::BeginEditParams(IObjParam* ip, ULONG flags, Animatable* prev)
{
    this->ip = ip;

    FinjinStaticGeometryDlgProc::instance.SetThing(this);

    FinjinStaticGeometryClassDesc::instance.BeginEditParams(ip, this, flags, prev);
}

void FinjinStaticGeometry::EndEditParams(IObjParam* ip, ULONG flags, Animatable* next)
{
    FinjinStaticGeometryClassDesc::instance.EndEditParams(ip, this, flags, next);

    this->ip = nullptr;
}

ObjectState FinjinStaticGeometry::Eval(TimeValue time)
{
    return ObjectState(this);
}

void FinjinStaticGeometry::InitNodeName(MSTR& s)
{
    s = _M("StaticGeometry");
}

void FinjinStaticGeometry::GetWorldBoundBox(TimeValue t, INode* inode, ViewExp* vpt, Box3& box)
{
    FinjinStaticGeometryDlgProc::instance.SetNode(inode);

    Matrix3 tm = GetSizeMatrix(t) * inode->GetObjectTM(t);
    if (GetAlwaysFaceView())
        tm = MeshIconUtilities::MakeBillboardTM(tm, vpt);

    auto& edgeVertices = GetEdgeVertices();
    box = MeshIconUtilities::GetCustomIconBox(&edgeVertices[0], edgeVertices.size(), &tm);
    box.EnlargeBy(.01f);
}

void FinjinStaticGeometry::GetLocalBoundBox(TimeValue t, INode* inode, ViewExp* vpt, Box3& box)
{
    FinjinStaticGeometryDlgProc::instance.SetNode(inode);

    Matrix3 tm = GetSizeMatrix(t);
    auto& edgeVertices = GetEdgeVertices();
    box = MeshIconUtilities::GetCustomIconBox(&edgeVertices[0], edgeVertices.size(), &tm);    
    box.EnlargeBy(.01f);
}

BOOL FinjinStaticGeometry::GetAlwaysFaceView()
{
    return FALSE;
}

float FinjinStaticGeometry::GetWidth(TimeValue t)
{
    return this->pblock->GetFloat(PB_WIDTH, t);
}

void FinjinStaticGeometry::SetWidth(float value, TimeValue t)
{
    this->pblock->SetValue(PB_WIDTH, t, value);
}

float FinjinStaticGeometry::GetLength(TimeValue t)
{
    return this->pblock->GetFloat(PB_LENGTH, t);
}

void FinjinStaticGeometry::SetLength(float value, TimeValue t)
{
    this->pblock->SetValue(PB_LENGTH, t, value);
}

float FinjinStaticGeometry::GetHeight(TimeValue t)
{
    return this->pblock->GetFloat(PB_HEIGHT, t);
}

void FinjinStaticGeometry::SetHeight(float value, TimeValue t)
{
    this->pblock->SetValue(PB_HEIGHT, t, value);
}

bool FinjinStaticGeometry::GetCastShadows(TimeValue t)
{
    return this->pblock->GetInt(PB_CAST_SHADOWS) ? true : false;
}

Point3 FinjinStaticGeometry::GetSize(TimeValue t)
{
    return Point3(GetWidth(t), GetLength(t), GetHeight(t));
}

Matrix3 FinjinStaticGeometry::GetSizeMatrix(TimeValue t)
{
    return ScaleMatrix(GetSize(t));
}

std::vector<Point3>& FinjinStaticGeometry::GetEdgeVertices()
{
    return DEFAULT_ICON_EDGE_VERTICES;
}

Class_ID FinjinStaticGeometry::ClassID() 
{
    return GetClassClassID();
}        

Class_ID FinjinStaticGeometry::GetClassClassID()
{
    return FinjinMaxClassID::StaticGeometry;
}

SClass_ID FinjinStaticGeometry::SuperClassID() 
{ 
    return HELPER_CLASS_ID; 
}

void FinjinStaticGeometry::GetClassName(MSTR& s)
{
    s = MaxUtilities::GetStringM(IDS_STATIC_GEOMETRY_CLASS_NAME);
}

RefTargetHandle FinjinStaticGeometry::Clone(RemapDir& remap)
{
    FinjinStaticGeometry* newHelper = new FinjinStaticGeometry();

    //Copy everything
    newHelper->ReplaceReference(PARAM_BLOCK_REF, remap.CloneRef(this->pblock));
        
    BaseClone(this, newHelper, remap);
    
    return newHelper;
}

RefResult FinjinStaticGeometry::NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate)
{
    return REF_SUCCEED;
}

int FinjinStaticGeometry::NumSubs() 
{ 
    return NUM_REFS; 
}

MSTR FinjinStaticGeometry::SubAnimName(int i) 
{
    return MaxUtilities::GetString(IDS_PARAMETERS); 
}                

Animatable* FinjinStaticGeometry::SubAnim(int i) 
{
    return this->pblock; 
}

int FinjinStaticGeometry::NumRefs() 
{ 
    return NUM_REFS; 
}

RefTargetHandle FinjinStaticGeometry::GetReference(int i) 
{
    return this->pblock; 
}

void FinjinStaticGeometry::SetReference(int i, RefTargetHandle rtarg) 
{ 
    this->pblock = (IParamBlock2*)rtarg; 
}

ClassDesc* FinjinStaticGeometry::GetClassDesc()
{
    return &FinjinStaticGeometryClassDesc::instance;
}
