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
#include "GeometryStateBase.hpp"
#include "MathUtilities.hpp"

using namespace Finjin::Exporter;


//Local types-------------------------------------------------------------------
class BoneHierarchy
{
    struct Entry
    {
    public:
        Entry()
        {
        }

        Entry(MeshBonePtr meshBone)
        {
            this->meshBone = meshBone;
        }

        bool operator < (const Entry& other) const
        {
            return this->meshBone->object.GetLocalName() < other.meshBone->object.GetLocalName();
        }

        Entry& AddChild(MeshBonePtr& meshBone)
        {
            for (auto& child : this->children)
            {
                if (child.meshBone == meshBone)
                    return child;
            }

            this->children.push_back(Entry(meshBone));
            return this->children.back();
        }

        void SortChildren()
        {
            std::sort(this->children.begin(), this->children.end());

            for (auto& child : this->children)
                child.SortChildren();
        }

        void GetOrderedBones(MeshBones& bones)
        {
            if (this->meshBone != nullptr)
                bones.push_back(this->meshBone);

            for (auto& child : this->children)
                child.GetOrderedBones(bones);
        }

    public:
        MeshBonePtr meshBone;
        std::vector<Entry> children;
    };

public:
    BoneHierarchy(MeshSkeleton& meshSkeleton)
    {
        for (auto& meshBone : meshSkeleton.bones)
            Add(meshSkeleton, meshBone);
    }

    void GetOrderedBones(MeshBones& bones)
    {
        bones.clear();
        this->superRoot.SortChildren();
        this->superRoot.GetOrderedBones(bones);
    }

private:
    Entry& Add(MeshSkeleton& meshSkeleton, MeshBonePtr& meshBone)
    {
        if (meshBone->IsRoot())
            return this->superRoot.AddChild(meshBone);
        else
        {
            MeshBonePtr foundBone = meshSkeleton.FindBone(meshBone->parent);
            return Add(meshSkeleton, foundBone).AddChild(meshBone);
        }
    }

private:
    Entry superRoot;
};


//Implementation----------------------------------------------------------------
GeometryStateBase::GeometryStateBase() : meshSkeleton(this), meshMorpher(this)
{
}

GeometryStateBase::~GeometryStateBase()
{
    Clear();
}

void GeometryStateBase::Clear()
{
    this->submeshes.clear();

    this->meshPositions.clear();
    this->meshNormals.Destroy();

    this->meshSkeleton.Clear();
    this->meshSkeletonAnimations.clear();

    this->meshMorpher.Clear();
    this->meshMorpherAnimations.clear();

    this->meshMorphAnimations.clear();

    this->standardBounds.Reset();
    this->animatedBounds.Reset();

    this->geometryType = GeometryStateSubmesh::NO_GEOMETRY_TYPE;
}

