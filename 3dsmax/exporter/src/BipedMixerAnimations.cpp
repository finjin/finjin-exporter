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
#include "BipedMixerAnimations.hpp"
#include "FileUtilities.hpp"
#include "MaxUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
bool BipedMixerAnimations::Initialize(ObjectAccessor object)
{
    //FINJIN_EXPORTER_METHOD_ENTRY_FORMAT("wxT(BipedMixerAnimations::Initialize(%s)"), object.GetLocalName().wx_str());

    this->skinningModifier = nullptr;
    this->animations.clear();
                
    INode* rootBone = MaxUtilities::FindBipedRootBone(object.node, this->bipedAttachedWithPhysique);
    IBipMaster* bipedMaster = (rootBone != nullptr) ? GetBipMasterInterface(rootBone->GetTMController()) : nullptr;
    
    std::vector<IMixer*> mixers;
    mixers.reserve(2);
    IMixer* bipedMixer = (bipedMaster != nullptr) ? bipedMaster->GetMixer() : nullptr;    
    if (bipedMixer != nullptr)
        mixers.push_back(bipedMixer);
    //FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Biped mixer: %p"), bipedMixer);
    IMixer* maxMixer = TheMaxMixerManager.GetMaxMixer(object.node);    
    if (maxMixer != nullptr && maxMixer != bipedMixer)
        mixers.push_back(maxMixer);
    //FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Max mixer: %p"), maxMixer);
    if (!mixers.empty())
    {
        if (this->bipedAttachedWithPhysique)
            this->skinningModifier = MaxUtilities::FindPhysiqueModifier(object.node);
        else
            this->skinningModifier = MaxUtilities::FindSkinModifier(object.node);
    }
    //FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Skinning modifier: %p"), this->skinningModifier);

    if (!mixers.empty() && this->skinningModifier != nullptr)
    {
        this->animations.reserve(50);

        //FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("%d mixers"), mixers.size());
        for (size_t mixerIndex = 0; mixerIndex < mixers.size(); ++mixerIndex)
        {
            IMixer* mixer = mixers[mixerIndex];
            //FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("%d track groups"), mixer->NumTrackgroups());
            for (int trackGroupIndex = 0; trackGroupIndex < mixer->NumTrackgroups(); trackGroupIndex++)
            {        
                IMXtrackgroup* mixerTrackGroup = mixer->GetTrackgroup(trackGroupIndex);
                //FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("  %d tracks for group %d"), mixerTrackGroup->NumTracks(), trackGroupIndex);
                for (int trackIndex = 0; trackIndex < mixerTrackGroup->NumTracks(); trackIndex++)
                {
                    IMXtrack* mixerTrack = mixerTrackGroup->GetTrack(trackIndex);
                    //FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("    track type: %d"), mixerTrack->GetTrackType());
                    if (mixerTrack->GetTrackType() == LAYERTRACK)
                    {
                        for (int row = TOP_ROW; row <= BOT_ROW; row++)
                        {
                            //Get the animations
                            int clipCount = mixerTrack->NumClips(row);
                            for (int clipIndex = 0; clipIndex < clipCount; clipIndex++)
                            {
                                auto clip = mixerTrack->GetClip(clipIndex, row);

                                auto fileName = ApplicationStringToWxString(clip->GetFile().GetFullFilePath());

                                //Animation name
                                Animation animation;                                
                                animation.name = FileUtilities::GetFileName(fileName, false);

                                //Animation interval
                                int start, end;
                                clip->GetGlobalBounds(&start, &end);
                                animation.interval.start.SetNativeTime(start);
                                animation.interval.end.SetNativeTime(end);

                                this->animations.push_back(animation);                    
                            }
                        }
                    }
                }
            }
        }
    }

    return !this->animations.empty();
}
