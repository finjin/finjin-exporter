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
#include "FinjinSceneSettingsObject.hpp"
#include "FinjinObjectSettingsObject.hpp"
#include "FinjinNodeAnimationSettingsObject.hpp"
#include "FinjinMeshAnimationSettingsObject.hpp"
#include "FinjinGlobalSettings.hpp"
#include "MaxMaterialUtilities.hpp"
#include "FileUtilities.hpp"
#include "StringUtilities.hpp"
#include "MaxBitmapUtilities.hpp"
#include "MaxControlUtilities.hpp"
#include "RenderCubeMapDialog.hpp"
#include "RenderTerrainMapDialog.hpp"
#include "ConvertToFramesDialog.hpp"
#include "Direct3DState.hpp"
#include "TerrainMapRenderer.hpp"
#include "CubeMapRenderer.hpp"
#include "MaxMathUtilities.hpp"
#include "ApplicationAccessor.hpp"
#include "Strings.hpp"
#include "wxApplicationWindow.hpp"
#include "FinjinVersion.hpp"
#include "FinjinResourceManager.hpp"
#include "Exporter.hpp"
#include "MaterialExporter.hpp"
#include "MaxUtilities.hpp"
#include "MaterialExporter.hpp"
#include "Dialogs.hpp"
#include "ExporterSystem.hpp"
#include "FinjinMaxClassID.hpp"
#include "resource.h"
#include "tinyxml2.h"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
FinjinGlobalUtilities FinjinGlobalUtilities::instance;


//Local functions--------------------------------------------------------------
#if !wxUSE_UNICODE
    #define fileopen fopen
#elif defined(__WXMSW__)
    #define fileopen _wfopen
#elif defined(__WXMAC__)
    inline FILE* fileopen(const wchar_t* name, const wchar_t* mode)
    {
        wxString wxname(name);
        wxString wxmode(mode);
        return fopen(wxname.ToUTF8().data(), wxmode.ToUTF8().data());
    }
#endif

inline bool LoadXmlDocument(tinyxml2::XMLDocument& doc, const wxString& fileName)
{
    auto result = false;
    auto fp = fileopen(fileName.wx_str(), wxT("rb"));
    if (fp != nullptr)
    {
        doc.LoadFile(fp);
        result = !doc.Error();
        fclose(fp);
    }
    return result;
}

static bool HasStandardMenuBar()
{    
    //There doesn't appear to be any API for determining whether enhanced menus (and the legacy menu bar) are in effect
    //This code instead looks for the current workspace's .maxsettings file, parses it, and sees if a "ShowLegacyMenuBar" item is present

    MSTR configFileName = GetCUIFrameMgr()->GetConfigFile();
    auto wxconfigFileName = ApplicationStringToWxString(configFileName);
    wxconfigFileName.Replace(wxT(".cuix"), wxT(".maxsettings"));
    
    auto hasStandardMenuBar = true;
    tinyxml2::XMLDocument doc;
    if (LoadXmlDocument(doc, wxconfigFileName))
    {
        auto rootXmlElement = doc.RootElement();
        if (rootXmlElement != nullptr)
        {
            auto value = rootXmlElement->Attribute("ShowLegacyMenuBar");
            if (value != nullptr && strcmp(value, "false") == 0)
                hasStandardMenuBar = false;
        }
    }
    return hasStandardMenuBar;
}


//Local classes-----------------------------------------------------------------
class FinjinGlobalUtilitiesClassDesc : public ClassDesc2 
{
public:
    int IsPublic() override                      {return 1;}
    void* Create(BOOL loading = FALSE) override  {return &FinjinGlobalUtilities::instance;}
    const MCHAR* ClassName() override            {return MaxUtilities::GetStringM(IDS_GLOBAL_UTILITIES_CLASS_NAME);}
    SClass_ID SuperClassID() override            {return GUP_CLASS_ID;}
    Class_ID ClassID() override                  {return FinjinMaxClassID::GlobalUtilities;}
    const MCHAR* Category() override             {return MaxUtilities::GetStringM(IDS_PLUGINS_CATEGORY);}
    const MCHAR* InternalName() override         {return _M("Finjin");}
    HINSTANCE HInstance() override               {return wxGetInstance();}

public:
    static FinjinGlobalUtilitiesClassDesc instance;
};
FinjinGlobalUtilitiesClassDesc FinjinGlobalUtilitiesClassDesc::instance;

class FinjinGlobalActions : public FPStaticInterface
{
public:
    DECLARE_DESCRIPTOR(FinjinGlobalActions)

    enum 
    {
        fnId_ExportScene,
        fnId_ExportSceneSelectedObjects,
        
        fnId_ExportSceneMaterials,
        fnId_ExportSelectedMaterialEditorMaterial,    
        fnId_ExportCurrentMaterialLibrary,

        fnId_SceneSettings,
        fnId_ObjectSettings,

        fnId_RenderCubeMap,
        
        fnId_ViewLog,

        fnId_Help,
        fnId_About,

        fnId_GlobalSettings,
        fnId_RefreshFinjinMenu,

        fnId_CheckDuplicateObjectNames,
        fnId_RunExternalViewerWithLastExportedFile,
        fnId_RunExternalViewer
    };

