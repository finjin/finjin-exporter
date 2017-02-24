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
#include "SimpleMeshInfoBase.hpp"
#include "CoordinateSystemConverter.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class SimpleMeshInfo : public SimpleMeshInfoBase
    {
    public:
        /**
         * Determines if the specified object can have its mesh info extracted.
         * @param object [in] - The object to test.
         * @return Returns true if the object can have its mesh info extracted, false otherwise.
         */
        static bool CanHandle(ObjectAccessor object, TimeAccessor time = TimeAccessor());

        bool Create
            (
            ObjectAccessor object, 
            TimeAccessor time = TimeAccessor(), 
            TransformAccessor* transformation = nullptr
            );

        bool Create
            (
            ObjectAccessor object, 
            const CoordinateSystemConverter& coordinateConverter,
            float scale = 1.0f,
            TimeAccessor time = TimeAccessor(),
            TransformAccessor* transformation = nullptr
            ) override;
    };

} }
