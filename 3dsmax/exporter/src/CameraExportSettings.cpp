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
#include "CameraExportSettings.hpp"
#include "MathUtilities.hpp"
#include "FinjinSceneExporterContext.hpp"
#include "SceneExportSettings.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
CameraExportSettings::CameraExportSettings(FinjinSceneDocument* scene) : FinjinSceneDocument_Camera(scene)
{
}

void CameraExportSettings::Initialize(ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, FinjinSceneExporterContext& finjinExporterContext)
{
    Initialize(object, finjinExporterContext.GetSceneExportSettings()->time, finjinExporterContext.GetSceneExportSettings()->scale, finjinExporterContext.GetSceneExportSettings()->scaledWorldUnitsPerMeter);

    finjinExporterContext.GetMovableObjectSettings(this, object, objectSettings, finjinExporterContext.GetSceneExportSettings()->time, finjinExporterContext.GetSceneExportSettings()->conversionManager, finjinExporterContext.GetSceneExportSettings()->scale);
}

void CameraExportSettings::Initialize(ObjectAccessor object, TimeAccessor time, float scale, float scaledWorldUnitsPerMeter)
{
    CameraState cameraState;
    auto cameraObject = static_cast<CameraObject*>(object.node->EvalWorldState(time.GetNativeTime()).obj);
    cameraObject->EvalCameraState(time.GetNativeTime(), FOREVER, &cameraState);

    //Vertical field of view
    auto maxInterface = GetCOREInterface();
    float aspectRatio = (float)maxInterface->GetRendWidth()/(float)maxInterface->GetRendHeight();
    this->fov = MathUtilities::HorizontalToVerticalFOV(cameraState.fov, aspectRatio);

    //Orthographic?
    this->isOrtho = cameraState.isOrtho ? true : false;

    //Ortho window size
    float orthoWidth = 2 * cameraState.tdist * tanf(cameraState.fov / 2);
    float orthoHeight = 2 * cameraState.tdist * tanf(this->fov / 2);
    this->orthoWidth = orthoWidth * scale;
    this->orthoHeight = orthoHeight * scale;

    //Near/far clip
    this->nearClip = std::max(cameraState.hither * scale, .01f * scale);
    this->farClip = cameraState.yon * scale;
}
