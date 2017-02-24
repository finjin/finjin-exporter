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
#include "FinjinSceneExporterContext.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "SceneExportSettings.hpp"
#include "ProgressDialog.hpp"
#include "ApplicationAccessor.hpp"
#include "GeometryState.hpp"
#include "DetectableObjectType.hpp"
#include "FileUtilities.hpp"
#include "MaterialExporter.hpp"
#include "ExporterUtilities.hpp"
#include "FinjinSceneDocument.hpp"
#include "IgnoreObjectType.hpp"
#include "SimpleMeshInfo.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "ApplicationAccessor.hpp"
#include "MeshAnimationsEvaluator.hpp"
#include "MathUtilities.hpp"
#include "AnimationExportSettings.hpp"
#include "AnimationUtilities.hpp"
#include "FinjinGlobalSettings.hpp"
#include "SimpleUri.hpp"
#include "ExportedGeometry.hpp"
#include "AssetReference.hpp"
#include "AssetClass.hpp"
#include "WrappedFileWriter.hpp"
#include "FinjinResourceManager.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"
#include "FinjinVersion.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
FinjinSceneExporterContext::FinjinSceneExporterContext(ExporterHandler* handler, FinjinSceneSettingsAccessor sceneSettings, SceneExportSettings* sceneExportSettings, FinjinSceneExporterContextFlags flags) :
    ExporterContext(handler, sceneSettings, sceneExportSettings)
{
    this->scene = nullptr;
    this->flags = flags;    
    this->dataChunkWriter = nullptr;
}

FinjinSceneExporterContext::~FinjinSceneExporterContext()
{    
}

