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

    /** Handles the initialization and shutdown of a Direct3D interface. */
    class Direct3DState
    {
    public:
        /** Constructor. */
        Direct3DState();

        /** Destructor. */
        virtual ~Direct3DState();
         
        bool Create();
        void Destroy();

    public:
        /** The Direct3D interface. */
        IDirect3D9* d3d;

        /** The Direct3D device interface. */
        IDirect3DDevice9* d3dDevice;
    };

} }

#endif
