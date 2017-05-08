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


//Macros------------------------------------------------------------------------
typedef wxNotebook TabControl; //wxNotebook manages the tabs and the pages
#define TAB_CONTROL_SIZER_FLAGS 0
#define DEFAULT_TAB_CONTROL_SIZE wxDefaultSize


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class TabControlPages
    {
    public:
        TabControlPages(TabControl* control = nullptr, wxSizer* pageSizer = nullptr)
        {
            this->control = control;
        }

        TabControl* GetTabControl()
        {
            return this->control;
        }

        void SetTabControl(TabControl* control, wxSizer*)
        {
            this->control = control;
            this->control->DeleteAllPages();
        }

        wxWindow* GetTabParentWindow()
        {
            return this->control;
        }

        size_t GetPageCount() const
        {
            return this->control->GetPageCount();
        }

        wxString GetPageText(size_t index)
        {
            return this->control->GetPageText(index);
        }

        wxWindow* GetPage(size_t index)
        {
            return this->control->GetPage(index);
        }

        void AddPage(wxWindow* page, const wxString& tabText)
        {
            this->control->AddPage(page, tabText);
        }

        void DeletePage(int pageIndex)
        {
            this->control->DeletePage(pageIndex);
        }

        void DeleteAllPages()
        {
            this->control->DeleteAllPages();
        }

        void ChangeSelection(int pageIndex)
        {
            this->control->ChangeSelection(pageIndex);
        }

        int GetSelection() const
        {
            return this->control->GetSelection();
        }

        bool CanHidePages() const
        {
            return false;
        }

        void OnPageChanged()
        {
        }

    private:
        TabControl* control;
    };

} }
