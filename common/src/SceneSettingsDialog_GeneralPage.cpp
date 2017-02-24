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
#include "SceneSettingsDialog_GeneralPage.hpp"
#include "FlagAliasDialog.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "StringUtilities.hpp"
#include "StringSplitter.hpp"
#include "Strings.hpp"
#include "ApplicationControls.hpp"
#include "ModalDialogFix.hpp"
#include "FinjinGlobalSettings.hpp"
#include "FlagDialog.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
const wxString SceneSettingsDialog_GeneralPage::TITLE(wxT("General"));


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(SceneSettingsDialog_GeneralPage, SettingsPage)
    EVT_BUTTON(SceneSettingsDialog_GeneralPage::ADD_FLAG_BUTTON, SceneSettingsDialog_GeneralPage::OnAddFlagButton)
END_EVENT_TABLE()

SceneSettingsDialog_GeneralPage::SceneSettingsDialog_GeneralPage( wxWindow* parent, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    this->sceneSettings = sceneSettings;

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxStaticBoxSizer* generalSettingsSizer;
    generalSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("General Settings") ), wxVERTICAL );
    
    wxFlexGridSizer* sceneManagerSizer;
    sceneManagerSizer = new wxFlexGridSizer( 1, 2, 0, 0 );
    sceneManagerSizer->SetFlexibleDirection( wxBOTH );
    sceneManagerSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    sceneManagerLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Scene Manager"), wxDefaultPosition, wxDefaultSize, 0 );
    sceneManagerLabel->Wrap( -1 );
    sceneManagerSizer->Add( sceneManagerLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
    
    customSceneManagerText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
    customSceneManagerText->SetMaxLength( 0 ); 
    sceneManagerSizer->Add( customSceneManagerText, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    
    generalSettingsSizer->Add( sceneManagerSizer, 0, wxEXPAND, 5 );
    
    
    wxBoxSizer* sceneScaleSizer;
    sceneScaleSizer = new wxBoxSizer( wxHORIZONTAL );
    
    sceneScaleLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Scale"), wxDefaultPosition, wxDefaultSize, 0 );
    sceneScaleLabel->Wrap( -1 );
    sceneScaleSizer->Add( sceneScaleLabel, 0, wxALL, 5 );
    
    wxArrayString sceneScaleChoiceChoices;
    sceneScaleChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, sceneScaleChoiceChoices, 0 );
    sceneScaleChoice->SetSelection( 0 );
    sceneScaleSizer->Add( sceneScaleChoice, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    divideByLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Divided By"), wxDefaultPosition, wxDefaultSize, 0 );
    divideByLabel->Wrap( -1 );
    sceneScaleSizer->Add( divideByLabel, 0, wxALL, 5 );
    
    dividedBySpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(70, -1), 0, 0, 1000, SpinnerControl::FLOAT_UNIT, 3);
    sceneScaleSizer->Add( dividedBySpinner, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
    
    generalSettingsSizer->Add( sceneScaleSizer, 0, wxEXPAND, 5 );
    
    wxFlexGridSizer* generalSettingsCheckboxesSizer;
    generalSettingsCheckboxesSizer = new wxFlexGridSizer( 3, 1, 0, 0 );
    generalSettingsCheckboxesSizer->SetFlexibleDirection( wxBOTH );
    generalSettingsCheckboxesSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    exportHierarchyCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Hierarchy"), wxDefaultPosition, wxDefaultSize, 0 );
    
    generalSettingsCheckboxesSizer->Add( exportHierarchyCheckbox, 0, wxALL, 5 );
    
    exportHiddenObjectsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Export Hidden Objects"), wxDefaultPosition, wxDefaultSize, 0 );
    
    generalSettingsCheckboxesSizer->Add( exportHiddenObjectsCheckbox, 0, wxALL, 5 );
    
    generalSettingsSizer->Add( generalSettingsCheckboxesSizer, 1, wxEXPAND, 5 );
    
    topSizer->Add( generalSettingsSizer, 0, wxEXPAND|wxTOP, 5 );
    
    wxBoxSizer* aliasesSizer;
    aliasesSizer = new wxBoxSizer( wxHORIZONTAL );
    
    wxStaticBoxSizer* flagAliasesSizer;
    flagAliasesSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Flags") ), wxVERTICAL );
    
    wxGridSizer* flagAliasesControlsSizer;
    flagAliasesControlsSizer = new wxGridSizer( 1, 2, 0, 0 );
    
    addFlagButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxDefaultSize, 0 );
    flagAliasesControlsSizer->Add( addFlagButton, 0, wxRIGHT|wxLEFT, 5 );
    
    flagAliasesSizer->Add( flagAliasesControlsSizer, 0, wxEXPAND, 5 );
    
    flagAliasesText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
    flagAliasesSizer->Add( flagAliasesText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    aliasesSizer->Add( flagAliasesSizer, 1, wxEXPAND|wxRIGHT, 5 );
    
    topSizer->Add( aliasesSizer, 1, wxEXPAND|wxTOP, 5 );

    this->addFlagButton->SetId(ADD_FLAG_BUTTON);
    
    SetSizer(topSizer);
    Layout();
}

SceneSettingsDialog_GeneralPage::~SceneSettingsDialog_GeneralPage()
{
}


bool SceneSettingsDialog_GeneralPage::GetGUIData()
{
    //Scene scale units
    int selectedScaleIndex = this->sceneScaleChoice->GetSelection();
    this->sceneSettings.SetSceneScaleUnit((wxChar*)this->sceneScaleChoice->GetClientData(selectedScaleIndex));
    
    //Scale "divide by"
    this->sceneSettings.SetSceneScaleDivide(this->dividedBySpinner->GetValue());
        
    //Export hierarchy
    this->sceneSettings.SetExportHierarchy(this->exportHierarchyCheckbox->GetValue());

    //Export hidden objects
    this->sceneSettings.SetIgnoreHiddenObjects(!this->exportHiddenObjectsCheckbox->GetValue());

    //Scene manager
    this->sceneSettings.SetSceneManager(this->customSceneManagerText->GetValue());
    
    //Flags
    this->sceneSettings.ClearFlags();

    wxString flagsText = this->flagAliasesText->GetValue();
    StringSplitter flags(flagsText, wxT('\n'), StringSplitterFlags::TRIM | StringSplitterFlags::IGNORE_EMPTY);

    std::set<wxString> flagNames;
    for (size_t i = 0; i < flags.size(); i++)
    {
        if (!StringUtilities::IsInteger(flags[i]))
        {
            //Make sure name is unique
            if (flagNames.find(flags[i]) == flagNames.end())
            {
                flagNames.insert(flags[i]);
                this->sceneSettings.AddFlag(flags[i]);
            }
        }
    }
    
    return true;
}

bool SceneSettingsDialog_GeneralPage::SetGUIData()
{
    //Scene scale units
    this->sceneScaleChoice->Freeze();
    for (int i = 0; i < Strings::STANDARD_SCENE_SCALE_COUNT; i++)
    {
        this->sceneScaleChoice->Append(Strings::STANDARD_SCENE_SCALES_TEXT[i]);
        this->sceneScaleChoice->SetClientData(i, (wxChar*)Strings::STANDARD_SCENE_SCALES_VALUE[i]);
        if (this->sceneSettings.GetSceneScaleUnit() == Strings::STANDARD_SCENE_SCALES_VALUE[i])
            this->sceneScaleChoice->SetSelection(i);
    }
    this->sceneScaleChoice->Thaw();
    if (this->sceneScaleChoice->GetSelection() < 0)
        this->sceneScaleChoice->SetSelection(0);

    //Scale "divide by" value    
    this->dividedBySpinner->SetValue(this->sceneSettings.GetSceneScaleDivide());

    //Export hierarchy
    this->exportHierarchyCheckbox->SetValue(this->sceneSettings.GetExportHierarchy());

    //Export hidden objects
    this->exportHiddenObjectsCheckbox->SetValue(!this->sceneSettings.GetIgnoreHiddenObjects());

    //Scene manager
    this->customSceneManagerText->SetValue(this->sceneSettings.GetSceneManager());

    //Flags
    this->addFlagButton->Enable(FinjinGlobalSettings::GetInstance().HasFlags());

    wxString flagText;
    int flagCount = this->sceneSettings.GetFlagCount();
    wxString flagName;
    for (int i = 0; i < flagCount; i++)
    {
        this->sceneSettings.GetFlag(i, flagName);
        if (!flagName.empty())
        {
            flagText += flagName;
            flagText += wxT("\n");
        }        
    }
    this->flagAliasesText->SetValue(flagText);    

    return true;
}

void SceneSettingsDialog_GeneralPage::OnAddFlagButton(wxCommandEvent& event)
{
    auto& globalSettings = FinjinGlobalSettings::GetInstance();

    SUSPEND_CUSTOM_CONTROLS;
    std::vector<wxString> flagNames;
    std::vector<int> flagBits;
    globalSettings.GetFlags(flagNames, flagBits);
    FlagDialog dialog(this, Strings::ADD_FLAG, flagNames, flagBits);
    ModalDialogFix modalDialogFix(&dialog);
    if (dialog.ShowModal() == wxID_OK)
    {
        //Get query flags
        wxString text = this->flagAliasesText->GetValue();

        //Append alias or bit
        wxString flagText;
        if (!dialog.name.empty())
            flagText += dialog.name;                                    
        else if (dialog.bit != -1)
            flagText += StringUtilities::ToString(dialog.bit);                                
        
        if (!flagText.empty())
        {
            text += flagText;
            text += wxT("\n");

            //Set query flags                                    
            this->flagAliasesText->SetValue(text);
        }
    }
}
