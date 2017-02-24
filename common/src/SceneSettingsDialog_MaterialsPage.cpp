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
#include "SceneSettingsDialog_MaterialsPage.hpp"
#include "ChooseMaterialsPrompt.hpp"
#include "StringSplitter.hpp"
#include "Strings.hpp"
#include "StringUtilities.hpp"
#include "ApplicationControls.hpp"
#include "ApplicationAccessor.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "ModalDialogFix.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
const wxString SceneSettingsDialog_MaterialsPage::TITLE(wxT("Materials"));


//Implementation---------------------------------------------------------------
BEGIN_EVENT_TABLE(SceneSettingsDialog_MaterialsPage, SettingsPage)
    EVT_BUTTON(SceneSettingsDialog_MaterialsPage::ADDITIONAL_MATERIAL_BUTTON, SceneSettingsDialog_MaterialsPage::OnAdditionalMaterialButton)
    EVT_BUTTON(SceneSettingsDialog_MaterialsPage::IGNORE_MATERIAL_BUTTON, SceneSettingsDialog_MaterialsPage::OnIgnoreMaterialButton)    
END_EVENT_TABLE()

SceneSettingsDialog_MaterialsPage::SceneSettingsDialog_MaterialsPage( wxWindow* parent, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    this->sceneSettings = sceneSettings;

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* additionalAndIgnoreSizer;
    additionalAndIgnoreSizer = new wxBoxSizer( wxHORIZONTAL );
    
    wxStaticBoxSizer* additionalMaterialsSizer;
    additionalMaterialsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Additional Materials") ), wxVERTICAL );
    
    addAdditionalMaterialButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxDefaultSize, 0 );
    additionalMaterialsSizer->Add( addAdditionalMaterialButton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5 );
    
    addAdditionalMaterialsText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
    additionalMaterialsSizer->Add( addAdditionalMaterialsText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    additionalAndIgnoreSizer->Add( additionalMaterialsSizer, 1, wxEXPAND|wxRIGHT|wxTOP, 5 );
    
    wxStaticBoxSizer* ignoreMaterialsSizer;
    ignoreMaterialsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Ignore Materials") ), wxVERTICAL );
    
    addIgnoreMaterialButton = new wxButton( this, wxID_ANY, wxT("Add..."), wxDefaultPosition, wxDefaultSize, 0 );
    ignoreMaterialsSizer->Add( addIgnoreMaterialButton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5 );
    
    ignoreMaterialsText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
    ignoreMaterialsText->SetMaxLength( 0 ); 
    ignoreMaterialsSizer->Add( ignoreMaterialsText, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
    
    
    additionalAndIgnoreSizer->Add( ignoreMaterialsSizer, 1, wxEXPAND|wxTOP, 5 );
    
    
    topSizer->Add( additionalAndIgnoreSizer, 1, wxEXPAND, 5 );

    this->addAdditionalMaterialButton->SetId(ADDITIONAL_MATERIAL_BUTTON);
    this->addIgnoreMaterialButton->SetId(IGNORE_MATERIAL_BUTTON);
    
    SetSizer(topSizer);
    Layout();
}

SceneSettingsDialog_MaterialsPage::~SceneSettingsDialog_MaterialsPage()
{
}

bool SceneSettingsDialog_MaterialsPage::GetGUIData() 
{
    //Additional materials
    {
        this->sceneSettings.ClearAdditionalMaterials();

        auto materialsText = this->addAdditionalMaterialsText->GetValue();
        StringSplitter materialNames(materialsText, wxT("\n"), StringSplitterFlags::TRIM | StringSplitterFlags::IGNORE_EMPTY);
                
        MaterialAccessorSet addedAdditionalMaterials;
        for (size_t i = 0; i < materialNames.size(); i++)
        {
            //Make sure the material name has an actual material
            auto additionalMaterialIterator = this->additionalMaterials.find(materialNames[i]);
            if (additionalMaterialIterator != this->additionalMaterials.end())
            {
                //Make sure the material hasn't been added already
                MaterialAccessor material = additionalMaterialIterator->second;
                if (addedAdditionalMaterials.find(material) == addedAdditionalMaterials.end())
                {
                    //Add it
                    this->sceneSettings.AddAdditionalMaterial(material);
                    addedAdditionalMaterials.insert(material);
                }
            }
        }
    }

    //Ignore materials
    {
        this->sceneSettings.ClearDisallowedMaterials();

        auto materialsText = this->ignoreMaterialsText->GetValue();
        StringSplitter materialNames(materialsText, wxT("\n"), StringSplitterFlags::TRIM | StringSplitterFlags::IGNORE_EMPTY);
                    
        MaterialAccessorSet addedDisallowedMaterials;
        for (size_t i = 0; i < materialNames.size(); i++)
        {
            //Make sure the material name has an actual material
            auto disallowedMaterialIterator = this->disallowedMaterials.find(materialNames[i]);
            if (disallowedMaterialIterator != this->disallowedMaterials.end())
            {
                //Make sure the material hasn't been added already
                MaterialAccessor material = disallowedMaterialIterator->second;
                if (addedDisallowedMaterials.find(material) == addedDisallowedMaterials.end())
                {
                    //Add it
                    this->sceneSettings.AddDisallowedMaterial(material);
                    addedDisallowedMaterials.insert(material);
                }
            }
        }
    }

    return true;
}

bool SceneSettingsDialog_MaterialsPage::SetGUIData() 
{
    wxString materialsText;

    //Additional materials
    materialsText.clear();
    int additionalMaterialCount = this->sceneSettings.GetAdditionalMaterialCount();
    for (int i = 0; i < additionalMaterialCount; i++)
    {
        auto material = this->sceneSettings.GetAdditionalMaterial(i);
        auto materialName = material.GetFullName();

        this->additionalMaterials[materialName] = material;
        materialsText += materialName;
        materialsText += wxT("\n");
    }
    this->addAdditionalMaterialsText->SetValue(materialsText);

    //Ignore materials
    materialsText.clear();
    int disallowedMaterialCount = this->sceneSettings.GetDisallowedMaterialCount();
    for (int i = 0; i < disallowedMaterialCount; i++)
    {
        auto material = this->sceneSettings.GetDisallowedMaterial(i);
        auto materialName = material.GetFullName();

        this->disallowedMaterials[materialName] = material;
        materialsText += materialName;
        materialsText += wxT("\n");
    }
    this->ignoreMaterialsText->SetValue(materialsText);
    
    return true;
}

void SceneSettingsDialog_MaterialsPage::OnAdditionalMaterialButton(wxCommandEvent& event)
{
    ChooseMaterialsPrompt choose(this, HowMany::CHOOSE_MANY);
    if (choose.ShowPrompt())
    {
        for (size_t i = 0; i < choose.GetMaterials().size(); i++)
        {
            auto materialName = choose.GetMaterials()[i].GetFullName();

            this->addAdditionalMaterialsText->AppendText(materialName);
            this->addAdditionalMaterialsText->AppendText(wxT("\n"));

            //Hold onto material
            if (this->additionalMaterials.find(materialName) == this->additionalMaterials.end())
                this->additionalMaterials[materialName] = choose.GetMaterials()[i];
        }
    }
}

void SceneSettingsDialog_MaterialsPage::OnIgnoreMaterialButton(wxCommandEvent& event)
{
    ChooseMaterialsPrompt choose(this, HowMany::CHOOSE_MANY);
    if (choose.ShowPrompt())
    {
        for (size_t i = 0; i < choose.GetMaterials().size(); i++)
        {
            auto materialName = choose.GetMaterials()[i].GetFullName();

            this->ignoreMaterialsText->AppendText(materialName);
            this->ignoreMaterialsText->AppendText(wxT("\n"));

            //Hold onto material
            if (this->disallowedMaterials.find(materialName) == this->disallowedMaterials.end())
                this->disallowedMaterials[materialName] = choose.GetMaterials()[i];
        }
    }    
}
