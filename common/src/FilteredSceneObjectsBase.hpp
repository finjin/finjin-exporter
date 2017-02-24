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
#include "ObjectFilterType.hpp"
#include "ObjectAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Base class for filtering objects based on the selected types. */
    class FilteredSceneObjectsBase
    {
    public:
        /** 
         * Constructor.
         * @param types [in] - The types of objects to allow.
         */
        FilteredSceneObjectsBase(ObjectFilterType types = ObjectFilterType::ALL)
        {
            this->types = types;
        }

        /** Destructor. */
        virtual ~FilteredSceneObjectsBase()
        {
        }

        /** Filters all the objects as appropriate, placing the results into the 'items' member. */
        virtual void Initialize(bool sortByFullName) = 0;

        /** Sets the types of objects to allow. */
        void SetTypes(ObjectFilterType types)
        {
            this->types = types;
        }

        /** 
         * Sets the objects that are explicitly filtered. 
         * If there are no explicit objects to filter, all the scene objects are used.
         */
        void SetChooseObjects(const ObjectAccessorSet& chooseObjects)
        {
            this->chooseObjects = chooseObjects;
        }

        /** Sets the objects that are never allowed. */
        void SetExcludeObjects(const ObjectAccessorSet& excludeObjects)
        {
            this->excludeObjects = excludeObjects;
        }

        /** Gets the number of filtered objects. */
        size_t size() const
        {
            return this->items.size();
        }

        /** A filtered object. */
        struct FilteredObject
        {
            /** The object. */
            ObjectAccessor object;

            /** The type. */
            ObjectFilterType type;

            bool operator == (const FilteredObject& other) const
            {
                return this->object == other.object;
            }
        };

        /** Gets the filtered object at the specified index. */
        FilteredObject& operator [] (size_t i)
        {
            return this->items[i];
        }

        void Sort(bool useFullName)
        {
            if (useFullName)
                std::sort(this->items.begin(), this->items.end(), SortByFullName);
            else
                std::sort(this->items.begin(), this->items.end(), SortByName);
        }

    protected:
        /** Determines if this object is less than the other by comparing full names. */
        static bool SortByFullName(const FilteredObject& a, const FilteredObject& b)
        {            
            return a.object.GetFullName() < b.object.GetFullName();
        }

        /** Determines if this object is less than the other by comparing local names. */
        static bool SortByName(const FilteredObject& a, const FilteredObject& b)
        {            
            return a.object.GetLocalName() < b.object.GetLocalName();
        }

    protected:
        ObjectFilterType types;
        ObjectAccessorSet chooseObjects;
        ObjectAccessorSet excludeObjects;
        std::vector<FilteredObject> items;
    };

} }
