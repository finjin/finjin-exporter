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
#include "FinjinVersion.hpp"

#define RAW_VERSION_TEXT "0.5.0"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
const int FinjinVersion::NUMBER = 1;

const int FinjinVersion::MAJOR = 0;
const int FinjinVersion::MINOR = 5;
const int FinjinVersion::PATCH = 0;
const char* FinjinVersion::VERSION_TEXT_CSTR(RAW_VERSION_TEXT);
const wxString FinjinVersion::VERSION_TEXT(wxT(RAW_VERSION_TEXT));
