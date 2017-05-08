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
#include "FinjinNodeAnimationSettingsAccessor.hpp"
#include "FinjinMeshAnimationSettingsAccessor.hpp"
#include "FinjinNoteTrack.hpp"
#include "TextureCoordinateSetMappings.hpp"
#include "ManualLod.hpp"
#include "UserDataSettingsAccessor.hpp"
#include "ExtendedValueAccessor.hpp"
#include "AllowExportBone.hpp"
#include "SkeletonReferencePose.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class DetectableObjectType;
    class FinjinSceneSettingsAccessorBase;

    struct AllObjectSettings : public UserDataSettingsAccessor
    {
        AllObjectSettings()
        {
            this->visibilityAffectObjectOnly = false;
            this->renderingDistance = 0;
            this->useRootAsExportedParent = false;
            this->isAttachedObject = false;
            this->ignoreChildren = false;
            this->childOrder = 0;
            this->isPrefab = false;
            this->isPrefabInstance = false;
            this->enableQueryFlags = false;
            this->nodeAnimationSampleType = SampleType::USE_PARENT;
            this->nodeAnimationSampleInterval = 0;
            this->pointSize = 10.0;
            this->enableSky = true;
            this->embedSkeletonInMesh = true;
            this->linkSkeletonToMesh = true;
            this->exportMesh = true;
            this->exportedMeshDeformedBySkeleton = true;
            this->exportPoses = true;
            this->removeBonesWithNoVertexInfluence = false;
            this->skeletonAnimationSampleType = SampleType::USE_PARENT;
            this->skeletonAnimationSampleInterval = 0;
            this->vertexAnimationSampleType = SampleType::USE_PARENT;
            this->vertexAnimationSampleInterval = 0;
            this->exportVertexColors = false;
            this->exportTangents = false;
            this->exportBinormals = false;
            this->animateBoundingVolumes = false;
            this->checkMeshInstance = false;
            this->useCustomSubmeshes = false;
        }

        struct NodeAnimation
        {
            NodeAnimation()
            {
                this->timeScale = 0;
                this->copyFirstKeyToLast = false;
                this->useAnimationStartTime = false;
                this->embedAnimation = true;
                this->linkToMainObject = true;
                this->enabled = false;
                this->looped = false;
                this->sampleInterval = 0;
                this->sampleType = SampleType::USE_PARENT;
            }

            wxString animationName;
            wxString timeScaleType;
            double timeScale;
            bool copyFirstKeyToLast;
            bool useAnimationStartTime;
            bool embedAnimation;
            bool linkToMainObject;
            bool enabled;
            bool looped;
            double sampleInterval;
            SampleType sampleType;
            FinjinTimeInterval timeInterval;
            wxString translationInterpolationType;
            wxString rotationInterpolationType;
        };

        struct MeshAnimation
        {
            wxString track;
            wxString animationName;
            wxString timeScaleType;
            double timeScale;
            bool copyFirstKeyToLast;
            bool exportAsMorph;
            InheritedBool morphWholeObject;
            bool useAnimationStartTime;
            bool looped;
            double sampleInterval;
            SampleType sampleType;
            FinjinTimeInterval timeInterval;
            bool embedAnimation;
            bool linkToMainObject;
            bool overrideAnimatedRootStartTime;
            TimeAccessor animatedRootStartTime;
            std::vector<AllowExportBone> allowExportBones;
            std::vector<BoneTranslationMask> boneTranslationMasks;
        };

        wxString GetUserID() {return this->userID;}
        wxString GetUserData() {return this->userData;}
        wxString GetUserDataClass() {return this->userDataClass;}

        void SetUserID(wxString value) {this->userID = value;}
        void SetUserData(wxString value) {this->userData = value;}
        void SetUserDataClass(wxString value) {this->userDataClass = value;}

        wxString userID;
        wxString userData;
        wxString userDataClass;

        std::vector<ExtendedValue> extendedValues;
        std::vector<int> extendedValueIDs;

        wxString type;
        wxString visibility;
        bool visibilityAffectObjectOnly;
        wxString renderQueueName;
        float renderingDistance;
        ObjectAccessor exportedParent;
        bool useRootAsExportedParent;
        bool isAttachedObject;
        bool ignoreChildren;
        wxString childSorting;
        int childOrder;
        bool isPrefab;
        bool isPrefabInstance;
        wxString prefabName;
        std::vector<wxString> noteTrackNames;
        bool enableQueryFlags;
        std::vector<NamedFlag> queryFlags;
        SampleType nodeAnimationSampleType;
        double nodeAnimationSampleInterval;
        std::vector<NodeAnimation> nodeAnimations;
        double pointSize;
        bool enableSky;
        bool embedSkeletonInMesh;
        bool linkSkeletonToMesh;
        wxString itemType;
        wxString meshAnimationType;
        bool exportSkeleton;
        bool exportedMeshDeformedBySkeleton;
        wxString skeletonReferencePoseType;
        TimeAccessor skeletonReferencePoseTime;
        bool exportPoses;
        bool removeBonesWithNoVertexInfluence;
        SampleType skeletonAnimationSampleType;
        double skeletonAnimationSampleInterval;
        SampleType vertexAnimationSampleType;
        double vertexAnimationSampleInterval;
        ObjectAccessor animatedRoot;
        std::vector<MeshAnimation> meshAnimations;
        bool exportMesh;
        wxString meshName;
        wxString skeletonName;
        wxString primitiveType;
        bool exportVertexColors;
        bool exportTangents;
        bool exportBinormals;
        bool animateBoundingVolumes;
        bool checkMeshInstance;
        bool useCustomSubmeshes;
        SubmeshesSettings submeshesSettings;
        std::vector<ManualLod> manualLods;
    };

    /**
     * FinjinObjectSettingsAccessorBase and the application-specific FinjinObjectSettingsAccessor
     * classes provide generic access to an Finjin object settings object.
     */
    class FinjinObjectSettingsAccessorBase :
        public UserDataSettingsAccessor,
        public ExtendedValueAccessor
    {
    public:
        virtual ~FinjinObjectSettingsAccessorBase() {}

        virtual wxString GetType() = 0;
        virtual wxString GetVisibility() = 0;
        virtual bool GetVisibilityAffectObjectOnly() = 0;
        virtual wxString GetRenderQueueName() = 0;
        virtual int GetRenderPriority() = 0;
        virtual float GetRenderingDistance() = 0;
        virtual ObjectAccessor GetExportedParent() = 0;
        virtual bool GetUseRootAsExportedParent() = 0;
        virtual bool GetAttachedObject() = 0;
        virtual bool GetIgnoreChildren() = 0;
        virtual wxString GetChildSorting() = 0;
        virtual int GetChildOrder() = 0;
        virtual bool GetIsPrefab() = 0;
        virtual bool GetIsPrefabInstance() = 0;
        virtual wxString GetPrefabName() = 0;
        virtual int GetNoteTrackNameCount() = 0;
        virtual wxString GetNoteTrackName(int i) = 0;
        virtual bool GetExportFlags() = 0;
        virtual int GetFlagCount() = 0;
        virtual void GetFlag(int i, wxString& name) = 0;
        virtual SampleType GetNodeAnimationSampleType() = 0;
        virtual double GetNodeAnimationSampleInterval() = 0;
        virtual int GetNodeAnimationCount() = 0;
        virtual FinjinNodeAnimationSettingsAccessor GetNodeAnimation(int i) = 0;
        virtual float GetPointSize() = 0;
        virtual bool GetEnableSky() = 0;
        virtual bool GetEmbedSkeletonInMesh() = 0;
        virtual bool GetLinkSkeletonToMesh() = 0;
        virtual wxString GetItemType() = 0;
        virtual wxString GetMeshAnimationType() = 0;
        virtual bool GetExportSkeleton() = 0;
        virtual bool GetExportedMeshDeformedBySkeleton() = 0;
        virtual wxString GetSkeletonReferencePoseType() = 0;
        virtual TimeAccessor GetSkeletonReferencePoseTime() = 0;
        virtual bool GetExportPoses() = 0;
        virtual bool GetRemoveBonesWithNoVertexInfluence() = 0;
        virtual SampleType GetSkeletonAnimationSampleType() = 0;
        virtual double GetSkeletonAnimationSampleInterval() = 0;
        virtual SampleType GetVertexAnimationSampleType() = 0;
        virtual double GetVertexAnimationSampleInterval() = 0;
        virtual ObjectAccessor GetAnimatedRoot() = 0;
        virtual int GetMeshAnimationCount() = 0;
        virtual FinjinMeshAnimationSettingsAccessor GetMeshAnimation(int i) = 0;
        virtual bool GetExportMesh() = 0;
        virtual wxString GetMeshName() = 0;
        virtual wxString GetSkeletonName() = 0;
        virtual wxString GetPrimitiveType() = 0;
        virtual bool GetExportVertexColors() = 0;
        virtual bool GetExportTangents() = 0;
        virtual bool GetExportBinormals() = 0;
        virtual bool GetAnimateBoundingVolumes() = 0;
        virtual bool GetCheckMeshInstance() = 0;
        virtual bool GetUseCustomSubmeshes() = 0;
        virtual void GetSubmeshesSettings(SubmeshesSettings& value) = 0;
        virtual int GetManualLodCount() = 0;
        virtual void GetManualLod(int i, ObjectAccessor& object, float& distance) = 0;
        virtual bool GetCalculateManualLodDistances() = 0;

        virtual void SetType(wxString value) = 0;
        virtual void SetVisibility(wxString value) = 0;
        virtual void SetVisibilityAffectObjectOnly(bool value) = 0;
        virtual void SetRenderQueueName(wxString value) = 0;
        virtual void SetRenderPriority(int value) = 0;
        virtual void SetRenderingDistance(float value) = 0;
        virtual void SetExportedParent(ObjectAccessor value) = 0;
        virtual void SetUseRootAsExportedParent(bool value) = 0;
        virtual void SetAttachedObject(bool value) = 0;
        virtual void SetIgnoreChildren(bool value) = 0;
        virtual void SetChildSorting(wxString value) = 0;
        virtual void SetChildOrder(int value) = 0;
        virtual void SetIsPrefab(bool value) = 0;
        virtual void SetIsPrefabInstance(bool value) = 0;
        virtual void SetPrefabName(wxString value) = 0;
        virtual void ClearNoteTrackNames() = 0;
        virtual void AddNoteTrackName(wxString value) = 0;
        virtual void SetExportFlags(bool value) = 0;
        virtual void ClearFlags() = 0;
        virtual void AddFlag(wxString name) = 0;
        virtual void SetNodeAnimationSampleType(SampleType value) = 0;
        virtual void SetNodeAnimationSampleInterval(double value) = 0;
        virtual FinjinNodeAnimationSettingsAccessor AddNewNodeAnimation() = 0;
        virtual FinjinNodeAnimationSettingsAccessor AddNodeAnimationCopy(int i) = 0;
        virtual void RemoveNodeAnimation(FinjinNodeAnimationSettingsAccessor value) = 0;
        virtual void SetPointSize(float value) = 0;
        virtual void SetEnableSky(bool value) = 0;
        virtual void SetEmbedSkeletonInMesh(bool value) = 0;
        virtual void SetLinkSkeletonToMesh(bool value) = 0;
        virtual void SetItemType(wxString value) = 0;
        virtual void SetMeshAnimationType(wxString value) = 0;
        virtual void SetExportSkeleton(bool value) = 0;
        virtual void SetExportedMeshDeformedBySkeleton(bool value) = 0;
        virtual void SetSkeletonReferencePoseType(wxString value) = 0;
        virtual void SetSkeletonReferencePoseTime(TimeAccessor value) = 0;
        virtual void SetExportPoses(bool value) = 0;
        virtual void SetRemoveBonesWithNoVertexInfluence(bool value) = 0;
        virtual void SetSkeletonAnimationSampleType(SampleType value) = 0;
        virtual void SetSkeletonAnimationSampleInterval(double value) = 0;
        virtual void SetVertexAnimationSampleType(SampleType value) = 0;
        virtual void SetVertexAnimationSampleInterval(double value) = 0;
        virtual void SetAnimatedRoot(ObjectAccessor value) = 0;
        virtual FinjinMeshAnimationSettingsAccessor AddNewMeshAnimation() = 0;
        virtual FinjinMeshAnimationSettingsAccessor AddMeshAnimationCopy(int i) = 0;
        virtual void RemoveMeshAnimation(FinjinMeshAnimationSettingsAccessor value) = 0;
        virtual void SetExportMesh(bool value) = 0;
        virtual void SetMeshName(wxString value) = 0;
        virtual void SetSkeletonName(wxString value) = 0;
        virtual void SetPrimitiveType(wxString value) = 0;
        virtual void SetExportVertexColors(bool value) = 0;
        virtual void SetExportTangents(bool value) = 0;
        virtual void SetExportBinormals(bool value) = 0;
        virtual void SetAnimateBoundingVolumes(bool value) = 0;
        virtual void SetCheckMeshInstance(bool value) = 0;
        virtual void SetUseCustomSubmeshes(bool value) = 0;
        virtual void SetSubmeshesSettings(const SubmeshesSettings& value) = 0;
        virtual void AddManualLod(ObjectAccessor object, float distance) = 0;
        virtual void SetManualLod(int i, ObjectAccessor object, float distance) = 0;
        virtual void RemoveManualLod(int i) = 0;
        virtual void SetCalculateManualLodDistances(bool value) = 0;

        virtual void CopyFrom(FinjinObjectSettingsAccessorBase& other);

        virtual void TouchReferences() {}

        virtual wxString GetNodeAnimationSampleValueText();
        virtual void SetNodeAnimationSampling(double sampling, SampleType sampleType, bool sampleTypeValid);
        virtual void GetNodeAnimations(std::vector<FinjinNodeAnimationSettingsAccessor>& animations);
        virtual void GetNodeAnimationNames(std::vector<wxString>& names);

        virtual wxString GetVertexAnimationSampleValueText();
        virtual void SetVertexAnimationSampling(double sampling, SampleType sampleType, bool sampleTypeValid);
        virtual wxString GetSkeletonAnimationSampleValueText();
        virtual void SetSkeletonAnimationSampling(double sampling, SampleType sampleType, bool sampleTypeValid);

        virtual void GetMeshAnimations(std::vector<FinjinMeshAnimationSettingsAccessor>& animations);
        virtual void GetMeshAnimationNames(std::vector<wxString>& names);

        ChildSortType GetChildSortingValue();
        void GetNoteTracks(ObjectAccessor object, std::vector<FinjinNoteTrack>& noteTracks);
        FinjinMeshAnimationSettingsAccessor GetMeshAnimationByName(const wxString& name, int* foundAt = nullptr);
        void GetMeshAnimationsByTrack(std::vector<FinjinMeshAnimationSettingsAccessor>& animations, const wxString& track);
        PrimitiveType GetPrimitiveTypeValue();
        DetectableObjectType* GetDetectedType(ObjectAccessor object);

        void GetSkeletonReferencePose(SkeletonReferencePose& referencePose);
        void GetManualLods(ObjectAccessor object, ManualLods& manualLods);

        void GetAllSettings(AllObjectSettings& objectSettings);
        void SetAllSettings(const AllObjectSettings& objectSettings);

        static TimeAccessor GetAutoManualLodDistanceTime();
    };

} }
