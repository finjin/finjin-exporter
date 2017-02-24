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
#include "FinjinCubeMapHelper.hpp"

#if defined(FINJIN_EXPORTER_ENABLE_D3D_FUNCTIONALITY)

#include "MaxUtilities.hpp"
#include "FileUtilities.hpp"
#include "MeshIconUtilities.hpp"
#include "SnapUtilities.hpp"
#include "MaxControlUtilities.hpp"
#include "CubeMapRenderer.hpp"
#include "CenteredCreateMouseCallback.hpp"
#include "wxAdoptedWindow.hpp"
#include "Strings.hpp"
#include "FinjinMaxClassID.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Local classes----------------------------------------------------------------
class FinjinCubeMapHelperClassDesc : public ClassDesc2
{
public:
    int IsPublic() override                      {return TRUE;}
    void* Create(BOOL loading = FALSE) override  {return new FinjinCubeMapHelper();}
    const MCHAR* ClassName() override            {return MaxUtilities::GetStringM(IDS_CUBE_MAP_HELPER_CLASS_NAME);}
    SClass_ID SuperClassID() override            {return HELPER_CLASS_ID;}
    Class_ID ClassID() override                  {return FinjinCubeMapHelper::GetClassClassID();}
    const MCHAR* Category() override             {return MaxUtilities::GetStringM(IDS_PLUGINS_CATEGORY);}
    const MCHAR* InternalName() override         {return _M("FinjinCubeMapHelper");}
    HINSTANCE HInstance() override               {return wxGetInstance();}

public:
    static FinjinCubeMapHelperClassDesc instance;
};
FinjinCubeMapHelperClassDesc FinjinCubeMapHelperClassDesc::instance;

class FinjinCubeMapHelperDlgProc : public ParamMap2UserDlgProc
{
public:
    FinjinCubeMapHelperDlgProc() {}

    void DeleteThis() override {}

    void SetThing(ReferenceTarget* m) 
    {
        this->helper = (FinjinCubeMapHelper*)m;
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

                switch (id)
                {
                    case IDC_RENDER_NOW_BUTTON:
                    {
                        wxString baseFileName = this->helper->GetBaseFileName();
                        if (baseFileName.empty())
                            MaxControlUtilities::ShowMessage(MaxUtilities::GetString(IDS_BASE_FILE_NAME_REQUIRED), MaxUtilities::GetString2(IDS_DATA_ENTRY_ERROR), MB_ICONINFORMATION | MB_OK);
                        else
                        {
                            wxAdoptedWindow window(hWnd);
                            wxDirDialog dialog(&window, Strings::CHOOSE_DIRECTORY, wxEmptyString);
                            if (dialog.ShowModal() == wxID_OK)
                            {
                                auto directory = dialog.GetPath();
                                wxString fileName = FileUtilities::JoinPath(directory, baseFileName);
                                FileUtilities::CreateFileDirectoryRecursive(fileName);

                                CubeMapRenderer renderer;
                                renderer.SetCenterNode(this->node);                                
                                renderer.RenderToFile(fileName, this->helper->GetCubeFaceSize(), this->helper->GetRenderToSeparateFiles());
                            }
                        }
                        
                        break;
                    }
                }
            }
        }

        return 0;
    }

public:
    FinjinCubeMapHelper* helper;
    INode* node;

    static FinjinCubeMapHelperDlgProc instance;
};
FinjinCubeMapHelperDlgProc FinjinCubeMapHelperDlgProc::instance;

class FinjinCubeMapHelperCreateCallback : public CenteredCreateMouseCallback
{
public:
    FinjinCubeMapHelperCreateCallback()
    {
        this->object = nullptr;
    }

    void SetObject(FinjinCubeMapHelper* obj) 
    {
        this->object = obj;
    }
    
protected:
    void SetCreationSize(float size)
    {
        this->object->SetIconSize(size);
    }

