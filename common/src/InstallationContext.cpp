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
#include "InstallationContext.hpp"
#include "FileUtilities.hpp"
#if defined(__WXMSW__)
    #include "FileVersionInfo.hpp"
#endif

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------

//InstallationContext::InstallationInfo---------
InstallationContext::InstallationInfo::InstallationInfo()
{
    this->result = true;        
}

InstallationContext::InstallationInfo::InstallationInfo(const wxString& message)
{
    this->result = true;
    this->message = message;
}

InstallationContext::InstallationInfo::InstallationInfo(bool result, const wxString& message)
{
    this->result = result;
    this->message = message;
}

InstallationContext::InstallationInfo::~InstallationInfo()
{
}

bool InstallationContext::InstallationInfo::ComputeResult()
{
    if (!this->childInfo.empty())
    {
        this->result = true;

        for (auto info : this->childInfo)
            this->result &= info->ComputeResult();
    }

    return this->result;
}


//InstallationContext---------
InstallationContext::InstallationContext()
{
    this->installationInfo.reset(new InstallationInfo);
    this->infoStack.push_back(this->installationInfo);
    this->resultCount = 0;
    this->failureFlag = false;
    this->mainWindow = 0;
}

InstallationContext::~InstallationContext()
{
}

void InstallationContext::StartInstallationItem(const wxString& displayName)
{   
    auto newInfo = std::make_shared<InstallationInfo>(displayName);

    //Add info to current item
    this->infoStack.back()->childInfo.push_back(newInfo);

    //Make info the current item
    this->infoStack.push_back(newInfo);
}

void InstallationContext::FinishInstallationItem()
{    
    //Remove the current info item
    this->infoStack.pop_back();
}

void InstallationContext::LogResult(bool success, const wxString& message)
{
    //Add the result to the current info item
    this->infoStack.back()->childInfo.push_back(std::make_shared<InstallationInfo>(success, message));
    this->resultCount++;

    if (!success)
        this->failureFlag = true;
}

void InstallationContext::LogSuccess(const wxString& message)
{
    LogResult(true, message);
}

void InstallationContext::LogFailure(const wxString& message)
{
    wxString failureMessage = wxT("Failure: ") + message;
    LogResult(false, failureMessage);
}

void InstallationContext::LogDoesNotExist(const wxString& thing)
{
    wxString message = wxString::Format(wxT("%s does not exist."), thing.wx_str());
    LogFailure(message);
}

bool InstallationContext::LogDirectoryCopy(const wxString& source, const wxString& destination)
{
    auto result = FileUtilities::CopyDirectory(source, destination);
    if (result)
        LogSuccess(wxString::Format(wxT("Copied directory '%s' to '%s'."), FileUtilities::PrettyPath(source).wx_str(), FileUtilities::PrettyPath(destination).wx_str()));
    else
        LogFailure(wxString::Format(wxT("Could not copy directory '%s'."), FileUtilities::PrettyPath(source).wx_str()));    
    return result;
}

bool InstallationContext::LogDirectoryCreate(const wxString& path)
{
    auto result = FileUtilities::CreateDirectoryRecursive(path);
    if (result)
        LogSuccess(wxString::Format(wxT("Created/verified directory '%s'."), FileUtilities::PrettyPath(path).wx_str()));
    else
        LogFailure(wxString::Format(wxT("Could not create directory '%s'."), FileUtilities::PrettyPath(path).wx_str()));    
    return result;
}

bool InstallationContext::LogDirectoryDelete(const wxString& path)
{
    auto result = FileUtilities::DeleteDirectory(path);
    if (result)
        LogSuccess(wxString::Format(wxT("Deleted directory '%s'."), FileUtilities::PrettyPath(path).wx_str()));
    else
        LogFailure(wxString::Format(wxT("Could not delete directory '%s'."), FileUtilities::PrettyPath(path).wx_str()));    
    return result;
}

bool InstallationContext::LogOptionalDirectoryDelete(const wxString& path)
{
    auto result = true;
    if (wxDirExists(path))
        result = LogDirectoryDelete(path);
    else
        LogSuccess(wxString::Format(wxT("Directory '%s' does not exist and was not deleted."), FileUtilities::PrettyPath(path).wx_str()));
    return result;
}

#define PRETTY_SOURCE_FILE(baseName, fullPath) (FileUtilities::PrettyPath(fullPath))

bool InstallationContext::LogFileCopy(const wxString& source, const wxString& destination)
{
    wxString sourceBaseName = wxFileName(source).GetFullName();
    auto result = FileUtilities::CopyFileEnsureReplaceable(source, destination);    
    if (result)
        LogSuccess(wxString::Format(wxT("Copied '%s' to '%s'."), PRETTY_SOURCE_FILE(sourceBaseName, source).wx_str(), FileUtilities::PrettyPath(destination).wx_str()));
    else
        LogFailure(wxString::Format(wxT("Could not copy file '%s'."), PRETTY_SOURCE_FILE(sourceBaseName, source).wx_str()));    
    return result;
}

