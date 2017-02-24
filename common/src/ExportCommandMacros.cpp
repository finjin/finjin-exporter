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
#include "ExportCommandMacros.hpp"
#include "ApplicationAccessor.hpp"
#include "FileUtilities.hpp"
#include "Strings.hpp"
#include "MacroExpander.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
ExportCommandMacros::ExportCommandMacros()
{
    this->applicationDirectory = ApplicationAccessor::GetApplicationDirectory();    
}

void ExportCommandMacros::Finalize()
{
    FileUtilities::EnsureTrailingPathSeparator(this->applicationDirectory);
    FileUtilities::EnsureTrailingPathSeparator(this->exportDirectory);
    FileUtilities::EnsureTrailingPathSeparator(this->exportFilePath);
    FileUtilities::EnsureTrailingPathSeparator(this->exportFileName);
}

bool ExportCommandMacros::Expand(wxString& text)
{
    if (text.empty())
        return true;

    MacroExpander expander;

    expander.SetMatchErrorFormatString(Strings::EXPORT_COMMAND_MACRO_NO_VALUE_FOUND_FOR_SEQUENCE_FORMAT);
    
    expander.AddVariable(wxT("AppDir"), this->applicationDirectory);
    expander.AddVariable(wxT("ExportDir"), this->exportDirectory);
    expander.AddVariable(wxT("ExportFilePath"), this->exportFilePath);
    expander.AddVariable(wxT("ExportFileName"), this->exportFileName);

    return expander.Expand(text);
}
