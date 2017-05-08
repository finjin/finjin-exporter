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
#include "MeshSkeleton.hpp"
#include "MaxUtilities.hpp"
#include "MaxMathUtilities.hpp"
#include "GeometryStateBase.hpp"
#include "Strings.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
MeshSkeleton::MeshSkeleton(GeometryStateBase* geometryState) : MeshSkeletonBase(geometryState)
{
}

void MeshSkeleton::Clear()
{
    MeshSkeletonBase::Clear();

    this->physiqueModifier.Clear();
    this->skinModifier.Clear();
}

bool MeshSkeleton::Initialize(INode* maxNode, const CoordinateSystemConverter& coordinateConverter, float scale, const SkeletonReferencePose* referencePose)
{
    Clear();

    if (referencePose != nullptr)
        this->referencePose = *referencePose;

    this->physiqueModifier = MaxUtilities::FindPhysiqueModifier(maxNode);
    this->skinModifier = MaxUtilities::FindSkinModifier(maxNode);
    if (this->physiqueModifier.IsValid() || this->skinModifier.IsValid())
    {
        Matrix3 maxSkinTransform;
        auto maxSkinTransformValid = false;
        if (this->physiqueModifier.IsValid())
        {
            auto physiqueExport = static_cast<IPhysiqueExport*>(this->physiqueModifier->GetInterface(I_PHYINTERFACE));

            //Get initial skin transformation
            switch (this->referencePose.type)
            {
                case SkeletonReferencePose::BIND_POSE: maxSkinTransformValid = physiqueExport->GetInitNodeTM(maxNode, maxSkinTransform) == MATRIX_RETURNED; break;
                case SkeletonReferencePose::SPECIFIC_TIME: maxSkinTransformValid = true; maxSkinTransform = maxNode->GetNodeTM(this->referencePose.time.GetNativeTime()); break;
            }

            //Get bones and influences
            auto physiqueContextExport = static_cast<IPhyContextExport*>(physiqueExport->GetContextInterface(maxNode));
            physiqueContextExport->ConvertToRigid(TRUE);
            physiqueContextExport->AllowBlending(TRUE);

            int skinnedVertexCount = physiqueContextExport->GetNumberVertices();
            if (this->geometryState != nullptr)
                this->weightedVertices.resize(skinnedVertexCount);

            auto loggedFailedPhysiqueMessage = false;
            for (int vertexIndex = 0; vertexIndex < skinnedVertexCount; vertexIndex++)
            {
                auto vertexExport = physiqueContextExport->GetVertexInterface(vertexIndex);
                if (vertexExport == nullptr)
                {
                    //This has happened in only one case, where the object was of a "boolean" type
                    if (!loggedFailedPhysiqueMessage)
                    {
                        FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, Strings::PHYSIQUE_DATA_CANNOT_BE_EXPORTED);
                        loggedFailedPhysiqueMessage = true;
                    }
                }
                else if (vertexExport->GetVertexType() == RIGID_NON_BLENDED_TYPE)
                {
                    auto rigidVertexExport = static_cast<IPhyRigidVertex*>(vertexExport);
                    auto maxBone = rigidVertexExport->GetNode();

                    //Add bone
                    if (!HasBone(maxBone))
                        AddBone(MeshBonePtr(new MeshBone(maxBone)));

                    //Add influence
                    if (this->geometryState != nullptr)
                        this->weightedVertices[vertexIndex].AddBoneInfluence(maxBone, 1.0f);
                }
                else if (vertexExport->GetVertexType() == RIGID_BLENDED_TYPE)
                {
                    auto blendedVertexExport = static_cast<IPhyBlendedRigidVertex*>(vertexExport);

                    for (int boneIndex = 0; boneIndex < blendedVertexExport->GetNumberNodes(); boneIndex++)
                    {
                        auto maxBone = blendedVertexExport->GetNode(boneIndex);

                        //Add bone
                        if (!HasBone(maxBone))
                            AddBone(MeshBonePtr(new MeshBone(maxBone)));

                        //Add influence
                        if (this->geometryState != nullptr)
                            this->weightedVertices[vertexIndex].AddBoneInfluence(maxBone, blendedVertexExport->GetWeight(boneIndex));
                    }
                }
                physiqueContextExport->ReleaseVertexInterface(vertexExport);
            }

            physiqueExport->ReleaseContextInterface(physiqueContextExport);
            this->physiqueModifier->ReleaseInterface(I_PHYINTERFACE, physiqueExport);
        }
        else if (this->skinModifier.IsValid())
        {
            //Get skin interface
            auto iskin = static_cast<ISkin*>(this->skinModifier->GetInterface(I_SKIN));
            if (iskin != nullptr && iskin->GetNumBones() > 0)
            {
                //Get initial skin transformation
                switch (this->referencePose.type)
                {
                    case SkeletonReferencePose::BIND_POSE: maxSkinTransformValid = iskin->GetSkinInitTM(maxNode, maxSkinTransform, false) == SKIN_OK; break;
                    case SkeletonReferencePose::SPECIFIC_TIME: maxSkinTransformValid = true; maxSkinTransform = maxNode->GetNodeTM(this->referencePose.time.GetNativeTime()); break;
                }

                //Get bones and influences
                auto iskinContextData = iskin->GetContextInterface(maxNode);

                //Bones
                for (int boneIndex = 0; boneIndex < iskin->GetNumBones(); boneIndex++)
                    AddBone(MeshBonePtr(new MeshBone(iskin->GetBone(boneIndex))));

                //Influences
                if (this->geometryState != nullptr)
                {
                    int skinnedVertexCount = iskinContextData->GetNumPoints();
                    this->weightedVertices.resize(skinnedVertexCount);
                    for (int vertexIndex = 0; vertexIndex < skinnedVertexCount; vertexIndex++)
                    {
                        int vertexBoneCount = iskinContextData->GetNumAssignedBones(vertexIndex);
                        for (int boneIndex = 0; boneIndex < vertexBoneCount; boneIndex++)
                        {
                            auto bone = iskin->GetBone(iskinContextData->GetAssignedBone(vertexIndex, boneIndex));
                            this->weightedVertices[vertexIndex].AddBoneInfluence(bone, iskinContextData->GetBoneWeight(vertexIndex, boneIndex));
                        }
                    }
                }

                //Release skin interface
                this->skinModifier->ReleaseInterface(I_SKIN, iskin);
            }
            else
                this->skinModifier.Clear();
        }

        //Set initial skin transformation
        if (!maxSkinTransformValid)
            maxSkinTransform = maxNode->GetNodeTM(0);
        if (this->physiqueModifier.IsValid() && this->referencePose.type == SkeletonReferencePose::BIND_POSE)
        {
            //TODO: This needs to be corrected or removed
            /*auto pivotOffset = MaxMathUtilities::GetObjectOffsetTM(maxNode);
            pivotOffset.Invert();
            maxSkinTransform *= pivotOffset;*/
        }
        this->initialSkinTransform.Set(maxSkinTransform, coordinateConverter, scale);

        //Initialize all the bones
        Disable();
        for (auto& bone : this->bones)
            InitializeBone(bone.get(), coordinateConverter, scale);
        UpdateHierarchy();
        Restore();
    }

    return IsValid();
}

