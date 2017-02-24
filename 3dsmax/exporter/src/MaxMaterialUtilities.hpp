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


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Various utility functions for dealing with materials in 3DS Max.*/
    class MaxMaterialUtilities
    {
    public:    
        /** Determines if the specified material is a DMTL_CLASS_ID or DMTL2_CLASS_ID material. */
        static bool IsStandardMaterial(Mtl* mtl);

        /** Determines if the specified material is a MULTI_CLASS_ID material. */
        static bool IsMultiMaterial(Mtl* mtl);

        /** Gets the selected material in the material editor. */
        static Mtl* GetSelectedMaterialEditorMaterial();

        /** 
         * Gets the submaterial at the specified index.
         * @param mtl [in] - A material that is presumed to have submaterials. If it
         * doesn't have submaterials, then it is returned.
         * @param index [in] - Index of the material to retrieve. This is usually
         * a material identifier from a mesh.
         * @return The submaterial is returned.
         */
        static Mtl* GetMtl(Mtl* mtl, MtlID index);

        /** 
         * Gets the submaterial at the specified index.
         * @param mtl [in] - A material that is presumed to have submaterials. If it
         * doesn't have submaterials, then it is returned.
         * @param index [in] - Index of the material to retrieve. This is usually
         * a material identifier from a mesh.
         * @param trueIndex [out] - The index of the submaterial that is returned.
         * This will be different that index if the index is greater than the number
         * of submaterials.
         * @return The submaterial is returned.
         */
        static Mtl* GetMtl(Mtl* mtl, MtlID index, MtlID& trueIndex);
    };

} }
