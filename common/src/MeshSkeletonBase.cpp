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
#include "MeshSkeletonBase.hpp"
#include "GeometryStateBase.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
MeshSkeletonBase::MeshSkeletonBase(GeometryStateBase* geometryState)
{
    this->geometryState = geometryState;
}

MeshSkeletonBase::~MeshSkeletonBase()
{
    Clear();
}

void MeshSkeletonBase::Clear()
{
    this->referencePose.Clear();

    this->bones.clear();

    this->auxiliaryBoneObjects.clear();

    this->weightedVertices.clear();    
}

void MeshSkeletonBase::UpdateHierarchy()
{
    auto sceneSettings = FinjinSceneSettingsAccessor::GetSceneSettings(false);
    if (sceneSettings.IsValid())
    {
        for (auto& bone : this->bones)
        {
            auto boneObjectSettings = sceneSettings.GetObjectSettings(bone->object, false);
            if (boneObjectSettings.IsValid())
            {
                if (!sceneSettings.GetExportHierarchy() || boneObjectSettings.GetUseRootAsExportedParent())
                {
                    bone->parent = nullptr;
                    bone->SetInitialState(this->initialSkinTransform);
                }
                else
                {
                    auto exportedParentObject = boneObjectSettings.GetExportedParent();
                    if (exportedParentObject.IsValid())
                    {
                        auto exportedParentBone = GetBone(exportedParentObject);
                        if (exportedParentBone != nullptr && 
                            exportedParentBone->object != bone->object && 
                            !bone->object.HasDescendant(exportedParentBone->object))
                        {
                            bone->parent = exportedParentBone;
                            bone->SetInitialState(this->initialSkinTransform);
                        }
                    }
                }
            }
            else if (!sceneSettings.GetExportHierarchy())
            {
                bone->parent = nullptr;
                bone->SetInitialState(this->initialSkinTransform);
            }
        }
    }
}

void MeshSkeletonBase::GetBones(ObjectAccessorSet& boneObjects)
{
    for (auto& bone : this->bones)
        boneObjects.insert(bone->object);
}

MeshBonePtr MeshSkeletonBase::FindBone(ObjectAccessor object)
{
    for (auto& bone : this->bones)
    {
        if (object == bone->object)
            return bone;
    }    
    return nullptr;
}

MeshBonePtr MeshSkeletonBase::FindBone(MeshBoneBase* meshBone)
{
    for (auto& bone : this->bones)
    {
        if (meshBone == bone.get())
            return bone;
    }    
    return nullptr;
}

MeshBone* MeshSkeletonBase::GetBone(ObjectAccessor object)
{
    for (auto& bone : this->bones)
    {
        if (object == bone->object)
            return bone.get();
    }    
    return nullptr;
}

int MeshSkeletonBase::GetBoneIndex(MeshBoneBase* meshBone) const
{
    int index = 0;
    for (auto& bone : this->bones)
    {
        if (meshBone == bone.get())
            return index;
        index++;
    }    
    return -1;
}

int MeshSkeletonBase::GetChildBoneCount(MeshBoneBase* meshBone) const
{
    int count = 0;
    for (auto& bone : this->bones)
    {
        if (meshBone == bone->parent)
            count++;
    }
    return count;
}

int MeshSkeletonBase::GetBoneIndex(ObjectAccessor object) const
{
    int index = 0;
    for (auto& bone : this->bones)
    {
        if (object == bone->object)
            return index;
        index++;
    }    
    return -1;
}

bool MeshSkeletonBase::HasBone(ObjectAccessor object) const
{
    for (auto& bone : this->bones)
    {
        if (object == bone->object)
            return true;
    }    
    return false;
}

bool MeshSkeletonBase::HasBoneAncestor(ObjectAccessor object) const
{
    auto parent = object.GetParent();
    while (parent.IsValid() && !parent.IsRoot())
    {
        if (HasBone(parent))
            return true;
        parent = parent.GetParent();
    }
    return false;
}

bool MeshSkeletonBase::HasBoneDescendant(ObjectAccessor object) const
{
    ObjectAccessorVector children;
    object.GetChildren(children);
    for (size_t i = 0; i < children.size(); i++)
    {
        if (HasBone(children[i]))
            return true;        
    }

    for (size_t i = 0; i < children.size(); i++)
    {
        if (HasBoneDescendant(children[i]))
            return true;        
    }

    return false;
}

void MeshSkeletonBase::AddBone(MeshBonePtr bone)
{
    this->bones.push_back(bone);
}

void MeshSkeletonBase::RemoveBonesWithNoVertexInfluence()
{
    for (auto boneIterator = this->bones.begin(); boneIterator != this->bones.end(); )
    {
        auto meshBone = *boneIterator;
        if (!BoneHasVertexInfluence(meshBone->object))
        {                
            RemovingBone(meshBone.get());
            boneIterator = this->bones.erase(boneIterator);
        }
        else
            ++boneIterator;
    }
}

