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
#include "InstallationDialogBase.hpp"
#include "InstallationDetailsDialog.hpp"
#if defined(__WXMSW__)
    #include <shlobj.h>
#endif

#if defined (__WXMAC__)
    #define DIALOG_WIDTH 360
    #define DIALOG_HEIGHT 443
#else
    #define DIALOG_WIDTH 200
    #define DIALOG_HEIGHT 443
#endif

using namespace Finjin::Exporter;


//Local functions--------------------------------------------------------------
static void AddIndent(wxString& message, int indent)
{
    for (int i = 0; i < indent; i++)
        message += wxT("     ");
}

static void AddString(wxString& message, const wxString& stringToAdd, size_t count = 1)
{
    for (size_t i = 0; i < count; i++)
        message += stringToAdd;
}

enum State
{
    FALSE_STATE,
    TRUE_STATE,
    ALL_STATES
};

static void FormatInstallationInfoDetails(wxString& message, const std::list<std::shared_ptr<InstallationContext::InstallationInfo> >& details, int indent, State state)
{
    for (auto detail : details)
    {
        if (state == ALL_STATES || (State)detail->result == state)
        {
            auto& detailMessage = detail->message;
            if (!detailMessage.empty())
            {
                //Output all lines of the message
                wxStringTokenizer detailMessageTokens(detailMessage, wxT("\n"));
                wxString longestLine;
                for (int detailMessageIndex = 0; detailMessageTokens.HasMoreTokens(); detailMessageIndex++)
                {
                    auto line = detailMessageTokens.GetNextToken();

                    //Indent first
                    AddIndent(message, indent);

                    //Prefix with a letter if the information item is a leaf
                    if (detail->childInfo.empty())
                        message += (detailMessageIndex == 0) ? wxT("-") : wxT(">");

                    //Output the message line and move to next line
                    message += line;
                    message += wxT("\n");

                    //Hold onto the longest line
                    if (line.length() > longestLine.length())
                        longestLine = line;
                }

                //Underline the message if it's at the root level
                if (indent == 0)
                {
                    //Add hyphens for each character
                    //Extra characters are needed since we're not using a fixed font
                    AddString(message, wxT("-"), longestLine.length() * 3 / 2);
                    message += wxT("\n");
                }
            }

            //Recurse
            FormatInstallationInfoDetails(message, detail->childInfo, indent + 1, state);

            //Add an extra newline to separate this chunk of text from the next one
            if (indent == 0)
                message += wxT("\n");
        }
    }
}

static void FormatInstallationInfo(wxString& message, const InstallationContext::InstallationInfo& installationInfo, State state)
{
    FormatInstallationInfoDetails(message, installationInfo.childInfo, 0, state);    
}


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(InstallationDialogBase, wxDialog)
    EVT_CLOSE(InstallationDialogBase::OnCloseWindow)
    EVT_RADIOBUTTON(InstallationDialogBase::INSTALL_RADIO_BUTTON, InstallationDialogBase::OnInstallRadioButton)
    EVT_RADIOBUTTON(InstallationDialogBase::UNINSTALL_RADIO_BUTTON, InstallationDialogBase::OnUninstallRadioButton)
    EVT_BUTTON(wxID_OK, InstallationDialogBase::OnCloseButton)    
    EVT_BUTTON(InstallationDialogBase::INSTALL_BUTTON, InstallationDialogBase::OnInstallButton)
END_EVENT_TABLE()

InstallationDialogBase::InstallationDialogBase()
{
    this->installationMode = InstallationMode::INSTALL;
    this->applicationName = wxT("???");    
}

InstallationDialogBase::~InstallationDialogBase()
{
    for (auto pluginItem = this->pluginItems.begin();
        pluginItem != this->pluginItems.end();
        ++pluginItem)
    {
        delete *pluginItem;
    }

    for (auto otherItem = this->otherItems.begin();
        otherItem != this->otherItems.end();
        ++otherItem)
    {
        delete *otherItem;
    }
}