bool MeshSkeleton::InitializeBone(MeshBone* bone, const CoordinateSystemConverter& coordinateConverter, float scale)
{
    if (bone->initialized)
        return true;

    //TODO: This needs to be corrected or removed
    /*bone->skinHasPivotOffset =
        this->physiqueModifier.IsValid() &&
        this->referencePose.type == SkeletonReferencePose::BIND_POSE;*/

    auto isChildBone = true;

    //Determine if the bone is a root
    auto parentBoneNode = bone->object.node->GetParentNode();
    if (!parentBoneNode->IsRootNode() && MaxUtilities::IsBipedBone(parentBoneNode))
    {
        //Do nothing. Parent is a bone, so child is a bone too
    }
    else if (parentBoneNode->IsRootNode() || !HasBoneAncestor(bone->object))
        isChildBone = false;

    //Add parent if necessary
    if (isChildBone)
    {
        bone->parent = FindBone(parentBoneNode).get();
        if (bone->parent == nullptr)
        {
            MeshBonePtr parentBone(new MeshBone(parentBoneNode, false));
            bone->parent = parentBone.get();
            AddBone(parentBone);
        }

        //Ensure parent is initialized
        InitializeBone(static_cast<MeshBone*>(bone->parent), coordinateConverter, scale);
    }

    //Get bone transformation
    Matrix3 maxBoneTransform;
    auto success = false;
    switch (this->referencePose.type)
    {
        case SkeletonReferencePose::BIND_POSE:
        {
            if (this->physiqueModifier.IsValid())
            {
                auto physiqueExport = static_cast<IPhysiqueExport*>(this->physiqueModifier->GetInterface(I_PHYINTERFACE));
                if (physiqueExport != nullptr)
                {
                    success = physiqueExport->GetInitNodeTM(bone->object, maxBoneTransform) == MATRIX_RETURNED;
                    this->physiqueModifier->ReleaseInterface(I_PHYINTERFACE, physiqueExport);
                }
            }
            else if (this->skinModifier.IsValid())
            {
                auto iskin = static_cast<ISkin*>(this->skinModifier->GetInterface(I_SKIN));
                if (iskin != nullptr)
                {
                    success = iskin->GetBoneInitTM(bone->object, maxBoneTransform, false) == SKIN_OK;
                    this->skinModifier->ReleaseInterface(I_SKIN, iskin);
                }
            }
            break;
        }
        case SkeletonReferencePose::SPECIFIC_TIME:
        {
            success = true;
            maxBoneTransform = bone->object.node->GetNodeTM(this->referencePose.time.GetNativeTime());
            break;
        }
    }

    if (!success)
    {
        //Failure may occur for bones that were implicitly added to the skeleton
        //Getting the node transformation works correctly since the skeleton has been placed into "initial pose" mode by this point
        maxBoneTransform = bone->object.node->GetNodeTM(0);
    }

    //Initialize the bone's initial position/rotation/scale state
    bone->initialWorldTransform.Set(maxBoneTransform, coordinateConverter, scale);
    bone->SetInitialState(this->initialSkinTransform);

    //Add children if necessary
    for (int i = 0; i < bone->object.node->NumberOfChildren(); i++)
    {
        ObjectAccessor child = bone->object.node->GetChildNode(i);

        if (!HasBone(child) &&
            (this->geometryState == nullptr || !child.HasDescendant(this->geometryState->createObject)) &&
            (MaxUtilities::IsAnyBone(child) || MaxUtilities::IsUnknownObject(child) || HasBoneDescendant(child)))
        {
            auto childBone = new MeshBone(child, false);
            childBone->parent = bone;
            AddBone(MeshBonePtr(childBone));
            InitializeBone(childBone, coordinateConverter, scale);
        }
    }

    return isChildBone;
}