    BEGIN_FUNCTION_MAP
        FN_ACTION(fnId_ExportScene, ExportScene);
        FN_ACTION(fnId_ExportSceneSelectedObjects, ExportSceneSelectedObjects);
        FN_ACTION(fnId_ExportSceneMaterials, ExportSceneMaterials);
        FN_ACTION(fnId_ExportSelectedMaterialEditorMaterial, ExportSelectedMaterialEditorMaterial);
        FN_ACTION(fnId_ExportCurrentMaterialLibrary, ExportCurrentMaterialLibrary);
        FN_ACTION(fnId_SceneSettings, SceneSettings);
        FN_ACTION(fnId_ObjectSettings, ObjectSettings);
        FN_ACTION(fnId_GlobalSettings, GlobalSettings);
        FN_ACTION(fnId_RenderCubeMap, RenderCubeMap);
        FN_ACTION(fnId_ViewLog, ViewLog);
        FN_ACTION(fnId_Help, Help);
        FN_ACTION(fnId_About, About);
        FN_ACTION(fnId_RefreshFinjinMenu, RefreshFinjinMenu);
        FN_ACTION(fnId_CheckDuplicateObjectNames, CheckDuplicateObjectNames);
        FN_ACTION(fnId_RunExternalViewerWithLastExportedFile, RunExternalViewerWithLastExportedFile);
        FN_ACTION(fnId_RunExternalViewer, RunExternalViewer);
    END_FUNCTION_MAP

    FPStatus ExportScene() 
    { 
        FinjinGlobalUtilities::GetInstance().OnExportScene(); 
        return FPS_OK; 
    }

    FPStatus ExportSceneSelectedObjects() 
    { 
        FinjinGlobalUtilities::GetInstance().OnExportSceneSelectedObjects(); 
        return FPS_OK; 
    }

    FPStatus ExportSelectedMaterialEditorMaterial() 
    { 
        FinjinGlobalUtilities::GetInstance().OnExportSelectedMaterialEditorMaterial(); 
        return FPS_OK; 
    }

    FPStatus ExportSceneMaterials() 
    { 
        FinjinGlobalUtilities::GetInstance().OnExportSceneMaterials(); 
        return FPS_OK; 
    }

    FPStatus ExportCurrentMaterialLibrary() 
    { 
        FinjinGlobalUtilities::GetInstance().OnExportCurrentMaterialLibrary(); 
        return FPS_OK; 
    }

    FPStatus SceneSettings() 
    { 
        FinjinGlobalUtilities::GetInstance().OnSceneSettings(); 
        return FPS_OK; 
    }

    FPStatus ObjectSettings() 
    { 
        FinjinGlobalUtilities::GetInstance().OnObjectSettings(); 
        return FPS_OK; 
    }

    FPStatus GlobalSettings() 
    { 
        FinjinGlobalUtilities::GetInstance().OnGlobalSettings(); 
        return FPS_OK; 
    }

    FPStatus RenderCubeMap()
    {
        FinjinGlobalUtilities::GetInstance().OnRenderCubeMap(); 
        return FPS_OK; 
    }

    FPStatus ViewLog()
    {
        FinjinGlobalUtilities::GetInstance().OnViewLog(); 
        return FPS_OK; 
    }

    FPStatus Help() 
    { 
        FinjinGlobalUtilities::GetInstance().OnHelp(); 
        return FPS_OK; 
    }

    FPStatus About() 
    { 
        FinjinGlobalUtilities::GetInstance().OnAbout(); 
        return FPS_OK; 
    }

    FPStatus RefreshFinjinMenu()
    {
        FinjinGlobalUtilities::GetInstance().OnRefreshFinjinMenu();
        return FPS_OK; 
    }

    FPStatus CheckDuplicateObjectNames()
    {
        FinjinGlobalUtilities::GetInstance().OnCheckDuplicateObjectNames();
        return FPS_OK; 
    }

    FPStatus RunExternalViewerWithLastExportedFile()
    {
        ExporterSystem::RunExternalViewerWithLastExportedFile();
        return FPS_OK; 
    }

    FPStatus RunExternalViewer()
    {
        ExporterSystem::RunExternalViewer();
        return FPS_OK; 
    }
};

