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
#include "FinjinSceneTranslator.hpp"
#include "FinjinCustomHelperIconTranslator.hpp"
#include "ApplicationAccessor.hpp"
#include "FileUtilities.hpp"
#include "wxApplicationWindow.hpp"
#include "FinjinVersion.hpp"
#include "FinjinSceneSettingsNode.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "FinjinGlobalSettings.hpp"
#include "FinjinCustomDummy.hpp"
#include "FinjinStaticGeometry.hpp"
#include "FinjinInstancedGeometry.hpp"
#include "RegisterSceneSettingsTypes.hpp"
#include "RegisterDetectableObjectTypes.hpp"
#include "Strings.hpp"
#include "HelpSystem.hpp"
#include "Dialogs.hpp"
#include "FinjinResourceManager.hpp"
#include "Exporter.hpp"
#include "MaterialExporter.hpp"
#include "Logger.hpp"
#include "ChooseObjectsPrompt.hpp"
#include "ChooseMaterialsPrompt.hpp"
#include "ExporterSystem.hpp"
#include "MayaUtilities.hpp"
#include "StringSplitter.hpp"
#include "StringUtilities.hpp"
#include <maya/MFnPlugin.h>
#include <maya/MUiMessage.h>

using namespace Finjin::Exporter;


//Globals-----------------------------------------------------------------------
static FinjinGlobalSettings globalSettings;

static const MString FINJIN_MENU_LABEL("Finjin");

static MCallbackId mayaExitingCallbackID = 0;
static MCallbackId beforeNewCallbackID = 0;
static MCallbackId beforeOpenCallbackID = 0;
static MCallbackId afterOpenCallbackID = 0;
static MCallbackId beforeSaveCallbackID = 0;
static MCallbackId afterSaveCallbackID = 0;
static MCallbackId beforeImportCallbackID = 0;
static MCallbackId afterImportCallbackID = 0;
static MCallbackId beforeExportCallbackID = 0;
static MCallbackId afterExportCallbackID = 0;
static MCallbackId nodeAddedCallbackID = 0;
static MCallbackId nodeRemovedCallbackID = 0;


//Local functions---------------------------------------------------------------
static void InitializeCommon()
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("InitializeCommon()"));

    if (!ExporterSystem::IsInitialized())
    {
        //Initialize wxWidgets
    #if defined(__WXMSW__)
        wxSetInstance(MhInstPlugin);
    #endif
        wxInitialize();

        ExporterSystem::Initialize();
    }
}

static void ShutdownCommon()
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("ShutdownCommon()"));

    if (ExporterSystem::IsInitialized())
    {
        ExporterSystem::Shutdown();

        //Shut down wxWidgets
        wxUninitialize();
    }
}


//Local types-------------------------------------------------------------------
/** This is needed to properly initialize wxWidgets */
class DummyApp : public wxApp
{
public:
    ~DummyApp()
    {
    #if defined(__WXMAC__)
        //There appears to be a timing issue with the unloading of plugins within Maya on MacOS that causes the freeing of this pool to sometimes crash
        //This is kind of ugly, but just set the pool to null to prevent it from being freed
        m_macPool = nullptr;
    #endif
    }
};
IMPLEMENT_APP_NO_MAIN(DummyApp)

class ExportSceneCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        MStatus result = MS::kSuccess;
        if (args.length() == 0)
            Exporter::ExportWithDialog(false);
        else
            Exporter::ExportWithFileName(ApplicationStringToWxString(args.asString(0)), false);
        return result;
    }

    static void* Creator()
    {
        return new ExportSceneCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinExportScene");
        return name;
    }
};

class ExportSelectedObjectsCommand : public ExportSceneCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        MStatus result = MS::kSuccess;
        if (args.length() == 0)
            Exporter::ExportWithDialog(true);
        else
            Exporter::ExportWithFileName(ApplicationStringToWxString(args.asString(0)), true);
        return result;
    }

    static void* Creator()
    {
        return new ExportSelectedObjectsCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinExportSelectedObjects");
        return name;
    }
};

