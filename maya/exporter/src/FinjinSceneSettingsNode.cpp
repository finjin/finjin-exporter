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
#include "FinjinSceneSettingsNode.hpp"
#include "FinjinMayaTypeID.hpp"
#include "NodeAttributeAdder.hpp"
#include "ExtendedValue.hpp"
#include "FinjinObjectSettingsAccessor.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
FinjinSceneSettingsNode::Attributes FinjinSceneSettingsNode::attributes;
std::list<MObject> FinjinSceneSettingsNode::attributesList;
std::list<MObject> FinjinSceneSettingsNode::userDataAttributesList;
std::list<MObject> FinjinSceneSettingsNode::extendedValueAttributesList;

const MString FinjinSceneSettingsNode::TYPE_NAME("FinjinSceneSettingsNode");
const MTypeId FinjinSceneSettingsNode::TYPE_ID(FinjinMayaTypeID::SCENE_SETTINGS_NODE);


//Implementation---------------------------------------------------------------
FinjinSceneSettingsNode::FinjinSceneSettingsNode() 
{
}

FinjinSceneSettingsNode::~FinjinSceneSettingsNode() 
{
}

MStatus FinjinSceneSettingsNode::Initialize()
{
    return AddAttributes() == MStatus::kSuccess ? MStatus::kSuccess : MS::kFailure;
}

void* FinjinSceneSettingsNode::Creator()
{
    return new FinjinSceneSettingsNode();
}

MStatus FinjinSceneSettingsNode::AddAttributes()
{
    NodeAttributeAdder adder;      
    adder.hidden = false;
    
    attributes.CreateUserDataAttributes(userDataAttributesList, adder);

    attributes.CreateExtendedValueAttributes(extendedValueAttributesList, adder);
        
    attributesList.push_back(attributes.sceneManager = adder.AddString("SceneManager"));
    attributesList.push_back(attributes.exportEnvironment = adder.AddBool("ExportEnvironment", true));
    attributesList.push_back(attributes.ambientLightColor = adder.AddColor("AmbientLightColor", MColor(.33f, .33f, .33f)));
    attributesList.push_back(attributes.ambientLightColorSync = adder.AddInt("AmbientLightColorSync", (int)ValueSourceSync::SEPARATE));
    attributesList.push_back(attributes.backgroundColor = adder.AddColor("BackgroundColor"));
    attributesList.push_back(attributes.backgroundColorSync = adder.AddInt("BackgroundColorSync", (int)ValueSourceSync::SEPARATE));
    attributesList.push_back(attributes.environmentNear = adder.AddDistance("EnvironmentNear", MDistance(0, INTERNAL_UNIT_TYPE)));
    attributesList.push_back(attributes.environmentFar = adder.AddDistance("EnvironmentFar", MDistance(FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE, INTERNAL_UNIT_TYPE)));
    attributesList.push_back(attributes.environmentFarObject = adder.AddObjectReference("EnvironmentFarObject"));
    attributesList.push_back(attributes.fogSync = adder.AddInt("FogSync", (int)ValueSourceSync::SEPARATE));
    attributesList.push_back(attributes.fogMode = adder.AddString("FogMode"));
    attributesList.push_back(attributes.fogDensity = adder.AddFloat("FogDensity"));
    attributesList.push_back(attributes.fogStart = adder.AddFloat("FogStart"));
    attributesList.push_back(attributes.fogEnd = adder.AddFloat("FogEnd", 1));
    attributesList.push_back(attributes.fogColor = adder.AddColor("FogColor"));
    
    attributesList.push_back(attributes.shadowTechnique = adder.AddString("ShadowTechnique"));
    attributesList.push_back(attributes.shadowColor = adder.AddColor("ShadowColor"));
    attributesList.push_back(attributes.shadowFarDistance = adder.AddFloat("ShadowFarDistance"));
        
    attributesList.push_back(attributes.sceneScaleUnit = adder.AddString("SceneScaleUnit"));
    attributesList.push_back(attributes.sceneScaleDivide = adder.AddFloat("SceneScaleDivide", 1.0));

    attributesList.push_back(attributes.exportHierarchy = adder.AddBool("ExportHierarchy", true));
    attributesList.push_back(attributes.ignoreHiddenObjects = adder.AddBool("IgnoreHiddenObjects", true));

    attributesList.push_back(attributes.nodeAnimationSampleInterval = adder.AddDouble("NodeAnimationSampleInterval"));
    attributesList.push_back(attributes.nodeAnimationSampleType = adder.AddInt("NodeAnimationSampleType", (int)SampleType::USE_PARENT));
    attributesList.push_back(attributes.configurationTargetName = adder.AddString("ConfigurationTargetName"));
    attributesList.push_back(attributes.skeletonAnimationSampleInterval = adder.AddDouble("SkeletonAnimationSampleInterval"));
    attributesList.push_back(attributes.skeletonAnimationSampleType = adder.AddInt("SkeletonAnimationSampleType", (int)SampleType::USE_PARENT));
    attributesList.push_back(attributes.vertexAnimationSampleInterval = adder.AddDouble("VertexAnimationSampleInterval"));
    attributesList.push_back(attributes.vertexAnimationSampleType = adder.AddInt("VertexAnimationSampleType", (int)SampleType::USE_PARENT));
    
    attributesList.push_back(attributes.exportMeshes = adder.AddBool("ExportMeshes", true));
    attributesList.push_back(attributes.disallowedMeshes = adder.AddObjectReferenceArray("DisallowedMeshes"));
    attributesList.push_back(attributes.exportVertexColors = adder.AddBool("ExportVertexColors", true));
    attributesList.push_back(attributes.submeshNaming = adder.AddString("SubmeshNaming"));
    attributesList.push_back(attributes.submeshCustomName = adder.AddString("SubmeshCustomName"));    
    attributesList.push_back(attributes.convertBitmapsToTextures = adder.AddBool("ConvertBitmapsToTextures", false));
    attributesList.push_back(attributes.exportTangents = adder.AddBool("ExportTangents", true));
    attributesList.push_back(attributes.exportBinormals = adder.AddBool("ExportBinormals"));
    attributesList.push_back(attributes.checkMeshInstances = adder.AddBool("CheckMeshInstances", false));
    attributesList.push_back(attributes.alwaysCreateNewSubmeshesWhenMerging = adder.AddBool("AlwaysCreateNewSubmeshesWhenMerging", true));
    attributesList.push_back(attributes.animateBoundingVolumes = adder.AddBool("AnimateBoundingVolumes"));
    attributesList.push_back(attributes.vertexColorFormat = adder.AddString("VertexColorFormat"));
    attributesList.push_back(attributes.useExplicitVertexColor = adder.AddBool("UseExplicitVertexColor"));
    
    attributesList.push_back(attributes.exportMaterials = adder.AddBool("ExportMaterials", true));
    attributesList.push_back(attributes.additionalMaterials = adder.AddObjectReferenceArray("AdditionalMaterials"));
    attributesList.push_back(attributes.disallowedMaterials = adder.AddObjectReferenceArray("DisallowedMaterials"));
    attributesList.push_back(attributes.createExtraMaterials = adder.AddBool("CreateExtraMaterials"));
    attributesList.push_back(attributes.includeParentMaterialName = adder.AddBool("IncludeParentMaterialName"));
    attributesList.push_back(attributes.noMaterialName = adder.AddString("NoMaterialName"));

    attributesList.push_back(attributes.renameBitmapsFrom = adder.AddStringArray("RenameBitmapsFrom"));
    attributesList.push_back(attributes.renameBitmapsTo = adder.AddStringArray("RenameBitmapsTo"));

    attributesList.push_back(attributes.renameBitmapExtensionsFrom = adder.AddStringArray("RenameBitmapExtensionsFrom"));
    attributesList.push_back(attributes.renameBitmapExtensionsTo = adder.AddStringArray("RenameBitmapExtensionsTo"));

    attributesList.push_back(attributes.exportFlags = adder.AddBool("ExportFlags", true));
    attributesList.push_back(attributes.flagNames = adder.AddStringArray("FlagNames"));
    
    attributesList.push_back(attributes.resourcesChanged = adder.AddInt("ResourcesChangedCounterPassNotification"));

    attributesList.push_back(attributes.objectSettings = FinjinObjectSettingsAccessor::CreateObjectSettingsArrayAttribute(adder, "ObjectSettings"));

    return MS::kSuccess;
}

