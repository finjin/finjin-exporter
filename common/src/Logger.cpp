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
#include "Logger.hpp"

using namespace Finjin::Exporter;


//Local functions---------------------------------------------------------------
static wxString GetIndentString(int indent, const wxString& oneIndent = wxT("     "))
{
    wxString result;
    for (int i = 0; i < indent; i++)
        result += oneIndent;
    return result;
}


//Static initialization---------------------------------------------------------
Logger::Listeners Logger::listeners;
LogMessageType LogListener::DEFAULT_LOG_LEVEL = INFO_LOG_MESSAGE;


//Implementation----------------------------------------------------------------

//LogListener
LogListener::LogListener(LogMessageType logLevel)
{
    this->logLevel = logLevel;
    this->depth = 0;
    this->indent = 0;
}

LogMessageType LogListener::GetLogLevel() const
{
    return this->logLevel;
}

void LogListener::SetLogLevel(LogMessageType logLevel)
{
    this->logLevel = logLevel;
}

bool LogListener::CanHandle(LogMessageType type) const
{
    return type <= this->logLevel;
}

void LogListener::LogMessage(LogMessageType type, const wxChar* format, ...)
{
    wxString message;
    FINJIN_EXPORTER_FORMAT_STRING_VA(message, format);
    wxString indentedMessage = GetIndentString(this->depth) + message;
    LogMessage(type, indentedMessage);
}

void LogListener::StartMessage(LogMessageType type, const wxString& heading)
{
    this->depth++;
}

void LogListener::ContinueMessage(LogMessageType type, const wxString& message)
{
    LogMessage(type, message);
}

void LogListener::EndMessage()
{
    this->depth--;
}

//Logger
void Logger::LogMessage(LogMessageType type, const wxChar* format, ...)
{
    if (CanHandle(type))
    {
        wxString message;
        FINJIN_EXPORTER_FORMAT_STRING_VA(message, format);
        LogMessage(type, message);
    }
}

void Logger::LogMessage(LogMessageType type, const wxString& message)
{
    for (auto listener : listeners)
    {
        if (listener->CanHandle(type))
        {
            auto indentedMessage = GetIndentString(listener->indent) + message;
            listener->LogMessage(type, indentedMessage);
        }
    }
}

void Logger::LogResults(LogMessageType type, const wxArrayString& results, const wxString& heading)
{
    if (!results.empty())
    {
        for (auto listener : listeners)
        {
            if (listener->CanHandle(type))
            {
                auto indentedHeading = GetIndentString(listener->indent) + heading;

                listener->StartMessage(type, indentedHeading);
                for (size_t i = 0; i < results.size(); i++)
                    listener->ContinueMessage(type, indentedHeading);
                listener->EndMessage();
            }
        }
    }
}

void Logger::SetLogLevel(LogMessageType type)
{
    LogListener::DEFAULT_LOG_LEVEL = type;

    for (auto listener : listeners)
        listener->SetLogLevel(type);
}

void Logger::Indent()
{
    for (auto listener : listeners)
        listener->indent++;
}

void Logger::Unindent()
{
    for (auto listener : listeners)
    {
        if (listener->indent > 0)
            listener->indent--;
    }

}

void Logger::Indent(LogMessageType type)
{
    for (auto listener : listeners)
    {
        if (listener->CanHandle(type))
            listener->indent++;
    }
}

void Logger::Unindent(LogMessageType type)
{
    for (auto listener : listeners)
    {
        if (listener->CanHandle(type) && listener->indent > 0)
            listener->indent--;
    }

}

bool Logger::CanHandle(LogMessageType type)
{
    for (auto listener : listeners)
    {
        if (listener->CanHandle(type))
            return true;
    }

    return false;
}

void Logger::AddListener(LogListener* listener)
{
    if (listener != nullptr &&
        std::find(listeners.begin(), listeners.end(), listener) == listeners.end())
    {
        listeners.push_back(listener);
    }
}

void Logger::RemoveListener(LogListener* listener)
{
    listeners.remove(listener);
}

void Logger::ClearListeners()
{
    listeners.clear();
}
