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
#include "FinjinCustomDummy.hpp"
#include "MaxUtilities.hpp"
#include "MeshIconUtilities.hpp"
#include "SnapUtilities.hpp"
#include "CenteredCreateMouseCallback.hpp"
#include "Strings.hpp"
#include "wxAdoptedWindow.hpp"
#include "CustomHelperIconReaderWriter.hpp"
#include "FinjinMaxClassID.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Local classes----------------------------------------------------------------
class FinjinCustomDummyClassDesc : public ClassDesc2
{
public:
    int IsPublic() override                      {return TRUE;}
    void* Create(BOOL loading = FALSE) override  {return new FinjinCustomDummy();}
    const MCHAR* ClassName() override            {return MaxUtilities::GetStringM(IDS_CUSTOM_DUMMY_CLASS_NAME);}
    SClass_ID SuperClassID() override            {return HELPER_CLASS_ID;}
    Class_ID ClassID() override                  {return FinjinCustomDummy::GetClassClassID();}
    const MCHAR* Category() override             {return MaxUtilities::GetStringM(IDS_PLUGINS_CATEGORY);}
    const MCHAR* InternalName() override         {return _M("FinjinDummy");}
    HINSTANCE HInstance() override               {return wxGetInstance();}

public:
    static FinjinCustomDummyClassDesc instance;
};
FinjinCustomDummyClassDesc FinjinCustomDummyClassDesc::instance;

class FinjinCustomDummyDlgProc : public ParamMap2UserDlgProc
{
public:
    FinjinCustomDummyDlgProc() {}

    void DeleteThis() override {}

    void SetThing(ReferenceTarget* m) 
    {
        this->dummy = (FinjinCustomDummy*)m;
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
                    case IDC_USE_CUSTOM_ICON_BUTTON:
                    {
                        wxAdoptedWindow window(hWnd);
                        wxFileDialog dialog(&window, Strings::CHOOSE_FILE, wxEmptyString, wxEmptyString, Strings::CUSTOM_HELPER_ICON_FILE_FILTER);
                        if (dialog.ShowModal() == wxID_OK)
                        {
                            wxString file = dialog.GetPath();
                            if (this->dummy->UseCustomIconFile(file))
                                GetCOREInterface()->ForceCompleteRedraw();
                        }

                        break;
                    }
                    case IDC_USE_DEFAULT_ICON_BUTTON:
                    {
                        if (this->dummy->UseDefaultIcon())                        
                            GetCOREInterface()->ForceCompleteRedraw();
                        
                        break;
                    }
                }
            }
        }

        return 0;
    }

public:
    FinjinCustomDummy* dummy;
    static FinjinCustomDummyDlgProc instance;
};
FinjinCustomDummyDlgProc FinjinCustomDummyDlgProc::instance;

class FinjinCustomDummyCreateCallback : public CenteredCreateMouseCallback
{
public:
    FinjinCustomDummyCreateCallback()
    {
        this->object = nullptr;
    }

    void SetObject(FinjinCustomDummy* obj) 
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
    FinjinCustomDummy* object;

public:    
    static FinjinCustomDummyCreateCallback instance;
};
FinjinCustomDummyCreateCallback FinjinCustomDummyCreateCallback::instance;

static std::vector<Point3> DEFAULT_ICON_EDGE_VERTICES;


//Globals----------------------------------------------------------------------
enum {CUSTOM_DUMMY_PARAM_BLOCK_ID = 0};
enum {CUSTOM_DUMMY_VERSION = 1};
enum {MAIN_MAP, NUM_MAPS};

static ParamBlockDesc2 FinjinCustomDummyParamBlock
    (
    CUSTOM_DUMMY_PARAM_BLOCK_ID, _M("Parameters"),  0, &FinjinCustomDummyClassDesc::instance, P_VERSION | P_AUTO_CONSTRUCT | P_AUTO_UI | P_MULTIMAP, 
    
    //Version
    CUSTOM_DUMMY_VERSION,
    
    //Reference number
    FinjinCustomDummy::PARAM_BLOCK_REF, 
    
    //Rollout
    NUM_MAPS,
    MAIN_MAP, IDD_CUSTOM_DUMMY_ROLLUP, IDS_PARAMETERS, 0, 0, &FinjinCustomDummyDlgProc::instance,
    
    FinjinCustomDummy::PB_ICON_SIZE, _M("IconSize"), TYPE_FLOAT, P_RESET_DEFAULT, IDS_ICON_SIZE,
        p_default, 1.0f,
        p_range, 0.05f, FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE,
        p_ui, MAIN_MAP, TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_SIZE_EDIT, IDC_SIZE_SPIN, 0.01f,
        p_end,
    FinjinCustomDummy::PB_ALWAYS_FACE_VIEW, _M("AlwaysFaceView"), TYPE_BOOL, P_RESET_DEFAULT, IDS_ALWAYS_FACE_VIEW,
        p_default, FALSE,
        p_ui, MAIN_MAP, TYPE_SINGLECHEKBOX, IDC_ALWAYS_FACE_VIEW_CHECK,
        p_end,
    FinjinCustomDummy::PB_TEXT, _M("Text"), TYPE_STRING, P_RESET_DEFAULT, IDS_TEXT,
        p_ui, MAIN_MAP, TYPE_EDITBOX, IDC_TEXT_EDIT,
        p_end,
    p_end
    );