void GeometryStateBase::Merge(GeometryStateBase& mergeGeometry, bool alwaysCreateNewSubmeshes)
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("GeometryStateBase::Merge(%s, %d)"), mergeGeometry.createObject.GetLocalName().wx_str(), alwaysCreateNewSubmeshes);

    //Get matrices for transforming vertices and normals
    auto mergeTransformation = mergeGeometry.transform.GetRelativeTo(this->transform);
    MatrixAccessor transformMatrix, normalTransformMatrix;
    mergeTransformation.MakeTransformMatrices(transformMatrix, normalTransformMatrix);

    auto geometryMergeTransformation = mergeGeometry.transform.GetRelativeTo(this->geometryTransform);
    MatrixAccessor geometryTransformMatrix, geometryNormalTransformMatrix;
    geometryMergeTransformation.MakeTransformMatrices(geometryTransformMatrix, geometryNormalTransformMatrix);

    //Merge points-----------------------------------
    auto oldPointsSize = this->meshPositions.size();
    this->meshPositions.resize(oldPointsSize + mergeGeometry.meshPositions.size());
    for (size_t pointIndex = 0; pointIndex < mergeGeometry.meshPositions.size(); pointIndex++)
    {
        auto newPoint = mergeGeometry.meshPositions[pointIndex];
        geometryTransformMatrix.TransformPoint(newPoint);
        this->meshPositions[oldPointsSize + pointIndex] = newPoint;
    }

    //Reallocate indexing-----------------------------------
    for (auto submesh : this->submeshes)
        submesh->Reserve(this->meshPositions.size());

    //Merge submeshes-----------------------------------
    std::unordered_map<GeometryStateSubmesh*, GeometryStateSubmesh*> mergedSubmeshMap;
    for (auto& mergeSubmesh : mergeGeometry.submeshes)
    {
        auto compatibleSubmesh = alwaysCreateNewSubmeshes ? nullptr : FindSubmeshForMerge(mergeGeometry, mergeSubmesh.get());
        if (compatibleSubmesh == nullptr)
        {
            //No existing submesh matches, so use the submesh to merge directly
            mergeSubmesh->InitializeForMerge((int)this->submeshes.size(), oldPointsSize);
            this->submeshes.push_back(mergeSubmesh);

            mergedSubmeshMap[mergeSubmesh.get()] = mergeSubmesh.get();

            //Update vertices and bounds
            for (auto& vertex : mergeSubmesh->vertexList)
            {
                //Update vertex
                geometryTransformMatrix.TransformPoint(vertex.position);
                geometryNormalTransformMatrix.TransformNormal(vertex.normal);
            }
        }
        else
        {
            //Found a compatible submesh

            //Merge the extra data
            compatibleSubmesh->submeshProperties.Merge(mergeSubmesh->submeshProperties);

            mergedSubmeshMap[mergeSubmesh.get()] = compatibleSubmesh;

            auto vertexOffset = (unsigned int)compatibleSubmesh->vertexList.GetSize(); //Save this for later

            //Add vertices from the mergable submesh to the compatible submesh
            for (auto vertex : mergeSubmesh->vertexList)
            {
                //Transform vertex
                geometryTransformMatrix.TransformPoint(vertex.position);
                geometryNormalTransformMatrix.TransformNormal(vertex.normal);

                //Add vertex and index
                compatibleSubmesh->vertexList.Add(vertex, 0, true); //Always add vertices
            }

            //Add indices from the mergable submesh to the compatible submesh
            for (size_t i = 0; i < mergeSubmesh->indices.size(); i++)
                compatibleSubmesh->indices.push_back(vertexOffset + mergeSubmesh->indices[i]);

            //Update reindexing by adding onto the existing indexing
            for (size_t i = 0; i < mergeSubmesh->GetReindexedVertexCount(); i++)
            {
                auto& sourceReindexed = mergeSubmesh->GetReindexedVertexConst(i);
                auto& destinationReindexed = compatibleSubmesh->GetReindexedVertex(oldPointsSize + i);

                destinationReindexed = sourceReindexed;
                for (size_t j = 0; j < destinationReindexed.size(); j++)
                    destinationReindexed[j] += vertexOffset;
            }

            //Update primitive count
            compatibleSubmesh->primitiveCount += mergeSubmesh->primitiveCount;

            //Add visible edges from mergable submesh to the compatible submesh
            for (auto& edge : mergeSubmesh->visibleEdges)
            {
                compatibleSubmesh->visibleEdges.insert(FinjinEdge(vertexOffset + edge.e1, vertexOffset + edge.e2));
            }
        }
    }

    //Merge bounding volumes-----------------------------------
    this->standardBounds.MergeFrom(mergeGeometry, geometryTransformMatrix);
    this->animatedBounds.MergeFrom(mergeGeometry, geometryTransformMatrix);

    //Merge skeleton animations------------------------
    if (!mergeGeometry.meshSkeletonAnimations.empty())
    {
        auto oldSize = this->meshSkeletonAnimations.size();
        this->meshSkeletonAnimations.resize(oldSize + mergeGeometry.meshSkeletonAnimations.size());
        for (size_t animationIndex = 0; animationIndex < mergeGeometry.meshSkeletonAnimations.size(); animationIndex++)
        {
            auto& animation = this->meshSkeletonAnimations[oldSize + animationIndex];
            animation = mergeGeometry.meshSkeletonAnimations[animationIndex];

            //Remap the bone pointer if possible
            for (size_t boneIndex = 0; boneIndex < animation->boneAnimations.size(); boneIndex++)
            {
                auto& boneAnimation = animation->boneAnimations[boneIndex];
                auto foundBone = this->meshSkeleton.FindBone(boneAnimation->bone->object);
                if (foundBone != nullptr)
                    boneAnimation->bone = foundBone.get();
            }
        }
    }

    //Merge skeleton-----------------------------------
    this->meshSkeleton.Merge(mergeGeometry.meshSkeleton);

    //Merge morpher-----------------------------------
    this->meshMorpher.Merge(mergeGeometry.meshMorpher, geometryTransformMatrix, geometryNormalTransformMatrix);
    for (auto& morphTarget : this->meshMorpher.morphTargets)
    {
        //Remap the submesh pointer for each subtarget
        for (size_t subtargetIndex = 0; subtargetIndex < morphTarget->subtargets.size(); subtargetIndex++)
        {
            //This search will fail for previously merged targets
            //That's not a problem since those pointers are still valid
            auto foundTarget = mergedSubmeshMap.find(morphTarget->subtargets[subtargetIndex]->submesh);
            if (foundTarget != mergedSubmeshMap.end())
                morphTarget->subtargets[subtargetIndex]->submesh = foundTarget->second;
        }
    }

    //Merge mesh animations-----------------------------------

    //Morpher animations
    if (!mergeGeometry.meshMorpherAnimations.empty())
    {
        auto oldSize = this->meshMorpherAnimations.size();
        this->meshMorpherAnimations.resize(oldSize + mergeGeometry.meshMorpherAnimations.size());
        for (size_t animationIndex = 0; animationIndex < mergeGeometry.meshMorpherAnimations.size(); animationIndex++)
        {
            auto& animation = this->meshMorpherAnimations[oldSize + animationIndex];
            animation = mergeGeometry.meshMorpherAnimations[animationIndex];
            for (size_t subanimationIndex = 0; subanimationIndex < animation->subanimations.size(); subanimationIndex++)
            {
                auto subanimation = animation->subanimations[subanimationIndex];
                subanimation->submesh = mergedSubmeshMap[subanimation->submesh];
            }
        }
    }

    //Morph animations
    if (!mergeGeometry.meshMorphAnimations.empty())
    {
        auto oldSize = this->meshMorphAnimations.size();
        this->meshMorphAnimations.resize(oldSize + mergeGeometry.meshMorphAnimations.size());
        for (size_t animationIndex = 0; animationIndex < mergeGeometry.meshMorphAnimations.size(); animationIndex++)
        {
            auto& animation = this->meshMorphAnimations[oldSize + animationIndex];
            animation = mergeGeometry.meshMorphAnimations[animationIndex];
            for (size_t subanimationIndex = 0; subanimationIndex < animation->subanimations.size(); subanimationIndex++)
            {
                auto subanimation = animation->subanimations[subanimationIndex];
                subanimation->submesh = mergedSubmeshMap[subanimation->submesh];
                for (size_t keyIndex = 0; keyIndex < subanimation->keys.size(); keyIndex++)
                {
                    auto& key = subanimation->keys[keyIndex];
                    for (size_t i = 0; i < key.points.size(); i++)
                    {
                        geometryTransformMatrix.TransformPoint(key.points[i].position);
                        geometryNormalTransformMatrix.TransformNormal(key.points[i].normal);
                    }
                }
            }
        }
    }

    mergeGeometry.Clear();
}