MObject FinjinSceneSettingsNode::GetSceneSettings(bool createIfNecessary, bool* newInstance)
{
    MObject sceneSettingsNode;

    //Try to find the scene settings node
    auto isNew = false;
    MIteratorType iteratorType;
    iteratorType.setObjectType(MIteratorType::kMObject);
    MItDependencyNodes nodeIterator(iteratorType);
    for (; !nodeIterator.isDone(); nodeIterator.next())
    {            
        MFnDependencyNode nodeFn(nodeIterator.item());
        if (nodeFn.typeId() == TYPE_ID)
        {
            //Found scene settings
            sceneSettingsNode = nodeIterator.item();
            break;
        }            
    }

    //Create scene settings node if it doesn't exist
    if (sceneSettingsNode.isNull() && createIfNecessary)
    {
        isNew = true;

        {
            MDGModifier graphModifier;
            sceneSettingsNode = graphModifier.createNode(TYPE_ID);
            graphModifier.doIt();
        }
        {
            MDGModifier graphModifier;
            graphModifier.renameNode(sceneSettingsNode, TYPE_NAME);
            graphModifier.doIt();
        }
    }

    if (newInstance != nullptr)
        *newInstance = isNew;

    return sceneSettingsNode;
}

MObject FinjinSceneSettingsNode::GetSceneSettingsByIndex(int index)
{
    MObject sceneSettingsNode;

    //Try to find the scene settings node
    MIteratorType iteratorType;
    iteratorType.setObjectType(MIteratorType::kMObject);
    MItDependencyNodes nodeIterator(iteratorType);
    for (; !nodeIterator.isDone(); nodeIterator.next())
    {            
        MFnDependencyNode nodeFn(nodeIterator.item());
        if (nodeFn.typeId() == TYPE_ID)
        {
            //Found scene settings
            if (index-- == 0)
            {
                sceneSettingsNode = nodeIterator.item();
                break;
            }
        }            
    }

    return sceneSettingsNode;
}

bool FinjinSceneSettingsNode::IsSceneSettingsNode(MObject& obj)
{
    MFnDependencyNode nodeFn(obj);
    return nodeFn.typeId() == TYPE_ID;
}
