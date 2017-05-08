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
#include "FinjinSceneExporterHandler.hpp"
#include "DetectableObjectType.hpp"
#include "GeometryState.hpp"
#include "MathUtilities.hpp"
#include "MaxUtilities.hpp"
#include "ExportedObject.hpp"
#include "FinjinSceneDocument.hpp"
#include "ExporterUtilities.hpp"
#include "StringUtilities.hpp"
#include "FileUtilities.hpp"
#include "Strings.hpp"
#include "GeometryStateSubmeshProperties.hpp"
#include "SimpleMeshInfo.hpp"
#include "ApplicationAccessor.hpp"
#include "ExporterContext.hpp"
#include "PropertyValues.hpp"
#include "CubeMapRenderer.hpp"
#include "AnimationUtilities.hpp"
#include "FinjinGlobalSettings.hpp"
#include "CameraExportSettings.hpp"
#include "LightExportSettings.hpp"
#include "SkyExportSettings.hpp"
#include "PhysicalObjectExportSettings.hpp"

#include "EmptyObjectType.hpp"
#include "CameraObjectType.hpp"
#include "LightObjectType.hpp"
#include "MeshObjectType.hpp"
#include "SkyObjectType.hpp"
#include "PhysicalObjectType.hpp"
#include "CubeMapHelperObjectType.hpp"
#include "StaticGeometryObjectType.hpp"
#include "InstancedGeometryObjectType.hpp"
#include "PrefabInstanceObjectType.hpp"
#include "FinjinCubeMapHelper.hpp"
#include "FinjinStaticGeometry.hpp"
#include "FinjinInstancedGeometry.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
bool FinjinSceneExporterHandler::Initialize(AssetClass assetType, const wxString& fileName, bool exportSelected, FinjinSceneSettingsAccessor sceneSettings)
{
    return ExporterHandler::Initialize(assetType, fileName, exportSelected, sceneSettings);
}

bool FinjinSceneExporterHandler::RunExport()
{
    FinjinSceneExporterContext finjinExporterContext(this, this->sceneSettings, &this->sceneExportSettings, FinjinSceneExporterContextFlags::NONE);
    finjinExporterContext.RunExport(this->exportFilePath, this->exportSelected);
    return true;
}

void FinjinSceneExporterHandler::ExportScene(ExportableObjectPtr& exportableObject, FinjinSceneDocument& scene, ExporterContext& context)
{
    auto& finjinExporterContext = static_cast<FinjinSceneExporterContext&>(context);

    finjinExporterContext.StartProgress(exportableObject.get());

    finjinExporterContext.StartSceneExport(&scene);

    FinjinSceneDocumentParentItems parentItems(nullptr, &scene);
    ExportChildren(scene, parentItems, exportableObject, finjinExporterContext);

    finjinExporterContext.FinishSceneExport();
}

void FinjinSceneExporterHandler::ExportChildren
    (
    FinjinSceneDocument& scene,
    FinjinSceneDocumentParentItems parentItems,
    ExportableObjectPtr object,
    FinjinSceneExporterContext& finjinExporterContext
    )
{
    if (!finjinExporterContext.ShouldIgnoreChildren(object.get()))
    {
        for (auto childPtr : object->childObjects)
        {
            if (!childPtr->IsConsumedByParent())
            {
                auto childParentItems = ExportExportableObject(scene, parentItems, childPtr, true, finjinExporterContext);

                //Export children
                if (childParentItems.IsValid())
                    ExportChildren(scene, childParentItems, childPtr, finjinExporterContext);
            }
        }
    }
}

