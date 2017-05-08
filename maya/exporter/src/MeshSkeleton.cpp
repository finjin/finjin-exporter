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
#include "Logger.hpp"
#include "MayaUtilities.hpp"
#include "GeometryStateBase.hpp"
#include "ApplicationAccessor.hpp"
#include <maya/MIkSystem.h>

using namespace Finjin::Exporter;


//Local functions---------------------------------------------------------------
static MObject GetJointForCluster(MObject& jointCluster)
{
    MObject result;
    MFnDependencyNode fnNode(jointCluster);
    MObject attrJoint = fnNode.attribute("matrix");
    MPlug pJointPlug(jointCluster, attrJoint);
    MPlugArray conns;
    if (pJointPlug.connectedTo(conns, true, false))
        result = conns[0].node();
    return result;
}


//Implementation----------------------------------------------------------------
MeshSkeleton::MeshSkeleton(GeometryStateBase* geometryState) : MeshSkeletonBase(geometryState)
{
}

MeshSkeleton::~MeshSkeleton()
{
    Clear();
}

void MeshSkeleton::Clear()
{
    MeshSkeletonBase::Clear();

    this->smoothClusters.clear();
    this->rigidClusters.clear();
}

bool MeshSkeleton::Initialize(MObject object, const CoordinateSystemConverter& coordinateConverter, float scale, const SkeletonReferencePose* referencePose)
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("MeshSkeleton::Initialize()"));

    Clear();

    if (referencePose != nullptr)
        this->referencePose = *referencePose;

    MStatus status;
    MFnMesh mesh(object, &status);
    if (status != MStatus::kSuccess)
        return false;

    //If there's a skin, get the joints (bones)
    if (FindMeshSkin(mesh))
    {
        this->weightedVertices.resize(mesh.numVertices());

        size_t envelopeIndex = 0;

        if (!this->smoothClusters.empty())
        {
            //Smooth skin

            this->originalEnvelopes.resize(this->smoothClusters.size());
            for (auto smoothCluster = this->smoothClusters.begin();
                smoothCluster != this->smoothClusters.end();
                ++smoothCluster)
            {
                MFnSkinCluster smoothSkin(*smoothCluster);

                //Hold onto initial envelope
                this->originalEnvelopes[envelopeIndex++] = smoothSkin.envelope();

                //Get bones
                MDagPathArray influenceObjects;
                unsigned int influenceObjectCount = smoothSkin.influenceObjects(influenceObjects);
                for (unsigned int influenceIndex = 0; influenceIndex < influenceObjectCount; influenceIndex++)
                    AddBone(MeshBonePtr(new MeshBone(influenceObjects[influenceIndex].node())));

                //Get influences
                if (this->geometryState != nullptr)
                {
                    MDagPath path;
                    mesh.getPath(path);
                    int vertexIndex = 0;
                    for (MItGeometry geoIt(path); !geoIt.isDone(); geoIt.next(), vertexIndex++)
                    {
                        MFloatArray weightArray;
                        unsigned int numInfluences;
                        smoothSkin.getWeights(path, geoIt.component(), weightArray, numInfluences);

                        for (unsigned int weightIndex = 0; weightIndex < weightArray.length(); weightIndex++)
                        {
                            float weight = weightArray[weightIndex];
                            if (weight > 0.001f)
                                this->weightedVertices[vertexIndex].AddBoneInfluence(influenceObjects[weightIndex].node(), weight);
                        }
                    }
                }
            }
        }
        else
        {
            //Rigid skin

            this->originalEnvelopes.resize(this->rigidClusters.size());
            for (auto rigidCluster = this->rigidClusters.begin();
                rigidCluster != this->rigidClusters.end();
                ++rigidCluster)
            {
                MFnWeightGeometryFilter jointCluster(rigidCluster->first);
                MObject joint = rigidCluster->second;
                MFnDependencyNode jointDepNode(joint);

                //Hold onto initial envelope
                this->originalEnvelopes[envelopeIndex++] = jointCluster.envelope();

                //Add the bone
                AddBone(MeshBonePtr(new MeshBone(joint)));

                //Get influences
                if (this->geometryState != nullptr)
                {
                    MObject deformSet = jointCluster.deformerSet();
                    MFnSet setFn(deformSet);
                    MSelectionList clusterSetList;
                    setFn.getMembers(clusterSetList, true);
                    for (unsigned int clusterSetIndex = 0;
                        clusterSetIndex < clusterSetList.length();
                        clusterSetIndex++)
                    {
                        MDagPath skinpath;
                        MObject components;
                        clusterSetList.getDagPath(clusterSetIndex, skinpath, components);

                        MFloatArray weights;
                        jointCluster.getWeights(skinpath, components, weights);

                        MItGeometry geomIter(skinpath, components);
                        for (unsigned int weightIndex = 0;
                            !geomIter.isDone() && weightIndex < weights.length();
                            geomIter.next(), weightIndex++)
                        {
                            float weight = weights[weightIndex];
                            if (weight > 0)
                                this->weightedVertices[weightIndex].AddBoneInfluence(joint, weight);
                        }
                    }
                }
            }
        }

        //Initialize all the bone parents
        for (auto& bone : this->bones)
            InitializeBoneParent(bone.get());

        //Set up for initializing bones
        MString savedPoseName;
        MTime savedPoseTime;
        bool savedPose = false;
        MSelectionList selectionList;
        if (this->geometryState != nullptr)
        {
            MGlobal::getActiveSelectionList(selectionList);
            SelectBones();
            savedPose = SetBindPose(savedPoseName, savedPoseTime);
        }

        //Set initial skin transformation
        ObjectAccessor objectAccessor(object);
        MMatrix skinMatrix;
        switch (this->referencePose.type)
        {
            case SkeletonReferencePose::BIND_POSE: MayaUtilities::GetFullWorldMatrix(objectAccessor.container, skinMatrix); break;
            case SkeletonReferencePose::SPECIFIC_TIME:
            {
                TimeChanger timeChanger(this->referencePose.time);
                MayaUtilities::GetFullWorldMatrix(objectAccessor.container, skinMatrix);
                break;
            }
        }
        MTransformationMatrix skinTransform = skinMatrix;
        this->initialSkinTransform.Set(skinTransform, coordinateConverter, scale);

        //Bones
        for (auto& bone : this->bones)
            InitializeBone(bone.get(), coordinateConverter, scale);
        UpdateHierarchy();

        //Un-initialize
        if (this->geometryState != nullptr)
        {
            if (savedPose)
                UnsetBindPose(savedPoseName, savedPoseTime);
            MGlobal::setActiveSelectionList(selectionList, MGlobal::kReplaceList);
        }
    }

    return IsValid();
}