void GeometryStateBase::Merge(GeometryStateBaseList& mergeGeometries, bool alwaysCreateNewSubmeshes)
{
    for (auto geometryIterator : mergeGeometries)
    {
        Merge(*geometryIterator, alwaysCreateNewSubmeshes);
        delete geometryIterator;
    }
    mergeGeometries.clear();
}

bool GeometryStateBase::IsMatchingInstance(const GeometryStateBase& other, float tolerance)
{
    //Check some high level settings first
    if (this->submeshes.size() != other.submeshes.size() ||
        !MathUtilities::AlmostZero(this->standardBounds.radius - other.standardBounds.radius, tolerance) ||
        !this->standardBounds.box.AlmostEquals(other.standardBounds.box, tolerance) ||
        this->meshSkeletonAnimations != other.meshSkeletonAnimations ||
        this->meshMorpherAnimations != other.meshMorpherAnimations ||
        this->meshMorphAnimations != other.meshMorphAnimations)
    {
        FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("High level settings check failed"));
        return false;
    }

    //Check the submeshes
    auto submeshIterator = this->submeshes.begin();
    auto otherSubmeshIterator = other.submeshes.begin();
    for (; submeshIterator != this->submeshes.end(); ++submeshIterator, ++otherSubmeshIterator)
    {
        auto thisSubmesh = submeshIterator->get();
        auto otherSubmesh = otherSubmeshIterator->get();

        if (thisSubmesh->vertexList.size() != otherSubmesh->vertexList.size())
        {
            //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Submesh %d/%d vertex list size check failed"), thisSubmesh->index, otherSubmesh->index);
            return false;
        }

        if (thisSubmesh->indices.size() != otherSubmesh->indices.size())
        {
            //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Submesh %d/%d indices check failed"), thisSubmesh->index, otherSubmesh->index);
            return false;
        }

        if (thisSubmesh->geometryType != otherSubmesh->geometryType)
        {
            //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Submesh %d/%d geometry type check failed"), thisSubmesh->index, otherSubmesh->index);
            return false;
        }

        if (!thisSubmesh->submeshProperties.MatchesForInstancing(otherSubmesh->submeshProperties))
        {
            //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Submesh %d/%d submesh properties check failed"), thisSubmesh->index, otherSubmesh->index);
            return false;
        }

        if (!thisSubmesh->vertexList.IsAlmostSame(otherSubmesh->vertexList, 3, tolerance))
        {
            //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Submesh %d/%d vertex list check failed"), thisSubmesh->index, otherSubmesh->index);
            return false;
        }
    }

    //If we made it this far, the two geometry states match
    return true;
}