FinjinSceneDocumentParentItems FinjinSceneExporterHandler::ExportExportableObject
    (
    FinjinSceneDocument& scene,
    FinjinSceneDocumentParentItems parentItems,
    ExportableObjectPtr exportableObjectPtr,
    bool createNodeIfNeeded,
    FinjinSceneExporterContext& finjinExporterContext,
    bool affectsObjectProgressCounter
    )
{
    FinjinSceneDocumentParentItems newParentItems;
    if (finjinExporterContext.ShouldIgnoreObject(exportableObjectPtr.get()))
    {
        //Object should be ignored entirely. Return null parent items
        return newParentItems;
    }
    else if (!createNodeIfNeeded)
    {
        //Not creating node. Use parent's node instead
        newParentItems.node = parentItems.node;
        if (newParentItems.node == nullptr)
            newParentItems.object = parentItems.object;
    }

    auto exportableObject = exportableObjectPtr.get();

    if (affectsObjectProgressCounter)
        finjinExporterContext.objectGraphProgress->Update(1);

    //if (++exportableObject->exportCount > 1)
    //    return newParentItems;

    if (createNodeIfNeeded && exportableObjectPtr->detectedObjectType->GetDescriptor().CanAttachToNode())
        newParentItems.node = ExportNode(scene, parentItems, exportableObject, finjinExporterContext).get();

    if (exportableObjectPtr->detectedObjectType == &CameraObjectType::GetInstance())
    {
        ExportCamera(scene, newParentItems, exportableObject, finjinExporterContext);
    }
    else if (exportableObjectPtr->detectedObjectType == &LightObjectType::GetInstance())
    {
        ExportLight(scene, newParentItems, exportableObject, finjinExporterContext);
    }
    else if (exportableObjectPtr->detectedObjectType == &SkyObjectType::GetInstance())
    {
        ExportSky(scene, exportableObject, finjinExporterContext);
    }
    else if (exportableObjectPtr->detectedObjectType == &PhysicalObjectType::GetInstance())
    {
        ExportPhysical(scene, newParentItems, exportableObject, finjinExporterContext);
    }
    else if (exportableObjectPtr->detectedObjectType == &MeshObjectType::GetInstance())
    {
        ExportEntity(scene, newParentItems, exportableObject, finjinExporterContext);
    }
    else if (exportableObjectPtr->detectedObjectType == &CubeMapHelperObjectType::GetInstance())
    {
        ExportCubeMapHelper(scene, exportableObject, finjinExporterContext);
    }
    else if (exportableObjectPtr->detectedObjectType == &StaticGeometryObjectType::GetInstance())
    {
        ExportStaticGeometry(scene, exportableObject, finjinExporterContext);
    }
    else if (exportableObjectPtr->detectedObjectType == &InstancedGeometryObjectType::GetInstance())
    {
        ExportInstancedGeometry(scene, exportableObject, finjinExporterContext);
    }

    //Export sibling objects
    if (newParentItems.IsValid() && !exportableObject->siblingObjects.empty())
    {
        for (auto sibling : exportableObject->siblingObjects)
        {
            ExportExportableObject(scene, newParentItems, sibling, false, finjinExporterContext);
        }
    }

    return newParentItems;
}

std::shared_ptr<FinjinSceneDocument_SceneNode> FinjinSceneExporterHandler::ExportNode
    (
    FinjinSceneDocument& scene,
    FinjinSceneDocumentParentItems parentItems,
    ExportableObject* exportableObject,
    FinjinSceneExporterContext& finjinExporterContext
    )
{
    std::shared_ptr<FinjinSceneDocument_SceneNode> exportedNode(new FinjinSceneDocument_SceneNode(&scene));

    //Determine whether the node represents a prefab
    exportedNode->isPrefab = exportableObject->detectedObjectType->GetDescriptor().CanBePrefab() && exportableObject->objectSettings.GetIsPrefab();
    if (exportedNode->isPrefab)
    {
        exportedNode->prefabName = exportableObject->object.GetLocalName();
        ExporterUtilities::FixObjectName(exportedNode->prefabName);
        if (exportedNode->prefabName.empty())
            exportedNode->isPrefab = false;
    }
    else if (exportableObject->detectedObjectType == &PrefabInstanceObjectType::GetInstance())
    {
        exportedNode->prefabName = exportableObject->objectSettings.GetPrefabName();
        if (!exportedNode->prefabName.empty())
            PreparePrefabAssetReference(exportedNode);
    }

    //Add node to appropriate parent
    if (exportedNode->isPrefab)
    {
        //It's a prefab node. The scene owns prefabs
        scene.AddChildItem(exportedNode);
    }
    else
    {
        //Standard (non-prefab) node. Add to parent as usual
        parentItems.GetBestParentNode()->AddChildItem(exportedNode);
    }

    finjinExporterContext.GetItemSettings(exportedNode.get(), exportableObject->object, exportableObject->objectSettings);

    //Visibility
    if (!exportableObject->objectSettings.GetVisibilityAffectObjectOnly())
        exportedNode->visibility = ExporterUtilities::GetNodeVisibility(exportableObject->objectSettings.GetVisibility());

    //Transform
    exportedNode->transform = exportableObject->GetConvertedLocalNodeTransformation(this->sceneExportSettings.time, this->sceneExportSettings.conversionManager, this->sceneExportSettings.scale);//, &exportedNode->objectRotation);

    //Track target
    if (!exportedNode->isPrefab)
    {
        bool lookAtIsFromController;
        auto maxTargetNode = MaxUtilities::GetLookAtNode(exportableObject->object.node, &lookAtIsFromController);
        if (maxTargetNode != nullptr)
        {
            //Find the exportable object for the target node
            auto targetExportableObject = exportableObject->GetDependentExportableObject(maxTargetNode);
            if (targetExportableObject == nullptr)
                targetExportableObject = finjinExporterContext.FindExportableObject(maxTargetNode);
            if (targetExportableObject != nullptr)
            {
                //Exportable object found. Export target
                std::shared_ptr<FinjinSceneDocument_TrackTarget> exportedTrackTarget(new FinjinSceneDocument_TrackTarget(&scene));
                exportedNode->AddChildItem(exportedTrackTarget);

                //Node name
                exportedTrackTarget->nodeName = finjinExporterContext.MakeNodeOrObjectName(targetExportableObject->object, false);

                //Local direction
                if (lookAtIsFromController)
                    MaxUtilities::GetLookAtDirection(exportableObject->object.node, exportedTrackTarget->localDirection, this->sceneExportSettings.conversionManager);
                else if (exportableObject->detectedObjectType->GetDescriptor().IsCamera() || exportableObject->detectedObjectType->GetDescriptor().IsLight())
                    exportedTrackTarget->localDirection.Set(0,-1,0); //UpAxis::Y
            }
        }
    }

    //Animations
    finjinExporterContext.GenerateNodeAnimations(exportedNode.get(), exportableObject);

    return exportedNode;
}

