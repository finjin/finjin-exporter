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
#include "AccessorSetAndMap.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    typedef AccessorSet<ObjectAccessor> ObjectAccessorSet;
    typedef std::list<ObjectAccessor> ObjectAccessorList;
    typedef std::vector<ObjectAccessor> ObjectAccessorVector;

    /** Determines if two object accessor sets contain the same objects */
    inline bool ObjectAccessorSetsSame(const ObjectAccessorSet& a, const ObjectAccessorSet& b)
    {
        if (a.size() != b.size())
            return false;

        for (ObjectAccessorSet::const_iterator i = a.begin(); i != a.end(); ++i)
        {
            if (b.find(*i) == b.end())
                return false;
        }

        return true;
    }

    /**
     * Determines if object accessor set 'a' contains object accessor set 'b'.
     */
    inline bool ObjectAccessorSetContains(const ObjectAccessorSet& a, const ObjectAccessorSet& b)
    {
        for (ObjectAccessorSet::const_iterator i = b.begin(); i != b.end(); ++i)
        {
            if (a.find(*i) == a.end())
                return false;
        }

        return true;
    }

    inline void GetObjectTree(ObjectAccessor object, ObjectAccessorSet& treeObjects)
    {    
        treeObjects.insert(object);

        ObjectAccessorVector children;
        object.GetChildren(children);
        for (size_t i = 0; i < children.size(); i++)
            GetObjectTree(children[i], treeObjects);
    }

} }
