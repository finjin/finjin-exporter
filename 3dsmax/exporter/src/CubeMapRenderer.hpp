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

//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Renders a cube map to a file or files. */
    class CubeMapRenderer
    {
    public:
        CubeMapRenderer();
        
        /** 
         * Sets the node that serves as the center point from which the rendering takes place.
         * @param node [in] - The node to set. This may be null. If null (the default), the 
         * rendering takes place from (0,0,0).
         */
        void SetCenterNode(INode* node);

        /** 
         * Performs the render.
         * @param fileName [in] - The file to which the cube map is rendered.
         * @param dimensions [in] - The dimensions of a cube map face.
         * @param separateFiles [in] - Indicates whether the cube map faces are 
         * rendered to separate files. If true, the output files are named with the following 
         * suffixes: _fr, _bk, _up, _dn, _lf, _rt. If false, all cube map faces are saved in a DDS file, 
         * regardless of the input file's extension.
         */
        void RenderToFile
            (
            const wxString& fileName, 
            int dimensions,
            bool separateFiles
            );

    protected:
        INode* centerNode;
    };

} }

#endif
