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
#include "BasicLocatorNode.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** The Finjin instanced geometry container locator. */
    class FinjinInstancedGeometry : public BasicLocatorNode
    {
    public:
        FinjinInstancedGeometry();
        ~FinjinInstancedGeometry();

        MStatus compute(const MPlug& plug, MDataBlock& data);

        void DrawLocator(M3dView& view, M3dView::DisplayStyle style, M3dView::DisplayStatus status);

        MBoundingBox boundingBox() const;

        static void* Creator();
        static MStatus Initialize();

    private:
        static MStatus AddAttributes();

    public:
        static const MString TYPE_NAME;
        static const MTypeId TYPE_ID;

        struct Attributes
        {
            MObject length;
            MObject width;
            MObject height;
            MObject color;
            MObject batchCount;
            MObject castShadows;
        };
        static Attributes attributes;
    };

} }
