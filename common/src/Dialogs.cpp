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
#include "Dialogs.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "wxApplicationWindow.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "FinjinObjectSettingsAccessor.hpp"
#include "Strings.hpp"
#include "ApplicationAccessor.hpp"
#include "SceneSettingsDialog.hpp"
#include "ObjectSettingsDialog.hpp"
#include "GlobalSettingsDialog.hpp"
#include "UserDataSettingsDialog.hpp"
#include "GlobalSettingsDialog_UserDataClassPage.hpp"
#include "AboutDialog.hpp"
#include "FinjinResourceManager.hpp"
#include "ModalDialogFix.hpp"
#include "FileUtilities.hpp"
#include "GlobalSettingsDialog_GeneralPage.hpp"
#include "GlobalSettingsDialog_MeshesPage.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
Dialogs::ModelessDialogs Dialogs::modelessDialogs;
Dialogs::ModelessLogFileDialogs Dialogs::modelessLogFileDialogs;
wxWindow* Dialogs::nullParent = nullptr;


//Implementation---------------------------------------------------------------
void Dialogs::Initialize()
{
#if defined(__WXMAC__)
    nullParent = new wxApplicationWindow(false);
#endif
}

void Dialogs::Shutdown()
{
    for (auto dialog : modelessDialogs)
    {
        auto parent = dialog->GetParent();
        if (parent != nullptr)
        {
            ApplicationAccessor::UnregisterModelessDialog(dialog);
            if (parent != nullParent)
            {
                dialog->Reparent(nullParent);
                parent->Destroy();
            }
        }
        dialog->Destroy();
    }

    modelessDialogs.clear();

    modelessLogFileDialogs.clear();

    if (nullParent != nullptr)
    {
        nullParent->Destroy();
        nullParent = nullptr;
    }
}

int Dialogs::ShowMessage(const wxString& message, const wxString& title, long style)
{
    if (!ApplicationAccessor::InQuietMode())
    {
        wxApplicationWindow appWindow;
        SUSPEND_CUSTOM_CONTROLS;
        wxMessageDialog dialog(&appWindow, message, title, style | wxCENTER);
        return dialog.ShowModal();
    }
    else
        return wxID_CANCEL;
}

int Dialogs::ShowMessage(wxWindow* parent, const wxString& message, const wxString& title, long style)
{
    if (!ApplicationAccessor::InQuietMode())
    {
        SUSPEND_CUSTOM_CONTROLS;
        wxMessageDialog dialog(parent, message, title, style | wxCENTER);
        return dialog.ShowModal();
    }
    else
        return wxID_CANCEL;
}

bool Dialogs::ShowSceneSettingsDialog(FinjinSceneSettingsAccessor sceneSettings)
{
    return ShowSceneSettingsDialog(-1, sceneSettings);
}

bool Dialogs::ShowSceneSettingsDialog(int initialPage, FinjinSceneSettingsAccessor sceneSettings)
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("Dialogs::ShowSceneSettingsDialog(%d)"), initialPage);

    if (!sceneSettings.IsValid())
        sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();

    auto appWindow = new wxApplicationWindow();
    SUSPEND_CUSTOM_CONTROLS;
    SceneSettingsDialog dialog(appWindow, sceneSettings, initialPage);
    ModalDialogFix modalDialogFix(&dialog);
    bool result = dialog.ShowModal() == wxID_OK;
        
    return result;
}

bool Dialogs::ShowSceneSettingsDialog(const wxString& initialPageTitle, FinjinSceneSettingsAccessor sceneSettings)
{
    if (!sceneSettings.IsValid())
        sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();

    auto appWindow = new wxApplicationWindow();
    SUSPEND_CUSTOM_CONTROLS;
    SceneSettingsDialog dialog(appWindow, sceneSettings, -1, initialPageTitle);
    ModalDialogFix modalDialogFix(&dialog);
    bool result = dialog.ShowModal() == wxID_OK;
    
    return result;
}

bool Dialogs::ShowObjectSettingsDialog(FinjinSceneSettingsAccessor sceneSettings)
{
    auto result = false;

    ObjectAccessorVector objects;
    ObjectAccessor::GetSelectedObjects(objects);
    if (objects.empty())
    {
        ShowMessage(Strings::OBJECT_SETTINGS_ERROR_NO_SELECTED_OBJECT, Strings::OBJECT_SETTINGS_ERROR_CAPTION, wxINFORMATION_DIALOG_FLAGS);
    }
    else
    {
        if (!sceneSettings.IsValid())
            sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();
        auto objectSettings = sceneSettings.GetObjectSettings(objects[0]);
                
        auto appWindow = new wxApplicationWindow();
        SuspendCustomControlsHook suspendHook;
        ObjectSettingsDialog dialog(appWindow, objects[0], objectSettings, sceneSettings);
        ModalDialogFix modalDialogFix(&dialog);
        suspendHook.Unsuspend(); //For some reason, if this isn't present there is a crash
        if (dialog.ShowModal() == wxID_OK)
        {
            for (size_t i = 1; i < objects.size(); i++)
            {
                auto selectedObjectSettings = sceneSettings.GetObjectSettings(objects[i]);
                selectedObjectSettings.CopyFrom(objectSettings);
            }

            result = true;
        }        
    }

    return result;
}

