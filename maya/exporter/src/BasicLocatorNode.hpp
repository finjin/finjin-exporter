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


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Base class for some basic and common locator functionality. */
    class BasicLocatorNode : public MPxLocatorNode 
    {
    public:
        bool isBounded() const;

        void draw(M3dView& view, const MDagPath& dagPath, M3dView::DisplayStyle style, M3dView::DisplayStatus status);

        virtual void DrawLocator(M3dView& view, M3dView::DisplayStyle style, M3dView::DisplayStatus status) {}

    protected:
        void ApplyStateColour(float intensity, M3dView::DisplayStatus status);

        //    These functions draw a circle on the specified plane
        //    \param    r        -    the radius of the circle
        //    \param    divs    -    the number of divisions around the circle
        //
        void DrawCircleXY(float r, int divs);
        void DrawCircleXZ(float r, int divs);
        void DrawCircleYZ(float r, int divs);

        void DrawFilledCircleXY(float r, int divs);
        void DrawFilledCircleXZ(float r, int divs);
        void DrawFilledCircleYZ(float r, int divs);


        //    These functions draw a quad in the specified plane.
        //    \brief    w    -    the width
        //    \brief    h    -    the height
        //
        void DrawQuadXY(float w, float h);
        void DrawQuadXZ(float w, float h);
        void DrawQuadYZ(float w, float h);

        void DrawFilledQuadXY(float w, float h);
        void DrawFilledQuadXZ(float w, float h);
        void DrawFilledQuadYZ(float w, float h);


        //    These functions draw a 2 crossed lines in the specified plane.
        //    \brief    w    -    the width
        //    \brief    h    -    the height
        //
        void DrawCrossXY(float w, float h);
        void DrawCrossXZ(float w, float h);
        void DrawCrossYZ(float w, float h);

        //    These functions draw an arc on the specified plane
        //    \param    sr        -    the start radius of the spiral
        //    \param    er        -    the end radius of the spiral
        //    \param    sa        -    the start angle in degrees
        //     \param    ea        -    the end angle in degrees
        //    \param    h        -    the height of the spiral to draw
        //    \param    divs    -    the number of divisions around the circle
        //
        void DrawSpiralXY(float sr, float er, float sa, float ea, float h, int divs);
        void DrawSpiralXZ(float sr, float er, float sa, float ea, float h, int divs);
        void DrawSpiralYZ(float sr, float er, float sa, float ea, float h, int divs);


        void DrawSphere(float r);
        void DrawCone(float r, float h);
        void DrawCube(float w, float h, float d);
        void DrawFilledCube(float w, float h, float d);

        //    These functions draw a 2 crossed lines in the specified plane.
        //    \brief    r    -    the radius
        //    \brief    h    -    the height
        //
        void DrawCylinderXY(float r, float h);
        void DrawCylinderXZ(float r, float h);
        void DrawCylinderYZ(float r, float h);
    };

} }
