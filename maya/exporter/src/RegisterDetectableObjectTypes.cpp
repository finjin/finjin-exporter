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
#include "ObjectTypeDetector.hpp"
#include "EmptyObjectType.hpp"
#include "IgnoreObjectType.hpp"
#include "PhysicalObjectType.hpp"
#include "MeshObjectType.hpp"
#include "SubmeshObjectType.hpp"
#include "SkyObjectType.hpp"
#include "LightObjectType.hpp"
#include "CameraObjectType.hpp"
#include "StaticGeometryObjectType.hpp"
#include "InstancedGeometryObjectType.hpp"
#include "RootObjectType.hpp"
#include "PrefabInstanceObjectType.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
void RegisterDetectableObjectTypes()
{
    //Universal types
    ObjectTypeDetector::RegisterType(EmptyObjectType::GetInstance());
    ObjectTypeDetector::RegisterType(IgnoreObjectType::GetInstance());
    ObjectTypeDetector::RegisterType(PrefabInstanceObjectType::GetInstance());
    ObjectTypeDetector::RegisterType(PhysicalObjectType::GetInstance());

    //Non-universal types
    ObjectTypeDetector::RegisterType(StaticGeometryObjectType::GetInstance());
    ObjectTypeDetector::RegisterType(InstancedGeometryObjectType::GetInstance());
    ObjectTypeDetector::RegisterType(LightObjectType::GetInstance());
    ObjectTypeDetector::RegisterType(CameraObjectType::GetInstance());
    ObjectTypeDetector::RegisterType(MeshObjectType::GetInstance());
    ObjectTypeDetector::RegisterType(SubmeshObjectType::GetInstance());
    ObjectTypeDetector::RegisterType(SkyObjectType::GetInstance());
    ObjectTypeDetector::RegisterType(RootObjectType::GetInstance());
}
