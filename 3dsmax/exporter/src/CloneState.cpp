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
#include "CloneState.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "FinjinObjectSettingsObject.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
CloneState::CloneState()
{
    Reset();
}

void CloneState::Reset()
{
    this->clonedNodeCount = 0;
    this->maybeClonedNodes.clear();
    this->previousPreviousSelectedNodes.clear();
    this->previousSelectedNodes.clear();
    this->selectedNodes.clear();
}

void CloneState::AddMaybeClonedNode(INode* node)
{
    this->maybeClonedNodes.push_back(node);
    this->clonedNodeCount = 0;
}

void CloneState::RemoveMaybeClonedNode(INode* node)
{
    ObjectAccessor object(node);
    auto foundAt = std::find(this->maybeClonedNodes.begin(), this->maybeClonedNodes.end(), object);
    if (foundAt != this->maybeClonedNodes.end())
        this->maybeClonedNodes.erase(foundAt);

    this->clonedNodeCount = 0;
}

void CloneState::OnCloneNode()
{
    if (++this->clonedNodeCount == this->maybeClonedNodes.size())
    {
        //Cloning operation complete

        FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings(false);
        if (sceneSettings.IsValid())
        {
            //There are Finjin object settings to clone
            auto sourceNode = this->previousPreviousSelectedNodes.begin();
            auto clonedNode = this->maybeClonedNodes.begin();
            for (;
                clonedNode != this->maybeClonedNodes.end() && sourceNode != this->previousPreviousSelectedNodes.end();
                ++clonedNode, ++sourceNode)
            {
                FinjinObjectSettingsAccessor sourceObjectSettings = sceneSettings.GetObjectSettings(*sourceNode);
                FinjinObjectSettingsAccessor clonedObjectSettings = sceneSettings.GetObjectSettings(*clonedNode);
                clonedObjectSettings.object->CopyFrom(sourceObjectSettings.object);
            }
        }

        this->maybeClonedNodes.clear();
    }
}

void CloneState::OnSelectionSetChanged()
{
    this->previousPreviousSelectedNodes = this->previousSelectedNodes;
    this->previousSelectedNodes = this->selectedNodes;
    ObjectAccessor::GetSelectedObjects(this->selectedNodes);
}
