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
#include "FinjinSceneSettingsAccessor.hpp"
#include "FinjinObjectSettingsAccessor.hpp"
#include "FinjinSceneSettingsNode.hpp"
#include "ApplicationAccessor.hpp"
#include "FinjinGlobalSettings.hpp"
#include "MayaUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
FinjinSceneSettingsAccessor FinjinSceneSettingsAccessor::GetSceneSettings(bool createIfNecessary, bool* newInstance)
{
    bool isNew;
    FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsNode::GetSceneSettings(createIfNecessary, &isNew);
    if (isNew)
        FinjinGlobalSettings::GetInstance().GetSceneSettingsDefaults(sceneSettings);

    if (newInstance != nullptr)
        *newInstance = isNew;

    return sceneSettings;
}

void FinjinSceneSettingsAccessor::MergeObjectSettings(FinjinSceneSettingsAccessor& settings)
{
    MPlug objectSettingsPlugs(settings.obj, FinjinSceneSettingsNode::attributes.objectSettings);
    unsigned int objectSettingsCount = objectSettingsPlugs.evaluateNumElements();
    for (int i = objectSettingsCount - 1; i >= 0; i--)
    {
        //Create an accessor for the other object settings
        FinjinObjectSettingsAccessor objectSettings(objectSettingsPlugs[i]);

        //Copy the other object settings to settings in this scene settings object
        MPlug objectPlug = objectSettingsPlugs[i].child(FinjinObjectSettingsAccessor::attributes.object);
        MObject object = MayaPlug::GetObjectReference(objectPlug);
        GetObjectSettings(object).CopyFrom(objectSettings);
    }
}

void FinjinSceneSettingsAccessor::MergeSceneSettings()
{    
    FinjinSceneSettingsAccessor currentSceneSettings = FinjinSceneSettingsNode::GetSceneSettingsByIndex(0);
    if (currentSceneSettings.IsValid())
    {
        //Keep getting extra scene settings, merging them, and deleting them until there are no extras left
        for (MObject newSceneSettingsNode = FinjinSceneSettingsNode::GetSceneSettingsByIndex(1);
            !newSceneSettingsNode.isNull();
            newSceneSettingsNode = FinjinSceneSettingsNode::GetSceneSettingsByIndex(1))
        {
            FinjinSceneSettingsAccessor newSceneSettings(newSceneSettingsNode);
            currentSceneSettings.MergeObjectSettings(newSceneSettings);
                
            MayaUtilities::DeleteObject(newSceneSettingsNode);
        }
    }
}

FinjinObjectSettingsAccessor FinjinSceneSettingsAccessor::GetObjectSettings(ObjectAccessor object, bool createIfNecessary, bool* newInstance)
{
    FinjinObjectSettingsAccessor objectSettings;

    //Try to get the object settings from the scene settings
    MPlug objectSettingsPlugs(this->obj, FinjinSceneSettingsNode::attributes.objectSettings);
    unsigned int objectSettingsCount = objectSettingsPlugs.evaluateNumElements();
    for (unsigned int i = 0; i < objectSettingsCount; i++)
    {
        MPlug objectSettingsPlug = objectSettingsPlugs[i];
        MPlug objectPlug = objectSettingsPlug.child(FinjinObjectSettingsAccessor::attributes.object);
        if (object == MayaPlug::GetObjectReference(objectPlug))
        {
            objectSettings.rootPlug = objectSettingsPlug;
            break;
        }
    }

    //If settings were not present, try to get them from the object itself (old Finjin versions stored them there)
    static const wxString BASIC_OBJECT_ATTRIBUTE(wxT("Object_PointSize")); //The name of basic object settings attribute
    
    //If object settings are not present by now, they don't exist anywhere
    //Add new object settings to scene settings
    if (!objectSettings.IsValid())
    {
        auto isNew = false;

        if (createIfNecessary)
        {   
            //Create new attribute
            isNew = true;

            //Add to scene settings
            MPlug objectSettingsPlug = objectSettingsPlugs[MayaPlug::AddAttributeElement(objectSettingsPlugs, BASIC_OBJECT_ATTRIBUTE)];
            MPlug objectPlug = objectSettingsPlug.child(FinjinObjectSettingsAccessor::attributes.object);
            MayaPlug::SetObjectReference(objectPlug, object.obj);

            //Update object settings plug to use the scene object settings
            objectSettings.rootPlug = objectSettingsPlug;
        }

        if (newInstance != nullptr)
            *newInstance = isNew;
    }

    return objectSettings;
}

