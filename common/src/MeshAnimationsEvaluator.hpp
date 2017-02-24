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


//Includes---------------------------------------------------------------------
#include "FinjinSceneSettingsAccessor.hpp"
#include "FinjinObjectSettingsAccessor.hpp"
#include "SceneExportSettings.hpp"
#include "ObjectExportSettings.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class GeometryState;
    class MeshAnimationTypeDetector;

    /** 
     * This class generates all the mesh animations for a GeometryState.
     */
    class MeshAnimationsEvaluator
    {
    public:
        MeshAnimationsEvaluator(FinjinSceneSettingsAccessor sceneSettings, const SceneExportSettings& _sceneExportSettings);

        /** 
         * Determines if any animations were detected. 
         * This should be called after either GetAnimations() or Evaluate().
         */
        bool HasAnimations() const;

        /**
         * Retrieves the mesh animations for the specifed object. 
         * It does not evaluate them, it just collects them in preparation for evaluation or for
         * a call to HasAnimations().
         * @param object [in] - The object whose mesh animations are retrieved.
         * @param objectSettings [in] - The object settings corresponding to GeometryState.
         * @param objectExportSettings [in] - The object export settings corresponding to GeometryState.
         */
        void GetAnimations
            (
            ObjectAccessor object,
            FinjinObjectSettingsAccessor objectSettings,
            const ObjectExportSettings& objectExportSettings
            );

        /**
         * Evaluates the mesh animations for the specified GeometryState.
         * Interanlly, GetAnimations() is called if necessary, so that needn't be called first.
         * @param geometryState [out] - The evaluated animations are placed into this object.
         * @param objectSettings [in] - The object settings corresponding to GeometryState.
         * @param objectExportSettings [in] - The object export settings corresponding to GeometryState.
         */
        void Evaluate
            (
            GeometryState* geometryState, 
            FinjinObjectSettingsAccessor objectSettings,
            const ObjectExportSettings& objectExportSettings
            );

    private:
        /** Gets the mesh animations for the specified mesh animation type. */
        void GetAnimations
            (
            FinjinObjectSettingsAccessor objectSettings,
            const ObjectExportSettings& objectExportSettings,
            MeshAnimationTypeDetector& meshAnimationType
            );

        /** Evaluates the mesh animations for the specified mesh animation type. */
        void EvaluateAnimations
            (
            GeometryState* geometryState, 
            FinjinObjectSettingsAccessor objectSettings,
            const ObjectExportSettings& objectExportSettings,
            MeshAnimationTypeDetector& meshAnimationType
            );

    private:
        FinjinSceneSettingsAccessor sceneSettings;
        const SceneExportSettings& sceneExportSettings;

    private:
        bool evaluatedAnimations;

    public:
        struct MeshObjectAnimationSettings
        {
            enum Target
            {
                OBJECT,
                SKELETON,
                MORPHER
            };
            
            MeshObjectAnimationSettings
                (
                FinjinMeshAnimationSettingsAccessor settings, 
                Target target, 
                bool morphWholeObject = false
                )
            {
                this->settings = settings;
                this->target = target;
                this->morphWholeObject = morphWholeObject;
            }

            FinjinMeshAnimationSettingsAccessor settings;
            Target target;

            /** Only used when target is OBJECT */
            bool morphWholeObject;
        };
        std::vector<MeshObjectAnimationSettings> meshObjectAnimations;

        std::vector<FinjinMeshAnimationSettingsAccessor> meshSkeletonAnimations;
        
        std::vector<FinjinMeshAnimationSettingsAccessor> meshMorpherAnimations;

        std::vector<WxTimeDuration> allSampleTimes;    
    };

} }
