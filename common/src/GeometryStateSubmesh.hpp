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
#include "MaterialAccessor.hpp"
#include "VertexList.hpp"
#include "GeometryStateSubmeshProperties.hpp"
#include "ReindexedVertex.hpp"
#include "AssetReference.hpp"
#include "FinjinEdge.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class MeshSkeleton;

    /**
     * A GeometryState submesh.
     */
    class GeometryStateSubmesh
    {
    public:
        /** This constructor should not be used. */
        GeometryStateSubmesh() {}

        /**
         * Main constructor.
         * @param material [in] - The material that this submesh uses.
         * @param submeshProperties [in] - The properties that this submesh inherits.
         */
        GeometryStateSubmesh(MaterialAccessor material, const GeometryStateSubmeshProperties& submeshProperties);

        /**
         * Called on an already initialized submesh in preparation for being merged and added to another GeometryState.
         * @param index [in] - The new index for the submesh.
         * @param reindexingPrefixLength [in] - The number of unused reindexing entries at the beginning of the
         * reindexedVertices collection.
         */
        void InitializeForMerge(int index, size_t reindexingPrefixLength);

        /**
         * Gets the submesh indices.
         * @param indices [out] - An array of indices. This array is assumed to be big enough to store the index values.
         * @param The number of indices copied is returned.
         */
        void GetIndices(std::vector<uint32_t>& indices) const;

        /**
         * Gets the submesh indices.
         * @param indices [out] - An array of indices. This array is assumed to be big enough to store the index values.
         * @param The number of indices copied is returned.
         */
        void GetIndices(std::vector<uint16_t>& indices) const;

        /** Determines whether this submesh contains the specified vertex. */
        bool ContainsVertex(int vertexIndex) const;

        size_t GetReindexedVertexCount() const;
        void Reserve(size_t count);
        ReindexedVertex& GetReindexedVertex(size_t i);
        const ReindexedVertex& GetReindexedVertexConst(size_t i) const;

        void BuildVertexBoneAssignments(MeshSkeleton& meshSkeleton);

    public:
        /** The material used by this submesh. */
        MaterialAccessor material;

        /** The type of geometry contained in this submesh. */
        enum GeometryType
        {
            NO_GEOMETRY_TYPE,
            MESH,
            SHAPE
        };
        GeometryType geometryType;

        /** All the vertices in the submesh. */
        VertexList vertexList;

        /** All the indices in the submesh */
        std::vector<uint32_t> indices;

        struct OriginalIndices
        {
            /* The index of the vertex in the face (0, 1, 2). */
            unsigned int cornerIndex;

            /* The index of the vertex in the mesh. */
            unsigned int vertexIndex;

            /* The index of the face in the mesh that contains the vertex. */
            unsigned int faceIndex;

            unsigned int smoothingGroups;

            OriginalIndices()
            {
            }

            OriginalIndices
                (
                unsigned int cornerIndex,
                unsigned int vertexIndex,
                unsigned int faceIndex,
                unsigned int smoothingGroups
                )
            {
                this->cornerIndex = cornerIndex;
                this->vertexIndex = vertexIndex;
                this->faceIndex = faceIndex;
                this->smoothingGroups = smoothingGroups;
            }
        };

        /**
         * This array contains the original index for each entry in vertexList.
         * Only used when geometryType is MESH.
         * When sampling an a mesh to generate morphs animations, the sampled meshes can be resegmented
         * by using this array.
         */
        std::vector<OriginalIndices> addedOriginalIndices;

    private:
        /** Indicates how many of the first entries in reindexedVertices are empty. */
        size_t reindexingPrefixLength;

        size_t reindexedVertexCount;

        /**
         * Maps source vertex positions to many vertices in the submesh.
         * Only used when geometryType is MESH.
         */
        ReindexedVertices reindexedVertices;

    public:
        /**
         * The number of primitives in the submesh.
         * If geometryType is MESH, this is the number of faces.
         * If geometryType is SHAPE, this is the number of segments.
         */
        int primitiveCount;

        /** The zero-based submesh index. */
        int index;

        /** Used only during export. Stores index into another array that contains mapping information. */
        int mappingIndex;

        /** The explicit submesh name. May be empty. */
        wxString name;

        /** The visible edges of the submesh. Used only when the geometryType is MESH. */
        FinjinEdgeSet visibleEdges;

        /** Various submesh properties. */
        GeometryStateSubmeshProperties submeshProperties;

        /** An array of texture coordinate set dimensions. */
        typedef std::vector<unsigned short> TextureCoordSetDimensionArray;

        /** An array of texture coordinate set dimensions for each submesh. */
        typedef std::vector<TextureCoordSetDimensionArray> SubmeshesTextureCoordSetDimensionArray;

        struct VertexBoneAssignment
        {
            uint32_t vertexIndex;
            uint32_t boneIndex;
            float weight;

            VertexBoneAssignment(uint32_t vertexIndex, uint32_t boneIndex, float weight)
            {
                this->vertexIndex = vertexIndex;
                this->boneIndex = boneIndex;
                this->weight = weight;
            }
        };
        std::vector<VertexBoneAssignment> vertexBoneAssignments;

        struct BufferRange
        {
            BufferRange()
            {
                this->bufferIndex = 0;
                this->startIndex = 0;
                this->elementCount = 0;
            }

            bool empty() const
            {
                return this->elementCount == 0;
            }

            size_t bufferIndex;
            size_t startIndex;
            size_t elementCount;
        };

        typedef BufferRange IndexBufferRange;
        typedef BufferRange VertexBufferRange;

        IndexBufferRange indexBufferRange;
        VertexBufferRange vertexBufferRange;
    };

    typedef std::shared_ptr<GeometryStateSubmesh> GeometryStateSubmeshPtr;
    typedef std::list<GeometryStateSubmeshPtr> GeometryStateSubmeshes;

} }
