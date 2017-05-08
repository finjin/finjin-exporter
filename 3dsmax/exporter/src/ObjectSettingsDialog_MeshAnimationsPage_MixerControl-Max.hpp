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
#include "BipedMixerAnimations.hpp"
#include "FinjinObjectSettingsAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * Implements the mixer control logic for the 'Sync to Mixer' button
     * on the Mesh Animations page of the Object Settings dialog.
     */
    class ObjectSettingsDialog_MeshAnimationsPage_MixerControl
    {
    public:
        /**
         * Initializes the mixer control data.
         * @param object [in] - The object to check for mixer control data.
         * @param objectSettings [in] - The object settings.
         * @return If the object has a mixer control with some animations
         * defined on it, true is returned. Otherwise, false is returned.
         */
        bool Initialize(ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings);

        /**
         * Synchronizes the mesh animation settings to the mixer.
         * @return If at least one animation was updated or added, true is returned.
         * Otherwise, false is returned.
         */
        bool SyncSettingsToMixer();

    private:
        FinjinObjectSettingsAccessor objectSettings;
        BipedMixerAnimations bipedMixerAnimations;
    };

} }
