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
#include "FilteredSceneObjects.hpp"
#include "MaxUtilities.hpp"
#include "GeometryState.hpp"

using namespace Finjin::Exporter;


//Classes----------------------------------------------------------------------
FilteredSceneObjects::FilteredSceneObjects(ObjectFilterType types) : FilteredSceneObjectsBase(types)
{        
}

void FilteredSceneObjects::Initialize(bool sortByFullName)
{
    this->items.clear();

    if (!this->chooseObjects.empty())
    {
        for (auto object : this->chooseObjects)
            HandleObject(object.node);
    }
    else
    {   
        ObjectAccessorVector allObjects;
        ObjectAccessor::GetRootObject().GetAllObjects(allObjects);
        for (size_t i = 0; i < allObjects.size(); i++)
            HandleObject(allObjects[i].node);
    }

    Sort(sortByFullName);
}

void FilteredSceneObjects::HandleObject(INode* node)
{
    FilteredObject filteredObject;

    auto object = node->GetObjectRef();
    if (this->excludeObjects.find(node) != this->excludeObjects.end())
    {
        //Do nothing
    }
    else if (MaxUtilities::IsAnyBone(node))
    {
        if (AnySet(this->types & ObjectFilterType::OTHER))
        {
            filteredObject.object = node;
            filteredObject.type = ObjectFilterType::OTHER;
        }
    }
    else if (object->ClassID() == PLANE_CLASS_ID)
    {
        filteredObject.object = node;
        filteredObject.type = ObjectFilterType::PLANE;
    }
    else if (GeometryState::CanHandle(node))
    {
        if (AnySet(this->types & ObjectFilterType::MESH))
        {
            filteredObject.object = node;
            filteredObject.type = ObjectFilterType::MESH;
        }
    }
    else if (object->SuperClassID() == CAMERA_CLASS_ID)
    {
        if (AnySet(this->types & ObjectFilterType::CAMERA))
        {
            filteredObject.object = node;
            filteredObject.type = ObjectFilterType::CAMERA;
        }
    }
    else if (object->SuperClassID() == LIGHT_CLASS_ID)
    {
        if (AnySet(this->types & ObjectFilterType::LIGHT))
        {
            filteredObject.object = node;
            filteredObject.type = ObjectFilterType::LIGHT;
        }
    }
    else if (AnySet(this->types & ObjectFilterType::OTHER))
    {
        filteredObject.object = node;
        filteredObject.type = ObjectFilterType::OTHER;
    }

    if (filteredObject.object.IsValid())
        this->items.push_back(filteredObject);
}
