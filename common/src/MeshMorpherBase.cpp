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
#include "MeshMorpherBase.hpp"
#include "GeometryStateBase.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
MeshMorpherBase::MeshMorpherBase(GeometryStateBase* geometryState)
{
    this->geometryState = geometryState;
}

MeshMorpherBase::~MeshMorpherBase()
{
    Clear();
}

void MeshMorpherBase::Clear()
{
    this->morphTargets.clear();
}

void MeshMorpherBase::GetMorphTargetNames(std::set<wxString>& names)
{
    for (auto morphTarget : this->morphTargets)
        names.insert(morphTarget->name);
}

void MeshMorpherBase::AddMorphTarget(MeshMorphTargetPtr morphTargetIterator)
{
    this->morphTargets.push_back(morphTargetIterator);
}

void MeshMorpherBase::Merge(MeshMorpherBase& mergeMorpher, const MatrixAccessor& transformMatrix, const MatrixAccessor& normalTransformMatrix)
{
    //Merge in the points
    for (auto morphTarget : this->morphTargets)
    {
        morphTarget->Transform(transformMatrix, normalTransformMatrix);
        this->morphTargets.push_back(morphTarget);
    }

    mergeMorpher.morphTargets.clear();
}

void MeshMorpherBase::FinishCreate()
{
    FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("MeshMorpherBase::FinishCreate()"));

    if (!this->morphTargets.empty() && this->geometryState != nullptr && !this->geometryState->meshPositions.empty())
    {
        for (auto morphTarget : this->morphTargets)
        {
            //Segment the morph target into subtargets - one for each submesh
            morphTarget->subtargets.resize(this->geometryState->submeshes.size());
            for (auto submesh : this->geometryState->submeshes)
            {
                auto& subtarget = morphTarget->subtargets[submesh->index];
                subtarget.reset(new MeshMorphTarget::Subtarget);
                subtarget->submesh = submesh.get();
                subtarget->offsets.reserve(submesh->indices.size());

                for (size_t vertexIndex = 0; vertexIndex < this->geometryState->meshPositions.size(); vertexIndex++)
                {
                    if (submesh->ContainsVertex((int)vertexIndex))
                    {
                        auto& reindexedVertex = submesh->GetReindexedVertexConst(vertexIndex);
                        for (size_t reindexed = 0; reindexed < reindexedVertex.size(); reindexed++)
                        {
                            auto& originalIndices = submesh->addedOriginalIndices[reindexedVertex[reindexed]];
                            MeshMorphTarget::Offset offset
                                (
                                reindexedVertex[reindexed],
                                morphTarget->creationPositions[vertexIndex],
                                morphTarget->creationNormals.GetNormal(originalIndices.cornerIndex, originalIndices.vertexIndex, originalIndices.faceIndex, originalIndices.smoothingGroups),
                                morphTarget->creationNormals.GetTangent(originalIndices.cornerIndex, originalIndices.vertexIndex, originalIndices.faceIndex, originalIndices.smoothingGroups),
                                this->geometryState->meshPositions[vertexIndex],
                                this->geometryState->meshNormals.GetNormal(originalIndices.cornerIndex, originalIndices.vertexIndex, originalIndices.faceIndex, originalIndices.smoothingGroups),
                                this->geometryState->meshNormals.GetTangent(originalIndices.cornerIndex, originalIndices.vertexIndex, originalIndices.faceIndex, originalIndices.smoothingGroups)
                                );
                            subtarget->offsets.push_back(offset);
                        }
                    }
                }
            }
        }
    }
}

void MeshMorpherBase::TransformVertices(const MatrixAccessor& transformMatrix, const MatrixAccessor& normalTransformMatrix)
{
    for (auto morphTarget : this->morphTargets)
        morphTarget->Transform(transformMatrix, normalTransformMatrix);
}

void MeshMorpherBase::SampleMorphTargetInfluences(MorphTargetInfluences& targetInfluences, TimeAccessor sampleTime, bool forceAllTargets)
{
    targetInfluences.reserve(this->morphTargets.size());

    for (auto morphTarget : this->morphTargets)
    {
        MorphTargetInfluence targetInfluence;
        targetInfluence.target = morphTarget.get();
        targetInfluence.influence = targetInfluence.target->GetInfluence(sampleTime);
        if (forceAllTargets || targetInfluence.influence > 0)
            targetInfluences.push_back(targetInfluence);
    }
}
