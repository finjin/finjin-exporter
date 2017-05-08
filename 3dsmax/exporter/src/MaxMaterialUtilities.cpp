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
#include "MaxMaterialUtilities.hpp"
#include "MaxControlUtilities.hpp"
#include "StringUtilities.hpp"
#include "MaterialAccessor.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
bool MaxMaterialUtilities::IsStandardMaterial(Mtl* mtl)
{
    return
        mtl != nullptr &&
        (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0) || mtl->ClassID() == Class_ID(DMTL2_CLASS_ID, 0));
}

bool MaxMaterialUtilities::IsMultiMaterial(Mtl* mtl)
{
    return mtl != nullptr && mtl->ClassID() == Class_ID(MULTI_CLASS_ID, 0);
}

Mtl* MaxMaterialUtilities::GetSelectedMaterialEditorMaterial()
{
    Mtl* selectedMaterial = nullptr;
    int slotIndex = GetCOREInterface7()->GetActiveMtlSlot();
    if (slotIndex >= 0)
        selectedMaterial = (Mtl*)GetCOREInterface()->GetMtlSlot(slotIndex);
    return selectedMaterial;
}

Mtl* MaxMaterialUtilities::GetMtl(Mtl* mtl, MtlID index)
{
    if (mtl != nullptr && IsMultiMaterial(mtl))
        return mtl->GetSubMtl(index % mtl->NumSubMtls());
    else
        return mtl;
}

Mtl* MaxMaterialUtilities::GetMtl(Mtl* mtl, MtlID index, MtlID& trueIndex)
{
    if (mtl != nullptr && IsMultiMaterial(mtl))
    {
        trueIndex = index % mtl->NumSubMtls();
        return mtl->GetSubMtl(trueIndex);
    }
    else
    {
        trueIndex = 0;
        return mtl;
    }
}
