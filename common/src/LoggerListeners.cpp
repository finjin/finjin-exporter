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
#include "LoggerListeners.hpp"
#include "ApplicationAccessor.hpp"
#include "FileUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------

//ApplicationConsoleLogListener
ApplicationConsoleLogListener::ApplicationConsoleLogListener(LogMessageType logLevel) : LogListener(logLevel)
{
}

void ApplicationConsoleLogListener::LogMessage(LogMessageType type, const wxString& message)
{
    if (CanHandle(type))
        ApplicationAccessor::LogMessage(message);
}

//WxFileLogListener
WxFileLogListener::WxFileLogListener(LogMessageType logLevel, const wxString& _fileName) : LogListener(logLevel), fileName(_fileName)
{
}

WxFileLogListener::~WxFileLogListener()
{
    this->file.Close();
}

bool WxFileLogListener::IsInitialized() const
{
    return this->file.IsOpened();
}

void WxFileLogListener::Initialize()
{
    if (!IsInitialized())
    {
        auto fullFilePath = FileUtilities::JoinPath(ApplicationAccessor::GetFinjinHomeDirectory(true), this->fileName);
        this->file.Open(fullFilePath, wxFile::write);
    }
}

void WxFileLogListener::Uninitialize()
{
    this->file.Close();
}

void WxFileLogListener::LogMessage(LogMessageType type, const wxString& message)
{
    if (CanHandle(type))
    {
        Initialize();

        auto s = message.ToUTF8();
        this->file.Write(s.data(), strlen(s.data()));
        this->file.Write("\r\n", strlen("\r\n"));
        this->file.Flush();
    }
}
