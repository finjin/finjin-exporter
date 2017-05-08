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
#include "BaseDirectory.hpp"
#include "UserDataParameters.hpp"
#include "ExtendedValueParameters.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class FinjinObjectSettingsObject;

    /** The Finjin scene settings object. */
    class FinjinSceneSettingsObject : public Object
    {
    public:
        FinjinSceneSettingsObject();
        ~FinjinSceneSettingsObject();

        //From BaseObject
        CreateMouseCallBack* GetCreateMouseCallBack() override;

        //From Object
        ObjectState Eval(TimeValue time) override;
        void InitNodeName(MSTR& s) override;
        Interval ObjectValidity(TimeValue t) override        {return FOREVER;}
        BOOL IsRenderable() override                          {return FALSE;}

        Class_ID ClassID() override;
        SClass_ID SuperClassID() override;
        void GetClassName(MSTR& s) override;

        RefTargetHandle Clone(RemapDir& remap) override;
        RefResult NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate) override;

        int NumSubs() override;
        MSTR SubAnimName(int i) override;
        Animatable* SubAnim(int i) override;

        int NumRefs() override;
        RefTargetHandle GetReference(int i) override;
        void SetReference(int i, RefTargetHandle rtarg) override;

        int NumParamBlocks() override { return 1; }
        IParamBlock2* GetParamBlock(int i) override { return this->pblock; }
        IParamBlock2* GetParamBlockByID(BlockID id) override { return (this->pblock->ID() == id) ? this->pblock : nullptr; }

        void DeleteThis() override { delete this; }

        static Class_ID GetClassClassID();
        static ClassDesc* GetClassDesc();

        DECLARE_USER_DATA_OBJECT_METHODS

        DECLARE_EXTENDED_VALUE_OBJECT_METHODS

        wxString GetSceneManager();
        bool GetExportEnvironment();
        Color GetAmbientLightColor();
        ValueSourceSync GetAmbientLightColorSync();
        Color GetBackgroundColor();
        ValueSourceSync GetBackgroundColorSync();
        float GetEnvironmentNear();
        float GetEnvironmentFar();
        INode* GetEnvironmentFarObject();
        ValueSourceSync GetFogSync();
        wxString GetFogMode();
        float GetFogDensity();
        float GetFogStart();
        float GetFogEnd();
        Color GetFogColor();
        wxString GetShadowTechnique();
        Color GetShadowColor();
        float GetShadowFarDistance();
        wxString GetSceneScaleUnit();
        float GetSceneScaleDivide();
        bool GetExportHierarchy();
        bool GetIgnoreHiddenObjects();
        double GetNodeAnimationSampleInterval();
        SampleType GetNodeAnimationSampleType();
        wxString GetConfigurationTargetName();
        double GetSkeletonAnimationSampleInterval();
        SampleType GetSkeletonAnimationSampleType();
        double GetVertexAnimationSampleInterval();
        SampleType GetVertexAnimationSampleType();
        bool GetExportMeshes();
        int GetDisallowedMeshCount();
        INode* GetDisallowedMesh(int index);
        void ClearDisallowedMeshes();
        bool GetExportVertexColors();
        wxString GetSubmeshNaming();
        wxString GetSubmeshCustomName();
        bool GetConvertBitmapsToTextures();
        bool GetExportTangents();
        bool GetExportBinormals();
        bool GetAnimateBoundingVolumes();
        bool GetMakeBoundingBoxesCubes();
        bool GetCheckMeshInstances();
        bool GetAlwaysCreateNewSubmeshesWhenMerging();
        wxString GetVertexColorFormat();
        bool GetUseExplicitVertexColor();
        bool GetExportMaterials();
        int GetAdditionalMaterialCount();
        Mtl* GetAdditionalMaterial(int i);
        void ClearAdditionalMaterials();
        int GetDisallowedMaterialCount();
        Mtl* GetDisallowedMaterial(int index);
        void ClearDisallowedMaterials();
        bool GetCreateExtraMaterials();
        bool GetIncludeParentMaterialName();
        wxString GetNoMaterialName();
        wxString GetRenderSystem();
        int GetRenameBitmapCount();
        void GetRenameBitmap(int index, wxString& from, wxString& to);
        void ClearRenameBitmaps();
        int GetRenameBitmapExtensionCount();
        void GetRenameBitmapExtension(int index, wxString& from, wxString& to);
        void ClearRenameBitmapExtensions();

        bool GetExportFlags();
        int GetFlagCount();
        void GetFlag(int index, wxString& name);
        wxString GetQueryFlagName(int index);
        void GetFlagNames(std::set<wxString>& names);
        void ClearFlags();

        void SetSceneManager(wxString value);
        void SetExportEnvironment(bool value);
        void SetAmbientLightColor(Color value);
        void SetAmbientLightColorSync(ValueSourceSync value);
        void SetBackgroundColor(Color value);
        void SetBackgroundColorSync(ValueSourceSync value);
        void SetEnvironmentNear(float value);
        void SetEnvironmentFar(float value);
        void SetEnvironmentFarObject(INode* value);
        void SetFogSync(ValueSourceSync value);
        void SetFogMode(wxString value);
        void SetFogDensity(float value);
        void SetFogStart(float value);
        void SetFogEnd(float value);
        void SetFogColor(Color value);
        void SetShadowTechnique(wxString value);
        void SetShadowColor(Color value);
        void SetShadowFarDistance(float value);
        void SetSceneScaleUnit(wxString value);
        void SetSceneScaleDivide(float value);
        void SetExportHierarchy(bool value);
        void SetIgnoreHiddenObjects(bool value);
        void SetNodeAnimationSampleInterval(double value);
        void SetNodeAnimationSampleType(SampleType value);
        void SetConfigurationTargetName(wxString value);
        void SetSkeletonAnimationSampleInterval(double value);
        void SetSkeletonAnimationSampleType(SampleType value);
        void SetVertexAnimationSampleInterval(double value);
        void SetVertexAnimationSampleType(SampleType value);
        void SetExportMeshes(bool value);
        void AddDisallowedMesh(INode* value);
        void SetExportVertexColors(bool value);
        void SetSubmeshNaming(wxString value);
        void SetSubmeshCustomName(wxString value);
        void SetConvertBitmapsToTextures(bool value);
        void SetExportTangents(bool value);
        void SetExportBinormals(bool value);
        void SetAnimateBoundingVolumes(bool value);
        void SetMakeBoundingBoxesCubes(bool value);
        void SetCheckMeshInstances(bool value);
        void SetAlwaysCreateNewSubmeshesWhenMerging(bool value);
        void SetVertexColorFormat(wxString value);
        void SetUseExplicitVertexColor(bool value);
        void SetExportMaterials(bool value);
        void AddAdditionalMaterial(Mtl* value);
        void AddDisallowedMaterial(Mtl* value);
        void SetCreateExtraMaterials(bool value);
        void SetIncludeParentMaterialName(bool value);
        void SetNoMaterialName(wxString value);
        void SetRenderSystem(wxString value);
        void AddRenameBitmap(wxString from, wxString to);
        void AddRenameBitmapExtension(wxString from, wxString to);

        void SetExportFlags(bool value);
        void AddFlag(wxString name);

        static FinjinSceneSettingsObject* GetSceneSettingsByIndex(int index, INode** settingsNode = nullptr);
        static FinjinSceneSettingsObject* GetSceneSettings(bool createIfNecessary = true, bool* newInstance = nullptr);

        void MergeObjectSettings(FinjinSceneSettingsObject* settings);
        void CleanObjectSettings();
        FinjinObjectSettingsObject* GetObjectSettings(INode* node, bool createIfNecessary = true, bool* newInstance = nullptr);
        void RemoveObjectSettings(INode* node);

    public:
        enum
        {
            PARAM_BLOCK_REF,
            NUM_REFS
        };

        /** Parameter block parameters */
        enum
        {
            USER_DATA_PARAM_BLOCK_IDS,

            PB_NODES,
            PB_OBJECTS,

            PB_SCENE_MANAGER,
            PB_ENVIRONMENT_NEAR,
            PB_ENVIRONMENT_FAR,
            PB_ENVIRONMENT_FAR_OBJECT,
            PB_FOG_MODE,
            PB_FOG_DENSITY,

            PB_SHADOW_TECHNIQUE,
            PB_SHADOW_COLOR,
            PB_SHADOW_FAR_DISTANCE,

            PB_SCENE_SCALE_DIVIDE,

            PB_IGNORE_HIDDEN_LAYERS,
            PB_IGNORE_HIDDEN_OBJECTS,

            PB_EXPORT_MESHES,
            PB_DISALLOWED_MESHES,
            PB_EXPORT_VERTEX_COLORS,
            PB_EXPORT_TANGENTS,
            PB_EXPORT_BINORMALS,
            PB_VERTEX_COLOR_FORMAT,

            PB_EXPORT_MATERIALS,
            PB_DISALLOWED_MATERIALS,
            PB_NO_MATERIAL_NAME,

            PB_RENDER_SYSTEM,

            PB_RENAME_BITMAPS_FROM,
            PB_RENAME_BITMAPS_TO,

            PB_RENAME_BITMAP_EXTENSIONS_FROM,
            PB_RENAME_BITMAP_EXTENSIONS_TO,

            PB_BEFORE_EXPORT_COMMANDS,
            PB_AFTER_EXPORT_COMMANDS,

            PB_SCENE_SCALE_UNIT,

            PB_EXPORT_FLAGS,
            PB_FLAG_NAMES,

            PB_INCLUDE_PARENT_MATERIAL_NAME,
            PB_USE_EXPLICIT_VERTEX_COLOR,

            PB_EXPORT_ENVIRONMENT,
            PB_AMBIENT_LIGHT_COLOR,
            PB_AMBIENT_LIGHT_COLOR_SYNC,
            PB_BACKGROUND_COLOR,
            PB_BACKGROUND_COLOR_SYNC,
            PB_FOG_SYNC,
            PB_FOG_START,
            PB_FOG_END,
            PB_FOG_COLOR,
            PB_OPTIMIZE_SKELETON_ANIMATIONS,

            EXTENDED_VALUE_PARAM_BLOCK_IDS,

            PB_CHECK_MESH_INSTANCES,
            PB_ADDITIONAL_MATERIALS,

            PB_SUBMESH_NAMING,
            PB_SUBMESH_CUSTOM_NAME,
            PB_ALWAYS_CREATE_NEW_SUBMESHES_WHEN_MERGING,

            PB_CONVERT_BITMAPS_TO_TEXTURES,

            PB_ANIMATE_BOUNDING_VOLUMES,
            PB_MESH_TEXT_PATH,

            PB_CREATE_EXTRA_MATERIALS,

            PB_EXPORT_HIERARCHY,
            PB_NODE_ANIMATION_SAMPLE_INTERVAL,
            PB_NODE_ANIMATION_SAMPLE_TYPE,
            PB_SKELETON_ANIMATION_SAMPLE_INTERVAL,
            PB_SKELETON_ANIMATION_SAMPLE_TYPE,
            PB_VERTEX_ANIMATION_SAMPLE_INTERVAL,
            PB_VERTEX_ANIMATION_SAMPLE_TYPE,
            PB_CONFIGURATION_TARGET_NAME
        };

        /** Reference 0 */
        IParamBlock2* pblock;

        IObjParam* ip;
    };

} }