void FinjinSceneExporterHandler::ExportCamera
    (
    FinjinSceneDocument& scene,
    FinjinSceneDocumentParentItems parentItems,
    ExportableObject* exportableObject,
    FinjinSceneExporterContext& finjinExporterContext
    )
{
    std::shared_ptr<CameraExportSettings> exportedCamera(new CameraExportSettings(&scene));
    if (parentItems.AddAttachedObject(exportedCamera))
        exportedCamera->Initialize(exportableObject->object, exportableObject->objectSettings, finjinExporterContext);
}

void FinjinSceneExporterHandler::ExportLight
    (
    FinjinSceneDocument& scene,
    FinjinSceneDocumentParentItems parentItems,
    ExportableObject* exportableObject,
    FinjinSceneExporterContext& finjinExporterContext
    )
{
    std::shared_ptr<LightExportSettings> exportedLight(new LightExportSettings(&scene));
    if (parentItems.AddAttachedObject(exportedLight))
        exportedLight->Initialize(exportableObject->object, exportableObject->objectSettings, finjinExporterContext);
}

void FinjinSceneExporterHandler::ExportEntity
    (
    FinjinSceneDocument& scene,
    FinjinSceneDocumentParentItems parentItems,
    ExportableObject* exportableObject,
    FinjinSceneExporterContext& finjinExporterContext
    )
{
    //FINJIN_EXPORTER_METHOD_ENTRY_FORMAT("FinjinSceneExporterHandler::ExportEntity(%s)", exportableObject->object.GetLocalName().wx_str());

    MaterialAccessorVector submeshMaterials;
    ExportedGeometryManualLods manualLods;

    auto geometryState = finjinExporterContext.GetOrCreateGeometry(exportableObject, &submeshMaterials, &manualLods);
    if (geometryState != nullptr)
    {
        //Create exported entity
        std::shared_ptr<FinjinSceneDocument_Entity> exportedEntity(new FinjinSceneDocument_Entity(&scene));
        if (parentItems.AddAttachedObject(exportedEntity))
        {
            //Associate the exported object with the geometry that was used
            ExportedGeometry exportedGeometry(exportableObject, geometryState, &manualLods, finjinExporterContext.GetSceneSettings(), finjinExporterContext.GetSceneExportSettings());
            exportedEntity->meshRef = [this, exportableObject, geometryState]()
            {
                return this->sceneExportSettings.GetAssetReference(exportableObject, geometryState);
            };

            //Create subentities
            exportedEntity->subentities.resize(geometryState->submeshes.size());
            for (auto submesh : geometryState->submeshes)
            {
                auto submeshMaterial = submeshMaterials[submesh->index];
                exportedEntity->subentities[submesh->index].materialRef = [this, submeshMaterial]()
                {
                    return this->sceneExportSettings.GetAssetReference(submeshMaterial);
                };
                if (submesh->submeshProperties.useCustomSubmeshes)
                {
                    exportedEntity->subentities[submesh->index].renderQueue = submesh->submeshProperties.renderQueueName;
                    exportedEntity->subentities[submesh->index].renderPriority = submesh->submeshProperties.renderPriority;
                }
            }

            //Create manual LODs
            exportedEntity->manualLods.resize(manualLods.size());
            for (size_t manualLodIndex = 0; manualLodIndex < manualLods.size(); manualLodIndex++)
            {
                size_t submeshMaterialIndex = 0;
                for (auto submesh : geometryState->submeshes)
                {
                    auto submeshMaterial = manualLods[manualLodIndex].submeshMaterials[submeshMaterialIndex++];
                    auto materialRef = [this, submeshMaterial]()
                    {
                        return this->sceneExportSettings.GetAssetReference(submeshMaterial);
                    };
                    exportedEntity->manualLods[manualLodIndex].subentityMaterialRefs.push_back(materialRef);
                }
            }

            //Bone attachments
            if (!FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(parentItems.object, FinjinSceneDocument_BoneAttachment))
                ExportBoneAttachments(scene, exportedEntity.get(), exportableObject, geometryState, finjinExporterContext);

            //Various render settings
            finjinExporterContext.GetRenderableObjectSettings(exportedEntity.get(), exportableObject->object, exportableObject->objectSettings, this->sceneExportSettings.time, this->sceneExportSettings.conversionManager, this->sceneExportSettings.scale);

            //Save data for later
            if (exportableObject->objectSettings.GetExportMesh())
                finjinExporterContext.exportedGeometries.push_back(exportedGeometry);
        }
    }
}

