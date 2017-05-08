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
#include "ChooseObjectsPrompt-Max.hpp"
#include "MaxUtilities.hpp"
#include "GeometryState.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------

//ChooseObjectsPromptMax::SelectObjectCallback
const MCHAR* ChooseObjectsPromptMax::SelectObjectCallback::dialogTitle()
{
    return MaxUtilities::GetStringM(this->prompt->howMany == HowMany::CHOOSE_ONE ? IDS_CHOOSE_OBJECT : IDS_CHOOSE_OBJECTS);
}

const MCHAR* ChooseObjectsPromptMax::SelectObjectCallback::buttonText()
{
    return MaxUtilities::GetStringM(IDS_OK);
}

int ChooseObjectsPromptMax::SelectObjectCallback::filter(INode* node)
{
    BOOL result = FALSE;

    if (this->prompt->excludeObjects.find(node) != this->prompt->excludeObjects.end())
    {
        //Do nothing
    }
    else if (!this->prompt->chooseObjects.empty())
        result = this->prompt->chooseObjects.find(node) != this->prompt->chooseObjects.end();
    else
    {
        auto objectRef = node->GetObjectRef();
        BOOL isMesh = GeometryState::CanHandle(node);
        BOOL isPlane = objectRef->ClassID() == PLANE_CLASS_ID;
        BOOL isCamera = objectRef->SuperClassID() == CAMERA_CLASS_ID;
        BOOL isLight = objectRef->SuperClassID() == LIGHT_CLASS_ID;

        if (AnySet(prompt->types & ObjectFilterType::MESH))
            result |= isMesh;
        if (AnySet(prompt->types & ObjectFilterType::PLANE))
            result |= isPlane;
        if (AnySet(prompt->types & ObjectFilterType::CAMERA))
            result |= isCamera;
        if (AnySet(prompt->types & ObjectFilterType::LIGHT))
            result |= isLight;
        if (AnySet(prompt->types & ObjectFilterType::OTHER))
            result |= !isMesh && !isPlane && !isCamera && !isLight;
    }

    return result;
}

void ChooseObjectsPromptMax::SelectObjectCallback::proc(INodeTab& nodeTab)
{
    this->prompt->objects.resize(nodeTab.Count());
    for (int i = 0; i < nodeTab.Count(); i++)
        this->prompt->objects[i] = nodeTab[i];
}

//ChooseObjectsPromptMax
ChooseObjectsPromptMax::ChooseObjectsPromptMax
    (
    wxWindow* parent,
    HowMany howMany,
    ObjectFilterType types,
    const ObjectAccessorSet* chooseObjects,
    const ObjectAccessorSet* excludeObjects
    )
{
    this->parent = parent;
    this->howMany = howMany;
    this->types = types;
    if (chooseObjects != nullptr)
        this->chooseObjects = *chooseObjects;
    if (excludeObjects != nullptr)
        this->excludeObjects = *excludeObjects;
}

bool ChooseObjectsPromptMax::ShowPrompt()
{
    SelectObjectCallback callback(this);
    return GetCOREInterface()->DoHitByNameDialog(&callback) ? true : false;
}

ObjectAccessor ChooseObjectsPromptMax::GetObject()
{
    ObjectAccessor object;
    if (!this->objects.empty())
        object = this->objects[0];
    return object;
}