class ExportSceneMaterialsCommand : public ExportSceneCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        MaterialExporter::ExportSceneMaterialsWithDialog();

        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new ExportSceneMaterialsCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinExportMaterials");
        return name;
    }
};

class ExportSelectedHypershadeMaterialsCommand : public ExportSceneCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        MaterialExporter::ExportSelectedMaterialsWithDialog();

        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new ExportSelectedHypershadeMaterialsCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinExportSelectedMaterials");
        return name;
    }
};

class SceneSettingsCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        auto sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();
        Dialogs::ShowSceneSettingsDialog(sceneSettings);

        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new SceneSettingsCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinSceneSettings");
        return name;
    }
};

class ObjectSettingsCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        Dialogs::ShowObjectSettingsDialog();

        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new ObjectSettingsCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinObjectSettings");
        return name;
    }
};

class GlobalSettingsCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        Dialogs::ShowGlobalSettingsDialog();

        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new GlobalSettingsCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinGlobalSettings");
        return name;
    }
};

class RunExternalViewerCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        ExporterSystem::RunExternalViewer();

        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new RunExternalViewerCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinRunExternalViewer");
        return name;
    }
};

class RunExternalViewerWithLastExportedFileCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        ExporterSystem::RunExternalViewerWithLastExportedFile();

        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new RunExternalViewerWithLastExportedFileCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinRunExternalViewerWithLastExportedFile");
        return name;
    }
};

class FinjinSyncSubmeshSettingsCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        std::vector<ObjectAccessor> objects;

        if (args.length() > 0)
        {
            objects.reserve(args.length());
            for (unsigned int i = 0; i < args.length(); i++)
            {
                auto object = ObjectAccessor::GetObjectByName(ApplicationStringToWxString(args.asString(i)));
                if (object.IsValid())
                    objects.push_back(object);
            }
        }
        else
            ObjectAccessor::GetSelectedObjects(objects);

        ExporterSystem::SyncSubmeshSettings(objects);

        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new FinjinSyncSubmeshSettingsCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinSyncSubmeshSettings");
        return name;
    }
};

class CreateCustomDummyCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        MDagModifier graphModifier;
        MObject object = graphModifier.createNode(FinjinCustomDummy::TYPE_ID);
        graphModifier.doIt();
        graphModifier.renameNode(object, FinjinCustomDummy::TYPE_NAME + "Transform");
        graphModifier.doIt();

        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new CreateCustomDummyCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinCreateCustomDummy");
        return name;
    }
};

class CreateStaticGeometryCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        MDagModifier graphModifier;
        MObject object = graphModifier.createNode(FinjinStaticGeometry::TYPE_ID);
        graphModifier.doIt();
        graphModifier.renameNode(object, FinjinStaticGeometry::TYPE_NAME + "Transform");
        graphModifier.doIt();

        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new CreateStaticGeometryCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinCreateStaticGeometry");
        return name;
    }
};

class CreateInstancedGeometryCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        MDagModifier graphModifier;
        MObject object = graphModifier.createNode(FinjinInstancedGeometry::TYPE_ID);
        graphModifier.doIt();
        graphModifier.renameNode(object, FinjinInstancedGeometry::TYPE_NAME + "Transform");
        graphModifier.doIt();

        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new CreateInstancedGeometryCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinCreateInstancedGeometry");
        return name;
    }
};

class ViewLogCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        Dialogs::ShowLogFileDialog();
        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new ViewLogCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinViewLog");
        return name;
    }
};

class HelpCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        ExporterSystem::ShowHelp();
        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new HelpCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinHelp");
        return name;
    }
};

class AboutCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        Dialogs::ShowAboutDialog();
        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new AboutCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinAbout");
        return name;
    }
};

