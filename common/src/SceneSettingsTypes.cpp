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
#include "SceneSettingsTypes.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
SceneSettingsTypes::Types SceneSettingsTypes::types;


//Implementation----------------------------------------------------------------
void SceneSettingsTypes::RegisterType(SceneSettingsType& type)
{
    if (std::find(types.begin(), types.end(), &type) == types.end())
        types.push_back(&type);
}

int SceneSettingsTypes::GetTypeCount()
{
    return (int)types.size();
}

SceneSettingsType* SceneSettingsTypes::GetType(int i)
{
    return types[i];
}

void SceneSettingsTypes::ClearTypes()
{
    types.clear();
}