bool MeshSkeletonBase::BoneHasVertexInfluence(ObjectAccessor object) const
{
    for (size_t i = 0; i < this->weightedVertices.size(); i++)
    {
        if (this->weightedVertices[i].HasBoneInfluence(object))
            return true;
    }
    return false;
}

void MeshSkeletonBase::RemovingBone(MeshBone* bone)
{
    for (auto meshBone : this->bones)
    {
        if (meshBone->parent == bone)
        {
            meshBone->parent = bone->parent;
            meshBone->SetInitialState(this->initialSkinTransform);
        }
    }
}

void MeshSkeletonBase::GetBoneAttachedObjects(MeshBone* bone, ObjectAccessorVector& attachedObjects) const
{
    ObjectAccessorVector children;
    bone->object.GetChildren(children);
    attachedObjects.reserve(children.size());

    for (size_t childIndex = 0; childIndex < children.size(); childIndex++)
    {
        if (!HasBone(children[childIndex]))
            attachedObjects.push_back(children[childIndex]);
    }
}

bool MeshSkeletonBase::WeightedVerticesHasSameBones(int a, int b) const
{
    if (this->weightedVertices.size() == 0 ||
        a < 0 || a >= (int)this->weightedVertices.size() ||
        b < 0 || b >= (int)this->weightedVertices.size())
    {
        return false;
    }

    return this->weightedVertices[a].HasSameBones(this->weightedVertices[b]);
}

void MeshSkeletonBase::Merge(MeshSkeletonBase& mergeSkeleton)
{
    if (mergeSkeleton.bones.empty())
        return;

    //Only need to set the initial skin transform if this skeleton hasn't been used
    TransformAccessor newInitialSkinTransform;
    auto initialSkinOffset = mergeSkeleton.initialSkinTransform.GetRelativeTo(mergeSkeleton.geometryState->transform);
    if (this->bones.empty())
    {
        this->initialSkinTransform = this->geometryState->transform;
        this->initialSkinTransform.Concatenate(initialSkinOffset);

        newInitialSkinTransform = this->initialSkinTransform;
    }
    else
    {
        newInitialSkinTransform = this->geometryState->transform;
        newInitialSkinTransform.Concatenate(initialSkinOffset);
    }
    
    //Merge bones
    for (auto& bone : this->bones)
    {
        if (!HasBone(bone->object))
        {
            if (bone->IsRoot())
                bone->SetInitialState(newInitialSkinTransform);
            this->bones.push_back(bone);
        }
    }

    //Re-set parent pointers (this really only needs to be done for new bones, but this code does it for all)
    for (auto& bone : this->bones)
    {
        if (!bone->IsRoot())
            bone->parent = GetBone(bone->parent->object);
    }
    
    //Merge auxiliary bones
    for (auto aux : mergeSkeleton.auxiliaryBoneObjects)
        this->auxiliaryBoneObjects.insert(aux);
    
    //Merge vertex weights. At this point these probably aren't needed but keep them anyway
    size_t oldVertexWeightSize = this->weightedVertices.size();
    this->weightedVertices.resize(oldVertexWeightSize + mergeSkeleton.weightedVertices.size());
    for (size_t i = 0; i < mergeSkeleton.weightedVertices.size(); i++)
        this->weightedVertices[oldVertexWeightSize + i] = mergeSkeleton.weightedVertices[i];

    mergeSkeleton.bones.clear();
    mergeSkeleton.auxiliaryBoneObjects.clear();
    mergeSkeleton.weightedVertices.clear();
}

void MeshSkeletonBase::TransformToWorldSpace()
{
    if (this->bones.empty())
        return;

    TransformAccessor identityTransform;
    TransformAccessor offsetTransform = this->initialSkinTransform.GetRelativeTo(this->geometryState->transform); 
    
    //Re-initialize root bones
    for (auto& bone : this->bones)
    {
        if (bone->IsRoot())
        {
            //TODO: Fix or remove this
            /*if (bone->skinHasPivotOffset)
                bone->SetInitialState(identityTransform);
            else*/
                bone->SetInitialState(offsetTransform);
        }            
    }
}

void MeshSkeletonBase::FinishCreate()
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("MeshSkeletonBase::FinishCreate()"));

    //Remove unnecessary bones
    if (this->geometryState != nullptr && AnySet(this->geometryState->createFlags & GeometryStateFlags::REMOVE_BONES_WITH_NO_VERTEX_INFLUENCE))
        RemoveBonesWithNoVertexInfluence();

    //Remove excess bone assignments
    RemoveExcessAssignments();
}

void MeshSkeletonBase::RemoveExcessAssignments(size_t maxAssignments)
{
    for (size_t i = 0; i < this->weightedVertices.size(); i++)
        this->weightedVertices[i].RemoveExcessAssignments(maxAssignments);
}
