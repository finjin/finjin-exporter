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


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Utility functions for dealing with the mesh icons used with Finjin helpers. */
    class MeshIconUtilities
    {
    public:
        /**
         * Draws the icon.
         * @param viewport [in] - The viewport being drawn into.
         * @param edgeVertices [in] - Array of pairs of vertices making up
         * the edges of a wireframe mesh icon.
         * @param edgeVertexCount [in] - The number of vertices in edgeVertices.
         * @param text [in] - The text to draw in addition to the edges. This can be null.
         * @param matrix [in] - A matrix used to transform vertices. This can be null.
         * @param selected [in] - Indicates whether the icon should be drawn in its
         * 'selected' state.
         * @param frozen [in] - Indicates whether the icon should be drawn in its
         * 'frozen' state. This is ignored if being drawn in its 'selected' state.
         * @param color [in] - The color to use if the icon is not being drawn in
         * its 'selected' or 'frozen' states.
         */
        static void DrawCustomIcon
            (
            ViewExp* viewport,
            const Point3* edgeVertices,
            size_t edgeVertexCount,
            const wxString* text,
            Matrix3* matrix,
            BOOL selected,
            BOOL frozen,
            COLORREF color
            );

        /**
         * Gets the icon's bounding box.
         * @param edgeVertices [in] - Array of pairs of vertices making up
         * the edges of a wireframe mesh icon.
         * @param edgeVertexCount [in] - The number of vertices in edgeVertices.
         * @param text [in] - The text to draw in addition to the edges. This can be null.
         * @param matrix [in] - A matrix used to transform vertices before calculating
         * the bounding box. This can be null.
         * @return The icon's bounding box.
         */
        static Box3 GetCustomIconBox
            (
            const Point3* edgeVertices,
            size_t edgeVertexCount,
            Matrix3* matrix
            );

        /**
         * Makes a wireframe box mesh of the given dimensions.
         * @param edgeVertices [out] - The output edge vertices.
         * @param width [in] - Width of the box.
         * @param height [in] - Height of the box.
         * @param length [in] - Length of the box.
         * @param matrix [in] - A matrix used to transform vertices. This can be null.
         */
        static void MakeBoxEdges
            (
            std::vector<Point3>& edgeVertices,
            float width,
            float height,
            float length,
            Matrix3* matrix
            );

        /**
         * Makes a wireframe quad mesh of the given dimensions.
         * @param edgeVertices [out] - The output edge vertices.
         * @param width [in] - Width of the quad.
         * @param length [in] - Length of the quad.
         * @param matrix [in] - A matrix used to transform vertices. This can be null.
         */
        static void MakeQuadEdges
            (
            std::vector<Point3>& edgeVertices,
            float width,
            float length,
            Matrix3* matrix
            );

        /**
         * Similar to MakeQuadEdges(), except an additional edge is created to represent a normal pointing away from the quad.
         * @param edgeVertices [out] - The output edge vertices.
         * @param width [in] - Width of the quad.
         * @param length [in] - Length of the quad.
         * @param normalLength [in] - Length of the normal.
         * @param matrix [in] - A matrix used to transform vertices. This can be null.
         */
        static void MakeQuadEdgesWithNormal
            (
            std::vector<Point3>& edgeVertices,
            float width,
            float length,
            float normalLength,
            Matrix3* matrix
            );

        /**
         * Creates a transformation matrix that makes an object point at the view.
         * @param m [in] - The object's transformation matrix.
         * @param viewport [in] - The viewport being drawn into.
         */
        static Matrix3 MakeBillboardTM(Matrix3 m, ViewExp* viewport);

        /**
         * Performs hit testing the icon.
         * @param viewport [in] - The viewport being drawn into.
         * @param edgeVertices [in] - Array of pairs of vertices making up
         * the edges of a wireframe mesh icon.
         * @param matrix [in] - A matrix used to transform vertices. This is usually
         * a scale matrix containing the dimensions of the icon.
         * @param alwaysFaceView [in] - Indicates whether the icon faces the view.
         * @param t [in] - The time at which the hit test is occurring.
         * @param inode [in] - The node that contains the object being hit tested.
         * @param type [in] - The type of hit testing to perform. This value is passed in to HitTest() by 3DS Max.
         * @param crossing [in] - The state of the crossing setting. This value is passed in to HitTest() by 3DS Max.
         * @param flags [in] - The hit test flags. This value is passed in to HitTest() by 3DS Max.
         * @param p [in] - The screen point to test. This value is passed in to HitTest() by 3DS Max.
         * @return The hit test result is returned.
         */
        static int HitTestCustomIcon
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
            );

    private:
        static void MakeBoxCorners(Point3* p, float width, float height, float length);

        static void MakeQuadCorners(Point3* p, float width, float length);
    };

} }
