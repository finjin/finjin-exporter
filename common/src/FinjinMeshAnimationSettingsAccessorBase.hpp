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
#include "FinjinVector.hpp"
#include "ObjectAccessor.hpp"
#include "FinjinTimeInterval.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class FinjinObjectSettingsAccessorBase;
    class FinjinSceneSettingsAccessorBase;
    struct MeshAnimationType;
    class MeshAnimationTracksDetectorBase;

    /**
     * FinjinMeshAnimationSettingsAccessorBase and the application-specific FinjinMeshAnimationSettingsAccessor
     * classes provide generic access to an Finjin mesh animation settings object.
     */
    class FinjinMeshAnimationSettingsAccessorBase
    {
    public:
        virtual ~FinjinMeshAnimationSettingsAccessorBase() {}

        virtual wxString GetTrack() = 0;
        virtual wxString GetAnimationName() = 0;
        virtual wxString GetTimeScaleType() = 0;
        virtual double GetTimeScale() = 0;
        virtual bool GetCopyFirstKeyToLast() = 0;
        virtual bool GetExportAsMorph() = 0;
        virtual InheritedBool GetMorphWholeObject() = 0;
        virtual bool GetOverrideAnimatedRootStartTime() = 0;
        virtual TimeAccessor GetAnimatedRootStartTime() = 0;
        virtual bool GetUseAnimationStartTime() = 0;
        virtual bool GetLooped() = 0;
        virtual double GetSampleInterval() = 0;
        virtual SampleType GetSampleType() = 0;
        virtual FinjinTimeInterval GetTimeInterval() = 0;
        virtual bool GetEmbedAnimation() = 0;
        virtual bool GetLinkToMainObject() = 0;
        virtual int GetBoneTranslationMaskCount() = 0;
        virtual void GetBoneTranslationMask(int index, ObjectAccessor& object, FinjinVector3& mask) = 0;
        virtual FinjinVector3 GetBoneTranslationMask(ObjectAccessor object) = 0;
        virtual int GetAllowExportBoneCount() = 0;
        virtual void GetAllowExportBone(int index, ObjectAccessor& object, bool& allow, bool& recursive) = 0;

        virtual void SetTrack(wxString value) = 0;
        virtual void SetAnimationName(wxString value) = 0;
        virtual void SetTimeScaleType(wxString value) = 0;
        virtual void SetTimeScale(double value) = 0;
        virtual void SetCopyFirstKeyToLast(bool value) = 0;
        virtual void SetExportAsMorph(bool value) = 0;
        virtual void SetMorphWholeObject(InheritedBool value) = 0;
        virtual void SetOverrideAnimatedRootStartTime(bool value) = 0;
        virtual void SetAnimatedRootStartTime(TimeAccessor value) = 0;
        virtual void SetUseAnimationStartTime(bool value) = 0;
        virtual void SetLooped(bool value) = 0;
        virtual void SetSampleInterval(double value) = 0;
        virtual void SetSampleType(SampleType value) = 0;
        virtual void SetTimeInterval(FinjinTimeInterval interval) = 0;
        virtual void SetEmbedAnimation(bool value) = 0;
        virtual void SetLinkToMainObject(bool value) = 0;
        virtual void ClearBoneTranslationMasks() = 0;
        virtual void AddBoneTranslationMask(ObjectAccessor object, FinjinVector3 mask) = 0;
        virtual void ClearAllowExportBones() = 0;
        virtual void AddAllowExportBone(ObjectAccessor object, bool allow, bool recursive) = 0;

        /** Gets the time scale factor. */
        double GetRealTimeScale();

        /**
         * Determines whether this animation should be generated as a combined morph.
         * @param objectValue [in] - The corresponding object-level (parent) setting. This is used if
         * the mesh animation setting is 'use parent'.
         */
        bool GetGenerateCombinedMorphsValue(bool objectValue);

        /**
         * Determines whether the bone is allowed in this animation, taking into account the
         * hierarchical bone inclusions/exclusions.
         * @param bone [in] - The bone to check.
         * @return If the bone is allowed in the animation, true is returned. Otherwise, false is returned.
         */
        bool IsBoneAllowed(ObjectAccessor bone);

        /**
         * Gets the animation's sample type and sample interval, taking into account the object settings
         * and global settings if necessary.
         * @param sampleType [out] - The retrieved sample type.
         * @param sampleInterval [out] - The retrieved sample interval.
         * @param objectSettings [in] - The object settings.
         * @param sceneSettings [in] - The scene settings.
         * @param meshAnimationType [in] - The mesh animation type.
         * @param tracksDetector [in] - The available mesh animation tracks for the object.
         */
        void GetSampleTypeAndIntervalValues
            (
            SampleType& sampleType,
            double& sampleInterval,
            FinjinObjectSettingsAccessorBase& objectSettings,
            FinjinSceneSettingsAccessorBase& sceneSettings,
            MeshAnimationType& meshAnimationType,
            MeshAnimationTracksDetectorBase& tracksDetector
            );

        /**
         * Copies all the settings from the specified source to this animation.
         */
        virtual void CopyFrom(FinjinMeshAnimationSettingsAccessorBase& from);
    };

} }