static FinjinGlobalActions globalActions
    (
    Interface_ID(0x21da1875, 0x9f754881), _M("Finjin"), 0, &FinjinGlobalUtilitiesClassDesc::instance, FP_ACTIONS,

    kActionMainUIContext,

    FinjinGlobalActions::fnId_ExportScene, _M("ExportScene"), IDS_EXPORT_SCENE_ACTION_DESCRIPTION, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_EXPORT_SCENE_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_ExportSceneSelectedObjects, _M("ExportSelectedObjects"), IDS_EXPORT_SCENE_SELECTION_ACTION_DESCRIPTION, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_EXPORT_SCENE_SELECTION_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_ExportSceneMaterials, _M("ExportSceneMaterials"), IDS_EXPORT_SCENE_MATERIALS_ACTION_DESCRIPTION, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_EXPORT_SCENE_MATERIALS_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_ExportSelectedMaterialEditorMaterial, _M("ExportSelectedMaterialEditorMaterial"), IDS_EXPORT_SELECTED_MATERIAL_EDITOR_MATERIAL_ACTION_DESCRIPTION, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_EXPORT_SELECTED_MATERIAL_EDITOR_MATERIAL_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_ExportCurrentMaterialLibrary, _M("ExportCurrentMaterialLibrary"), IDS_EXPORT_MATERIAL_LIBRARY_ACTION_DESCRIPTION, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_EXPORT_MATERIAL_LIBRARY_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_SceneSettings, _M("SceneSettings"), IDS_SCENE_SETTINGS_ACTION_DESCRIPTION, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_SCENE_SETTINGS_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_ObjectSettings, _M("ObjectSettings"), IDS_OBJECT_SETTINGS_ACTION_DESCRIPTION, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_OBJECT_SETTINGS_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_GlobalSettings, _M("GlobalSettings"), IDS_GLOBAL_SETTINGS_ACTION_DESCRIPTION, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_GLOBAL_SETTINGS_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_RenderCubeMap, _M("RenderCubeMap"), IDS_RENDER_CUBE_MAP_DESCRIPTION, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_RENDER_CUBE_MAP_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_ViewLog, _M("ViewLog"), IDS_VIEW_LOG_ACTION_DESCRIPTION, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_VIEW_LOG_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_Help, _M("Help"), IDS_HELP_ACTION_DESCRIPTION, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_HELP_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_About, _M("About"), IDS_ABOUT_ACTION_DESCRIPTION, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_ABOUT_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_RefreshFinjinMenu, _M("RefreshFinjinMenu"), IDS_REFRESH_FINJIN_MENU_DESCRIPTION, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_REFRESH_FINJIN_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_CheckDuplicateObjectNames, _M("CheckDuplicateObjectNames"), IDS_CHECK_DUPLICATE_OBJECT_NAMES_MENU_DESCRIPTION, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_CHECK_DUPLICATE_OBJECT_NAMES_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_RunExternalViewerWithLastExportedFile, _M("RunExternalViewerWithLastExportedFile"), IDS_RUN_EXTERNAL_VIEWER_WITH_LAST_EXPORTED_FILE, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_RUN_EXTERNAL_VIEWER_WITH_LAST_EXPORTED_FILE_MENU_TEXT,
        p_end,

    FinjinGlobalActions::fnId_RunExternalViewer, _M("RunExternalViewer"), IDS_RUN_EXTERNAL_VIEWER, 0,
        f_category, _M("Finjin Actions"), IDS_PLUGINS_CATEGORY,
        f_menuText, IDS_RUN_EXTERNAL_VIEWER_MENU_TEXT,
        p_end,

    p_end
    );

class IFinjinGlobalFunctions : public FPStaticInterface
{
public:
    virtual FinjinSceneSettingsObject* GetSceneSettings() = 0;
    virtual FinjinObjectSettingsObject* GetObjectSettingsByNode(INode* node) = 0;
    virtual FinjinObjectSettingsObject* GetObjectSettingsByName(MSTR nodeName) = 0;
    virtual FinjinNodeAnimationSettingsObject* CreateNodeAnimation() = 0;
    virtual FinjinMeshAnimationSettingsObject* CreateMeshAnimation() = 0;
    virtual void EnableDetailedLogging(BOOL enable) = 0;
    virtual Value* GetFinjinVersion() = 0;
    virtual void SyncSubmeshSettingsByNode(INode* node) = 0;
    virtual void SyncSubmeshSettingsByName(MSTR nodeName) = 0;
        
    enum 
    { 
        fnId_getSceneSettings,
        fnId_getObjectSettingsByNode,
        fnId_getObjectSettingsByName,
        fnId_createNodeAnimation,
        fnId_createMeshAnimation,
        fnId_enableDetailedLogging,
        fnId_getVersion,
        fnId_SyncSubmeshSettingsByNode,
        fnId_SyncSubmeshSettingsByName
    };
};

class FinjinGlobalFunctions : public IFinjinGlobalFunctions
{
public:
    FinjinSceneSettingsObject* GetSceneSettings()
    {
        return FinjinSceneSettingsAccessor::GetSceneSettings().object;
    }

    FinjinObjectSettingsObject* GetObjectSettingsByNode(INode* node)
    {
        FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();
        return sceneSettings.GetObjectSettings(node).object;
    }

    FinjinObjectSettingsObject* GetObjectSettingsByName(MSTR nodeName)
    {
        FinjinObjectSettingsAccessor objectSettings;

        ObjectAccessor object;
        if (nodeName.length() > 0)
            object = GetCOREInterface()->GetINodeByName(nodeName);
        else
        {
            std::vector<ObjectAccessor> selectedObjects;
            ObjectAccessor::GetSelectedObjects(selectedObjects);
            if (!selectedObjects.empty())
                object = selectedObjects[0];
        }

        if (object.IsValid())
        {
            FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();
            objectSettings = sceneSettings.GetObjectSettings(object);            
        }

        return objectSettings.object;
    }

    FinjinNodeAnimationSettingsObject* CreateNodeAnimation()
    {
        return new FinjinNodeAnimationSettingsObject;
    }

    FinjinMeshAnimationSettingsObject* CreateMeshAnimation()
    {
        return new FinjinMeshAnimationSettingsObject;
    }

    void EnableDetailedLogging(BOOL enable)
    {
        ExporterSystem::EnableDetailedLogging(enable ? true : false);        
    }

    Value* GetFinjinVersion()
    {
        one_typed_value_local(String* result);
        vl.result = new String(WxStringToApplicationStringM(FinjinVersion::VERSION_TEXT));
        return_value(vl.result);
    }

