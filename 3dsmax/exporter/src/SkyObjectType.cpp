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
#include "SkyObjectType.hpp"
#include "FinjinObjectSettingsAccessor.hpp"
#include "UserDataSettingsPage.hpp"
#include "ObjectSettingsDialog_SkyPage.hpp"
#include "ObjectSettingsDialog_NodeAnimationsPage.hpp"
#include "ObjectTypeDetectionContext.hpp"
#include "MaxUtilities.hpp"
#include "GeometryState.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
void SkyObjectType::Detect(ObjectTypeDetectionContext& context)
{
    if (context.maxObject != nullptr)
    {
        auto parent = context.object.node->GetParentNode();        
        if (parent->IsRootNode() && GeometryState::CanHandle(context.object))
            context.AddType(this, context.typeName.empty());
    }
}

void SkyObjectType::CreateSettingsPages(TabControlPages& tabPages, ObjectAccessor& object, FinjinObjectSettingsAccessor& objectSettings, FinjinSceneSettingsAccessor& sceneSettings)
{   
    tabPages.AddPage(new UserDataSettingsPage(tabPages.GetTabParentWindow(), objectSettings, UserDataUsage::OBJECT), UserDataSettingsPage::TITLE);
    tabPages.AddPage(new ObjectSettingsDialog_NodeAnimationsPage(tabPages.GetTabParentWindow(), object, objectSettings, sceneSettings), ObjectSettingsDialog_NodeAnimationsPage::TITLE);
    tabPages.AddPage(new ObjectSettingsDialog_SkyPage(tabPages.GetTabParentWindow(), objectSettings), ObjectSettingsDialog_SkyPage::TITLE);
}
