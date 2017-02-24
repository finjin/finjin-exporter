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
#include "FinjinColor.hpp"
#include "TransformAccessor.hpp"
#include "TimeAccessor.hpp"
#include "FinjinNoteTrack.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** 
     * ObjectAccessorBase and the application-specific ObjectAccessor classes provide generic
     * access to application scene objects (not materials).
     */
    class ObjectAccessorBase
    {
    public:
        virtual ~ObjectAccessorBase() {}

        /**
         * Overridden object name. 
         * If non-empty, this is returned by the GetLocalName() methods instead of the actual object name.
         */
        wxString objectName;

        /**
         * Overridden node name. 
         * If non-empty, this is returned by the GetLocalName() methods instead of the actual node name.
         */
        wxString nodeName;
    };

} }
