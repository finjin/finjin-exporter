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
#include "HelpSystem.hpp"
#include "wxApplicationWindow.hpp"
#include "ApplicationAccessor.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
HelpSystem::HelpSystem(const wxString& helpFileName)
{
    //Create the help file controller
    this->helpFileName = helpFileName;
    
#if defined(__WXMSW__)
    this->appWindow = new wxApplicationWindow(false);
    this->helpController = new wxCHMHelpController(this->appWindow);
    this->helpController->Initialize(this->helpFileName);
#endif
}

HelpSystem::~HelpSystem()
{
#if defined(__WXMSW__)
    //Delete the help file controller
    this->helpController->Quit();
    delete this->helpController;
    
    delete this->appWindow;
#endif
}

void HelpSystem::Show()
{
#if defined(__WXMSW__)
    this->helpController->DisplayContents();
#else
    auto fileType = wxTheMimeTypesManager->GetFileTypeFromExtension(wxT("html"));
    if (fileType != nullptr)
    {
        wxString url;
        if (!this->helpFileName.empty())
            url = this->helpFileName;
        else
            url = wxT("http://finjin.com/support");
        
        auto command = fileType->GetOpenCommand(url);
        if (!command.empty())
            wxExecute(command);
    }
#endif
}
