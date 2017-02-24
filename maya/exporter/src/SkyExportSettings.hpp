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
#include "FinjinSceneDocument.hpp"
#include "FinjinObjectSettingsAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class FinjinSceneExporterContext;

    /** Collects sky box settings. */
    class SkyExportSettings : public FinjinSceneDocument_Sky
    {
    public:
        SkyExportSettings(FinjinSceneDocument* scene = nullptr);

        void Initialize(ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, FinjinSceneExporterContext& finjinExporterContext);
    };

} }