bool MeshSkeleton::IsValid() const
{
    return !this->bones.empty();
}

void MeshSkeleton::Enable(bool enable)
{
    if (IsValid())
    {
        if (this->referencePose.type == SkeletonReferencePose::BIND_POSE)
        {
            if (this->physiqueModifier.IsValid())
            {
                auto physiqueExport = static_cast<IPhysiqueExport*>(this->physiqueModifier->GetInterface(I_PHYINTERFACE));
                if (physiqueExport != nullptr)
                {
                    physiqueExport->SetInitialPose(!enable);
                    this->physiqueModifier->NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
                    this->physiqueModifier->ReleaseInterface(I_PHYINTERFACE, physiqueExport);
                }
            }
            else if (this->skinModifier.IsValid())
                this->skinModifier.Enable(enable);
        }
    }
}

void MeshSkeleton::Restore()
{
    if (IsValid())
    {
        if (this->referencePose.type == SkeletonReferencePose::BIND_POSE)
        {
            if (this->physiqueModifier.IsValid())
            {
                auto physiqueExport = static_cast<IPhysiqueExport*>(this->physiqueModifier->GetInterface(I_PHYINTERFACE));
                if (physiqueExport != nullptr)
                {
                    physiqueExport->SetInitialPose(false);
                    this->physiqueModifier->ReleaseInterface(I_PHYINTERFACE, physiqueExport);
                }
            }
            else if (this->skinModifier.IsValid())
                this->skinModifier.Restore();
        }
    }
}

bool MeshSkeleton::HasSkeleton(ObjectAccessor object)
{
    return
        MaxUtilities::FindPhysiqueModifier(object.node) != nullptr ||
        MaxUtilities::FindSkinModifier(object.node) != nullptr;
}
