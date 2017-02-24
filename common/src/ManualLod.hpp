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
#include "ObjectAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** A manual level of detail object. */
    class ManualLod
    {
    public:
        ManualLod()
        {
            this->distance = 0;
            this->objectSettingsIndex = -1;
        }

        /** Determines whether this manual lod's distance is less than the other's. Used for sorting. */
        bool operator < (const ManualLod& other) const
        {
            return this->distance < other.distance;
        }

        /** Determines whether this manual lod's distance equal to the other's. */
        bool operator == (const ManualLod& other) const
        {
            return this->object == other.object && this->distance == other.distance;
        }

    public:
        /** The manual lod object. */
        ObjectAccessor object;

        /** The manual lod distance. */
        float distance;

        /** 
         * The index used to access this manual LOD in the object settings for an object.
         * This is used when editing manual LODs in the object settings dialog.
         */
        int objectSettingsIndex;
    };

    class ManualLods
    {
    public:
        bool empty() const {return this->lods.empty();}
        void clear() {this->lods.clear();}
        void resize(size_t newSize) {this->lods.resize(newSize);}
        size_t size() const {return this->lods.size();}
        ManualLod& operator [] (size_t i) {return this->lods[i];}
        const ManualLod& operator [] (size_t i) const {return this->lods[i];}

        bool operator == (const ManualLods& other) const
        {
            return this->lods == other.lods;
        }

        void Add(ObjectAccessor object, float distance, int objectSettingsIndex = -1)
        {
            ManualLod lod;
            lod.object = object;
            lod.distance = distance;
            lod.objectSettingsIndex = objectSettingsIndex;
            this->lods.push_back(lod);
        }

        void Sort()
        {
            std::stable_sort(this->lods.begin(), this->lods.end());
        }

    private:
        std::vector<ManualLod> lods;
    };

} }