void FinjinSceneSettingsAccessor::RemoveObjectSettings(ObjectAccessor object)
{
    MPlug objectSettingsPlugs(this->obj, FinjinSceneSettingsNode::attributes.objectSettings);
    unsigned int objectSettingsCount = objectSettingsPlugs.evaluateNumElements();
    for (unsigned int i = 0; i < objectSettingsCount; i++)
    {
        MPlug objectSettingsPlug = objectSettingsPlugs[i];
        MPlug objectPlug = objectSettingsPlug.child(FinjinObjectSettingsAccessor::attributes.object);
        MObject objectRef = MayaPlug::GetObjectReference(objectPlug);
        if (objectRef.isNull() || object == objectRef)
        {
            MayaPlug::RemoveAttributeElement(objectSettingsPlug);
            break;
        }
    }        
}

void FinjinSceneSettingsAccessor::Upgrade()
{
    MPlug objectSettingsPlugs(this->obj, FinjinSceneSettingsNode::attributes.objectSettings);
    unsigned int objectSettingsCount = objectSettingsPlugs.evaluateNumElements();
    for (unsigned int i = 0; i < objectSettingsCount; i++)
    {
        FinjinObjectSettingsAccessor objectSettings(objectSettingsPlugs[i]);
        objectSettings.Upgrade();
    }
}

IMPLEMENT_STATIC_ATTRIBUTE_USER_DATA_ACCESSOR_METHODS(FinjinSceneSettingsAccessor, obj, FinjinSceneSettingsNode::attributes)

IMPLEMENT_STATIC_ATTRIBUTE_EXTENDED_VALUE_ACCESSOR_METHODS(FinjinSceneSettingsAccessor, obj, FinjinSceneSettingsNode::attributes)

wxString FinjinSceneSettingsAccessor::GetSceneScaleUnit()
{
    return ApplicationStringToWxString(MPlug(this->obj, FinjinSceneSettingsNode::attributes.sceneScaleUnit).asString());
}

float FinjinSceneSettingsAccessor::GetSceneScaleDivide()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.sceneScaleDivide).asFloat();
}

bool FinjinSceneSettingsAccessor::GetExportHierarchy()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportHierarchy).asBool();
}

bool FinjinSceneSettingsAccessor::GetIgnoreHiddenObjects()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.ignoreHiddenObjects).asBool();
}

wxString FinjinSceneSettingsAccessor::GetSceneManager()
{
    return ApplicationStringToWxString(MPlug(this->obj, FinjinSceneSettingsNode::attributes.sceneManager).asString());
}

bool FinjinSceneSettingsAccessor::GetExportFlags()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportFlags).asBool();
}

int FinjinSceneSettingsAccessor::GetFlagCount()
{
    return (int)MPlug(this->obj, FinjinSceneSettingsNode::attributes.flagNames).evaluateNumElements();
}

void FinjinSceneSettingsAccessor::GetFlag(int i, wxString& name)
{
    name = ApplicationStringToWxString(MPlug(this->obj, FinjinSceneSettingsNode::attributes.flagNames)[i].asString());    
}

double FinjinSceneSettingsAccessor::GetNodeAnimationSampleInterval()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.nodeAnimationSampleInterval).asDouble();
}

