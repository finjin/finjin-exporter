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
#include "FinjinObjectSettingsAccessor.hpp"
#include "FinjinSceneSettingsAccessorBase.hpp"
#include "BaseDirectory.hpp"
#include "UserDataSettingsAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class FinjinSceneSettingsObject;

    /** Access to a Finjin scene settings object. */
    class FinjinSceneSettingsAccessor : public FinjinSceneSettingsAccessorBase
    {
    public:
        FinjinSceneSettingsAccessor() {this->object = nullptr;}
        FinjinSceneSettingsAccessor(FinjinSceneSettingsObject* object) {this->object = object;}

        bool IsValid() const {return this->object != nullptr;}

        static FinjinSceneSettingsAccessor GetSceneSettings(bool createIfNecessary = true, bool* newInstance = nullptr);

        FinjinObjectSettingsAccessor GetObjectSettings(ObjectAccessor object, bool createIfNecessary = true, bool* newInstance = nullptr) override;

        static void MergeSceneSettings();
        void CleanObjectSettings();

        void RemoveObjectSettings(ObjectAccessor object);

        FINJIN_EXPORTER_DECLARE_USER_DATA_ACCESSOR_METHODS_OVERRIDE

        FINJIN_EXPORTER_DECLARE_EXTENDED_VALUE_ACCESSOR_METHODS_OVERRIDE

        wxString GetSceneScaleUnit() override;
        float GetSceneScaleDivide() override;
        bool GetExportHierarchy() override;
        bool GetIgnoreHiddenObjects() override;
        wxString GetSceneManager() override;
        bool GetExportFlags() override;
        int GetFlagCount() override;
        void GetFlag(int i, wxString& name) override;
        double GetNodeAnimationSampleInterval() override;
        SampleType GetNodeAnimationSampleType() override;
        wxString GetConfigurationTargetName() override;
        double GetSkeletonAnimationSampleInterval() override;
        SampleType GetSkeletonAnimationSampleType() override;
        double GetVertexAnimationSampleInterval() override;
        SampleType GetVertexAnimationSampleType() override;
        wxString GetSubmeshNaming() override;
        wxString GetSubmeshCustomName() override;
        bool GetExportVertexColors() override;
        wxString GetVertexColorFormat() override;
        bool GetUseExplicitVertexColor() override;
        bool GetExportTangents() override;
        bool GetExportBinormals() override;
        bool GetAnimateBoundingVolumes() override;
        bool GetCheckMeshInstances() override;
        bool GetAlwaysCreateNewSubmeshesWhenMerging() override;
        bool GetExportMeshes() override;
        int GetDisallowedMeshCount() override;
        ObjectAccessor GetDisallowedMesh(int i) override;
        bool GetCreateExtraMaterials() override;
        bool GetIncludeParentMaterialName() override;
        wxString GetNoMaterialName() override;
        bool GetExportMaterials() override;
        int GetAdditionalMaterialCount() override;
        MaterialAccessor GetAdditionalMaterial(int i) override;
        int GetDisallowedMaterialCount() override;
        MaterialAccessor GetDisallowedMaterial(int i) override;
        int GetRenameBitmapCount() override;
        void GetRenameBitmap(int i, wxString& from, wxString& to) override;
        int GetRenameBitmapExtensionCount() override;
        void GetRenameBitmapExtension(int i, wxString& from, wxString& to) override;
        bool GetExportEnvironment() override;
        FinjinColor GetAmbientLightColor() override;
        ValueSourceSync GetAmbientLightColorSync() override;
        FinjinColor GetBackgroundColor() override;
        ValueSourceSync GetBackgroundColorSync() override;
        float GetEnvironmentNear() override;
        float GetEnvironmentFar() override;
        ObjectAccessor GetEnvironmentFarObject() override;
        ValueSourceSync GetFogSync() override;
        wxString GetFogMode() override;
        float GetFogDensity() override;
        float GetFogStart() override;
        float GetFogEnd() override;
        FinjinColor GetFogColor() override;
        wxString GetShadowTechnique() override;
        FinjinColor GetShadowColor() override;
        float GetShadowFarDistance() override;

        void SetSceneScaleUnit(wxString value) override;
        void SetSceneScaleDivide(float value) override;
        void SetExportHierarchy(bool value) override;
        void SetIgnoreHiddenObjects(bool value) override;
        void SetSceneManager(wxString value) override;
        void SetExportFlags(bool value) override;
        void ClearFlags() override;
        void AddFlag(wxString name) override;
        void SetNodeAnimationSampleInterval(double value) override;
        void SetNodeAnimationSampleType(SampleType value) override;
        void SetConfigurationTargetName(wxString value) override;
        void SetSkeletonAnimationSampleInterval(double value) override;
        void SetSkeletonAnimationSampleType(SampleType value) override;
        void SetVertexAnimationSampleInterval(double value) override;
        void SetVertexAnimationSampleType(SampleType value) override;
        void SetSubmeshNaming(wxString value) override;
        void SetSubmeshCustomName(wxString value) override;
        void SetExportVertexColors(bool value) override;
        void SetVertexColorFormat(wxString value) override;
        void SetUseExplicitVertexColor(bool value) override;
        void SetExportTangents(bool value) override;
        void SetExportBinormals(bool value) override;
        void SetAnimateBoundingVolumes(bool value) override;
        void SetCheckMeshInstances(bool value) override;
        void SetAlwaysCreateNewSubmeshesWhenMerging(bool value) override;
        void SetExportMeshes(bool value) override;
        void ClearDisallowedMeshes() override;
        void AddDisallowedMesh(ObjectAccessor value) override;
        void SetCreateExtraMaterials(bool value) override;
        void SetIncludeParentMaterialName(bool value) override;
        void SetNoMaterialName(wxString value) override;
        void SetExportMaterials(bool value) override;
        void ClearAdditionalMaterials() override;
        void AddAdditionalMaterial(MaterialAccessor value) override;
        void ClearDisallowedMaterials() override;
        void AddDisallowedMaterial(MaterialAccessor value) override;
        void ClearRenameBitmaps() override;
        void AddRenameBitmap(wxString from, wxString to) override;
        void ClearRenameBitmapExtensions() override;
        void AddRenameBitmapExtension(wxString from, wxString to) override;
        void SetExportEnvironment(bool value) override;
        void SetAmbientLightColor(FinjinColor value) override;
        void SetAmbientLightColorSync(ValueSourceSync value) override;
        void SetBackgroundColor(FinjinColor value) override;
        void SetBackgroundColorSync(ValueSourceSync value) override;
        void SetEnvironmentNear(float value) override;
        void SetEnvironmentFar(float value) override;
        void SetEnvironmentFarObject(ObjectAccessor value) override;
        void SetFogSync(ValueSourceSync value) override;
        void SetFogMode(wxString value) override;
        void SetFogDensity(float value) override;
        void SetFogStart(float value) override;
        void SetFogEnd(float value) override;
        void SetFogColor(FinjinColor value) override;
        void SetShadowTechnique(wxString value) override;
        void SetShadowColor(FinjinColor value) override;
        void SetShadowFarDistance(float value) override;

    public:
        FinjinSceneSettingsObject* object;
    };

} }
