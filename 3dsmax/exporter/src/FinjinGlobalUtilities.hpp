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


#pragma once


//Includes----------------------------------------------------------------------
#include "FinjinGlobalSettings.hpp"
#include "CloneState.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * The main Finjin plugin class that handles initialization/shutdown,
     * registers notifications, creates the Finjin menu, and handles all the
     * top level commands.
     */
    class FinjinGlobalUtilities : public GUP
    {
    private:
        FinjinGlobalUtilities();
        ~FinjinGlobalUtilities();

    public:
        void DeleteThis();

        DWORD Start();
        void Stop();
        DWORD_PTR Control(DWORD parameter);

        //Gets the action table.
        FPInterfaceDesc* GetDesc();

        //Actions exposed to MaxScript.
        void OnExportScene();
        void OnExportSceneSelectedObjects();
        void OnExportSelectedMaterialEditorMaterial();
        void OnExportSceneMaterials();
        void OnExportCurrentMaterialLibrary();
        void OnSceneSettings();
        void OnObjectSettings();
        void OnGlobalSettings();
        void OnCheckDuplicateObjectNames();
        void OnRenderCubeMap();
        void OnViewLog();
        void OnHelp();
        void OnAbout();
        void OnRefreshFinjinMenu();

        static ClassDesc* GetClassDesc();

        static FinjinGlobalUtilities& GetInstance() {return instance;}

    private:
        void CreateFinjinMenu();
        void RemoveFinjinMenu(bool alreadyFoundStandardMenuBar = false);
        bool FinjinMenuExists();

        //Callback functions registered with 3DS Max.
        static void InterfaceChanged(void* param, NotifyInfo* info);
        static void OnBeforeSceneLoad(void* param, NotifyInfo* info);
        static void OnAfterSceneLoad(void* param, NotifyInfo* info);
        static void OnSceneReset(void* param, NotifyInfo* info);
        static void OnSceneNew(void* param, NotifyInfo* info);
        static void OnScenePreSave(void* param, NotifyInfo* info);
        static void OnScenePostSave(void* param, NotifyInfo* info);
        static void OnNodeAdded(void* param, NotifyInfo* info);
        static void OnNodePreDelete(void* param, NotifyInfo* info);
        static void OnNodeClone(void* param, NotifyInfo* info);
        static void OnNodePostMtl(void* param, NotifyInfo* info);
        static void OnSelectionSetChanged(void* param, NotifyInfo* info);
        static void OnNodeUnhide(void* param, NotifyInfo* info);
        static void OnPreMerge(void* param, NotifyInfo* info);
        static void OnPostMerge(void* param, NotifyInfo* info);
        static void OnSystemShutdown(void* param, NotifyInfo* info);
        static void OnPreWorkspaceChange(void* param, NotifyInfo* info);
        static void OnPostWorkspaceChange(void* param, NotifyInfo* info);

    public:
        static FinjinGlobalUtilities instance;

    private:
        FinjinGlobalSettings globalSettings;
        CloneState cloneState;
    };

} }
