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
#include "MeshIconUtilities.hpp"
#include "StringUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
void MeshIconUtilities::DrawCustomIcon
    (
    ViewExp* viewport,
    const Point3* edgeVertices,
    size_t edgeVertexCount,
    const wxString* text,
    Matrix3* matrix,
    BOOL selected,
    BOOL frozen,
    COLORREF color
    )
{
    auto gw = viewport->getGW();

    //Set transformation matrix
    if (matrix != nullptr)
        gw->setTransform(*matrix);

    //Disable z buffer
    DWORD limits = gw->getRndLimits();
    gw->setRndLimits(limits & ~GW_Z_BUFFER);

    //Set drawing colors
    if (selected)
    {
        gw->setColor(TEXT_COLOR, GetUIColor(COLOR_SELECTION));
        gw->setColor(LINE_COLOR, GetUIColor(COLOR_SELECTION));
    }
    else if (frozen)
    {
        gw->setColor(TEXT_COLOR, GetUIColor(COLOR_FREEZE));
        gw->setColor(LINE_COLOR, GetUIColor(COLOR_FREEZE));
    }
    else
    {
        gw->setColor(TEXT_COLOR, Color(color));
        gw->setColor(LINE_COLOR, Color(color));
    }

    //Draw the edges
    Point3 v[2];
    for (size_t i = 0; i < edgeVertexCount; i += 2)
    {
        v[0] = edgeVertices[i];
        v[1] = edgeVertices[i + 1];
        gw->polyline(2, v, nullptr, nullptr, FALSE, nullptr);
    }

    //Draw text if necessary
    if (text != nullptr && !text->empty())
    {
        Point3 pos(0,0,0);
        auto maxText = WxStringToApplicationStringM(*text);
        gw->text(&pos, maxText);
    }

    //Restore original render limits
    gw->setRndLimits(limits);
}

Box3 MeshIconUtilities::GetCustomIconBox
    (
    const Point3* edgeVertices,
    size_t edgeVertexCount,
    Matrix3* matrix
    )
{
    Matrix3 m(1);
    if (matrix != nullptr)
        m = *matrix;

    Box3 box;
    Point3 v;
    for (size_t i = 0; i < edgeVertexCount; i++)
    {
        v = edgeVertices[i] * m;
        if (!box.Contains(v))
            box += v;
    }

    return box;
}

void MeshIconUtilities::MakeBoxCorners
    (
    Point3* p,
    float width,
    float height,
    float length
    )
{
    width *= 0.5f;
    height *= 0.5f;
    length *= 0.5f;

    //Top verts
    p[0] = Point3(-width, -length, height);
    p[1] = Point3(-width, length, height);
    p[2] = Point3(width, length, height);
    p[3] = Point3(width, -length, height);

    //Bottom verts
    p[4] = Point3(-width, -length, -height);
    p[5] = Point3(width, -length, -height);
    p[6] = Point3(width, length, -height);
    p[7] = Point3(-width, length, -height);
}

void MeshIconUtilities::MakeQuadCorners
    (
    Point3* p,
    float width,
    float length
    )
{
    width *= 0.5f;
    float height = 0;
    length *= 0.5f;

    p[0] = Point3(-width, -length, height);
    p[1] = Point3(-width, length, height);
    p[2] = Point3(width, length, height);
    p[3] = Point3(width, -length, height);;
}

void MeshIconUtilities::MakeBoxEdges
    (
    std::vector<Point3>& edgeVertices,
    float width,
    float height,
    float length,
    Matrix3* matrix
    )
{
    std::vector<Point3>& v = edgeVertices;

    //Determine transformation matrix
    Matrix3 m(1);
    if (matrix != nullptr)
        m = *matrix;

    //Get corners
    Point3 p[8];
    MakeBoxCorners(p, width, height, length);

    //Create edges
    v.resize(24);
    v[0] = p[0] * m; v[1] = p[1] * m;
    v[2] = p[1] * m; v[3] = p[2] * m;
    v[4] = p[2] * m; v[5] = p[3] * m;
    v[6] = p[3] * m; v[7] = p[0] * m;

    v[8] = p[4] * m; v[9] = p[5] * m;
    v[10] = p[5] * m; v[11] = p[6] * m;
    v[12] = p[6] * m; v[13] = p[7] * m;
    v[14] = p[7] * m; v[15] = p[4] * m;

    v[16] = p[0] * m; v[17] = p[4] * m;
    v[18] = p[1] * m; v[19] = p[7] * m;
    v[20] = p[2] * m; v[21] = p[6] * m;
    v[22] = p[3] * m; v[23] = p[5] * m;
}