    void SyncSubmeshSettingsByNode(INode* node)
    {
        if (node != nullptr)
        {
            std::vector<ObjectAccessor> objects;
            objects.resize(1);
            objects[0] = node;
            ExporterSystem::SyncSubmeshSettings(objects);
        }
    }

    void SyncSubmeshSettingsByName(MSTR nodeName)
    {   
        std::vector<ObjectAccessor> objects;

        if (nodeName.length() > 0)
        {
            ObjectAccessor object = ObjectAccessor::GetObjectByName(ApplicationStringToWxString(nodeName));
            if (object.IsValid())
            {
                objects.resize(1);
                objects[0] = object;
            }
        }
        else
            ObjectAccessor::GetSelectedObjects(objects);

        ExporterSystem::SyncSubmeshSettings(objects);            
    }

    DECLARE_DESCRIPTOR(FinjinGlobalFunctions);
    BEGIN_FUNCTION_MAP;
        FN_0(IFinjinGlobalFunctions::fnId_getSceneSettings, TYPE_REFTARG, GetSceneSettings);
        FN_1(IFinjinGlobalFunctions::fnId_getObjectSettingsByNode, TYPE_REFTARG, GetObjectSettingsByNode, TYPE_INODE);
        FN_1(IFinjinGlobalFunctions::fnId_getObjectSettingsByName, TYPE_REFTARG, GetObjectSettingsByName, TYPE_STRING);
        FN_0(IFinjinGlobalFunctions::fnId_createNodeAnimation, TYPE_REFTARG, CreateNodeAnimation);
        FN_0(IFinjinGlobalFunctions::fnId_createMeshAnimation, TYPE_REFTARG, CreateMeshAnimation);
        VFN_1(IFinjinGlobalFunctions::fnId_enableDetailedLogging, EnableDetailedLogging, TYPE_BOOL);
        FN_0(IFinjinGlobalFunctions::fnId_getVersion, TYPE_VALUE, GetFinjinVersion);
        VFN_1(IFinjinGlobalFunctions::fnId_SyncSubmeshSettingsByNode, SyncSubmeshSettingsByNode, TYPE_INODE);
        VFN_1(IFinjinGlobalFunctions::fnId_SyncSubmeshSettingsByName, SyncSubmeshSettingsByName, TYPE_STRING);
    END_FUNCTION_MAP;
};

FinjinGlobalFunctions globalFunctions
    (
    Interface_ID(0x4bbd4da1, 0xf26504dd), _M("Functions"), 0, &FinjinGlobalUtilitiesClassDesc::instance, FP_CORE,

    IFinjinGlobalFunctions::fnId_getSceneSettings, _M("GetSceneSettings"), 0, TYPE_REFTARG, 0, 0,

    IFinjinGlobalFunctions::fnId_getObjectSettingsByNode, _M("GetObjectSettingsByNode"), 0, TYPE_REFTARG, 0, 1,
        _M("node"), 0, TYPE_INODE,

    IFinjinGlobalFunctions::fnId_getObjectSettingsByName, _M("GetObjectSettingsByName"), 0, TYPE_REFTARG, 0, 1,
        _M("name"), 0, TYPE_STRING,
    
    IFinjinGlobalFunctions::fnId_createNodeAnimation, _M("CreateNodeAnimation"), 0, TYPE_REFTARG, 0, 0,

    IFinjinGlobalFunctions::fnId_createMeshAnimation, _M("CreateMeshAnimation"), 0, TYPE_REFTARG, 0, 0,

    IFinjinGlobalFunctions::fnId_enableDetailedLogging, _M("EnableDetailedLogging"), 0, TYPE_VOID, 0, 1,
        _M("enable"), 0, TYPE_BOOL,

    IFinjinGlobalFunctions::fnId_getVersion, _M("GetVersion"), 0, TYPE_VALUE, 0, 0,

    IFinjinGlobalFunctions::fnId_SyncSubmeshSettingsByNode, _M("SyncSubmeshSettingsByNode"), 0, TYPE_VOID, 0, 1,
        _M("node"), 0, TYPE_INODE,

    IFinjinGlobalFunctions::fnId_SyncSubmeshSettingsByName, _M("SyncSubmeshSettingsByName"), 0, TYPE_VOID, 0, 1,
        _M("name"), 0, TYPE_STRING,
    
    p_end
);


//Local functions-----------------------------------------------------------
static void GetNegativelyScaledNodes(INodeTab& nodes, INode* node)
{
    if (ObjectAccessor::IsSettingsObject(node))
        return;

    //Decompose the node's transform
    Matrix3 tm = node->GetNodeTM(0);
    AffineParts parts;
    decomp_affine(tm, &parts);

    //Determine scaling
    Matrix3 stretchTM;
    parts.u.MakeMatrix(stretchTM);
    Matrix3 flipTM = ScaleMatrix(Point3(parts.f, parts.f, parts.f));
    Matrix3 finalScaleTM = Inverse(stretchTM) * flipTM * stretchTM;
    Point3 scale = finalScaleTM * parts.k;    

    //Add node if any scale component is negative
    if (scale.x < 0 || scale.y < 0 || scale.z < 0 || parts.f < 0)
        nodes.Append(1, &node);

    //Recurse
    for (int i = 0; i < node->NumberOfChildren(); i++)
        GetNegativelyScaledNodes(nodes, node->GetChildNode(i));
}

