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
#include "FinjinObjectSettingsAccessor.hpp"
#include "FinjinObjectSettingsObject.hpp"
#include "FinjinNodeAnimationSettingsObject.hpp"
#include "FinjinMeshAnimationSettingsObject.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
FINJIN_EXPORTER_IMPLEMENT_USER_DATA_ACCESSOR_METHODS(FinjinObjectSettingsAccessor, object)

FINJIN_EXPORTER_IMPLEMENT_EXTENDED_VALUE_ACCESSOR_METHODS(FinjinObjectSettingsAccessor, object)

wxString FinjinObjectSettingsAccessor::GetType()
{
    return this->object->GetType();
}

wxString FinjinObjectSettingsAccessor::GetVisibility()
{
    return this->object->GetVisibility();
}

bool FinjinObjectSettingsAccessor::GetVisibilityAffectObjectOnly()
{
    return this->object->GetVisibilityAffectObjectOnly();
}

wxString FinjinObjectSettingsAccessor::GetRenderQueueName()
{
    return this->object->GetRenderQueue();
}

int FinjinObjectSettingsAccessor::GetRenderPriority()
{
    return this->object->GetRenderPriority();
}

float FinjinObjectSettingsAccessor::GetRenderingDistance()
{
    return this->object->GetRenderingDistance();
}

ObjectAccessor FinjinObjectSettingsAccessor::GetExportedParent()
{
    return this->object->GetExportedParent();
}

bool FinjinObjectSettingsAccessor::GetUseRootAsExportedParent()
{
    return this->object->GetUseRootAsExportedParent();
}

bool FinjinObjectSettingsAccessor::GetAttachedObject()
{
    return this->object->GetAttachedObject();
}

bool FinjinObjectSettingsAccessor::GetIgnoreChildren()
{
    return this->object->GetIgnoreChildren();
}

wxString FinjinObjectSettingsAccessor::GetChildSorting()
{
    return this->object->GetChildSorting();
}

int FinjinObjectSettingsAccessor::GetChildOrder()
{
    return this->object->GetChildOrder();
}

bool FinjinObjectSettingsAccessor::GetIsPrefab()
{
    return this->object->GetIsPrefab();
}

bool FinjinObjectSettingsAccessor::GetIsPrefabInstance()
{
    return this->object->GetIsPrefabInstance();
}

wxString FinjinObjectSettingsAccessor::GetPrefabName()
{
    return this->object->GetPrefabName();
}

int FinjinObjectSettingsAccessor::GetNoteTrackNameCount()
{
    return this->object->GetNoteTrackNameCount();
}

wxString FinjinObjectSettingsAccessor::GetNoteTrackName(int i)
{
    return this->object->GetNoteTrackName(i);
}

bool FinjinObjectSettingsAccessor::GetExportFlags()
{
    return this->object->GetExportFlags();
}

int FinjinObjectSettingsAccessor::GetFlagCount()
{
    return this->object->GetFlagCount();
}

void FinjinObjectSettingsAccessor::GetFlag(int i, wxString& name)
{
    this->object->GetFlag(i, name);
}

SampleType FinjinObjectSettingsAccessor::GetNodeAnimationSampleType()
{
    return this->object->GetNodeAnimationSampleType();
}

double FinjinObjectSettingsAccessor::GetNodeAnimationSampleInterval()
{
    return this->object->GetNodeAnimationSampleInterval();
}

int FinjinObjectSettingsAccessor::GetNodeAnimationCount()
{
    return this->object->GetNodeAnimationCount();
}

FinjinNodeAnimationSettingsAccessor FinjinObjectSettingsAccessor::GetNodeAnimation(int i)
{
    return this->object->GetNodeAnimation(i);
}

float FinjinObjectSettingsAccessor::GetPointSize()
{
    return this->object->GetPointSize();
}

bool FinjinObjectSettingsAccessor::GetEnableSky()
{
    return this->object->GetEnableSky();
}

bool FinjinObjectSettingsAccessor::GetEmbedSkeletonInMesh()
{
    return this->object->GetEmbedSkeletonInMesh();
}

bool FinjinObjectSettingsAccessor::GetLinkSkeletonToMesh()
{
    return this->object->GetLinkSkeletonToMesh();
}

wxString FinjinObjectSettingsAccessor::GetItemType()
{
    return this->object->GetItemType();
}

wxString FinjinObjectSettingsAccessor::GetMeshAnimationType()
{
    return this->object->GetMeshAnimationType();
}

