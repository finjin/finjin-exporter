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
#include "UserDataSettingsPage.hpp"
#include "FinjinResourceManager.hpp"
#include "Strings.hpp"
#include "StringUtilities.hpp"
#include "Dialogs.hpp"
#include "ApplicationControls.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "ApplicationAccessor.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
const wxString UserDataSettingsPage::TITLE(wxT("User Data"));


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(UserDataSettingsPage, SettingsPage)
    EVT_CHOICE(UserDataSettingsPage::CLASS_CHOICE, UserDataSettingsPage::OnClassChoice)
    EVT_BUTTON(UserDataSettingsPage::GENERATE_ID_BUTTON, UserDataSettingsPage::OnGenerateIDButton)
    EVT_BUTTON(UserDataSettingsPage::CONFIGURE_BUTTON, UserDataSettingsPage::OnConfigureButton)
END_EVENT_TABLE()

UserDataSettingsPage::UserDataSettingsPage
    (
    wxWindow* parent,
    UserDataSettingsAccessor& userDataSettings,
    UserDataUsage classUsage,
    FinjinSceneSettingsAccessor sceneSettings
    ) :
    SettingsPage(parent, wxID_ANY, wxDefaultPosition, wxSize( 500,458 ), wxTAB_TRAVERSAL), settings(userDataSettings)
{
    this->classUsage = classUsage;
    this->sceneSettings = sceneSettings;
    if (!this->sceneSettings.IsValid())
        this->sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings();
    FinjinResourceManager::GetInstance().AddUserDataTypesChangedListener(this);

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* idReferenceSizer;
    idReferenceSizer = new wxBoxSizer( wxHORIZONTAL );

    idLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("ID"), wxDefaultPosition, wxDefaultSize, 0 );
    idLabel->Wrap( -1 );
    idReferenceSizer->Add( idLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    idText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 240,-1 ), 0 );
    idReferenceSizer->Add( idText, 0, wxRIGHT, 5 );

    generateIDButton = new wxButton( this, wxID_ANY, wxT("Generate"), wxDefaultPosition, wxDefaultSize, 0 );
    idReferenceSizer->Add( generateIDButton, 0, 0, 5 );

    idReferenceSizer->Add( 0, 0, 1, 0, 5 );

    topSizer->Add( idReferenceSizer, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

    wxBoxSizer* userDataClassSizer;
    userDataClassSizer = new wxBoxSizer( wxHORIZONTAL );

    userDataClassLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("User Data Class"), wxDefaultPosition, wxDefaultSize, 0 );
    userDataClassLabel->Wrap( -1 );
    userDataClassSizer->Add( userDataClassLabel, 0, wxALL, 5 );

    wxArrayString userDataClassChoiceChoices;
    userDataClassChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, userDataClassChoiceChoices, 0 );
    userDataClassChoice->SetSelection( 0 );
    userDataClassSizer->Add( userDataClassChoice, 1, 0, 5 );

    configureUserDataClassButton = new wxButton( this, wxID_ANY, wxT("Configure..."), wxDefaultPosition, wxDefaultSize, 0 );
    userDataClassSizer->Add( configureUserDataClassButton, 0, wxRIGHT|wxLEFT, 5 );

    topSizer->Add( userDataClassSizer, 0, wxEXPAND, 5 );

    userDataText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    topSizer->Add( userDataText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    userDataControlsWindow = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL|wxTAB_TRAVERSAL );
    userDataControlsWindow->SetScrollRate( 5, 5 );
    this->userDataControlsWindow->Connect(wxID_ANY, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(UserDataSettingsPage::OnCheckBox), 0, this);
    topSizer->Add( userDataControlsWindow, 1, wxEXPAND | wxALL, 5 );

    this->userDataClassChoice->SetId(CLASS_CHOICE);
    this->generateIDButton->SetId(GENERATE_ID_BUTTON);
    this->configureUserDataClassButton->SetId(CONFIGURE_BUTTON);

    SetGUIData();

    SetSizer(topSizer);

    Layout();
}

UserDataSettingsPage::~UserDataSettingsPage()
{
    FinjinResourceManager::GetInstance().RemoveUserDataTypesChangedListener(this);

    this->classControls.Destroy();
}

bool UserDataSettingsPage::GetGUIData()
{
    //ID edit
    this->settings.SetUserID(this->idText->GetValue());

    //User data
    int curSel = this->userDataClassChoice->GetSelection();
    if (curSel > 0)
    {
        //A class is selected

        //Class
        auto& userDataTypes = FinjinResourceManager::GetInstance().GetUserDataTypes();
        auto classType = userDataTypes.GetClass(curSel - 1, this->classUsage);
        this->settings.SetUserDataClass(classType->name);
        this->lastSelectedClassName = classType->name;

        //Data
        wxString userData;
        this->classControls.GetGUIData(userData);
        this->settings.SetUserData(userData);
    }
    else
    {
        //No class is selected
        this->settings.SetUserDataClass(wxEmptyString);
        this->settings.SetUserData(this->userDataText->GetValue());
    }

    return true;
}