class ChooseObjectDialogCommandBase : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args, HowMany howMany)
    {
        auto filterType = ObjectFilterType::NONE;
        if (args.length() > 0)
        {
            for (unsigned int i = 0; i < args.length(); i++)
            {
                const MString arg = args.asString(i);
                if (arg == "renderable")
                    filterType |= ObjectFilterType::RENDERABLE;
                else if (arg == "mesh")
                    filterType |= ObjectFilterType::MESH;
                else if (arg == "plane")
                    filterType |= ObjectFilterType::PLANE;
                else if (arg == "camera")
                    filterType |= ObjectFilterType::CAMERA;
                else if (arg == "light")
                    filterType |= ObjectFilterType::LIGHT;
                else if (arg == "other")
                    filterType |= ObjectFilterType::OTHER;
            }
        }
        else
            filterType = ObjectFilterType::ALL;

        if (filterType != ObjectFilterType::NONE)
        {
            auto appWindow = new wxApplicationWindow(true);
            ChooseObjectsPrompt prompt(appWindow, howMany, filterType);
            if (prompt.ShowPrompt())
            {
                if (howMany == HowMany::CHOOSE_ONE)
                {
                    MString result = WxStringToApplicationString(prompt.GetObject().GetFullName());
                    setResult(result);
                }
                else
                {
                    auto& objects = prompt.GetObjects();
                    MStringArray result;
                    result.setLength((unsigned int)objects.size());
                    for (size_t i = 0; i < objects.size(); i++)
                        result.set(WxStringToApplicationString(objects[i].GetFullName()), (unsigned int)i);
                    setResult(result);
                }
            }
        }

        return MS::kSuccess;
    }
};

class ChooseObjectDialogCommand : public ChooseObjectDialogCommandBase
{
public:
    MStatus doIt(const MArgList& args) override
    {
        return ChooseObjectDialogCommandBase::doIt(args, HowMany::CHOOSE_ONE);
    }

    static void* Creator()
    {
        return new ChooseObjectDialogCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinChooseObjectDialog");
        return name;
    }
};

class ChooseObjectsDialogCommand : public ChooseObjectDialogCommandBase
{
public:
    MStatus doIt(const MArgList& args) override
    {
        return ChooseObjectDialogCommandBase::doIt(args, HowMany::CHOOSE_MANY);
    }

    static void* Creator()
    {
        return new ChooseObjectsDialogCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinChooseObjectsDialog");
        return name;
    }
};

class ChooseMaterialDialogCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        MStringArray result(2, "");

        auto appWindow = new wxApplicationWindow(true);
        ChooseMaterialsPrompt prompt(appWindow, HowMany::CHOOSE_ONE);
        if (prompt.ShowPrompt())
        {
            result.set("ok", 0);
            result.set(WxStringToApplicationString(prompt.GetMaterial().GetFullName()), 1);
        }
        else
            result.set("cancel", 0);

        setResult(result);

        return MS::kSuccess;
    }

    static void* Creator()
    {
        return new ChooseMaterialDialogCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinChooseMaterialDialog");
        return name;
    }
};

class FinjinCustomDummyChooseCustomIconCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        MString nodeName = args.asString(0);
        MObject object = MayaUtilities::GetObjectByName(nodeName);
        if (!object.isNull())
            FinjinCustomDummy::ChooseCustomIcon(object);

        return MStatus::kSuccess;
    }

    static void* Creator()
    {
        return new FinjinCustomDummyChooseCustomIconCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinCustomDummyChooseCustomIcon");
        return name;
    }
};

class FinjinCustomDummyUseDefaultIconCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        MString nodeName = args.asString(0);
        MObject object = MayaUtilities::GetObjectByName(nodeName);
        if (!object.isNull())
            FinjinCustomDummy::UseDefaultIcon(object);

        return MStatus::kSuccess;
    }

    static void* Creator()
    {
        return new FinjinCustomDummyUseDefaultIconCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinCustomDummyUseDefaultIcon");
        return name;
    }
};

class FinjinEnableDetailedLoggingCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        bool enable = args.length() > 0 ? args.asBool(0) : true;

        ExporterSystem::EnableDetailedLogging(enable);

        return MStatus::kSuccess;
    }

    static void* Creator()
    {
        return new FinjinEnableDetailedLoggingCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinEnableDetailedLogging");
        return name;
    }
};

class FinjinGetVersionCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        MString version = WxStringToApplicationString(FinjinVersion::VERSION_TEXT);
        setResult(version);

        return MStatus::kSuccess;
    }

    static void* Creator()
    {
        return new FinjinGetVersionCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinGetVersion");
        return name;
    }
};

class FinjinGetSceneSettingsCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        auto sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();
        MFnDependencyNode depNode(sceneSettings.obj);
        MString name = depNode.name();

        setResult(name);

        return MStatus::kSuccess;
    }

    static void* Creator()
    {
        return new FinjinGetSceneSettingsCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinGetSceneSettings");
        return name;
    }
};

class FinjinGetObjectSettingsCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        MString name;

        ObjectAccessor object;
        if (args.length() > 0)
            object = ObjectAccessor::GetObjectByName(ApplicationStringToWxString(args.asString(0)));
        else
        {
            std::vector<ObjectAccessor> selectedObjects;
            ObjectAccessor::GetSelectedObjects(selectedObjects);
            if (!selectedObjects.empty())
                object = selectedObjects[0];
        }

        if (object.IsValid())
        {
            auto sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();
            auto objectSettings = sceneSettings.GetObjectSettings(object);
            MayaPlug::GetFullAttributeName(objectSettings.rootPlug, name);
        }

        setResult(name);

        return MStatus::kSuccess;
    }

    static void* Creator()
    {
        return new FinjinGetObjectSettingsCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinGetObjectSettings");
        return name;
    }
};

class FinjinAddMeshAnimationCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        MString name;

        if (args.length() > 0)
        {
            auto object = ObjectAccessor::GetObjectByName(ApplicationStringToWxString(args.asString(0)));
            if (object.IsValid())
            {
                auto sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();
                auto objectSettings = sceneSettings.GetObjectSettings(object);
                FinjinMeshAnimationSettingsAccessor animSettings = objectSettings.AddNewMeshAnimation();
                if (args.length() > 1)
                    animSettings.SetAnimationName(ApplicationStringToWxString(args.asString(1)));
                MayaPlug::GetFullAttributeName(animSettings.rootPlug, name);
            }
        }

        setResult(name);

        return MStatus::kSuccess;
    }

    static void* Creator()
    {
        return new FinjinAddMeshAnimationCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinAddMeshAnimation");
        return name;
    }
};

class FinjinAddNodeAnimationCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        MString name;

        if (args.length() > 0)
        {
            auto object = ObjectAccessor::GetObjectByName(ApplicationStringToWxString(args.asString(0)));
            if (object.IsValid())
            {
                auto sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();
                auto objectSettings = sceneSettings.GetObjectSettings(object);
                auto animSettings = objectSettings.AddNewNodeAnimation();
                if (args.length() > 1)
                    animSettings.SetAnimationName(ApplicationStringToWxString(args.asString(1)));
                MayaPlug::GetFullAttributeName(animSettings.rootPlug, name);
            }
        }

        setResult(name);

        return MStatus::kSuccess;
    }

    static void* Creator()
    {
        return new FinjinAddNodeAnimationCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("FinjinAddNodeAnimation");
        return name;
    }
};

class CheckDuplicateObjectNamesCommand : public MPxCommand
{
public:
    MStatus doIt(const MArgList& args) override
    {
        Dialogs::ShowDuplicateObjectNamesDialog();

        return MStatus::kSuccess;
    }

    static void* Creator()
    {
        return new CheckDuplicateObjectNamesCommand;
    }

