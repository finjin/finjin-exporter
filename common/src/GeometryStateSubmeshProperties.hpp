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
#include "ObjectExportSettings.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "FinjinObjectSettingsAccessor.hpp"
#include "FinjinMeshAnimationSettingsAccessor.hpp"
#include "AnimationUtilities.hpp"
#include "finjin/common/WxNumericStruct.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    enum class MeshAnimationFlags
    {
        NONE = 0,

        /** Indicates the submesh is associated with a skeleton */
        HAS_SKELETON = 1 << 0,

        /** Indicates the submesh is associated with a morpher */
        HAS_MORPHER = 1 << 1,

        /** Indicates the submesh is associated with a morph */
        HAS_MORPHS = 1 << 2
    };
    FINJIN_ENUM_BITWISE_OPERATIONS(MeshAnimationFlags)

    /** Various submesh properties collected for easy copying. */
    class GeometryStateSubmeshProperties
    {
    public:
        /**
         * Constructor.
         * Note that this constructor does nothing, so Initialize() must be called.
         */
        GeometryStateSubmeshProperties();

        /** 
         * Initializes the submesh properties.
         * @param object [in] - The object that the submesh is derived from.
         * @param objectSettings [in] - Settings for the object.
         * @param sceneSettings [in] - The scene settings.
         */
        void Initialize
            (
            ObjectAccessor object,
            FinjinObjectSettingsAccessor objectSettings, 
            FinjinSceneSettingsAccessor sceneSettings
            );

        /** 
         * Initializes the submesh properties.
         * @param object [in] - The object that the submesh is derived from.
         * @param objectSettings [in] - Settings for the object.
         * @param objectExportSettings [in] - Export settings for the object.
         * @param sceneSettings [in] - The scene settings.
         */
        void Initialize
            (
            ObjectAccessor object,
            FinjinObjectSettingsAccessor objectSettings, 
            const ObjectExportSettings& objectExportSettings,
            FinjinSceneSettingsAccessor sceneSettings
            );

        /** 
         * Initializes the submesh properties.
         * @param object [in] - The object that the submesh is derived from.
         * @param objectSettings [in] - Settings for the object.
         * @param objectExportSettings [in] - Export settings for the object. This may be null.
         * @param sceneSettings [in] - The scene settings.
         */
        void Initialize
            (
            ObjectAccessor object,
            FinjinObjectSettingsAccessor objectSettings, 
            const ObjectExportSettings* objectExportSettings,
            FinjinSceneSettingsAccessor sceneSettings
            );

        /** 
         * Merges other submesh properties into this set of properties.
         * Before calling this, you must make sure that merging is valid by calling MatchesForMerge().
         * @param other [in/out] - The other properties to merge into this set of properties. This might have some of its
         * contents modified too if necessary.
         */
        void Merge(GeometryStateSubmeshProperties& other);

        /** 
         * Determines whether the specified properties can be merged into this set of properties.
         */ 
        bool MatchesForMerge(const GeometryStateSubmeshProperties& other);

        /** 
         * Determines whether the specified properties appear to be an instance of this set of properties.
         */ 
        bool MatchesForInstancing(const GeometryStateSubmeshProperties& other);

        /** Indicates whether vertex colors are exportable. */
        bool CanExportVertexColors() const;

        /** Indicates whether tangents can be generated. */
        bool CanGenerateTangents() const;

        /** Indicates whether binormals can be generated. */
        bool CanGenerateBinormals() const;

        typedef std::vector<unsigned short> TextureCoordSetDimensionArray;

        /** Sets the dimensions for the texture coordinate sets in the submesh. */         
        void SetTextureCoordinateSetDimensions(const TextureCoordSetDimensionArray& textureCoordSetDimensions);

        /** Indicates whether the submesh is animated in some way. */
        bool IsAnimated() const;

        /** 
         * Indicates whether the submesh has an animation type that can be merged with another submesh. 
         * @param other [in] - The properties of a submesh to potentially merge with.
         */
        bool HasMergableAnimation(const GeometryStateSubmeshProperties& other) const;

        //ShaderGeneratorState::RenderingFlags GetShaderRequirements();

    public:
        FinjinObjectSettingsAccessor objectSettings;
        PrimitiveType primitiveType;
        float pointSize;
        wxString vertexFormatName;
        std::vector<WxGpuVertexFormatStruct::Element> vertexFormatElements;
        bool exportVertexColors;        
        bool hasVertexColors;
        bool has2DTextureCoordinates;
        bool hasNormals;
        bool generateTangents;
        bool generateBinormals;
        ManualLods manualLods;
        
        std::set<wxString> morphTargetNames;
        ObjectAccessorSet bones;

        /** The dimensions of each texture coordinate set */
        TextureCoordSetDimensionArray textureCoordSetDimensions; 

        bool useCustomSubmeshes;
        wxString renderQueueName;
        int renderPriority;

        bool requiresDummyBone;

        MeshAnimationFlags animationFlags;
    };

} }
