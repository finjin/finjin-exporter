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


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Access to a material. */
    class MaterialAccessor
    {
    public:
        /** Default constructor. */
        MaterialAccessor();

        /** Constructor that takes a material. */
        MaterialAccessor(MObject obj);

        /** Assigns the material to this accessor. */
        void operator = (MObject obj);

        /** Determines whether this material equals the other material. */
        bool operator == (const MaterialAccessor& other) const;

        /** Determines whether this material doesn't equal the other material. */
        bool operator != (const MaterialAccessor& other) const;

        /** Determines whether this material is less than the other material. */
        bool operator < (const MaterialAccessor& other) const;

        /** Determines whether this material equals the other material, ignoring the parent material. */
        bool MaterialEquals(const MaterialAccessor& other) const {return *this == other;}

        /** Gets the material's parent material. */
        MaterialAccessor GetParentMaterial() {return MaterialAccessor();}

        /** Gets the material's local name. */
        wxString GetLocalName() const;

        /** Gets the material's full name. */
        wxString GetFullName() const;

        /** Determines whether the material is valid. */
        bool IsValid() const;

        /**
         * Resolves the material.
         * This is necessary for external materials.
         */
        void Resolve();

        /**
         * Expands this material's submaterials.
         * @param materials [out] - The expanded materials. If the material
         * has submaterials, the submaterials will be output. Otherwise,
         * just the material will be output.
         */
        void Expand(std::vector<MaterialAccessor>& materials);

        /** Gets a material by name. */
        static MaterialAccessor GetMaterialByName(const wxString& name);

        /** Gets a texture by name. */
        static MaterialAccessor GetTextureByName(const wxString& name);

        /** Gets all the scene materials. */
        static void GetAllMaterials(std::vector<MaterialAccessor>& materials);

        /** Gets all the selected materials. */
        static void GetSelectedMaterials(std::vector<MaterialAccessor>& materials);

    public:
        MObject obj;

        enum Culling
        {
            DEFAULT_CULLING,
            NO_CULLING,
            OPPOSITE_CULLING
        };
        Culling culling;
    };

} }


//Includes----------------------------------------------------------------------
#include "MaterialAccessorExtras.hpp"
