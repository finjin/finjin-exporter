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
#include "FinjinSceneSettingsAccessor.hpp"
#include "TabControl.hpp"
#include "ObjectAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** 
     * A base class for defininig classes of scene settings.
     * These settings aren't really types, but they can be thought of as such since the
     * settings correspond to a page of related settings.
     */
    class SceneSettingsType
    {
    public:
        virtual ~SceneSettingsType() {}

        /** Creates the settings pages for the type. */
        virtual void CreateSettingsPages(TabControlPages& tabPages, FinjinSceneSettingsAccessor& sceneSettings) {}

        /** Gets the objects that the scene type depends on. */
        virtual void GetDependencies(ObjectAccessorList& dependencies, FinjinSceneSettingsAccessorBase& sceneSettings) {}
    };

} }