static void ExportMaterialTab(Tab<MtlBaseHandle>& materialTab)
{
    MaterialAccessorVector materials;
    materials.resize(materialTab.Count());
    for (int i = 0; i < materialTab.Count(); i++)
        materials[i] = (Mtl*)materialTab[i];
    MaterialExporter::ExportMaterialsWithDialog(materials);    
}


//FinjinGlobalUtilities----------------------------------------
FinjinGlobalUtilities::FinjinGlobalUtilities()
{   
}
        
FinjinGlobalUtilities::~FinjinGlobalUtilities()
{    
}

void FinjinGlobalUtilities::DeleteThis() 
{
    //Do nothing
    //delete this;
}

DWORD FinjinGlobalUtilities::Start()
{
    //Register system callbacks
    RegisterNotification(InterfaceChanged, this, NOTIFY_COLOR_CHANGE);
    RegisterNotification(OnBeforeSceneLoad, this, NOTIFY_FILE_PRE_OPEN);
    RegisterNotification(OnAfterSceneLoad, this, NOTIFY_FILE_POST_OPEN);
    RegisterNotification(OnSceneReset, this, NOTIFY_SYSTEM_PRE_RESET);
    RegisterNotification(OnSceneNew, this, NOTIFY_SYSTEM_PRE_NEW);
    RegisterNotification(OnScenePreSave, this, NOTIFY_FILE_PRE_SAVE);
    RegisterNotification(OnScenePostSave, this, NOTIFY_FILE_POST_SAVE);
    RegisterNotification(OnNodeAdded, this, NOTIFY_SCENE_ADDED_NODE);
    RegisterNotification(OnNodePreDelete, this, NOTIFY_SCENE_PRE_DELETED_NODE);    
    RegisterNotification(OnNodeClone, this, NOTIFY_NODE_CLONED);
    RegisterNotification(OnNodePostMtl, this, NOTIFY_NODE_POST_MTL);
    RegisterNotification(OnSelectionSetChanged, this, NOTIFY_SELECTIONSET_CHANGED);    
    RegisterNotification(OnNodeUnhide, this, NOTIFY_NODE_UNHIDE);
    RegisterNotification(OnPreMerge, this, NOTIFY_FILE_PRE_MERGE);
    RegisterNotification(OnPostMerge, this, NOTIFY_FILE_POST_MERGE);
    RegisterNotification(OnSystemShutdown, this, NOTIFY_SYSTEM_SHUTDOWN);
    RegisterNotification(OnPreWorkspaceChange, this, NOTIFY_PRE_WORKSPACE_CHANGE);
    RegisterNotification(OnPostWorkspaceChange, this, NOTIFY_POST_WORKSPACE_CHANGE);

    //If Finjin is being uninstalled, don't create the Finjin menu
    auto uninstallFileName = FileUtilities::JoinPath(ApplicationAccessor::GetApplicationDirectory(), _T("finjin-exporter-uninstall.txt"));
    if (wxFileExists(uninstallFileName))
    {
        RemoveFinjinMenu();
        wxRemoveFile(uninstallFileName);
    }
    else
        CreateFinjinMenu();

    ExporterSystem::Initialize();

    return GUPRESULT_KEEP;
}

void FinjinGlobalUtilities::Stop()
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("FinjinGlobalUtilities::Stop()"));

    //Unregister system callbacks
    UnRegisterNotification(InterfaceChanged, this, NOTIFY_COLOR_CHANGE);
    UnRegisterNotification(OnBeforeSceneLoad, this, NOTIFY_FILE_PRE_OPEN);
    UnRegisterNotification(OnAfterSceneLoad, this, NOTIFY_FILE_POST_OPEN);
    UnRegisterNotification(OnSceneReset, this, NOTIFY_SYSTEM_PRE_RESET);
    UnRegisterNotification(OnSceneNew, this, NOTIFY_SYSTEM_PRE_NEW);
    UnRegisterNotification(OnScenePreSave, this, NOTIFY_FILE_PRE_SAVE);
    UnRegisterNotification(OnScenePostSave, this, NOTIFY_FILE_POST_SAVE);
    UnRegisterNotification(OnNodeAdded, this, NOTIFY_SCENE_ADDED_NODE);
    UnRegisterNotification(OnNodePreDelete, this, NOTIFY_SCENE_PRE_DELETED_NODE);    
    UnRegisterNotification(OnNodeClone, this, NOTIFY_NODE_CLONED);
    UnRegisterNotification(OnNodePostMtl, this, NOTIFY_NODE_POST_MTL);
    UnRegisterNotification(OnSelectionSetChanged, this, NOTIFY_SELECTIONSET_CHANGED);    
    UnRegisterNotification(OnNodeUnhide, this, NOTIFY_NODE_UNHIDE);
    UnRegisterNotification(OnPreMerge, this, NOTIFY_FILE_PRE_MERGE);
    UnRegisterNotification(OnPostMerge, this, NOTIFY_FILE_POST_MERGE);
    UnRegisterNotification(OnSystemShutdown, this, NOTIFY_SYSTEM_SHUTDOWN);
    UnRegisterNotification(OnPreWorkspaceChange, this, NOTIFY_PRE_WORKSPACE_CHANGE);
    UnRegisterNotification(OnPostWorkspaceChange, this, NOTIFY_POST_WORKSPACE_CHANGE);
}

