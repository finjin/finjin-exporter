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
#include "MaterialExporterHandler.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
void MaterialExporterHandler::Initialize
    (
    const MaterialAccessor& material, 
    FinjinSceneSettingsAccessor sceneSettings, 
    const SceneExportSettings& sceneExportSettings
    )
{
    this->material = material;
    this->sceneSettings = sceneSettings;
    this->sceneExportSettings = &sceneExportSettings;
        
    this->material.Resolve();

    CalculateRequirements();
}

void MaterialExporterHandler::GetBitmapFileNames(GenericBitmapFileNameSet& bitmapFileNames) const 
{
    for (auto bitmapFileName : this->bitmapFileNames)
    {
        bitmapFileNames.insert(bitmapFileName);
    }
}

void MaterialExporterHandler::AddBitmapFileName(const wxString& fileName)
{
    if (!fileName.empty())
    {
        GenericBitmapFileName bitmapFileName(fileName, GenericBitmapFileName::STANDARD);
        this->bitmapFileNames.insert(bitmapFileName);
    }
}

void MaterialExporterHandler::AddAnimatedBitmapFileName(const wxString& fileName, int frameCount)
{
    if (!fileName.empty())
    {
        GenericBitmapFileName bitmapFileName(fileName, frameCount);
        this->bitmapFileNames.insert(bitmapFileName);
    }
}

void MaterialExporterHandler::AddCubicTextureBitmapFileBaseName(const wxString& fileName)
{
    if (!fileName.empty())
    {
        GenericBitmapFileName bitmapFileName(fileName, GenericBitmapFileName::CUBIC);
        this->bitmapFileNames.insert(bitmapFileName);
    }    
}
