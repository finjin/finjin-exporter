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
#include "UserDataAttributes.hpp"
#include "ExtendedValueAttributes.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** The scene settings object. */
    class FinjinSceneSettingsNode : public MPxNode
    {
    public:
        FinjinSceneSettingsNode();
        ~FinjinSceneSettingsNode();

        MStatus compute(const MPlug& plug, MDataBlock& block) {return MStatus::kSuccess;}

        static void* Creator();
        static MStatus Initialize();

        static MObject GetSceneSettings(bool createIfNecessary = true, bool* newInstance = nullptr);

        static MObject GetSceneSettingsByIndex(int index);

        static bool IsSceneSettingsNode(MObject& obj);

    private:
        static MStatus AddAttributes();

    public:
        static const MString TYPE_NAME;
        static const MTypeId TYPE_ID;

        struct Attributes : public UserDataAttributes, public ExtendedValueAttributes
        {
            MObject sceneManager;
            MObject exportEnvironment;
            MObject ambientLightColor;
            MObject ambientLightColorSync;
            MObject backgroundColor;
            MObject backgroundColorSync;
            MObject environmentColor;
            MObject environmentNear;
            MObject environmentFar;
            MObject environmentFarObject;
            MObject fogSync;
            MObject fogMode;
            MObject fogDensity;
            MObject fogStart;
            MObject fogEnd;
            MObject fogColor;

            MObject shadowTechnique;
            MObject shadowColor;
            MObject shadowFarDistance;

            MObject sceneScaleUnit;
            MObject sceneScaleDivide;

            MObject exportHierarchy;
            MObject ignoreHiddenObjects;

            MObject nodeAnimationSampleInterval;
            MObject nodeAnimationSampleType;
            MObject configurationTargetName;
            MObject skeletonAnimationSampleInterval;
            MObject skeletonAnimationSampleType;
            MObject vertexAnimationSampleInterval;
            MObject vertexAnimationSampleType;

            MObject exportMeshes;
            MObject disallowedMeshes;
            MObject submeshNaming;
            MObject submeshCustomName;
            MObject exportVertexColors;
            MObject convertBitmapsToTextures;
            MObject exportTangents;
            MObject exportBinormals;
            MObject animateBoundingVolumes;
            MObject checkMeshInstances;
            MObject alwaysCreateNewSubmeshesWhenMerging;
            MObject vertexColorFormat;
            MObject useExplicitVertexColor;

            MObject exportMaterials;
            MObject additionalMaterials;
            MObject disallowedMaterials;
            MObject createExtraMaterials;
            MObject includeParentMaterialName;
            MObject noMaterialName;

            MObject renameBitmapsFrom;
            MObject renameBitmapsTo;

            MObject renameBitmapExtensionsFrom;
            MObject renameBitmapExtensionsTo;

            MObject exportFlags;
            MObject flagNames;

            MObject resourcesChanged;

            MObject objectSettings;
        };
        static Attributes attributes;
        static std::list<MObject> attributesList;
        static std::list<MObject> userDataAttributesList;
        static std::list<MObject> extendedValueAttributesList;
    };

} }
