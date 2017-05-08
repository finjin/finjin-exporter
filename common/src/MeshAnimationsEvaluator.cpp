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
#include "MeshAnimationsEvaluator.hpp"
#include "MeshAnimationTypeDetector.hpp"
#include "MeshAnimationTracksDetector.hpp"
#include "GeometryState.hpp"
#include "AnimationExportSettings.hpp"
#include "AnimationUtilities.hpp"
#include "MathUtilities.hpp"
#include "ExporterUtilities.hpp"
#include "FinjinGlobalSettings.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
MeshAnimationsEvaluator::MeshAnimationsEvaluator
    (
    FinjinSceneSettingsAccessor sceneSettings,
    const SceneExportSettings& _sceneExportSettings
    ) : sceneExportSettings(_sceneExportSettings)
{
    this->sceneSettings = sceneSettings;
    this->evaluatedAnimations = false;
}

bool MeshAnimationsEvaluator::HasAnimations() const
{
    return
        !this->meshObjectAnimations.empty() ||
        !this->meshSkeletonAnimations.empty() ||
        !this->meshMorpherAnimations.empty();
}

void MeshAnimationsEvaluator::GetAnimations
    (
    ObjectAccessor object,
    FinjinObjectSettingsAccessor objectSettings,
    const ObjectExportSettings& objectExportSettings
    )
{
    MeshAnimationTypeDetector meshAnimationType;
    meshAnimationType.Detect(object, objectSettings);

    GetAnimations(objectSettings, objectExportSettings, meshAnimationType);
}

void MeshAnimationsEvaluator::GetAnimations
    (
    FinjinObjectSettingsAccessor objectSettings,
    const ObjectExportSettings& objectExportSettings,
    MeshAnimationTypeDetector& meshAnimationType
    )
{
    this->evaluatedAnimations = true;

    int meshAnimationCount = objectSettings.GetMeshAnimationCount();

    this->meshObjectAnimations.clear();
    this->meshObjectAnimations.reserve(meshAnimationCount);

    this->meshSkeletonAnimations.clear();
    this->meshSkeletonAnimations.reserve(meshAnimationCount);

    this->meshMorpherAnimations.clear();
    this->meshMorpherAnimations.reserve(meshAnimationCount);

    for (MeshAnimationTracksDetector::TrackType trackType = MeshAnimationTracksDetector::OBJECT;
        trackType < MeshAnimationTracksDetector::TRACK_TYPE_COUNT;
        trackType++)
    {
        //Get the animations for the track
        std::vector<FinjinMeshAnimationSettingsAccessor> meshAnimations;
        wxString trackName = MeshAnimationTracksDetector::GetTrackName(trackType);
        objectSettings.GetMeshAnimationsByTrack(meshAnimations, trackName);

        //Add the animations to the appropriate collection
        if (trackType == MeshAnimationTracksDetector::OBJECT)
        {
            if (meshAnimationType.selectedType->UsesObjectTrack())
            {
                for (size_t i = 0; i < meshAnimations.size(); i++)
                {
                    MeshObjectAnimationSettings settings
                        (
                        meshAnimations[i],
                        MeshObjectAnimationSettings::OBJECT,
                        meshAnimations[i].GetGenerateCombinedMorphsValue(meshAnimationType.selectedType->GeneratesCombinedMorphs())
                        );
                    this->meshObjectAnimations.push_back(settings);
                }
            }
        }
        else if (MeshAnimationTracksDetector::IsSkeletonBased(trackType))
        {
            if (meshAnimationType.selectedType->UsesSkeletonTrack() || meshAnimationType.selectedType->GeneratesMorphsExclusively())
            {
                for (size_t i = 0; i < meshAnimations.size(); i++)
                {
                    if (meshAnimations[i].GetExportAsMorph() || meshAnimationType.selectedType->GeneratesMorphsExclusively())
                        this->meshObjectAnimations.push_back(MeshObjectAnimationSettings(meshAnimations[i], MeshObjectAnimationSettings::SKELETON));
                    else
                        this->meshSkeletonAnimations.push_back(meshAnimations[i]);
                }
            }
        }
        else if (MeshAnimationTracksDetector::IsMorpherBased(trackType))
        {
            if (meshAnimationType.selectedType->UsesMorpherTrack() || meshAnimationType.selectedType->GeneratesMorphsExclusively())
            {
                for (size_t i = 0; i < meshAnimations.size(); i++)
                {
                    if (meshAnimations[i].GetExportAsMorph() || meshAnimationType.selectedType->GeneratesMorphsExclusively())
                        this->meshObjectAnimations.push_back(MeshObjectAnimationSettings(meshAnimations[i], MeshObjectAnimationSettings::MORPHER));
                    else
                        this->meshMorpherAnimations.push_back(meshAnimations[i]);
                }
            }
        }
    }
}