//Implementation---------------------------------------------------------------
FinjinCustomDummy::FinjinCustomDummy()
{
    this->pblock = nullptr;
    this->suspendSnap = false;
    FinjinCustomDummyClassDesc::instance.MakeAutoParamBlocks(this);

    if (DEFAULT_ICON_EDGE_VERTICES.empty())
        MeshIconUtilities::MakeBoxEdges(DEFAULT_ICON_EDGE_VERTICES, 1, 1, 1, nullptr);
}

FinjinCustomDummy::~FinjinCustomDummy()
{
}

#define EDGE_VERTICES_CHUNK     0x0100

IOResult FinjinCustomDummy::Load(ILoad* iload)
{
    ULONG numBytes;
    IOResult res = IO_OK;

    while (IO_OK == (res = iload->OpenChunk())) 
    {
        switch (iload->CurChunkID()) 
        {
            case EDGE_VERTICES_CHUNK: 
            {
                //Load vertex count
                int vertexCount;
                iload->Read(&vertexCount, sizeof(vertexCount), &numBytes);
                this->edgeVertices.resize(vertexCount);

                //Load edge vertices
                iload->Read(&this->edgeVertices[0], sizeof(Point3) * vertexCount, &numBytes);

                break;
            }                
        }

        res = iload->CloseChunk();
        if (res != IO_OK)  
            return res;
    }

    return HelperObject::Load(iload);
}

IOResult FinjinCustomDummy::Save(ISave* isave)
{
    ULONG numBytes;

    if (!this->edgeVertices.empty())
    {
        isave->BeginChunk(EDGE_VERTICES_CHUNK);
        
        //Save vertex count
        int vertexCount = (int)this->edgeVertices.size();
        isave->Write(&vertexCount, sizeof(vertexCount), &numBytes);

        //Save edge vertices
        isave->Write(&this->edgeVertices[0], sizeof(Point3) * vertexCount, &numBytes);

        isave->EndChunk();
    }

    return HelperObject::Save(isave);
}

int FinjinCustomDummy::HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2* p, ViewExp* vpt)
{
    return MeshIconUtilities::HitTestCustomIcon(vpt, GetEdgeVertices(), GetIconSizeMatrix(t), GetAlwaysFaceView(t), t, inode, type, crossing, flags, p);
}

void FinjinCustomDummy::Snap(TimeValue t, INode* inode, SnapInfo* snap, IPoint2* p, ViewExp* vpt)
{
    if (this->suspendSnap)
        return;

    SnapUtilities::DefaultSnap(t, inode, snap, p, vpt, GetIconSizeMatrix(t));
}

int FinjinCustomDummy::Display(TimeValue t, INode* inode, ViewExp* vpt, int flags)
{
    //Get viewport graphics window
    auto gw = vpt->getGW();    

    //Get transform
    auto tm = GetIconSizeMatrix(t) * inode->GetObjectTM(t);
    if (GetAlwaysFaceView())
        tm = MeshIconUtilities::MakeBillboardTM(tm, vpt);
        
    //Draw the icon
    auto text = GetText();
    auto& edgeVertices = GetEdgeVertices();
    MeshIconUtilities::DrawCustomIcon(vpt, &edgeVertices[0], edgeVertices.size(), &text, &tm, inode->Selected(), inode->IsFrozen(), inode->GetWireColor());
    
    return 0;
}

CreateMouseCallBack* FinjinCustomDummy::GetCreateMouseCallBack()
{
    FinjinCustomDummyCreateCallback::instance.SetObject(this);
    return &FinjinCustomDummyCreateCallback::instance;
}

void FinjinCustomDummy::BeginEditParams(IObjParam* ip, ULONG flags, Animatable* prev)
{
    this->ip = ip;

    FinjinCustomDummyDlgProc::instance.SetThing(this);

    FinjinCustomDummyClassDesc::instance.BeginEditParams(ip, this, flags, prev);
}

void FinjinCustomDummy::EndEditParams(IObjParam* ip, ULONG flags, Animatable* next)
{
    FinjinCustomDummyClassDesc::instance.EndEditParams(ip, this, flags, next);

    this->ip = nullptr;
}

ObjectState FinjinCustomDummy::Eval(TimeValue time)
{
    return ObjectState(this);
}

void FinjinCustomDummy::InitNodeName(MSTR& s)
{
    s = _M("Dummy");
}

