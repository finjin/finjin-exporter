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
#include "ObjectSettingsDialog_MeshAnimationsPage_MixerControl-Max.hpp"
#include "PropertyValues.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
bool ObjectSettingsDialog_MeshAnimationsPage_MixerControl::Initialize(ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings)
{
    this->objectSettings = objectSettings;
    return this->bipedMixerAnimations.Initialize(object);
}

bool ObjectSettingsDialog_MeshAnimationsPage_MixerControl::SyncSettingsToMixer()
{
    //The animation names that have been handled. Used to prevent duplicate animation names
    std::set<wxString> animationNames;

    //Synchronize to the mixer animations
    for (size_t i = 0; i < this->bipedMixerAnimations.size(); i++)
    {
        auto& animation = this->bipedMixerAnimations[i];

        //Make sure the animation name hasn't already been encountered
        if (animationNames.find(animation.name) == animationNames.end())
        {
            //The animation name is now used
            animationNames.insert(animation.name);

            wxString trackName;
            if (this->bipedMixerAnimations.bipedAttachedWithPhysique)
                trackName = PropertyValues::MeshAnimationTrack::PHYSIQUE;
            else
                trackName = PropertyValues::MeshAnimationTrack::SKIN;

            //Synchronize
            int foundAt;
            auto existingAnimation = this->objectSettings.GetMeshAnimationByName(animation.name, &foundAt);
            if (foundAt >= 0)
            {
                //The animation already exists. Update it
                if (this->bipedMixerAnimations.skinningModifier != nullptr)
                {
                    existingAnimation.SetTrack(trackName);
                    existingAnimation.SetTimeInterval(animation.interval);
                }
            }
            else
            {
                //The animation doesn't exist. Add it

                //Set what little information is known
                if (this->bipedMixerAnimations.skinningModifier != nullptr)
                {
                    existingAnimation = this->objectSettings.AddNewMeshAnimation();
                    existingAnimation.SetTrack(trackName);
                    existingAnimation.SetTimeInterval(animation.interval);
                    existingAnimation.SetAnimationName(animation.name);
                }
            }
        }
    }

    return !animationNames.empty();
}
