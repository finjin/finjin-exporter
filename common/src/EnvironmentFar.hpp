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


//Includes----------------------------------------------------------------------
#include "ObjectAccessor.hpp"
#include "SimpleMeshInfo.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Gets the environment far distance based on either a mesh's dimensions or a float value. */
    class EnvironmentFar
    {
    public:
        /**
         * Initializes the far distance.
         * If neither of the inputs are valid, a default distance is used.
         * @param environmentFarObject [in] - If a valid mesh object, the size of this object
         * is used to generate the environment far distance.
         * @param environmentFar [in] - A floating point value indicating the far distance.
         */
        EnvironmentFar(ObjectAccessor environmentFarObject, float environmentFar)
        {
            this->value = environmentFar;
            if (environmentFarObject.IsValid())
            {
                SimpleMeshInfo mesh;
                if (mesh.Create(environmentFarObject))
                    this->value = mesh.GetBoundingBox().GetHalfSize().Average();
            }
            if (this->value <= 0)
                this->value = FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE;
        }

    public:
        /** The environment far distance. */
        float value;
    };

} }