void FinjinCustomDummy::GetWorldBoundBox(TimeValue t, INode* inode, ViewExp* vpt, Box3& box)
{
    auto tm = GetIconSizeMatrix(t) * inode->GetObjectTM(t);
    if (GetAlwaysFaceView(t))
        tm = MeshIconUtilities::MakeBillboardTM(tm, vpt);

    auto& edgeVertices = GetEdgeVertices();
    box = MeshIconUtilities::GetCustomIconBox(&edgeVertices[0], edgeVertices.size(), &tm);
    box.EnlargeBy(.01f);
}

void FinjinCustomDummy::GetLocalBoundBox(TimeValue t, INode* inode, ViewExp* vpt, Box3& box)
{
    auto tm = GetIconSizeMatrix(t);
    auto& edgeVertices = GetEdgeVertices();
    box = MeshIconUtilities::GetCustomIconBox(&edgeVertices[0], edgeVertices.size(), &tm);    
    box.EnlargeBy(.01f);
}

float FinjinCustomDummy::GetIconSize(TimeValue t)
{
    return this->pblock->GetFloat(PB_ICON_SIZE, t);
}

void FinjinCustomDummy::SetIconSize(float value, TimeValue t)
{
    this->pblock->SetValue(PB_ICON_SIZE, t, value);
}

BOOL FinjinCustomDummy::GetAlwaysFaceView(TimeValue t)
{
    return this->pblock->GetInt(PB_ALWAYS_FACE_VIEW, t);
}

void FinjinCustomDummy::SetAlwaysFaceView(BOOL value, TimeValue t)
{
    this->pblock->SetValue(PB_ALWAYS_FACE_VIEW, t, value);
}

wxString FinjinCustomDummy::GetText(TimeValue t)
{
    return this->pblock->GetStr(PB_TEXT, t);
}

void FinjinCustomDummy::SetText(wxString value, TimeValue t)
{
    this->pblock->SetValue(PB_TEXT, t, WxStringToApplicationStringM(value));
}

Matrix3 FinjinCustomDummy::GetIconSizeMatrix(TimeValue t)
{
    float size = GetIconSize(t);
    return ScaleMatrix(Point3(size, size, size));
}

bool FinjinCustomDummy::UseDefaultIcon()
{
    bool result = !this->edgeVertices.empty();
    this->edgeVertices.clear();
    return result;
}

bool FinjinCustomDummy::UseCustomIconFile(wxString fileName)
{
    auto result = false;

    //Load file
    CustomHelperIconReader reader;
    if (reader.Read(fileName) && reader.vertices.size() % 2 == 0)
    {
        this->edgeVertices.resize(reader.vertices.size());
        for (size_t i = 0; i < reader.vertices.size(); i++)
            this->edgeVertices[i] = Point3(reader.vertices[i].x, reader.vertices[i].y, reader.vertices[i].z);
    }

    return result;
}

std::vector<Point3>& FinjinCustomDummy::GetEdgeVertices()
{
    if (!this->edgeVertices.empty())
        return this->edgeVertices;
    else
        return DEFAULT_ICON_EDGE_VERTICES;
}

Class_ID FinjinCustomDummy::ClassID() 
{
    return GetClassClassID();
}        

Class_ID FinjinCustomDummy::GetClassClassID()
{
    return FinjinMaxClassID::CustomDummy;
}

SClass_ID FinjinCustomDummy::SuperClassID() 
{ 
    return HELPER_CLASS_ID; 
}

void FinjinCustomDummy::GetClassName(MSTR& s)
{
    s = MaxUtilities::GetStringM(IDS_CUSTOM_DUMMY_CLASS_NAME);
}

RefTargetHandle FinjinCustomDummy::Clone(RemapDir& remap)
{
    FinjinCustomDummy* newHelper = new FinjinCustomDummy();

    //Copy everything
    newHelper->ReplaceReference(PARAM_BLOCK_REF, remap.CloneRef(this->pblock));
    newHelper->edgeVertices = this->edgeVertices;
    
    BaseClone(this, newHelper, remap);
    
    return newHelper;
}

RefResult FinjinCustomDummy::NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate)
{
    return REF_SUCCEED;
}

int FinjinCustomDummy::NumSubs() 
{ 
    return NUM_REFS; 
}

MSTR FinjinCustomDummy::SubAnimName(int i) 
{
    return MaxUtilities::GetString(IDS_PARAMETERS); 
}                

Animatable* FinjinCustomDummy::SubAnim(int i) 
{
    return this->pblock; 
}

int FinjinCustomDummy::NumRefs() 
{ 
    return NUM_REFS; 
}

RefTargetHandle FinjinCustomDummy::GetReference(int i) 
{
    return this->pblock; 
}

void FinjinCustomDummy::SetReference(int i, RefTargetHandle rtarg) 
{ 
    this->pblock = (IParamBlock2*)rtarg; 
}

ClassDesc* FinjinCustomDummy::GetClassDesc()
{
    return &FinjinCustomDummyClassDesc::instance;
}
