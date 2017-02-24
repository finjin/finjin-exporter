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
     * Logs a debug message upon creation and destruction.
     * This is used to log the entry and exit of functions.
     */
    class MethodEntry
    {
    public:
        /** 
         * Constructor that logs a message.
         * @param format [in] - The message format string. This should resemble the signature of the function
         * from which it is called.
         */
        MethodEntry(const wxChar* format, ...);

        /** Destructor. */
        ~MethodEntry();

    private:
        /** The formatted message. */
        wxString message;
    };

} }

//#define FINJIN_EXPORTER_LOG_METHOD_ENTRY(message) 
//#define FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(format, ...) 

#define FINJIN_EXPORTER_LOG_METHOD_ENTRY(message) MethodEntry methodEntry(message);
#define FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(format, ...) MethodEntry methodEntry(wxString::Format(format, __VA_ARGS__));


//#define DEV_METHOD_ENTRY(message)
//#define DEV_METHOD_ENTRY_FORMAT(format, ...)

#define DEV_METHOD_ENTRY(message) MethodEntry methodEntry(message);
#define DEV_METHOD_ENTRY_FORMAT(format, ...) MethodEntry methodEntry(wxString::Format(format, __VA_ARGS__));