void MeshIconUtilities::MakeQuadEdges
    (
    std::vector<Point3>& edgeVertices,
    float width,
    float length,
    Matrix3* matrix
    )
{
    std::vector<Point3>& v = edgeVertices;

    //Determine transformation matrix
    Matrix3 m(1);
    if (matrix != nullptr)
        m = *matrix;

    //Get corners
    Point3 p[4];
    MakeQuadCorners(p, width, length);

    //Create edges
    v.resize(8);
    v[0] = p[0] * m; v[1] = p[1] * m;
    v[2] = p[1] * m; v[3] = p[2] * m;
    v[4] = p[2] * m; v[5] = p[3] * m;
    v[6] = p[3] * m; v[7] = p[0] * m;
}

void MeshIconUtilities::MakeQuadEdgesWithNormal
    (
    std::vector<Point3>& edgeVertices,
    float width,
    float length,
    float normalLength,
    Matrix3* matrix
    )
{
    std::vector<Point3>& v = edgeVertices;

    //Determine transformation matrix
    Matrix3 m(1);
    if (matrix != nullptr)
        m = *matrix;

    //Get corners
    Point3 p[4];
    MakeQuadCorners(p, width, length);

    //Create edges
    v.resize(10);
    v[0] = p[0] * m; v[1] = p[1] * m;
    v[2] = p[1] * m; v[3] = p[2] * m;
    v[4] = p[2] * m; v[5] = p[3] * m;
    v[6] = p[3] * m; v[7] = p[0] * m;

    //Create the normal's edge
    //Offset it slightly from the center so that the 3DS Max axes don't block it
    float offset = 0.1f;
    v[8] = Point3(offset, offset, 0.0f) * m; v[9] = Point3(offset, offset, normalLength) * m;
}

Matrix3 MeshIconUtilities::MakeBillboardTM(Matrix3 m, ViewExp* viewport)
{
    //Get the object transformation without rotation
    AffineParts parts;
    decomp_affine(m, &parts);
    m.SetRow(0, Point3(parts.k.x, 0.0f, 0.0f));
    m.SetRow(1, Point3(0.0f, parts.k.y, 0.0f));
    m.SetRow(2, Point3(0.0f, 0.0f, parts.k.z));

    //Get the view-to-world rotation
    Matrix3 worldToView;
    viewport->GetAffineTM(worldToView);
    worldToView.SetTrans(Point3(0,0,0));
    Matrix3 viewToWorldRotation = Inverse(worldToView);

    //The billboard matrix will cancel out the world-to-view transformation
    //and scale/translate the object
    return viewToWorldRotation * m;
}

int MeshIconUtilities::HitTestCustomIcon
    (
    ViewExp* viewport,
    const std::vector<Point3>& edgeVertices,
    Matrix3 matrix,
    BOOL alwaysFaceView,
    TimeValue t,
    INode* inode,
    int type,
    int crossing,
    int flags,
    IPoint2* p
    )
{
    //Get viewport graphics window
    auto gw = viewport->getGW();

    //Set identity transform in graphics window
    Matrix3 tm(1);
    gw->setTransform(tm);

    //Get and modify current render limits
    DWORD savedLimits = gw->getRndLimits();
    gw->setRndLimits((savedLimits | GW_PICK) & ~GW_ILLUM);

    //Create hit region
    HitRegion hitRegion;
    MakeHitRegion(hitRegion, type, crossing, 4, p);
    gw->setHitRegion(&hitRegion);
    gw->clearHitCode();

    //Set into the graphics window the transform to be used when drawing the icon
    tm = matrix * inode->GetObjectTM(t);
    if (alwaysFaceView)
        tm = MakeBillboardTM(tm, viewport);
    gw->setTransform(tm);

    //Draw the icon
    DrawCustomIcon(viewport, &edgeVertices[0], edgeVertices.size(), nullptr, nullptr, false, false, 0);

    //Restore previous render limits
    gw->setRndLimits(savedLimits);

    if ((hitRegion.type != POINT_RGN) && !hitRegion.crossing)
        return TRUE;
    else
        return gw->checkHitCode();
}