bool MeshSkeleton::IsValid() const
{
    return !this->bones.empty();
}

bool MeshSkeleton::InitializeBoneParent(MeshBone* bone)
{
    auto isChildBone = true;

    MObject parentBoneObject = bone->object.GetParent().obj;
    if (!parentBoneObject.isNull() && !parentBoneObject.hasFn(MFn::kJoint))
    {
        //Parent is some type of object that indicates the end of the skeleton
        isChildBone = false;
    }

    //Get parent if necessary
    if (isChildBone)
        bone->parent = FindBone(parentBoneObject).get();

    bone->hasBoneAncestorInScene = HasBoneAncestorInScene(bone);

    return isChildBone;
}

bool MeshSkeleton::InitializeBone(MeshBone* bone, const CoordinateSystemConverter& coordinateConverter, float scale)
{
    if (bone->initialized)
        return true;

    auto isChildBone = true;

    //Determine if the bone is a root
    MObject parentBoneObject = bone->object.GetParent().obj;
    if (!parentBoneObject.isNull() && !parentBoneObject.hasFn(MFn::kJoint))
    {
        //Parent is some type of object that indicates the end of the skeleton
        if (!MayaUtilities::IsRoot(parentBoneObject) && //Don't add the root
            parentBoneObject.apiType() != MFn::kTransform) //Sometimes users have the mesh and skeleton under a transform node, so don't add that either
        {
            this->auxiliaryBoneObjects.insert(parentBoneObject);
        }
        isChildBone = false;
    }

    //Add parent if necessary
    if (isChildBone)
    {
        if (bone->parent == nullptr)
        {
            bone->parent = FindBone(parentBoneObject).get();
            if (bone->parent == nullptr)
            {
                auto parentBone = new MeshBone(parentBoneObject, false);
                bone->parent = parentBone;
                AddBone(MeshBonePtr(parentBone));
            }
        }

        //Ensure parent is initialized
        InitializeBone(static_cast<MeshBone*>(bone->parent), coordinateConverter, scale);
    }

    //Initialize the bone's initial position/rotation/scale state
    TransformAccessor mayaBoneTransform;
    switch (this->referencePose.type)
    {
        case SkeletonReferencePose::BIND_POSE: mayaBoneTransform = bone->GetNodeTransformation(); break;
        case SkeletonReferencePose::SPECIFIC_TIME: mayaBoneTransform = bone->GetNodeTransformation(this->referencePose.time); break;
    }
    bone->initialWorldTransform.Set(mayaBoneTransform, coordinateConverter, scale);
    bone->SetInitialState(this->initialSkinTransform);

    //Add children if necessary
    MFnDagNode dagNode(bone->object.container);
    unsigned int childCount = dagNode.childCount();
    for (unsigned int i = 0; i < childCount; i++)
    {
        MObject child = dagNode.child(i);
        if (!HasBone(child) &&
            (this->geometryState == nullptr || !ObjectAccessor(child).HasDescendant(this->geometryState->createObject)) &&
            HasBoneDescendant(child))
        {
            auto childBone = new MeshBone(child, false);
            childBone->parent = bone;
            AddBone(MeshBonePtr(childBone));
            InitializeBone(childBone, coordinateConverter, scale);
        }
    }

    return isChildBone;
}

