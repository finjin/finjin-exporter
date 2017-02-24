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
#include "AboutDialog.hpp"
#include "ApplicationAccessor.hpp"
#include "SystemUtilities.hpp"
#include "ApplicationControls.hpp"
#include "SettingsPage.hpp"
#include "FinjinVersion.hpp"
#include "FinjinGlobalSettings.hpp"
#include "Strings.hpp"

#if defined(__WXMAC__)
    #define DIALOG_WIDTH 615
    #define DIALOG_HEIGHT 556  //Logo not shown on MacOS
#else
    #define DIALOG_WIDTH 420
    #define DIALOG_HEIGHT 516
#endif

using namespace Finjin::Exporter;


//Local classes----------------------------------------------------------------
class AboutDialog_AcknowledgmentsPage : public SettingsPage 
{
public:
    AboutDialog_AcknowledgmentsPage
        (
        wxWindow* parent, 
        wxWindowID id = wxID_ANY, 
        const wxPoint& pos = wxDefaultPosition, 
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL 
        )
        : SettingsPage(parent, id, pos, size, style)
    {
        wxBoxSizer* topSizer;
        topSizer = new wxBoxSizer(wxVERTICAL);
        
        thirdPartyLibrariesLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("3rd Party Libraries"), wxDefaultPosition, wxDefaultSize, 0 );
        thirdPartyLibrariesLabel->Wrap( -1 );
        topSizer->Add( thirdPartyLibrariesLabel, 0, wxALL, 5 );
        
        thirdPartyLibrariesText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_NO_VSCROLL|wxTE_AUTO_URL|wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY );
        thirdPartyLibrariesText->SetValue
            (
            wxT("wxWidgets - http://www.wxwidgets.org\n")
            wxT("TinyXML-2 - http://www.grinninglizard.com/tinyxml2")
            );
        topSizer->Add( thirdPartyLibrariesText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
        
        SetSizer(topSizer);
        Layout();
    }

    void OnTextURL( wxTextUrlEvent& event )
    { 
        //There seems to be a bug with wxWidgets
        //The submitted event needs to be ignored if the mouse is moving, otherwise we'll end up handling
        //multiple messages, resulting in the URL being displayed multiple times
        auto& ev = event.GetMouseEvent();
        if (ev.Moving())
            return;

        //Show clicked URL in browser window
        auto url = this->thirdPartyLibrariesText->GetRange(event.GetURLStart(), event.GetURLEnd());
        SystemUtilities::ShowWebPage(url);
    }

    ~AboutDialog_AcknowledgmentsPage() {}    

protected:
    wxStaticText* thirdPartyLibrariesLabel;
    ApplicationTextCtrl* thirdPartyLibrariesText;

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(AboutDialog_AcknowledgmentsPage, SettingsPage)
    EVT_TEXT_URL(wxID_ANY, AboutDialog_AcknowledgmentsPage::OnTextURL)
END_EVENT_TABLE()


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(AboutDialog, FinjinDialog)
    EVT_CLOSE(AboutDialog::OnCloseWindow)
    EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, AboutDialog::OnNotebookPageChanged)
END_EVENT_TABLE()

AboutDialog::AboutDialog()
{       
}

AboutDialog::AboutDialog(wxWindow* parent)
{
    Create(parent);
}

bool AboutDialog::Create(wxWindow* parent)
{
    if (!FinjinDialog::Create(parent, wxID_ANY, wxT("About Finjin"), wxDefaultPosition, wxSize(DIALOG_WIDTH, DIALOG_HEIGHT)))
        return false;

    CreateControls();

    return true;
}