    static const MString& GetCommandName()
    {
        static const MString name("CheckDuplicateObjectNames");
        return name;
    }
};


//Callbacks---------------------------------------------------------------------
static void MayaExitingCallback(void* clientData)
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("MayaExitingCallback()"));

    ShutdownCommon();
}

static void BeforeNewSceneCallback(void* clientData)
{
    ExporterSystem::OnSceneReset();
}

/*
Maya's scene loading process:
-User selects scene file
-BeforeOpenSceneCallback() is called by Maya. At this point, the previous scene data is still present
-The new scene data is loaded by Maya
-AfterOpenSceneCallback() is called by Maya
*/

static void BeforeOpenSceneCallback(void* clientData)
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("BeforeOpenSceneCallback()"));

    ExporterSystem::OnBeforeOpenScene();
}

static void AfterOpenSceneCallback(void* clientData)
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("AfterOpenSceneCallback()"));

    ExporterSystem::OnAfterOpenScene();

    auto sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings(false);
    if (sceneSettings.IsValid())
        sceneSettings.Upgrade();
}

static void BeforeSaveSceneCallback(void* clientData)
{
}

static void AfterSaveSceneCallback(void* clientData)
{
}

static void BeforeImportSceneCallback(void* clientData)
{
    ExporterSystem::OnBeforeMerge();
}

static void AfterImportSceneCallback(void* clientData)
{
    ExporterSystem::OnAfterMerge();
    FinjinSceneSettingsAccessor::MergeSceneSettings();
}

static void BeforeExportSceneCallback(void* clientData)
{
}

static void AfterExportSceneCallback(void* clientData)
{
}

static void NodeAddedCallback(MObject& nodeIterator, void* clientData)
{
    //Do nothing
}

static void NodeRemovedCallback(MObject& nodeIterator, void* clientData)
{
    auto sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings(false);
    if (sceneSettings.IsValid() && sceneSettings.obj != nodeIterator)
        sceneSettings.RemoveObjectSettings(nodeIterator);
}

