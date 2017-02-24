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
#include "GeometryStateSubmesh.hpp"
#include "MeshSkeleton.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
GeometryStateSubmesh::GeometryStateSubmesh(MaterialAccessor material, const GeometryStateSubmeshProperties& submeshProperties)
{
    this->material = material;
    this->submeshProperties = submeshProperties;
    this->geometryType = NO_GEOMETRY_TYPE;
    this->reindexingPrefixLength = 0;
    this->reindexedVertexCount = 0;
    this->primitiveCount = 0;
    this->index = -1;        
    this->mappingIndex = -1;
}

void GeometryStateSubmesh::InitializeForMerge(int index, size_t reindexingPrefixLength)
{
    this->reindexingPrefixLength = reindexingPrefixLength;    
    this->reindexedVertexCount += reindexingPrefixLength;
    this->index = index;
}

void GeometryStateSubmesh::GetIndices(std::vector<uint32_t>& indices) const
{
    indices.resize(this->indices.size());
    memcpy(indices.data(), this->indices.data(), this->indices.size() * sizeof(uint32_t));
}

void GeometryStateSubmesh::GetIndices(std::vector<uint16_t>& indices) const
{
    indices.resize(this->indices.size());
    for (size_t i = 0; i < this->indices.size(); i++)
        indices[i] = (uint16_t)this->indices[i];        
}

bool GeometryStateSubmesh::ContainsVertex(int vertexIndex) const
{
    return !GetReindexedVertexConst(vertexIndex).empty();
}

size_t GeometryStateSubmesh::GetReindexedVertexCount() const
{
    return this->reindexedVertexCount;
}

void GeometryStateSubmesh::Reserve(size_t count)
{
    this->vertexList.Reserve(count);
    this->reindexedVertexCount = count;    
}

ReindexedVertex& GeometryStateSubmesh::GetReindexedVertex(size_t i)
{
    static ReindexedVertex nullReindex;
    
    if (i >= this->reindexingPrefixLength && i < this->reindexedVertexCount)
    {
        size_t optimizedCount = this->reindexedVertexCount - this->reindexingPrefixLength;
        if (optimizedCount > this->reindexedVertices.size())
            this->reindexedVertices.resize(optimizedCount);

        i -= this->reindexingPrefixLength;
        return this->reindexedVertices[i];
    }
    else
    {
        nullReindex.clear();
        return nullReindex;
    }
}

const ReindexedVertex& GeometryStateSubmesh::GetReindexedVertexConst(size_t i) const
{
    static const ReindexedVertex nullReindex;
    if (i >= this->reindexingPrefixLength && i < this->reindexedVertexCount)
    {
        i -= this->reindexingPrefixLength;
        return (i < this->reindexedVertices.size()) ? this->reindexedVertices[i] : nullReindex;
    }
    else
        return nullReindex;
}

void GeometryStateSubmesh::BuildVertexBoneAssignments(MeshSkeleton& meshSkeleton)
{
    if (!this->vertexBoneAssignments.empty())
        return;

    struct OutputVBA
    {
        std::set<uint16_t> boneIndices;
    };

    //const size_t boneCount = geometryState->meshSkeleton.bones.size();
                    
    //This keeps track of the output vertex/bone assignments. 
    //This is necessary since removing vertex duplicates (done at an earlier stage) can result in output assignments mapping to the same vertex index/bone index pair.
    std::vector<OutputVBA> outputVbas;
    outputVbas.resize(this->vertexList.size());

    for (size_t vertexIndex = 0; vertexIndex < this->GetReindexedVertexCount(); vertexIndex++)
    {
        if (ContainsVertex((int)vertexIndex))                        
        {
            auto& reindexedVertex = GetReindexedVertexConst(vertexIndex);
            auto& weightedVertex = reindexedVertex.weightedVertex;
            if (!weightedVertex.assignments.empty())
            {
                for (size_t assignmentIndex = 0; assignmentIndex < weightedVertex.assignments.size(); assignmentIndex++) 
                {
                    auto& assignment = weightedVertex.assignments[assignmentIndex];                            

                    auto boneIndex = meshSkeleton.GetBoneIndex(assignment.bone);
                    for (size_t reindexed = 0; reindexed < reindexedVertex.size(); reindexed++)
                    {
                        auto vertexIndex = reindexedVertex[reindexed];                                    
                        if (outputVbas[vertexIndex].boneIndices.find(boneIndex) == outputVbas[vertexIndex].boneIndices.end())
                        {
                            this->vertexBoneAssignments.push_back(VertexBoneAssignment(vertexIndex, boneIndex, assignment.weight));

                            outputVbas[vertexIndex].boneIndices.insert(boneIndex);
                        }
                    }
                }
            }
            else
            {
                //No weights for this vertex
                //LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Vertex %d of submesh %d has no bone assignment. Assigning to dummy bone."), vertexIndex, submeshState->index);
            }
        }
    }
}