bool FinjinObjectSettingsAccessor::GetExportSkeleton()
{
    return this->object->GetExportSkeleton();
}

bool FinjinObjectSettingsAccessor::GetExportedMeshDeformedBySkeleton()
{
    return this->object->GetExportedMeshDeformedBySkeleton();
}

wxString FinjinObjectSettingsAccessor::GetSkeletonReferencePoseType()
{
    return this->object->GetSkeletonReferencePoseType();
}

TimeAccessor FinjinObjectSettingsAccessor::GetSkeletonReferencePoseTime()
{
    TimeAccessor value;
    value.SetNativeTime(this->object->GetSkeletonReferencePoseTime());
    return value;
}

bool FinjinObjectSettingsAccessor::GetExportPoses()
{
    return this->object->GetExportPoses();
}

bool FinjinObjectSettingsAccessor::GetRemoveBonesWithNoVertexInfluence()
{
    return this->object->GetRemoveBonesWithNoVertexInfluence();
}

SampleType FinjinObjectSettingsAccessor::GetSkeletonAnimationSampleType()
{
    return this->object->GetSkeletonAnimationSampleType();
}

double FinjinObjectSettingsAccessor::GetSkeletonAnimationSampleInterval()
{
    return this->object->GetSkeletonAnimationSampleInterval();
}

SampleType FinjinObjectSettingsAccessor::GetVertexAnimationSampleType()
{
    return this->object->GetVertexAnimationSampleType();
}

double FinjinObjectSettingsAccessor::GetVertexAnimationSampleInterval()
{
    return this->object->GetVertexAnimationSampleInterval();
}

ObjectAccessor FinjinObjectSettingsAccessor::GetAnimatedRoot()
{
    return this->object->GetAnimatedRoot();
}

int FinjinObjectSettingsAccessor::GetMeshAnimationCount()
{
    return this->object->GetMeshAnimationCount();
}

FinjinMeshAnimationSettingsAccessor FinjinObjectSettingsAccessor::GetMeshAnimation(int i)
{
    return this->object->GetMeshAnimation(i);
}

bool FinjinObjectSettingsAccessor::GetExportMesh()
{
    return this->object->GetExportMesh();
}

wxString FinjinObjectSettingsAccessor::GetMeshName()
{
    return this->object->GetMeshName();
}

wxString FinjinObjectSettingsAccessor::GetSkeletonName()
{
    return this->object->GetSkeletonName();
}

wxString FinjinObjectSettingsAccessor::GetPrimitiveType()
{
    return this->object->GetPrimitiveType();
}

bool FinjinObjectSettingsAccessor::GetExportVertexColors()
{
    return this->object->GetExportVertexColors();
}

bool FinjinObjectSettingsAccessor::GetExportTangents()
{
    return this->object->GetExportTangents();
}

bool FinjinObjectSettingsAccessor::GetExportBinormals()
{
    return this->object->GetExportBinormals();
}

bool FinjinObjectSettingsAccessor::GetAnimateBoundingVolumes()
{
    return this->object->GetAnimateBoundingVolumes();
}

bool FinjinObjectSettingsAccessor::GetCheckMeshInstance()
{
    return this->object->GetCheckMeshInstance();
}

bool FinjinObjectSettingsAccessor::GetUseCustomSubmeshes()
{
    return this->object->GetUseCustomSubmeshes();
}

void FinjinObjectSettingsAccessor::GetSubmeshesSettings(SubmeshesSettings& value)
{
    this->object->GetSubmeshesSettings(value);
}

int FinjinObjectSettingsAccessor::GetManualLodCount()
{
    return this->object->GetManualLODCount();
}

void FinjinObjectSettingsAccessor::GetManualLod(int i, ObjectAccessor& object, float& distance)
{
    this->object->GetManualLOD(i, object.node, distance);
}

bool FinjinObjectSettingsAccessor::GetCalculateManualLodDistances()
{
    return this->object->GetCalculateManualLodDistances();
}

void FinjinObjectSettingsAccessor::SetType(wxString value)
{
    this->object->SetType(value);
}

void FinjinObjectSettingsAccessor::SetVisibility(wxString value)
{
    this->object->SetVisibility(value);
}

void FinjinObjectSettingsAccessor::SetVisibilityAffectObjectOnly(bool value)
{
    this->object->SetVisibilityAffectObjectOnly(value);
}

void FinjinObjectSettingsAccessor::SetRenderQueueName(wxString value)
{
    this->object->SetRenderQueue(value);
}

