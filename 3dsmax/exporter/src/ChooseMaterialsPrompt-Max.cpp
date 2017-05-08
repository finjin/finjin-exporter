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
#include "ChooseMaterialsPrompt-Max.hpp"
#include "MaxUtilities.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
ChooseMaterialsPrompt::ChooseMaterialsPrompt(wxWindow* parent, HowMany howMany)
{
    this->parent = parent;
}

bool ChooseMaterialsPrompt::ShowPrompt()
{
    BOOL newMat;
    BOOL cancel;
    auto sourceMaterial = GetCOREInterface()->DoMaterialBrowseDlg
        (
        (HWND)this->parent->GetHWND(),
        BROWSE_MATSONLY | BROWSE_INSTANCEONLY,
        newMat,
        cancel
        );

    if (cancel || sourceMaterial == nullptr)
    {
        //Do nothing
        sourceMaterial = nullptr;
    }

    this->materials.clear();
    if (sourceMaterial != nullptr)
    {
        this->materials.resize(1);
        this->materials[0] = (Mtl*)sourceMaterial;
    }

    return !this->materials.empty();
}

MaterialAccessor ChooseMaterialsPrompt::GetMaterial()
{
    MaterialAccessor material;
    if (!this->materials.empty())
        material = this->materials[0];
    return material;
}
