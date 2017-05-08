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
#include "DetectableObjectType.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * This is base class for an application-specific object detect context.
     * A "context" provides and collects information about an object during type detection.
     */
    class ObjectTypeDetectionContextBase
    {
    public:
        virtual ~ObjectTypeDetectionContextBase() {}

        /** A detected type. */
        struct DetectedType
        {
            /** Default constructor. */
            DetectedType()
            {
                this->isSelected = false;
            }

            /**
             * Constructor that takes a type and a boolean indicating whether it was selected.
             * @param type [in] - The type that was detected.
             * @param isSelected [in] - Indicates whether the type was selected.
             */
            DetectedType(DetectableObjectType& type, bool isSelected)
            {
                this->type = &type;
                this->isSelected = isSelected;
            }

            /** Determines if this detected types name is less than the other's. Used during sorting. */
            bool operator < (const DetectedType& other) const
            {
                return this->type->GetDescriptor().GetDisplayName() < other.type->GetDescriptor().GetDisplayName();
            }

            /** Pointer to the detected type. */
            DetectableObjectType* type;

            /** Indicates whether the type is the selected type. */
            bool isSelected;
        };

        /**
         * Adds the type.
         * @param type [in] - Pointer to the detected type.
         * @param isSelected [in] - Indicates whether the type is the selected type.
         */
        void AddType(DetectableObjectType* type, bool isSelected = false)
        {
            if (this->disallowedTypes.find(type) == this->disallowedTypes.end())
            {
                auto mappedTypeIterator = this->mappedTypes.find(type);
                if (mappedTypeIterator != this->mappedTypes.end())
                    type = mappedTypeIterator->second;

                for (auto& detectedType : this->types)
                {
                    if (detectedType.type == type)
                    {
                        detectedType.isSelected |= isSelected;
                        return;
                    }
                }

                DetectedType detectedType(*type, isSelected);
                this->types.push_back(detectedType);
            }
        }

        /** Finds the first selected type. */
        const DetectableObjectType* GetSelectedType()
        {
             return GetSelectedType(this->types);
        }

        /** Finds the first selected type. */
        template <class T>
        static DetectableObjectType* GetSelectedType(T& detectedTypes)
        {
            for (auto& detectedType : detectedTypes)
            {
                if (detectedType.isSelected)
                    return detectedType.type;
            }
            return nullptr;
        }

        /** The object being detected. */
        ObjectAccessor object;

        /** The object's type value, as stored with the scene. */
        wxString typeName;

        /** The list of detected types into which the results go. */
        std::list<DetectedType> types;

        /** Types that are not allowed. Used for special cases. */
        std::unordered_set<DetectableObjectType*> disallowedTypes;

        /** Turns one detected type into another. Used for special cases. */
        std::unordered_map<DetectableObjectType*, DetectableObjectType*> mappedTypes;
    };

} }
