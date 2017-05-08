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

#if defined(FINJIN_EXPORTER_ENABLE_D3D_FUNCTIONALITY)

//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * Renders a terrain maps to a file.
     * The objects that are rendered can be specified explicitly, or not at all.
     * If no explicit objects are used, the entire scene is used.
     */
    class TerrainMapRenderer
    {
    public:
        /** The type of map to render. */
        enum Mode
        {
            /** The terrain colors. */
            COLOR_MAP,

            /** The terrain height. */
            HEIGHT_MAP
        };

        /** Constructor that takes a rendering mode. */
        TerrainMapRenderer(Mode mode);

        /** Adds the selected scene nodes to the render list. */
        void AddSelectedNodes();

        /** Adds the specified scene node to the render list. */
        void AddNode(INode* node);

        /**
         * Performs the render.
         * @param fileName [in] - The file to which the cube map is rendered.
         * @param width [in] - The width of the rendered image.
         * @param height [in] - The height of the rendered image.
         * @param bytesPerPixel [in] - The number of bytes per pixel. Should be 1 or 2.
         * @param rotate90DegreesClockwise [in] - Indicates whether the rotated image
         * should be rotated 90 degrees clockwise.
         */
        void RenderToFile
            (
            const wxString& fileName,
            int width,
            int height,
            int bytesPerPixel,
            bool rotate90DegreesClockwise
            );

    protected:
        Mode mode;
        std::unordered_set<INode*> nodesToRender;
    };

} }

#endif