void FinjinSceneExporterHandler::ExportSky
    (
    FinjinSceneDocument& scene,
    ExportableObject* exportableObject,
    FinjinSceneExporterContext& finjinExporterContext
    )
{
    if (scene.GetEnvironment() != nullptr)
    {
        auto geometryState = finjinExporterContext.GetOrCreateGeometry(exportableObject, nullptr, nullptr);
        if (geometryState != nullptr)
        {
            std::shared_ptr<SkyExportSettings> exportedSky(new SkyExportSettings(&scene));
            scene.GetEnvironment()->AddSky(exportedSky);

            //Associate the exported object with the geometry that was used
            ExportedGeometry exportedGeometry(exportableObject, geometryState, nullptr, finjinExporterContext.GetSceneSettings(), finjinExporterContext.GetSceneExportSettings());
            exportedSky->meshRef = [this, exportableObject]()
            {
                return this->sceneExportSettings.GetAssetReference(AssetClass::MESH, exportableObject);
            };

            exportedSky->Initialize(exportableObject->object, exportableObject->objectSettings, finjinExporterContext);
            finjinExporterContext.GenerateNodeAnimations(exportedSky.get(), exportableObject);

            finjinExporterContext.exportedGeometries.push_back(exportedGeometry);
        }
    }
}

void FinjinSceneExporterHandler::ExportPhysical
    (
    FinjinSceneDocument& scene,
    FinjinSceneDocumentParentItems parentItems,
    ExportableObject* exportableObject,
    FinjinSceneExporterContext& finjinExporterContext
    )
{
    std::shared_ptr<PhysicalObjectExportSettings> exportedPhysical(new PhysicalObjectExportSettings(&scene));
    exportedPhysical->Initialize(exportableObject->object, exportableObject->objectSettings, finjinExporterContext);
    parentItems.GetBestParentNode()->AddChildItem(exportedPhysical);
}

void FinjinSceneExporterHandler::ExportCubeMapHelper
    (
    FinjinSceneDocument& scene,
    ExportableObject* exportableObject,
    FinjinSceneExporterContext& finjinExporterContext
    )
{
#if defined(FINJIN_EXPORTER_ENABLE_D3D_FUNCTIONALITY)
    //Render the cube map if it's enabled
    auto helper = static_cast<FinjinCubeMapHelper*>(exportableObject->object.node->GetObjectRef());
    if (helper->GetRenderOnExport())
    {
        auto fileNames = this->sceneExportSettings.ResolveAssetPaths(AssetClass::TEXTURE, helper->GetBaseFileName());
        if (!fileNames.absolutePath.empty())
        {
            FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, Strings::RENDERING_CUBE_MAP_FORMAT, fileNames.absolutePath.wx_str());

            CubeMapRenderer cubeMapRenderer;
            cubeMapRenderer.SetCenterNode(exportableObject->object.node);

            cubeMapRenderer.RenderToFile(fileNames.absolutePath, helper->GetCubeFaceSize(), helper->GetRenderToSeparateFiles());
        }
    }
