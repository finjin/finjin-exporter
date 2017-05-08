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
#include "ObjectAccessor.hpp"
#include "FinjinObjectSettingsAccessor.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Performs sorting of an object's children. */
    class ChildObjectSorter
    {
    public:
        /** Contains a single child object as well as sorting info. */
        struct ChildObject
        {
            /** Default constructor. */
            ChildObject();

            /**
             * Constructor that takes all info.
             * @param object [in] - The child object.
             * @param objectSettings [in] - The child object settings.
             * @param data [in] - Data associated with the child object.
             * @param defaultOrder [in] - The default ordering of the child
             */
            ChildObject(ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, void* data = nullptr, int defaultOrder = 0);

            void Set(ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, void* data = nullptr, int defaultOrder = 0);

            /** The child object. */
            ObjectAccessor object;

            /** The child object settings. */
            FinjinObjectSettingsAccessor objectSettings;

            /** Data associated with the child object. */
            void* data;

            /** Name of the child object. */
            wxString name;

            /** The child order, obtained from the object settings. */
            int order;

            /** The child default order, obtained from the order in which it was encountered in the scene. */
            int defaultOrder;
        };

        /**
         * Constructor.
         * @param sortType [in] - The type of sorting to perform on children.
         */
        ChildObjectSorter(ChildSortType sortType = ChildSortType::NONE);

        /**
         * Functor that compares two child objects according to the sort type.
         */
        bool operator () (const ChildObject& a, const ChildObject& b) const;

    private:
        ChildSortType sortType;
    };

} }
