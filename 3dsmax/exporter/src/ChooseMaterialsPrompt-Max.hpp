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
#include "MaterialAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Prompts the user to choose one or more Finjin materials. */
    class ChooseMaterialsPrompt
    {
    public:
        /** 
         * Initializes the prompt.
         * @param parent [in] - The parent window.
         * @param howMany [in] - The number of materials that can be chosen.
         * Since the 3DS Max material dialog only allows one material to be
         * selected, this is treated as HowMany::CHOOSE_ONE.
         */
        ChooseMaterialsPrompt(wxWindow* parent, HowMany howMany = HowMany::CHOOSE_MANY);

        /** 
         * Shows the dialog.
         * @return If the user selects at least one Finjin material, true is returned. 
         * Otherwise, false is returned.
         */
        bool ShowPrompt();

        /** Gets the material that was selected. */
        MaterialAccessor GetMaterial();

        /** Gets the materials that were selected. */
        const MaterialAccessorVector& GetMaterials() {return this->materials;}

    private:
        wxWindow* parent;
        MaterialAccessorVector materials;
    };

} }
