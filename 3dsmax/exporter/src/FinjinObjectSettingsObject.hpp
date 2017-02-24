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
#include "ObjectTypeDetector.hpp"
#include "UserDataParameters.hpp"
#include "ExtendedValueParameters.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {
    
    class FinjinSceneSettingsObject;
    class FinjinNodeAnimationSettingsObject;
    class FinjinMeshAnimationSettingsObject;
    class TextureCoordinateSetMappings;

    /** The Finjin object settings object. */
    class FinjinObjectSettingsObject : public Object
    {
    public:
        FinjinObjectSettingsObject();
        ~FinjinObjectSettingsObject();

        //From BaseObject
        CreateMouseCallBack* GetCreateMouseCallBack() override;

        //From Object
        ObjectState Eval(TimeValue time) override;
        void InitNodeName(MSTR& s) override;
        Interval ObjectValidity(TimeValue t) override        {return FOREVER;}
        BOOL IsRenderable() override                          {return FALSE;}
        
        Class_ID ClassID() override;
        SClass_ID SuperClassID() override;
        void GetClassName(MSTR& s) override;

        RefTargetHandle Clone(RemapDir& remap) override;
        RefResult NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate) override;

        int NumSubs() override;
        MSTR SubAnimName(int i) override;
        Animatable* SubAnim(int i) override;

        int NumRefs() override;
        RefTargetHandle GetReference(int i) override;
        void SetReference(int i, RefTargetHandle rtarg) override;

        int NumParamBlocks() { return 1; }
        IParamBlock2* GetParamBlock(int i) override { return this->pblock; }
        IParamBlock2* GetParamBlockByID(BlockID id) override { return (this->pblock->ID() == id) ? this->pblock : nullptr; }

        void DeleteThis() override { delete this; }        

        static Class_ID GetClassClassID();
        static ClassDesc* GetClassDesc();

        void CopyFrom(FinjinObjectSettingsObject* other);

        DECLARE_USER_DATA_OBJECT_METHODS

        DECLARE_EXTENDED_VALUE_OBJECT_METHODS

        wxString GetType();
        wxString GetVisibility();
        bool GetVisibilityAffectObjectOnly();
        bool GetIsPrefab();
        bool GetIsPrefabInstance();
        wxString GetPrefabName();
        double GetNodeAnimationSampleInterval();
        SampleType GetNodeAnimationSampleType();
        int GetNoteTrackNameCount();
        wxString GetNoteTrackName(int index);
        void ClearNoteTrackNames();
        wxString GetRenderQueue();
        int GetRenderPriority();
        bool GetExportMesh();
        wxString GetMeshName();
        bool GetAnimateBoundingVolumes();
        wxString GetSkeletonName();
        wxString GetMeshAnimationType();
        double GetSkeletonAnimationSampleInterval();
        SampleType GetSkeletonAnimationSampleType();
        double GetVertexAnimationSampleInterval();
        INode* GetAnimatedRoot();
        SampleType GetVertexAnimationSampleType();
        wxString GetPrimitiveType();
        bool GetExportVertexColors();
        bool GetExportTangents();
        bool GetExportBinormals();
        wxString GetItemType();
        bool GetEnableSky();
        bool GetEmbedSkeletonInMesh();
        bool GetLinkSkeletonToMesh();
        float GetPointSize();
        bool GetIgnoreChildren();
        wxString GetChildSorting();
        int GetChildOrder();
        int GetNodeAnimationCount();
        FinjinNodeAnimationSettingsObject* GetNodeAnimation(int index);
        void GetNodeAnimationNames(std::vector<wxString>& names);
        int GetMeshAnimationCount();
        FinjinMeshAnimationSettingsObject* GetMeshAnimation(int index);
        FinjinMeshAnimationSettingsObject* GetMeshAnimation(const wxString& name, int* foundAt = nullptr);
        void GetMeshAnimationNames(std::vector<wxString>& names);        
        bool GetExportFlags();
        int GetFlagCount();
        void GetFlag(int index, wxString& name);
        void GetFlagNames(std::set<wxString>& names);
        void SetExportFlags(bool value);
        void ClearFlags();
        int GetManualLODCount();
        void GetManualLOD(int index, INode*& node, float& distance);
        INode* GetManuaLODNode(int index);
        void RemoveManualLOD(int index);
        bool GetCalculateManualLodDistances();
        float GetRenderingDistance();
        bool GetCheckMeshInstance();
        bool GetUseCustomSubmeshes();
        void GetSubmeshesSettings(SubmeshesSettings& value);
        bool GetExportSkeleton();
        bool GetExportedMeshDeformedBySkeleton();
        wxString GetSkeletonReferencePoseType();
        TimeValue GetSkeletonReferencePoseTime();
        bool GetExportPoses();
        bool GetRemoveBonesWithNoVertexInfluence();
        bool GetAttachedObject();
        INode* GetExportedParent();
        bool GetUseRootAsExportedParent();
                
        void SetType(wxString value);
        void SetVisibility(wxString value);
        void SetVisibilityAffectObjectOnly(bool value);
        void SetIsPrefab(bool value);
        void SetIsPrefabInstance(bool value);
        void SetPrefabName(wxString value);
        void SetNodeAnimationSampleInterval(double value);
        void SetNodeAnimationSampleType(SampleType value);
        void AddNoteTrackName(wxString value);
        void SetRenderQueue(wxString value);
        void SetRenderPriority(int value);
        void SetExportMesh(bool value);
        void SetMeshName(wxString value);
        void SetAnimateBoundingVolumes(bool value);
        void SetSkeletonName(wxString value);
        void SetMeshAnimationType(wxString value);
        void SetSkeletonAnimationSampleInterval(double value);
        void SetSkeletonAnimationSampleType(SampleType value);
        void SetVertexAnimationSampleInterval(double value);
        void SetAnimatedRoot(INode* value);
        void SetVertexAnimationSampleType(SampleType value);
        void SetPrimitiveType(wxString value);
        void SetExportVertexColors(bool value);
        void SetExportTangents(bool value);
        void SetExportBinormals(bool value);
        void SetItemType(wxString value);
        void SetEnableSky(bool value);
        void SetEmbedSkeletonInMesh(bool value);
        void SetLinkSkeletonToMesh(bool value);
        void SetPointSize(float value);
        void SetIgnoreChildren(bool value);
        void SetChildSorting(wxString value);
        void SetChildOrder(int value);
        void AddNodeAnimation(FinjinNodeAnimationSettingsObject* value);
        void InsertNodeAnimation(FinjinNodeAnimationSettingsObject* value, int index);
        void AddMeshAnimation(FinjinMeshAnimationSettingsObject* value);
        void InsertMeshAnimation(FinjinMeshAnimationSettingsObject* value, int index);
        void AddFlag(wxString name);
        void AddManualLOD(INode* node, float distance);
        void SetManualLOD(int index, INode* node, float distance);
        void SetCalculateManualLodDistances(bool value);
        void SetRenderingDistance(float value);
        void SetCheckMeshInstance(bool value);
        void SetUseCustomSubmeshes(bool value);
        void SetSubmeshesSettings(const SubmeshesSettings& value);
        void SetExportSkeleton(bool value);
        void SetExportedMeshDeformedBySkeleton(bool value);
        void SetSkeletonReferencePoseType(wxString value);
        void SetSkeletonReferencePoseTime(TimeValue value);
        void SetExportPoses(bool value);
        void SetRemoveBonesWithNoVertexInfluence(bool value);
        void SetAttachedObject(bool value);
        void SetExportedParent(INode* value);
        void SetUseRootAsExportedParent(bool value);
                
    public:
        enum
        {
            PARAM_BLOCK_REF,
            NUM_REFS
        };

        /** Parameter block parameters */
        enum
        {
            USER_DATA_PARAM_BLOCK_IDS,

            PB_TYPE,
            PB_VISIBILITY,
            PB_TARGET_TYPE,
            PB_IS_PREFAB,
            PB_IS_PREFAB_INSTANCE,
            PB_PREFAB_NAME,
            PB_NODE_ANIMATION_SAMPLE_INTERVAL,            
            PB_NODE_ANIMATION_SAMPLE_TYPE,
            PB_NOTE_TRACK_NAMES,
            PB_RENDER_QUEUE_NAME,
            PB_RENDER_QUEUE_PRIORITY,
            
            PB_TEXTURE_COORDINATE_SET_COUNT,
            
            PB_MESH_NAME,
                        
            PB_SKELETON_NAME,
            PB_MESH_ANIMATION_TYPE,
            PB_SKELETON_ANIMATION_SAMPLE_INTERVAL,
            PB_SKELETON_ANIMATION_SAMPLE_TYPE,
            PB_VERTEX_ANIMATION_SAMPLE_INTERVAL,
            PB_VERTEX_ANIMATION_SAMPLE_TYPE,
            
            PB_PRIMITIVE_TYPE,
            PB_EXPORT_VERTEX_COLORS,
            PB_EXPORT_TANGENTS,
            PB_EXPORT_BINORMALS,
            
            PB_ITEM_TYPE,
            
            PB_ENABLE_SKY,

            PB_POINT_SIZE,

            PB_IGNORE_CHILDREN,
            PB_CHILD_SORTING,
            PB_CHILD_ORDER,

            PB_NODE_ANIMATIONS,
            PB_MESH_ANIMATIONS,

            PB_FLAG_NAMES,
            
            PB_LOD_TYPE,
            PB_LOD_NODES,
            PB_LOD_DISTANCES,

            PB_RENDERING_DISTANCE,

            PB_VISIBILITY_AFFECT_OBJECT_ONLY,

            PB_TEXTURE_COORDINATE_TYPE,
            PB_EXPORT_POSES,

            PB_EMBED_SKELETON_IN_MESH,
            PB_LINK_SKELETON_TO_MESH,
            
            PB_IS_ATTACHED_OBJECT,
            PB_ANIMATED_ROOT,
            
            PB_USE_CUSTOM_SUBMESHES,
            PB_SUBMESHES_SETTINGS,

            EXTENDED_VALUE_PARAM_BLOCK_IDS,

            PB_REMOVE_BONES_WITH_NO_VERTEX_INFLUENCE,
            PB_EXPORT_MESH_ANIMATIONS_TO_SEPARATE_FILE,
            PB_LINK_MESH_ANIMATIONS_TO_MAIN_OBJECT,
            PB_EXPORTED_PARENT,

            PB_CHECK_MESH_INSTANCE,

            PB_EXPORT_SKELETON,

            PB_CALCULATE_MANUAL_LOD_DISTANCES,
            PB_ANIMATE_BOUNDING_VOLUMES,

            PB_EXPORT_MESH,

            PB_EXPORTED_MESH_DEFORMED_BY_SKELETON,

            PB_USE_ROOT_AS_EXPORTED_PARENT,

            PB_EXPORT_FLAGS,
            
            PB_SKELETON_REFERENCE_POSE_TYPE,
            PB_SKELETON_REFERENCE_POSE_TIME
        };

        /** Reference 0 */
        IParamBlock2* pblock;

        IObjParam* ip;
    };

} }
