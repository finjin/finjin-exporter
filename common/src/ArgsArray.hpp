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
     * Turns an array of command line arguments into something suitable for passing to wxCommand.
     * Based on code from wxWidgets
     */
    class ArgsArray
    {
    #if wxMAJOR_VERSION >= 3
        typedef char ARG_CHAR;
    #else
        typedef wxChar ARG_CHAR;
    #endif
        
    public:
        /**
          * @param args [in] - The command line arguments
          */
        ArgsArray(const wxArrayString& args)
        {
            this->commands = args;
            
            auto argc = this->commands.size();
            this->argv.resize(argc + 1);
            this->argv[argc] = nullptr; //Terminator
            
        #if defined(__WXMSW__)
            for (int i = 0; i < this->commands.size(); i++)
                this->commands[i] = wxT("\"") + this->commands[i] + wxT("\"");
        #endif
            
            for (int i = 0; i < this->commands.size(); i++)
                this->argv[i] = wxStrdup(this->commands[i]);
        }
        
        ~ArgsArray()
        {
            for (auto item : this->argv)
                free(item);
        }
        
        /** Returns an array of args suitable for passing to wxCommand(). */
        operator ARG_CHAR**() const { return (ARG_CHAR**)this->argv.data(); }
        
        void ToCommandString(wxString& commandString)
        {
            commandString.Clear();
            
            for (int i = 0; i < this->commands.GetCount(); i++)
            {
                if (i > 0)
                    commandString += wxT(" ");
                
                commandString += this->commands[i];
            }
        }
    private:
        wxArrayString commands;
        
        std::vector<ARG_CHAR*> argv;
    };

} }
