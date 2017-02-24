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
#include "TimeChanger.hpp"
#include "MayaUtilities.hpp"
#include "FinjinSceneExporterContext.hpp"
#include "SceneExportSettings.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
LightExportSettings::LightExportSettings(FinjinSceneDocument* scene) : FinjinSceneDocument_Light(scene)
{
}

void LightExportSettings::Initialize(ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, FinjinSceneExporterContext& finjinExporterContext)
{
    TimeChanger timeChanger(finjinExporterContext.GetSceneExportSettings()->time);
    MFnLight light(object.obj);

    finjinExporterContext.GetMovableObjectSettings(this, object, objectSettings, finjinExporterContext.GetSceneExportSettings()->time, finjinExporterContext.GetSceneExportSettings()->conversionManager, finjinExporterContext.GetSceneExportSettings()->scale);

    //Light type
    if (object.obj.hasFn(MFn::kPointLight))
        this->lightType = LightType::POINT_LIGHT;
    else if (object.obj.hasFn(MFn::kDirectionalLight))
        this->lightType = LightType::DIRECTIONAL_LIGHT;
    else if (object.obj.hasFn(MFn::kSpotLight))
        this->lightType = LightType::SPOT_LIGHT;
    else
        this->lightType = LightType::DIRECTIONAL_LIGHT;
    
    //Shadows
    this->castShadows = 
        MayaPlug::GetBoolAttribute(object.obj, "useDepthMapShadows", true, finjinExporterContext.GetSceneExportSettings()->time) ||
        MayaPlug::GetBoolAttribute(object.obj, "useRayTraceShadows", true, finjinExporterContext.GetSceneExportSettings()->time);

    //Power
    this->power = light.intensity();

    //Color
    MColor color = light.color();
    this->color.Set(color.r, color.g, color.b, color.a);
        
    //Range
    float range = MayaPlug::GetFloatAttribute(object.obj, "fogRadius", 0, finjinExporterContext.GetSceneExportSettings()->time) * finjinExporterContext.GetSceneExportSettings()->scale;
    if (range > 0)
        this->range[1] = range;
    
    //Spotlight range
    if (this->lightType == LightType::SPOT_LIGHT)
    {
        float coneAngle = MayaPlug::GetFloatAttribute(object.obj, "coneAngle", 0, finjinExporterContext.GetSceneExportSettings()->time);
        float coneSpread = fabsf(MayaPlug::GetFloatAttribute(object.obj, "penumbraAngle", 0, finjinExporterContext.GetSceneExportSettings()->time));

        this->coneRange[0] = coneAngle - coneSpread;
        this->coneRange[1] = coneAngle + coneSpread;
    }

    //On?
    this->isOn = 
        HasEffect(false) && 
        (MayaPlug::GetBoolAttribute(object.obj, "emitDiffuse", false, finjinExporterContext.GetSceneExportSettings()->time) || 
         MayaPlug::GetBoolAttribute(object.obj, "emitSpecular", false, finjinExporterContext.GetSceneExportSettings()->time));    
}