void MeshSkeleton::Enable(bool enable)
{
    //FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("MeshSkeleton::Enable(%d)"), (int)enable);

    if (IsValid())
    {
        if (this->referencePose.type == SkeletonReferencePose::BIND_POSE)
        {
            //Set auto keyframing state
            this->autoKeyState.Enable(enable);

            //Set constraints
            this->nodeMenuItemsState.Enable(enable);

            //Set envelopes
            const float enableValue = enable ? 1 : 0;
            if (!this->smoothClusters.empty())
            {
                for (auto smoothCluster = this->smoothClusters.begin();
                    smoothCluster != this->smoothClusters.end();
                    ++smoothCluster)
                {
                    MFnSkinCluster smoothSkin(*smoothCluster);
                    smoothSkin.setEnvelope(enableValue);
                }
            }
            else
            {
                for (auto rigidCluster = this->rigidClusters.begin();
                    rigidCluster != this->rigidClusters.end();
                    ++rigidCluster)
                {
                    MFnWeightGeometryFilter jointCluster(rigidCluster->first);
                    jointCluster.setEnvelope(enableValue);
                }
            }
        }
    }
}

void MeshSkeleton::Restore()
{
    //FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("MeshSkeleton::Restore()"));

    if (IsValid())
    {
        if (this->referencePose.type == SkeletonReferencePose::BIND_POSE)
        {
            //Restore envelopes
            size_t envelopeIndex = 0;
            if (!this->smoothClusters.empty())
            {
                for (auto smoothCluster = this->smoothClusters.begin();
                    smoothCluster != this->smoothClusters.end();
                    ++smoothCluster)
                {
                    MFnSkinCluster smoothSkin(*smoothCluster);
                    smoothSkin.setEnvelope(this->originalEnvelopes[envelopeIndex++]);
                }
            }
            else
            {
                for (auto rigidCluster = this->rigidClusters.begin();
                    rigidCluster != this->rigidClusters.end();
                    ++rigidCluster)
                {
                    MFnWeightGeometryFilter jointCluster(rigidCluster->first);
                    jointCluster.setEnvelope(this->originalEnvelopes[envelopeIndex++]);
                }
            }

            //Restore constraints
            this->nodeMenuItemsState.Restore();

            //Restore auto keyframing state
            this->autoKeyState.Restore();
        }
    }
}


bool MeshSkeleton::HasSkeleton(ObjectAccessor object)
{
    MeshSkeleton meshSkeleton(nullptr);
    CoordinateSystemConverter nullConverter(ApplicationAccessor::GetUpAxis());
    return meshSkeleton.Initialize(object, nullConverter, 1, nullptr);
}

