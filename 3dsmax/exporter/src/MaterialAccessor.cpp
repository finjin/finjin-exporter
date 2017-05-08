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
#include "MaterialAccessor.hpp"
#include "MaxMaterialUtilities.hpp"
#include <XRef\iXrefMaterial.h>

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
MaterialAccessor::MaterialAccessor()
{
    this->parentMtl = nullptr;
    this->mtl = nullptr;
}

MaterialAccessor::MaterialAccessor(Mtl* mtl)
{
    this->parentMtl = nullptr;
    this->mtl = mtl;
}

MaterialAccessor::MaterialAccessor(const MaterialAccessor& other)
{
    this->parentMtl = other.parentMtl;
    this->mtl = other.mtl;
}

MaterialAccessor::operator Mtl* ()
{
    return this->mtl;
}

void MaterialAccessor::operator = (Mtl* mtl)
{
    this->parentMtl = nullptr;
    this->mtl = mtl;
}

void MaterialAccessor::operator = (const MaterialAccessor& other)
{
    this->parentMtl = other.parentMtl;
    this->mtl = other.mtl;
}

bool MaterialAccessor::operator == (const MaterialAccessor& other) const
{
    return this->parentMtl == other.parentMtl && this->mtl == other.mtl;
}

bool MaterialAccessor::operator != (const MaterialAccessor& other) const
{
    return this->parentMtl != other.parentMtl || this->mtl != other.mtl;
}

bool MaterialAccessor::operator < (const MaterialAccessor& other) const
{
    if (this->parentMtl < other.parentMtl)
        return true;

    return this->mtl < other.mtl;
}

bool MaterialAccessor::MaterialEquals(const MaterialAccessor& other) const
{
    return this->mtl == other.mtl;
}

MaterialAccessor MaterialAccessor::GetParentMaterial()
{
    return this->parentMtl;
}

wxString MaterialAccessor::GetLocalName() const
{
    return IsValid() ? ApplicationStringToWxString(this->mtl->GetName()) : wxEmptyString;
}

wxString MaterialAccessor::GetFullName() const
{
    return GetLocalName();
}

bool MaterialAccessor::IsValid() const
{
    return this->mtl != nullptr;
}

void MaterialAccessor::Resolve()
{
    if (this->mtl != nullptr && this->mtl->ClassID() == XREFMATERIAL_CLASS_ID)
    {
        IXRefMaterial* xrefMaterial = (IXRefMaterial*)this->mtl;
        this->mtl = xrefMaterial->GetSourceMaterial(true);
    }
}

void MaterialAccessor::Expand(MaterialAccessorVector& materials)
{
    if (this->mtl != nullptr)
    {
        if (MaxMaterialUtilities::IsMultiMaterial(this->mtl))
        {
            materials.resize(this->mtl->NumSubMtls());
            for (int i = 0; i < (int)materials.size(); i++)
            {
                materials[i].parentMtl = this->mtl;
                materials[i].mtl = this->mtl->GetSubMtl(i);
            }
        }
        else
        {
            materials.resize(1);
            materials[0].mtl = this->mtl;
        }
    }
    else
        materials.clear();
}

MaterialAccessor MaterialAccessor::GetMaterialByName(const wxString& name)
{
    MSTR maxName = WxStringToApplicationStringM(name);
    return GetCOREInterface7()->FindMtlNameInScene(maxName);
}

void MaterialAccessor::GetAllMaterials(MaterialAccessorVector& materials)
{
    Tab<MtlBaseHandle>& materialTab = *GetCOREInterface()->GetSceneMtls();

    materials.resize(materialTab.Count());
    for (int i = 0; i < materialTab.Count(); i++)
        materials[i] = (Mtl*)materialTab[i];
}

void MaterialAccessor::GetSelectedMaterials(MaterialAccessorVector& materials)
{
    MtlBase* selectedMaterial = MaxMaterialUtilities::GetSelectedMaterialEditorMaterial();
    if (selectedMaterial != nullptr)
    {
        materials.resize(1);
        materials[0] = (Mtl*)selectedMaterial;
    }
    else
        materials.clear();
}