DWORD_PTR FinjinGlobalUtilities::Control(DWORD parameter)
{
    return GUPRESULT_KEEP;
}

FPInterfaceDesc* FinjinGlobalUtilities::GetDesc()
{ 
    return &globalActions; 
}

void FinjinGlobalUtilities::CreateFinjinMenu()
{
    if (!HasStandardMenuBar())
        return;

    RemoveFinjinMenu(true);

    //Get menu
    IMenuManager* menuManager = Max()->GetMenuManager();
    auto mainMenu = menuManager->GetMainMenuBar();
    if (mainMenu == nullptr) 
        return;

    //Create the "Finjin" submenu
    IMenuItem* separatorMenuItem = nullptr;
    auto finjinMenu = GetIMenu();
    finjinMenu->SetTitle(MaxUtilities::GetString(IDS_MENU_NAME));
    menuManager->RegisterMenu(finjinMenu, 0);

    //Create the "Export" submenu
    auto exportMenu = GetIMenu();
    exportMenu->SetTitle(MaxUtilities::GetString(IDS_EXPORT));
    menuManager->RegisterMenu(exportMenu, 0);
    
        //Add "export scene" menu item
        auto exportSceneMenuItem = GetIMenuItem();
        exportSceneMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_ExportScene));
        exportMenu->AddItem(exportSceneMenuItem);
            
        //Add "export selected objects" menu item
        auto exportSceneSelectedMenuItem = GetIMenuItem();
        exportSceneSelectedMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_ExportSceneSelectedObjects));
        exportMenu->AddItem(exportSceneSelectedMenuItem);    

        //Add separator menu item
        separatorMenuItem = GetIMenuItem();
        separatorMenuItem->ActAsSeparator();
        exportMenu->AddItem(separatorMenuItem);

        //Add "export scene materials" menu item
        auto exportSceneMaterialsMenuItem = GetIMenuItem();
        exportSceneMaterialsMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_ExportSceneMaterials));
        exportMenu->AddItem(exportSceneMaterialsMenuItem);    

        //Add "export selected material editor material" menu item
        auto exportSelectedMaterialEditorMaterialMenuItem = GetIMenuItem();
        exportSelectedMaterialEditorMaterialMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_ExportSelectedMaterialEditorMaterial));
        exportMenu->AddItem(exportSelectedMaterialEditorMaterialMenuItem);    

        //Add "export material library" menu item
        auto exportCurrentMaterialLibraryMenuItem = GetIMenuItem();
        exportCurrentMaterialLibraryMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_ExportCurrentMaterialLibrary));
        exportMenu->AddItem(exportCurrentMaterialLibraryMenuItem);    

        auto exportMenuItem = GetIMenuItem();
        exportMenuItem->SetSubMenu(exportMenu);
        finjinMenu->AddItem(exportMenuItem);

    //Add separator menu item
    separatorMenuItem = GetIMenuItem();
    separatorMenuItem->ActAsSeparator();
    finjinMenu->AddItem(separatorMenuItem);

    //Create the "External Viewer" submenu
    auto externalViewerMenu = GetIMenu();
    externalViewerMenu->SetTitle(MaxUtilities::GetString(IDS_EXTERNAL_VIEWER));
    menuManager->RegisterMenu(externalViewerMenu, 0);
    
        //Add "Run With Last Exported File..." menu item
        auto runExternalViewerWithLastExportedFileMenuItem = GetIMenuItem();
        runExternalViewerWithLastExportedFileMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_RunExternalViewerWithLastExportedFile));
        externalViewerMenu->AddItem(runExternalViewerWithLastExportedFileMenuItem);
            
        //Add "Run..." menu item
        auto runExternalViewerMenuItem = GetIMenuItem();
        runExternalViewerMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_RunExternalViewer));
        externalViewerMenu->AddItem(runExternalViewerMenuItem);    

        auto externalViewerMenuItem = GetIMenuItem();
        externalViewerMenuItem->SetSubMenu(externalViewerMenu);
        finjinMenu->AddItem(externalViewerMenuItem);

    //Add separator menu item
    separatorMenuItem = GetIMenuItem();
    separatorMenuItem->ActAsSeparator();
    finjinMenu->AddItem(separatorMenuItem);

    //Add "global settings" menu item
    auto globalSettingsMenuItem = GetIMenuItem();
    globalSettingsMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_GlobalSettings));
    finjinMenu->AddItem(globalSettingsMenuItem);

    //Add "scene settings" menu item
    auto sceneSettingsMenuItem = GetIMenuItem();
    sceneSettingsMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_SceneSettings));
    finjinMenu->AddItem(sceneSettingsMenuItem);

    //Add "object settings" menu item
    auto objectSettingsMenuItem = GetIMenuItem();
    objectSettingsMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_ObjectSettings));
    finjinMenu->AddItem(objectSettingsMenuItem);

    //Add separator menu item
    separatorMenuItem = GetIMenuItem();
    separatorMenuItem->ActAsSeparator();
    finjinMenu->AddItem(separatorMenuItem);

    //Add "Check Duplicate Object Names" item
    auto checkDuplicateObjectNamesMenuItem = GetIMenuItem();
    checkDuplicateObjectNamesMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_CheckDuplicateObjectNames));
    finjinMenu->AddItem(checkDuplicateObjectNamesMenuItem);

    //Add separator menu item
    separatorMenuItem = GetIMenuItem();
    separatorMenuItem->ActAsSeparator();
    finjinMenu->AddItem(separatorMenuItem);