GeometryStateSubmesh* GeometryStateBase::FindSubmeshForMerge(const GeometryStateBase& mergeGeometry, const GeometryStateSubmesh* submesh)
{
    GeometryStateSubmesh* compatibleSubmesh = nullptr;

    for (auto submesh : this->submeshes)
    {
        auto maybeCompatibleSubmesh = submesh.get();
        if (maybeCompatibleSubmesh->material == submesh->material &&
            maybeCompatibleSubmesh->submeshProperties.MatchesForMerge(submesh->submeshProperties))
        {
            compatibleSubmesh = maybeCompatibleSubmesh;
            break;
        }
    }

    return compatibleSubmesh;
}

void GeometryStateBase::UpdateBounds(bool isFirst, bool isNew, const FinjinVector3& point)
{
    //Update the bounding box and radius
    if (isFirst)
    {
        //Initialize bounds
        this->standardBounds.radius = point.LengthSquared();
        this->standardBounds.box.min = this->standardBounds.box.max = point;
    }
    else if (isNew)
    {
        //Update bounds
        this->standardBounds.radius = std::max(this->standardBounds.radius, point.LengthSquared());
        this->standardBounds.box.IncludePoint(point);
    }
}

void GeometryStateBase::UpdateAnimationData(bool forceSkeleton, bool forceMorpher)
{
    auto animationFlags = MeshAnimationFlags::NONE;
    if (this->meshSkeleton.IsValid() && (forceSkeleton || !this->meshSkeletonAnimations.empty()))
    {
        animationFlags |= MeshAnimationFlags::HAS_SKELETON;
        this->meshSkeleton.GetBones(this->submeshProperties.bones);
    }
    if (this->meshMorpher.IsValid() && (forceMorpher || !this->meshMorpherAnimations.empty()))
    {
        animationFlags |= MeshAnimationFlags::HAS_MORPHER;
        this->meshMorpher.GetMorphTargetNames(this->submeshProperties.morphTargetNames);
    }
    if (!this->meshMorphAnimations.empty())
        animationFlags |= MeshAnimationFlags::HAS_MORPHS;

    //Modify the geometry extra data
    for (auto submesh : this->submeshes)
    {
        submesh->submeshProperties.animationFlags |= animationFlags;
        submesh->submeshProperties.bones = this->submeshProperties.bones;
        submesh->submeshProperties.morphTargetNames = this->submeshProperties.morphTargetNames;
    }
}

bool GeometryStateBase::SampleSubmeshPoints
    (
    SubmeshesPoints& submeshesPoints,
    const CoordinateSystemConverter& coordinateConverter,
    float scale,
    TimeAccessor time,
    GeometryStateFlags flags
    )
{
    //This can only be called if the submeshes are part of a mesh (not a spline shape)
    for (auto submesh : this->submeshes)
    {
        if (submesh->geometryType != GeometryStateSubmesh::MESH)
            return false;
    }

    std::vector<FinjinVector3> positions;
    MeshNormals normals;
    if (SamplePoints(positions, coordinateConverter, scale, time, flags, &normals))
    {
        submeshesPoints.resize(this->submeshes.size());
        for (auto submesh : this->submeshes)
        {
            auto& submeshPoints = submeshesPoints[submesh->index];

            submeshPoints.points.resize(submesh->addedOriginalIndices.size());
            for (size_t pointIndex = 0; pointIndex < submesh->addedOriginalIndices.size(); pointIndex++)
            {
                auto& originalIndices = submesh->addedOriginalIndices[pointIndex];

                submeshPoints.points[pointIndex].position = positions[originalIndices.vertexIndex];
                submeshPoints.points[pointIndex].normal = normals.GetNormal(originalIndices.cornerIndex, originalIndices.vertexIndex, originalIndices.faceIndex, originalIndices.smoothingGroups);
                submeshPoints.points[pointIndex].tangent = normals.GetTangent(originalIndices.cornerIndex, originalIndices.vertexIndex, originalIndices.faceIndex, originalIndices.smoothingGroups);
            }
        }

        return true;
    }
    else
        return false;
}

