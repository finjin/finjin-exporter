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


//Includes---------------------------------------------------------------------
#include "FinjinSceneSettingsAccessor.hpp"
#include "ExtendedValueAccessor.hpp"
#include "LogFileDialog.hpp"
#include "UserDataTypes.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** 
     * A collection of utility functions used to display dialogs. 
     * Whenever new top-level dialogs are added to Finjin, those dialogs should be 
     * displayed using a utility function in the class so that any required 'fixing'
     * is performed as necessary.
     */
    class Dialogs
    {
    public:
        /** Creates whatever modeless dialogs are needed. */
        static void Initialize();

        /** 
         * Destroys all modeless dialogs being managed by this class. 
         * This should be called before the Finjin plugin is shut down.
         */
        static void Shutdown();

        /** 
         * Shows a message in a message box, using the application window as the parent.
         * @param message [in] - The message to display.
         * @param title [in] - The text shown in the title bar of the message box.
         * @param style [in] - The wxWidgets flags indicating the style of the message box.
         */
        static int ShowMessage
            (
            const wxString& message, 
            const wxString& title = wxEmptyString, 
            long style = wxOK
            );

        /** 
         * Shows a message in a message box.
         * @param parent [in] - The parent window.
         * @param message [in] - The message to display.
         * @param title [in] - The text shown in the title bar of the message box.
         * @param style [in] - The wxWidgets flags indicating the style of the message box.
         */
        static int ShowMessage
            (
            wxWindow* parent, 
            const wxString& message, 
            const wxString& title = wxEmptyString, 
            long style = wxOK
            );

        /**
         * Shows the scene settings dialog.
         * @param sceneSettings [in] - The scene settings. If not specified, the current scene settings 
         * will be retrieved.
         * @return The return value used to indicate whether the user had selected OK in the dialog.
         * Due to how the dialog is currently designed, true is always returned.
         */
        static bool ShowSceneSettingsDialog
            (
            FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor()
            );

        /**
         * Shows the scene settings dialog.
         * @param initialPage [in] - Index of the page to show. If a negative value is specified, the default page is used.
         * @param sceneSettings [in] - The scene settings. If not specified, the current scene settings 
         * will be retrieved.
         * @return The return value used to indicate whether the user had selected OK in the dialog.
         * Due to how the dialog is currently designed, true is always returned.
         */
        static bool ShowSceneSettingsDialog
            (
            int initialPage, 
            FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor()
            );

        /**
         * Shows the scene settings dialog.
         * @param initialPage [in] - The name of the page to show. If an empty string is specified, the default page is used.
         * @param sceneSettings [in] - The scene settings. If not specified, the current scene settings 
         * will be retrieved.
         * @return The return value used to indicate whether the user had selected OK in the dialog.
         * Due to how the dialog is currently designed, true is always returned.
         */
        static bool ShowSceneSettingsDialog
            (
            const wxString& initialPageTitle, 
            FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor()
            );
        
        /**
         * Shows the object settings dialog, using the current selection set.
         * After the user closes the dialog by pressing OK, the object settings are copied to all the objects
         * in the current selection set.
         * @param sceneSettings [in] - The scene settings. If not specified, the current scene settings 
         * will be retrieved.
         * @return If the object settings dialog was shown, true is returned. Otherwise, false is returned.
         */
        static bool ShowObjectSettingsDialog
            (
            FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor()
            );

        /**
         * Shows the global settings dialog.
         * @return If the user selected OK in the dialog. true is returned. Otherwise, false is returned.
         */
        static bool ShowGlobalSettingsDialog();
        static bool ShowGlobalSettingsDialog(int initialPage);
        
        static bool ShowVertexFormatDialog(wxWindow* parent = nullptr);

        /**
         * Shows the user data settings dialog.
         * @param userDataSettings [in/out] - The accessor for the user data. This will be used
         * for retrieving the data when the dialog is shown, and putting the data back when it is dismissed.
         * @param classUsage [in] - Indicates what type of classes to allow.
         * @param objectName [in] - The name of the object that owns the user data. This is used for
         * formatting the string shown in the dialog's title bar.
         * @param sceneSettings [in] - The scene settings. If not specified, the current scene settings 
         * will be retrieved.
         * @param parent [in] - The dialog's parent window. If not specified, the application window will
         * be used.
         * @return If the user selected OK in the dialog. true is returned. Otherwise, false is returned.
         */
        static bool ShowUserDataSettingsDialog
            (
            UserDataSettingsAccessor& userDataSettings, 
            UserDataUsage classUsage, 
            const wxString& objectName,
            FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor(), 
            wxWindow* parent = nullptr
            );
        
        /**
         * Shows the user data class settings dialog. This is the dialog that lets you configure
         * where user data classes, which are used with user data, are located.
         * @param parent [in] - The dialog's parent window. If not specified, the application window will
         * be used.
         * @return If the user selected OK in the dialog. true is returned. Otherwise, false is returned.
         */
        static bool ShowUserDataClassSettingsDialog(wxWindow* parent = nullptr);

        /**
         * Shows all the extended values managed by the accessor in a dialog. 
         * This is useful for debugging.
         * @param extendedValueAccessor [in/out] - The accessor for the extended values. 
         * @param objectName [in] - The name of the object that owns the extended values. This is used for
         * formatting the string shown in the dialog's title bar.
         */
        static void ShowExtendedValues
            (
            ExtendedValueAccessor& extendedValueAccessor, 
            const wxString& objectName = wxEmptyString
            );
        
        /** Shows the dialog for the main Finjin log file. */
        static void ShowLogFileDialog();

        /** 
         * Shows a dialog for the specified log file.
         * @param fileName [in] - The name of the log file.
         * @param modeless [in] - Indicates whether a modeless dialog should be shown. If false, a modal
         * dialog is used.
         * @param monitorMethod [in] - The method used to monitor the log file.
         */
        static void ShowLogFileDialog
            (
            const wxString& fileName, 
            bool modeless, 
            LogFileDialog::MonitorMethod monitorMethod = LogFileDialog::MonitorMethod::NONE
            );
        
        /** Shows the dialog that indicates which objects have duplicate names. */
        static void ShowDuplicateObjectNamesDialog();

        /**
         * Shows the Finjin 'About' dialog.
         */
        static void ShowAboutDialog();
        
        static wxWindow* GetNullParent();

    private:
        typedef std::list<wxWindow*> ModelessDialogs;

        /** 
         * A list of all the modeless dialogs. This list is cleared when Shutdown()
         * is called.
         */
        static ModelessDialogs modelessDialogs;

        typedef std::unordered_map<wxString, wxWindow*> ModelessLogFileDialogs;

        /** 
         * A list of all the modeless log file dialogs. These are just pointers to dialogs stored
         * in the modelessDialogs list.
         */
        static ModelessLogFileDialogs modelessLogFileDialogs;

        static wxWindow* nullParent;
    };

} }
