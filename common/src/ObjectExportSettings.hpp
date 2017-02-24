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
#include "FinjinObjectSettingsAccessor.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "ManualLod.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class SceneExportSettings;

    enum class ObjectExportSettingsFlags
    {
        NONE = 0,

        /** Don't process level of detail settings. */
        IGNORE_LOD = 1 << 0
    };
    FINJIN_ENUM_BITWISE_OPERATIONS(ObjectExportSettingsFlags)

    /** Collects some common object settings used during the export process. */
    class ObjectExportSettings
    {
    public:
        void Initialize
            (
            ObjectAccessor object,
            FinjinObjectSettingsAccessor objectSettings,
            FinjinSceneSettingsAccessor sceneSettings,
            const SceneExportSettings& sceneExportSettings,
            ObjectExportSettingsFlags flags = ObjectExportSettingsFlags::NONE
            );

    public:
        //Mesh settings----------------------
        ManualLods manualLods;
        std::vector<FinjinObjectSettingsAccessor> manualLodsObjectSettings;
    };

} }