void GeometryStateBase::AnimateBoundingVolumes
    (
    const std::vector<WxTimeDuration>& times,
    const CoordinateSystemConverter& coordinateConverter,
    float scale,
    const TransformAccessor* transform,
    ObjectAccessor rootObject
    )
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("GeometryStateBase::AnimateBoundingVolumes()"));

    FinjinVector3 rootOrigin;
    if (rootObject.IsValid() && !times.empty())
    {
        TimeAccessor sampleTime;
        sampleTime.SetSeconds(times[0].ToSecondsDouble());
        TransformAccessor rootTransform(rootObject.GetFullWorldTransformation(sampleTime), coordinateConverter, scale);
        rootOrigin = rootTransform.GetTranslation();
    }

    this->animatedBounds = this->standardBounds;

    auto boundingBoxEmpty = this->standardBounds.box.IsEmpty();

    std::vector<FinjinVector3> points;
    for (size_t i = 0; i < times.size(); i++)
    {
        TimeAccessor sampleTime;
        sampleTime.SetSeconds(times[i].ToSecondsDouble());

        if (!SamplePoints(points, coordinateConverter, scale, sampleTime, GeometryStateFlags::NONE))
            break;

        //Apply extra transformations if necessary
        if (rootObject.IsValid() && i > 0)
        {
            TransformAccessor rootTransform(rootObject.GetFullWorldTransformation(sampleTime), coordinateConverter, scale);
            FinjinVector3 rootOffset = rootTransform.GetTranslation() - rootOrigin;
            for (size_t pointIndex = 0; pointIndex < points.size(); pointIndex++)
                points[pointIndex] -= rootOffset;
        }
        if (transform != nullptr)
        {
            for (size_t pointIndex = 0; pointIndex < points.size(); pointIndex++)
                transform->TransformPoint(points[pointIndex]);
        }

        //Update bounding volumes
        this->animatedBounds.box.IncludePoints(&points[0], (int)points.size(), boundingBoxEmpty);
        boundingBoxEmpty = false;
        if (i == 0)
        {
            float radius;
            FinjinVector3::GetLongest(&points[0], (int)points.size(), &radius);
            if (this->animatedBounds.radius == 0 || radius > this->animatedBounds.radius)
                this->animatedBounds.radius = radius;
        }
    }
}

bool GeometryStateBase::RequiresDummyBone() const
{
    auto animationFlags = this->submeshes.empty() ? MeshAnimationFlags::NONE : this->submeshes.front()->submeshProperties.animationFlags;

    for (auto submesh : this->submeshes)
    {
        if (submesh->submeshProperties.requiresDummyBone)
            return true;
        if ((animationFlags & MeshAnimationFlags::HAS_SKELETON) !=
            (submesh->submeshProperties.animationFlags & MeshAnimationFlags::HAS_SKELETON))
            return true;
    }

    return false;
}

bool GeometryStateBase::HasSubmeshWithVertexElement(WxGpuVertexFormatStructMetadata::ElementID elementID) const
{
    for (auto submesh : this->submeshes)
    {
        for (auto& element : submesh->submeshProperties.vertexFormatElements)
        {
            if (element.elementID == elementID)
                return true;
        }
    }

    return false;
}

void GeometryStateBase::TransformToWorldSpace()
{
    MatrixAccessor transformMatrix, normalTransformMatrix;
    this->transform.MakeTransformMatrices(transformMatrix, normalTransformMatrix);

    //Transform vertices
    TransformVertices(transformMatrix, normalTransformMatrix);

    //Transform skeleton
    this->meshSkeleton.TransformToWorldSpace();

    //Transform the animation keys of root bones
    for (size_t skeletonAnimationIndex = 0; skeletonAnimationIndex < this->meshSkeletonAnimations.size(); skeletonAnimationIndex++)
    {
        auto skeletonAnimation = this->meshSkeletonAnimations[skeletonAnimationIndex].get();
        for (size_t boneIndex = 0; boneIndex < skeletonAnimation->boneAnimations.size(); boneIndex++)
        {
            auto boneAnimation = skeletonAnimation->boneAnimations[boneIndex].get();
            if (boneAnimation->bone->IsRoot())
            {
                for (size_t keyIndex = 0; keyIndex < boneAnimation->keys.size(); keyIndex++)
                {
                    auto newKey = boneAnimation->keys[keyIndex].GetTransform();
                    newKey.Concatenate(this->meshSkeleton.initialSkinTransform);
                    boneAnimation->keys[keyIndex].SetTransform(newKey); //TODO: Is this correct?
                }
            }
        }
    }

    this->transform.SetIdentity();
    this->geometryTransform.SetIdentity();
}