/** Plugin entry point. */
MStatus initializePlugin(MObject obj)
{
    MFnPlugin plugin(obj, "Finjin", FinjinVersion::VERSION_TEXT_CSTR, "Any");

    InitializeCommon();

    //Register translators
    #define REGSITER_TRANSLATOR(type) plugin.registerFileTranslator(type::GetTranslatorName(), 0, type::Creator);
    REGSITER_TRANSLATOR(FinjinSceneTranslator_FSTD);
    REGSITER_TRANSLATOR(FinjinSceneTranslator_FSBD);
    REGSITER_TRANSLATOR(FinjinSceneTranslator_JSON);
    REGSITER_TRANSLATOR(FinjinSceneTranslator_CFG);
    REGSITER_TRANSLATOR(FinjinCustomHelperIconTranslator);
    #undef REGSITER_TRANSLATOR

    //Register commands
    #define REGISTER_COMMAND(type) plugin.registerCommand(type::GetCommandName(), type::Creator)
    REGISTER_COMMAND(ExportSceneCommand);
    REGISTER_COMMAND(ExportSelectedObjectsCommand);
    REGISTER_COMMAND(ExportSceneMaterialsCommand);
    REGISTER_COMMAND(ExportSelectedHypershadeMaterialsCommand);
    REGISTER_COMMAND(SceneSettingsCommand);
    REGISTER_COMMAND(ObjectSettingsCommand);
    REGISTER_COMMAND(GlobalSettingsCommand);
    REGISTER_COMMAND(RunExternalViewerCommand);
    REGISTER_COMMAND(RunExternalViewerWithLastExportedFileCommand);
    REGISTER_COMMAND(CheckDuplicateObjectNamesCommand);
    REGISTER_COMMAND(CreateCustomDummyCommand);
    REGISTER_COMMAND(CreateStaticGeometryCommand);
    REGISTER_COMMAND(CreateInstancedGeometryCommand);
    REGISTER_COMMAND(ViewLogCommand);
    REGISTER_COMMAND(HelpCommand);
    REGISTER_COMMAND(AboutCommand);
    REGISTER_COMMAND(ChooseObjectDialogCommand);
    REGISTER_COMMAND(ChooseObjectsDialogCommand);
    REGISTER_COMMAND(ChooseMaterialDialogCommand);
    REGISTER_COMMAND(FinjinCustomDummyChooseCustomIconCommand);
    REGISTER_COMMAND(FinjinCustomDummyUseDefaultIconCommand);
    REGISTER_COMMAND(FinjinEnableDetailedLoggingCommand);
    REGISTER_COMMAND(FinjinGetVersionCommand);
    REGISTER_COMMAND(FinjinGetSceneSettingsCommand);
    REGISTER_COMMAND(FinjinGetObjectSettingsCommand);
    REGISTER_COMMAND(FinjinAddMeshAnimationCommand);
    REGISTER_COMMAND(FinjinAddNodeAnimationCommand);
    REGISTER_COMMAND(FinjinSyncSubmeshSettingsCommand);
    #undef REGISTER_COMMAND

    //Finjin menu
    MGlobal::executeCommand(MString("menu -tearOff true -label \"") + FINJIN_MENU_LABEL + MString("\" -parent $gMainWindow"));
        MGlobal::executeCommand("menuItem -tearOff true -subMenu true -label \"Export\"");
            MGlobal::executeCommand(MString("menuItem -command \"") + ExportSceneCommand::GetCommandName() + MString("\" -label \"Export Scene...\""));
            MGlobal::executeCommand(MString("menuItem -command \"") + ExportSelectedObjectsCommand::GetCommandName() + MString("\" -label \"Export Selected Objects...\""));
            MGlobal::executeCommand("menuItem -divider true");
            MGlobal::executeCommand(MString("menuItem -command \"") + ExportSceneMaterialsCommand::GetCommandName() + MString("\" -label \"Export Scene Materials...\""));
            MGlobal::executeCommand(MString("menuItem -command \"") + ExportSelectedHypershadeMaterialsCommand::GetCommandName() + MString("\" -label \"Export Selected Hypershade Materials...\""));
            MGlobal::executeCommand("setParent -menu ..");
        MGlobal::executeCommand("menuItem -divider true");
        MGlobal::executeCommand("menuItem -tearOff true -subMenu true -label \"External Viewer\"");
            MGlobal::executeCommand(MString("menuItem -command \"") + RunExternalViewerWithLastExportedFileCommand::GetCommandName() + MString("\" -label \"Run With Last Exported File...\""));
            MGlobal::executeCommand(MString("menuItem -command \"") + RunExternalViewerCommand::GetCommandName() + MString("\" -label \"Run...\""));
            MGlobal::executeCommand("setParent -menu ..");
        MGlobal::executeCommand("menuItem -divider true");
        MGlobal::executeCommand(MString("menuItem -command \"") + GlobalSettingsCommand::GetCommandName() + MString("\" -label \"Global Settings...\""));
        MGlobal::executeCommand(MString("menuItem -command \"") + SceneSettingsCommand::GetCommandName() + MString("\" -label \"Scene Settings...\""));
        MGlobal::executeCommand(MString("menuItem -command \"") + ObjectSettingsCommand::GetCommandName() + MString("\" -label \"Object Settings...\""));
        MGlobal::executeCommand("menuItem -divider true");
        MGlobal::executeCommand(MString("menuItem -command \"") + CheckDuplicateObjectNamesCommand::GetCommandName() + MString("\" -label \"Check Duplicate Object Names\""));
        MGlobal::executeCommand("menuItem -divider true");
        MGlobal::executeCommand(MString("menuItem -command \"") + CreateCustomDummyCommand::GetCommandName() + MString("\" -label \"Create Custom Dummy\""));
        //MGlobal::executeCommand(MString("menuItem -command \"") + CreateStaticGeometryCommand::GetCommandName() + MString("\" -label \"Create Static Geometry\""));
        //MGlobal::executeCommand(MString("menuItem -command \"") + CreateInstancedGeometryCommand::GetCommandName() + MString("\" -label \"Create Instanced Geometry\""));
        MGlobal::executeCommand("menuItem -divider true");
        MGlobal::executeCommand(MString("menuItem -command \"") + ViewLogCommand::GetCommandName() + MString("\" -label \"View Log...\""));
        MGlobal::executeCommand("menuItem -divider true");
        MGlobal::executeCommand(MString("menuItem -command \"") + HelpCommand::GetCommandName() + MString("\" -label \"Help...\""));
        MGlobal::executeCommand(MString("menuItem -command \"") + AboutCommand::GetCommandName() + MString("\" -label \"About Finjin...\""));

    //Node types
    #define REGISTER_NODE(type, nodeType, classification) plugin.registerNode(type::TYPE_NAME, type::TYPE_ID, type::Creator, type::Initialize, nodeType, classification)
    REGISTER_NODE(FinjinSceneSettingsNode, MPxNode::kDependNode, 0);
    REGISTER_NODE(FinjinCustomDummy, MPxNode::kLocatorNode, 0);
    REGISTER_NODE(FinjinStaticGeometry, MPxNode::kLocatorNode, 0);
    REGISTER_NODE(FinjinInstancedGeometry, MPxNode::kLocatorNode, 0);
    #undef REGISTER_NODE

    //Add callbacks
    mayaExitingCallbackID = MSceneMessage::addCallback(MSceneMessage::kMayaExiting, MayaExitingCallback);
    beforeNewCallbackID = MSceneMessage::addCallback(MSceneMessage::kBeforeNew, BeforeNewSceneCallback);
    beforeOpenCallbackID = MSceneMessage::addCallback(MSceneMessage::kBeforeOpen, BeforeOpenSceneCallback);
    afterOpenCallbackID = MSceneMessage::addCallback(MSceneMessage::kAfterOpen, AfterOpenSceneCallback);
    beforeSaveCallbackID = MSceneMessage::addCallback(MSceneMessage::kBeforeSave, BeforeSaveSceneCallback);
    afterSaveCallbackID = MSceneMessage::addCallback(MSceneMessage::kAfterSave, AfterSaveSceneCallback);
    beforeImportCallbackID = MSceneMessage::addCallback(MSceneMessage::kBeforeImport, BeforeImportSceneCallback);
    afterImportCallbackID = MSceneMessage::addCallback(MSceneMessage::kAfterImport, AfterImportSceneCallback);
    beforeExportCallbackID = MSceneMessage::addCallback(MSceneMessage::kBeforeExport, BeforeExportSceneCallback);
    afterExportCallbackID = MSceneMessage::addCallback(MSceneMessage::kAfterExport, AfterExportSceneCallback);
    nodeAddedCallbackID = MDGMessage::addNodeAddedCallback(NodeAddedCallback);
    nodeRemovedCallbackID = MDGMessage::addNodeRemovedCallback(NodeRemovedCallback);

    ApplicationAccessor::RegisterTypes();

    return MS::kSuccess;
}

