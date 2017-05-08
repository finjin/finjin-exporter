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


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * Log message types/levels.
     * Each successive message type corresponds to more detailed logging.
     */
    enum LogMessageType
    {
        ERROR_LOG_MESSAGE,
        WARNING_LOG_MESSAGE,
        INFO_LOG_MESSAGE,
        DEBUG_LOG_MESSAGE
    };

    /** A listeners for Logger log messages. */
    class LogListener
    {
        friend class Logger;

    public:
        static LogMessageType DEFAULT_LOG_LEVEL;

        /** Constructs the listener with the default log level. */
        LogListener(LogMessageType logLevel = DEFAULT_LOG_LEVEL);

        virtual ~LogListener() {}

        /** Gets the log level for the listener. */
        LogMessageType GetLogLevel() const;

        /**
         * Sets the log level for the listener.
         * A derived class can choose to ignore this call.
         */
        virtual void SetLogLevel(LogMessageType logLevel);

        /** Determines if the listeners can handle the specified message type. */
        bool CanHandle(LogMessageType type) const;

        /** Logs a formatted message. */
        void LogMessage(LogMessageType type, const wxChar* format, ...);

        /** Logs a message. */
        virtual void LogMessage(LogMessageType type, const wxString& message) = 0;

        /** Starts a multi-line message. */
        virtual void StartMessage(LogMessageType type, const wxString& heading);

        /** Continues a multi-line message. */
        virtual void ContinueMessage(LogMessageType type, const wxString& message);

        /** Ends a multi-line message. */
        virtual void EndMessage();

    protected:
        int indent;
        int depth;
        LogMessageType logLevel;
    };

    /** The main logger. */
    class Logger
    {
    public:
        /** Logs a formatted message. */
        static void LogMessage(LogMessageType type, const wxChar* format, ...);

        /** Logs a message. */
        static void LogMessage(LogMessageType type, const wxString& message);

        /** Logs a sequence of related messages. */
        static void LogResults(LogMessageType type, const wxArrayString& results, const wxString& heading);

        /** Calls SetLogLevel() on all listeners and modifies the default log level. */
        static void SetLogLevel(LogMessageType type);

        /** Increases the indentation level. */
        static void Indent();

        /** Decreases the indentation level. */
        static void Unindent();

        /** Increases the indentation level for listeners that can handle the specified log message type. */
        static void Indent(LogMessageType type);

        /** Increases the indentation level for listeners that can handle the specified log message type. */
        static void Unindent(LogMessageType type);

        /** Determines if there is a listener that can handle the specified message type. */
        static bool CanHandle(LogMessageType type);

        /** Adds a log listener. */
        static void AddListener(LogListener* listener);

        /** Removes a previously added log listener. */
        static void RemoveListener(LogListener* listener);

        /** Removes all log listeners. */
        static void ClearListeners();

    protected:
        typedef std::list<LogListener*> Listeners;
        static Listeners listeners;
    };

    /** Adds a temporary LogListener to Logger. */
    class ScopedLogListener
    {
    public:
        ScopedLogListener(LogListener& listener)
        {
            this->listener = &listener;
            Logger::AddListener(this->listener);
        }

        ~ScopedLogListener()
        {
            Logger::RemoveListener(this->listener);
        }

    private:
        LogListener* listener;
    };

} }


//Macros------------------------------------------------------------------------
//#define FINJIN_EXPORTER_LOG_MESSAGE(type, ...) ;
#define FINJIN_EXPORTER_LOG_MESSAGE(type, ...) if (Logger::CanHandle(type)) {Logger::LogMessage(type, wxString::Format(__VA_ARGS__));}

#define FINJIN_EXPORTER_DEV_LOG_MESSAGE(type, message) ;
//#define FINJIN_EXPORTER_DEV_LOG_MESSAGE(type, ...) if (Logger::CanHandle(type)) {Logger::LogMessage(type, wxString::Format(__VA_ARGS__));}
