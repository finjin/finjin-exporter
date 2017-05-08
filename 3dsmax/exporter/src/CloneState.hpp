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


//Includes----------------------------------------------------------------------
#include "ObjectAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * Assists in handling the cloning of Finjin object settings.
     * Used exclusively by the FinjinGlobalUtilities class.
     */
    class CloneState
    {
    public:
        CloneState();

        /** Resets the clone state. */
        void Reset();

        /** Adds a node to what may become a set of cloned nodes. */
        void AddMaybeClonedNode(INode* node);

        /** Removes a node from what may become a set of cloned nodes. */
        void RemoveMaybeClonedNode(INode* node);

        /**
         * Call this in response to a 3DS "clone" notification.
         * This may perform cloning of Finjin object settings.
         */
        void OnCloneNode();

        /**
         * Call this in response to a 3DS "selection change" notification.
         * Selection changes actually occur during 3DS Max's cloning of multiple
         * selected objects.
        */
        void OnSelectionSetChanged();

    private:
        size_t clonedNodeCount;
        typedef ObjectAccessorVector Nodes;
        Nodes maybeClonedNodes;
        Nodes previousPreviousSelectedNodes;
        Nodes previousSelectedNodes;
        Nodes selectedNodes;
    };

} }
