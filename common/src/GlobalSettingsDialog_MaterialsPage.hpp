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


//Includes----------------------------------------------------------------------
#include "SettingsPage.hpp"
#include "SpinnerControl.hpp"
#include "FinjinObjectSettingsAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class GlobalSettingsDialog_MaterialsPage : public SettingsPage
    {
    private:
        DECLARE_EVENT_TABLE()

    protected:
        wxStaticText* noMaterialNameLabel;
        ApplicationTextCtrl* noMaterialNameText;
        wxStaticText* noMaterialNameDescriptionLabel;
        wxCheckBox* includeParentMaterialInNameCheckbox;
        wxCheckBox* createExtraMaterialsCheckbox;
        wxCheckBox* copyBitmapsToExportDirectoryCheckbox;
        wxCheckBox* convertBitmapsToTexturesCheckbox;

    public:
        GlobalSettingsDialog_MaterialsPage(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
        ~GlobalSettingsDialog_MaterialsPage();

        bool GetGUIData() override;
        bool SetGUIData() override;

        static const wxString TITLE;

    private:
    };

} }