void GeometryStateBase::TransformVertices(const TransformAccessor& transform)
{
    MatrixAccessor transformMatrix, normalTransformMatrix;
    transform.MakeTransformMatrices(transformMatrix, normalTransformMatrix);
    TransformVertices(transformMatrix, normalTransformMatrix);
}

void GeometryStateBase::TransformVertices(const MatrixAccessor& transformMatrix, const MatrixAccessor& normalTransformMatrix)
{
    //Transform points array
    for (size_t i = 0; i < this->meshPositions.size(); i++)
        transformMatrix.TransformPoint(this->meshPositions[i]);

    //Transform submesh vertices and recalculate bounding volumes
    for (auto submesh : this->submeshes)
    {
        for (auto& vertex : submesh->vertexList)
        {
            transformMatrix.TransformPoint(vertex.position);
            normalTransformMatrix.TransformNormal(vertex.normal);
        }
    }

    //Transform bounds
    this->standardBounds.Transform(transformMatrix);
    this->animatedBounds.Transform(transformMatrix);

    //Transform morpher's vertices
    this->meshMorpher.TransformVertices(transformMatrix, normalTransformMatrix);

    //Transform morph animation vertices
    this->meshMorphAnimations.TransformVertices(transformMatrix, normalTransformMatrix);
}

bool GeometryStateBase::RemoveEmptySubmeshes()
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("GeometryStateBase::RemoveEmptySubmeshes()"));

    int removeCount = 0;

    for (auto submeshIterator = this->submeshes.begin(); submeshIterator != this->submeshes.end();)
    {
        auto submesh = submeshIterator->get();
        if (submesh->indices.empty() || submesh->vertexList.IsEmpty())
        {
            //Remove submesh from list and delete it (delete is automatic since it's a shared pointer)
            submeshIterator = submeshes.erase(submeshIterator);
            removeCount++;
        }
        else
            ++submeshIterator;
    }

    return removeCount > 0;
}

void GeometryStateBase::StartCreate
    (
    const wxString& meshName,
    ObjectAccessor object,
    const CoordinateSystemConverter& coordinateConverter,
    float scale,
    TimeAccessor time,
    GeometryStateFlags flags,
    const SkeletonReferencePose* referencePose
    )
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("GeometryStateBase::StartCreate(%s, %f, %f, %x)"), object.GetLocalName().wx_str(), scale, (float)time.GetSeconds(), (int)flags);

    this->meshName = meshName;
    this->createObject = object;
    this->createFlags = flags;

    this->transform.Set(object.GetNodeTransformation(time), coordinateConverter, scale);

    this->geometryTransform.Set(object.GetFullWorldTransformation(time), coordinateConverter, scale);

    if (NoneSet(flags & GeometryStateFlags::NO_SKELETON_CONTROL))
        this->meshSkeleton.Initialize(object, coordinateConverter, scale, referencePose);
    this->meshSkeleton.Enable(NoneSet(flags & GeometryStateFlags::NO_SKELETON_EFFECT));

    if (NoneSet(flags & GeometryStateFlags::NO_MORPHER_CONTROL))
        this->meshMorpher.Initialize(object, coordinateConverter, scale, this->submeshProperties.primitiveType);
    this->meshMorpher.Enable(NoneSet(flags & GeometryStateFlags::NO_MORPHER_EFFECT));
}