#if defined(FINJIN_EXPORTER_ENABLE_D3D_FUNCTIONALITY)
    //Add "Render Cube Map" item
    auto renderCubeMapMenuItem = GetIMenuItem();
    renderCubeMapMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_RenderCubeMap));
    finjinMenu->AddItem(renderCubeMapMenuItem);

    //Add separator menu item
    separatorMenuItem = GetIMenuItem();
    separatorMenuItem->ActAsSeparator();
    finjinMenu->AddItem(separatorMenuItem);
#endif

    //Add "view log" menu item
    IMenuItem* viewLogMenuItem = GetIMenuItem();
    viewLogMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_ViewLog));
    finjinMenu->AddItem(viewLogMenuItem);

    //Add separator menu item
    separatorMenuItem = GetIMenuItem();
    separatorMenuItem->ActAsSeparator();
    finjinMenu->AddItem(separatorMenuItem);

    //Add "help" menu item
    auto helpMenuItem = GetIMenuItem();
    helpMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_Help));
    finjinMenu->AddItem(helpMenuItem);    

    //Add "about" menu item
    auto aboutMenuItem = GetIMenuItem();
    aboutMenuItem->SetActionItem(globalActions.action_table->GetAction(FinjinGlobalActions::fnId_About));
    finjinMenu->AddItem(aboutMenuItem);    

     //Create a new menu item to hold the main Finjin menu
    auto finjinMenuItem = GetIMenuItem();
    finjinMenuItem->SetSubMenu(finjinMenu);
    
    //Add the sub-menu item to the main menu bar
    mainMenu->AddItem(finjinMenuItem, mainMenu->NumItems() - 1);

    menuManager->UpdateMenuBar();
}

void FinjinGlobalUtilities::RemoveFinjinMenu(bool alreadyFoundStandardMenuBar)
{
    if (alreadyFoundStandardMenuBar || HasStandardMenuBar())
    {
        //Remove existing Finjin menu(s)
        TSTR menuName = MaxUtilities::GetString(IDS_MENU_NAME);
        auto menuManager = Max()->GetMenuManager();
        IMenu* oldMenu;
        while ((oldMenu = menuManager->FindMenu(menuName)) != nullptr)
            menuManager->UnRegisterMenu(oldMenu);
        menuManager->UpdateMenuBar();
    }
}

bool FinjinGlobalUtilities::FinjinMenuExists()
{
    auto menuManager = Max()->GetMenuManager();
    auto mainMenu = menuManager->GetMainMenuBar();
    if (mainMenu == nullptr) 
        return false;
    
    TSTR menuName = MaxUtilities::GetString(IDS_MENU_NAME);
    auto oldMenu = menuManager->FindMenu(menuName);
    return oldMenu != nullptr;
}

void FinjinGlobalUtilities::OnExportScene()
{
    Exporter::ExportWithDialog(false);
}

void FinjinGlobalUtilities::OnExportSceneSelectedObjects()
{   
    Exporter::ExportWithDialog(true);
}

void FinjinGlobalUtilities::OnExportSelectedMaterialEditorMaterial()
{
    MaterialExporter::ExportSelectedMaterialsWithDialog();    
}

void FinjinGlobalUtilities::OnExportSceneMaterials()
{
    ExportMaterialTab(*Max()->GetSceneMtls());
}

void FinjinGlobalUtilities::OnExportCurrentMaterialLibrary()
{
    ExportMaterialTab(Max()->GetMaterialLibrary());    
}

void FinjinGlobalUtilities::OnSceneSettings()
{
    Dialogs::ShowSceneSettingsDialog(FinjinSceneSettingsAccessor::GetSceneSettings());
}

void FinjinGlobalUtilities::OnObjectSettings()
{
    Dialogs::ShowObjectSettingsDialog();
}

void FinjinGlobalUtilities::OnGlobalSettings()
{
    Dialogs::ShowGlobalSettingsDialog();
}

void FinjinGlobalUtilities::OnCheckDuplicateObjectNames()
{
    Dialogs::ShowDuplicateObjectNamesDialog();
}

void FinjinGlobalUtilities::OnRenderCubeMap()
{
#if defined(FINJIN_EXPORTER_ENABLE_D3D_FUNCTIONALITY)
    RenderCubeMapDialog dialog;
    if (GetCOREInterface()->GetSelNodeCount() > 0)
        dialog.centerNode = GetCOREInterface()->GetSelNode(0);
    
    if (dialog.DoModal(GetCOREInterface()->GetMAXHWnd()))
    {
        CubeMapRenderer cubeMapRenderer;
        cubeMapRenderer.SetCenterNode(dialog.centerNode);
        
        wxString fileName;
        if (dialog.type == RenderCubeMapDialog::SEPARATE_FILES)
            fileName = FileUtilities::JoinPath(dialog.destinationDirectory, dialog.destinationBaseFileName);
        else
            fileName = dialog.destinationFileName;

        cubeMapRenderer.RenderToFile(fileName, dialog.dimensions, dialog.type == RenderCubeMapDialog::SEPARATE_FILES);        
    }
#endif
}

