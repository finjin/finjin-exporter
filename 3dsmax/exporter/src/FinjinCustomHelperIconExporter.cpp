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
#include "FinjinCustomHelperIconExporter.hpp"
#include "CustomHelperIconReaderWriter.hpp"
#include "MaxUtilities.hpp"
#include "Strings.hpp"
#include "FinjinMaxClassID.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Local types-------------------------------------------------------------------
class FinjinCustomHelperIconExporterClassDesc : public ClassDesc
{
public:
    int IsPublic() override                        {return TRUE;}
    void* Create(BOOL loading = FALSE) override    {return new FinjinCustomHelperIconExporter;}
    const MCHAR* ClassName() override            {return MaxUtilities::GetStringM(IDS_CUSTOM_HELPER_ICON_EXPORTER_CLASS_NAME);}
    SClass_ID SuperClassID() override            {return SCENE_EXPORT_CLASS_ID;}
    Class_ID ClassID() override                    {return FinjinMaxClassID::CustomHelperIconExporter;}
    const MCHAR* Category() override             {return MaxUtilities::GetStringM(IDS_PLUGINS_CATEGORY);}

public:
    static FinjinCustomHelperIconExporterClassDesc& GetInstance()
    {
        static FinjinCustomHelperIconExporterClassDesc instance;
        return instance;
    }
};

struct IconEdge
{
    unsigned long v1, v2;
};


//Implementation----------------------------------------------------------------
FinjinCustomHelperIconExporter::FinjinCustomHelperIconExporter()
{
    this->time = 0;
    this->maxInterface = nullptr;
}

int FinjinCustomHelperIconExporter::ExtCount()
{
    return 1;
}

const TCHAR* FinjinCustomHelperIconExporter::Ext(int n)
{
    return _T("chi");
}

const TCHAR* FinjinCustomHelperIconExporter::LongDesc()
{
    return MaxUtilities::GetString(IDS_CUSTOM_HELPER_ICON_EXPORTER_LONG_DESCRIPTION);
}

const TCHAR* FinjinCustomHelperIconExporter::ShortDesc()
{
    return MaxUtilities::GetString(IDS_CUSTOM_HELPER_ICON_EXPORTER_SHORT_DESCRIPTION);
}

const TCHAR* FinjinCustomHelperIconExporter::AuthorName()
{
    return Strings::COMPANY_NAME_INFO.wx_str();
}

const TCHAR* FinjinCustomHelperIconExporter::CopyrightMessage()
{
    return Strings::COPYRIGHT_INFO.wx_str();
}

const TCHAR* FinjinCustomHelperIconExporter::OtherMessage1()
{
    return _T("");
}

const TCHAR* FinjinCustomHelperIconExporter::OtherMessage2()
{
    return _T("");
}

unsigned int FinjinCustomHelperIconExporter::Version()
{
    return 100;
}

void FinjinCustomHelperIconExporter::ShowAbout(HWND hWnd)
{
}

int FinjinCustomHelperIconExporter::DoExport
    (
    const TCHAR* fileName,
    ExpInterface* exportInterface,
    Interface* maxInterface,
    BOOL suppressPrompts,
    DWORD options
    )
{
    //Update member variables
    this->time = maxInterface->GetTime();
    this->maxInterface = maxInterface;

    //First see if there's anything to even attempt to export
    int nodeCount = maxInterface->GetRootNode()->NumberOfChildren();
    if (nodeCount == 0)
    {
        if (!suppressPrompts)
        {
            MessageBox(maxInterface->GetMAXHWnd(), wxT("There is no object to export"), MaxUtilities::GetString2(IDS_CUSTOM_HELPER_ICON_EXPORTER), MB_OK);
            return IMPEXP_SUCCESS;
        }
        else
            return IMPEXP_FAIL;
    }

    //Get first "meshy" child of root node
    INode* nodeToSave = nullptr;
    for (int i = 0; i < nodeCount; i++)
    {
        INode* node = maxInterface->GetRootNode()->GetChildNode(i);
        if (IsNodeMesh(node))
        {
            nodeToSave = node;
            break;
        }
    }

    //Make sure we have something valid
    if (nodeToSave == nullptr)
    {
        if (!suppressPrompts)
        {
            MessageBox(maxInterface->GetMAXHWnd(), wxT("Couldn't find a mesh object to export"), MaxUtilities::GetString2(IDS_CUSTOM_HELPER_ICON_EXPORTER), MB_OK);
            return IMPEXP_SUCCESS;
        }
        else
            return IMPEXP_FAIL;
    }

    SaveIcon(nodeToSave, ApplicationStringToWxString(fileName));

    return IMPEXP_SUCCESS;
}

BOOL FinjinCustomHelperIconExporter::SupportsOptions(int ext, DWORD options)
{
    if (options & SCENE_EXPORT_SELECTED)
        return TRUE;
    else
        return FALSE;
}

ClassDesc* FinjinCustomHelperIconExporter::GetClassDesc()
{
    return &FinjinCustomHelperIconExporterClassDesc::GetInstance();
}

void FinjinCustomHelperIconExporter::SaveIcon(INode* node, const wxString& fileName)
{
    //Converts a meshy node and saves it as a C code 3d icon file
    Object* obj = node->EvalWorldState(this->time).obj;
    TriObject* triObj = (TriObject*)obj->ConvertToType(this->time, triObjectClassID);
    MNMesh mnmesh(triObj->mesh);
    int vertexCount = mnmesh.VNum();
    int edgeCount = mnmesh.ENum();

    //Count the number of visible edges
    int visibleEdgeCount = 0;
    for (int i = 0; i < edgeCount; i++)
    {
        MNEdge* edge = mnmesh.E(i);

        if (!edge->GetFlag(MN_EDGE_INVIS))
            visibleEdgeCount++;
    }

    //Convert the vertices and edges
    CustomHelperIconWriter icon;
    icon.edges.resize(visibleEdgeCount);
    icon.vertices.resize(vertexCount);

    //Extract the visible edges
    int count = 0;
    for (int i = 0; i < edgeCount; i++)
    {
        MNEdge* edge = mnmesh.E(i);

        if (!edge->GetFlag(MN_EDGE_INVIS))
        {
            icon.edges[count].v1 = edge->v1;
            icon.edges[count].v2 = edge->v2;
            count++;
        }
    }

    //Get vertices
    for (int i = 0; i < vertexCount; i++)
    {
        MNVert* v = mnmesh.V(i);
        icon.vertices[i].Set(v->p.x, v->p.y, v->p.z);
    }

    //Save
    if (icon.IsValid())
    {
        icon.Normalize();
        icon.Write(fileName, wxT("Point3"));
    }

    //Clean up
    if (triObj != obj)
        triObj->DeleteThis();
}

bool FinjinCustomHelperIconExporter::IsNodeMesh(INode* node)
{
    //Determines whether or not a node is a mesh
    ObjectState os = node->EvalWorldState(this->time);

    if (os.obj->CanConvertToType(triObjectClassID))
        return true;

    return false;
}
