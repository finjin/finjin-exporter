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
#include "ObjectTypeDetectionContext.hpp"
#include "DetectableObjectType.hpp"
#include "FinjinObjectSettingsAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {
    
    /** 
     * Base class for detecting object types.
     */ 
    class ObjectTypeDetectorBase
    {
    public:
        /** Constructor. */
        ObjectTypeDetectorBase() {this->selectedType = nullptr;}
        
        /** Destructor. */
        virtual ~ObjectTypeDetectorBase() {}

        /** 
         * Called to prepare the object type detection context.
         * An overridden implementation can add some extra data to the context if necessary.
         */
        virtual void PrepareContext(ObjectTypeDetectionContext& context) {}

        /** 
         * Adds a disallowed type.
         * This should be called before calling Detect().
         */
        void AddDisallowedType(DetectableObjectType* type);

        /** 
         * Adds a mapped type.
         * This should be called before calling Detect().
         */
        void AddMappedType(DetectableObjectType* source, DetectableObjectType* destination);

        /**
         * Detects the types that the object can be assigned.
         * @param object [in] - The object.
         * @param objectSettings [in] - The object settings.
         */
        DetectableObjectType* Detect(ObjectAccessor object, FinjinObjectSettingsAccessorBase& objectSettings);

        /**
         * Detects the types that the object can be assigned.
         * @param object [in] - The object.
         * @param typeName [in] - The object's currently assigned type name.
         * @param objectSettings [in] - The object settings.
         */
        DetectableObjectType* Detect(ObjectAccessor object, const wxString& typeName, FinjinObjectSettingsAccessorBase& objectSettings);

        /**
         * Detects the types that the object can be assigned.
         * @param object [in] - The object.
         * @param typeNamePtr [in] - The object's currently assigned type name. This can be null.
         * @param useTypeNamePtr [in] - Indicates whether typeNamePtr should be used. If false, the type
         * is retrieved from the object settings.
         * @param objectSettings [in] - The object settings.
         */
        DetectableObjectType* Detect(ObjectAccessor object, const wxString* typeNamePtr, bool useTypeNamePtr, FinjinObjectSettingsAccessorBase& objectSettings);

        /** Global function for registering a detectable object type. */
        static void RegisterType(DetectableObjectType& detector);

        /** Gets the number of registered types. */
        static int GetTypeCount();

        /** Clears the registered types. */
        static void ClearTypes();

    public:
        /** The selected type. */
        DetectableObjectType* selectedType;

        /** 
         * Contains all the types that the object can be assigned, including the selected type.
         * This will be valid after the type detection has occurred.
         */
        std::vector<ObjectTypeDetectionContext::DetectedType> detectedTypes;

        /** Types that are not allowed. Used for special cases. */
        std::unordered_set<DetectableObjectType*> disallowedTypes;

        /** Turns one detected type into another. Used for special cases. */
        std::unordered_map<DetectableObjectType*, DetectableObjectType*> mappedTypes;

    protected:
        typedef std::list<DetectableObjectType*> Detectors;
        static Detectors detectors;
    };

} }