void FinjinObjectSettingsAccessor::SetRenderPriority(int value)
{
    this->object->SetRenderPriority(value);
}

void FinjinObjectSettingsAccessor::SetRenderingDistance(float value)
{
    this->object->SetRenderingDistance(value);
}

void FinjinObjectSettingsAccessor::SetExportedParent(ObjectAccessor value)
{
    this->object->SetExportedParent(value);
}

void FinjinObjectSettingsAccessor::SetUseRootAsExportedParent(bool value)
{
    this->object->SetUseRootAsExportedParent(value);
}

void FinjinObjectSettingsAccessor::SetAttachedObject(bool value)
{
    this->object->SetAttachedObject(value);
}

void FinjinObjectSettingsAccessor::SetIgnoreChildren(bool value)
{
    this->object->SetIgnoreChildren(value);
}

void FinjinObjectSettingsAccessor::SetChildSorting(wxString value)
{
    this->object->SetChildSorting(value);
}

void FinjinObjectSettingsAccessor::SetChildOrder(int value)
{
    this->object->SetChildOrder(value);
}

void FinjinObjectSettingsAccessor::SetIsPrefab(bool value)
{
    this->object->SetIsPrefab(value);
}

void FinjinObjectSettingsAccessor::SetIsPrefabInstance(bool value)
{
    this->object->SetIsPrefabInstance(value);
}

void FinjinObjectSettingsAccessor::SetPrefabName(wxString value)
{
    this->object->SetPrefabName(value);
}

void FinjinObjectSettingsAccessor::ClearNoteTrackNames()
{
    this->object->ClearNoteTrackNames();
}

void FinjinObjectSettingsAccessor::AddNoteTrackName(wxString value)
{
    this->object->AddNoteTrackName(value);
}

void FinjinObjectSettingsAccessor::SetExportFlags(bool value)
{
    this->object->SetExportFlags(value);
}

void FinjinObjectSettingsAccessor::ClearFlags()
{
    this->object->ClearFlags();
}

void FinjinObjectSettingsAccessor::AddFlag(wxString name)
{
    this->object->AddFlag(name);
}

void FinjinObjectSettingsAccessor::SetNodeAnimationSampleType(SampleType value)
{
    this->object->SetNodeAnimationSampleType(value);
}

void FinjinObjectSettingsAccessor::SetNodeAnimationSampleInterval(double value)
{
    this->object->SetNodeAnimationSampleInterval(value);
}

FinjinNodeAnimationSettingsAccessor FinjinObjectSettingsAccessor::AddNewNodeAnimation()
{
    FinjinNodeAnimationSettingsObject* animation = new FinjinNodeAnimationSettingsObject;
    this->object->AddNodeAnimation(animation);
    return animation;
}

FinjinNodeAnimationSettingsAccessor FinjinObjectSettingsAccessor::AddNodeAnimationCopy(int i)
{
    FinjinNodeAnimationSettingsAccessor sourceAccessor = GetNodeAnimation(i);
    FinjinNodeAnimationSettingsObject* animation = (FinjinNodeAnimationSettingsObject*)sourceAccessor.object->Clone(DefaultRemapDir());
    this->object->AddNodeAnimation(animation);
    return animation;
}

void FinjinObjectSettingsAccessor::RemoveNodeAnimation(FinjinNodeAnimationSettingsAccessor value)
{
    value.object->DeleteMe();
}

void FinjinObjectSettingsAccessor::SetPointSize(float value)
{
    this->object->SetPointSize(value);
}

void FinjinObjectSettingsAccessor::SetEnableSky(bool value)
{
    this->object->SetEnableSky(value);
}

void FinjinObjectSettingsAccessor::SetEmbedSkeletonInMesh(bool value)
{
    this->object->SetEmbedSkeletonInMesh(value);
}

void FinjinObjectSettingsAccessor::SetLinkSkeletonToMesh(bool value)
{
    this->object->SetLinkSkeletonToMesh(value);
}

void FinjinObjectSettingsAccessor::SetItemType(wxString value)
{
    this->object->SetItemType(value);
}

void FinjinObjectSettingsAccessor::SetMeshAnimationType(wxString value)
{
    this->object->SetMeshAnimationType(value);
}

void FinjinObjectSettingsAccessor::SetExportSkeleton(bool value)
{
    this->object->SetExportSkeleton(value);
}

void FinjinObjectSettingsAccessor::SetExportedMeshDeformedBySkeleton(bool value)
{
    this->object->SetExportedMeshDeformedBySkeleton(value);
}

