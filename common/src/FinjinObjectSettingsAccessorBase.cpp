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


//Includes----------------------------------------------------------------------
#include "FinjinPrecompiled.hpp"
#include "FinjinObjectSettingsAccessorBase.hpp"
#include "FinjinSceneSettingsAccessorBase.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "ObjectTypeDetector.hpp"
#include "MathUtilities.hpp"
#include "ApplicationAccessor.hpp"

using namespace Finjin::Exporter;



//Implementation---------------------------------------------------------------
void FinjinObjectSettingsAccessorBase::CopyFrom(FinjinObjectSettingsAccessorBase& other)
{
    AllObjectSettings objectSettings;
    other.GetAllSettings(objectSettings);
    SetAllSettings(objectSettings);
}

wxString FinjinObjectSettingsAccessorBase::GetNodeAnimationSampleValueText()
{
    return PropertyValues::ChooseAnimationSampleValue(GetNodeAnimationSampleType(), GetNodeAnimationSampleInterval());
}

void FinjinObjectSettingsAccessorBase::SetNodeAnimationSampling(double sampling, SampleType sampleType, bool sampleTypeValid)
{
    if (!sampleTypeValid)
    {
        SetNodeAnimationSampleInterval(0);
        SetNodeAnimationSampleType(SampleType::INTERVAL);        
    }
    else if (sampleType == SampleType::INTERVAL)
    {
        SetNodeAnimationSampleInterval(sampling);
        SetNodeAnimationSampleType(SampleType::INTERVAL);        
    }
    else if (sampleType == SampleType::RATE)
    {
        double rate = sampling;
        double interval = 0;
        if (rate != 0)
            interval = 1 / rate;
        SetNodeAnimationSampleInterval(interval);
        SetNodeAnimationSampleType(SampleType::RATE);
    }
}

void FinjinObjectSettingsAccessorBase::GetNodeAnimations(std::vector<FinjinNodeAnimationSettingsAccessor>& animations)
{
    int count = GetNodeAnimationCount();
    animations.resize(count);
    for (int i = 0; i < count; i++)
        animations[i] = GetNodeAnimation(i);
}

void FinjinObjectSettingsAccessorBase::GetNodeAnimationNames(std::vector<wxString>& names)
{                
    std::vector<FinjinNodeAnimationSettingsAccessor> animations;
    GetNodeAnimations(animations);
    names.resize(animations.size());
    for (size_t i = 0; i < animations.size(); i++)
        names[i] = animations[i].GetAnimationName();
}

wxString FinjinObjectSettingsAccessorBase::GetVertexAnimationSampleValueText()
{
    return PropertyValues::ChooseAnimationSampleValue(GetVertexAnimationSampleType(), GetVertexAnimationSampleInterval());
}

void FinjinObjectSettingsAccessorBase::SetVertexAnimationSampling(double sampling, SampleType sampleType, bool sampleTypeValid)
{
    if (!sampleTypeValid)
    {
        SetVertexAnimationSampleInterval(0);
        SetVertexAnimationSampleType(SampleType::INTERVAL);
    }
    else if (sampleType == SampleType::INTERVAL)
    {
        SetVertexAnimationSampleInterval(sampling);
        SetVertexAnimationSampleType(SampleType::INTERVAL);
    }
    else if (sampleType == SampleType::RATE)
    {
        double rate = sampling;
        double interval = 0;
        if (rate != 0)
            interval = 1 / rate;
        SetVertexAnimationSampleInterval(interval);
        SetVertexAnimationSampleType(SampleType::RATE);
    }
}

wxString FinjinObjectSettingsAccessorBase::GetSkeletonAnimationSampleValueText()
{
    return PropertyValues::ChooseAnimationSampleValue(GetSkeletonAnimationSampleType(), GetSkeletonAnimationSampleInterval());
}

