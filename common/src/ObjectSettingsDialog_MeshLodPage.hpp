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
#include "SettingsPage.hpp"
#include "SpinnerControl.hpp"
#include "FinjinObjectSettingsAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Mesh level-of-detail page for the object settings dialog. */
    class ObjectSettingsDialog_MeshLodPage : public SettingsPage
    {
    private:
        DECLARE_EVENT_TABLE()

    protected:
        ApplicationListCtrl* manualLodList;
        wxButton* addManualLodButton;
        wxButton* removeManualLodButton;
        wxButton* editManualLodButton;
        wxCheckBox* calculateManualDistancesCheckbox;

        ObjectAccessor object;
        FinjinObjectSettingsAccessor objectSettings;

        ManualLods manualLods;

    public:
        ObjectSettingsDialog_MeshLodPage(wxWindow* parent, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 494,573 ), long style = wxTAB_TRAVERSAL );
        ~ObjectSettingsDialog_MeshLodPage();

        enum
        {
            MANUAL_LOD_LIST,
            ADD_MANUAL_LOD_BUTTON,
            EDIT_MANUAL_LOD_BUTTON,
            REMOVE_MANUAL_LOD_BUTTON,
            CALCULATE_MANUAL_DISTANCES_CHECKBOX
        };
        
        void GetExcludedLodObjects(ObjectAccessorSet& excludedLodObjects);

        bool GetGUIData() override;
        bool SetGUIData() override;    

        void UpdateManualLodGUIData();
        void UpdateManualLodButtons();
        void OnEditManualLod();

        void OnManualLodListActivate(wxListEvent& event);
        void OnManualLodListActivateSelectionChange(wxListEvent& event);
        void OnAddManualLodButton(wxCommandEvent& event);
        void OnEditManualLodButton(wxCommandEvent& event);
        void OnRemoveManualLodButton(wxCommandEvent& event);
        void OnCalculateDistancesCheckbox(wxCommandEvent& event);

        static const wxString TITLE;
    };

} }