bool MeshSkeleton::FindMeshSkin(MFnMesh& mesh)
{
    auto result = false;

    MPlug inMeshPlug = mesh.findPlug("inMesh");
    if (inMeshPlug.isConnected())
    {
        //Walk the tree upstream
        MStatus status;
        MItDependencyGraph dgIt
            (
            inMeshPlug,
            MFn::kInvalid,
            MItDependencyGraph::kUpstream,
            MItDependencyGraph::kDepthFirst,
            MItDependencyGraph::kPlugLevel,
            &status
            );
        if (status == MS::kSuccess)
        {
            dgIt.disablePruningOnFilter();
            for ( ;!dgIt.isDone(); dgIt.next())
            {
                MObject thisNode = dgIt.thisNode();
                if (thisNode.apiType() == MFn::kSkinClusterFilter)
                {
                    this->smoothClusters.push_back(thisNode);
                    result = true;
                }
                else if (thisNode.apiType() == MFn::kJointCluster)
                {
                    MObject joint = GetJointForCluster(thisNode);
                    if (!joint.isNull())
                    {
                        this->rigidClusters.push_back(std::make_pair(thisNode, joint));
                        result = true;
                    }
                }
            }
        }
    }

    return result;
}

void MeshSkeleton::SelectBones()
{
    int selectedCount = 0;
    for (auto boneIterator = this->bones.begin();
        boneIterator != this->bones.end();
        ++boneIterator)
    {
        MeshBonePtr bone = *boneIterator;

        if (bone->IsRoot() && bone->isExplicit && !bone->hasBoneAncestorInScene)
        {
            MString name = WxStringToApplicationString(bone->object.GetFullName());

            if (DISPLAY_MAYA_COMMAND)
                MGlobal::executeCommand("print \"Selecting bone: " + name + "\\n\"");
            MGlobal::selectByName(name, selectedCount == 0 ? MGlobal::kReplaceList : MGlobal::kAddToList);

            selectedCount++;
        }
    }

    //MGlobal::executeCommand("print \"\\n\"");
}

bool MeshSkeleton::SetBindPose(MString& savedPoseName, MTime& savedPoseTime)
{
    //FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("MeshSkeleton::SetBindPose()"));

    bool savedPose = false;
    savedPoseName.clear();

    Disable();

    MCommandResult results;
    MStringArray attachedBindPoses;
    MGlobal::executeCommand("dagPose -q -bindPose", results, DISPLAY_MAYA_COMMAND);
    if (results.getResult(attachedBindPoses) == MStatus::kSuccess && attachedBindPoses.length() > 0)
    {
        MString attachedBindPose = attachedBindPoses[0];
        if (attachedBindPose.length() > 0)
        {
            //Save the bones in their current state. This works on the current selection
            MGlobal::executeCommand("dagPose -save", savedPoseName, DISPLAY_MAYA_COMMAND);

            //Restore the selected bones to their bind pose state
            MGlobal::executeCommand("dagPose -restore -global -bindPose", DISPLAY_MAYA_COMMAND);

            //FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, "Saved pose: %s", savedPoseName.asChar());
            //MGlobal::executeCommand("print \"Saved pose for setting bind pose: " + savedPoseName + "\\n\"");
        }
        else
        {
            savedPoseTime = MAnimControl::currentTime();
            MAnimControl::setCurrentTime(MAnimControl::minTime());
        }

        savedPose = true;
    }

    return savedPose;
}

void MeshSkeleton::UnsetBindPose(const MString& savedPoseName, MTime& savedPoseTime)
{
    //FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("MeshSkeleton::UnsetBindPose()"));

    if (savedPoseName.length() > 0)
    {
        MGlobal::executeCommand("dagPose -restore -global \"" + savedPoseName + "\"", DISPLAY_MAYA_COMMAND);
        MayaUtilities::DeleteObject(savedPoseName);

        //MGlobal::executeCommand("print \"Restored bind pose: " + savedPoseName + "\\n\"");
    }
    else
        MAnimControl::setCurrentTime(savedPoseTime);

    Restore();
}

bool MeshSkeleton::HasBoneAncestorInScene(MeshBone* bone) const
{
    for (ObjectAccessor parent = bone->object.GetParent();
        parent.IsValid() && !parent.IsRoot();
        parent = parent.GetParent())
    {
        if (HasBone(parent))
            return true;
    }
    return false;
}
