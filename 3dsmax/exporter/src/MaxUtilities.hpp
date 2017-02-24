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
#include "FinjinVector.hpp"
#include "CoordinateSystemConverter.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** A collection of 3DS Max utility functions. */
    class MaxUtilities
    {
    public:
        /** 
         * Gets the value of the specified property as a string. 
         * @param propertySet [in] - The property set identifier. 
         * @param propid [in] - The property identifier.
         * @return The property value is returned.
         */
        static wxString GetPropertyString(int propertySet, PROPID propid);

        /** 
         * Sets the value of the specified property as a string. 
         * @param propertySet [in] - The property set identifier. 
         * @param propid [in] - The property identifier.
         * @param value [in] - The property value.
         */
        //static void SetPropertyString(int propertySet, PROPID propid, const wxString& value);

        /** Gets the scene's summary information. */
        static wxString GetSceneSummaryInfo();

        /** Sets the scene's summary information. */
        //static void SetSceneSummaryInfo(const wxString& text);

        /** 
         * Determines whether an array of ClassIDs contains a particular ClassID.
         * @param classIDs [in] - An array of ClassIDs.
         * @param classIDCount [in] - The number of ClassIDs.
         * @param id [in] - The ClassID to search for.
         * @return If the ClassID can be found, true is returned. Otherwise,
         * false is returned.
         */
        static bool ContainsClassID
            (
            const Class_ID* classIDs, 
            int classIDCount, 
            Class_ID id
            );

        /** 
         * Searches through the object's modifier stack to find the modifier with any of the specified ClassIDs.
         * @param node [in] - The node whose modifier stack is searched.
         * @param classIDs [in] - An array of ClassIDs.
         * @param classIDCount [in] - The number of ClassIDs.
         * @param id [in] - The ClassID to search for.
         * @return The matching modifier is found. It may be null.
         */
        static Modifier* FindModifier
            (
            INode* node, 
            const Class_ID* classIDs, 
            int cidCount
            );

        /** 
         * Searches through the object's modifier stack to find a modifier with the specified ClassID.
         * @param node [in] - The node whose modifier stack is searched.
         * @param id [in] - The ClassID to search for.
         * @return The matching modifier is found. It may be null.
         */
        static Modifier* FindModifier(INode* node, Class_ID id);

        /** 
         * Verifies that the mesh modifiers in the specified object's modifier stack are in the proper order.
         * The ordering is important during export because some modifiers, such as Edit Mesh, can
         * invalidate the vertex information in previous modifiers, such as the Skin modifier.
         * Without this check the exporter runs the risk of trying to access invalid data.
         */
        static bool VerifyMeshModifierStack(INode* node);

        /** Determines if the specified object is a bone in a Biped. */
        static bool IsBipedBone(INode* node);

        /** Determines if the specified object is a standard bone. */
        static bool IsNonBipedBone(INode* node);

        /** Determines if the specified object is a Character Animation Toolkit (CAT) bone. */
        static bool IsCATBone(INode* node);

        /** Determines if the specified object is any kind of bone. */
        static bool IsAnyBone(INode* node);

        /** 
         * Determines if the specified object is unknown.
         * An unknown object is one that has its corresponding plug missing.
         */
        static bool IsUnknownObject(INode* node);
        
        /** 
         * Finds the root biped bone referenced by the object.
         * @param bone [in] - A Biped bone.
         * @return Gets the bone that is the ancestor of all bones.
         */
        static INode* FindRootmostBipedBone(INode* bone);

        /** 
         * Finds the biped root bone that is attached to the node, if any. This is done by
         * finding the physique or skin modifier applied to the node's object, and then searching the
         * attached bones for the root Biped.
         * @param node [in] - The node to search
         * @param usesPhysiqueModifier [out] - If the biped was attached with a physique modifier, this value will be true.
         * If the biped was attached with a skin modifier, this value will be false. If there is no biped, this
         * value is undefined
         */
        static INode* FindBipedRootBone(INode* node, bool& usesPhysiqueModifier);

        /** * Searches through the object's modifier stack to find a Physique modifier. */
        static Modifier* FindPhysiqueModifier(INode* node);

        /** * Searches through the object's modifier stack to find a Skin modifier. */
        static Modifier* FindSkinModifier(INode* node);

        /** * Searches through the object's modifier stack to find a Morpher modifier. */
        static Modifier* FindMorpherModifier(INode* node);
        
        /** 
         * Removes/enables the non-uniform scale from the specified node's biped controller, if it has one. 
         * @param node [in] - The node whose biped controller has non-uniform scale removed/enabled.
         * @param remove [in] - Indicates whether non-uniform scale is being removed or enabled.
         */
        static void RemoveBipedNonuniformScale(INode* node, bool remove = true);

        /** 
         * Gets all the nodes below the specified parent, including the parent. 
         * @param nodes [out] - The retrieved nodes.
         * @param parentNode [in] - The root node from which the node retrieval occurs.
         * If null, the root node is used (and not included in the output).
         * @param ignoreSettings [in] - Indicates whether settings objects are ignored.
         */
        static void GetAllNodes
            (
            std::unordered_set<INode*>& nodes, 
            INode* parentNode = nullptr, 
            bool ignoreSettings = true
            );    

        /** 
         * Gets the selected nodes.
         * @param nodes [out] - The currently selected nodes.
         * @return The number of selected nodes is returned.
         */
        static int GetSelectedNodes(std::unordered_set<INode*>& nodes);    

        /** 
         * Gets the node's child nodes.
         * @param node [in] - The node whose child nodes are retrieved.
         * @param childNodes [out] - The retrieved child nodes.
         */
        static void GetChildNodes(INode* node, INodeTab& childNodes);
        
        /** 
         * Gets the identifier of a parameter, within a given parameter block, based on a name. 
         * The search is not case sensitive.
         * @param paramID [out] - The found parameter identifier.
         * @param name [in] - The name of the parameter to search for.
         * @param pblock [in] - The parameter block to search.
         * @return If the parameter was found, true is returned. Otherwise, false is returned.
         */
        static bool GetParamIDByName(ParamID& paramID, const MCHAR* name, IParamBlock2* pblock);

        /** 
         * Gets the identifier and parameter block of a parameter based on a name. 
         * The search is not case sensitive.
         * @param pblock [out] - The found parameter block.
         * @param paramID [out] - The found parameter identifier.
         * @param name [in] - The name of the parameter to search for.
         * @param name [in] - The object whose parameter blocks are searched.
         * @return If the parameter was found, true is returned. Otherwise, false is returned.
         */
        static bool GetParamIDByName(IParamBlock2*& pblock, ParamID& paramID, const MCHAR* name, Animatable* a);

        static bool GetParamIDByName(IParamBlock*& pblock, int& paramID, const MCHAR* name, Animatable* a, bool nameStartsWith = false);
        static bool GetSubAnimByName(Animatable*& subAnim, const MCHAR* name, Animatable* a, bool nameStartsWith = false);

        /** Gets the matrix that places the camera at the origin, looking down the 3DS Max world Y axis. */
        static void GetCameraIdentityMatrix(Matrix3& m);

        /** Gets the matrix that places the camera at the origin, looking down the 3DS Max world Y axis. */
        static Matrix3 GetCameraIdentityMatrix();

        /**
         * Gets the dimensions of the bounding box for the specified box object.
         * @param size [out] - The dimensions of the bounding box.
         * @param node [in] - The object whose bounding box size is retrieved.
         * @param transform [in] - Indicates whether the node's scaling is used to transform the bounding
         * box before retrieving the dimensions.
         * @param time [in] - The time at which the box size is evaluated. If < 0, the current time is used.
         * @return If the specified object was a box object and its dimensions were retrieved, true is
         * returned. Otherwise, false is returned.
         */
        static bool GetBoxSize(FinjinVector3& size, INode* node, bool transform = false, TimeValue time = -1);

        /**
         * Gets the radius and segment count for the specified sphere object.
         * @param radius [out] - The radius of the sphere.
         * @param segmentCount [out] - The segment count of the sphere.
         * @param node [in] - The object whose radius and segment count are retrieved.
         * @param time [in] - The time at which the box size is evaluated. If < 0, the current time is used.
         * @return If the specified object was a sphere object and its values were retrieved, true is
         * returned. Otherwise, false is returned.
         */
        static bool GetSphereRadiusAndSegmentCount(float& radius, FinjinIVector2& segmentCount, INode* node, TimeValue time = -1);

        /**
         * Gets the radius and length for the specified cylinder object.
         * @param radius [out] - The radius of the cylinder.
         * @param length [out] - The length of the cylinder.
         * @param node [in] - The object whose radius and length are retrieved.
         * @return If the specified object was a cylinder object and its radius and length were retrieved, true is
         * returned. Otherwise, false is returned.
         */
        static bool GetCylinderRadiusAndLength(float& radius, float& length, INode* node, TimeValue time = -1);

        /**
         * Gets the radius and length for the specified capsule object.
         * @param radius [out] - The radius of the capsule.
         * @param length [out] - The length of the capsule.
         * @param node [in] - The object whose radius and length are retrieved.
         * @return If the specified object was a capsule object and its radius and length were retrieved, true is
         * returned. Otherwise, false is returned.
         */
        static bool GetCapsuleRadiusAndLength(float& radius, float& length, INode* node, TimeValue time = -1);

        /**
         * Gets the segment count for the specified plane object.
         * @param segmentCount [out] - The segment count of the plane.
         * @param node [in] - The object whose segment count is retrieved.
         * @param time [in] - The time at which the box size is evaluated. If < 0, the current time is used.
         * @return If the specified object was a plane object and its values were retrieved, true is
         * returned. Otherwise, false is returned.
         */
        static bool GetPlaneSegmentCount(FinjinIVector2& segmentCount, INode* node, TimeValue time = -1);

        /** 
         * Gets the current unit info.
         * @param name [out] - The name of the current unit.
         * @param fractional [out] - The fraction of the specified unit name. For most unit types
         * this value is 1, but for others it will be in (0, 1).
         * @param abbreviate [in] - Indicates whether the returned unit name should be abbreviated.
         */
        static void GetCurrentUnitInfo(wxString& name, float& fractional, bool abbreviate = false);

        /** Gets the scale that 3DS Max uses to convert internal units to user interface units. */
        static float GetActiveScale();

        /**
         * Gets the node that the specified object is looking at, if any.
         * @param node [in] - The node whose 'look at' is returned.
         * @param gotFromController [out] - Indicates whether the returned look at node was
         * retrieved from a controller.
         * @return The node being looked at is returned. This may be null.
         */
        static INode* GetLookAtNode(INode* node, bool* gotFromController = nullptr);

        /** 
         * Gets the direction vector the specified object uses to look at a target object.
         * @param node [in] - The node whose 'look at' direction is returned.
         * @param direction [out] - The direction vector.
         * @param converter [in] - The coordinate system converter in use.
         * @return If the look at direction was retrieved, true is returned. Otherwise,
         * false is returned.
         */
        static bool GetLookAtDirection
            (
            INode* node, 
            FinjinVector3& direction, 
            const CoordinateSystemConverter& converter
            );

        /** The maximum length of a string that can be loaded by GetString(). */
        enum {MAX_GET_STRING_LENGTH = 512};

        /**
         * Loads a string with the specified resource ID.
         * @param id [in] - The resource identifier.
         * @return A pointer to the loaded string. The string should not be
         * any longer than MAX_GET_STRING_LENGTH and should not be freed.
         * Note that the returned string points to a global buffer and should
         * copied to a new buffer if the string needs to be kept between
         * success successive calls to GetString(). If no string could 
         * be located for the specified ID, null is returned.
         */
        static TCHAR* GetString(int id);

        /**
         * Same functionality as GetString(), but uses a different buffer.
         * This is handy for situations where two resource strings need to be
         * passed into another function, such as when calling MessageBox().
         */
        static TCHAR* GetString2(int id);

        static MCHAR* GetStringM(int id);
    };

} }