void AboutDialog::CreateControls()
{
    //Format Finjin/program version message
    auto versionMessage = wxString::Format
        (
        wxT("Version %s for %s %s"), 
        FinjinVersion::VERSION_TEXT.wx_str(), 
        ApplicationAccessor::APPLICATION_LONG_NAME.wx_str(),
        ApplicationAccessor::GetApplicationVersion().wx_str()
        );

    //License message    
    wxString licenseMessage;

    wxStaticText* descriptionText;
    wxHyperlinkCtrl* homeHyperlink;
    wxHyperlinkCtrl* githubHyperlink;
    wxStaticText* copyrightText;
    wxStaticText* versionText;
    wxStaticText* licenseText;
    wxButton* closeButton;

    SetSizeHints(wxDefaultSize, wxDefaultSize);
    
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* tabPagesSizer = nullptr;
    int tabControlProportion = 1;        

#if defined(__WXMSW__)
    wxStaticBitmap* logoBitmap;
    logoBitmap = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("IDB_ABOUT_LOGO"), wxBITMAP_TYPE_BMP_RESOURCE ), wxDefaultPosition, wxDefaultSize, 0 );
    logoBitmap->SetMinSize( wxSize( 350,138 ) );
    topSizer->Add( logoBitmap, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10);
#else
    topSizer->Add( wxEXPAND, 20 );
#endif
    
    wxBoxSizer* controlsSizer;
    controlsSizer = new wxBoxSizer(wxVERTICAL);
    
    descriptionText = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Finjin Exporter for ") + ApplicationAccessor::APPLICATION_LONG_NAME, wxDefaultPosition, wxDefaultSize, 0 );
    descriptionText->Wrap( -1 );
    controlsSizer->Add( descriptionText, 0, wxALIGN_CENTER|wxALL, 5 );
    
    homeHyperlink = new wxHyperlinkCtrl( this, wxID_ANY, wxT("finjin.com"), wxT("http://finjin.com"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    ApplicationControls::AdjustContainerBackgroundColor(homeHyperlink);
    controlsSizer->Add( homeHyperlink, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    githubHyperlink = new wxHyperlinkCtrl( this, wxID_ANY, wxT("github.com/finjin"), wxT("https://github.com/finjin"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    ApplicationControls::AdjustContainerBackgroundColor(githubHyperlink);
	controlsSizer->Add( githubHyperlink, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
    
    copyrightText = new ApplicationStaticTextCtrl( this, wxID_ANY, Strings::COPYRIGHT_INFO, wxDefaultPosition, wxDefaultSize, 0 );
    copyrightText->Wrap( -1 );
    controlsSizer->Add( copyrightText, 0, wxALIGN_CENTER|wxALL, 5 );
    
    versionText = new ApplicationStaticTextCtrl( this, wxID_ANY, versionMessage, wxDefaultPosition, wxDefaultSize, 0 );
    versionText->Wrap( -1 );
    controlsSizer->Add( versionText, 0, wxALIGN_CENTER|wxALL, 5 );

    licenseText = new ApplicationStaticTextCtrl( this, wxID_ANY, licenseMessage, wxDefaultPosition, wxDefaultSize, 0 );
    licenseText->Wrap( -1 );
    controlsSizer->Add( licenseText, 0, wxALIGN_CENTER|wxALL, 5 );
    
    tabControl = new TabControl( this, wxID_ANY, wxDefaultPosition, DEFAULT_TAB_CONTROL_SIZE, 0 );
    ApplicationControls::AdjustContainerBackgroundColor(tabControl);
    this->pages.SetTabControl(tabControl, tabPagesSizer);
    this->pages.AddPage(new AboutDialog_AcknowledgmentsPage(this->pages.GetTabParentWindow()), wxT("Acknowledgments"));
    
    controlsSizer->Add( tabControl, tabControlProportion, wxEXPAND | wxALL | TAB_CONTROL_SIZER_FLAGS, 5 );
    
    if (tabPagesSizer != nullptr)
        controlsSizer->Add( tabPagesSizer, 1, wxEXPAND | wxDOWN | wxRIGHT | wxLEFT, 5);
    
    closeButton = new wxButton( this, wxID_OK, wxT("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    controlsSizer->Add( closeButton, 0, wxALIGN_CENTER|wxDOWN | wxRIGHT | wxLEFT, 5 );
    
    topSizer->Add( controlsSizer, 1, wxEXPAND, 5 );
    
    copyrightText->Show(false);

    SetSizer(topSizer);
    
    Layout();
    Centre(wxBOTH);
}    

void AboutDialog::OnCloseWindow(wxCloseEvent& event)
{
    EndModal(wxID_OK);
}

void AboutDialog::OnNotebookPageChanged(wxNotebookEvent& event)
{
    this->pages.OnPageChanged();
}