#if defined(__WXMSW__)
static bool GetFileCreationTime(const wxString& file, FILETIME& fileTime)
{
    auto result = false;

    HANDLE hfile = CreateFile(file.wx_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hfile != INVALID_HANDLE_VALUE)
    {
        result = GetFileTime(hfile, &fileTime, 0, 0) == TRUE;
        CloseHandle(hfile);
    }

    return result;
}
#endif

bool InstallationContext::LogFileCopyDated(const wxString& source, const wxString& destination)
{
    wxString sourceBaseName = wxFileName(source).GetFullName();
    wxString destinationBaseName = wxFileName(destination).GetFullName();

#if defined(__WXMSW__)
    //Get source time
    FILETIME sourceFileTime;
    if (!GetFileCreationTime(source, sourceFileTime))
    {
        LogFailure(wxString::Format(wxT("Could not copy dated file '%s'. Unable to get creation time.'"), PRETTY_SOURCE_FILE(sourceBaseName, source).wx_str()));
        return false;
    }

    //Possibly compare the source version to the destination version
    if (wxFileExists(destination))
    {
        //Exit early if the destination doesn't have a time
        FILETIME destinationFileTime;
        if (!GetFileCreationTime(destination, destinationFileTime))
        {
            LogFailure(wxString::Format(wxT("Could not copy dated file '%s'. Unable to get creation time for existing file '%s'"), PRETTY_SOURCE_FILE(sourceBaseName, source).wx_str(), FileUtilities::PrettyPath(destination).wx_str()));
            return false;
        }

        //Exist early if the destination is a more recent version
        LONG compareResult = CompareFileTime(&destinationFileTime, &sourceFileTime);
        if (compareResult >= 0)
        {
            wxString reason;
            if (compareResult == 0)
                reason = wxT("the same");
            else
                reason = wxT("newer");
            LogSuccess(wxString::Format(wxT("Skipped copying '%s to '%s'. The existing file is %s."), PRETTY_SOURCE_FILE(sourceBaseName, source).wx_str(), FileUtilities::PrettyPath(destination).wx_str(), reason.wx_str()));
            return true;
        }
    }
#endif

    //At this point, either the destination file doesn't exist or it's older than the current one
    return LogFileCopy(source, destination);
}

bool InstallationContext::LogFileCopyVersioned(const wxString& source, const wxString& destination)
{
#if defined(__WXMSW__)
    wxString sourceBaseName = wxFileName(source).GetFullName();
    wxString destinationBaseName = wxFileName(destination).GetFullName();

    //Get source version info
    FileVersionInfo sourceVersionInfo;
    if (!sourceVersionInfo.Create(source.wx_str()))
    {
        LogFailure(wxString::Format(wxT("Could not copy versioned file '%s'. Unable to get version info.'"), PRETTY_SOURCE_FILE(sourceBaseName, source).wx_str()));
        return false;
    }

    //Possibly compare the source version to the destination version
    if (wxFileExists(destination))
    {
        //Exit early if the destination doesn't have a version
        FileVersionInfo destinationVersionInfo;
        if (!destinationVersionInfo.Create(destination.wx_str()))
        {
            LogFailure(wxString::Format(wxT("Could not copy versioned file '%s'. Unable to get version info for existing file '%s'"), PRETTY_SOURCE_FILE(sourceBaseName, source).wx_str(), FileUtilities::PrettyPath(destination).wx_str()));
            return false;
        }

        //Determine whether the file is 32-bit or 64-bit
        DWORD sourceBinaryType;
        DWORD destinationBinaryType;
        bool bitsMatch = true; //Assume both are 32-bit or 64-bit since this is the most likely scenario
        if (GetBinaryType(source.wx_str(), &sourceBinaryType) && 
            GetBinaryType(destination.wx_str(), &destinationBinaryType) &&
            sourceBinaryType != destinationBinaryType)
        {
            //32-bit/64-bit mismatch
            bitsMatch = false;
        }

        //Exist early if the destination is a more recent version
        if (bitsMatch && destinationVersionInfo.GetProductVersion() >= sourceVersionInfo.GetProductVersion())
        {
            wxString reason;
            if (destinationVersionInfo.GetProductVersion() == sourceVersionInfo.GetProductVersion())
                reason = wxT("the same");
            else
                reason = wxT("newer");
            LogSuccess(wxString::Format(wxT("Skipped copying '%s (version %s)' to '%s (version %s)'. The existing file is %s."), PRETTY_SOURCE_FILE(sourceBaseName, source).wx_str(), sourceVersionInfo.GetProductVersion().wx_str(), FileUtilities::PrettyPath(destination).wx_str(), destinationVersionInfo.GetProductVersion().wx_str(), reason.wx_str()));
            return true;
        }
    }
#endif

    //At this point, either the destination file doesn't exist or it's older than the current one
    return LogFileCopy(source, destination);
}