SampleType FinjinSceneSettingsAccessor::GetNodeAnimationSampleType()
{
    return (SampleType)MPlug(this->obj, FinjinSceneSettingsNode::attributes.nodeAnimationSampleType).asInt();
}

wxString FinjinSceneSettingsAccessor::GetConfigurationTargetName()
{
    return ApplicationStringToWxString(MPlug(this->obj, FinjinSceneSettingsNode::attributes.configurationTargetName).asString());
}

double FinjinSceneSettingsAccessor::GetSkeletonAnimationSampleInterval()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.skeletonAnimationSampleInterval).asDouble();
}

SampleType FinjinSceneSettingsAccessor::GetSkeletonAnimationSampleType()
{
    return (SampleType)MPlug(this->obj, FinjinSceneSettingsNode::attributes.skeletonAnimationSampleType).asInt();
}

double FinjinSceneSettingsAccessor::GetVertexAnimationSampleInterval()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.vertexAnimationSampleInterval).asDouble();
}

SampleType FinjinSceneSettingsAccessor::GetVertexAnimationSampleType()
{
    return (SampleType)MPlug(this->obj, FinjinSceneSettingsNode::attributes.vertexAnimationSampleType).asInt();
}

wxString FinjinSceneSettingsAccessor::GetSubmeshNaming()
{
    return ApplicationStringToWxString(MPlug(this->obj, FinjinSceneSettingsNode::attributes.submeshNaming).asString());
}

wxString FinjinSceneSettingsAccessor::GetSubmeshCustomName()
{
    return ApplicationStringToWxString(MPlug(this->obj, FinjinSceneSettingsNode::attributes.submeshCustomName).asString());
}

bool FinjinSceneSettingsAccessor::GetExportVertexColors()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportVertexColors).asBool();
}

wxString FinjinSceneSettingsAccessor::GetVertexColorFormat()
{
    return ApplicationStringToWxString(MPlug(this->obj, FinjinSceneSettingsNode::attributes.vertexColorFormat).asString());
}

bool FinjinSceneSettingsAccessor::GetUseExplicitVertexColor()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.useExplicitVertexColor).asBool();
}

bool FinjinSceneSettingsAccessor::GetExportTangents()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportTangents).asBool();
}

bool FinjinSceneSettingsAccessor::GetExportBinormals()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportBinormals).asBool();
}

bool FinjinSceneSettingsAccessor::GetCheckMeshInstances()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.checkMeshInstances).asBool();
}

bool FinjinSceneSettingsAccessor::GetAlwaysCreateNewSubmeshesWhenMerging()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.alwaysCreateNewSubmeshesWhenMerging).asBool();
}

bool FinjinSceneSettingsAccessor::GetAnimateBoundingVolumes()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.animateBoundingVolumes).asBool();
}

bool FinjinSceneSettingsAccessor::GetExportMeshes()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportMeshes).asBool();
}

int FinjinSceneSettingsAccessor::GetDisallowedMeshCount()
{
    return MayaPlug::GetObjectReferenceCount(this->obj, FinjinSceneSettingsNode::attributes.disallowedMeshes);    
}

ObjectAccessor FinjinSceneSettingsAccessor::GetDisallowedMesh(int i)
{
    return MayaPlug::GetObjectReference(this->obj, FinjinSceneSettingsNode::attributes.disallowedMeshes, i);    
}

bool FinjinSceneSettingsAccessor::GetCreateExtraMaterials()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.createExtraMaterials).asBool();
}

bool FinjinSceneSettingsAccessor::GetIncludeParentMaterialName()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.includeParentMaterialName).asBool();
}

wxString FinjinSceneSettingsAccessor::GetNoMaterialName()
{
    return ApplicationStringToWxString(MPlug(this->obj, FinjinSceneSettingsNode::attributes.noMaterialName).asString());
}

bool FinjinSceneSettingsAccessor::GetExportMaterials()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportMaterials).asBool();
}

