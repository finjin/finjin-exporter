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
#include "FinjinDialog.hpp"
#include "TabControl.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * A modal dialog that describes the Finjin plugin.
     * This dialog can be seen within the application by selecting 'About...' in the
     * Finjin menu.
     */
    class AboutDialog : public FinjinDialog
    {
    public:
        AboutDialog();
        AboutDialog(wxWindow* parent);

        bool Create(wxWindow* parent);
        void OnCloseWindow(wxCloseEvent& event);
        void OnNotebookPageChanged(wxNotebookEvent& event);

    protected:
        void CreateControls();

    private:
        DECLARE_EVENT_TABLE()

        TabControl* tabControl;
        TabControlPages pages;
    };

} }