    void SuspendSnap(bool suspend)
    {
        this->object->suspendSnap = suspend;
    }    

private:
    FinjinCubeMapHelper* object;

public:    
    static FinjinCubeMapHelperCreateCallback instance;
};
FinjinCubeMapHelperCreateCallback FinjinCubeMapHelperCreateCallback::instance;

static std::vector<Point3> DEFAULT_ICON_EDGE_VERTICES;


//Globals----------------------------------------------------------------------
enum {CUSTOM_HELPER_PARAM_BLOCK_ID = 0};
enum {CUSTOM_HELPER_VERSION = 1};
enum {MAIN_MAP, NUM_MAPS};

static ParamBlockDesc2 FinjinCubeMapHelperParamBlock
    (
    CUSTOM_HELPER_PARAM_BLOCK_ID, _M("Parameters"),  0, &FinjinCubeMapHelperClassDesc::instance, P_VERSION | P_AUTO_CONSTRUCT | P_AUTO_UI | P_MULTIMAP, 
    
    //Version
    CUSTOM_HELPER_VERSION,
    
    //Reference number
    FinjinCubeMapHelper::PARAM_BLOCK_REF, 
    
    //Rollout
    NUM_MAPS,
    MAIN_MAP, IDD_CUBE_MAP_HELPER_ROLLUP, IDS_PARAMETERS, 0, 0, &FinjinCubeMapHelperDlgProc::instance,
    
    FinjinCubeMapHelper::PB_ICON_SIZE, _M("IconSize"), TYPE_FLOAT, P_RESET_DEFAULT, IDS_ICON_SIZE,
        p_default, 1.0f,
        p_range, 0.05f, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE,
        p_ui, MAIN_MAP, TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_SIZE_EDIT, IDC_SIZE_SPIN, 0.01f,
        p_end,
    FinjinCubeMapHelper::PB_RENDER_ON_EXPORT, _M("RenderOnExport"), TYPE_BOOL, P_RESET_DEFAULT, IDS_RENDER_ON_EXPORT,
        p_default, TRUE,
        p_ui, MAIN_MAP, TYPE_SINGLECHEKBOX, IDC_RENDER_ON_EXPORT_CHECK,
        p_end,
    FinjinCubeMapHelper::PB_CUBE_FACE_SIZE, _M("CubeFaceSize"), TYPE_INT, P_RESET_DEFAULT, IDS_CUBE_FACE_SIZE,
        p_default, 512,
        p_range, 1, 4096,
        p_ui, MAIN_MAP, TYPE_SPINNER, EDITTYPE_INT, IDC_CUBE_FACE_SIZE_EDIT, IDC_CUBE_FACE_SIZE_SPIN, 1.0f,
        p_end,    
    FinjinCubeMapHelper::PB_FILE_TYPE, _M("FileType"), TYPE_INT, P_RESET_DEFAULT, IDS_FILE_TYPE,
        p_default, 0,    
        p_range, 0, 1,
        p_ui, MAIN_MAP, TYPE_RADIO, 2, IDC_SEPARATE_FILES_RADIO, IDC_DDS_FILE_RADIO,
        p_end,
    FinjinCubeMapHelper::PB_BASE_FILE_NAME, _M("BaseFileName"), TYPE_STRING, P_RESET_DEFAULT, IDS_TEXT,
        p_ui, MAIN_MAP, TYPE_EDITBOX, IDC_BASE_FILE_NAME_EDIT,
        p_end,
    p_end
    );


//Implementation---------------------------------------------------------------
FinjinCubeMapHelper::FinjinCubeMapHelper()
{
    this->pblock = nullptr;
    this->suspendSnap = false;
    FinjinCubeMapHelperClassDesc::instance.MakeAutoParamBlocks(this);

    if (DEFAULT_ICON_EDGE_VERTICES.empty())
        MeshIconUtilities::MakeBoxEdges(DEFAULT_ICON_EDGE_VERTICES, 1, 1, 1, nullptr);
}