bool Dialogs::ShowGlobalSettingsDialog()
{
    return ShowGlobalSettingsDialog(-1);    
}

bool Dialogs::ShowGlobalSettingsDialog(int initialPage)
{
    auto appWindow = new wxApplicationWindow();
    SUSPEND_CUSTOM_CONTROLS;
    GlobalSettingsDialog dialog(appWindow, initialPage, wxT(""));
    ModalDialogFix modalDialogFix(&dialog);
    return dialog.ShowModal() == wxID_OK;
}

bool Dialogs::ShowUserDataSettingsDialog
    (
    UserDataSettingsAccessor& userDataSettings, 
    UserDataUsage classUsage, 
    const wxString& objectName,
    FinjinSceneSettingsAccessor sceneSettings, 
    wxWindow* parent
    )
{
    if (!sceneSettings.IsValid())
        sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();

    SuspendCustomControlsHook suspendHook;
    if (parent == nullptr)
    {
        auto appWindow = new wxApplicationWindow();
        UserDataSettingsDialog dialog(appWindow, objectName, userDataSettings, classUsage, sceneSettings);
        ModalDialogFix modalDialogFix(&dialog);
        suspendHook.Unsuspend(); //For some reason, if this isn't present there is a crash
        return dialog.ShowModal() == wxID_OK;
    }
    else
    {
        UserDataSettingsDialog dialog(parent, objectName, userDataSettings, classUsage, sceneSettings);
        ModalDialogFix modalDialogFix(&dialog);
        suspendHook.Unsuspend(); //For some reason, if this isn't present there is a crash
        return dialog.ShowModal() == wxID_OK;
    }
}

bool Dialogs::ShowVertexFormatDialog(wxWindow* parent)
{
    SUSPEND_CUSTOM_CONTROLS;
    if (parent == nullptr)
    {
        auto appWindow = new wxApplicationWindow();
        GlobalSettingsDialog dialog(appWindow, -1, GlobalSettingsDialog_MeshesPage::TITLE);
        ModalDialogFix modalDialogFix(&dialog);
        return dialog.ShowModal() == wxID_OK;
    }
    else
    {
        GlobalSettingsDialog dialog(parent, -1, GlobalSettingsDialog_MeshesPage::TITLE);
        ModalDialogFix modalDialogFix(&dialog);
        return dialog.ShowModal() == wxID_OK;
    }

    return false;
}

bool Dialogs::ShowUserDataClassSettingsDialog(wxWindow* parent)
{
    SUSPEND_CUSTOM_CONTROLS;
    if (parent == nullptr)
    {
        auto appWindow = new wxApplicationWindow();
        GlobalSettingsDialog dialog(appWindow, -1, GlobalSettingsDialog_UserDataClassPage::TITLE);
        ModalDialogFix modalDialogFix(&dialog);
        return dialog.ShowModal() == wxID_OK;
    }
    else
    {
        GlobalSettingsDialog dialog(parent, -1, GlobalSettingsDialog_UserDataClassPage::TITLE);
        ModalDialogFix modalDialogFix(&dialog);
        return dialog.ShowModal() == wxID_OK;
    }
}

void Dialogs::ShowExtendedValues(ExtendedValueAccessor& extendedValueAccessor, const wxString& objectName)
{
    std::vector<ExtendedValue> extendedValues;
    std::vector<int> ids;
    extendedValueAccessor.GetAllExtendedValues(extendedValues, &ids);

    wxString message = wxString::Format(wxT("There are %d extended values\n"), (int)extendedValues.size());
    FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, message);

    for (size_t i = 0; i < extendedValues.size(); i++)
    {
        wxString messageLine;
        messageLine += wxString::Format
            (
            wxT("   Extended value %d: id=%d, type=%s[%d], value=%s"), 
            (int)i,
            ids[i], 
            extendedValues[i].GetTypeString().wx_str(), 
            extendedValues[i].GetCount(),
            extendedValues[i].ToString(false).wx_str()
            );

        FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, messageLine);

        message += messageLine;
        message += wxT("\n");
    }
    
    wxString title;
    if (objectName.empty())
        title = wxT("Extended Values");
    else
        title = wxString::Format(wxT("Extended Values for %s"), objectName.wx_str());
    
    ShowMessage(message, title);
}

