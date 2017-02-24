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
#include "FinjinSceneSettingsObject.hpp"
#include "MaxControlUtilities.hpp"
#include "FinjinGlobalSettings.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
FinjinSceneSettingsAccessor FinjinSceneSettingsAccessor::GetSceneSettings(bool createIfNecessary, bool* newInstance)
{
    bool isNew;
    FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsObject::GetSceneSettings(createIfNecessary, &isNew);
    if (isNew)
        FinjinGlobalSettings::GetInstance().GetSceneSettingsDefaults(sceneSettings);
    
    if (newInstance != nullptr)
        *newInstance = isNew;

    return sceneSettings;
}

FinjinObjectSettingsAccessor FinjinSceneSettingsAccessor::GetObjectSettings(ObjectAccessor object, bool createIfNecessary, bool* newInstance)
{
    return this->object->GetObjectSettings(object.node, createIfNecessary, newInstance);
}

void FinjinSceneSettingsAccessor::MergeSceneSettings()
{
    auto currentSceneSettings = FinjinSceneSettingsObject::GetSceneSettingsByIndex(0);
    if (currentSceneSettings != nullptr)
    {
        INode* newSceneSettingsNode;
        FinjinSceneSettingsObject* newSceneSettings;
        while ((newSceneSettings = FinjinSceneSettingsObject::GetSceneSettingsByIndex(1, &newSceneSettingsNode)) != nullptr)
        {
            currentSceneSettings->MergeObjectSettings(newSceneSettings);
            GetCOREInterface()->DeleteNode(newSceneSettingsNode, FALSE);
        }
    }
}

void FinjinSceneSettingsAccessor::CleanObjectSettings()
{
    this->object->CleanObjectSettings();
}

void FinjinSceneSettingsAccessor::RemoveObjectSettings(ObjectAccessor object)
{
    this->object->RemoveObjectSettings(object.node);
}

FINJIN_EXPORTER_IMPLEMENT_USER_DATA_ACCESSOR_METHODS(FinjinSceneSettingsAccessor, object)

FINJIN_EXPORTER_IMPLEMENT_EXTENDED_VALUE_ACCESSOR_METHODS(FinjinSceneSettingsAccessor, object)

wxString FinjinSceneSettingsAccessor::GetSceneScaleUnit()
{
    return this->object->GetSceneScaleUnit();    
}

float FinjinSceneSettingsAccessor::GetSceneScaleDivide()
{
    return this->object->GetSceneScaleDivide();
}

bool FinjinSceneSettingsAccessor::GetExportHierarchy()
{
    return this->object->GetExportHierarchy();
}

bool FinjinSceneSettingsAccessor::GetIgnoreHiddenObjects()
{
    return this->object->GetIgnoreHiddenObjects();
}

wxString FinjinSceneSettingsAccessor::GetSceneManager()
{
    return this->object->GetSceneManager();    
}

bool FinjinSceneSettingsAccessor::GetExportFlags()
{
    return this->object->GetExportFlags();
}

int FinjinSceneSettingsAccessor::GetFlagCount()
{
    return this->object->GetFlagCount();
}

void FinjinSceneSettingsAccessor::GetFlag(int i, wxString& name)
{
    this->object->GetFlag(i, name);    
}

double FinjinSceneSettingsAccessor::GetNodeAnimationSampleInterval()
{
    return this->object->GetNodeAnimationSampleInterval();
}

SampleType FinjinSceneSettingsAccessor::GetNodeAnimationSampleType()
{
    return this->object->GetNodeAnimationSampleType();
}

wxString FinjinSceneSettingsAccessor::GetConfigurationTargetName()
{
    return this->object->GetConfigurationTargetName();
}

double FinjinSceneSettingsAccessor::GetSkeletonAnimationSampleInterval()
{
    return this->object->GetSkeletonAnimationSampleInterval();
}

SampleType FinjinSceneSettingsAccessor::GetSkeletonAnimationSampleType()
{
    return this->object->GetSkeletonAnimationSampleType();
}

double FinjinSceneSettingsAccessor::GetVertexAnimationSampleInterval()
{
    return this->object->GetVertexAnimationSampleInterval();
}

SampleType FinjinSceneSettingsAccessor::GetVertexAnimationSampleType()
{
    return this->object->GetVertexAnimationSampleType();
}

wxString FinjinSceneSettingsAccessor::GetSubmeshNaming()
{
    return this->object->GetSubmeshNaming();
}

wxString FinjinSceneSettingsAccessor::GetSubmeshCustomName()
{
    return this->object->GetSubmeshCustomName();
}

bool FinjinSceneSettingsAccessor::GetExportVertexColors()
{
    return this->object->GetExportVertexColors();
}