bool UserDataSettingsPage::SetGUIData()
{
    //ID edit
    this->idText->SetValue(this->settings.GetUserID());

    //User data class and data
    this->lastSelectedClassName = wxEmptyString;
    this->userDataText->SetValue(this->settings.GetUserData());
    int curSel = FillClassComboBox(this->settings.GetUserDataClass());
    if (curSel > 0)
    {
        //A class is selected. Initialize its controls
        InitializeClassControls(curSel - 1);
    }

    UpdateDataGUIVisibilityAndLayout();

    return true;
}

void UserDataSettingsPage::OnClassChoice(wxCommandEvent& event)
{
    FlushDataGUIToText();

    int curSel = this->userDataClassChoice->GetSelection();
    if (curSel > 0)
    {
        //A class is selected. Initialize its controls
        InitializeClassControls(curSel - 1);
    }
    else
    {
        //No class is selected. Initialize the plain text edit control

        //Class
        this->lastSelectedClassName.clear();
    }

    UpdateDataGUIVisibilityAndLayout();
}

void UserDataSettingsPage::OnGenerateIDButton(wxCommandEvent& event)
{
    this->idText->SetValue(WxUuid::CreateRandom().ToString());
}

void UserDataSettingsPage::OnConfigureButton(wxCommandEvent& event)
{
    this->classControls.GetGUIData();

    Dialogs::ShowUserDataClassSettingsDialog(this);
}

void UserDataSettingsPage::OnCheckBox(wxCommandEvent& event)
{
    auto checkbox = static_cast<wxCheckBox*>(event.GetEventObject());
    if (this->classControls.HandleCheckBoxClick(checkbox))
    {
        this->userDataControlsWindow->Layout();
        Refresh();
    }
}

void UserDataSettingsPage::FlushDataGUIToText()
{
    if (!this->userDataText->IsShown())
    {
        wxString userData;
        this->classControls.GetGUIData(userData);
        this->userDataText->SetValue(userData);
    }
}

void UserDataSettingsPage::UpdateDataGUIVisibilityAndLayout()
{
    int curSel = this->userDataClassChoice->GetSelection();
    this->userDataText->Show(curSel == 0);
    this->userDataControlsWindow->Show(curSel != 0);
    Layout();
}

int UserDataSettingsPage::FillClassComboBox(const wxString& selectedClass)
{
    int selectedComboItem = 0;

    //Clear combo box
    this->userDataClassChoice->Clear();

    //Clear list of class names
    this->comboBoxClassNames.clear();

    //Add "None"
    this->userDataClassChoice->Freeze();
    this->userDataClassChoice->Append(Strings::NONE);

    //Add class names
    auto& userDataTypes = FinjinResourceManager::GetInstance().GetUserDataTypes();
    this->comboBoxClassNames.reserve(userDataTypes.classes.size());
    for (auto classIterator = userDataTypes.classes.begin(); classIterator != userDataTypes.classes.end(); ++classIterator)
    {
        auto classType = classIterator->second;

        //Make sure the class is supported by this tab page's usage
        if (classType->IsSupportedUsage(this->classUsage))
        {
            //Add class display name to combo box
            int currentItemIndex = this->userDataClassChoice->Append(classType->displayName);

            //Add class name to internal list
            this->comboBoxClassNames.push_back(classType->name);

            //Determine if the recently added item should be the selected item
            if (selectedClass == classType->name)
                selectedComboItem = currentItemIndex;
        }
    }
    this->userDataClassChoice->Thaw();
    this->userDataClassChoice->SetSelection(selectedComboItem);
    return selectedComboItem;
}

void UserDataSettingsPage::InitializeClassControls(int classIndex)
{
    auto& userDataTypes = FinjinResourceManager::GetInstance().GetUserDataTypes();
    auto classType = userDataTypes.GetClass(classIndex, this->classUsage);
    this->lastSelectedClassName = classType->name;

    this->classControls.Initialize(classType, this->userDataControlsWindow, this->userDataText->GetValue());

    Layout();
}

void UserDataSettingsPage::UserDataTypesChanging()
{
    FlushDataGUIToText();
}

void UserDataSettingsPage::UserDataTypesChanged()
{
    //Fill the combo box, hopefully selecting the previously selected class
    int curSel = FillClassComboBox(this->lastSelectedClassName);

    //Update the class controls
    if (curSel == 0)
    {
        //The previously selected class could not be found. Destroy controls
        this->lastSelectedClassName.clear();
        this->classControls.Destroy();
    }
    else
    {
        //The previously selected class was found. Update the controls
        InitializeClassControls(curSel - 1);
    }

    //Perform a general GUI update
    UpdateDataGUIVisibilityAndLayout();
}