FinjinCubeMapHelper::~FinjinCubeMapHelper()
{
}

int FinjinCubeMapHelper::HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2* p, ViewExp* vpt)
{
    FinjinCubeMapHelperDlgProc::instance.SetNode(inode);

    return MeshIconUtilities::HitTestCustomIcon(vpt, GetEdgeVertices(), GetIconSizeMatrix(t), GetAlwaysFaceView(), t, inode, type, crossing, flags, p);
}

void FinjinCubeMapHelper::Snap(TimeValue t, INode* inode, SnapInfo* snap, IPoint2* p, ViewExp* vpt)
{
    FinjinCubeMapHelperDlgProc::instance.SetNode(inode);

    if (this->suspendSnap)
        return;

    SnapUtilities::DefaultSnap(t, inode, snap, p, vpt, GetIconSizeMatrix(t));
}

int FinjinCubeMapHelper::Display(TimeValue t, INode* inode, ViewExp* vpt, int flags)
{
    FinjinCubeMapHelperDlgProc::instance.SetNode(inode);

    //Get viewport graphics window
    auto gw = vpt->getGW();    

    //Get transform
    auto tm = GetIconSizeMatrix(t) * inode->GetObjectTM(t);
    if (GetAlwaysFaceView())
        tm = MeshIconUtilities::MakeBillboardTM(tm, vpt);
    
    //Draw the icon
    auto& edgeVertices = GetEdgeVertices();
    MeshIconUtilities::DrawCustomIcon(vpt, &edgeVertices[0], edgeVertices.size(), nullptr, &tm, inode->Selected(), inode->IsFrozen(), inode->GetWireColor());
        
    return 0;
}

CreateMouseCallBack* FinjinCubeMapHelper::GetCreateMouseCallBack()
{
    FinjinCubeMapHelperCreateCallback::instance.SetObject(this);
    return &FinjinCubeMapHelperCreateCallback::instance;
}

void FinjinCubeMapHelper::BeginEditParams(IObjParam* ip, ULONG flags, Animatable* prev)
{
    this->ip = ip;

    FinjinCubeMapHelperDlgProc::instance.SetThing(this);

    FinjinCubeMapHelperClassDesc::instance.BeginEditParams(ip, this, flags, prev);
}

void FinjinCubeMapHelper::EndEditParams(IObjParam* ip, ULONG flags, Animatable* next)
{
    FinjinCubeMapHelperClassDesc::instance.EndEditParams(ip, this, flags, next);

    this->ip = nullptr;
}

ObjectState FinjinCubeMapHelper::Eval(TimeValue time)
{
    return ObjectState(this);
}

void FinjinCubeMapHelper::InitNodeName(MSTR& s)
{
    s = _M("CubeMap");
}

void FinjinCubeMapHelper::GetWorldBoundBox(TimeValue t, INode* inode, ViewExp* vpt, Box3& box)
{
    FinjinCubeMapHelperDlgProc::instance.SetNode(inode);

    auto tm = GetIconSizeMatrix(t) * inode->GetObjectTM(t);
    if (GetAlwaysFaceView())
        tm = MeshIconUtilities::MakeBillboardTM(tm, vpt);

    auto& edgeVertices = GetEdgeVertices();
    box = MeshIconUtilities::GetCustomIconBox(&edgeVertices[0], edgeVertices.size(), &tm);
    box.EnlargeBy(.01f);
}

void FinjinCubeMapHelper::GetLocalBoundBox(TimeValue t, INode* inode, ViewExp* vpt, Box3& box)
{
    FinjinCubeMapHelperDlgProc::instance.SetNode(inode);

    auto tm = GetIconSizeMatrix(t);
    auto& edgeVertices = GetEdgeVertices();
    box = MeshIconUtilities::GetCustomIconBox(&edgeVertices[0], edgeVertices.size(), &tm);    
    box.EnlargeBy(.01f);
}

