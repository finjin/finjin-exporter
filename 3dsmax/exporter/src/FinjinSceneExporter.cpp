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
#include "FinjinSceneExporter.hpp"
#include "FinjinSceneExporterHandler.hpp"
#include "Exporter.hpp"
#include "MaxUtilities.hpp"
#include "Strings.hpp"
#include "wxAdoptedWindow.hpp"
#include "Dialogs.hpp"
#include "FinjinVersion.hpp"
#include "FinjinMaxClassID.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Local classes-----------------------------------------------------------------
class FinjinSceneExporterClassDesc : public ClassDesc 
{
public:
    int IsPublic() override                        {return TRUE;}
    void* Create(BOOL loading = FALSE) override    {return new FinjinSceneExporter;}
    const MCHAR* ClassName() override              {return MaxUtilities::GetStringM(IDS_EXPORTER_CLASS_NAME);}
    SClass_ID SuperClassID() override              {return SCENE_EXPORT_CLASS_ID;}
    Class_ID ClassID() override                    {return FinjinMaxClassID::SceneExporter;}
    const MCHAR* Category() override               {return MaxUtilities::GetStringM(IDS_PLUGINS_CATEGORY);}

public:
    static FinjinSceneExporterClassDesc& GetInstance()
    {
        static FinjinSceneExporterClassDesc instance;
        return instance;
    }
};


//Implementation----------------------------------------------------------------
int FinjinSceneExporter::ExtCount()
{
    return 4;
}

const TCHAR* FinjinSceneExporter::Ext(int n)
{
    switch (n)
    {
        case 0: return _T("fstd-scene");
        case 1: return _T("fsbd-scene");
        case 2: return _T("json-scene");
        case 3: return _T("cfg-scene");
        default: return _T("");
    }
}

const TCHAR* FinjinSceneExporter::LongDesc()
{
    return MaxUtilities::GetString(IDS_EXPORTER_LONG_DESCRIPTION);
}

const TCHAR* FinjinSceneExporter::ShortDesc()
{
    return MaxUtilities::GetString(IDS_EXPORTER_SHORT_DESCRIPTION);
}

const TCHAR* FinjinSceneExporter::AuthorName()
{
    return Strings::COMPANY_NAME_INFO.wx_str();
}

const TCHAR* FinjinSceneExporter::CopyrightMessage()
{
    return Strings::COPYRIGHT_INFO.wx_str();
}

const TCHAR* FinjinSceneExporter::OtherMessage1()
{
    return _T("");
}

const TCHAR* FinjinSceneExporter::OtherMessage2()
{
    return _T("");
}

unsigned int FinjinSceneExporter::Version()
{
    return FinjinVersion::MAJOR*100 + FinjinVersion::MINOR;
}

void FinjinSceneExporter::ShowAbout(HWND hWnd)
{    
    Dialogs::ShowAboutDialog();
}

int FinjinSceneExporter::DoExport
    (
    const TCHAR* fileName, 
    ExpInterface* exportInterface, 
    Interface* maxInterface, 
    BOOL suppressPrompts, 
    DWORD options
    )
{
    Exporter::ExportWithFileName
        (
        ApplicationStringToWxString(fileName), 
        (options & SCENE_EXPORT_SELECTED) ? true : false, 
        &FinjinSceneExporterHandlerFactory::GetInstance(),
        suppressPrompts ? false : true
        );
    return IMPEXP_SUCCESS;
}

BOOL FinjinSceneExporter::SupportsOptions(int ext, DWORD options)
{    
    if (options & SCENE_EXPORT_SELECTED)
        return TRUE;
    else
        return FALSE;
}

ClassDesc* FinjinSceneExporter::GetClassDesc()
{
    return &FinjinSceneExporterClassDesc::GetInstance();
}