void FinjinSceneExporterContext::RunExport(const wxString& exportFileName, bool exportSelected)
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("FinjinSceneExporterContext::RunExport(%s, %d)"), exportFileName.wx_str(), (int)exportSelected);

    //First step: Build graph of all the scene objects---------------
    FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Building scene graph"));
    ExportableObjectPtr exportableObjectRoot(new ExportableObject);
    exportableObjectRoot->Initialize(ObjectAccessor::GetRootObject(), *this);
    FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Resolving dependencies"));
    exportableObjectRoot->ResolveDependencies(*this);

    //Second step: Modify the object graph to operate on just the selected objects---------------
    if (exportSelected)
    {
        FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Getting selected objects"));
        ReplaceChildrenWithSelectedObjects(exportableObjectRoot.get());
    }

    //Third step: Add "additional" materials---------------------------------------
    int additionalMaterialCount = this->sceneSettings.GetAdditionalMaterialCount();
    for (int additionalMaterialIndex = 0; additionalMaterialIndex < additionalMaterialCount; additionalMaterialIndex++)
    {
        auto additionalMaterial = this->sceneSettings.GetAdditionalMaterial(additionalMaterialIndex);

        MaterialAccessorVector additionalMaterials;
        additionalMaterial.Expand(additionalMaterials);
        
        for (size_t expandedMaterialIndex = 0; expandedMaterialIndex < additionalMaterials.size(); expandedMaterialIndex++)
            AddMaterial(additionalMaterials[expandedMaterialIndex]);
    }

    //Fourth step: Export the scene graph from the application to an ExportedObject, and figure out meshes-----------------
    std::shared_ptr<ExportedObject> exportedScene;
    if (exportableObjectRoot->detectedObjectType != nullptr)
        exportedScene = exportableObjectRoot->detectedObjectType->Export(exportableObjectRoot, *this);
    if (exportedScene != nullptr)
    {
        std::unordered_set<GeometryState*> exportedGeometryStates;    
        for (auto& exportedGeometry : this->exportedGeometries)
        {   
            if (exportedGeometry.geometryState == nullptr ||
                exportedGeometry.geometryState->submeshes.empty() ||
                exportedGeometryStates.find(exportedGeometry.geometryState) != exportedGeometryStates.end())
            {
                continue;
            }

            //Record the geometry state so it isn't exported again
            exportedGeometryStates.insert(exportedGeometry.geometryState);

            //Final processing on geometry
            exportedGeometry.geometryState->FinalizeVertexAndIndexBuffersForWrite(true);

            //Add mesh and skeleton to unique list for export
            this->uniqueExportedGeometries.push_back(exportedGeometry);
            if (exportedGeometry.geometryState->meshSkeleton.IsValid())
                this->uniqueExportedSkeletons.push_back(exportedGeometry);
        }
    }

    //Fifth step: Write file(s)
    if (exportedScene != nullptr)
    {
        FINJIN_WX_DECLARE_ERROR(error);

        WxDataChunkWriter::Settings writerSettings;
        writerSettings.byteOrder = this->sceneExportSettings->byteOrder;
        writerSettings.maxBytesPerLine = FinjinGlobalSettings::GetInstance().maxBytesPerLine;
        writerSettings.blobTextFormat = FinjinGlobalSettings::GetInstance().blobTextFormat;
        writerSettings.lengthHintPropertyName = StandardAssetDocumentPropertyNames::LENGTH_HINT;
        writerSettings.customHeader = [](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);
            
            if (FinjinGlobalSettings::GetInstance().GetExportAnyDocumentCreationData())
            {
                writer.WriteChunk(StandardAssetDocumentChunkNames::DOCUMENT_CREATION, [](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    auto& globalSettings = FinjinGlobalSettings::GetInstance();

                    //Application
                    if (globalSettings.exportApplicationName)
                    {
                        auto applicationName = wxString::Format(wxT("Finjin Exporter for %s"), ApplicationAccessor::APPLICATION_SHORT_NAME.wx_str());
                        writer.WriteString(StandardAssetDocumentPropertyNames::APPLICATION, applicationName, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                    }

                    //Exporter version
                    if (globalSettings.exportExporterVersion)
                    {
                        writer.WriteString(StandardAssetDocumentPropertyNames::EXPORTER_VERSION_TEXT, FinjinVersion::VERSION_TEXT, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        writer.WriteUInt32(StandardAssetDocumentPropertyNames::EXPORTER_VERSION_NUMBER, FinjinVersion::NUMBER, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                    }

                    //Author
                    if (globalSettings.exportAuthor)
                    {
                        auto author = ApplicationAccessor::GetSceneAuthor();
                        if (!author.empty())
                        {
                            writer.WriteString(StandardAssetDocumentPropertyNames::AUTHOR, author, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }

                        auto lastAuthor = ApplicationAccessor::GetLastAuthor();
                        if (!author.empty() && lastAuthor != author)
                        {
                            writer.WriteString(StandardAssetDocumentPropertyNames::LAST_AUTHOR, lastAuthor, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }
                    }
            
                    //Timestamp
                    if (globalSettings.exportExportTime)
                    {
                        auto exportTime = wxDateTime::Now().ToUTC();
                        writer.WriteDateTime(StandardAssetDocumentPropertyNames::EXPORT_TIME, exportTime, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                    }
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        };

        auto sceneObjectName = FileUtilities::GetFileNameNoExtension(exportFileName);
        ExporterUtilities::FixObjectName(sceneObjectName);
            
        for (; this->sceneExportSettings->HasCurrentFileFormat(); this->sceneExportSettings->NextFileFormat())
        {
            auto currentFileFormat = this->sceneExportSettings->GetCurrentFileFormat();            
            FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, Strings::EXPORTING_FILE_FORMAT_FORMAT, WxStreamingFileFormatUtilities::ToString(currentFileFormat).wx_str());

            SceneDataChunkWriterController chunkWriterController(this->sceneExportSettings);

            auto exportSceneFileNamesForFormat = this->sceneExportSettings->GetAssetFilePaths(this->sceneExportSettings->defaultAssetType, sceneObjectName);            
            auto dataChunkWriter = WxStreamingFileFormatUtilities::CreateFileWriter(exportSceneFileNamesForFormat.absolutePath, currentFileFormat, writerSettings, chunkWriterController, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            dataChunkWriter->SetContextStringProcessor(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ExporterUtilities::FixObjectName);
            
            this->dataChunkWriter = dataChunkWriter.get();
            this->dataChunkWriter->WriteWriterHeader(error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            dataChunkWriter->SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, sceneObjectName);

            this->dataChunkWriter->WriteChunk(StandardAssetDocumentChunkNames::SCENE, [this, exportedScene](WxDataChunkWriter& writer, WxError& error)
            {
                //Name
                writer.WriteString(StandardAssetDocumentPropertyNames::NAME, writer.GetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME), error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Materials
                if (!this->materialHandlerMap.empty())
                {
                    MaterialExporter materialExporter;
                    materialExporter.SetProgressCalculator(this->materialsProgress);
                    materialExporter.SetBitmapsProgressCalculator(this->bitmapsProgress);
                
                    materialExporter.Export(*this->dataChunkWriter, this->materialHandlerMap, this->sceneSettings, *this->sceneExportSettings);
                
                    materialExporter.ExportBitmaps(*this->dataChunkWriter, this->sceneSettings, *this->sceneExportSettings, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }

                //Meshes
                if (!this->uniqueExportedGeometries.empty())
                {
                    this->meshesProgress->SetRange(this->uniqueExportedGeometries.size());

                    FINJIN_WX_DECLARE_ERROR(error);

                    this->dataChunkWriter->WriteChunk(StandardAssetDocumentChunkNames::MESHES, [this](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->uniqueExportedGeometries.size(), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        size_t meshIndex = 0;
                        for (auto& exportedGeometry : this->uniqueExportedGeometries)
                        {
                            WxChunkName meshChunkName(StandardAssetDocumentChunkNames::MESH, meshIndex++);
                            writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, this->sceneExportSettings->GetAssetName(AssetClass::MESH, exportedGeometry.exportableObject));
                            writer.WriteChunk(meshChunkName, [this, &exportedGeometry](WxDataChunkWriter& writer, WxError& error)
                            {
                                FINJIN_WX_ERROR_METHOD_START(error);

                                exportedGeometry.Write(writer, error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                this->meshesProgress->Update(1);
                            }, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }
                    }, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }
            
                //Scene graph
                exportedScene->Write(*this->dataChunkWriter, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)            

            this->dataChunkWriter = nullptr;
        }        
    }

    FinishProgress();
}

void FinjinSceneExporterContext::StartProgress(ExportableObject* rootObject)
{
    this->objectGraphProgress = this->sceneProgress.AddCalculator(wxT("scene object"));
    int objectCount = GetExportableChildCount(rootObject);
    this->objectGraphProgress->SetRange(objectCount);
    
    this->meshesProgress = this->sceneProgress.AddCalculator(wxT("mesh"));

    if (this->sceneSettings.GetExportMaterials())
        this->materialsProgress = this->sceneProgress.AddCalculator(wxT("material"));
    else
        this->materialsProgress = nullptr;
    
    if (this->sceneExportSettings->copyBitmaps || this->sceneExportSettings->IsEmbeddedAssetType(AssetClass::TEXTURE))
        this->bitmapsProgress = this->sceneProgress.AddCalculator(wxT("bitmap"));
    else
        this->bitmapsProgress = nullptr;
    
    this->sceneProgress.AddProgressChangeListener(this);
}

void FinjinSceneExporterContext::FinishProgress()
{
    this->sceneProgress.RemoveCalculators();
    this->sceneProgress.SetRange(1);
    this->sceneProgress.UpdateProgress(1);
}

void FinjinSceneExporterContext::StartSceneExport(FinjinSceneDocument* scene)
{
    this->scene = scene;
    if (this->scene == nullptr)
        return;

    //Scene settings----------------------------------------    
    this->scene->id = this->sceneSettings.GetUserID();
    this->scene->userData = this->sceneSettings.GetUserData();
    this->scene->sceneManager = this->sceneSettings.GetSceneManager();
    this->scene->unitsPerMeter = this->sceneExportSettings->scaledWorldUnitsPerMeter;
    this->scene->unitType = this->sceneSettings.GetSceneScaleUnit();
    ApplicationAccessor::GetScale(this->scene->unitType);
    if (this->scene->unitType.empty())
        this->scene->unitType = ApplicationAccessor::GetDefaultUnitType();
    
    auto scaleDivide = this->sceneSettings.GetSceneScaleDivide();
    if (!MathUtilities::AlmostZero(scaleDivide) && !MathUtilities::AlmostZero(1 - scaleDivide))
    {
        //Scale divider is not the default value
        this->scene->unitType += wxT("/");
        this->scene->unitType += StringUtilities::ToString(scaleDivide);        
    }

    //Flags-----------------------------
    if (this->sceneSettings.GetExportFlags())
        ExporterUtilities::ExportFlags(*this->scene, this->sceneSettings);
    
    //Environment--------------------------------------------
    if (this->sceneSettings.GetExportEnvironment())
    {
        std::shared_ptr<FinjinSceneDocument_Environment> environment(new FinjinSceneDocument_Environment(this->scene));
        this->scene->SetEnvironment(environment);

        //Ambient color
        environment->ambientLightColor = this->sceneSettings.GetAmbientLightColorValue();

        //Background color
        environment->backgroundColor = this->sceneSettings.GetBackgroundColorValue();
        
        //Environment range
        float rangeFar = 0.0f;
        {
            auto environmentRange = ExporterUtilities::ExportEnvironmentRange(*this->scene, this->sceneSettings, *this->sceneExportSettings);
            rangeFar = environmentRange->farClip;
            environment->SetRange(std::move(environmentRange));
        }

        //Fog
        auto fogMode = this->sceneSettings.GetFogModeValue();
        if (fogMode != FogMode::NONE)
        {
            wxString fogModeText;
            FinjinColor fogColor;
            float fogDensity, fogStart, fogEnd;
            this->sceneSettings.GetFogValues(fogModeText, fogColor, fogDensity, fogStart, fogEnd, rangeFar, this->sceneExportSettings->time);
            std::unique_ptr<FinjinSceneDocument_Fog> fog(new FinjinSceneDocument_Fog(this->scene, fogMode, fogDensity, fogStart, fogEnd, fogColor));
            environment->SetFog(std::move(fog));
        }
        
        //Shadows
        {
            auto shadows = ExporterUtilities::ExportShadows(*this->scene, this->sceneSettings, *this->sceneExportSettings);
            environment->SetShadows(std::move(shadows));
        }
    }
}

void FinjinSceneExporterContext::FinishSceneExport()
{    
}

void FinjinSceneExporterContext::AddMaterial(MaterialAccessor material)
{
    if (AnySet(this->flags & FinjinSceneExporterContextFlags::ALWAYS_ADD_MATERIALS) || this->sceneSettings.CanExportMaterial(material))
    {
        if (this->materialHandlerMap.find(material) == this->materialHandlerMap.end())
        {
            //New material
            auto factory = MaterialExporter::GetFactoryForMaterial(material);
            if (factory != nullptr)
            {
                std::shared_ptr<MaterialExporterHandler> exporterHandler(factory->CreateHandler());

                exporterHandler->Initialize(material, this->sceneSettings, *this->sceneExportSettings);

                this->materialHandlerMap.insert(std::make_pair(material, exporterHandler));
            }
        }
    }
}

void FinjinSceneExporterContext::ProgressChanged(ProgressCalculator* progress)
{
    int itemIndex = RoundToInt(progress->GetProgress() * progress->GetRange());
    int itemTotal = RoundToInt(progress->GetRange());
    
    wxString message;
    if (!progress->GetName().empty())
    {
        message = wxString::Format
            (
            wxT("Exporting %s %d of %d"), 
            progress->GetName().wx_str(), 
            itemIndex, 
            itemTotal
            );
    }
    
    if (ProgressDialog::GetInstance() != nullptr)
    {
        ProgressDialog::GetInstance()->UpdateProgress(this->sceneProgress.GetProgress(), progress->GetProgress());
        ProgressDialog::GetInstance()->SetSubprogressLabel(message);
    }

    //This is just a test
    if (!message.empty())
    {
        FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, message);

        /*ApplicationAccessor::LogMessage
            (
            "%s...progress: %f, total progress: %f", 
            message.wx_str(),
            progress->GetProgress(), 
            this->sceneProgress.GetProgress()
            );*/
    }
    else
    {
        //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, "Total progress: 1");        
    }
}

GeometryState* FinjinSceneExporterContext::GetOrCreateGeometry
    (
    ExportableObject* exportableObject,
    MaterialAccessorVector* submeshMaterials,
    ExportedGeometryManualLods* manualLods,
    GeometryState* parentGeometry,
    bool createGeometryStates
    )
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("FinjinSceneExporterContext::GetOrCreateGeometry(%s)"), exportableObject->object.GetLocalName().wx_str());

    //Get all mesh animation settings
    MeshAnimationsEvaluator meshAnimationEval(this->sceneSettings, *this->sceneExportSettings);
    meshAnimationEval.GetAnimations(exportableObject->object, exportableObject->objectSettings, exportableObject->objectExportSettings);

    //Get the geometry
    GeometryState* geometryState = nullptr;
    auto geometryFlags = GeometryStateFlags::NONE;
    if ((parentGeometry != nullptr && parentGeometry->meshSkeleton.IsValid()) || 
        exportableObject->objectSettings.GetExportSkeleton() ||
        !exportableObject->objectSettings.GetExportedMeshDeformedBySkeleton())
    {
        geometryFlags |= GeometryStateFlags::NO_SKELETON_EFFECT;
    }
    if ((parentGeometry != nullptr && parentGeometry->meshMorpher.IsValid()) || exportableObject->objectSettings.GetExportPoses())
        geometryFlags |= GeometryStateFlags::NO_MORPHER_EFFECT;
    if (!this->sceneSettings.GetCreateExtraMaterials())
        geometryFlags |= GeometryStateFlags::NO_EXTRA_MATERIALS;
    if (this->sceneSettings.GetUseExplicitVertexColor())
        geometryFlags |= GeometryStateFlags::FORCE_WHITE_VERTEX_COLORS;
    if (exportableObject->objectSettings.GetRemoveBonesWithNoVertexInfluence())
        geometryFlags |= GeometryStateFlags::REMOVE_BONES_WITH_NO_VERTEX_INFLUENCE;
    if (createGeometryStates)
    {
        geometryState = new GeometryState;
        geometryState->submeshProperties.Initialize(exportableObject->object, exportableObject->objectSettings, exportableObject->objectExportSettings, this->sceneSettings);
    }
    SubmeshesSettings submeshesSettings;
    auto useCustomTextureCoordinateSets = exportableObject->objectSettings.GetUseCustomSubmeshes();
    if (useCustomTextureCoordinateSets)
        exportableObject->objectSettings.GetSubmeshesSettings(submeshesSettings);
    SkeletonReferencePose referencePose;
    exportableObject->objectSettings.GetSkeletonReferencePose(referencePose);
    if ((geometryState != nullptr && geometryState->Create
            (
            exportableObject->objectSettings.GetMeshName(),
            exportableObject->object, 
            this->sceneExportSettings->conversionManager, 
            this->sceneExportSettings->scale, 
            referencePose.type == SkeletonReferencePose::SPECIFIC_TIME ? referencePose.time : this->sceneExportSettings->time, 
            geometryFlags,
            useCustomTextureCoordinateSets ? &submeshesSettings : nullptr,
            &referencePose
            )) ||
        !createGeometryStates)
    {        
        //Try to find matching vertex formats for the submeshes
        for (auto submesh : geometryState->submeshes)
        {
            submesh->submeshProperties.vertexFormatName = FinjinResourceManager::GetInstance().FindVertexFormatName(submesh->submeshProperties.vertexFormatElements);
        }

        //Convert manual LOD meshes 
        if (manualLods != nullptr)
            manualLods->reserve(exportableObject->objectExportSettings.manualLods.size());
        for (size_t i = 0; i < exportableObject->objectExportSettings.manualLods.size(); i++)
        {
            ExportableObjectPtr exportableLodObjectPtr = exportableObject->dependentObjectMap[exportableObject->objectExportSettings.manualLods[i].object];

            ExportedGeometryManualLod manualLod;
            manualLod.exportableObject = exportableLodObjectPtr.get();
            manualLod.geometryState = GetOrCreateGeometry(exportableLodObjectPtr.get(), &manualLod.submeshMaterials, nullptr, nullptr, createGeometryStates);
            if (manualLod.geometryState != nullptr || !createGeometryStates)
            {
                if (manualLods != nullptr)
                    manualLods->push_back(manualLod);
                
                //Associate the exported object with the geometry that was used
                ExportedGeometry exportedGeometry(exportableLodObjectPtr.get(), manualLod.geometryState, nullptr, this->sceneSettings, this->sceneExportSettings);
                this->exportedGeometries.push_back(exportedGeometry);
            }
        }

        if (geometryState != nullptr)
        {
            //Evaluate the mesh animations
            meshAnimationEval.Evaluate(geometryState, exportableObject->objectSettings, exportableObject->objectExportSettings);        
            geometryState->UpdateAnimationData
                (
                exportableObject->objectSettings.GetExportSkeleton(), 
                exportableObject->objectSettings.GetExportPoses()
                );

            //Update bounding volumes
            if (this->sceneSettings.GetAnimateBoundingVolumes() || exportableObject->objectSettings.GetAnimateBoundingVolumes())
                geometryState->AnimateBoundingVolumes(meshAnimationEval.allSampleTimes, this->sceneExportSettings->conversionManager, this->sceneExportSettings->scale, 0, exportableObject->objectSettings.GetAnimatedRoot());
            
            //Merge all mergable meshes
            for (auto& mergedSource : exportableObject->mergedObjects)
            {
                auto mergableGeometry = GetOrCreateGeometry(mergedSource.get(), nullptr, nullptr, geometryState);
                if (mergableGeometry != nullptr)
                {
                    geometryState->Merge(*mergableGeometry, this->sceneSettings.GetAlwaysCreateNewSubmeshesWhenMerging());
                    delete mergableGeometry;
                }
            }

            //Offset mesh and animations by object offset if necessary, and update bounding volumes
            //TODO: Implement this?
            //if (exportableObject->detectedObjectType->GetDescriptor().UsesPivotAsObjectOffset())
            //{
                //TransformAccessor objectOffsetTransformation(exportableObject->object.GetObjectOffsetTransformation(this->sceneExportSettings->time), this->sceneExportSettings->conversionManager, this->sceneExportSettings->scale);
                //geometryState->TransformVertices(objectOffsetTransformation);
            //}    
        }

        //Get all submesh materials
        if (geometryState != nullptr)
        {
            if (submeshMaterials != nullptr)
                submeshMaterials->reserve(geometryState->submeshes.size());
            for (auto submesh : geometryState->submeshes)
            {
                //Add material to scene's collection
                AddMaterial(submesh->material);

                //Add to submesh materials
                if (submeshMaterials != nullptr)
                    submeshMaterials->push_back(submesh->material);
            }
        }
        else
        {
            SubmeshesSettings submeshesSettings;
            GeometryState::GetSubmeshesSettings(exportableObject->object, submeshesSettings, geometryFlags);
            if (submeshMaterials != nullptr)
                submeshMaterials->reserve(submeshesSettings.size());
            
            for (size_t i = 0; i < submeshesSettings.size(); i++)
            {
                //Add material to scene's collection
                AddMaterial(submeshesSettings[i].material);

                //Add to submesh materials
                if (submeshMaterials != nullptr)
                    submeshMaterials->push_back(submeshesSettings[i].material);
            }
        }

        //Try to get a geometry that matches the one that was just created
        //There's no need to do this for mergable submeshes since they're going to be deleted after a merge
        if (geometryState != nullptr)
        {
            auto checkInstances = this->sceneSettings.GetCheckMeshInstances() || exportableObject->objectSettings.GetCheckMeshInstance();
            if (parentGeometry == nullptr && checkInstances)
            {
                auto originalInstance = FindOriginalInstance(geometryState);
                if (originalInstance != nullptr)
                {
                    //Found a match
                    //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("Found a match"));

                    //Delete the one that was just created
                    delete geometryState;

                    //Use the geometry state that was found
                    geometryState = originalInstance->geometryState;
                }
                else
                {
                    //There is no match
                    //FINJIN_EXPORTER_LOG_MESSAGE(INFO_LOG_MESSAGE, wxT("No match"));

                    //Save the geometry state        
                    this->geometryStates.push_back(std::shared_ptr<GeometryState>(geometryState));            
                }
            }
        }
    }
    else
    {
        delete geometryState;
        geometryState = nullptr;
    }

    return geometryState;
}

ExportedGeometry* FinjinSceneExporterContext::FindOriginalInstance(GeometryState* geometryState)
{    
    float tolerance = 0.001f;

    for (auto& exportedGeometry : this->exportedGeometries)
    {
        if (exportedGeometry.geometryState == nullptr)
            continue;

        if (exportedGeometry.geometryState->IsMatchingInstance(*geometryState, tolerance))
            return &exportedGeometry;
    }

    return nullptr;
}

wxString FinjinSceneExporterContext::MakeNodeOrObjectName(ObjectAccessor object, bool useObjectName)
{
    auto name = object.GetLocalName(useObjectName);
    ExporterUtilities::FixObjectName(name);
    return name;
}

int FinjinSceneExporterContext::GetExportableChildCount(ExportableObject* object)
{
    int count = 0;

    if (!ShouldIgnoreChildren(object))
    {
        for (auto childPtr : object->childObjects)
        {
            if (!ShouldIgnoreObject(childPtr.get()))
            {
                count++;

                if (childPtr->objectSettings.IsValid())
                    count += GetExportableChildCount(childPtr.get());
            }
        }   
    }

    return count;
}

bool FinjinSceneExporterContext::ShouldIgnoreObject(ExportableObject* exportableObject, bool rebuildingObjectGraph)
{        
    if (exportableObject->object.IsRoot())
        return false;

    return 
        !exportableObject->objectSettings.IsValid() ||
        (!rebuildingObjectGraph && exportableObject->IsConsumedByParent()) ||
        (this->sceneSettings.GetIgnoreHiddenObjects() && !exportableObject->object.IsVisible()) ||
        exportableObject->detectedObjectType->GetDescriptor().IsMerged() ||
        exportableObject->detectedObjectType == &IgnoreObjectType::GetInstance() ||
        ObjectAccessor::IsSettingsObject(exportableObject->object);
}

bool FinjinSceneExporterContext::ShouldIgnoreChildren(ExportableObject* exportableObject)
{
    if (exportableObject->object.IsRoot())
        return false;

    return 
        !exportableObject->objectSettings.IsValid() ||
        (exportableObject->objectSettings.IsValid() && exportableObject->objectSettings.GetIgnoreChildren()) ||
        exportableObject->detectedObjectType->GetDescriptor().ConsumesAllChildren();
}

void FinjinSceneExporterContext::GetItemSettings(FinjinSceneDocument_Item* item, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings)
{
    //Name
    auto isObject = FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item, FinjinSceneDocument_MovableObject);
    auto isNodeOrObject = isObject || FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item, FinjinSceneDocument_SceneNode);
    if (isNodeOrObject)
        item->name = MakeNodeOrObjectName(object, isObject);
    else
        item->name = object.GetLocalName();

    //ID
    item->id = objectSettings.GetUserID();

    //User data
    item->userData = objectSettings.GetUserData();    

    //Note tracks
    objectSettings.GetNoteTracks(object, item->noteTracks);
    
    //Child order
    item->childOrder = objectSettings.GetChildOrder();

    //Flags
    ExporterUtilities::ExportFlags(*item, objectSettings);
}

void FinjinSceneExporterContext::GetMovableObjectSettings
    (
    FinjinSceneDocument_MovableObject* movableObject, 
    ObjectAccessor object,
    FinjinObjectSettingsAccessor objectSettings,
    TimeAccessor time, 
    CoordinateSystemConverter& conversionManager, 
    float scale
    )
{
    GetItemSettings(movableObject, object, objectSettings);

    TransformAccessor transformation(object.GetObjectOffsetTransformation(time), conversionManager, scale);
    movableObject->objectOffsetTransform = transformation;
}

void FinjinSceneExporterContext::GetRenderableObjectSettings
    (
    FinjinSceneDocument_RenderableMovableObject* renderableObject, 
    ObjectAccessor object,
    FinjinObjectSettingsAccessor objectSettings,
    TimeAccessor time, 
    CoordinateSystemConverter& conversionManager, 
    float scale
    )
{
    GetMovableObjectSettings(renderableObject, object, objectSettings, time, conversionManager, scale);

    //Visibility
    if (objectSettings.GetVisibilityAffectObjectOnly())
    {
        auto visibility = objectSettings.GetVisibility();
        if (visibility == PropertyValues::Visibility::VISIBLE ||
            visibility == PropertyValues::Visibility::TREE_VISIBLE)
        {
            renderableObject->visibility = FinjinSceneDocument_RenderableMovableObject::OBJECT_VISIBLE;
        }
        else if (visibility == PropertyValues::Visibility::HIDDEN ||
            visibility == PropertyValues::Visibility::TREE_HIDDEN)
        {
            renderableObject->visibility = FinjinSceneDocument_RenderableMovableObject::OBJECT_HIDDEN;
        }
    }
    
    //Render queue
    renderableObject->renderQueue = objectSettings.GetRenderQueueName();
    renderableObject->renderPriority = objectSettings.GetRenderPriority();
    
    //Rendering distance
    renderableObject->renderingDistance = objectSettings.GetRenderingDistance() * this->sceneExportSettings->scale;

    //Cast shadows
    renderableObject->castShadows = object.GetCastShadows();

    //Receive shadows
    renderableObject->receiveShadows = object.GetReceiveShadows();
}

void FinjinSceneExporterContext::GenerateNodeAnimations(FinjinSceneDocument_Item* item, ExportableObject* exportableObject)
{
    std::vector<FinjinNodeAnimationSettingsAccessor> nodeAnimationSettings;
    exportableObject->objectSettings.GetNodeAnimations(nodeAnimationSettings);

    for (size_t animationIndex = 0; animationIndex < nodeAnimationSettings.size(); animationIndex++)
    {
        AnimationExportSettings animationSettings
            (
            nodeAnimationSettings[animationIndex], 
            exportableObject->objectSettings.GetNodeAnimationSampleInterval(), 
            this->sceneSettings.GetNodeAnimationSampleInterval(),
            FinjinGlobalSettings::GetInstance().nodeAnimationSampleInterval
            );

        //Create the animation
        std::shared_ptr<FinjinSceneDocument_NodeAnimation> createdAnimation(new FinjinSceneDocument_NodeAnimation(item->sceneDocument));
        item->AddChildItem(createdAnimation);
        
        //Name
        createdAnimation->name = animationSettings.name;

        //Enable
        createdAnimation->enable = nodeAnimationSettings[animationIndex].GetEnabled();

        //Loop
        createdAnimation->loop = nodeAnimationSettings[animationIndex].GetLooped();

        //Interpolation
        //createdAnimation->translationInterpolation = nodeAnimationSettings[animationIndex].GetTranslationInterpolationType();
        //createdAnimation->rotationInterpolation = nodeAnimationSettings[animationIndex].GetRotationInterpolationType();

        //Generate sample times
        std::vector<WxTimeDuration> sampleTimes;
        AnimationUtilities::GetSampledKeyTimes(sampleTimes, animationSettings.timeInterval, animationSettings.sampleInterval);
        
        //Add keys
        createdAnimation->keys.resize(sampleTimes.size());
        for (size_t sampleIndex = 0; sampleIndex < sampleTimes.size(); sampleIndex++)
        {
            auto& key = createdAnimation->keys[sampleIndex];

            //Time
            key.time = animationSettings.GetOutputKeyTime(sampleTimes, sampleIndex);

            //Transform
            auto sampleTime = animationSettings.GetSampleTime(sampleTimes, sampleIndex);
            key.SetTransform(exportableObject->GetConvertedLocalNodeTransformation(sampleTime, this->sceneExportSettings->conversionManager, this->sceneExportSettings->scale));
        }
    }
}
