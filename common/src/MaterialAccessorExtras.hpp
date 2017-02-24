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


//Includes---------------------------------------------------------------------
#include "AccessorSetAndMap.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    typedef AccessorSet<MaterialAccessor> MaterialAccessorSet;
    typedef std::vector<MaterialAccessor> MaterialAccessorVector;
    template <class T>
    class MaterialAccessorMap : public std::map<MaterialAccessor, T>
    {
    };

    typedef std::list<std::pair<MaterialAccessor, wxString> > MaterialToString;

} }