void FinjinObjectSettingsAccessorBase::SetSkeletonAnimationSampling(double sampling, SampleType sampleType, bool sampleTypeValid)
{
    if (!sampleTypeValid)
    {
        SetSkeletonAnimationSampleInterval(0);
        SetSkeletonAnimationSampleType(SampleType::INTERVAL);
    }
    else if (sampleType == SampleType::INTERVAL)
    {
        SetSkeletonAnimationSampleInterval(sampling);
        SetSkeletonAnimationSampleType(SampleType::INTERVAL);
    }
    else if (sampleType == SampleType::RATE)
    {
        double rate = sampling;
        double interval = 0;
        if (rate != 0)
            interval = 1 / rate;
        SetSkeletonAnimationSampleInterval(interval);
        SetSkeletonAnimationSampleType(SampleType::RATE);
    }
}

void FinjinObjectSettingsAccessorBase::GetMeshAnimations(std::vector<FinjinMeshAnimationSettingsAccessor>& animations)
{
    int count = GetMeshAnimationCount();
    animations.resize(count);
    for (int i = 0; i < count; i++)
        animations[i] = GetMeshAnimation(i);
}

void FinjinObjectSettingsAccessorBase::GetMeshAnimationNames(std::vector<wxString>& names)
{                
    std::vector<FinjinMeshAnimationSettingsAccessor> animations;
    GetMeshAnimations(animations);
    names.resize(animations.size());
    for (size_t i = 0; i < animations.size(); i++)
        names[i] = animations[i].GetAnimationName();
}

ChildSortType FinjinObjectSettingsAccessorBase::GetChildSortingValue()
{
    auto childSorting = GetChildSorting();

    if (childSorting == wxT("name"))
        return ChildSortType::BY_NAME;                            
    else if (childSorting == wxT("child-order"))
        return ChildSortType::BY_ORDER;
    else
        return ChildSortType::NONE;
}

void FinjinObjectSettingsAccessorBase::GetNoteTracks(ObjectAccessor object, std::vector<FinjinNoteTrack>& noteTracks)
{   
    //Get the note tracks
    object.GetNoteTracks(noteTracks);

    //Set the custom names
    int trackNameCount = GetNoteTrackNameCount();
    for (int trackIndex = 0; trackIndex < (int)noteTracks.size() && trackIndex < trackNameCount; trackIndex++)
        noteTracks[trackIndex].name = GetNoteTrackName(trackIndex);
}

FinjinMeshAnimationSettingsAccessor FinjinObjectSettingsAccessorBase::GetMeshAnimationByName(const wxString& name, int* foundAt)
{
    if (foundAt != nullptr)
        *foundAt = -1;

    int count = GetMeshAnimationCount();
    for (int i = 0; i < count; i++)
    {
        auto anim = GetMeshAnimation(i);
        if (anim.GetAnimationName() == name)
        {
            if (foundAt != nullptr)
                *foundAt = i;
            return anim;
        }
    }
    return FinjinMeshAnimationSettingsAccessor();
}

void FinjinObjectSettingsAccessorBase::GetMeshAnimationsByTrack(std::vector<FinjinMeshAnimationSettingsAccessor>& animations, const wxString& track)
{
    int count = GetMeshAnimationCount();
    animations.reserve(count);
    for (int i = 0; i < count; i++)
    {
        auto anim = GetMeshAnimation(i);        
        auto animTrack = anim.GetTrack();        
        if (animTrack == track)
            animations.push_back(anim);
    }
}

PrimitiveType FinjinObjectSettingsAccessorBase::GetPrimitiveTypeValue()
{
    auto primitiveTypeValue = PrimitiveType::DEFAULT;

    //Determine the primitive type to export
    auto geometryType = GetPrimitiveType();
    if (!geometryType.empty())
    {
        if (geometryType == PropertyValues::PrimitiveType::POINTS)
            primitiveTypeValue = PrimitiveType::POINTS;
        else if (geometryType == PropertyValues::PrimitiveType::LINES)
            primitiveTypeValue = PrimitiveType::LINES;
    }

    return primitiveTypeValue;
}

DetectableObjectType* FinjinObjectSettingsAccessorBase::GetDetectedType(ObjectAccessor object)
{
    ObjectTypeDetector typeDetector;
    typeDetector.Detect(object, *this);
    return typeDetector.selectedType;
}

