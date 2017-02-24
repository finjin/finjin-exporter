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
#include "GlobalSettingsDialog_FlagsPage.hpp"
#include "FinjinGlobalSettings.hpp"
#include "Strings.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "ApplicationAccessor.hpp"
#include "RelativeLocationPrompt.hpp"
#include "Dialogs.hpp"
#include "FlagAliasDialog.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "StringSplitter.hpp"
#include "ApplicationControls.hpp"
#include "ModalDialogFix.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
const wxString GlobalSettingsDialog_FlagsPage::TITLE(wxT("Flags"));


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(GlobalSettingsDialog_FlagsPage, SettingsPage)    
    EVT_BUTTON(GlobalSettingsDialog_FlagsPage::ADD_BUTTON, GlobalSettingsDialog_FlagsPage::OnAddButton)
END_EVENT_TABLE()

GlobalSettingsDialog_FlagsPage::GlobalSettingsDialog_FlagsPage(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* aliasesSizer;
    aliasesSizer = new wxBoxSizer( wxHORIZONTAL );
    
    wxStaticBoxSizer* innerAliasesSizer;
    innerAliasesSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Flag Names") ), wxVERTICAL );
    
    wxGridSizer* queryFlagAliasesControlsSizer;
    queryFlagAliasesControlsSizer = new wxGridSizer( 1, 2, 0, 0 );
    
    addButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxDefaultSize, 0 );
    queryFlagAliasesControlsSizer->Add( addButton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5 );
    
    innerAliasesSizer->Add( queryFlagAliasesControlsSizer, 0, wxEXPAND, 5 );
    
    flagsText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
    innerAliasesSizer->Add( flagsText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    aliasesSizer->Add( innerAliasesSizer, 1, wxEXPAND|wxRIGHT, 5 );
    
    topSizer->Add( aliasesSizer, 1, wxEXPAND|wxTOP, 5 );
    
    this->addButton->SetId(ADD_BUTTON);
    
    SetSizer(topSizer);
    Layout();
}

GlobalSettingsDialog_FlagsPage::~GlobalSettingsDialog_FlagsPage()
{
}

bool GlobalSettingsDialog_FlagsPage::GetGUIData()
{
    auto& settings = FinjinGlobalSettings::GetInstance();
    
    //Flags
    settings.flags.clear();
    wxString flagsTextString = this->flagsText->GetValue();
    if (!flagsTextString.empty())
    {
        std::set<wxString> flagNames;
        std::set<int> flagBits;
        wxString flagName, flagBitText;
        
        StringSplitter flags(flagsTextString, wxT("\n"), StringSplitterFlags::TRIM | StringSplitterFlags::IGNORE_EMPTY);
        for (size_t i = 0; i < flags.size(); i++)
        {
            if (StringUtilities::ParseKeyValueLine(flags[i], flagName, flagBitText))
            {
                //Make sure name is unique
                if (flagNames.find(flagName) == flagNames.end() &&
                    StringUtilities::IsInteger(flagBitText))
                {
                    //Make sure bit is unique
                    int flagBit = StringUtilities::ToInt(flagBitText);
                    if (flagBits.find(flagBit) == flagBits.end())
                    {
                        flagNames.insert(flagName);
                        flagBits.insert(flagBit);
                        settings.flags.push_back(NamedFlag(flagName, flagBit));
                    }
                }
            }
        }            
    }

    return true;
}

bool GlobalSettingsDialog_FlagsPage::SetGUIData()
{
    auto& settings = FinjinGlobalSettings::GetInstance();

    //Flags
    wxString flagText;
    for (auto& f : settings.flags)
    {
        StringUtilities::AppendKeyAndValue
            (
            flagText, 
            f.name, 
            StringUtilities::ToString(f.bit), 
            wxT("\n")
            );
    }
    this->flagsText->SetValue(flagText);

    return true;
}

void GlobalSettingsDialog_FlagsPage::OnAddButton(wxCommandEvent& event)
{
    std::set<wxString> flagNames;
    std::set<int> flagBits;

    wxString flagsTextString = this->flagsText->GetValue();
    if (!flagsTextString.empty())
    {
        wxString flagName, flagBit;
        
        StringSplitter flags(flagsTextString, wxT("\n"), StringSplitterFlags::TRIM | StringSplitterFlags::IGNORE_EMPTY);
        for (size_t i = 0; i < flags.size(); i++)
        {
            if (StringUtilities::ParseKeyValueLine(flags[i], flagName, flagBit))
            {
                //Make sure name is unique
                if (flagNames.find(flagName) == flagNames.end() &&
                    StringUtilities::IsInteger(flagBit))
                {
                    flagNames.insert(flagName);
                    flagBits.insert(StringUtilities::ToInt(flagBit));                    
                }
            }
        }            
    }

    SUSPEND_CUSTOM_CONTROLS;
    FlagAliasDialog dialog(this, Strings::ADD_FLAG_ALIAS, flagNames, flagBits);
    ModalDialogFix modalDialogFix(&dialog);
    if (dialog.ShowModal() == wxID_OK)
    {
        //Append new alias
        StringUtilities::AppendKeyAndValue(flagsTextString, dialog.name, StringUtilities::ToString(dialog.bit), wxT("\n"));

        //Set query flag aliases
        this->flagsText->SetValue(flagsTextString);
    }
}