int FinjinSceneSettingsAccessor::GetAdditionalMaterialCount()
{
    return MayaPlug::GetObjectReferenceCount(this->obj, FinjinSceneSettingsNode::attributes.additionalMaterials);    
}

MaterialAccessor FinjinSceneSettingsAccessor::GetAdditionalMaterial(int i)
{
    return MayaPlug::GetObjectReference(this->obj, FinjinSceneSettingsNode::attributes.additionalMaterials, i);    
}

int FinjinSceneSettingsAccessor::GetDisallowedMaterialCount()
{
    return MayaPlug::GetObjectReferenceCount(this->obj, FinjinSceneSettingsNode::attributes.disallowedMaterials);    
}

MaterialAccessor FinjinSceneSettingsAccessor::GetDisallowedMaterial(int i)
{
    return MayaPlug::GetObjectReference(this->obj, FinjinSceneSettingsNode::attributes.disallowedMaterials, i);    
}

int FinjinSceneSettingsAccessor::GetRenameBitmapCount()
{
    return (int)MPlug(this->obj, FinjinSceneSettingsNode::attributes.renameBitmapsFrom).evaluateNumElements();
}

void FinjinSceneSettingsAccessor::GetRenameBitmap(int i, wxString& from, wxString& to)
{
    from = ApplicationStringToWxString(MPlug(this->obj, FinjinSceneSettingsNode::attributes.renameBitmapsFrom)[i].asString());
    to = ApplicationStringToWxString(MPlug(this->obj, FinjinSceneSettingsNode::attributes.renameBitmapsTo)[i].asString());
}

int FinjinSceneSettingsAccessor::GetRenameBitmapExtensionCount()
{
    return (int)MPlug(this->obj, FinjinSceneSettingsNode::attributes.renameBitmapExtensionsFrom).evaluateNumElements();
}

void FinjinSceneSettingsAccessor::GetRenameBitmapExtension(int i, wxString& from, wxString& to)
{
    from = ApplicationStringToWxString(MPlug(this->obj, FinjinSceneSettingsNode::attributes.renameBitmapExtensionsFrom)[i].asString());
    to = ApplicationStringToWxString(MPlug(this->obj, FinjinSceneSettingsNode::attributes.renameBitmapExtensionsTo)[i].asString());
}

bool FinjinSceneSettingsAccessor::GetExportEnvironment()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportEnvironment).asBool();
}

FinjinColor FinjinSceneSettingsAccessor::GetAmbientLightColor()
{
    return MayaPlug::GetColor(this->obj, FinjinSceneSettingsNode::attributes.ambientLightColor);
}

ValueSourceSync FinjinSceneSettingsAccessor::GetAmbientLightColorSync()
{
    return (ValueSourceSync)MPlug(this->obj, FinjinSceneSettingsNode::attributes.ambientLightColorSync).asInt();
}

FinjinColor FinjinSceneSettingsAccessor::GetBackgroundColor()
{
    return MayaPlug::GetColor(this->obj, FinjinSceneSettingsNode::attributes.backgroundColor);
}

ValueSourceSync FinjinSceneSettingsAccessor::GetBackgroundColorSync()
{
    return (ValueSourceSync)MPlug(this->obj, FinjinSceneSettingsNode::attributes.backgroundColorSync).asInt();
}

float FinjinSceneSettingsAccessor::GetEnvironmentNear()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.environmentNear).asMDistance().as(INTERNAL_UNIT_TYPE);
}

float FinjinSceneSettingsAccessor::GetEnvironmentFar()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.environmentFar).asMDistance().as(INTERNAL_UNIT_TYPE);
}

ObjectAccessor FinjinSceneSettingsAccessor::GetEnvironmentFarObject()
{
    return MayaPlug::GetObjectReference(this->obj, FinjinSceneSettingsNode::attributes.environmentFarObject);
}

