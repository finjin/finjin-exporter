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
     * Expands the macros in the 'before export' and 'after export' area of the
     * events page in the Scene Settings dialog. This expansion is done during export.
     */
    class ExportCommandMacros
    {
    public:
        ExportCommandMacros();

        /** Finalizes the initialization of the macros. */
        void Finalize();
        
        /** 
         * Expands the macros specified in the text. 
         * @return If all of the patterns of the form $(PATTERN) are replaced, true is returned. Otherwise, false is returned.
         */
        bool Expand(wxString& text);

    public:
        wxString applicationDirectory;
        wxString exportDirectory;
        wxString exportFilePath;
        wxString exportFileName;
    };

} }
