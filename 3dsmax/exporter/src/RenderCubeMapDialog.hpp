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


//Includes---------------------------------------------------------------------
#include "ResourceDialog.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {
    
    /** Dialog that lets users select the parameters for rendering a cube map. */
    class RenderCubeMapDialog : public ResourceDialog
    {
    public:
        RenderCubeMapDialog();

        int DoModal(HWND hWndParent = nullptr);

    protected:
        INT_PTR DialogProc(UINT message, WPARAM wParam, LPARAM lParam, bool& handled);

    public:
        int dimensions;
        INode* centerNode;

        enum Type
        {
            SEPARATE_FILES,
            DDS_FILE
        };
        Type type;
        wxString destinationDirectory;
        wxString destinationBaseFileName;
        wxString destinationFileName;

    protected:
        static int lastDimensions;        
        static Type lastType;
        static wxString lastDestinationDirectory;
        static wxString lastDestinationBaseFileName;
        static wxString lastDestinationFileName;

        ISpinnerControl* dimensionSpin;
    };

} }