void FinjinObjectSettingsAccessorBase::GetSkeletonReferencePose(SkeletonReferencePose& referencePose)
{
    auto type = GetSkeletonReferencePoseType();
    referencePose.type = type == wxT("time") ? SkeletonReferencePose::SPECIFIC_TIME : SkeletonReferencePose::BIND_POSE;
    referencePose.time = GetSkeletonReferencePoseTime();
}

void FinjinObjectSettingsAccessorBase::GetManualLods(ObjectAccessor object, ManualLods& manualLods)
{
    auto currentTime = GetAutoManualLodDistanceTime();
    auto objectPosition = object.GetNodeTransformation(currentTime).GetTranslation();
    auto calculateDistances = GetCalculateManualLodDistances();

    int manualLodCount = GetManualLodCount();
    manualLods.resize(manualLodCount);
    for (int i = 0; i < manualLodCount; i++)
    {
        GetManualLod(i, manualLods[i].object, manualLods[i].distance);
        if (calculateDistances && manualLods[i].object.IsValid())
            manualLods[i].distance = (manualLods[i].object.GetNodeTransformation(currentTime).GetTranslation() - objectPosition).Length();
        manualLods[i].objectSettingsIndex = i;
    }
    
    manualLods.Sort();
}

void FinjinObjectSettingsAccessorBase::GetAllSettings(AllObjectSettings& objectSettings)
{
    objectSettings.userID = GetUserID();
    objectSettings.userData = GetUserData();
    objectSettings.userDataClass = GetUserDataClass();

    objectSettings.extendedValues.clear();
    objectSettings.extendedValueIDs.clear();
    GetAllExtendedValues(objectSettings.extendedValues, &objectSettings.extendedValueIDs);
    
    objectSettings.type = GetType();
    objectSettings.visibility = GetVisibility();
    objectSettings.visibilityAffectObjectOnly = GetVisibilityAffectObjectOnly();
    objectSettings.renderQueueName = GetRenderQueueName();
    objectSettings.renderingDistance = GetRenderingDistance();
        
    objectSettings.exportedParent = GetExportedParent();
    objectSettings.useRootAsExportedParent = GetUseRootAsExportedParent();
    objectSettings.isAttachedObject = GetAttachedObject();
    objectSettings.ignoreChildren = GetIgnoreChildren();
    objectSettings.childSorting = GetChildSorting();
    objectSettings.childOrder = GetChildOrder();
    objectSettings.isPrefab = GetIsPrefab();
    objectSettings.isPrefabInstance = GetIsPrefabInstance();
    objectSettings.prefabName = GetPrefabName();

    objectSettings.noteTrackNames.resize(GetNoteTrackNameCount());
    for (int i = 0; i < GetNoteTrackNameCount(); i++)
        objectSettings.noteTrackNames[i] = GetNoteTrackName(i);

    objectSettings.enableQueryFlags = GetExportFlags();
    objectSettings.queryFlags.resize(GetFlagCount());
    for (int i = 0; i < GetFlagCount(); i++)
    {
        GetFlag(i, objectSettings.queryFlags[i].name);
        objectSettings.queryFlags[i].bit = -1;
    }
    
    objectSettings.nodeAnimationSampleType = GetNodeAnimationSampleType();
    objectSettings.nodeAnimationSampleInterval = GetNodeAnimationSampleInterval();
    
    std::vector<FinjinNodeAnimationSettingsAccessor> nodeAnimations;
    GetNodeAnimations(nodeAnimations);
    objectSettings.nodeAnimations.clear();
    objectSettings.nodeAnimations.resize(nodeAnimations.size());
    for (size_t i = 0; i < nodeAnimations.size(); i++)
    {
        objectSettings.nodeAnimations[i].animationName = nodeAnimations[i].GetAnimationName();
        objectSettings.nodeAnimations[i].timeScaleType = nodeAnimations[i].GetTimeScaleType();
        objectSettings.nodeAnimations[i].timeScale = nodeAnimations[i].GetTimeScale();
        objectSettings.nodeAnimations[i].copyFirstKeyToLast = nodeAnimations[i].GetCopyFirstKeyToLast();
        objectSettings.nodeAnimations[i].useAnimationStartTime = nodeAnimations[i].GetUseAnimationStartTime();
        objectSettings.nodeAnimations[i].embedAnimation = nodeAnimations[i].GetEmbedAnimation();
        objectSettings.nodeAnimations[i].linkToMainObject = nodeAnimations[i].GetLinkToMainObject();
        objectSettings.nodeAnimations[i].enabled = nodeAnimations[i].GetEnabled();
        objectSettings.nodeAnimations[i].looped = nodeAnimations[i].GetLooped();
        objectSettings.nodeAnimations[i].sampleInterval = nodeAnimations[i].GetSampleInterval();
        objectSettings.nodeAnimations[i].sampleType = nodeAnimations[i].GetSampleType();
        objectSettings.nodeAnimations[i].timeInterval = nodeAnimations[i].GetTimeInterval();
        objectSettings.nodeAnimations[i].translationInterpolationType = nodeAnimations[i].GetTranslationInterpolationType();
        objectSettings.nodeAnimations[i].rotationInterpolationType = nodeAnimations[i].GetRotationInterpolationType();
    }
    
    objectSettings.pointSize = GetPointSize();
    objectSettings.enableSky = GetEnableSky();
    objectSettings.embedSkeletonInMesh = GetEmbedSkeletonInMesh();
    objectSettings.linkSkeletonToMesh = GetLinkSkeletonToMesh();
    objectSettings.itemType = GetItemType();
    objectSettings.meshAnimationType = GetMeshAnimationType();
    objectSettings.exportSkeleton = GetExportSkeleton();
    objectSettings.exportedMeshDeformedBySkeleton = GetExportedMeshDeformedBySkeleton();
    objectSettings.skeletonReferencePoseType = GetSkeletonReferencePoseType();
    objectSettings.skeletonReferencePoseTime = GetSkeletonReferencePoseTime();
    objectSettings.exportPoses = GetExportPoses();
    objectSettings.removeBonesWithNoVertexInfluence = GetRemoveBonesWithNoVertexInfluence();
    objectSettings.skeletonAnimationSampleType = GetSkeletonAnimationSampleType();
    objectSettings.skeletonAnimationSampleInterval = GetSkeletonAnimationSampleInterval();
    objectSettings.vertexAnimationSampleType = GetVertexAnimationSampleType();
    objectSettings.vertexAnimationSampleInterval = GetVertexAnimationSampleInterval();    
    objectSettings.animatedRoot = GetAnimatedRoot();
    
    std::vector<FinjinMeshAnimationSettingsAccessor> meshAnimations;
    GetMeshAnimations(meshAnimations);
    objectSettings.meshAnimations.clear();
    objectSettings.meshAnimations.resize(meshAnimations.size());
    for (size_t i = 0; i < meshAnimations.size(); i++)
    {
        objectSettings.meshAnimations[i].track = meshAnimations[i].GetTrack();
        objectSettings.meshAnimations[i].animationName = meshAnimations[i].GetAnimationName();
        objectSettings.meshAnimations[i].timeScaleType = meshAnimations[i].GetTimeScaleType();
        objectSettings.meshAnimations[i].timeScale = meshAnimations[i].GetTimeScale();
        objectSettings.meshAnimations[i].copyFirstKeyToLast = meshAnimations[i].GetCopyFirstKeyToLast();
        objectSettings.meshAnimations[i].exportAsMorph = meshAnimations[i].GetExportAsMorph();
        objectSettings.meshAnimations[i].morphWholeObject = meshAnimations[i].GetMorphWholeObject();
        objectSettings.meshAnimations[i].overrideAnimatedRootStartTime = meshAnimations[i].GetOverrideAnimatedRootStartTime();
        objectSettings.meshAnimations[i].animatedRootStartTime = meshAnimations[i].GetAnimatedRootStartTime();
        objectSettings.meshAnimations[i].useAnimationStartTime = meshAnimations[i].GetUseAnimationStartTime();
        objectSettings.meshAnimations[i].looped = meshAnimations[i].GetLooped();
        objectSettings.meshAnimations[i].sampleInterval = meshAnimations[i].GetSampleInterval();
        objectSettings.meshAnimations[i].sampleType = meshAnimations[i].GetSampleType();        
        objectSettings.meshAnimations[i].timeInterval = meshAnimations[i].GetTimeInterval();
        objectSettings.meshAnimations[i].embedAnimation = meshAnimations[i].GetEmbedAnimation();
        objectSettings.meshAnimations[i].linkToMainObject = meshAnimations[i].GetLinkToMainObject();        
                 
        objectSettings.meshAnimations[i].boneTranslationMasks.resize(meshAnimations[i].GetBoneTranslationMaskCount());
        for (size_t j = 0; j < objectSettings.meshAnimations[i].boneTranslationMasks.size(); j++)
        {
            meshAnimations[i].GetBoneTranslationMask
                (
                (int)j, 
                objectSettings.meshAnimations[i].boneTranslationMasks[j].object, 
                objectSettings.meshAnimations[i].boneTranslationMasks[j].mask
                );
        }

        objectSettings.meshAnimations[i].allowExportBones.resize(meshAnimations[i].GetAllowExportBoneCount());
        for (size_t j = 0; j < objectSettings.meshAnimations[i].allowExportBones.size(); j++)
        {
            meshAnimations[i].GetAllowExportBone
                (
                (int)j, 
                objectSettings.meshAnimations[i].allowExportBones[j].object, 
                objectSettings.meshAnimations[i].allowExportBones[j].allow,
                objectSettings.meshAnimations[i].allowExportBones[j].recursive
                );
        }
    }
    
    objectSettings.exportMesh = GetExportMesh();
    objectSettings.meshName = GetMeshName();
    objectSettings.skeletonName = GetSkeletonName();
    objectSettings.primitiveType = GetPrimitiveType();
    objectSettings.exportVertexColors = GetExportVertexColors();
    objectSettings.exportTangents = GetExportTangents();
    objectSettings.exportBinormals = GetExportBinormals();
    objectSettings.animateBoundingVolumes = GetAnimateBoundingVolumes();
    objectSettings.checkMeshInstance = GetCheckMeshInstance();
    objectSettings.useCustomSubmeshes = GetUseCustomSubmeshes();    
    GetSubmeshesSettings(objectSettings.submeshesSettings);        
    
    objectSettings.manualLods.resize(GetManualLodCount());
    for (size_t i = 0; i < objectSettings.manualLods.size(); i++)
        GetManualLod((int)i, objectSettings.manualLods[i].object, objectSettings.manualLods[i].distance);
}

