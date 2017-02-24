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
#include "ObjectExportSettings.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "SceneExportSettings.hpp"
#include "MathUtilities.hpp"
#include "ApplicationAccessor.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
void ObjectExportSettings::Initialize
    (
    ObjectAccessor object,
    FinjinObjectSettingsAccessor objectSettings,
    FinjinSceneSettingsAccessor sceneSettings,
    const SceneExportSettings& sceneExportSettings,
    ObjectExportSettingsFlags flags
    )
{
    //FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("ObjectExportSettings::Initialize(%s)"), object.GetLocalName().wx_str());
    
    if (NoneSet(flags & ObjectExportSettingsFlags::IGNORE_LOD))
    {
        objectSettings.GetManualLods(object, this->manualLods);
        this->manualLodsObjectSettings.resize(this->manualLods.size());
        for (size_t manualLodIndex = 0; manualLodIndex < this->manualLods.size(); manualLodIndex++)
        {
            this->manualLods[manualLodIndex].distance *= sceneExportSettings.scale;

            this->manualLodsObjectSettings[manualLodIndex] = sceneSettings.GetObjectSettings(this->manualLods[manualLodIndex].object);
        }
    }    
}
