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
#include "FinjinObjectSettingsAccessor.hpp"
#include "MeshAnimationType.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * Detects the valid output mesh animation types for a mesh.
     *
     * When using the Finjin exporter, you can find the types on the mesh animations page
     * of the object settings dialog for a particular object.
     */
    class MeshAnimationTypeDetector
    {
    public:
        MeshAnimationTypeDetector();

        virtual ~MeshAnimationTypeDetector() {}

        /**
         * Detects the output mesh animation types for the specified mesh object.
         * @param object [in] - The object.
         * @param objectSettings [in] - The object settings.
         */
        void Detect(ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings);

        /**
         * Detects the output mesh animation types for the specified mesh object.
         * @param object [in] - The object.
         * @param typeNamePtr [in] - The selected animation type name. This will be one
         * of the values in PropertyValues::MeshAnimationType or an empty string. This may be null.
         * @param useTypeNamePtr [in] - Indicates whether the specified type name should be used.
         * If false, the mesh animation type in the object settings is used. Setting this value to true
         * and passing null for typeNamePtr allows you to get the default mesh animation type.
         * @param objectSettings [in] - The object settings.
         */
        void Detect
            (
            ObjectAccessor object,
            const wxString* typeNamePtr,
            bool useTypeNamePtr,
            FinjinObjectSettingsAccessor objectSettings
            );

        /**
         * Finds the index of the specified type.
         * @param name [in] - One of the values in PropertyValues::MeshAnimationType.
         * @return If found, the index of the type is returned. Otherwise, -1 is returned.
         */
        int FindTypeIndex(const wxString& name) const;

        /**
         * Finds the matching MeshAnimationType.
         * @param type [in] - The type to search for.
         * @return If found, a pointer to the matching MeshAnimationType is returned. Otherwise,
         * null is returned.
         */
        MeshAnimationType* FindType(MeshAnimationTypes type);

    private:
        /**
         * Adds a single mesh animation type.
         * @param type [in] - The mesh animation type.
         * @param name [in] - The mesh animation type name. This will be one of the values in PropertyValues::MeshAnimationType.
         * @param displayName [in] - The display name for the type.
         */
        void AddType(MeshAnimationTypes type, const wxString& name, const wxString& displayName);

        /** Detects all the mesh animation types and finds the selected type. */
        void DetectTypes(const wxString& typeName);

    public:
        /** The selected type */
        MeshAnimationType* selectedType;

        /** All the types that are allowed, including the selected type */
        std::vector<MeshAnimationType> types;

        /** Indicates whether the object has a skeleton. */
        bool hasSkeleton;

        /** Indicates whether the object has a morpher. */
        bool hasMorpher;
    };

} }