void InstallationDialogBase::Create()
{
    wxDialog::Create(0, wxID_ANY, wxT("Install Finjin"), wxDefaultPosition, wxSize( DIALOG_WIDTH, DIALOG_HEIGHT ), wxDEFAULT_DIALOG_STYLE);

#if defined(__WXMSW__)
    SetIcon(wxIcon(wxT("IDI_FINJIN"), wxBITMAP_TYPE_ICO_RESOURCE));
#endif
    
    SetSizeHints( wxSize(DIALOG_WIDTH,-1), wxDefaultSize );
    
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer( wxVERTICAL );
    
    wxBoxSizer* topModeSizer;
    topModeSizer = new wxBoxSizer( wxVERTICAL );
    
    wxBoxSizer* modeCenterSizer;
    modeCenterSizer = new wxBoxSizer( wxHORIZONTAL );
    
    installRadioButton = new wxRadioButton( this, wxID_ANY, wxT("Install"), wxDefaultPosition, wxDefaultSize, 0 );
    installRadioButton->SetValue(this->installationMode == InstallationMode::INSTALL);
    modeCenterSizer->Add( installRadioButton, 0, wxALL, 5 );
    
    uninstallRadioButton = new wxRadioButton( this, wxID_ANY, wxT("Uninstall"), wxDefaultPosition, wxDefaultSize, 0 );
    uninstallRadioButton->SetValue(this->installationMode == InstallationMode::UNINSTALL);
    modeCenterSizer->Add( uninstallRadioButton, 0, wxALL, 5 );
    
    topModeSizer->Add( modeCenterSizer, 0, wxALIGN_CENTER_HORIZONTAL, 0 );
    
    topSizer->Add( topModeSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 8 );

    wxStaticBoxSizer* aboutSizer;
    aboutSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("About") ), wxVERTICAL );
    
    aboutLabel = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    aboutLabel->Wrap( -1 );
    aboutLabel->SetMinSize( wxSize( 170,50 ) );
    
    aboutSizer->Add( aboutLabel, 1, wxALL|wxEXPAND, 5 );
    
    topSizer->Add( aboutSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 8 );
    
    auto versionsText = wxString::Format(wxT("%s Versions to Install"), this->applicationName.wx_str());
    versionsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, versionsText ), wxVERTICAL );    
    topSizer->Add( versionsSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 8 );
    
    otherFilesSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxEmptyString ), wxVERTICAL );    
    topSizer->Add( otherFilesSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 8 );

    this->pluginItemCheckboxes.reserve(this->pluginItems.size());
    for (auto pluginItem = this->pluginItems.begin();
        pluginItem != this->pluginItems.end();
        ++pluginItem)
    {
        wxCheckBox* checkBox = new wxCheckBox(this, wxID_ANY, (*pluginItem)->GetApplicationVersion().displayName, wxDefaultPosition, wxDefaultSize, 0 );    
        this->pluginItemCheckboxes.push_back(checkBox);
        checkBox->SetValue(true);
        versionsSizer->Add(checkBox, 0, wxALL, 5 );    
    }

    this->otherItemCheckboxes.reserve(this->otherItems.size());
    for (auto otherItem = this->otherItems.begin();
        otherItem != this->otherItems.end();
        ++otherItem)
    {
        wxCheckBox* checkBox = new wxCheckBox(this, wxID_ANY, (*otherItem)->GetDisplayName(), wxDefaultPosition, wxDefaultSize, 0 );    
        this->otherItemCheckboxes.push_back(checkBox);
        checkBox->SetValue(true);
        otherFilesSizer->Add(checkBox, 0, wxALL, 5 );            
    }

    wxBoxSizer* detailsSizer;
    detailsSizer = new wxBoxSizer( wxHORIZONTAL );
    
    showDetailsCheckBox = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    
    detailsSizer->Add( showDetailsCheckBox, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxTOP, 7 );
    
    topSizer->Add( detailsSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    wxBoxSizer* buttonsSizer;
    buttonsSizer = new wxBoxSizer( wxHORIZONTAL );
    
    installButton = new wxButton( this, wxID_OK, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    buttonsSizer->Add( installButton, 0, wxBOTTOM|wxLEFT|wxRIGHT, 5 );
    
    closeButton = new wxButton( this, wxID_ANY, wxT("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsSizer->Add( closeButton, 0, wxBOTTOM|wxRIGHT, 5 );

    this->installRadioButton->SetId(INSTALL_RADIO_BUTTON);
    this->uninstallRadioButton->SetId(UNINSTALL_RADIO_BUTTON);
    this->installButton->SetId(INSTALL_BUTTON);
    this->closeButton->SetId(wxID_OK);

    topSizer->Add( buttonsSizer, 0, wxALIGN_CENTER, 5 );
    
    this->SetSizer( topSizer );
    this->Layout();

    RefreshCheckBoxStates(true);

    UpdateInstallationModeControls();
}

void InstallationDialogBase::SetApplicationName(const wxString& name)
{
    this->applicationName = name;
}

void InstallationDialogBase::SetExtraSuccessMessage(const wxString& footer, InstallationMode mode)
{
    this->extraSuccessMessage[(size_t)mode] = footer;
}

void InstallationDialogBase::AddPluginInstallationItem(PluginInstallationItem* item)
{
    this->pluginItems.push_back(item);
}

void InstallationDialogBase::AddOtherInstallationItem(OtherInstallationItem* item)
{
    this->otherItems.push_back(item);
}

void InstallationDialogBase::OnCloseWindow(wxCloseEvent& event)
{
    EndModal(wxID_OK);
}

void InstallationDialogBase::OnInstallRadioButton(wxCommandEvent& event)
{
    if (this->installationMode != InstallationMode::INSTALL)
    {
        this->installationMode = InstallationMode::INSTALL;
        UpdateInstallationModeControls();
    }
}

void InstallationDialogBase::OnUninstallRadioButton(wxCommandEvent& event)
{
    if (this->installationMode != InstallationMode::UNINSTALL)
    {
        this->installationMode = InstallationMode::UNINSTALL;
        UpdateInstallationModeControls();
    }
}

void InstallationDialogBase::OnCloseButton(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

void InstallationDialogBase::OnInstallButton(wxCommandEvent& event)
{    
    InstallationContext context;    
    context.mainWindow = this;

    wxString installLower;
    wxString installedLower;
    wxString installingUpper;
    wxString installationLower;    
    wxString installationUpper;    
    if (this->installationMode == InstallationMode::INSTALL)
    {
        installLower = wxT("install");
        installedLower = wxT("installed");
        installingUpper = wxT("Installing");
        installationLower = wxT("installation");
        installationUpper = wxT("Installation");
    }
    else
    {
        installLower = wxT("uninstall");
        installedLower = wxT("uninstalled");
        installingUpper = wxT("Uninstalling");
        installationLower = wxT("uninstallation");
        installationUpper = wxT("Uninstallation");
    }
        
    wxString title;
    int pluginIndex = 0;
    int handledPluginCount = 0;
    for (auto pluginItem = this->pluginItems.begin();
        pluginItem != this->pluginItems.end();
        ++pluginItem)
    {
        wxCheckBox* checkBox = this->pluginItemCheckboxes[pluginIndex++];
        if (checkBox->GetValue())
        {
            title = wxString::Format(wxT("Finjin %s for %s"), installationUpper.wx_str(), (*pluginItem)->GetApplicationVersion().displayName.wx_str());
            context.StartInstallationItem(title);

            //Install plugin items
            context.ClearFailureFlag();
            context.applicationVersion = (*pluginItem)->GetApplicationVersion();
            context.StartInstallationItem(wxT("Plugin Files"));
            if (this->installationMode == InstallationMode::INSTALL)                    
            {
                if ((*pluginItem)->IsInstallSlow() && handledPluginCount == 0)
                {
                    auto& appVersion = (*pluginItem)->GetApplicationVersion();
                    auto slowMessage = wxString::Format(wxT("The installation of the Finjin Exporter for %s will take a few minutes.\n\nPlease be patient during this process.\n\nPress OK to start the installation."), appVersion.displayName.wx_str());
                    wxMessageDialog dialog(this, slowMessage, wxT("Installing Finjin"), wxOK | wxICON_INFORMATION | wxCENTER);
                    dialog.ShowModal();
                }
                
                (*pluginItem)->Install(context);
            }
            else
                (*pluginItem)->Uninstall(context);
            handledPluginCount++;

            context.FinishInstallationItem();
            bool continueInstall = !context.GetFailureFlag();

            //Install other items
            if (continueInstall)
            {
                int otherIndex = 0;
                for (auto otherItem = this->otherItems.begin();
                    otherItem != this->otherItems.end();
                    ++otherItem)
                {
                    auto checkBox = this->otherItemCheckboxes[otherIndex++];
                    if (this->installationMode == InstallationMode::UNINSTALL || (*otherItem)->CanInstall(context))
                    {
                        title = wxString::Format(wxT("%s %s"), installingUpper.wx_str(), (*otherItem)->GetDisplayName().wx_str());

                        context.StartInstallationItem((*otherItem)->GetDisplayName());
                        if (this->installationMode == InstallationMode::INSTALL)
                            (*otherItem)->Install(context);
                        else
                            (*otherItem)->Uninstall(context);
                        context.FinishInstallationItem();
                    }                
                }
            }
            else
                context.LogSuccess(wxString::Format(wxT("Skipping %s of other files."), installationLower.wx_str()));

            context.FinishInstallationItem();
        }
    }

    wxString message;
    if (context.WasUsed())
    {
        bool success = context.ComputeInstallationResults();
        
        wxString extraSuccessMessage;
        if (success)
        {       
            extraSuccessMessage = this->extraSuccessMessage[(size_t)this->installationMode];

            if (this->installationMode == InstallationMode::INSTALL)
            {                
            #if defined(__WXMSW__)
                //if (!extraSuccessMessage.empty())
                //    extraSuccessMessage += wxT("\n\n");
                //extraSuccessMessage += wxT("Please install the DirectX (February 2010) runtimes if you have not already done so.");
            #endif
            }
        }

        if (this->showDetailsCheckBox->GetValue())
        {
            if (success)
            {
                message = wxString::Format(wxT("The Finjin Exporter was successfully %s.\n\n"), installedLower.wx_str());
                if (!extraSuccessMessage.empty())
                {
                    message += extraSuccessMessage;
                    message += wxT("\n\n");
                }
            }

            if (AppendAdminMessageIfNecessary(message))
                message += wxT("\n\n");

            FormatInstallationInfo(message, *context.installationInfo, ALL_STATES);        
            InstallationDetailsDialog resultsDialog(this, message, this->installationMode);
            resultsDialog.ShowModal();
        }
        else
        {
            if (success)
            {
                message = wxString::Format(wxT("The Finjin Exporter was successfully %s."), installedLower.wx_str());
                if (!extraSuccessMessage.empty())
                {
                    message += wxT("\n\n");
                    message += extraSuccessMessage;
                }
                AppendAdminMessageIfNecessary(message, wxT("\n\n"));

                wxMessageDialog dialog(this, message, wxString::Format(wxT("Finjin %s Finished"), installationUpper.wx_str()), wxOK | wxICON_INFORMATION | wxCENTER);
                dialog.ShowModal();
            }
            else
            {
                message = wxT("Errors were encountered while installing the Finjin Exporter.\n\n");                
                
                if (AppendAdminMessageIfNecessary(message))
                    message += wxT("\n\n");

                FormatInstallationInfo(message, *context.installationInfo, FALSE_STATE);

                InstallationDetailsDialog resultsDialog(this, message, this->installationMode);
                resultsDialog.ShowModal();
            }
        }            
    }
    else
    {
        message = wxString::Format(wxT("You must select at least one %s version to %s."), this->applicationName.wx_str(), installLower.wx_str());
        wxMessageDialog dialog(this, message, wxString::Format(wxT("Finjin %s Error"), installationUpper.wx_str()), wxOK | wxICON_INFORMATION | wxCENTER);
        dialog.ShowModal();
    }
}

void InstallationDialogBase::RefreshCheckBoxStates(bool refreshOtherItems)
{
    InstallationContext context;
    
    int pluginIndex = 0;
    for (auto pluginItem = this->pluginItems.begin();
        pluginItem != this->pluginItems.end();
        ++pluginItem)
    {
        auto checkBox = this->pluginItemCheckboxes[pluginIndex++];
        if ((*pluginItem)->CanInstall(context))
        {
            checkBox->SetValue(true);        
            checkBox->Enable(true);
        }
        else
        {
            checkBox->SetValue(false);
            checkBox->Enable(false);
        }
    }

    if (refreshOtherItems)
    {
        int otherIndex = 0;
        for (auto otherItem = this->otherItems.begin();
            otherItem != this->otherItems.end();
            ++otherItem)
        {
            auto checkBox = this->otherItemCheckboxes[otherIndex++];
            if ((*otherItem)->CanInstall(context))
            {
                checkBox->SetValue(true);        
                checkBox->Enable(true);
            }
            else
            {
                checkBox->SetValue(false);
                checkBox->Enable(false);
            }
        }
    }
}

void InstallationDialogBase::UpdateInstallationModeControls()
{
    bool show;
    wxString installLower;
    wxString installUpper;
    wxString installationLower;
    if (this->installationMode == InstallationMode::INSTALL)
    {
        show = true;
        installLower = wxT("install");
        installUpper = wxT("Install");
        installationLower = wxT("installation");        
    }
    else
    {
        show = false;
        installLower = wxT("uninstall");
        installUpper = wxT("Uninstall");
        installationLower = wxT("uninstallation");        
    }

    SetTitle(wxString::Format(wxT("%s Finjin"), installUpper.wx_str()));

    //Update "About" text
    this->aboutLabel->SetLabel(wxString::Format(wxT("This tool will %s the Finjin Exporter for each of the selected %s versions."), installLower.wx_str(), this->applicationName.wx_str()));
    
    //Update "Application Versions to Install/Uninstall" text
    wxString versionsText = wxString::Format(wxT("%s Versions to %s"), this->applicationName.wx_str(), installUpper.wx_str());
    this->versionsSizer->GetStaticBox()->SetLabel(versionsText);

    //Show "Other Files to Install"
    this->otherFilesSizer->GetStaticBox()->SetLabel(wxString::Format(wxT("Other Files to %s"), installUpper.wx_str()));
    this->otherFilesSizer->Show(show);

    //Update "Show Installation Details" checkbox
    this->showDetailsCheckBox->SetLabel(wxString::Format(wxT("Show %s details"), installationLower.wx_str()));

    //Update "Install" button
    this->installButton->SetLabel(wxString::Format(wxT("&%s"), installUpper.wx_str()));

    Fit();
    
    this->aboutLabel->Wrap(this->aboutLabel->GetClientSize().x);
}

bool InstallationDialogBase::AppendAdminMessageIfNecessary(wxString& message, const wxChar* prefix)
{
#if defined(__WXMSW__)
    if (!IsUserAnAdmin())
    {
        if (prefix != nullptr)
            message += prefix;

        message += wxString::Format(wxT("Note: If Finjin does not appear to initialize correctly when running %s, try running this installer again as an administrator."), this->applicationName.wx_str());
        
        return true;
    }
#endif
    return false;
}