ValueSourceSync FinjinSceneSettingsAccessor::GetFogSync()
{
    return (ValueSourceSync)MPlug(this->obj, FinjinSceneSettingsNode::attributes.fogSync).asInt();    
}

wxString FinjinSceneSettingsAccessor::GetFogMode()
{
    return ApplicationStringToWxString(MPlug(this->obj, FinjinSceneSettingsNode::attributes.fogMode).asString());
}

float FinjinSceneSettingsAccessor::GetFogDensity()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.fogDensity).asFloat();
}

float FinjinSceneSettingsAccessor::GetFogStart()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.fogStart).asFloat();
}

float FinjinSceneSettingsAccessor::GetFogEnd()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.fogEnd).asFloat();
}

FinjinColor FinjinSceneSettingsAccessor::GetFogColor()
{
    return MayaPlug::GetColor(this->obj, FinjinSceneSettingsNode::attributes.fogColor);
}

wxString FinjinSceneSettingsAccessor::GetShadowTechnique()
{
    return ApplicationStringToWxString(MPlug(this->obj, FinjinSceneSettingsNode::attributes.shadowTechnique).asString());
}

FinjinColor FinjinSceneSettingsAccessor::GetShadowColor()
{
    return MayaPlug::GetColor(this->obj, FinjinSceneSettingsNode::attributes.shadowColor);
}

float FinjinSceneSettingsAccessor::GetShadowFarDistance()
{
    return MPlug(this->obj, FinjinSceneSettingsNode::attributes.shadowFarDistance).asFloat();
}

void FinjinSceneSettingsAccessor::SetSceneScaleUnit(wxString value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.sceneScaleUnit).setValue(WxStringToApplicationString(value));
}

void FinjinSceneSettingsAccessor::SetSceneScaleDivide(float value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.sceneScaleDivide).setValue(value);
}

void FinjinSceneSettingsAccessor::SetExportHierarchy(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportHierarchy).setValue(value);
}

void FinjinSceneSettingsAccessor::SetIgnoreHiddenObjects(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.ignoreHiddenObjects).setValue(value);
}

void FinjinSceneSettingsAccessor::SetSceneManager(wxString value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.sceneManager).setValue(WxStringToApplicationString(value));
}

void FinjinSceneSettingsAccessor::SetExportFlags(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportFlags).setValue(value);
}

void FinjinSceneSettingsAccessor::ClearFlags()
{
    MayaPlug::RemoveAttributeElements(this->obj, FinjinSceneSettingsNode::attributes.flagNames);    
}

void FinjinSceneSettingsAccessor::AddFlag(wxString name)
{
    MayaPlug::AddAttributeElementValue(this->obj, FinjinSceneSettingsNode::attributes.flagNames, name);
}

void FinjinSceneSettingsAccessor::SetNodeAnimationSampleInterval(double value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.nodeAnimationSampleInterval).setValue(value);
}

void FinjinSceneSettingsAccessor::SetNodeAnimationSampleType(SampleType value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.nodeAnimationSampleType).setValue((int)value);
}

void FinjinSceneSettingsAccessor::SetConfigurationTargetName(wxString value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.configurationTargetName).setValue(WxStringToApplicationString(value));
}

void FinjinSceneSettingsAccessor::SetSkeletonAnimationSampleInterval(double value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.skeletonAnimationSampleInterval).setValue(value);
}

void FinjinSceneSettingsAccessor::SetSkeletonAnimationSampleType(SampleType value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.skeletonAnimationSampleType).setValue((int)value);
}

void FinjinSceneSettingsAccessor::SetVertexAnimationSampleInterval(double value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.vertexAnimationSampleInterval).setValue(value);
}

void FinjinSceneSettingsAccessor::SetVertexAnimationSampleType(SampleType value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.vertexAnimationSampleType).setValue((int)value);
}

