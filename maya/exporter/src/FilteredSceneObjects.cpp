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
#include "MayaUtilities.hpp"

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
        for (auto objectIterator = this->chooseObjects.begin();
            objectIterator != this->chooseObjects.end();
            ++objectIterator)
        {
            HandleObject(objectIterator->obj);
        }
    }
    else
    {    
        MItDag dagIterator(MItDag::kBreadthFirst, MFn::kInvalid);
        for (; !dagIterator.isDone(); dagIterator.next())
        {
            MDagPath object;
            dagIterator.getPath(object);
            MObject node = object.node();
            HandleObject(node);
        }   
    }

    Sort(sortByFullName);
}

void FilteredSceneObjects::HandleObject(MObject& object)
{
    MFnDagNode dagNode(object);
    if (dagNode.isIntermediateObject())
        return;

    FilteredObject filteredObject;

    if (object.hasFn(MFn::kWorld))
    {
        //Do nothing
    }
    else if (this->excludeObjects.find(object) != this->excludeObjects.end())
    {
        //Do nothing
    }
    else if (object.hasFn(MFn::kJoint))
    {
        if (AnySet(this->types & ObjectFilterType::OTHER))
        {
            filteredObject.object = object;
            filteredObject.type = ObjectFilterType::OTHER;
        }
    }
    else if (object.hasFn(MFn::kTransform))
    {
        //Do nothing
    }
    else if (MayaUtilities::IsPlane(object) && AnySet(this->types & ObjectFilterType::PLANE))
    {
        filteredObject.object = object;
        filteredObject.type = ObjectFilterType::PLANE;
    }
    else if (object.hasFn(MFn::kMesh) || object.hasFn(MFn::kNurbsSurface) || object.hasFn(MFn::kNurbsCurve))
    {
        if (AnySet(this->types & ObjectFilterType::MESH))
        {
            filteredObject.object = object;
            filteredObject.type = ObjectFilterType::MESH;
        }
    }
    else if (object.hasFn(MFn::kCamera))
    {
        if (AnySet(this->types & ObjectFilterType::CAMERA) && !MayaUtilities::IsStandardViewportCamera(object))
        {
            filteredObject.object = object;
            filteredObject.type = ObjectFilterType::CAMERA;
        }
    }
    else if (object.hasFn(MFn::kLight))
    {
        if (AnySet(this->types & ObjectFilterType::LIGHT) && !object.hasFn(MFn::kAmbientLight))
        {
            filteredObject.object = object;
            filteredObject.type = ObjectFilterType::LIGHT;
        }
    }
    else if (AnySet(this->types & ObjectFilterType::OTHER))
    {
        if (ObjectAccessor::IsTraversableAsNode(object))
        {
            filteredObject.object = object;
            filteredObject.type = ObjectFilterType::OTHER;
        }
    }

    if (filteredObject.object.IsValid())
        this->items.push_back(filteredObject);
}