bool InstallationContext::LogFileDelete(const wxString& fileName)
{
    auto result = wxRemoveFile(fileName);
    if (result)
        LogSuccess(wxString::Format(wxT("Deleted file '%s'."), FileUtilities::PrettyPath(fileName).wx_str()));
    else
        LogFailure(wxString::Format(wxT("Could not delete file '%s'."), FileUtilities::PrettyPath(fileName).wx_str()));    
    return result;
}

bool InstallationContext::LogOptionalFileDelete(const wxString& fileName)
{
    auto result = true;
    if (wxFileExists(fileName))
        result = LogFileDelete(fileName);
    else
        LogSuccess(wxString::Format(wxT("File '%s' does not exist and was not deleted."), FileUtilities::PrettyPath(fileName).wx_str()));
    return result;
}

bool InstallationContext::LogFileMove(const wxString& source, const wxString& destination)
{
    //Perform the copy and delete
    bool copyResult = FileUtilities::CopyFileEnsureReplaceable(source, destination);    
    bool deleteResult = false;
    if (copyResult)
        deleteResult = wxRemoveFile(source);
    
    //Log results
    wxString sourceBaseName = wxFileName(source).GetFullName();
    if (copyResult)
    {
        if (deleteResult)
            LogSuccess(wxString::Format(wxT("Moved file '%s' to '%s'."), PRETTY_SOURCE_FILE(sourceBaseName, source).wx_str(), FileUtilities::PrettyPath(destination).wx_str()));
        else
            LogFailure(wxString::Format(wxT("Could not move file '%s'."), PRETTY_SOURCE_FILE(sourceBaseName, source).wx_str()));    
    }
    else
        LogFailure(wxString::Format(wxT("Could not copy file '%s' for move to '%s'."), PRETTY_SOURCE_FILE(sourceBaseName, source).wx_str(), FileUtilities::PrettyPath(destination).wx_str()));

    return copyResult && deleteResult;
}

bool InstallationContext::LogReadTextFile(const wxString& source, wxString& content)
{
    auto result = FileUtilities::ReadTextFile(source, content);
    if (!result)
        LogFailure(wxString::Format(wxT("Could not read file '%s'."), FileUtilities::PrettyPath(source).wx_str()));
    return result;
}

bool InstallationContext::LogWriteTextFile(const wxString& destination, const wxString& content)
{
    auto result = FileUtilities::WriteTextFile(destination, content);
    if (result)
        LogSuccess(wxString::Format(wxT("Wrote file '%s'."), FileUtilities::PrettyPath(destination).wx_str()));
    else
        LogFailure(wxString::Format(wxT("Could not write file '%s'."), FileUtilities::PrettyPath(destination).wx_str()));
    return result;
}

bool InstallationContext::LogExec(const wxString& command, bool logOutput, bool ignoreErrors)
{
    bool result;
    
    //Execute command
    wxArrayString output, errors;
    if (logOutput)
        result = wxExecute(command, output, errors) == 0;        
    else
        result = wxShell(command);

    //Log results
    if (result)
    {
        wxString message = wxString::Format(wxT("Executed command: %s"), command.wx_str());
        if (!output.empty())
        {
            message += wxT("\nOutput:");
            for (size_t i = 0; i < output.size(); i++)
            {
                message += wxT("\n");
                message += output[i];
            }            
        }
        if (!errors.empty())
        {
            if (ignoreErrors)
                message += wxT("\nErrors (Ignored):");
            else
                message += wxT("\nErrors:");
            for (size_t i = 0; i < errors.size(); i++)
            {
                message += wxT("\n");
                message += errors[i];
            }            
        }

        if (errors.empty() || ignoreErrors)
            LogSuccess(message);
        else
            LogFailure(message);
    }
    else
        LogFailure(wxString::Format(wxT("Could not execute command: %s"), command.wx_str()));

    return result;
};

bool InstallationContext::ComputeInstallationResults()
{
    return this->installationInfo->ComputeResult();
}

bool InstallationContext::WasUsed() const
{
    return this->resultCount > 0;
}

void InstallationContext::ClearFailureFlag()
{
    this->failureFlag = false;
}

bool InstallationContext::GetFailureFlag() const
{
    return this->failureFlag;
}