#endif
}

void FinjinSceneExporterHandler::ExportStaticGeometry
    (
    FinjinSceneDocument& scene,
    ExportableObject* exportableObject,
    FinjinSceneExporterContext& finjinExporterContext
    )
{
    auto staticGeometry = static_cast<FinjinStaticGeometry*>(exportableObject->object.node->GetObjectRef());

    std::shared_ptr<FinjinSceneDocument_StaticGeometry> exportedStaticGeometry(new FinjinSceneDocument_StaticGeometry(&scene));
    scene.AddStaticGeometry(exportedStaticGeometry);

    finjinExporterContext.GetRenderableObjectSettings(exportedStaticGeometry.get(), exportableObject->object, exportableObject->objectSettings, this->sceneExportSettings.time, this->sceneExportSettings.conversionManager, this->sceneExportSettings.scale);

    //Cast shadows
    exportedStaticGeometry->castShadows = staticGeometry->GetCastShadows();

    //Visibility
    exportedStaticGeometry->isVisible = exportableObject->object.IsVisible();

    //Origin
    TransformAccessor transformation(exportableObject->GetFullWorldTransformation(this->sceneExportSettings.time), this->sceneExportSettings.conversionManager, this->sceneExportSettings.scale);
    exportedStaticGeometry->position = transformation.GetTranslation() * this->sceneExportSettings.scale;

    //Dimensions
    Point3 dimensions = staticGeometry->GetSize() * this->sceneExportSettings.scale;
    exportedStaticGeometry->dimensions.Set(dimensions.x, dimensions.y, dimensions.z);
    this->sceneExportSettings.conversionManager.ConvertAbsoluteComponents(exportedStaticGeometry->dimensions);

    TransformAccessor transformationNotConverted(exportableObject->GetFullWorldTransformation(this->sceneExportSettings.time));
    ExportStaticAndInstancedGeometryEntities(scene, exportableObject, finjinExporterContext, exportedStaticGeometry.get(), transformationNotConverted);
}

void FinjinSceneExporterHandler::ExportInstancedGeometry
    (
    FinjinSceneDocument& scene,
    ExportableObject* exportableObject,
    FinjinSceneExporterContext& finjinExporterContext
    )
{
    auto instancedGeometry = static_cast<FinjinInstancedGeometry*>(exportableObject->object.node->GetObjectRef());

    std::shared_ptr<FinjinSceneDocument_InstancedGeometry> exportedInstancedGeometry(new FinjinSceneDocument_InstancedGeometry(&scene));
    scene.AddInstancedGeometry(exportedInstancedGeometry);

    finjinExporterContext.GetRenderableObjectSettings(exportedInstancedGeometry.get(), exportableObject->object, exportableObject->objectSettings, this->sceneExportSettings.time, this->sceneExportSettings.conversionManager, this->sceneExportSettings.scale);

    //Cast shadows
    exportedInstancedGeometry->castShadows = instancedGeometry->GetCastShadows();

    //Visibility
    exportedInstancedGeometry->isVisible = exportableObject->object.IsVisible();

    //Batch count. Subtract 1 since the main batch is assumed
    if (instancedGeometry->GetBatchCount() > 1)
        exportedInstancedGeometry->batchCount = instancedGeometry->GetBatchCount() - 1;

    //Origin
    TransformAccessor transformation(exportableObject->GetFullWorldTransformation(this->sceneExportSettings.time), this->sceneExportSettings.conversionManager, this->sceneExportSettings.scale);
    exportedInstancedGeometry->position = transformation.GetTranslation() * this->sceneExportSettings.scale;

    //Dimensions
    Point3 dimensions = instancedGeometry->GetSize() * this->sceneExportSettings.scale;
    exportedInstancedGeometry->dimensions.Set(dimensions.x, dimensions.y, dimensions.z);
    this->sceneExportSettings.conversionManager.ConvertAbsoluteComponents(exportedInstancedGeometry->dimensions);

    TransformAccessor transformationNotConverted(exportableObject->GetFullWorldTransformation(this->sceneExportSettings.time));
    ExportStaticAndInstancedGeometryEntities(scene, exportableObject, finjinExporterContext, exportedInstancedGeometry.get(), transformationNotConverted);
}