void FinjinObjectSettingsAccessor::SetSkeletonReferencePoseType(wxString value)
{
    this->object->SetSkeletonReferencePoseType(value);
}

void FinjinObjectSettingsAccessor::SetSkeletonReferencePoseTime(TimeAccessor value)
{
    this->object->SetSkeletonReferencePoseTime(value.GetNativeTime());
}

void FinjinObjectSettingsAccessor::SetExportPoses(bool value)
{
    this->object->SetExportPoses(value);
}

void FinjinObjectSettingsAccessor::SetRemoveBonesWithNoVertexInfluence(bool value)
{
    this->object->SetRemoveBonesWithNoVertexInfluence(value);
}

void FinjinObjectSettingsAccessor::SetSkeletonAnimationSampleType(SampleType value)
{
    this->object->SetSkeletonAnimationSampleType(value);
}

void FinjinObjectSettingsAccessor::SetSkeletonAnimationSampleInterval(double value)
{
    this->object->SetSkeletonAnimationSampleInterval(value);
}

void FinjinObjectSettingsAccessor::SetVertexAnimationSampleType(SampleType value)
{
    this->object->SetVertexAnimationSampleType(value);
}

void FinjinObjectSettingsAccessor::SetVertexAnimationSampleInterval(double value)
{
    this->object->SetVertexAnimationSampleInterval(value);
}

void FinjinObjectSettingsAccessor::SetAnimatedRoot(ObjectAccessor value)
{
    this->object->SetAnimatedRoot(value.node);
}

FinjinMeshAnimationSettingsAccessor FinjinObjectSettingsAccessor::AddNewMeshAnimation()
{
    auto animation = new FinjinMeshAnimationSettingsObject;
    this->object->AddMeshAnimation(animation);
    return animation;
}

FinjinMeshAnimationSettingsAccessor FinjinObjectSettingsAccessor::AddMeshAnimationCopy(int i)
{
    FinjinMeshAnimationSettingsAccessor sourceAccessor = GetMeshAnimation(i);
    auto animation = (FinjinMeshAnimationSettingsObject*)sourceAccessor.object->Clone(DefaultRemapDir());
    this->object->AddMeshAnimation(animation);
    return animation;
}

void FinjinObjectSettingsAccessor::RemoveMeshAnimation(FinjinMeshAnimationSettingsAccessor value)
{
    value.object->DeleteMe();
}

void FinjinObjectSettingsAccessor::SetExportMesh(bool value)
{
    this->object->SetExportMesh(value);
}

void FinjinObjectSettingsAccessor::SetMeshName(wxString value)
{
    this->object->SetMeshName(value);
}

void FinjinObjectSettingsAccessor::SetSkeletonName(wxString value)
{
    this->object->SetSkeletonName(value);
}

void FinjinObjectSettingsAccessor::SetPrimitiveType(wxString value)
{
    this->object->SetPrimitiveType(value);
}

void FinjinObjectSettingsAccessor::SetExportVertexColors(bool value)
{
    this->object->SetExportVertexColors(value);
}

void FinjinObjectSettingsAccessor::SetExportTangents(bool value)
{
    this->object->SetExportTangents(value);
}

void FinjinObjectSettingsAccessor::SetExportBinormals(bool value)
{
    this->object->SetExportBinormals(value);
}

void FinjinObjectSettingsAccessor::SetAnimateBoundingVolumes(bool value)
{
    this->object->SetAnimateBoundingVolumes(value);
}

void FinjinObjectSettingsAccessor::SetCheckMeshInstance(bool value)
{
    this->object->SetCheckMeshInstance(value);
}

void FinjinObjectSettingsAccessor::SetUseCustomSubmeshes(bool value)
{
    this->object->SetUseCustomSubmeshes(value);
}

void FinjinObjectSettingsAccessor::SetSubmeshesSettings(const SubmeshesSettings& value)
{
    this->object->SetSubmeshesSettings(value);
}

void FinjinObjectSettingsAccessor::AddManualLod(ObjectAccessor object, float distance)
{
    this->object->AddManualLOD(object.node, distance);
}

void FinjinObjectSettingsAccessor::SetManualLod(int i, ObjectAccessor object, float distance)
{
    this->object->SetManualLOD(i, object.node, distance);
}

void FinjinObjectSettingsAccessor::RemoveManualLod(int i)
{
    this->object->RemoveManualLOD(i);
}

void FinjinObjectSettingsAccessor::SetCalculateManualLodDistances(bool value)
{
    this->object->SetCalculateManualLodDistances(value);
}