/** Plugin entry point. */
MStatus uninitializePlugin(MObject obj)
{
    MFnPlugin plugin(obj);

    //De-register translators
    plugin.deregisterFileTranslator(FinjinSceneTranslator_FSTD::GetTranslatorName());
    plugin.deregisterFileTranslator(FinjinSceneTranslator_FSBD::GetTranslatorName());
    plugin.deregisterFileTranslator(FinjinSceneTranslator_JSON::GetTranslatorName());
    plugin.deregisterFileTranslator(FinjinSceneTranslator_CFG::GetTranslatorName());
    plugin.deregisterFileTranslator(FinjinCustomHelperIconTranslator::GetTranslatorName());

    //De-register commands
    plugin.deregisterCommand(ExportSceneCommand::GetCommandName());
    plugin.deregisterCommand(ExportSelectedObjectsCommand::GetCommandName());
    plugin.deregisterCommand(ExportSceneMaterialsCommand::GetCommandName());
    plugin.deregisterCommand(ExportSelectedHypershadeMaterialsCommand::GetCommandName());
    plugin.deregisterCommand(SceneSettingsCommand::GetCommandName());
    plugin.deregisterCommand(ObjectSettingsCommand::GetCommandName());
    plugin.deregisterCommand(GlobalSettingsCommand::GetCommandName());
    plugin.deregisterCommand(RunExternalViewerCommand::GetCommandName());
    plugin.deregisterCommand(RunExternalViewerWithLastExportedFileCommand::GetCommandName());
    plugin.deregisterCommand(CheckDuplicateObjectNamesCommand::GetCommandName());
    plugin.deregisterCommand(CreateCustomDummyCommand::GetCommandName());
    plugin.deregisterCommand(CreateStaticGeometryCommand::GetCommandName());
    plugin.deregisterCommand(CreateInstancedGeometryCommand::GetCommandName());
    plugin.deregisterCommand(ViewLogCommand::GetCommandName());
    plugin.deregisterCommand(HelpCommand::GetCommandName());
    plugin.deregisterCommand(AboutCommand::GetCommandName());
    plugin.deregisterCommand(ChooseObjectDialogCommand::GetCommandName());
    plugin.deregisterCommand(ChooseObjectsDialogCommand::GetCommandName());
    plugin.deregisterCommand(ChooseMaterialDialogCommand::GetCommandName());
    plugin.deregisterCommand(FinjinCustomDummyChooseCustomIconCommand::GetCommandName());
    plugin.deregisterCommand(FinjinCustomDummyUseDefaultIconCommand::GetCommandName());
    plugin.deregisterCommand(FinjinEnableDetailedLoggingCommand::GetCommandName());
    plugin.deregisterCommand(FinjinGetVersionCommand::GetCommandName());
    plugin.deregisterCommand(FinjinGetSceneSettingsCommand::GetCommandName());
    plugin.deregisterCommand(FinjinGetObjectSettingsCommand::GetCommandName());
    plugin.deregisterCommand(FinjinAddMeshAnimationCommand::GetCommandName());
    plugin.deregisterCommand(FinjinAddNodeAnimationCommand::GetCommandName());
    plugin.deregisterCommand(FinjinSyncSubmeshSettingsCommand::GetCommandName());

    //Finjin menu
    MGlobal::executeCommand(MString("menu -deleteAllItems -label \"") + FINJIN_MENU_LABEL + MString("\""));

    //Node types
    plugin.deregisterNode(FinjinSceneSettingsNode::TYPE_ID);
    plugin.deregisterNode(FinjinCustomDummy::TYPE_ID);
    plugin.deregisterNode(FinjinStaticGeometry::TYPE_ID);
    plugin.deregisterNode(FinjinInstancedGeometry::TYPE_ID);

    //Remove callbacks
    MMessage::removeCallback(mayaExitingCallbackID);
    MMessage::removeCallback(beforeNewCallbackID);
    MMessage::removeCallback(beforeOpenCallbackID);
    MMessage::removeCallback(afterOpenCallbackID);
    MMessage::removeCallback(beforeSaveCallbackID);
    MMessage::removeCallback(afterSaveCallbackID);
    MMessage::removeCallback(beforeImportCallbackID);
    MMessage::removeCallback(afterImportCallbackID);
    MMessage::removeCallback(beforeExportCallbackID);
    MMessage::removeCallback(afterExportCallbackID);
    MMessage::removeCallback(nodeAddedCallbackID);
    MMessage::removeCallback(nodeRemovedCallbackID);

    ShutdownCommon();

    return MS::kSuccess;
}
