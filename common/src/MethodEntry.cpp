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
#include "MethodEntry.hpp"
#include "Strings.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
MethodEntry::MethodEntry(const wxChar* format, ...)
{
    if (Logger::CanHandle(DEBUG_LOG_MESSAGE))
    {
        this->message.clear();
        FINJIN_EXPORTER_FORMAT_STRING_VA(this->message, format);
        if (!this->message.empty())
        {
            wxString logMessage = wxString::Format(wxT("%s %s"), Strings::ENTERING.wx_str(), this->message.wx_str());
            FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, logMessage);
            
            Logger::Indent(DEBUG_LOG_MESSAGE);            
        }
    }    
}

MethodEntry::~MethodEntry()
{
    if (!this->message.empty())
    {
        Logger::Unindent(DEBUG_LOG_MESSAGE);
        
        wxString logMessage = wxString::Format(wxT("%s %s"), Strings::LEAVING.wx_str(), this->message.wx_str());
        FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, logMessage);
    }
}
