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
#include "GeometryStateSubmeshProperties.hpp"
#include "MeshAnimationTracksDetector.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
GeometryStateSubmeshProperties::GeometryStateSubmeshProperties()
{
}

void GeometryStateSubmeshProperties::Initialize
    (
    ObjectAccessor object,
    FinjinObjectSettingsAccessor objectSettings,
    FinjinSceneSettingsAccessor sceneSettings
    )
{
    Initialize(object, objectSettings, 0, sceneSettings);
}

void GeometryStateSubmeshProperties::Initialize
    (
    ObjectAccessor object,
    FinjinObjectSettingsAccessor objectSettings,
    const ObjectExportSettings& objectExportSettings,
    FinjinSceneSettingsAccessor sceneSettings
    )
{
    Initialize(object, objectSettings, &objectExportSettings, sceneSettings);
}

void GeometryStateSubmeshProperties::Initialize
    (
    ObjectAccessor object,
    FinjinObjectSettingsAccessor objectSettings,
    const ObjectExportSettings* objectExportSettings,
    FinjinSceneSettingsAccessor sceneSettings
    )
{
    this->objectSettings = objectSettings;
    this->primitiveType = objectSettings.GetPrimitiveTypeValue();
    if (this->primitiveType == PrimitiveType::DEFAULT)
        this->primitiveType = object.GetDefaultPrimitiveType();
    this->pointSize = objectSettings.GetPointSize();
    this->exportVertexColors = objectSettings.GetExportVertexColors() || sceneSettings.GetExportVertexColors();
    this->hasVertexColors = false;
    this->has2DTextureCoordinates = false;
    this->hasNormals = false;
    this->generateTangents = false;
    this->generateBinormals = false;
    if (objectExportSettings != nullptr)
        this->manualLods = objectExportSettings->manualLods;
    else
        this->manualLods.clear();

    if (this->primitiveType == PrimitiveType::TRIANGLES)
    {
        this->hasNormals = true;
        this->generateTangents = this->hasNormals && (objectSettings.GetExportTangents() || sceneSettings.GetExportTangents());
        this->generateBinormals = this->generateTangents && (objectSettings.GetExportBinormals() || sceneSettings.GetExportBinormals());
    }

    this->useCustomSubmeshes = objectSettings.GetUseCustomSubmeshes();
    this->renderQueueName = objectSettings.GetRenderQueueName();
    this->renderPriority = -1;

    this->requiresDummyBone = false;
    this->animationFlags = MeshAnimationFlags::NONE;
}

void GeometryStateSubmeshProperties::Merge(GeometryStateSubmeshProperties& other)
{
    if (this->bones.empty() != other.bones.empty())
        this->requiresDummyBone = true;
    this->requiresDummyBone |= other.requiresDummyBone;

    this->animationFlags |= other.animationFlags;

    if (other.renderQueueName != this->renderQueueName || other.renderPriority != this->renderPriority)
        other.useCustomSubmeshes = true;
}

bool GeometryStateSubmeshProperties::MatchesForMerge(const GeometryStateSubmeshProperties& other)
{
    auto result =
        HasMergableAnimation(other) &&
        StringSetsSame(this->morphTargetNames, other.morphTargetNames) &&
        this->primitiveType == other.primitiveType &&
        this->pointSize == other.pointSize &&
        this->hasVertexColors == other.hasVertexColors &&
        this->hasNormals == other.hasNormals &&
        CanExportVertexColors() == other.CanExportVertexColors() &&
        CanGenerateTangents() == other.CanGenerateTangents() &&
        CanGenerateBinormals() == other.CanGenerateBinormals() &&
        this->textureCoordSetDimensions == other.textureCoordSetDimensions &&
        this->renderQueueName == other.renderQueueName &&
        this->renderPriority == other.renderPriority
        ;

    return result;
}

bool GeometryStateSubmeshProperties::MatchesForInstancing(const GeometryStateSubmeshProperties& other)
{
    std::vector<FinjinMeshAnimationSettingsAccessor> meshAnimations;
    std::vector<FinjinMeshAnimationSettingsAccessor> otherMeshAnimations;
    if (this->objectSettings.IsValid())
        this->objectSettings.GetMeshAnimations(meshAnimations);
    if (other.objectSettings.IsValid())
        const_cast<GeometryStateSubmeshProperties&>(other).objectSettings.GetMeshAnimations(otherMeshAnimations);

    return
        this->animationFlags == other.animationFlags &&
        StringSetsSame(this->morphTargetNames, other.morphTargetNames) &&
        ObjectAccessorSetsSame(this->bones, other.bones) &&
        AnimationUtilities::AnimationsSame(meshAnimations, otherMeshAnimations) &&
        this->primitiveType == other.primitiveType &&
        this->pointSize == other.pointSize &&
        this->hasVertexColors == other.hasVertexColors &&
        this->hasNormals == other.hasNormals &&
        CanExportVertexColors() == other.CanExportVertexColors() &&
        CanGenerateTangents() == other.CanGenerateTangents() &&
        CanGenerateBinormals() == other.CanGenerateBinormals() &&
        this->manualLods == other.manualLods &&
        this->textureCoordSetDimensions == other.textureCoordSetDimensions &&
        this->renderQueueName == other.renderQueueName &&
        this->renderPriority == other.renderPriority
        ;
}

bool GeometryStateSubmeshProperties::CanExportVertexColors() const
{
    return this->exportVertexColors && this->hasVertexColors;
}

bool GeometryStateSubmeshProperties::CanGenerateTangents() const
{
    return this->has2DTextureCoordinates && this->hasNormals && this->generateTangents;
}

bool GeometryStateSubmeshProperties::CanGenerateBinormals() const
{
    return CanGenerateTangents() && this->generateBinormals;
}

void GeometryStateSubmeshProperties::SetTextureCoordinateSetDimensions(const TextureCoordSetDimensionArray& textureCoordSetDimensions)
{
    this->textureCoordSetDimensions = textureCoordSetDimensions;

    this->has2DTextureCoordinates = false;
    for (size_t i = 0; i < this->textureCoordSetDimensions.size(); i++)
    {
        if (this->textureCoordSetDimensions[i] == 2)
        {
            this->has2DTextureCoordinates = true;
            break;
        }
    }
}

bool GeometryStateSubmeshProperties::IsAnimated() const
{
    return this->animationFlags != MeshAnimationFlags::NONE;
}

bool GeometryStateSubmeshProperties::HasMergableAnimation(const GeometryStateSubmeshProperties& other) const
{
    auto hasMixedMeshAnimation =
        AnySet(this->animationFlags) &&
        AnySet(other.animationFlags) &&
        NoneSet(this->animationFlags & other.animationFlags)
        ;

    return
        !hasMixedMeshAnimation &&
        (NoneSet(this->animationFlags) || this->animationFlags == MeshAnimationFlags::HAS_SKELETON);
}