void FinjinGlobalUtilities::OnViewLog()
{
    Dialogs::ShowLogFileDialog();    
}

void FinjinGlobalUtilities::OnHelp()
{   
    ExporterSystem::ShowHelp();    
}

void FinjinGlobalUtilities::OnAbout()
{
    Dialogs::ShowAboutDialog();
}

void FinjinGlobalUtilities::OnRefreshFinjinMenu()
{
    instance.CreateFinjinMenu();
}

void FinjinGlobalUtilities::InterfaceChanged(void* param, NotifyInfo* info)
{
    instance.CreateFinjinMenu();
}

/*
Max's scene loading process:
-User selects scene file
-BeforeOpenSceneCallback() is called by Max. At this point, the previous scene data is still present
-The new scene data is loaded by Max
-Finjin scene views are created by Max
-AfterOpenSceneCallback() is called by Max
*/

void FinjinGlobalUtilities::OnBeforeSceneLoad(void* param, NotifyInfo* info)
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("FinjinGlobalUtilities::OnBeforeSceneLoad()"));

    ExporterSystem::OnBeforeOpenScene();
}

void FinjinGlobalUtilities::OnAfterSceneLoad(void* param, NotifyInfo* info)
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("FinjinGlobalUtilities::OnAfterSceneLoad()"));

    instance.cloneState.Reset();

    ExporterSystem::OnAfterOpenScene();

    FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings(false);
    if (sceneSettings.IsValid())
        sceneSettings.CleanObjectSettings();
}

void FinjinGlobalUtilities::OnSceneReset(void* param, NotifyInfo* info)
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("FinjinGlobalUtilities::OnSceneReset()"));

    instance.cloneState.Reset();
    ExporterSystem::OnSceneReset();
}

void FinjinGlobalUtilities::OnSceneNew(void* param, NotifyInfo* info)
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("FinjinGlobalUtilities::OnSceneNew()"));

    auto newType = *(int*)info->callParam;
    if (newType == PRE_NEW_NEW_ALL)
    {
        instance.cloneState.Reset();            
        ExporterSystem::OnSceneReset();
    }
}

void FinjinGlobalUtilities::OnScenePreSave(void* param, NotifyInfo* info)
{    
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("FinjinGlobalUtilities::OnScenePreSave()"));
}

void FinjinGlobalUtilities::OnScenePostSave(void* param, NotifyInfo* info)
{   
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("FinjinGlobalUtilities::OnScenePostSave()"));
}

void FinjinGlobalUtilities::OnNodeAdded(void* param, NotifyInfo* info)
{
    auto node = (INode*)info->callParam;
    if (!ObjectAccessor::IsSettingsObject(node))
    {
        FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings(false);
        if (sceneSettings.IsValid())
        {
            sceneSettings.GetObjectSettings(node);
            instance.cloneState.AddMaybeClonedNode(node);            
        }

        //FinjinSceneViewManager::GetInstance().OnAddedObject(node);
    }
}

void FinjinGlobalUtilities::OnNodePreDelete(void* param, NotifyInfo* info)
{
    auto node = (INode*)info->callParam;

    instance.cloneState.RemoveMaybeClonedNode(node);    

    //FinjinSceneViewManager::GetInstance().OnRemovingObject(node);
    
    FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings(false);
    if (sceneSettings.IsValid() && sceneSettings.object != node->GetObjectRef())
        sceneSettings.RemoveObjectSettings(node);    
}

void FinjinGlobalUtilities::OnNodeClone(void* param, NotifyInfo* info)
{
    instance.cloneState.OnCloneNode();
}

void FinjinGlobalUtilities::OnNodePostMtl(void* param, NotifyInfo* info)
{
    auto node = (INode*)info->callParam;
    //FinjinSceneViewManager::GetInstance().OnAssignedMaterialToObject(node);
}

void FinjinGlobalUtilities::OnSelectionSetChanged(void* param, NotifyInfo* info)
{
    instance.cloneState.OnSelectionSetChanged();
}

void FinjinGlobalUtilities::OnNodeUnhide(void* param, NotifyInfo* info)
{
    auto node = (INode*)info->callParam;
    FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings(false);
    if (sceneSettings.IsValid() && sceneSettings.object == node->GetObjectRef())
        node->Hide(TRUE);
}

void FinjinGlobalUtilities::OnPreMerge(void* param, NotifyInfo* info)
{
    ExporterSystem::OnBeforeMerge();
}

void FinjinGlobalUtilities::OnPostMerge(void* param, NotifyInfo* info)
{
    ExporterSystem::OnAfterMerge();
    FinjinSceneSettingsAccessor::MergeSceneSettings();

    FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings(false);
    if (sceneSettings.IsValid())
        sceneSettings.CleanObjectSettings();
}

void FinjinGlobalUtilities::OnSystemShutdown(void* param, NotifyInfo* info)
{   
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("FinjinGlobalUtilities::OnSystemShutdown()"));

    ExporterSystem::Shutdown();
}

void FinjinGlobalUtilities::OnPreWorkspaceChange(void* param, NotifyInfo* info)
{    
}

void FinjinGlobalUtilities::OnPostWorkspaceChange(void* param, NotifyInfo* info)
{    
}

ClassDesc* FinjinGlobalUtilities::GetClassDesc()
{
    return &FinjinGlobalUtilitiesClassDesc::instance;
}