bool GeometryStateBase::FinishCreate
    (
    const GeometryStateSubmesh::SubmeshesTextureCoordSetDimensionArray& submeshesTextureCoordSetDimensions,
    std::vector<Vertex>& explodedVertices,
    std::vector<bool>& explodedEdgeVisibilities,
    int textureCoordinateSetIndexForTangents,
    std::vector<GeometryStateSubmesh*>& primitiveSubmeshes
    )
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("GeometryStateBase::FinishCreate(%s)"), this->createObject.GetLocalName().wx_str());

    //Convert exploded vertices back into optimized vertices
    if (this->geometryType == GeometryStateSubmesh::MESH && !explodedVertices.empty())
    {
        //Generate binormals on explodedVertices
        if (this->submeshProperties.generateBinormals && textureCoordinateSetIndexForTangents >= 0)
        {
            for (size_t index = 0; index < explodedVertices.size(); index++)
                explodedVertices[index].binormal = ((FinjinVector3&)explodedVertices[index].tangent).Cross(explodedVertices[index].normal) * explodedVertices[index].tangent.w;
        }

        int exportedTriangleVertexIndices[3];
        for (size_t explodedVertexIndex = 0; explodedVertexIndex < explodedVertices.size(); explodedVertexIndex++)
        {
            auto& explodedVertex = explodedVertices[explodedVertexIndex];

            auto submesh = primitiveSubmeshes[explodedVertex.originalFaceIndex];

            auto isNewVertex = false;
            auto actualVertexIndex = submesh->vertexList.Add(explodedVertex, &isNewVertex, AnySet(this->createFlags & GeometryStateFlags::FAST_VERTICES));
            submesh->indices.push_back(static_cast<uint32_t>(actualVertexIndex));
            if (isNewVertex)
            {
                submesh->addedOriginalIndices.push_back
                    (
                    GeometryStateSubmesh::OriginalIndices
                        (
                        explodedVertex.originalCornerIndex,
                        explodedVertex.originalFaceVertexIndex,
                        explodedVertex.originalFaceIndex,
                        explodedVertex.smoothingGroups
                        )
                    );
            }

            //Update reindex map
            auto& reindexedVertex = submesh->GetReindexedVertex(explodedVertex.originalFaceVertexIndex);
            if (std::find(reindexedVertex.begin(), reindexedVertex.end(), actualVertexIndex) == reindexedVertex.end())
                reindexedVertex.push_back(actualVertexIndex);

            exportedTriangleVertexIndices[explodedVertex.originalCornerIndex] = actualVertexIndex;

            //Set edge indices if we're on the last vertex of a face
            if (!explodedEdgeVisibilities.empty() && explodedVertex.originalCornerIndex == 2)
            {
                for (int edgeIndex = 0; edgeIndex < 3; edgeIndex++)
                {
                    if (explodedEdgeVisibilities[explodedVertexIndex + edgeIndex])
                    {
                        FinjinEdge edge(exportedTriangleVertexIndices[edgeIndex], exportedTriangleVertexIndices[(edgeIndex + 1) % 3]);
                        submesh->visibleEdges.insert(edge);
                    }
                }
            }

            //Initialize/update bounds
            UpdateBounds(explodedVertexIndex == 0, isNewVertex, explodedVertex.position);
        }
    }

    this->standardBounds.radius = sqrtf(this->standardBounds.radius);

    RemoveEmptySubmeshes();

    int submeshIndex = 0;
    for (auto submesh : this->submeshes)
    {
        //Set index again in case empty submeshes were removed
        submesh->index = submeshIndex;

        //Texture coordinate set dimensions
        if (!submeshesTextureCoordSetDimensions.empty())
            submesh->submeshProperties.SetTextureCoordinateSetDimensions(submeshesTextureCoordSetDimensions[submesh->index]);

        //Weighted vertex indexing. This is just the sequential ordering of the vertices
        if (this->meshSkeleton.IsValid())
        {
            for (size_t vertexIndex = 0; vertexIndex < submesh->GetReindexedVertexCount(); vertexIndex++)
            {
                if (vertexIndex < (int)this->meshSkeleton.weightedVertices.size())
                    submesh->GetReindexedVertex(vertexIndex).weightedVertex = this->meshSkeleton.weightedVertices[vertexIndex];
            }
        }

        submeshIndex++;
    }

    this->meshSkeleton.Restore();
    this->meshMorpher.Restore();

    this->meshSkeleton.FinishCreate();
    this->meshMorpher.FinishCreate();

    //Update bounds with morph targets
    if (this->meshMorpher.IsValid())
    {
        float localBoundingRadius = 0;
        for (auto morphTarget : this->meshMorpher.morphTargets)
        {
            for (size_t i = 0; i < morphTarget->creationPositions.size(); i++)
            {
                this->standardBounds.box.IncludePoint(morphTarget->creationPositions[i]);
                localBoundingRadius = std::max(localBoundingRadius, morphTarget->creationPositions[i].LengthSquared());
            }
        }
        localBoundingRadius = sqrtf(localBoundingRadius);

        this->standardBounds.radius = std::max(this->standardBounds.radius, localBoundingRadius);
    }

    this->animatedBounds = this->standardBounds;

    //Properly order the bones so that parent bones will be listed before their children
    BoneHierarchy boneHierarchy(this->meshSkeleton);
    boneHierarchy.GetOrderedBones(this->meshSkeleton.bones);

    return !this->submeshes.empty();
}

