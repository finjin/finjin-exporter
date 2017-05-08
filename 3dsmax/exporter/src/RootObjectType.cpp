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
#include "RootObjectType.hpp"
#include "FinjinObjectSettingsAccessor.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "ObjectTypeDetectionContext.hpp"
#include "SceneSettingsTypes.hpp"
#include "FinjinSceneExporterHandler.hpp"
#include "ExporterContext.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Local types-------------------------------------------------------------------
class ExportedFinjinScene : public ExportedObject
{
public:
    void Write(WxDataChunkWriter& writer, WxError& error) override
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        this->scene.Write(writer, error);
    }

public:
    FinjinSceneDocument scene;
};


//Implementation-----------------------------------f----------------------------
void RootObjectType::Detect(ObjectTypeDetectionContext& context)
{
    if (context.object.node->IsRootNode())
        context.AddType(this, context.typeName.empty());
}

void RootObjectType::GetDependencies(ExportableObject* exportableObject, ExporterContext& context)
{
    for (int i = 0; i < SceneSettingsTypes::GetTypeCount(); i++)
    {
        ObjectAccessorList dependencies;
        SceneSettingsTypes::GetType(i)->GetDependencies(dependencies, context.GetSceneSettings());
        PushAll(exportableObject->dependentObjects, dependencies);
    }
}

std::unique_ptr<ExportedObject> RootObjectType::Export(ExportableObjectPtr& exportableObject, ExporterContext& context)
{
    std::unique_ptr<ExportedObject> exportedObject;

    if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(context.GetHandler(), FinjinSceneExporterHandler))
    {
        auto exportedScene = new ExportedFinjinScene;
        exportedObject.reset(exportedScene);

        ((FinjinSceneExporterHandler*)context.GetHandler())->ExportScene(exportableObject, exportedScene->scene, context);
    }

    return exportedObject;
}
