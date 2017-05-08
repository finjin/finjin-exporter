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
#include "FinjinSceneSettingsAccessor.hpp"
#include "TextureCoordinateSetMappings.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Mesh page for the object settings dialog. */
    class ObjectSettingsDialog_MeshPage : public SettingsPage
    {
    private:
        DECLARE_EVENT_TABLE()

    protected:
        wxStaticText* meshNameLabel;
        ApplicationTextCtrl* meshNameText;
        wxStaticText* primitiveTypeLabel;
        ApplicationChoiceCtrl* primitiveTypeChoice;
        wxStaticText* pointSizeLabel;
        SpinnerControl* pointSizeSpinner;
        wxCheckBox* exportVertexColorsCheckbox;
        wxCheckBox* exportTangentsCheckbox;
        wxCheckBox* exportBinormalsCheckbox;
        wxCheckBox* checkMeshInstanceCheckbox;
        wxCheckBox* animateBoundingVolumesCheckbox;

        ObjectAccessor object;
        FinjinObjectSettingsAccessor objectSettings;

    public:
        ObjectSettingsDialog_MeshPage(wxWindow* parent, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 494,573 ), long style = wxTAB_TRAVERSAL );
        ~ObjectSettingsDialog_MeshPage();

        bool GetGUIData() override;
        bool SetGUIData() override;

        static const wxString TITLE;
    };

} }
