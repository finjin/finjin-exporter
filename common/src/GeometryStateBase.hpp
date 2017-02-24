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


//Includes---------------------------------------------------------------------
#include "GeometryStateSubmesh.hpp"
#include "ObjectAccessor.hpp"
#include "GeometryStateSubmeshProperties.hpp"
#include "MeshSkeleton.hpp"
#include "MeshMorpher.hpp"
#include "MeshMorphAnimation.hpp"
#include "FinjinBox.hpp"
#include "TextureCoordinateSetMappings.hpp"
#include "MeshNormals.hpp"
#include "VertexList.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class GeometryStateBase;
    typedef std::list<GeometryStateBase*> GeometryStateBaseList;

    enum class GeometryStateFlags
    {
        /** No flags. */
        NONE = 0,

        /** Indicates that if the mesh does not have vertex colors, white vertex colors should be created. */
        FORCE_WHITE_VERTEX_COLORS = 1 << 0,

        /** 
            * Indicates that the 'fast' method of determining vertices should be used.
            * This method does not perform duplicate vertex checking.
            */
        FAST_VERTICES = 1 << 1,

        /** 
            * Indicates that the morpher should be completely ignored.
            * This means that, if present, it is left in its active state.
            * Also, the meshMorpher member is not initialized.
            */
        NO_MORPHER_CONTROL = 1 << 2,

        /** 
            * Indicates that the skeleton should be completely ignored.
            * This means that, if present, it is left in its active state.
            * Also, the meshSkeleton member is not initialized.
            */
        NO_SKELETON_CONTROL = 1 << 3,

        /** 
            * Indicates morpher should be disabled when evaluating the geometry.
            * The meshMorpher member is still initialized, however.
            */
        NO_MORPHER_EFFECT = 1 << 4,

        /** 
            * Indicates skeleton should be disabled when evaluating the geometry.
            * The meshSkeleton member is still initialized, however.
            */
        NO_SKELETON_EFFECT = 1 << 5,

        /**
            * Indicates that bones that don't influence any vertices should be removed.
            * This is equivalent to calling meshSkeleton.RemoveBonesWithNoVertexInfluence().
            */
        REMOVE_BONES_WITH_NO_VERTEX_INFLUENCE = 1 << 6,

        /**
            * Indicates that no extra materials should be produced when exporting the mesh.
            * The only materials that are produced, are the ones that are applied.
            */
        NO_EXTRA_MATERIALS = 1 << 7
    };
    FINJIN_ENUM_BITWISE_OPERATIONS(GeometryStateFlags)

    /**
     * Base geometry class that collects the data and methods common to all the
     * application-specific GeometryState classes.
     */
    class GeometryStateBase
    {
    public:
        /** Constructor. */
        GeometryStateBase();

        /** Destructor. */
        virtual ~GeometryStateBase();

        /** 
         * Creates the geometry state.
         * @param object [in] - The object whose geometry state is extracted.
         * @param coordinateConverter [in] - The coordinate converter used to convert points, normals, and transformations.
         * @param scale [in] - The scale applied to points, skeleton bones, and transformations.
         * @param time [in] - The time at which the geometry state is evaluated.
         * @param flags [in] - Creation flags.
         * @param submeshesSettings [in] - Custom submesh texture coordinate set mappings.
         * If not specified, the default mappings are used.
         */
        virtual bool Create
            (
            const wxString& meshName,
            ObjectAccessor object, 
            const CoordinateSystemConverter& coordinateConverter,
            float scale = 1.0f,
            TimeAccessor time = TimeAccessor(), 
            GeometryStateFlags flags = GeometryStateFlags::NONE,
            SubmeshesSettings* submeshesSettings = nullptr,
            const SkeletonReferencePose* referencePose = nullptr
            ) = 0;

        /**
         * Samples all the geometry points.
         * @param points [out] - The collection of sample points.
         * @param coordinateConverter [in] - The coordinate converter used to convert points.
         * @param scale [in] - The scale applied to points.
         * @param time [in] - The time at which the geometry points are evaluated.
         * @param flags [in] - The following flags are supported: NO_SKELETON_EFFECT, NO_MORPHER_EFFECT.
         * @return If the geometry was sampled, true is returned. Otherwise, false is returned.
         */
        virtual bool SamplePoints
            (
            std::vector<FinjinVector3>& points, 
            const CoordinateSystemConverter& coordinateConverter,
            float scale = 1.0f,
            TimeAccessor time = TimeAccessor(), 
            GeometryStateFlags flags = GeometryStateFlags::NONE,
            MeshNormals* normals = nullptr
            ) = 0;

        /** Clears all members. */
        void Clear();

        /** 
         * Merges the specified geometry (vertices, skeletons, morphers, and animations) with this one.
         * @param mergeGeometry [in] - The geometry to merge into this one. After the merge is complete,
         * mergeGeometry should not be used again since its contents are modified during the merge process.
         * @param alwaysCreateNewSubmeshes [in] - Indicates whether new submeshes should be created
         * when merging. If false, an attempt will be made to merge submeshes into existing submeshes.
         */
        void Merge(GeometryStateBase& mergeGeometry, bool alwaysCreateNewSubmeshes);

        /** 
         * Merges the specified list of geometries with this one.
         * @param mergeGeometries [in/out] - List of geometries to merge into this one. After the merge is complete,
         * the objects in this list are deleted and the list is cleared.
         * @param alwaysCreateNewSubmeshes [in] - Indicates whether new submeshes should be created
         * when merging. If false, an attempt will be made to merge submeshes into existing submeshes.
         */
        void Merge(GeometryStateBaseList& mergeGeometries, bool alwaysCreateNewSubmeshes);

        /** 
         * Determines if the specified geometry is an instance of this geometry. 
         * The instance checking is done by comparing the state of the two geometries.
         * @param other [in] - The geometry to compare to.
         * @param tolerance [in] - A tolerance value used when comparing the vertices and bounding volumes 
         * of meshes. A larger tolerance is more likely to cause vertices and bounding volumes to match.
         * @return If this geometry and the other geometry are instances of one another, true is
         * returned. Otherwise, false is returned.
         */
        bool IsMatchingInstance(const GeometryStateBase& other, float tolerance);

        /** 
         * Transforms the skeletons, morph targets, animations, vertices and all related settings (like normals and bounding volumes) to world space.
         */
        void TransformToWorldSpace();
        
        /** 
         * Transforms the vertices and all related settings (like normals and bounding volumes) using the specified transform.
         */
        void TransformVertices(const TransformAccessor& transform);

        /** 
         * Updates various settings related to mesh animation. 
         * This is to be called after modifying the mesh animation members.
         * @param forceSkeleton [in] - Indicates that the skeleton should be considered in
         * updating the animation data, even if there are no skeleton animations.
         * @param forceMorpher [in] - Indicates that the morpher should be considered in
         * updating the animation data, even if there are no morpher animations.
         */
        void UpdateAnimationData(bool forceSkeleton, bool forceMorpher);

        struct SubmeshPoints
        {
            std::vector<SampledMeshPoint> points;
        };

        /** Points for all submeshes. */
        typedef std::vector<SubmeshPoints> SubmeshesPoints;

        /**
         * Samples all the geometry points, with the points segmented by submesh.
         * @param submeshesPoints [out] - The collection of sample points, segmented by submesh.
         * @param coordinateConverter [in] - The coordinate converter used to convert points.
         * @param scale [in] - The scale applied to points.
         * @param time [in] - The time at which the geometry points are evaluated.
         * @param flags [in] - The following flags are supported: NO_SKELETON_EFFECT, NO_MORPHER_EFFECT.
         * @return If the geometry was sampled, true is returned. Otherwise, false is returned.
         */
        bool SampleSubmeshPoints
            (
            SubmeshesPoints& submeshesPoints, 
            const CoordinateSystemConverter& coordinateConverter,
            float scale = 1.0f,
            TimeAccessor time = TimeAccessor(), 
            GeometryStateFlags flags = GeometryStateFlags::NONE
            );

        /** 
         * Adjusts the bounding volumes to fit the object sampled at the specified times.
         * @param times [in] - Collection of times at which geometry is sampled.
         * @param coordinateConverter [in] - The coordinate converter used to convert points.
         * @param scale [in] - The scale applied to points.
         * @param transform [in] - Additional transform to apply to sampled points. The transform is applied
         * after the root object's translation is subtracted.
         * @param rootObject [in] - An object whose translation is subtracted from the sampled points.
         */
        void AnimateBoundingVolumes
            (
            const std::vector<WxTimeDuration>& times, 
            const CoordinateSystemConverter& coordinateConverter, 
            float scale,
            const TransformAccessor* transform = nullptr,
            ObjectAccessor rootObject = ObjectAccessor()
            );

        /** 
         * Indicates whether the geometry requires a dummy bone. 
         * A dummy bone is needed in situations where there's a mesh skeleton but some vertices don't have 
         * bone assignments.
         */
        bool RequiresDummyBone() const;
        
        bool HasSubmeshWithVertexElement(WxGpuVertexFormatStructMetadata::ElementID elementID) const;

        void FinalizeVertexAndIndexBuffersForWrite(bool buildShared);
        
    protected:
        void TransformToWorldSpace(const MatrixAccessor& transformMatrix, const MatrixAccessor& normalTransformMatrix);

        /** 
         * Performs the actual transformation of vertices. 
         * @param transformMatrix [in] - The matrix used to transform vertices.
         * @param normalTransformMatrix [in] - The matrix used to transform normals.
         */
        void TransformVertices(const MatrixAccessor& transformMatrix, const MatrixAccessor& normalTransformMatrix);

        /**
         * Finds a submesh that is capable of having the specified geometry's submesh merged into it.
         * @param mergeGeometry [in] - The owner of the submesh parameter.
         * @param submesh [in] - The submesh to be merged.
         * @return If a submesh can be found in this geometry that can merge the specified submesh into it,
         * it is returned. Otherwise, null is returned.
         */
        GeometryStateSubmesh* FindSubmeshForMerge
            (
            const GeometryStateBase& mergeGeometry, 
            const GeometryStateSubmesh* submesh
            );

        /**
         * Removes any submeshes that contain no vertices.
         * @return If one or more empty submeshes are removed, true is returned. Otherwise, false is returned.
         */
        bool RemoveEmptySubmeshes();

        /**
         * Updates the bounding volumes.
         * @param isFirst [in] - Indicates whether this is the first time UpdateBounds() is being called.
         * If true, this resets the bounding volumes.
         * @param isNew [in] - Indicates whether the specified point is a newly added point.
         * @param point [in] - A point in the geometry that may modify the bounding volume.
         */
        void UpdateBounds(bool isFirst, bool isNew, const FinjinVector3& point);
        
        /**
         * Called by derived classes when starting the creation of the geometry state data.
         * @param object [in] - The object that contains the geometry data to extract.
         * @param coordinateConverter [in] - The coordinate converter used to convert points and normals.
         * @param scale [in] - The scale applied to points.
         * @param time [in] - The time at which the geometry points are evaluated.
         * @param flags [in] - Flags controlling the geometry creation.
         */
        void StartCreate
            (
            const wxString& meshName,
            ObjectAccessor object, 
            const CoordinateSystemConverter& coordinateConverter, 
            float scale, 
            TimeAccessor time,
            GeometryStateFlags flags,
            const SkeletonReferencePose* referencePose
            );

        /**
         * Called by derived classes when starting the creation of the geometry state data.
         * @param submeshesTextureCoordSetDimensions [in] - A collection of dimensions for all the texture coordinate
         * sets of all submeshes.
         * @return If the geometry state is valid (there is at least one submesh), true is returned. Otherwise,
         * false is returned.
         */
        bool FinishCreate
            (
            const GeometryStateSubmesh::SubmeshesTextureCoordSetDimensionArray& submeshesTextureCoordSetDimensions,
            std::vector<Vertex>& explodedVertices,
            std::vector<bool>& explodedEdgeVisibilities,
            int textureCoordinateSetIndexForTangents,
            std::vector<GeometryStateSubmesh*>& primitiveSubmeshes
            );

        /** Maps a material to a submesh. */
        typedef AccessorMap<MaterialAccessor, GeometryStateSubmesh*> MaterialsToSubmeshes;

        /** 
         * Gets the submesh for the specified material, creating the submesh if necessary.
         * @param materialsToSubmeshes [in/out] - A material/submesh map. This is first checked
         * to see if the necessary submesh exists. It is modified if a new submesh is created.
         * @param material [in] - The material for which a submesh should be retrieved.
         * @param isNew [out] - Pointer to a boolean that receives a value indicating whether the
         * returned submesh was newly created.
         * @return The submesh corresponding to the specified material is returned.
         */
        GeometryStateSubmesh* GetMaterialSubmesh
            (
            MaterialsToSubmeshes& materialsToSubmeshes, 
            MaterialAccessor material, 
            bool* isNew = nullptr
            );
            
    public:
        /** Name passed to StartCreate(). */
        wxString meshName;

        /** Object passed to StartCreate(). */
        ObjectAccessor createObject;

        /** Flags passed to StartCreate(). */
        GeometryStateFlags createFlags;

        /** The object's node transform at creation time. */
        TransformAccessor transform;

        /** The object's node transform with object offset at creation time. */
        TransformAccessor geometryTransform;

        /** All the geometry submeshes. */ 
        GeometryStateSubmeshes submeshes;

        /** 
         * Vertex positions, as they are ordered in the original mesh.
         * This is used with reindexing and also when initializing morphers.
         */
        std::vector<FinjinVector3> meshPositions;

        /** Mesh normals, as they exist in the original mesh. */
        MeshNormals meshNormals;

        /** 
         * The default mapping of source (user/application-defined) texture coordinates 
         * to what will eventually be exported. 
         */
        SubmeshesSettings submeshesSettings;

        /**
         * Properties that are applied to all created submeshes.
         */
        GeometryStateSubmeshProperties submeshProperties;

        /** The mesh skeleton. */
        MeshSkeleton meshSkeleton;

        /** Animations for the mesh skeleton. */
        MeshSkeleton::Animations meshSkeletonAnimations;

        /** The mesh morpher. */
        MeshMorpher meshMorpher;

        /** Animations for the mesh morpher. */
        MeshMorpher::Animations meshMorpherAnimations;

        /** Morph animations. */
        MeshMorphAnimations meshMorphAnimations;

        struct ProcessedLocalBounds
        {
            float radius;
            FinjinBox3 box;
            FinjinBox3 cube;
        };

        struct LocalBounds
        {
            LocalBounds()
            {
                this->radius = 0;
            }

            ProcessedLocalBounds GetProcessed(float scale) const
            {
                ProcessedLocalBounds result;

                auto boxHalfSize = (this->box.max - this->box.min) / 2.0f;
                auto boxCenter = (this->box.max + this->box.min) / 2.0f;
                boxHalfSize *= scale;
                
                result.box.min = boxCenter - boxHalfSize;
                result.box.max = boxCenter + boxHalfSize;

                auto biggestSide = std::max(boxHalfSize.x, std::max(boxHalfSize.y, boxHalfSize.z));
                boxHalfSize.x = boxHalfSize.y = boxHalfSize.z = biggestSide;
                result.cube.min = boxCenter - boxHalfSize;
                result.cube.max = boxCenter + boxHalfSize;

                result.radius = this->radius * scale;

                return result;
            }

            bool operator == (const LocalBounds& other) const
            {
                return this->radius == other.radius && this->box == other.box;
            }

            bool operator != (const LocalBounds& other) const
            {
                return !(operator == (other));
            }

            void Reset()
            {
                this->radius = 0;
                this->box.Reset();
            }

            void Transform(const MatrixAccessor& transformMatrix)
            {
                FinjinVector3 corners[8];
                this->box.GetCornerPoints(corners);
                for (size_t i = 0; i < 8; i++)
                    transformMatrix.TransformPoint(corners[i]);
                this->box.IncludePoints(corners, 8, true);

                //Transform bounding radius
                //This calculation isn't quite right but it's close enough
                FinjinBox3 sphereBox(this->radius * 2);
                sphereBox.GetSpherePoints(corners);
                for (size_t i = 0; i < 6; i++)
                    transformMatrix.TransformPoint(corners[i]);
                FinjinVector3::GetLongest(corners, 6, &this->radius);
            }

            void MergeFrom(GeometryStateBase& mergeGeometry, MatrixAccessor& geometryTransformMatrix)
            {
                FinjinVector3 corners[8];
                mergeGeometry.standardBounds.box.GetCornerPoints(corners);
                for (size_t i = 0; i < 8; i++)
                    geometryTransformMatrix.TransformPoint(corners[i]);
                this->box.IncludePoints(corners, 8);

                //Merge spheres
                //This calculation isn't quite right but it works well enough
                this->radius = this->box.GetHalfSize().GetMaxComponent();
            }

            float radius;
            FinjinBox3 box;
        };

        LocalBounds standardBounds;
        LocalBounds animatedBounds;

        /** Constructed in FinalizeVertexAndIndexBuffersForWrite(). The destination for all submesh vertices. */
        struct VertexBuffer
        {
            wxString formatName;
            std::vector<WxGpuVertexFormatStruct::Element> vertexFormatElements;
            VertexList vertices;
        };
        struct VertexBuffers : public std::vector<VertexBuffer>
        {
            size_t GetOrAddFormat(const wxString& formatName, const std::vector<WxGpuVertexFormatStruct::Element>& other)
            {
                auto containsFormat = false;
                size_t index = 0;
                for (auto& buffer : *this)
                {
                    if (WxGpuVertexFormatStruct::Element::ElementsSame(buffer.vertexFormatElements, other))
                        return index;
                    index++;
                }

                VertexBuffer emptyBuffer;
                push_back(emptyBuffer);
                back().formatName = formatName;
                back().vertexFormatElements = other;

                return index;
            }
        };
        VertexBuffers vertexBuffers;
        
        /** Constructed in FinalizeVertexAndIndexBuffersForWrite(). The destination for all submesh indices. */
        std::vector<uint16_t> indexBuffer16;
        std::vector<uint32_t> indexBuffer32;
        
    protected:
        /** The type of geometry extracted from createObject. */
        GeometryStateSubmesh::GeometryType geometryType;
    };

} }
