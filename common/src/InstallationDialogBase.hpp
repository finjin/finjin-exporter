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
#include "InstallationContext.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class PluginInstallationItem
    {
    public:
        virtual ~PluginInstallationItem() {}

        virtual const ApplicationVersion& GetApplicationVersion() const = 0;
        
        virtual bool CanInstall(const InstallationContext& context) const = 0;

        virtual bool IsInstallSlow() const {return false;}

        virtual void Install(InstallationContext& context) = 0;

        virtual void Uninstall(InstallationContext& context) = 0;
    };

    class OtherInstallationItem
    {
    public:
        virtual ~OtherInstallationItem() {}

        virtual wxString GetDisplayName() const = 0;

        virtual bool CanInstall(const InstallationContext& context) const = 0;
        
        virtual void Install(InstallationContext& context) = 0;

        virtual void Uninstall(InstallationContext& context) = 0;
    };

    class InstallationDialogBase : public wxDialog 
    {
    private:
        DECLARE_EVENT_TABLE()
    
    protected:
        wxRadioButton* installRadioButton;
        wxRadioButton* uninstallRadioButton;
        wxStaticText* aboutLabel;
        wxButton* installButton;
        wxButton* closeButton;
        wxStaticBoxSizer* versionsSizer;
        wxStaticBoxSizer* otherFilesSizer;
        wxCheckBox* showDetailsCheckBox;

    public:
        void SetApplicationName(const wxString& name);
        
        void SetExtraSuccessMessage(const wxString& footer, InstallationMode mode);
        
        void AddPluginInstallationItem(PluginInstallationItem* item);        
        void AddOtherInstallationItem(OtherInstallationItem* item); 

        enum
        {
            INSTALL_BUTTON,
            INSTALL_RADIO_BUTTON,
            UNINSTALL_RADIO_BUTTON
        };

        void OnCloseWindow(wxCloseEvent& event);
        void OnCloseButton(wxCommandEvent& event);
        void OnInstallRadioButton(wxCommandEvent& event);
        void OnUninstallRadioButton(wxCommandEvent& event);
        void OnInstallButton(wxCommandEvent& event);
        
        void RefreshCheckBoxStates(bool refreshOtherItems = false);
        void UpdateInstallationModeControls();

        bool AppendAdminMessageIfNecessary(wxString& message, const wxChar* prefix = nullptr);
    
    public:
        InstallationDialogBase();
        ~InstallationDialogBase();        

        void Create();

    private:
        InstallationMode installationMode;
        wxString applicationName;
        wxString extraSuccessMessage[(size_t)InstallationMode::COUNT];
        
        std::list<PluginInstallationItem*> pluginItems;
        std::vector<wxCheckBox*> pluginItemCheckboxes;

        std::list<OtherInstallationItem*> otherItems;
        std::vector<wxCheckBox*> otherItemCheckboxes;
    };

} }

#if defined(_WIN64) || defined(__LP64__)
    #define APPLICATION_PLATFORM wxT("x64")
#elif defined(__WXMSW__)
    #define APPLICATION_PLATFORM wxT("Win32")
#else
    #define APPLICATION_PLATFORM wxT("x86")
#endif

//Same as above, but without 'Win32'
#if defined(_WIN64) || defined(__LP64__)
    #define APPLICATION_PLATFORM_RAW wxT("x64")
#elif defined(__WXMSW__)
    #define APPLICATION_PLATFORM_RAW wxT("x86")
#else
    #define APPLICATION_PLATFORM_RAW wxT("x86")
#endif
