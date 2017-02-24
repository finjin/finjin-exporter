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
#include "MacroExpander.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
MacroExpander::MacroExpander()
{
    this->useEnvironment = true;
}

void MacroExpander::AddVariable(const wxString& key, const wxString& value)
{
    this->keyToValue[key] = value;
}

void MacroExpander::SetMatchErrorFormatString(const wxString& format)
{
    this->errorFormatString = format;
}

bool MacroExpander::Expand(wxString& text)
{
    auto result = true;

    wxString match, key, envValue;
    wxRegEx regex(wxT("\\$\\(([0-9A-Za-z_]+)\\)")); //Matches something like: $(VAR)
    wxString textBeforeReplacement; //Used to determine if the text has changed after an iteration
    while (regex.Matches(text, wxRE_NOTEOL) && textBeforeReplacement != text)
    {
        textBeforeReplacement = text;

        match = regex.GetMatch(text); //The entire $(VAR)
        key = regex.GetMatch(text, 1); //Just VAR
        if (this->useEnvironment && wxGetEnv(key, &envValue))
        {
            //An environment value was found
            text.Replace(match, envValue);
        }
        else
        {
            auto keyAndValue = keyToValue.find(key);
            if (keyAndValue != keyToValue.end())
            {
                //A "standard" value was found
                text.Replace(match, keyAndValue->second);
            }
            else 
            {
                if (!this->errorFormatString.empty())
                {
                    //No value was found for the pattern
                    auto message = wxString::Format(this->errorFormatString, match.wx_str());
                    FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, message);
                }

                result = false;
            }
        }
    }

    return result;
}
