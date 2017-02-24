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
#include "MeshObjectType.hpp"
#include "FinjinObjectSettingsAccessor.hpp"
#include "UserDataSettingsPage.hpp"
#include "ObjectSettingsDialog_MeshPage.hpp"
#include "ObjectSettingsDialog_SubmeshesPage.hpp"
#include "ObjectSettingsDialog_MeshLodPage.hpp"
#include "ObjectSettingsDialog_MeshAnimationsPage.hpp"
#include "ObjectSettingsDialog_NodeAnimationsPage.hpp"
#include "ObjectTypeDetectionContext.hpp"
#include "GeometryState.hpp"
#include "ExporterContext.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
void MeshObjectType::Detect(ObjectTypeDetectionContext& context)
{
    if (GeometryState::CanHandle(context.object.obj))
        context.AddType(this, context.typeName.empty());
}

void MeshObjectType::CreateSettingsPages(TabControlPages& tabPages, ObjectAccessor& object, FinjinObjectSettingsAccessor& objectSettings, FinjinSceneSettingsAccessor& sceneSettings)
{    
    tabPages.AddPage(new UserDataSettingsPage(tabPages.GetTabParentWindow(), objectSettings, UserDataUsage::OBJECT), UserDataSettingsPage::TITLE);
    tabPages.AddPage(new ObjectSettingsDialog_NodeAnimationsPage(tabPages.GetTabParentWindow(), object, objectSettings, sceneSettings), ObjectSettingsDialog_NodeAnimationsPage::TITLE);    
    tabPages.AddPage(new ObjectSettingsDialog_MeshPage(tabPages.GetTabParentWindow(), object, objectSettings, sceneSettings), ObjectSettingsDialog_MeshPage::TITLE);
    tabPages.AddPage(new ObjectSettingsDialog_SubmeshesPage(tabPages.GetTabParentWindow(), object, objectSettings, sceneSettings), ObjectSettingsDialog_SubmeshesPage::TITLE);
    tabPages.AddPage(new ObjectSettingsDialog_MeshLodPage(tabPages.GetTabParentWindow(), object, objectSettings), ObjectSettingsDialog_MeshLodPage::TITLE);
    tabPages.AddPage(new ObjectSettingsDialog_MeshAnimationsPage(tabPages.GetTabParentWindow(), object, objectSettings, sceneSettings), ObjectSettingsDialog_MeshAnimationsPage::TITLE);
}

void MeshObjectType::GetDependencies(ExportableObject* exportableObject, ExporterContext& context)
{
    context.AddMeshDependencies(exportableObject);
}

void MeshObjectType::ResolveDependencies(ExportableObject* exportableObject, ExporterContext& context)
{
    context.ResolveMeshDependencies(exportableObject);
}