float FinjinCubeMapHelper::GetIconSize(TimeValue t)
{
    return this->pblock->GetFloat(PB_ICON_SIZE, t);
}

void FinjinCubeMapHelper::SetIconSize(float value, TimeValue t)
{
    this->pblock->SetValue(PB_ICON_SIZE, t, value);
}

BOOL FinjinCubeMapHelper::GetAlwaysFaceView()
{
    return TRUE;
}

int FinjinCubeMapHelper::GetCubeFaceSize(TimeValue t)
{
    return this->pblock->GetInt(PB_CUBE_FACE_SIZE, t);
}

void FinjinCubeMapHelper::SetCubeFaceSize(int value, TimeValue t)
{
    this->pblock->SetValue(PB_CUBE_FACE_SIZE, t, value);
}

bool FinjinCubeMapHelper::GetRenderToSeparateFiles()
{
    return this->pblock->GetInt(PB_FILE_TYPE) == 0;
}

bool FinjinCubeMapHelper::GetRenderToDDSFile()
{
    return this->pblock->GetInt(PB_FILE_TYPE) == 1;
}

wxString FinjinCubeMapHelper::GetBaseFileName(TimeValue t)
{
    return this->pblock->GetStr(PB_BASE_FILE_NAME, t);
}

void FinjinCubeMapHelper::SetBaseFileName(wxString value, TimeValue t)
{
    this->pblock->SetValue(PB_BASE_FILE_NAME, t, WxStringToApplicationStringM(value));
}

bool FinjinCubeMapHelper::GetRenderOnExport(TimeValue t)
{
    return this->pblock->GetInt(PB_RENDER_ON_EXPORT) ? true : false;
}

Matrix3 FinjinCubeMapHelper::GetIconSizeMatrix(TimeValue t)
{
    float size = GetIconSize(t);
    return ScaleMatrix(Point3(size, size, size));
}

std::vector<Point3>& FinjinCubeMapHelper::GetEdgeVertices()
{
    return DEFAULT_ICON_EDGE_VERTICES;
}

Class_ID FinjinCubeMapHelper::ClassID() 
{
    return GetClassClassID();
}        

Class_ID FinjinCubeMapHelper::GetClassClassID()
{
    return FinjinMaxClassID::CubeMapHelper;
}

SClass_ID FinjinCubeMapHelper::SuperClassID() 
{ 
    return HELPER_CLASS_ID; 
}

void FinjinCubeMapHelper::GetClassName(MSTR& s)
{
    s = MaxUtilities::GetStringM(IDS_CUBE_MAP_HELPER_CLASS_NAME);
}

RefTargetHandle FinjinCubeMapHelper::Clone(RemapDir& remap)
{
    FinjinCubeMapHelper* newHelper = new FinjinCubeMapHelper();

    //Copy everything
    newHelper->ReplaceReference(PARAM_BLOCK_REF, remap.CloneRef(this->pblock));
        
    BaseClone(this, newHelper, remap);
    
    return newHelper;
}

RefResult FinjinCubeMapHelper::NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate)
{
    return REF_SUCCEED;
}

int FinjinCubeMapHelper::NumSubs() 
{ 
    return NUM_REFS; 
}

MSTR FinjinCubeMapHelper::SubAnimName(int i) 
{
    return MaxUtilities::GetString(IDS_PARAMETERS); 
}                

Animatable* FinjinCubeMapHelper::SubAnim(int i) 
{
    return this->pblock; 
}

int FinjinCubeMapHelper::NumRefs() 
{ 
    return NUM_REFS; 
}

RefTargetHandle FinjinCubeMapHelper::GetReference(int i) 
{
    return this->pblock; 
}

void FinjinCubeMapHelper::SetReference(int i, RefTargetHandle rtarg) 
{ 
    this->pblock = (IParamBlock2*)rtarg; 
}

ClassDesc* FinjinCubeMapHelper::GetClassDesc()
{
    return &FinjinCubeMapHelperClassDesc::instance;
}

#endif
