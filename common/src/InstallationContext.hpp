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

    class InstallationContext
    {
    public:
        InstallationContext();
        ~InstallationContext();

        /**
         * Starts a new installation block
         * Installation blocks are hierarchical, so it's safe to call this multiple times.
         * This should always be paired with a call to FinishInstallationItem()
         */
        void StartInstallationItem(const wxString& displayName);

        /**
         * Finishes an installation block
         * This should always be paired with a call to StartInstallationItem()
         */
        void FinishInstallationItem();

        /**
         * Logs a result
         * @param success [in] - Indicates whether the result was a success or failure
         * @param message [in] - The message to log. This message is logged without modification
         */
        void LogResult(bool success, const wxString& message);

        void LogSuccess(const wxString& message);

        /**
         * Logs a failure result. This is preferable to calling LogResult(false, ...) since
         * the message is modified to clearly indicate that it was a failure
         */
        void LogFailure(const wxString& message);

        /**
         * Logs a failure result for a thing that didn't exist
         * @param thing [in] - Name of the thing that didn't exist
         */
        void LogDoesNotExist(const wxString& thing);

        bool LogDirectoryCopy(const wxString& source, const wxString& destination);

        bool LogDirectoryCreate(const wxString& path);

        bool LogDirectoryDelete(const wxString& path);

        bool LogOptionalDirectoryDelete(const wxString& path);

        /**
         * Performs a file copy and logs the result
         * @param source [in] - The source file path
         * @param destination [in] - The destionation file path. So that the logging makes sense,
         * this path should contain the file name (in other words, don't pass in a directory, even
         * though that will result in a successful copy)
         * @return The result of the copy is returned. A true result indicates the file was copied
         */
        bool LogFileCopy(const wxString& source, const wxString& destination);

        bool LogFileCopyDated(const wxString& source, const wxString& destination);

        bool LogFileCopyVersioned(const wxString& source, const wxString& destination);

        bool LogFileDelete(const wxString& fileName);

        bool LogOptionalFileDelete(const wxString& fileName);

        bool LogFileMove(const wxString& source, const wxString& destination);

        bool LogReadTextFile(const wxString& source, wxString& content);
        bool LogWriteTextFile(const wxString& destination, const wxString& content);

        bool LogExec(const wxString& command, bool logOutput, bool ignoreErrors = false);

        /** Aggregates the results. This should be called when the installation is complete */
        bool ComputeInstallationResults();

        /** Determines whether any results were logged */
        bool WasUsed() const;

        void ClearFailureFlag();
        bool GetFailureFlag() const;

    public:
        ApplicationVersion applicationVersion;

        BuildVersion buildVersion;

        wxString applicationRootDirectory;

        class InstallationInfo
        {
        public:
            InstallationInfo();
            InstallationInfo(const wxString& message);
            InstallationInfo(bool result, const wxString& message);
            ~InstallationInfo();

            bool ComputeResult();

        public:
            bool result;
            wxString message;
            std::list<std::shared_ptr<InstallationInfo> > childInfo;
        };

        std::shared_ptr<InstallationInfo> installationInfo;
        std::list<std::shared_ptr<InstallationInfo> > infoStack;
        int resultCount;
        bool failureFlag;
        wxWindow* mainWindow;
    };

} }
