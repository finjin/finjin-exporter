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


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * Base class for all Finjin wxFrame-based windows.
     */
    class FinjinFrame : public wxFrame
    {
    public:
        /** Default constructor. */
        FinjinFrame();

        /**
         * Constructor.
         * @param parent [in] - The parent window.
         * @param id [in] - The dialog identifier.
         * @param title [in] - The dialog title.
         * @param pos [in] - Initial position of the dialog.
         * @param size [in] - Initial size of the dialog.
         * @param style [in] - Dialog style flags.
         * @param name [in] - The name of the dialog.
         */
        FinjinFrame
            (
            wxWindow* parent,
            wxWindowID id = wxID_ANY,
            const wxString& title = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_DIALOG_STYLE,
            const wxString& name = wxFrameNameStr
            );

        ~FinjinFrame();

        bool Create
            (
            wxWindow* parent,
            wxWindowID id = wxID_ANY,
            const wxString& title = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_DIALOG_STYLE,
            const wxString& name = wxFrameNameStr
            );

        bool Destroy();

    private:
        /**
         * Performs final initialization on the dialog.
         * @param style [in] - Dialog style flags passed to the constructor or Create().
         */
        void FinishCreation(long style);
    };

} }
