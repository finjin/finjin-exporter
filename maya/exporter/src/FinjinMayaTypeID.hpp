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

    /** Identifiers for all types. */
    class FinjinMayaTypeID
    {
    public:
        enum
        {
            SCENE_SETTINGS_NODE = 0x04152149,
            SCENE_SETTINGS_DATA = 0x04152150,
            OBJECT_SETTINGS_DATA = 0x04152151,

            CUSTOM_DUMMY_NODE = 0x04152152,
            INSTANCED_GEOMETRY_NODE = 0x04152159,
            STATIC_GEOMETRY_NODE = 0x0415215a
        };
    };

} }
