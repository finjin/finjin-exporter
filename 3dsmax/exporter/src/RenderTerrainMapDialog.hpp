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

//Includes----------------------------------------------------------------------
#include "ResourceDialog.hpp"
#include "TerrainMapRenderer.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Dialog that lets users select the parameters for rendering terrain maps. */
    class RenderTerrainMapDialog : public ResourceDialog
    {
    public:
        typedef TerrainMapRenderer::Mode Mode;

        /** Constructor that takes a rendering mode. */
        RenderTerrainMapDialog(Mode mode);

        int DoModal(HWND hWndParent = nullptr);

    protected:
        INT_PTR DialogProc(UINT message, WPARAM wParam, LPARAM lParam, bool& handled);

    public:
        bool useSelectedObjects;
        int width;
        int height;
        wxString destinationFileName;
        int bytesPerPixel;
        bool rotate90DegreesClockwise;

    protected:
        Mode mode;

        bool isSquare;

        struct Settings
        {
            Settings(int width, int height)
            {
                this->width = width;
                this->height = height;
                this->bytesPerPixel = 1;
                this->isSquare = true;
                this->rotate90DegreesClockwise = true;
            }

            int width;
            int height;
            wxString destinationFileName;
            int bytesPerPixel;
            bool isSquare;
            bool rotate90DegreesClockwise;
        };

        static Settings lastTerrainMapSettings;
        static Settings lastHeightMapSettings;

        ISpinnerControl* widthSpin;
        ISpinnerControl* heightSpin;
        ISpinnerControl* bytesPerPixelSpin;

        Settings* lastActiveSettings;
        int useSelectedObjectsStringID;
        int useAllObjectsStringID;
        int titleStringID;
    };

} }

#endif