wxString FinjinSceneSettingsAccessor::GetVertexColorFormat()
{
    return this->object->GetVertexColorFormat();    
}

bool FinjinSceneSettingsAccessor::GetUseExplicitVertexColor()
{
    return this->object->GetUseExplicitVertexColor();
}

bool FinjinSceneSettingsAccessor::GetExportTangents()
{
    return this->object->GetExportTangents();
}

bool FinjinSceneSettingsAccessor::GetExportBinormals()
{
    return this->object->GetExportBinormals();
}

bool FinjinSceneSettingsAccessor::GetAnimateBoundingVolumes()
{
    return this->object->GetAnimateBoundingVolumes();
}

bool FinjinSceneSettingsAccessor::GetCheckMeshInstances()
{
    return this->object->GetCheckMeshInstances();
}

bool FinjinSceneSettingsAccessor::GetAlwaysCreateNewSubmeshesWhenMerging()
{
    return this->object->GetAlwaysCreateNewSubmeshesWhenMerging();    
}

bool FinjinSceneSettingsAccessor::GetExportMeshes()
{
    return this->object->GetExportMeshes();
}

int FinjinSceneSettingsAccessor::GetDisallowedMeshCount()
{
    return this->object->GetDisallowedMeshCount();
}

ObjectAccessor FinjinSceneSettingsAccessor::GetDisallowedMesh(int i)
{
    return this->object->GetDisallowedMesh(i);
}

bool FinjinSceneSettingsAccessor::GetCreateExtraMaterials()
{
    return this->object->GetCreateExtraMaterials();
}

bool FinjinSceneSettingsAccessor::GetIncludeParentMaterialName()
{
    return this->object->GetIncludeParentMaterialName();
}

wxString FinjinSceneSettingsAccessor::GetNoMaterialName()
{
    return this->object->GetNoMaterialName();    
}

bool FinjinSceneSettingsAccessor::GetExportMaterials()
{
    return this->object->GetExportMaterials();
}

int FinjinSceneSettingsAccessor::GetAdditionalMaterialCount()
{
    return this->object->GetAdditionalMaterialCount();
}

MaterialAccessor FinjinSceneSettingsAccessor::GetAdditionalMaterial(int i)
{
    return this->object->GetAdditionalMaterial(i);
}

int FinjinSceneSettingsAccessor::GetDisallowedMaterialCount()
{
    return this->object->GetDisallowedMaterialCount();
}

MaterialAccessor FinjinSceneSettingsAccessor::GetDisallowedMaterial(int i)
{
    return this->object->GetDisallowedMaterial(i);
}

int FinjinSceneSettingsAccessor::GetRenameBitmapCount()
{
    return this->object->GetRenameBitmapCount();
}

void FinjinSceneSettingsAccessor::GetRenameBitmap(int i, wxString& from, wxString& to)
{
    this->object->GetRenameBitmap(i, from, to);
}

int FinjinSceneSettingsAccessor::GetRenameBitmapExtensionCount()
{
    return this->object->GetRenameBitmapExtensionCount();
}

void FinjinSceneSettingsAccessor::GetRenameBitmapExtension(int i, wxString& from, wxString& to)
{
    this->object->GetRenameBitmapExtension(i, from, to);
}

float FinjinSceneSettingsAccessor::GetEnvironmentNear()
{
    return this->object->GetEnvironmentNear();
}

bool FinjinSceneSettingsAccessor::GetExportEnvironment()
{
    return this->object->GetExportEnvironment();
}

FinjinColor FinjinSceneSettingsAccessor::GetAmbientLightColor()
{
    Color color = this->object->GetAmbientLightColor();
    return FinjinColor(color.r, color.g, color.b);
}

ValueSourceSync FinjinSceneSettingsAccessor::GetAmbientLightColorSync()
{
    return this->object->GetAmbientLightColorSync();
}

FinjinColor FinjinSceneSettingsAccessor::GetBackgroundColor()
{
    Color color = this->object->GetBackgroundColor();    
    return FinjinColor(color.r, color.g, color.b);
}

ValueSourceSync FinjinSceneSettingsAccessor::GetBackgroundColorSync()
{
    return this->object->GetBackgroundColorSync();
}

float FinjinSceneSettingsAccessor::GetEnvironmentFar()
{
    return this->object->GetEnvironmentFar();
}

ObjectAccessor FinjinSceneSettingsAccessor::GetEnvironmentFarObject()
{
    return this->object->GetEnvironmentFarObject();
}

ValueSourceSync FinjinSceneSettingsAccessor::GetFogSync()
{
    return this->object->GetFogSync();
}
        
wxString FinjinSceneSettingsAccessor::GetFogMode()
{
    return this->object->GetFogMode();
}

