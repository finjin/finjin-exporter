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
#include "LightExportSettings.hpp"
#include "FinjinSceneExporterContext.hpp"
#include "SceneExportSettings.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
LightExportSettings::LightExportSettings(FinjinSceneDocument* scene) : FinjinSceneDocument_Light(scene)
{
}

void LightExportSettings::Initialize(ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, FinjinSceneExporterContext& finjinExporterContext)
{
    finjinExporterContext.GetMovableObjectSettings(this, object, objectSettings, finjinExporterContext.GetSceneExportSettings()->time, finjinExporterContext.GetSceneExportSettings()->conversionManager, finjinExporterContext.GetSceneExportSettings()->scale);

    LightState lightState;
    auto lightObject = static_cast<LightObject*>(object.node->EvalWorldState(finjinExporterContext.GetSceneExportSettings()->time.GetNativeTime()).obj);
    lightObject->EvalLightState(finjinExporterContext.GetSceneExportSettings()->time.GetNativeTime(), FOREVER, &lightState);

    //Light type
    switch (lightState.type)
    {
        case FSPOT_LIGHT:
        case TSPOT_LIGHT: this->lightType = LightType::SPOT_LIGHT; break;
        case DIR_LIGHT:
        case TDIR_LIGHT: this->lightType = LightType::DIRECTIONAL_LIGHT; break;
        case OMNI_LIGHT: this->lightType = LightType::POINT_LIGHT; break;
        default: this->lightType = LightType::DIRECTIONAL_LIGHT; break;
    }

    //Shadows
    this->castShadows = lightState.shadow ? true : false;

    //Power
    this->power = lightState.intens;

    //Color
    this->color.Set(lightState.color.r, lightState.color.g, lightState.color.b);

    //Range
    float rangeEnd = lightState.attenEnd * finjinExporterContext.GetSceneExportSettings()->scale;
    if (rangeEnd > 0)
    {
        this->range[0] = lightState.attenStart * finjinExporterContext.GetSceneExportSettings()->scale;
        this->range[1] = rangeEnd;
    }

    //Spotlight range
    if (this->lightType == LightType::SPOT_LIGHT)
    {
        this->coneRange[0] = DegreesToRadians(lightState.hotsize);
        this->coneRange[1] = DegreesToRadians(lightState.fallsize);
    }

    //On?
    this->isOn = lightState.on ? true : false;
}
