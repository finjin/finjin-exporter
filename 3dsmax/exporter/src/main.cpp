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
#include "FinjinGlobalUtilities.hpp"
#include "FinjinObjectSettingsObject.hpp"
#include "FinjinSceneSettingsObject.hpp"
#include "FinjinNodeAnimationSettingsObject.hpp"
#include "FinjinMeshAnimationSettingsObject.hpp"
#include "FinjinSubmeshSettingsObject.hpp"
#include "FinjinSceneExporter.hpp"
#include "FinjinCustomHelperIconExporter.hpp"
#include "FinjinCustomDummy.hpp"
#include "FinjinCubeMapHelper.hpp"
#include "FinjinInstancedGeometry.hpp"
#include "FinjinStaticGeometry.hpp"
#include "ExtendedValueObject.hpp"
#include "ApplicationAccessor.hpp"
#include "MaxUtilities.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Local classes----------------------------------------------------------------
/** This is needed to properly initialize wxWidgets */
class DummyApp : public wxApp
{
};
IMPLEMENT_APP_NO_MAIN(DummyApp)


//Globals----------------------------------------------------------------------
static StaticVector<ClassDesc*, 50> classDescriptions;


//Functions--------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD reason, void*)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        //3DS Max custom controls initialization
        DisableThreadLibraryCalls(hInstance);
    
        //Initialize Explorer-style controls
        InitCommonControls();

        //Initialize wxWidgets
        wxSetInstance(hInstance);
        int argc = 0;
        wxChar** argv = nullptr;
        wxEntryStart(argc, argv);
        wxTheApp->CallOnInit();

        wxSystemOptions::SetOption(wxT("msw.staticbox.optimized-paint"), 0);
                
        ApplicationAccessor::RegisterTypes();

        classDescriptions.push_back(FinjinGlobalUtilities::GetClassDesc());
        classDescriptions.push_back(FinjinSceneExporter::GetClassDesc());        
        classDescriptions.push_back(FinjinObjectSettingsObject::GetClassDesc());
        classDescriptions.push_back(FinjinSceneSettingsObject::GetClassDesc());
        classDescriptions.push_back(FinjinNodeAnimationSettingsObject::GetClassDesc());
        classDescriptions.push_back(FinjinMeshAnimationSettingsObject::GetClassDesc());
        classDescriptions.push_back(FinjinSubmeshSettingsObject::GetClassDesc());
        classDescriptions.push_back(FinjinCustomHelperIconExporter::GetClassDesc());
        classDescriptions.push_back(FinjinCustomDummy::GetClassDesc());
    #if defined(FINJIN_EXPORTER_ENABLE_D3D_FUNCTIONALITY)
        classDescriptions.push_back(FinjinCubeMapHelper::GetClassDesc());
    #endif
        //classDescriptions.push_back(FinjinInstancedGeometry::GetClassDesc());
        //classDescriptions.push_back(FinjinStaticGeometry::GetClassDesc());
        classDescriptions.push_back(ExtendedValueObject::GetClassDesc());
    }
    else if (reason == DLL_PROCESS_DETACH)
    {
        //Shut down wxWidgets
        wxEntryCleanup();
    }

    return TRUE;
}

extern "C"
{
__declspec(dllexport) const TCHAR* LibDescription()
{
    return MaxUtilities::GetString(IDS_LIBRARY_DESCRIPTION);
}

__declspec(dllexport) int LibNumberClasses()
{
    return (int)classDescriptions.size();
}

__declspec(dllexport) ClassDesc* LibClassDesc(int i)
{
    return classDescriptions[i];
}

__declspec(dllexport) ULONG LibVersion()
{
    return VERSION_3DSMAX;
}

__declspec(dllexport) ULONG CanAutoDefer()
{
    return 0;
}
}