void FinjinObjectSettingsAccessorBase::SetAllSettings(const AllObjectSettings& objectSettings)
{
    SetUserID(objectSettings.userID);
    SetUserData(objectSettings.userData);
    SetUserDataClass(objectSettings.userDataClass);

    ClearExtendedValues();
    for (size_t i = 0; i < objectSettings.extendedValues.size(); i++)
        SetExtendedValue(objectSettings.extendedValueIDs[i], objectSettings.extendedValues[i]);

    SetType(objectSettings.type);
    SetVisibility(objectSettings.visibility);
    SetVisibilityAffectObjectOnly(objectSettings.visibilityAffectObjectOnly);
    SetRenderQueueName(objectSettings.renderQueueName);
    SetRenderingDistance(objectSettings.renderingDistance);

    SetExportedParent(objectSettings.exportedParent);
    SetUseRootAsExportedParent(objectSettings.useRootAsExportedParent);
    SetAttachedObject(objectSettings.isAttachedObject);
    SetIgnoreChildren(objectSettings.ignoreChildren);
    SetChildSorting(objectSettings.childSorting);
    SetChildOrder(objectSettings.childOrder);
    SetIsPrefab(objectSettings.isPrefab);
    SetIsPrefabInstance(objectSettings.isPrefabInstance);
    SetPrefabName(objectSettings.prefabName);

    ClearNoteTrackNames();
    for (size_t i = 0; i < objectSettings.noteTrackNames.size(); i++)
        AddNoteTrackName(objectSettings.noteTrackNames[i]);
    
    SetExportFlags(objectSettings.enableQueryFlags);
    ClearFlags();    
    for (size_t i = 0; i < objectSettings.queryFlags.size(); i++)
        AddFlag(objectSettings.queryFlags[i].name);
    
    SetNodeAnimationSampleType(objectSettings.nodeAnimationSampleType);
    SetNodeAnimationSampleInterval(objectSettings.nodeAnimationSampleInterval);
    
    std::vector<FinjinNodeAnimationSettingsAccessor> nodeAnimations;
    GetNodeAnimations(nodeAnimations);
    for (size_t i = 0; i < nodeAnimations.size(); i++)
        RemoveNodeAnimation(nodeAnimations[i]);

    for (size_t i = 0; i < objectSettings.nodeAnimations.size(); i++)
    {
        auto nodeAnimation = AddNewNodeAnimation();
        
        nodeAnimation.SetAnimationName(objectSettings.nodeAnimations[i].animationName);
        nodeAnimation.SetTimeScaleType(objectSettings.nodeAnimations[i].timeScaleType);
        nodeAnimation.SetTimeScale(objectSettings.nodeAnimations[i].timeScale);
        nodeAnimation.SetCopyFirstKeyToLast(objectSettings.nodeAnimations[i].copyFirstKeyToLast);
        nodeAnimation.SetUseAnimationStartTime(objectSettings.nodeAnimations[i].useAnimationStartTime);
        nodeAnimation.SetEmbedAnimation(objectSettings.nodeAnimations[i].embedAnimation);
        nodeAnimation.SetLinkToMainObject(objectSettings.nodeAnimations[i].linkToMainObject);
        nodeAnimation.SetEnabled(objectSettings.nodeAnimations[i].enabled);
        nodeAnimation.SetLooped(objectSettings.nodeAnimations[i].looped);
        nodeAnimation.SetSampleInterval(objectSettings.nodeAnimations[i].sampleInterval);
        nodeAnimation.SetSampleType(objectSettings.nodeAnimations[i].sampleType);
        nodeAnimation.SetTimeInterval(objectSettings.nodeAnimations[i].timeInterval);
        nodeAnimation.SetTranslationInterpolationType(objectSettings.nodeAnimations[i].translationInterpolationType);
        nodeAnimation.SetRotationInterpolationType(objectSettings.nodeAnimations[i].rotationInterpolationType);
    }

    SetPointSize(objectSettings.pointSize);
    SetEnableSky(objectSettings.enableSky);
    SetEmbedSkeletonInMesh(objectSettings.embedSkeletonInMesh);
    SetLinkSkeletonToMesh(objectSettings.linkSkeletonToMesh);
    SetItemType(objectSettings.itemType);
    SetMeshAnimationType(objectSettings.meshAnimationType);
    SetExportSkeleton(objectSettings.exportSkeleton);
    SetExportedMeshDeformedBySkeleton(objectSettings.exportedMeshDeformedBySkeleton);
    SetSkeletonReferencePoseType(objectSettings.skeletonReferencePoseType);
    SetSkeletonReferencePoseTime(objectSettings.skeletonReferencePoseTime);
    SetExportPoses(objectSettings.exportPoses);
    SetRemoveBonesWithNoVertexInfluence(objectSettings.removeBonesWithNoVertexInfluence);
    SetSkeletonAnimationSampleType(objectSettings.skeletonAnimationSampleType);
    SetSkeletonAnimationSampleInterval(objectSettings.skeletonAnimationSampleInterval);
    SetVertexAnimationSampleType(objectSettings.vertexAnimationSampleType);
    SetVertexAnimationSampleInterval(objectSettings.vertexAnimationSampleInterval);    
    SetAnimatedRoot(objectSettings.animatedRoot);
    
    std::vector<FinjinMeshAnimationSettingsAccessor> meshAnimations;
    GetMeshAnimations(meshAnimations);
    for (size_t i = 0; i < meshAnimations.size(); i++)
        RemoveMeshAnimation(meshAnimations[i]);
    for (size_t i = 0; i < objectSettings.meshAnimations.size(); i++)
    {
        auto meshAnimation = AddNewMeshAnimation();
        
        meshAnimation.SetTrack(objectSettings.meshAnimations[i].track);
        meshAnimation.SetAnimationName(objectSettings.meshAnimations[i].animationName);
        meshAnimation.SetTimeScaleType(objectSettings.meshAnimations[i].timeScaleType);
        meshAnimation.SetTimeScale(objectSettings.meshAnimations[i].timeScale);
        meshAnimation.SetCopyFirstKeyToLast(objectSettings.meshAnimations[i].copyFirstKeyToLast);
        meshAnimation.SetExportAsMorph(objectSettings.meshAnimations[i].exportAsMorph);
        meshAnimation.SetMorphWholeObject(objectSettings.meshAnimations[i].morphWholeObject);
        meshAnimation.SetOverrideAnimatedRootStartTime(objectSettings.meshAnimations[i].overrideAnimatedRootStartTime);
        meshAnimation.SetAnimatedRootStartTime(objectSettings.meshAnimations[i].animatedRootStartTime);
        meshAnimation.SetUseAnimationStartTime(objectSettings.meshAnimations[i].useAnimationStartTime);
        meshAnimation.SetLooped(objectSettings.meshAnimations[i].looped);
        meshAnimation.SetSampleInterval(objectSettings.meshAnimations[i].sampleInterval);
        meshAnimation.SetSampleType(objectSettings.meshAnimations[i].sampleType);
        meshAnimation.SetTimeInterval(objectSettings.meshAnimations[i].timeInterval);
        meshAnimation.SetEmbedAnimation(objectSettings.meshAnimations[i].embedAnimation);
        meshAnimation.SetLinkToMainObject(objectSettings.meshAnimations[i].linkToMainObject);        
                
        for (size_t j = 0; j < objectSettings.meshAnimations[i].boneTranslationMasks.size(); j++)
        {
            meshAnimation.AddBoneTranslationMask
                (
                objectSettings.meshAnimations[i].boneTranslationMasks[j].object, 
                objectSettings.meshAnimations[i].boneTranslationMasks[j].mask
                );
        }

        for (size_t j = 0; j < objectSettings.meshAnimations[i].allowExportBones.size(); j++)
        {
            meshAnimation.AddAllowExportBone
                (
                objectSettings.meshAnimations[i].allowExportBones[j].object, 
                objectSettings.meshAnimations[i].allowExportBones[j].allow,
                objectSettings.meshAnimations[i].allowExportBones[j].recursive
                );
        }
    }
    
    SetExportMesh(objectSettings.exportMesh);
    SetMeshName(objectSettings.meshName);
    SetSkeletonName(objectSettings.skeletonName);
    SetPrimitiveType(objectSettings.primitiveType);
    SetExportVertexColors(objectSettings.exportVertexColors);
    SetExportTangents(objectSettings.exportTangents);
    SetExportBinormals(objectSettings.exportBinormals);
    SetAnimateBoundingVolumes(objectSettings.animateBoundingVolumes);
    SetCheckMeshInstance(objectSettings.checkMeshInstance);
    SetUseCustomSubmeshes(objectSettings.useCustomSubmeshes);    
    SetSubmeshesSettings(objectSettings.submeshesSettings);    
    
    int manualLodCount = GetManualLodCount();
    for (int i = 0; i < manualLodCount; i++)
        RemoveManualLod(manualLodCount - i - 1);
    for (size_t i = 0; i < objectSettings.manualLods.size(); i++)
        AddManualLod(objectSettings.manualLods[i].object, objectSettings.manualLods[i].distance);

    SetCalculateManualLodDistances(GetCalculateManualLodDistances());
}

TimeAccessor FinjinObjectSettingsAccessorBase::GetAutoManualLodDistanceTime()
{
    return ApplicationAccessor::GetAnimationRange().start;
}
