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
#include "ObjectAccessor.hpp"
#include "UserDataSettingsAccessor.hpp"
#include "MaterialAccessor.hpp"
#include "UserDataSettingsAccessor.hpp"
#include "ExtendedValueAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * FinjinSceneSettingsAccessorBase and the application-specific FinjinSceneSettingsAccessor
     * classes provide generic access to an Finjin scene settings object.
     */
    class FinjinSceneSettingsAccessorBase :
        public UserDataSettingsAccessor,
        public ExtendedValueAccessor
    {
    public:
        virtual ~FinjinSceneSettingsAccessorBase() {}

        /**
         * Gets the object settings for the specified object.
         * @param object [in] - The object whose settings are retrieved.
         * @param createIfNecessary [in] - Indicates whether object settings should be created
         * if they don't already exist.
         * @param newInstance [out] - If specified, receives a boolean indicating whether the returned
         * object settings are newly created.
         * @return The object settings are returned.
         */
        virtual FinjinObjectSettingsAccessor GetObjectSettings
            (
            ObjectAccessor object,
            bool createIfNecessary = true,
            bool* newInstance = nullptr
            ) = 0;

        virtual wxString GetSceneScaleUnit() = 0;
        virtual float GetSceneScaleDivide() = 0;
        virtual bool GetExportHierarchy() = 0;
        virtual bool GetIgnoreHiddenObjects() = 0;
        virtual wxString GetSceneManager() = 0;
        virtual bool GetExportFlags() = 0;
        virtual int GetFlagCount() = 0;
        virtual void GetFlag(int i, wxString& name) = 0;
        virtual double GetNodeAnimationSampleInterval() = 0;
        virtual SampleType GetNodeAnimationSampleType() = 0;
        virtual wxString GetConfigurationTargetName() = 0;
        virtual double GetSkeletonAnimationSampleInterval() = 0;
        virtual SampleType GetSkeletonAnimationSampleType() = 0;
        virtual double GetVertexAnimationSampleInterval() = 0;
        virtual SampleType GetVertexAnimationSampleType() = 0;
        virtual wxString GetSubmeshNaming() = 0;
        virtual wxString GetSubmeshCustomName() = 0;
        virtual bool GetExportVertexColors() = 0;
        virtual wxString GetVertexColorFormat() = 0;
        virtual bool GetUseExplicitVertexColor() = 0;
        virtual bool GetExportTangents() = 0;
        virtual bool GetExportBinormals() = 0;
        virtual bool GetCheckMeshInstances() = 0;
        virtual bool GetAlwaysCreateNewSubmeshesWhenMerging() = 0;
        virtual bool GetAnimateBoundingVolumes() = 0;
        virtual bool GetExportMeshes() = 0;
        virtual int GetDisallowedMeshCount() = 0;
        virtual ObjectAccessor GetDisallowedMesh(int i) = 0;
        virtual bool GetCreateExtraMaterials() = 0;
        virtual bool GetIncludeParentMaterialName() = 0;
        virtual wxString GetNoMaterialName() = 0;
        virtual bool GetExportMaterials() = 0;
        virtual int GetAdditionalMaterialCount() = 0;
        virtual MaterialAccessor GetAdditionalMaterial(int i) = 0;
        virtual int GetDisallowedMaterialCount() = 0;
        virtual MaterialAccessor GetDisallowedMaterial(int i) = 0;
        virtual int GetRenameBitmapCount() = 0;
        virtual void GetRenameBitmap(int i, wxString& from, wxString& to) = 0;
        virtual int GetRenameBitmapExtensionCount() = 0;
        virtual void GetRenameBitmapExtension(int i, wxString& from, wxString& to) = 0;
        virtual bool GetExportEnvironment() = 0;
        virtual FinjinColor GetAmbientLightColor() = 0;
        virtual ValueSourceSync GetAmbientLightColorSync() = 0;
        virtual FinjinColor GetBackgroundColor() = 0;
        virtual ValueSourceSync GetBackgroundColorSync() = 0;
        virtual float GetEnvironmentNear() = 0;
        virtual float GetEnvironmentFar() = 0;
        virtual ObjectAccessor GetEnvironmentFarObject() = 0;
        virtual ValueSourceSync GetFogSync() = 0;
        virtual wxString GetFogMode() = 0;
        virtual float GetFogDensity() = 0;
        virtual float GetFogStart() = 0;
        virtual float GetFogEnd() = 0;
        virtual FinjinColor GetFogColor() = 0;
        virtual wxString GetShadowTechnique() = 0;
        virtual FinjinColor GetShadowColor() = 0;
        virtual float GetShadowFarDistance() = 0;

        virtual void SetSceneScaleUnit(wxString value) = 0;
        virtual void SetSceneScaleDivide(float value) = 0;
        virtual void SetExportHierarchy(bool value) = 0;
        virtual void SetIgnoreHiddenObjects(bool value) = 0;
        virtual void SetSceneManager(wxString value) = 0;
        virtual void SetExportFlags(bool value) = 0;
        virtual void ClearFlags() = 0;
        virtual void AddFlag(wxString name) = 0;
        virtual void SetNodeAnimationSampleInterval(double value) = 0;
        virtual void SetNodeAnimationSampleType(SampleType value) = 0;
        virtual void SetConfigurationTargetName(wxString value) = 0;
        virtual void SetSkeletonAnimationSampleInterval(double value) = 0;
        virtual void SetSkeletonAnimationSampleType(SampleType value) = 0;
        virtual void SetVertexAnimationSampleInterval(double value) = 0;
        virtual void SetVertexAnimationSampleType(SampleType value) = 0;
        virtual void SetSubmeshNaming(wxString value) = 0;
        virtual void SetSubmeshCustomName(wxString value) = 0;
        virtual void SetExportVertexColors(bool value) = 0;
        virtual void SetVertexColorFormat(wxString value) = 0;
        virtual void SetUseExplicitVertexColor(bool value) = 0;
        virtual void SetExportTangents(bool value) = 0;
        virtual void SetExportBinormals(bool value) = 0;
        virtual void SetCheckMeshInstances(bool value) = 0;
        virtual void SetAlwaysCreateNewSubmeshesWhenMerging(bool value) = 0;
        virtual void SetAnimateBoundingVolumes(bool value) = 0;
        virtual void SetExportMeshes(bool value) = 0;
        virtual void ClearDisallowedMeshes() = 0;
        virtual void AddDisallowedMesh(ObjectAccessor value) = 0;
        virtual void SetCreateExtraMaterials(bool value) = 0;
        virtual void SetIncludeParentMaterialName(bool value) = 0;
        virtual void SetNoMaterialName(wxString value) = 0;
        virtual void SetExportMaterials(bool value) = 0;
        virtual void ClearAdditionalMaterials() = 0;
        virtual void AddAdditionalMaterial(MaterialAccessor value) = 0;
        virtual void ClearDisallowedMaterials() = 0;
        virtual void AddDisallowedMaterial(MaterialAccessor value) = 0;
        virtual void ClearRenameBitmaps() = 0;
        virtual void AddRenameBitmap(wxString from, wxString to) = 0;
        virtual void ClearRenameBitmapExtensions() = 0;
        virtual void AddRenameBitmapExtension(wxString from, wxString to) = 0;
        virtual void SetExportEnvironment(bool value) = 0;
        virtual void SetAmbientLightColor(FinjinColor value) = 0;
        virtual void SetAmbientLightColorSync(ValueSourceSync value) = 0;
        virtual void SetBackgroundColor(FinjinColor value) = 0;
        virtual void SetBackgroundColorSync(ValueSourceSync value) = 0;
        virtual void SetEnvironmentNear(float value) = 0;
        virtual void SetEnvironmentFar(float value) = 0;
        virtual void SetEnvironmentFarObject(ObjectAccessor value) = 0;
        virtual void SetFogSync(ValueSourceSync value) = 0;
        virtual void SetFogMode(wxString value) = 0;
        virtual void SetFogDensity(float value) = 0;
        virtual void SetFogStart(float value) = 0;
        virtual void SetFogEnd(float value) = 0;
        virtual void SetFogColor(FinjinColor value) = 0;
        virtual void SetShadowTechnique(wxString value) = 0;
        virtual void SetShadowColor(FinjinColor value) = 0;
        virtual void SetShadowFarDistance(float value) = 0;

        virtual void TouchReferences() {}

        void GetSubmeshNamingValue(SubmeshNaming& naming);

        void GetFlags(std::vector<wxString>& names);

        wxString GetNodeAnimationSampleValueText();
        void SetNodeAnimationSampling(double sampling, SampleType sampleType, bool sampleTypeValid);

        wxString GetVertexAnimationSampleValueText();
        void SetVertexAnimationSampling(double sampling, SampleType sampleType, bool sampleTypeValid);

        wxString GetSkeletonAnimationSampleValueText();
        void SetSkeletonAnimationSampling(double sampling, SampleType sampleType, bool sampleTypeValid);

        bool CanExportMaterial(MaterialAccessor material);
        void RemoveDisallowedMaterials(MaterialAccessorSet& materials);
        float GetSceneScaleValue();
        FinjinColor GetAmbientLightColorValue();
        FinjinColor GetBackgroundColorValue();
        void SetAmbientLightColorValue(ValueSourceSync sync, FinjinColor color);
        void SetBackgroundColorValue(ValueSourceSync sync, FinjinColor color);
        float GetEnvironmentFarValue();

        FogMode GetFogModeValue();

        void GetFogValues(wxString& mode, FinjinColor& color, float& density, float& start, float& end, float range, TimeAccessor time = TimeAccessor(), bool* applicationHasLinearFog = 0);
        void SetFogValues(ValueSourceSync sync, wxString mode, FinjinColor color, float density, float start, float end, float range, TimeAccessor time = TimeAccessor());
    };

} }
