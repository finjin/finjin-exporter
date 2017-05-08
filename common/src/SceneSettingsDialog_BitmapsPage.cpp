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
#include "SceneSettingsDialog_BitmapsPage.hpp"
#include "StringSplitter.hpp"
#include "Strings.hpp"
#include "StringUtilities.hpp"
#include "ApplicationControls.hpp"
#include "ApplicationAccessor.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "ModalDialogFix.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
const wxString SceneSettingsDialog_BitmapsPage::TITLE(wxT("Bitmaps"));


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(SceneSettingsDialog_BitmapsPage, SettingsPage)
END_EVENT_TABLE()

SceneSettingsDialog_BitmapsPage::SceneSettingsDialog_BitmapsPage( wxWindow* parent, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    this->sceneSettings = sceneSettings;

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* bitmapExportControlSizer;
    bitmapExportControlSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Bitmap Export Control") ), wxVERTICAL );

    copyBitmapsToExportDirectoryCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Copy Bitmaps to Export Directory"), wxDefaultPosition, wxDefaultSize, 0 );
    bitmapExportControlSizer->Add( copyBitmapsToExportDirectoryCheckbox, 0, wxALL, 5 );

    convertBitmapsToTexturesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Convert Bitmaps to Textures (.texture)"), wxDefaultPosition, wxDefaultSize, 0 );
    bitmapExportControlSizer->Add( convertBitmapsToTexturesCheckbox, 0, wxALL, 5 );

    topSizer->Add( bitmapExportControlSizer, 0, wxEXPAND|wxTOP, 5 );

    SetSizer(topSizer);
    Layout();
}

SceneSettingsDialog_BitmapsPage::~SceneSettingsDialog_BitmapsPage()
{
}

bool SceneSettingsDialog_BitmapsPage::GetGUIData()
{
    //Copy/convert bitmaps
    //this->sceneSettings.SetCopyBitmaps(this->copyBitmapsToExportDirectoryCheckbox->GetValue());
    //this->sceneSettings.SetConvertBitmapsToTextures(this->convertBitmapsToTexturesCheckbox->GetValue());

    return true;
}

bool SceneSettingsDialog_BitmapsPage::SetGUIData()
{
    //Copy/convert bitmaps
    //this->copyBitmapsToExportDirectoryCheckbox->SetValue(this->sceneSettings.GetCopyBitmaps());
    //this->convertBitmapsToTexturesCheckbox->SetValue(this->sceneSettings.GetConvertBitmapsToTextures());

    return true;
}