void FinjinSceneSettingsAccessor::SetSubmeshNaming(wxString value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.submeshNaming).setValue(WxStringToApplicationString(value));
}

void FinjinSceneSettingsAccessor::SetSubmeshCustomName(wxString value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.submeshCustomName).setValue(WxStringToApplicationString(value));
}

void FinjinSceneSettingsAccessor::SetExportVertexColors(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportVertexColors).setValue(value);
}

void FinjinSceneSettingsAccessor::SetVertexColorFormat(wxString value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.vertexColorFormat).setValue(WxStringToApplicationString(value));
}

void FinjinSceneSettingsAccessor::SetUseExplicitVertexColor(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.useExplicitVertexColor).setValue(value);
}

void FinjinSceneSettingsAccessor::SetExportTangents(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportTangents).setValue(value);
}

void FinjinSceneSettingsAccessor::SetExportBinormals(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportBinormals).setValue(value);
}

void FinjinSceneSettingsAccessor::SetCheckMeshInstances(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.checkMeshInstances).setValue(value);
}

void FinjinSceneSettingsAccessor::SetAlwaysCreateNewSubmeshesWhenMerging(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.alwaysCreateNewSubmeshesWhenMerging).setValue(value);
}

void FinjinSceneSettingsAccessor::SetAnimateBoundingVolumes(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.animateBoundingVolumes).setValue(value);
}

void FinjinSceneSettingsAccessor::SetExportMeshes(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportMeshes).setValue(value);
}

void FinjinSceneSettingsAccessor::ClearDisallowedMeshes()
{
    MayaPlug::RemoveAttributeElements(this->obj, FinjinSceneSettingsNode::attributes.disallowedMeshes);
}

void FinjinSceneSettingsAccessor::AddDisallowedMesh(ObjectAccessor value)
{
    MayaPlug::AddObjectReference(this->obj, FinjinSceneSettingsNode::attributes.disallowedMeshes, value.obj);    
}

void FinjinSceneSettingsAccessor::SetCreateExtraMaterials(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.createExtraMaterials).setValue(value);
}

void FinjinSceneSettingsAccessor::SetIncludeParentMaterialName(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.includeParentMaterialName).setValue(value);
}

void FinjinSceneSettingsAccessor::SetNoMaterialName(wxString value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.noMaterialName).setValue(WxStringToApplicationString(value));
}

void FinjinSceneSettingsAccessor::SetExportMaterials(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportMaterials).setValue(value);
}

void FinjinSceneSettingsAccessor::ClearAdditionalMaterials()
{
    MayaPlug::RemoveAttributeElements(this->obj, FinjinSceneSettingsNode::attributes.additionalMaterials);
}

void FinjinSceneSettingsAccessor::AddAdditionalMaterial(MaterialAccessor value)
{
    MayaPlug::AddObjectReference(this->obj, FinjinSceneSettingsNode::attributes.additionalMaterials, value.obj);
}

void FinjinSceneSettingsAccessor::ClearDisallowedMaterials()
{
    MayaPlug::RemoveAttributeElements(this->obj, FinjinSceneSettingsNode::attributes.disallowedMaterials);
}

void FinjinSceneSettingsAccessor::AddDisallowedMaterial(MaterialAccessor value)
{
    MayaPlug::AddObjectReference(this->obj, FinjinSceneSettingsNode::attributes.disallowedMaterials, value.obj);
}

void FinjinSceneSettingsAccessor::ClearRenameBitmaps()
{
    MayaPlug::RemoveAttributeElements(this->obj, FinjinSceneSettingsNode::attributes.renameBitmapsFrom);
    MayaPlug::RemoveAttributeElements(this->obj, FinjinSceneSettingsNode::attributes.renameBitmapsTo);    
}

