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


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** 
     * Prompts the user for a file or directory location, which is then remapped to be relative to another location.
     */
    class RelativeLocationPrompt
    {
    public:
        /** 
         * Initializes the prompt to be a file dialog.
         * @param parent [in] - The parent window.
         * @param baseDirectory [in] - The directory that is considered the base for whatever file is selected.
         * @param promptTitle [in] - The text placed into the title bar of the file dialog.
         * @param filter [in] - The file filter used by the file dialog.
         * @param style [in] - The file dialog flags.
         */
        RelativeLocationPrompt
            (
            wxWindow* parent,
            const wxString baseDirectory,
            const wxString& promptTitle,
            const wxString& filter,
            long style = wxFD_DEFAULT_STYLE
            );

        /** 
         * Initializes the prompt to be a directory dialog.
         * @param parent [in] - The parent window.
         * @param baseDirectory [in] - The directory that is considered the base for whatever dialog is selected.
         * @param promptTitle [in] - The text placed into the title bar of the file dialog.
         * @param style [in] - The directory dialog flags.
         */
        RelativeLocationPrompt
            (
            wxWindow* parent,
            const wxString baseDirectory,
            const wxString& promptTitle,
            long style = wxDD_DEFAULT_STYLE
            );

        /** 
         * Shows the dialog and returns the relative location.
         * @return If the user cancels the dialog, an empty string is returned.
         */
        wxString ShowPrompt();

    private:
        wxWindow* parent;

        wxString baseDirectory;
        
        wxString promptTitle;
        
        enum LocationType
        {
            FILE_LOCATION,
            DIRECTORY_LOCATION
        };
        LocationType locationType;
        
        wxString filter;
        
        long style;
    };

} }
