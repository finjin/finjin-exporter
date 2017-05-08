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
#include "FinjinStaticGeometry.hpp"
#include "FinjinMayaTypeID.hpp"
#include "NodeAttributeAdder.hpp"
#include "MayaUtilities.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
FinjinStaticGeometry::Attributes FinjinStaticGeometry::attributes;

const MString FinjinStaticGeometry::TYPE_NAME("FinjinStaticGeometry");
const MTypeId FinjinStaticGeometry::TYPE_ID(FinjinMayaTypeID::STATIC_GEOMETRY_NODE);


//Implementation----------------------------------------------------------------
FinjinStaticGeometry::FinjinStaticGeometry()
{
}

FinjinStaticGeometry::~FinjinStaticGeometry()
{
}

MStatus FinjinStaticGeometry::compute(const MPlug& plug, MDataBlock& data)
{
    return MStatus::kSuccess;
}

void FinjinStaticGeometry::DrawLocator(M3dView& view, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{
    MObject object = thisMObject();
    float length = MPlug(object, attributes.length).asMDistance().as(UI_UNIT_TYPE);
    float width = MPlug(object, attributes.width).asMDistance().as(UI_UNIT_TYPE);
    float height = MPlug(object, attributes.height).asMDistance().as(UI_UNIT_TYPE);
    FinjinColor color = MayaPlug::GetColor(MPlug(object, attributes.color));

    if (status == M3dView::kLead || status == M3dView::kActive)
        glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
    else
    {
        //MColor outlineColor = colorRGB(status);
        //glColor4f(outlineColor.r, outlineColor.g, outlineColor.b, 0.5f);
        glColor4f(color.r, color.g, color.b, .5f);
    }

    DrawCube(width, height, length);
}

MBoundingBox FinjinStaticGeometry::boundingBox() const
{
    MObject object = thisMObject();
    float length = MPlug(object, attributes.length).asMDistance().as(UI_UNIT_TYPE);
    float width = MPlug(object, attributes.width).asMDistance().as(UI_UNIT_TYPE);
    float height = MPlug(object, attributes.height).asMDistance().as(UI_UNIT_TYPE);

    MBoundingBox bbox;

    bbox.expand(MPoint(-0.5f * width, 0.0f, 0.0f));
    bbox.expand(MPoint(0.5f * width, 0.0f, 0.0f));

    bbox.expand(MPoint(0.0f, -0.5f * height, 0.0f));
    bbox.expand(MPoint(0.0f, 0.5f * height, 0.0f));

    bbox.expand(MPoint(0.0f, 0.0f, -0.5f * length));
    bbox.expand(MPoint(0.0f, 0.0f, 0.5f * length));

    return bbox;
}

MStatus FinjinStaticGeometry::Initialize()
{
    return AddAttributes() == MStatus::kSuccess ? MStatus::kSuccess : MS::kFailure;
}

void* FinjinStaticGeometry::Creator()
{
    return new FinjinStaticGeometry();
}

MStatus FinjinStaticGeometry::AddAttributes()
{
    NodeAttributeAdder adder;

    attributes.length = adder.AddDistance("Length", MDistance(10, INTERNAL_UNIT_TYPE), 100);
    attributes.width = adder.AddDistance("Width", MDistance(10, INTERNAL_UNIT_TYPE), 100);
    attributes.height = adder.AddDistance("Height", MDistance(10, INTERNAL_UNIT_TYPE), 100);
    attributes.color = adder.AddColor("IconColor", MColor(0.0f, 1.0f, 0.0f, 0.5f));
    attributes.castShadows = adder.AddBool("CastShadows", true);

    return MS::kSuccess;
}