void GeometryStateBase::FinalizeVertexAndIndexBuffersForWrite(bool buildShared)
{
    //Convert submesh vertex and index buffers into common buffers within the geometry state itself

    //First build the index lists for each submesh, if necessary
    for (auto submesh : this->submeshes)
    {
        switch (submesh->submeshProperties.primitiveType)
        {
            case PrimitiveType::LINES:
            {
                if (submesh->geometryType == GeometryStateSubmesh::MESH)
                {
                    submesh->indices.reserve(submesh->visibleEdges.size() * 2);
                    for (auto& edge : submesh->visibleEdges)
                    {
                        submesh->indices.push_back(edge.e1);
                        submesh->indices.push_back(edge.e2);
                    }
                }

                break;
            }
            default: break;
        }
    }

    //TODO: Build the shared index/buffer lists
    if (buildShared)
    {
        this->vertexBuffers.reserve(this->submeshes.size());

        size_t sharedTotalIndexCount = 0;
        size_t sharedTotalVertexCount = 0;

        for (auto submesh : this->submeshes)
        {
            sharedTotalIndexCount += submesh->indices.size();
            sharedTotalVertexCount += submesh->vertexList.size();
        }

        auto use32BitIndexes = sharedTotalVertexCount > std::numeric_limits<uint16_t>::max();
        if (use32BitIndexes)
            this->indexBuffer32.reserve(sharedTotalIndexCount);
        else
            this->indexBuffer16.reserve(sharedTotalIndexCount);
        sharedTotalIndexCount = 0; //Reset to make it easier to set index buffer ranges

        for (auto submesh : this->submeshes)
        {
            auto bufferIndex = this->vertexBuffers.GetOrAddFormat(submesh->submeshProperties.vertexFormatName, submesh->submeshProperties.vertexFormatElements);
            auto& vertexBuffer = this->vertexBuffers[submesh->vertexBufferRange.bufferIndex];

            //Handle vertices
            submesh->vertexBufferRange.bufferIndex = bufferIndex;
            submesh->vertexBufferRange.startIndex = vertexBuffer.vertices.size();
            submesh->vertexBufferRange.elementCount = submesh->vertexList.size();

            if (vertexBuffer.vertices.IsEmpty())
                vertexBuffer.vertices.Reserve(sharedTotalIndexCount);
            auto vertexBufferStartIndex = vertexBuffer.vertices.size();
            vertexBuffer.vertices.FastAppend(submesh->vertexList);
            submesh->vertexList.Clear();

            //Handle indices
            if (!submesh->indices.empty())
            {
                submesh->indexBufferRange.bufferIndex = 0; //Always 0
                submesh->indexBufferRange.startIndex = sharedTotalIndexCount;
                submesh->indexBufferRange.elementCount = submesh->indices.size();

                if (use32BitIndexes)
                {
                    for (auto index : submesh->indices)
                        this->indexBuffer32.push_back(static_cast<uint32_t>(vertexBufferStartIndex + index));
                }
                else
                {
                    for (auto index : submesh->indices)
                        this->indexBuffer16.push_back(static_cast<uint16_t>(vertexBufferStartIndex + index));
                }

                sharedTotalIndexCount += submesh->indices.size();

                submesh->indices.clear();
            }
        }
    }
}

GeometryStateSubmesh* GeometryStateBase::GetMaterialSubmesh
    (
    MaterialsToSubmeshes& materialsToSubmeshes,
    MaterialAccessor material,
    bool* isNew
    )
{
    GeometryStateSubmesh* submesh = nullptr;

    //Look up existing submesh
    auto materialToSubmesh = materialsToSubmeshes.find(material);
    if (materialToSubmesh != materialsToSubmeshes.end())
    {
        //Use existing submesh
        submesh = materialToSubmesh->second;
        if (isNew != nullptr)
            *isNew = false;
    }
    else
    {
        //Create new submesh
        submesh = new GeometryStateSubmesh(material, this->submeshProperties);
        submesh->index = (int)this->submeshes.size();
        submesh->geometryType = this->geometryType;
        this->submeshes.push_back(GeometryStateSubmeshPtr(submesh));
        materialsToSubmeshes[material] = submesh;
        if (isNew != nullptr)
            *isNew = true;
    }

    return submesh;
}
