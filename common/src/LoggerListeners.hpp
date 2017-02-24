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

    /** A Logger listener that logs messages to the application console. */
    class ApplicationConsoleLogListener : public LogListener
    {
    public:
        ApplicationConsoleLogListener(LogMessageType logLevel);

        void LogMessage(LogMessageType type, const wxString& message) override;
    };

    /** A Logger listener that logs messages to a file. */
    class WxFileLogListener : public LogListener
    {
    public:
        WxFileLogListener(LogMessageType logLevel, const wxString& fileName);
        ~WxFileLogListener();

        bool IsInitialized() const;
        void Initialize();
        void Uninitialize();

        void LogMessage(LogMessageType type, const wxString& message) override;

    private:
        wxString fileName;
        wxFile file;
    };

} }