float FinjinSceneSettingsAccessor::GetFogDensity()
{
    return this->object->GetFogDensity();
}

float FinjinSceneSettingsAccessor::GetFogStart()
{
    return this->object->GetFogStart();
}

float FinjinSceneSettingsAccessor::GetFogEnd()
{
    return this->object->GetFogEnd();
}

FinjinColor FinjinSceneSettingsAccessor::GetFogColor()
{
    Color color = this->object->GetFogColor();
    return FinjinColor(color.r, color.g, color.b);
}

wxString FinjinSceneSettingsAccessor::GetShadowTechnique()
{
    return this->object->GetShadowTechnique();
}

FinjinColor FinjinSceneSettingsAccessor::GetShadowColor()
{
    Color color = this->object->GetShadowColor();
    return FinjinColor(color.r, color.g, color.b);
}

float FinjinSceneSettingsAccessor::GetShadowFarDistance()
{
    return this->object->GetShadowFarDistance();
}

void FinjinSceneSettingsAccessor::SetSceneScaleUnit(wxString value)
{
    this->object->SetSceneScaleUnit(value);    
}

void FinjinSceneSettingsAccessor::SetSceneScaleDivide(float value)
{
    this->object->SetSceneScaleDivide(value);        
}

void FinjinSceneSettingsAccessor::SetExportHierarchy(bool value)
{
    this->object->SetExportHierarchy(value);
}

void FinjinSceneSettingsAccessor::SetIgnoreHiddenObjects(bool value)
{
    this->object->SetIgnoreHiddenObjects(value);    
}

void FinjinSceneSettingsAccessor::SetSceneManager(wxString value)
{
    this->object->SetSceneManager(value);    
}

void FinjinSceneSettingsAccessor::SetExportFlags(bool value)
{
    this->object->SetExportFlags(value);
}

void FinjinSceneSettingsAccessor::ClearFlags()
{
    this->object->ClearFlags();
}

void FinjinSceneSettingsAccessor::AddFlag(wxString name)
{
    this->object->AddFlag(name);
}

void FinjinSceneSettingsAccessor::SetNodeAnimationSampleInterval(double value)
{
    this->object->SetNodeAnimationSampleInterval(value);    
}

void FinjinSceneSettingsAccessor::SetNodeAnimationSampleType(SampleType value)
{
    this->object->SetNodeAnimationSampleType(value);    
}

void FinjinSceneSettingsAccessor::SetConfigurationTargetName(wxString value)
{
    this->object->SetConfigurationTargetName(value);
}

void FinjinSceneSettingsAccessor::SetSkeletonAnimationSampleInterval(double value)
{
    this->object->SetSkeletonAnimationSampleInterval(value);    
}

void FinjinSceneSettingsAccessor::SetSkeletonAnimationSampleType(SampleType value)
{
    this->object->SetSkeletonAnimationSampleType(value);    
}

void FinjinSceneSettingsAccessor::SetVertexAnimationSampleInterval(double value)
{
    this->object->SetVertexAnimationSampleInterval(value);    
}

void FinjinSceneSettingsAccessor::SetVertexAnimationSampleType(SampleType value)
{
    this->object->SetVertexAnimationSampleType(value);    
}

void FinjinSceneSettingsAccessor::SetSubmeshNaming(wxString value)
{
    this->object->SetSubmeshNaming(value);
}

void FinjinSceneSettingsAccessor::SetSubmeshCustomName(wxString value)
{
    this->object->SetSubmeshCustomName(value);
}

void FinjinSceneSettingsAccessor::SetExportVertexColors(bool value)
{
    this->object->SetExportVertexColors(value);    
}

void FinjinSceneSettingsAccessor::SetVertexColorFormat(wxString value)
{
    this->object->SetVertexColorFormat(value);
}

void FinjinSceneSettingsAccessor::SetUseExplicitVertexColor(bool value)
{
    this->object->SetUseExplicitVertexColor(value);
}

void FinjinSceneSettingsAccessor::SetExportTangents(bool value)
{
    this->object->SetExportTangents(value);    
}

void FinjinSceneSettingsAccessor::SetExportBinormals(bool value)
{
    this->object->SetExportBinormals(value);    
}

void FinjinSceneSettingsAccessor::SetAnimateBoundingVolumes(bool value)
{
    this->object->SetAnimateBoundingVolumes(value);
}

void FinjinSceneSettingsAccessor::SetCheckMeshInstances(bool value)
{
    this->object->SetCheckMeshInstances(value);    
}

void FinjinSceneSettingsAccessor::SetAlwaysCreateNewSubmeshesWhenMerging(bool value)
{
    this->object->SetAlwaysCreateNewSubmeshesWhenMerging(value);    
}

