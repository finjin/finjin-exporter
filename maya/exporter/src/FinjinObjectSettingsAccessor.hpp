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
#include "FinjinObjectSettingsAccessorBase.hpp"
#include "UserDataSettingsAccessor.hpp"
#include "UserDataAttributes.hpp"
#include "ExtendedValueAttributes.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Access to a Finjin object settings object. */
    class FinjinObjectSettingsAccessor : public FinjinObjectSettingsAccessorBase
    {
    public:
        FinjinObjectSettingsAccessor() {}
        FinjinObjectSettingsAccessor(MPlug rootPlug) {this->rootPlug = rootPlug;}
        
        bool IsValid() const {return !this->rootPlug.isNull();}

        FINJIN_EXPORTER_DECLARE_EXTENDED_VALUE_ACCESSOR_METHODS_OVERRIDE

        FINJIN_EXPORTER_DECLARE_USER_DATA_ACCESSOR_METHODS_OVERRIDE

        wxString GetType() override;
        wxString GetVisibility() override;
        bool GetVisibilityAffectObjectOnly() override;
        wxString GetRenderQueueName() override;
        int GetRenderPriority() override;
        float GetRenderingDistance() override;
        ObjectAccessor GetExportedParent() override;
        bool GetUseRootAsExportedParent() override;
        bool GetAttachedObject() override;
        bool GetIgnoreChildren() override;
        wxString GetChildSorting() override;
        int GetChildOrder() override;
        bool GetIsPrefab() override;
        bool GetIsPrefabInstance() override;
        wxString GetPrefabName() override;
        int GetNoteTrackNameCount() override;
        wxString GetNoteTrackName(int i) override;
        bool GetExportFlags() override;
        int GetFlagCount() override;
        void GetFlag(int i, wxString& name) override;
        SampleType GetNodeAnimationSampleType() override;
        double GetNodeAnimationSampleInterval() override;
        int GetNodeAnimationCount() override;
        FinjinNodeAnimationSettingsAccessor GetNodeAnimation(int i) override;
        float GetPointSize() override;
        bool GetEnableSky() override;
        bool GetEmbedSkeletonInMesh() override;
        bool GetLinkSkeletonToMesh() override;
        wxString GetItemType() override;
        wxString GetMeshAnimationType() override;
        bool GetExportSkeleton() override;
        bool GetExportedMeshDeformedBySkeleton() override;
        wxString GetSkeletonReferencePoseType() override;
        TimeAccessor GetSkeletonReferencePoseTime() override;
        bool GetExportPoses() override;
        bool GetRemoveBonesWithNoVertexInfluence() override;
        SampleType GetSkeletonAnimationSampleType() override;
        double GetSkeletonAnimationSampleInterval() override;
        SampleType GetVertexAnimationSampleType() override;
        double GetVertexAnimationSampleInterval() override;
        ObjectAccessor GetAnimatedRoot() override;
        int GetMeshAnimationCount() override;
        FinjinMeshAnimationSettingsAccessor GetMeshAnimation(int i) override;
        bool GetExportMesh() override;
        wxString GetMeshName() override;
        wxString GetSkeletonName() override;
        wxString GetPrimitiveType() override;
        bool GetExportVertexColors() override;
        bool GetExportTangents() override;
        bool GetExportBinormals() override;
        bool GetAnimateBoundingVolumes() override;
        bool GetCheckMeshInstance() override;
        bool GetUseCustomSubmeshes() override;
        void GetSubmeshesSettings(SubmeshesSettings& value) override;
        int GetManualLodCount() override;
        void GetManualLod(int i, ObjectAccessor& object, float& distance) override;        
        bool GetCalculateManualLodDistances() override;

        void SetType(wxString value) override;
        void SetVisibility(wxString value) override;
        void SetVisibilityAffectObjectOnly(bool value) override;
        void SetRenderQueueName(wxString value) override;
        void SetRenderPriority(int value) override;
        void SetRenderingDistance(float value) override;
        void SetExportedParent(ObjectAccessor value) override;
        void SetUseRootAsExportedParent(bool value) override;
        void SetAttachedObject(bool value) override;
        void SetIgnoreChildren(bool value) override;
        void SetChildSorting(wxString value) override;
        void SetChildOrder(int value) override;
        void SetIsPrefab(bool value) override;
        void SetIsPrefabInstance(bool value) override;
        void SetPrefabName(wxString value) override;
        void ClearNoteTrackNames() override;
        void AddNoteTrackName(wxString value) override;
        void SetExportFlags(bool value) override;
        void ClearFlags() override;
        void AddFlag(wxString name) override;
        void SetNodeAnimationSampleType(SampleType value) override;
        void SetNodeAnimationSampleInterval(double value) override;        
        FinjinNodeAnimationSettingsAccessor AddNewNodeAnimation() override;
        FinjinNodeAnimationSettingsAccessor AddNodeAnimationCopy(int i) override;
        void RemoveNodeAnimation(FinjinNodeAnimationSettingsAccessor value) override;
        void SetPointSize(float value) override;
        void SetEnableSky(bool value) override;
        void SetEmbedSkeletonInMesh(bool value) override;
        void SetLinkSkeletonToMesh(bool value) override;
        void SetItemType(wxString value) override;
        void SetMeshAnimationType(wxString value) override;
        void SetExportSkeleton(bool value) override;
        void SetExportedMeshDeformedBySkeleton(bool value) override;
        void SetSkeletonReferencePoseType(wxString value) override;
        void SetSkeletonReferencePoseTime(TimeAccessor value) override;
        void SetExportPoses(bool value) override;
        void SetRemoveBonesWithNoVertexInfluence(bool value) override;
        void SetSkeletonAnimationSampleType(SampleType value) override;
        void SetSkeletonAnimationSampleInterval(double value) override;
        void SetVertexAnimationSampleType(SampleType value) override;
        void SetVertexAnimationSampleInterval(double value) override;
        void SetAnimatedRoot(ObjectAccessor value) override;
        FinjinMeshAnimationSettingsAccessor AddNewMeshAnimation() override;
        FinjinMeshAnimationSettingsAccessor AddMeshAnimationCopy(int i) override;
        void RemoveMeshAnimation(FinjinMeshAnimationSettingsAccessor value) override;
        void SetExportMesh(bool value) override;
        void SetMeshName(wxString value) override;
        void SetSkeletonName(wxString value) override;
        void SetPrimitiveType(wxString value) override;
        void SetExportVertexColors(bool value) override;
        void SetExportTangents(bool value) override;
        void SetExportBinormals(bool value) override;
        void SetAnimateBoundingVolumes(bool value) override;
        void SetCheckMeshInstance(bool value) override;
        void SetUseCustomSubmeshes(bool value) override;
        void SetSubmeshesSettings(const SubmeshesSettings& value) override;
        void AddManualLod(ObjectAccessor object, float distance) override;
        void SetManualLod(int i, ObjectAccessor object, float distance) override;
        void RemoveManualLod(int i) override;
        void SetCalculateManualLodDistances(bool value) override;

        static MObject CreateObjectSettingsArrayAttribute(NodeAttributeAdder& adder, const MString& name);
        static MPlug GetObjectSettings(ObjectAccessor object);

        void Upgrade();
        
    private:
        MPlug Child(MObject attribute, const MString& name);
        int ChildInt(MObject attribute, const MString& name, int defaultValue = 0);
        bool ChildBool(MObject attribute, const MString& name, bool defaultValue = false);
        float ChildFloat(MObject attribute, const MString& name, float defaultValue = 0);
        double ChildDouble(MObject attribute, const MString& name, double defaultValue = 0);
        MString ChildString(MObject attribute, const MString& name);
        int ChildNumElements(MObject attribute, const MString& name);

    public:
        MPlug rootPlug;

        struct ObjectSettingsAttributes : public UserDataAttributes, public ExtendedValueAttributes
        {
            MObject object;
            MObject version;

            MObject type;
            MObject visibility;
            MObject visibilityAffectObjectOnly;
            MObject targetType;
            MObject isPrefab;
            MObject isPrefabInstance;
            MObject prefabName;
            MObject nodeAnimationSampleInterval;
            MObject nodeAnimationSampleType;

            MObject noteTrackNames;
            MObject renderQueueName;
            MObject renderQueuePriority;
            MObject exportFlags;
            MObject flagNames;
                        
            MObject exportMesh;
            MObject meshName;
            MObject animateBoundingVolumes;
            MObject pointSize;
            MObject checkMeshInstance;
            MObject exportedMeshDeformedBySkeleton;
            MObject exportSkeleton;
            MObject skeletonReferencePoseType;
            MObject skeletonReferencePoseTime;
            MObject exportPoses;
            MObject removeBonesWithNoVertexInfluence;
            
            MObject skeletonName;
            MObject meshAnimationType;
            MObject skeletonAnimationSampleInterval;
            MObject skeletonAnimationSampleType;
            MObject vertexAnimationSampleInterval;
            MObject vertexAnimationSampleType;
            MObject animatedSkeletonRoot;
            MObject exportMeshAnimationsToSeparateFile;
            MObject linkMeshAnimationsToMainObject;
            
            MObject primitiveType;
            MObject exportVertexColors;
            MObject exportTangents;
            MObject exportBinormals;
            MObject useCustomSubmeshes;
            MObject submeshes;

            MObject lodObjectNames;
            MObject lodObjectReferences;
            MObject lodDistances;
            MObject calculateManualLodDistances;

            MObject itemType;
            
            MObject enableSky;
            
            MObject scale;
            
            MObject embedSkeletonInMesh;
            MObject linkSkeletonToMesh;
            
            MObject exportedParent;
            MObject useRootAsExportedParent;
            MObject isAttachedObject;
            MObject ignoreChildren;
            MObject childSorting;
            MObject childOrder;

            MObject renderingDistance;
            
            MObject nodeAnimations;
            MObject meshAnimations;

            struct SubmeshesAttributes
            {    
                MObject textureCoordinateSetSubmeshName;
                MObject textureCoordinateSetMaterial;
                MObject textureCoordinateSetMaterialObject;
                MObject textureCoordinateSetSources;
                MObject textureCoordinateSetDestinations;
                MObject textureCoordinateSetTypes;
                MObject textureAliases;
                MObject textureNames;
                MObject renderQueueName;
                MObject renderQueuePriority;
            };
            SubmeshesAttributes submeshesAttributes;
        };
        static ObjectSettingsAttributes attributes;
    };

} }