void FinjinSceneSettingsAccessor::AddRenameBitmap(wxString from, wxString to)
{
    MayaPlug::AddAttributeElementValue(this->obj, FinjinSceneSettingsNode::attributes.renameBitmapsFrom, from);
    MayaPlug::AddAttributeElementValue(this->obj, FinjinSceneSettingsNode::attributes.renameBitmapsTo, to);
}

void FinjinSceneSettingsAccessor::ClearRenameBitmapExtensions()
{
    MayaPlug::RemoveAttributeElements(this->obj, FinjinSceneSettingsNode::attributes.renameBitmapExtensionsFrom);
    MayaPlug::RemoveAttributeElements(this->obj, FinjinSceneSettingsNode::attributes.renameBitmapExtensionsTo);
}

void FinjinSceneSettingsAccessor::AddRenameBitmapExtension(wxString from, wxString to)
{
    MayaPlug::AddAttributeElementValue(this->obj, FinjinSceneSettingsNode::attributes.renameBitmapExtensionsFrom, from);
    MayaPlug::AddAttributeElementValue(this->obj, FinjinSceneSettingsNode::attributes.renameBitmapExtensionsTo, to);
}

void FinjinSceneSettingsAccessor::SetExportEnvironment(bool value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.exportEnvironment).setValue(value);
}

void FinjinSceneSettingsAccessor::SetAmbientLightColor(FinjinColor value)
{
    MayaPlug::SetColor(this->obj, FinjinSceneSettingsNode::attributes.ambientLightColor, value);
}

void FinjinSceneSettingsAccessor::SetAmbientLightColorSync(ValueSourceSync value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.ambientLightColorSync).setValue((int)value);
}

void FinjinSceneSettingsAccessor::SetBackgroundColor(FinjinColor value)
{
    MayaPlug::SetColor(this->obj, FinjinSceneSettingsNode::attributes.backgroundColor, value);
}

void FinjinSceneSettingsAccessor::SetBackgroundColorSync(ValueSourceSync value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.backgroundColorSync).setValue((int)value);
}

void FinjinSceneSettingsAccessor::SetEnvironmentNear(float value)
{
    MDistance distance(value, INTERNAL_UNIT_TYPE);
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.environmentNear).setValue(distance);
}

void FinjinSceneSettingsAccessor::SetEnvironmentFar(float value)
{
    MDistance distance(value, INTERNAL_UNIT_TYPE);
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.environmentFar).setValue(distance);
}

void FinjinSceneSettingsAccessor::SetEnvironmentFarObject(ObjectAccessor value)
{
    MayaPlug::SetObjectReference(this->obj, FinjinSceneSettingsNode::attributes.environmentFarObject, value.obj);    
}

void FinjinSceneSettingsAccessor::SetFogSync(ValueSourceSync value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.fogSync).setValue((int)value);
}

void FinjinSceneSettingsAccessor::SetFogMode(wxString value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.fogMode).setValue(WxStringToApplicationString(value));
}

void FinjinSceneSettingsAccessor::SetFogDensity(float value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.fogDensity).setValue(value);
}

void FinjinSceneSettingsAccessor::SetFogStart(float value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.fogStart).setValue(value);
}

void FinjinSceneSettingsAccessor::SetFogEnd(float value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.fogEnd).setValue(value);
}

void FinjinSceneSettingsAccessor::SetFogColor(FinjinColor value)
{
    MayaPlug::SetColor(this->obj, FinjinSceneSettingsNode::attributes.fogColor, value);
}

void FinjinSceneSettingsAccessor::SetShadowTechnique(wxString value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.shadowTechnique).setValue(WxStringToApplicationString(value));
}

void FinjinSceneSettingsAccessor::SetShadowColor(FinjinColor value)
{
    MayaPlug::SetColor(this->obj, FinjinSceneSettingsNode::attributes.shadowColor, value);
}
void FinjinSceneSettingsAccessor::SetShadowFarDistance(float value)
{
    MPlug(this->obj, FinjinSceneSettingsNode::attributes.shadowFarDistance).setValue(value);
}