void FinjinSceneSettingsAccessor::SetExportMeshes(bool value)
{
    this->object->SetExportMeshes(value);    
}

void FinjinSceneSettingsAccessor::ClearDisallowedMeshes()
{
    this->object->ClearDisallowedMeshes();
}

void FinjinSceneSettingsAccessor::AddDisallowedMesh(ObjectAccessor value)
{
    this->object->AddDisallowedMesh(value.node);
}

void FinjinSceneSettingsAccessor::SetCreateExtraMaterials(bool value)
{
    this->object->SetCreateExtraMaterials(value);
}

void FinjinSceneSettingsAccessor::SetIncludeParentMaterialName(bool value)
{
    this->object->SetIncludeParentMaterialName(value);
}

void FinjinSceneSettingsAccessor::SetNoMaterialName(wxString value)
{
    this->object->SetNoMaterialName(value);    
}

void FinjinSceneSettingsAccessor::SetExportMaterials(bool value)
{
    this->object->SetExportMaterials(value);
}

void FinjinSceneSettingsAccessor::ClearAdditionalMaterials()
{
    this->object->ClearAdditionalMaterials();
}

void FinjinSceneSettingsAccessor::AddAdditionalMaterial(MaterialAccessor value)
{
    this->object->AddAdditionalMaterial(value.mtl);
}

void FinjinSceneSettingsAccessor::ClearDisallowedMaterials()
{
    this->object->ClearDisallowedMaterials();
}

void FinjinSceneSettingsAccessor::AddDisallowedMaterial(MaterialAccessor value)
{
    this->object->AddDisallowedMaterial(value.mtl);
}

void FinjinSceneSettingsAccessor::ClearRenameBitmaps()
{
    this->object->ClearRenameBitmaps();
}

void FinjinSceneSettingsAccessor::AddRenameBitmap(wxString from, wxString to)
{
    this->object->AddRenameBitmap(from, to);
}

void FinjinSceneSettingsAccessor::ClearRenameBitmapExtensions()
{
    this->object->ClearRenameBitmapExtensions();
}

void FinjinSceneSettingsAccessor::AddRenameBitmapExtension(wxString from, wxString to)
{
    this->object->AddRenameBitmapExtension(from, to);
}

void FinjinSceneSettingsAccessor::SetEnvironmentNear(float value)
{
    this->object->SetEnvironmentNear(value);
}

void FinjinSceneSettingsAccessor::SetExportEnvironment(bool value)
{
    this->object->SetExportEnvironment(value);
}

void FinjinSceneSettingsAccessor::SetAmbientLightColor(FinjinColor value)
{
    this->object->SetAmbientLightColor(Color(value.r, value.g, value.b));
}

void FinjinSceneSettingsAccessor::SetAmbientLightColorSync(ValueSourceSync value)
{
    this->object->SetAmbientLightColorSync(value);
}

void FinjinSceneSettingsAccessor::SetBackgroundColor(FinjinColor value)
{
    this->object->SetBackgroundColor(Color(value.r, value.g, value.b));
}

void FinjinSceneSettingsAccessor::SetBackgroundColorSync(ValueSourceSync value)
{
    this->object->SetBackgroundColorSync(value);
}

void FinjinSceneSettingsAccessor::SetEnvironmentFar(float value)
{
    this->object->SetEnvironmentFar(value);
}

void FinjinSceneSettingsAccessor::SetEnvironmentFarObject(ObjectAccessor value)
{
    this->object->SetEnvironmentFarObject(value.node);    
}

void FinjinSceneSettingsAccessor::SetFogSync(ValueSourceSync value)
{
    this->object->SetFogSync(value);
}
        
void FinjinSceneSettingsAccessor::SetFogMode(wxString value)
{
    this->object->SetFogMode(value);
}

void FinjinSceneSettingsAccessor::SetFogDensity(float value)
{
    this->object->SetFogDensity(value);
}

void FinjinSceneSettingsAccessor::SetFogStart(float value)
{
    this->object->SetFogStart(value);
}

void FinjinSceneSettingsAccessor::SetFogEnd(float value)
{
    this->object->SetFogEnd(value);
}

void FinjinSceneSettingsAccessor::SetFogColor(FinjinColor value)
{
    Color color(value.r, value.g, value.b);
    this->object->SetFogColor(color);
}

void FinjinSceneSettingsAccessor::SetShadowTechnique(wxString value)
{
    this->object->SetShadowTechnique(value);
}

void FinjinSceneSettingsAccessor::SetShadowColor(FinjinColor value)
{
    this->object->SetShadowColor(Color(value.r, value.g, value.b));
}

void FinjinSceneSettingsAccessor::SetShadowFarDistance(float value)
{
    this->object->SetShadowFarDistance(value);
}
