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


//Includes----------------------------------------------------------------------
#include "FinjinPrecompiled.hpp"
#include "ChooseMaterialsPrompt-Generic.hpp"
#include "ChooseMaterialsDialog.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "ModalDialogFix.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
ChooseMaterialsPrompt::ChooseMaterialsPrompt(wxWindow* parent, HowMany howMany)
{
    this->parent = parent;
    this->howMany = howMany;
}

bool ChooseMaterialsPrompt::ShowPrompt() 
{
    SUSPEND_CUSTOM_CONTROLS;

    ChooseMaterialsDialog dialog(this->parent, this->howMany == HowMany::CHOOSE_ONE);
    ModalDialogFix modalDialogFix(&dialog);
    bool result = dialog.ShowModal() == wxID_OK;
    if (result)
        this->materials = dialog.chosenObjects;
    return result;
}

MaterialAccessor ChooseMaterialsPrompt::GetMaterial()
{
    MaterialAccessor material;
    if (!this->materials.empty())
        material = this->materials[0];
    return material;
}