void MeshAnimationsEvaluator::Evaluate
    (
    GeometryState* geometryState,
    FinjinObjectSettingsAccessor objectSettings,
    const ObjectExportSettings& objectExportSettings
    )
{
    MeshAnimationTypeDetector meshAnimationType;
    meshAnimationType.Detect(geometryState->createObject, objectSettings);

    if (!this->evaluatedAnimations)
        GetAnimations(objectSettings, objectExportSettings, meshAnimationType);

    EvaluateAnimations(geometryState, objectSettings, objectExportSettings, meshAnimationType);
}

void MeshAnimationsEvaluator::EvaluateAnimations
    (
    GeometryState* geometryState,
    FinjinObjectSettingsAccessor objectSettings,
    const ObjectExportSettings& objectExportSettings,
    MeshAnimationTypeDetector& meshAnimationType
    )
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("MeshAnimationsEvaluator::EvaluateAnimations(%s)"), geometryState->createObject.GetLocalName().wx_str());

    //Get morph animations
    std::list<WxTimeDuration> morphSampleTimes;
    geometryState->meshMorphAnimations.clear();
    geometryState->meshMorphAnimations.resize(this->meshObjectAnimations.size());
    for (size_t animationIndex = 0; animationIndex < this->meshObjectAnimations.size(); animationIndex++)
    {
        auto& meshObjectAnimation = this->meshObjectAnimations[animationIndex];
        AnimationExportSettings animationSettings
            (
            meshObjectAnimation.settings,
            objectSettings.GetVertexAnimationSampleInterval(),
            sceneSettings.GetVertexAnimationSampleInterval(),
            FinjinGlobalSettings::GetInstance().vertexAnimationSampleInterval
            );

        //Generate sample times
        std::vector<WxTimeDuration> sampleTimes;
        AnimationUtilities::GetSampledKeyTimes(sampleTimes, animationSettings.timeInterval, animationSettings.sampleInterval);
        PushAll(morphSampleTimes, sampleTimes);

        //Create the animation
        auto& createdAnimation = geometryState->meshMorphAnimations[animationIndex];
        createdAnimation.reset(new MeshMorphAnimation);
        createdAnimation->name = animationSettings.name;
        ExporterUtilities::FixObjectName(createdAnimation->name);
        createdAnimation->embedAnimation = animationSettings.embedAnimation;
        createdAnimation->linkToMainObject = animationSettings.linkToMainObject;

        //Set up subanimations
        createdAnimation->subanimations.resize(geometryState->submeshes.size());
        for (auto submesh : geometryState->submeshes)
        {
            auto& createdSubanimation = createdAnimation->subanimations[submesh->index];
            createdSubanimation.reset(new MeshMorphAnimation::Subanimation);

            createdSubanimation->submesh = submesh.get();
            createdSubanimation->keys.resize(sampleTimes.size());
        }

        //Create the subanimation keys
        for (size_t sampleIndex = 0; sampleIndex < sampleTimes.size(); sampleIndex++)
        {
            auto geometryFlags = GeometryStateFlags::NONE;
            switch (meshObjectAnimation.target)
            {
                case MeshObjectAnimationSettings::OBJECT:
                {
                    if (!meshObjectAnimation.morphWholeObject)
                        geometryFlags |= GeometryStateFlags::NO_MORPHER_EFFECT | GeometryStateFlags::NO_SKELETON_EFFECT;
                    break;
                }
                case MeshObjectAnimationSettings::SKELETON:
                {
                    geometryFlags |= GeometryStateFlags::NO_MORPHER_EFFECT;
                    break;
                }
                case MeshObjectAnimationSettings::MORPHER:
                {
                    geometryFlags |= GeometryStateFlags::NO_SKELETON_EFFECT;
                    break;
                }
            }

            //Put the samples into a each subanimation
            GeometryState::SubmeshesPoints submeshesPoints;
            auto sampleTime = animationSettings.GetSampleTime(sampleTimes, sampleIndex);
            geometryState->SampleSubmeshPoints(submeshesPoints, this->sceneExportSettings.conversionManager, this->sceneExportSettings.scale, sampleTime, geometryFlags);
            for (size_t submeshIndex = 0; submeshIndex < submeshesPoints.size(); submeshIndex++)
            {
                auto& createdSubanimation = createdAnimation->subanimations[submeshIndex];

                auto& key = createdSubanimation->keys[sampleIndex];
                key.time = animationSettings.GetOutputKeyTime(sampleTimes, sampleIndex);
                key.points.swap(submeshesPoints[submeshIndex].points);
            }
        }
    }

    //Get pose animations
    FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Evaluating %d pose animations"), (int)this->meshMorpherAnimations.size());
    std::list<WxTimeDuration> morpherSampleTimes;
    geometryState->meshMorpherAnimations.clear();
    auto exportingMorpherAnimations = objectSettings.GetExportPoses();
    if (exportingMorpherAnimations)
        geometryState->meshMorpherAnimations.resize(this->meshMorpherAnimations.size());

    auto morphTargetCount = geometryState->meshMorpher.morphTargets.size();
    for (size_t animationIndex = 0; animationIndex < this->meshMorpherAnimations.size(); animationIndex++)
    {
        auto& meshMorpherAnimation = this->meshMorpherAnimations[animationIndex];
        AnimationExportSettings animationSettings
            (
            meshMorpherAnimation,
            objectSettings.GetVertexAnimationSampleInterval(),
            sceneSettings.GetVertexAnimationSampleInterval(),
            FinjinGlobalSettings::GetInstance().vertexAnimationSampleInterval
            );

        //Generate sample times
        std::vector<WxTimeDuration> sampleTimes;
        AnimationUtilities::GetSampledKeyTimes(sampleTimes, animationSettings.timeInterval, animationSettings.sampleInterval);
        PushAll(morpherSampleTimes, sampleTimes);

        //Exit early if not exporting animations
        if (!exportingMorpherAnimations)
            continue;

        //Create the animation
        auto& createdAnimation = geometryState->meshMorpherAnimations[animationIndex];
        createdAnimation.reset(new MeshMorpher::Animation);
        createdAnimation->name = animationSettings.name;
        ExporterUtilities::FixObjectName(createdAnimation->name);
        createdAnimation->embedAnimation = animationSettings.embedAnimation;
        createdAnimation->linkToMainObject = animationSettings.linkToMainObject;

        //Set up subanimations
        createdAnimation->subanimations.resize(geometryState->submeshes.size());
        for (auto submesh : geometryState->submeshes)
        {
            auto& createdSubanimation = createdAnimation->subanimations[submesh->index];
            createdSubanimation.reset(new MeshMorpher::Subanimation);

            createdSubanimation->submesh = submesh.get();
            createdSubanimation->keys.resize(sampleTimes.size());
            for (size_t sampleIndex = 0; sampleIndex < sampleTimes.size(); sampleIndex++)
            {
                auto& key = createdSubanimation->keys[sampleIndex];
                key.time = animationSettings.GetOutputKeyTime(sampleTimes, sampleIndex);
                key.influences.reserve(morphTargetCount);
            }
        }

        //Create the subanimation keys
        for (size_t sampleIndex = 0; sampleIndex < sampleTimes.size(); sampleIndex++)
        {
            auto sampleTime = animationSettings.GetSampleTime(sampleTimes, sampleIndex);
            MeshMorpher::MorphTargetInfluences targetInfluences;
            geometryState->meshMorpher.SampleMorphTargetInfluences(targetInfluences, sampleTime);

            for (auto submesh : geometryState->submeshes)
            {
                auto& createdSubanimation = createdAnimation->subanimations[submesh->index];
                auto& key = createdSubanimation->keys[sampleIndex];

                for (size_t targetIndex = 0; targetIndex < targetInfluences.size(); targetIndex++)
                {
                    auto& targetInfluence = targetInfluences[targetIndex];
                    MeshMorpher::MorphSubtargetInfluence subtargetInfluence(targetInfluence.target->subtargets[submesh->index].get(), targetInfluence.influence);
                    key.influences.push_back(subtargetInfluence);
                }
            }
        }
    }

    //Get skeleton animations
    FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Evaluating %d skeleton animations"), (int)this->meshSkeletonAnimations.size());
    std::list<WxTimeDuration> skeletonSampleTimes;
    geometryState->meshSkeletonAnimations.clear();
    auto exportingSkeletonAnimations = objectSettings.GetExportSkeleton();
    if (exportingSkeletonAnimations)
        geometryState->meshSkeletonAnimations.resize(this->meshSkeletonAnimations.size());

    auto animatedRoot = objectSettings.GetAnimatedRoot();
    for (size_t animationIndex = 0; animationIndex < this->meshSkeletonAnimations.size(); animationIndex++)
    {
        auto& meshSkeletonAnimation = this->meshSkeletonAnimations[animationIndex];
        AnimationExportSettings animationSettings
            (
            meshSkeletonAnimation,
            objectSettings.GetSkeletonAnimationSampleInterval(),
            sceneSettings.GetSkeletonAnimationSampleInterval(),
            FinjinGlobalSettings::GetInstance().skeletonAnimationSampleInterval
            );

        //Generate sample times
        std::vector<WxTimeDuration> sampleTimes;
        AnimationUtilities::GetSampledKeyTimes(sampleTimes, animationSettings.timeInterval, animationSettings.sampleInterval);
        PushAll(skeletonSampleTimes, sampleTimes);

        //Exit early if not exporting animations
        if (!exportingSkeletonAnimations)
            continue;

        //Create the animation
        auto& createdAnimation = geometryState->meshSkeletonAnimations[animationIndex];
        createdAnimation.reset(new MeshSkeleton::Animation);
        createdAnimation->name = animationSettings.name;
        ExporterUtilities::FixObjectName(createdAnimation->name);
        createdAnimation->embedAnimation = animationSettings.embedAnimation;
        createdAnimation->linkToMainObject = animationSettings.linkToMainObject;

        //Get root origin
        FinjinVector3 animatedRootOrigin;
        if (animatedRoot.IsValid())
        {
            auto animatedRootStartTime = animationSettings.GetSampleTime(sampleTimes, 0);
            if (meshSkeletonAnimation.GetOverrideAnimatedRootStartTime())
                animatedRootStartTime = meshSkeletonAnimation.GetAnimatedRootStartTime();

            TransformAccessor rootTransform
                (
                animatedRoot.GetNodeTransformation(animatedRootStartTime),
                this->sceneExportSettings.conversionManager,
                this->sceneExportSettings.scale
                );
            animatedRootOrigin = rootTransform.GetTranslation();
        }

        //Set bone animations and create animation keys
        createdAnimation->boneAnimations.reserve(geometryState->meshSkeleton.bones.size());
        for (auto meshBone : geometryState->meshSkeleton.bones)
        {
            if (meshSkeletonAnimation.IsBoneAllowed(meshBone->object))
            {
                MeshSkeleton::BoneAnimationPtr createdBoneAnimation(new MeshSkeleton::BoneAnimation);
                createdAnimation->boneAnimations.push_back(createdBoneAnimation);

                createdBoneAnimation->bone = meshBone.get();
                createdBoneAnimation->keys.resize(sampleTimes.size());

                auto boneTranslationMask = meshSkeletonAnimation.GetBoneTranslationMask(meshBone->object);
                this->sceneExportSettings.conversionManager.ConvertAbsoluteComponents(boneTranslationMask);

                for (size_t sampleIndex = 0; sampleIndex < sampleTimes.size(); sampleIndex++)
                {
                    auto& key = createdBoneAnimation->keys[sampleIndex];

                    auto sampleTime = animationSettings.GetSampleTime(sampleTimes, sampleIndex);

                    //Sample the bone's transform
                    TransformAccessor boneWorldTransform
                        (
                        meshBone->GetNodeTransformation(sampleTime),
                        this->sceneExportSettings.conversionManager,
                        this->sceneExportSettings.scale
                        );

                    //Offset it by the animated root offset, if necessary
                    if (meshBone->IsRoot() && animatedRoot.IsValid())
                    {
                        TransformAccessor rootTransform
                            (
                            animatedRoot.GetNodeTransformation(animationSettings.GetSampleTime(sampleTimes, sampleIndex)),
                            this->sceneExportSettings.conversionManager,
                            this->sceneExportSettings.scale
                            );
                        auto animatedRootOffset = rootTransform.GetTranslation() - animatedRootOrigin;

                        boneWorldTransform.SetTranslation(boneWorldTransform.GetTranslation() - animatedRootOffset);
                    }

                    //Sample the bone's parent transform
                    TransformAccessor parentWorldTransform;
                    if (meshBone->IsRoot())
                    {
                        parentWorldTransform.Set
                            (
                            geometryState->createObject.GetNodeTransformation(sampleTime),
                            this->sceneExportSettings.conversionManager,
                            this->sceneExportSettings.scale
                            );
                    }
                    else
                    {
                        parentWorldTransform.Set
                            (
                            meshBone->parent->GetNodeTransformation(sampleTime),
                            this->sceneExportSettings.conversionManager,
                            this->sceneExportSettings.scale
                            );
                    }

                    //Get the bone transform relative to parent
                    key.time = animationSettings.GetOutputKeyTime(sampleTimes, sampleIndex);

                    auto relativeTransform = boneWorldTransform.GetRelativeTo(parentWorldTransform);
                    auto relativeTranslation = relativeTransform.GetTranslation() * boneTranslationMask;
                    relativeTransform.SetTranslation(relativeTranslation);
                    key.SetTransform(relativeTransform);
                }
            }
        }
    }

    //Combine all the sample times
    this->allSampleTimes.reserve(morphSampleTimes.size() + morpherSampleTimes.size() + skeletonSampleTimes.size());
    PushAll(this->allSampleTimes, morphSampleTimes);
    PushAll(this->allSampleTimes, morpherSampleTimes);
    PushAll(this->allSampleTimes, skeletonSampleTimes);
    SortAndRemoveDuplicates(this->allSampleTimes);
}