void Dialogs::ShowLogFileDialog()
{
    auto logFileName = FileUtilities::JoinPath(ApplicationAccessor::GetFinjinHomeDirectory(true), FINJIN_EXPORTER_LOG_FILE_NAME);
    ShowLogFileDialog(logFileName, true, LogFileDialog::MonitorMethod::NONE);
}

void Dialogs::ShowLogFileDialog(const wxString& fileName, bool modeless, LogFileDialog::MonitorMethod monitorMethod)
{
    if (modeless)
    {
        LogFileDialog* dialog = nullptr;

        auto foundAt = modelessLogFileDialogs.find(fileName);
        if (foundAt != modelessLogFileDialogs.end())
        {
            //The log dialog already exists
            dialog = (LogFileDialog*)foundAt->second;
        }
        else
        {
            //Create the dialog
            auto appWindow = new wxApplicationWindow(false);
            SUSPEND_CUSTOM_CONTROLS;
            dialog = new LogFileDialog(appWindow, fileName, monitorMethod);
            
            //Register it with application
            ApplicationAccessor::RegisterModelessDialog(dialog);

            //Store modeless dialog
            modelessDialogs.push_back(dialog);

            //Store log dialog as well, in case the file needs to be shown modelessly again
            modelessLogFileDialogs[fileName] = dialog;
        }

        dialog->Show();
        dialog->Raise();
    }
    else
    {
        auto appWindow = new wxApplicationWindow();
        SUSPEND_CUSTOM_CONTROLS;
        LogFileDialog dialog(appWindow, fileName, monitorMethod);
        ModalDialogFix modalDialogFix(&dialog);
        dialog.ShowModal();    
    }
}

void Dialogs::ShowDuplicateObjectNamesDialog()
{
    ObjectAccessorVector allObjects;
    ObjectAccessor::GetRootObject().GetAllObjects(allObjects);

    ObjectAccessorVector duplicateObjects;
    duplicateObjects.reserve(allObjects.size());

    std::unordered_set<wxString> uniqueObjectNames;

    wxString objectName;
    for (size_t i = 0; i < allObjects.size(); i++)
    {
        objectName = allObjects[i].GetLocalName();
        if (uniqueObjectNames.find(objectName) != uniqueObjectNames.end())
        {
            //Object name found. Record duplicate object
            duplicateObjects.push_back(allObjects[i]);
        }
        else
        {
            //Name not found. Record it
            uniqueObjectNames.insert(objectName);
        }
    }

    wxApplicationWindow appWindow;
    SUSPEND_CUSTOM_CONTROLS;
    if (duplicateObjects.empty())
    {
        //No duplicate names
        wxMessageDialog dialog(&appWindow, Strings::NO_OBJECTS_HAVE_DUPLICATE_NAMES, Strings::DUPLICATE_OBJECT_NAME_CHECK, wxINFORMATION_DIALOG_FLAGS);
        dialog.ShowModal();
    }
    else
    {
        //There are some duplicate objects
        std::vector<wxString> duplicateObjectNames;
        duplicateObjectNames.resize(duplicateObjects.size());
        for (size_t i = 0; i < duplicateObjects.size(); i++)
            duplicateObjectNames[i] = duplicateObjects[i].GetLocalName();
        std::sort(duplicateObjectNames.begin(), duplicateObjectNames.end());

        wxString objectNamesString;
        for (size_t i = 0; i < duplicateObjectNames.size(); i++)
        {
            if (i == 0 || (i > 0 && duplicateObjectNames[i - 1] != duplicateObjectNames[i]))
            {
                objectNamesString += duplicateObjectNames[i];
                objectNamesString += wxT("\n");
            }
        }

        wxString message = wxString::Format(Strings::SOME_OBJECTS_HAVE_DUPLICATE_NAMES_FORMAT, objectNamesString.wx_str());
        wxMessageDialog dialog(&appWindow, message, Strings::DUPLICATE_OBJECT_NAME_CHECK, wxYES_NO | wxCENTER | wxICON_INFORMATION);
        if (dialog.ShowModal() == wxID_YES)
        {
            //User wants to select the objects
            ObjectAccessor::SetSelectedObjects(duplicateObjects);
        }
    }
}

void Dialogs::ShowAboutDialog()
{
    wxApplicationWindow* parent = new wxApplicationWindow();
    SUSPEND_CUSTOM_CONTROLS;
    AboutDialog dialog(parent);
    ModalDialogFix modalDialogFix(&dialog);
    dialog.ShowModal();
}

wxWindow* Dialogs::GetNullParent()
{
    return nullParent;
}
