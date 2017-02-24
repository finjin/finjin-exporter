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
#include "FinjinBox.hpp"
#include "ObjectAccessor.hpp"
#include "CoordinateSystemConverter.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * Base mesh class that collects the data and methods common to all the
     * application-specific SimpleMesh classes. This class is used to extract very
     * simple information from a mesh, such as its bounding box, bounding radius, and 
     * plane (if it is known to contain a planar set of faces).
     */
    class SimpleMeshInfoBase
    {
    public:
        /** Constructor. */
        SimpleMeshInfoBase();

        /** Destructor. */
        virtual ~SimpleMeshInfoBase() {}

        /**
         * Creates the simple mesh info without any coordinate/scale conversion.
         * @param object [in] - The object whose mesh info is extracted.
         * @param time [in] - The time at which the mesh info is evaluated.
         * @param transformation [in] - An additional, optional, transformation applied to points and normals.
         * @return If the mesh info was retrieved, true is returned. Otherwise, false is returned.
         */
        bool Create
            (
            ObjectAccessor object, 
            TimeAccessor time = TimeAccessor(), 
            TransformAccessor* transformation = nullptr
            );

        /** 
         * Creates the simple mesh info with coordinate/scale conversion
         * @param object [in] - The object whose mesh info is extracted.
         * @param coordinateConverter [in] - The coordinate converter used to convert points, normals, and transformations.
         * @param scale [in] - The scale applied to points, skeleton bones, and transformations.
         * @param time [in] - The time at which the mesh info is evaluated.
         * @param transformation [in] - An additional, optional, transformation applied to points and normals.
         * @return If the mesh info was retrieved, true is returned. Otherwise, false is returned.
         */
        virtual bool Create
            (
            ObjectAccessor object, 
            const CoordinateSystemConverter& coordinateConverter,
            float scale = 1.0f,
            TimeAccessor time = TimeAccessor(),
            TransformAccessor* transformation = nullptr
            ) = 0;

        /** Gets the unconverted bounding radius. */
        float GetRawBoundingRadius() const;

        /** Gets the unconverted bounding box. */
        const FinjinBox3& GetRawBoundingBox() const;

        /** Gets the unconverted plane. */
        const FinjinVector4& GetRawPlane() const;

        /** Gets the converted bounding radius. */
        float GetBoundingRadius() const;

        /** Gets the converted bounding box. */
        const FinjinBox3& GetBoundingBox() const;

        /** Gets the converted plane. */
        const FinjinVector4& GetPlane() const;

    protected:
        /** Resets the bounding volumes. */
        void ResetBounds();

        /**
         * Updates the bounding volumes.
         * @param isFirst [in] - Indicates whether this is the first time UpdateBounds() is being called.
         * If true, this resets the bounding volumes.
         * @param rawVertex [in] - An unconverted point in the geometry that may modify the bounding volume.
         * @param vertex [in] - A converted point in the geometry that may modify the bounding volume.
         */
        void UpdateBounds(bool isFirst, const FinjinVector3& rawVertex, const FinjinVector3& vertex);

        /** Called when the bounding volume initialization is done. */
        void FinishBounds();
        
        /**
         * Initializes the member planes.
         * @param rawPlaneVertices [in] - At least 3 unconverted vertices that define a face.
         * @param planeVertices [in] - At least 3 converted vertices that define a face.
         */
        void InitializePlanes(const FinjinVector3* rawPlaneVertices, const FinjinVector3* planeVertices);

    protected:
        float rawBoundingRadius;
        FinjinBox3 rawBoundingBox;
        FinjinVector4 rawPlane;

        float boundingRadius;
        FinjinBox3 boundingBox;
        FinjinVector4 plane;
    };

} }
