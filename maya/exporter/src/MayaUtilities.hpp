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
#include "MayaPlug.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class AimTarget
    {
    public:
        bool IsValid() const
        {
            return !this->targetObject.isNull();
        }

    public:
        MObject targetObject;
        FinjinVector3 aimVector;
        FinjinVector3 upVector;
        
        enum WorldUpType
        {
            SCENE_UP = 0,
            OBJECT_UP = 1,
            OBJECT_ROTATION_UP = 2,
            VECTOR_UP = 3,
            NO_UP = 4
        };
        WorldUpType worldUpType;
        FinjinVector3 worldUpVector;
    };

    /** Various Maya utility functions. */
    class MayaUtilities
    {
    public:
        /**
         * Converts the triangle-relative indices to face-relative indices. This 
         * is used during triangulation, to get the indices of the original mesh vertices.
         * @param localIndex [out] - The output face-relative indices.
         * @param faceIndices [in] - Face-relative vertex indices. These indices map
         * back to mesh vertices.
         * @param triangleIndices [in] - Triangle-relative vertex indices. These indices
         * map back to face indices.
         */
        static void GetLocalMeshIndices
            (
            MIntArray& localIndex, 
            const MIntArray& faceIndices, 
            const MIntArray& triangleIndices
            );

        /** Deletes the specified object. */
        static void DeleteObject(MObject& obj);

        /** Deletes the specified object. */
        static void DeleteObject(MFnDagNode& dagNode);

        /** Deletes the object with the specified name. */
        static void DeleteObject(const MString& name);

        /** 
         * Determines if the specified object is a camera used in one of the standard viewports.
         * This determination is done by checking the camera's name against Maya's standard names.
         * @param obj [in] - The camera object.
         * @return If the camera is a viewport camera, true is returned. Otherwise, false is returned.
         */
        static bool IsStandardViewportCamera(MObject obj);

        /** Determines whether the specified object is the scene root. */
        static bool IsRoot(MObject obj);

        /** Gets the most appropriate parent for the object. */
        static MObject GetParent(MObject obj);

        /** Gets an object by name. */
        static MObject GetObjectByName(const MString& name);
        
        /** Determines whether the specified object contains some kind of mesh or curve object without any vertices. */
        static bool IsEmptyMeshOrCurve(MObject obj);

        /** 
         * Determines whether the specified object is some kind of Maya plane.
         * @param obj [in] - The object to check.
         * @param axisPlugs [out] - Collection of plugs that can be used to be used to
         * read the plane's axis, if the input object was a plane.
         * @return If the input object was a plane, true is returned. Otherwise, false is returned.
         */
        static bool IsPlane(MObject obj, std::vector<MPlug>* axisPlugs = 0);

        /** Determines whether the specified object is some kind of Maya sphere. */
        static bool IsSphereMesh(MObject obj);

        /** Determines whether the specified object is some kind of Maya cube. */
        static bool IsCubeMesh(MObject obj);

        /**
         * Gets the size and segment of the plane object.
         * @param size [out] - The retrieved plane size.
         * @param segmentCount [out] - The plane segment count.
         * @param object [in] - The plane object.
         * @return If the object was a plane and its size and segment count were retrieved, true is returned. 
         * Otherwise, false is returned.
         */
        static bool GetPlaneSizeAndSegmentCount
            (
            FinjinVector2& size, 
            FinjinIVector2& segmentCount, 
            MObject object
            );

        /**
         * Gets the radius and segment of the sphere object.
         * @param radius [out] - The retrieved sphere radius.
         * @param segmentCount [out] - The sphere segment count.
         * @param object [in] - The sphere object.
         * @return If the object was a sphere and its radius and segment count were retrieved, true is returned. 
         * Otherwise, false is returned.
         */
        static bool GetSphereRadiusAndSegmentCount
            (
            float& radius, 
            FinjinIVector2& segmentCount, 
            MObject object
            );

        /**
         * Gets the dimensions of the box object.
         * @param size [out] - The retrieved box dimensions.
         * @param object [in] - The box object.
         * @return If the object was a box and its dimensions were retrieved, true is returned. Otherwise,
         * false is returned.
         */
        static bool GetBoxSize(FinjinVector3& size, MObject object);

        /**
         * Gets the radius and length for the specified cylinder object.
         * @param radius [out] - The radius of the cylinder.
         * @param length [out] - The length of the cylinder.
         * @param object [in] - The object whose radius and length are retrieved.
         * @return If the specified object was a cylinder object and its radius and length were retrieved, true is
         * returned. Otherwise, false is returned.
         */
        static bool GetCylinderRadiusAndLength(float& radius, float& length, MObject object);

        /** 
         * Gets the object's world matrix at the current time.
         * @param object [in] - The object whose world matrix should be retrieved.
         * @param worldMatrix [out] - The retrieved world matrix.
         * @param addPivotOffset [in] - Indicates whether the pivot point should be added to the matrix.
         * @return If the world matrix was retrieved, true is returned. Otherwise, false is returned.
         */
        static bool GetFullWorldMatrix(MObject& object, MMatrix& worldMatrix);

        /** 
         * Gets the pivot point for the specified object at the specified time. 
         * The pivot point is the average of the rotate and scale pivots. Ideally, these two
         * should be the same.
         */
        static MVector GetPivotPoint(MObject obj, TimeAccessor time);

        /** 
         * Gets the pivot point for the specified object at the current time. 
         * The pivot point is the average of the rotate and scale pivots. Ideally, these two
         * should be the same.
         */
        static MVector GetPivotPoint(MObject obj);

        static bool GetAimTarget(MObject obj, AimTarget& aimTarget);
        
        /** 
         * Computes the inverse-transpose of the specified matrix. 
         * If the translation component is zero, this matrix can be used to transform normals.
         * @param m [in] - The matrix for which the inverse-transpose is computed.
         * @return The inverse-transpose is returned.
         */
        static MMatrix InverseTranspose(const MMatrix& m);

        /** 
         * Gets the best material for the shader object.
         * The 'best' material is the one that is either a Finjin material object, or non-null.
         */
        static MObject GetBestMaterial(MObject shader);

        /** Gets the active scale (the one that converts from internal units to user interface units). */
        static float GetActiveScale();

        /**
         * Finds the index of the specified string in a Maya string array.
         * @param stringArray [in] - The Maya string array to search.
         * @param s [in] - The string to search for.
         * @return If the string was found, the index is returned. Otherwise, -1 is returned.
         */
        static int FindString(const MStringArray& stringArray, const MString& s);

        static bool StartsWith(const char* s1, const char* s2);
        static bool EndsWith(const char* s1, const char* s2);
        static bool StartsWith(const MString& s1, const MString& s2);
        static bool EndsWith(const MString& s1, const MString& s2);
    };

} }
