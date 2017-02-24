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
#include "FinjinSceneTranslator.hpp"
#include "FinjinSceneExporterHandler.hpp"
#include "Exporter.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
FINJIN_IMPLEMENT_TRANSLATOR(FinjinSceneTranslator_FSTD, "1.0", "fstd-scene", "*.fstd-scene", "Finjin Streaming Text Scene File");
FINJIN_IMPLEMENT_TRANSLATOR(FinjinSceneTranslator_FSBD, "1.0", "fsbd-scene", "*.fsbd-scene", "Finjin Streaming Binary Scene File");
FINJIN_IMPLEMENT_TRANSLATOR(FinjinSceneTranslator_JSON, "1.0", "json-scene", "*.json-scene", "Finjin Streaming JSON Scene File");
FINJIN_IMPLEMENT_TRANSLATOR(FinjinSceneTranslator_CFG, "1.0", "cfg-scene", "*.cfg-scene", "Finjin Config Scene File");


//Implementation---------------------------------------------------------------
MStatus FinjinSceneTranslatorBase::writer
    (
    const MFileObject& file,
    const MString& options,
    MPxFileTranslator::FileAccessMode mode
    )
{
    MStatus status = MStatus::kNotImplemented;

    if (mode == MPxFileTranslator::kExportAccessMode ||
        mode == MPxFileTranslator::kExportActiveAccessMode)
    {
        Exporter::ExportWithFileName
            (
            ApplicationStringToWxString(file.fullName()), 
            mode == MPxFileTranslator::kExportActiveAccessMode, 
            &FinjinSceneExporterHandlerFactory::GetInstance(),
            true
            );

        status = MStatus::kSuccess;
    }

    return status;
}
