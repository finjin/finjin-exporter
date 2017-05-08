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
#include "GeometryStateBase.hpp"
#include "CoordinateSystemConverter.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class GeometryState : public GeometryStateBase
    {
    public:
        /**
         * Determines if the specified object can have its geometry state extracted.
         * @param object [in] - The object to test.
         * @return Returns true if the object can have its geometry state extracted, false otherwise.
         */
        static bool CanHandle(ObjectAccessor object);

        /**
         * Gets the texture coordinate set mappings for all the submeshes of the object.
         * @param object [in] - The object containing geometry whose texture coordinate sets should be retrieved.
         * @param submeshTextureCoordinateSetMappings [out] - The texture coordinate set mappings for all submeshes.
         */
        static void GetSubmeshesSettings
            (
            ObjectAccessor object,
            SubmeshesSettings& submeshTextureCoordinateSetMappings,
            GeometryStateFlags flags = GeometryStateFlags::NONE
            );

        bool Create
            (
            const wxString& meshName,
            ObjectAccessor object,
            const CoordinateSystemConverter& coordinateConverter,
            float scale = 1.0f,
            TimeAccessor time = TimeAccessor(),
            GeometryStateFlags flags = GeometryStateFlags::NONE,
            SubmeshesSettings* submeshesSettings = nullptr,
            const SkeletonReferencePose* referencePose = nullptr
            ) override;

        bool SamplePoints
            (
            std::vector<FinjinVector3>& positions,
            const CoordinateSystemConverter& coordinateConverter,
            float scale = 1.0f,
            TimeAccessor time = TimeAccessor(),
            GeometryStateFlags flags = GeometryStateFlags::NONE,
            MeshNormals* normals = nullptr
            ) override;
    };

} }