void FinjinSceneExporterHandler::ExportStaticAndInstancedGeometryEntities
    (
    FinjinSceneDocument& scene,
    ExportableObject* exportableObject,
    FinjinSceneExporterContext& finjinExporterContext,
    FinjinSceneDocument_Item* exportedOwnerItem,
    const TransformAccessor& ownerTransformNotConverted
    )
{
    //Entities
    for (auto childExportableObjectPtr : exportableObject->childObjects)
    {
        auto childExportableObject = childExportableObjectPtr.get();

        if (childExportableObject->detectedObjectType == &MeshObjectType::GetInstance())
        {
            //Mesh
            MaterialAccessorVector submeshMaterials;
            auto geometryState = finjinExporterContext.GetOrCreateGeometry(childExportableObject, &submeshMaterials);
            if (geometryState != nullptr)
            {
                //Exported entity
                std::shared_ptr<FinjinSceneDocument_StaticAndInstancedGeometryEntity> exportedEntity(new FinjinSceneDocument_StaticAndInstancedGeometryEntity(&scene));
                finjinExporterContext.GetItemSettings(exportedEntity.get(), exportableObject->object, exportableObject->objectSettings);
                exportedOwnerItem->AddChildItem(exportedEntity);

                //Associate the exported object with the geometry that was used
                ExportedGeometry exportedGeometry(childExportableObject, geometryState, nullptr, finjinExporterContext.GetSceneSettings(), finjinExporterContext.GetSceneExportSettings());
                finjinExporterContext.exportedGeometries.push_back(exportedGeometry);

                //Create subentities
                size_t submeshMaterialIndex = 0;
                for (auto submesh : geometryState->submeshes)
                {
                    auto materialName = this->sceneExportSettings.GetMaterialName(submeshMaterials[submeshMaterialIndex++]);//, submesh->submeshProperties.GetShaderRequirements());
                    exportedEntity->subentityMaterialNames.push_back(materialName);
                }

                //Add position/rotation/scale
                TransformAccessor childTransformation(childExportableObject->GetFullWorldTransformation(this->sceneExportSettings.time), this->sceneExportSettings.conversionManager, this->sceneExportSettings.scale);

                //Transform
                exportedEntity->transform = childTransformation;
            }
        }

        ExportStaticAndInstancedGeometryEntities(scene, childExportableObject, finjinExporterContext, exportedOwnerItem, ownerTransformNotConverted);
    }
}

void FinjinSceneExporterHandler::ExportBoneAttachments
    (
    FinjinSceneDocument& scene,
    FinjinSceneDocument_Entity* exportedEntity,
    ExportableObject* exportableObject,
    GeometryState* geometryState,
    FinjinSceneExporterContext& finjinExporterContext
    )
{
    AllBonesWithAttachments allBonesWithAttachments;
    finjinExporterContext.GetBoneAttachments(allBonesWithAttachments, exportableObject, geometryState);
    for (auto& boneAndAttachments : allBonesWithAttachments)
    {
        for (auto attachedObject : boneAndAttachments.attachedObjects)
        {
            std::shared_ptr<FinjinSceneDocument_BoneAttachment> exportedBoneAttachment(new FinjinSceneDocument_BoneAttachment(&scene));

            finjinExporterContext.GetItemSettings(exportedBoneAttachment.get(), exportableObject->object, exportableObject->objectSettings);

            exportedEntity->AddBoneAttachment(exportedBoneAttachment);

            //Attachment name - needed only if there is no attached object
            if (attachedObject->detectedObjectType == &EmptyObjectType::GetInstance())
                exportedBoneAttachment->name = finjinExporterContext.MakeNodeOrObjectName(attachedObject->object, false);

            //Bone name
            exportedBoneAttachment->boneName = boneAndAttachments.bone->object.GetLocalName();

            //Transform
            exportedBoneAttachment->transform = attachedObject->GetConvertedLocalNodeTransformation(this->sceneExportSettings.time, this->sceneExportSettings.conversionManager, this->sceneExportSettings.scale);

            //Object
            FinjinSceneDocumentParentItems parentItems;
            parentItems.object = exportedBoneAttachment.get();
            ExportExportableObject(scene, parentItems, attachedObject, false, finjinExporterContext, false);
        }
    }
}
