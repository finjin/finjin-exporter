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
     * Expands macros in a file path, using environment variables if available.
     * Macros are of the form $(VAR)
     */
    class MacroExpander
    {
    public:
        MacroExpander();

        /** 
         * Adds a custom macro variable. 
         * @param key [in] - The key. The key is of the form VAR. In other words, it does not contain the $() macro characters.
         * @param value [in] - The variable.
         */
        void AddVariable(const wxString& key, const wxString& value);

        /** Sets an error formatting string that is used to log the result of a macro that could not be expanded. */
        void SetMatchErrorFormatString(const wxString& format);

        /** 
         * Expands all of the macros in the specified text.
         * @text [in/out] - The text to expand.
         * @return If there were no errors, true is returned. Otherwise, false is returned.
         */
        bool Expand(wxString& text);

    private:
        bool useEnvironment;

        typedef std::unordered_map<wxString, wxString> KeyToValueLookup;
        KeyToValueLookup keyToValue;

        wxString errorFormatString;
    };

} }
