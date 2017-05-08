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

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
MaterialAccessor::MaterialAccessor()
{
    this->culling = DEFAULT_CULLING;
}

MaterialAccessor::MaterialAccessor(MObject obj)
{
    this->obj = obj;
    this->culling = DEFAULT_CULLING;
}

void MaterialAccessor::operator = (MObject obj)
{
    this->obj = obj;
}

bool MaterialAccessor::operator == (const MaterialAccessor& other) const
{
    return this->obj == other.obj && this->culling == other.culling;
}

bool MaterialAccessor::operator != (const MaterialAccessor& other) const
{
    return this->obj != other.obj || this->culling != other.culling;
}

bool MaterialAccessor::operator < (const MaterialAccessor& other) const
{
    return GetLocalName() < other.GetLocalName();
}

wxString MaterialAccessor::GetLocalName() const
{
    MFnDependencyNode depNode(this->obj);
    auto name = ApplicationStringToWxString(depNode.name());
    switch (this->culling)
    {
        case NO_CULLING: name += wxT("_NoCull"); break;
        case OPPOSITE_CULLING: name += wxT("_OppositeCull"); break;
        default: break;
    }
    return name;
}

wxString MaterialAccessor::GetFullName() const
{
    MFnDependencyNode depNode(this->obj);
    auto name = ApplicationStringToWxString(depNode.name());
    switch (this->culling)
    {
        case NO_CULLING: name += wxT("_NoCull"); break;
        case OPPOSITE_CULLING: name += wxT("_OppositeCull"); break;
        default: break;
    }
    return name;
}

bool MaterialAccessor::IsValid() const
{
    return !this->obj.isNull();
}

void MaterialAccessor::Resolve()
{
}

void MaterialAccessor::Expand(MaterialAccessorVector& materials)
{
    if (IsValid())
    {
        materials.resize(1);
        materials[0] = this->obj;
    }
    else
        materials.clear();
}

static MaterialAccessor GetShaderByName(const wxString& name, MFn::Type listType, const MString& listPlugName)
{
    MaterialAccessor material;

    if (!name.empty())
    {
        MString mayaName = WxStringToApplicationString(name);

        MItDependencyNodes nodeIt(listType);
        MFnDependencyNode depNode(nodeIt.item());
        MPlug shaders = depNode.findPlug(listPlugName);
        unsigned int count = shaders.evaluateNumElements();
        for (unsigned int i = 0; i < count; i++)
        {
            MPlug element = shaders.elementByLogicalIndex(i);
            MPlugArray inputs;
            element.connectedTo(inputs, true, false);
            if (inputs.length() > 0)
            {
                MObject object = inputs[0].node();
                MFnDependencyNode depNode(object);
                if (depNode.name() == mayaName)
                {
                    material = object;
                    break;
                }
            }
        }
    }

    return material;
}

MaterialAccessor MaterialAccessor::GetMaterialByName(const wxString& name)
{
    return GetShaderByName(name, MFn::kShaderList, "shaders");
}

MaterialAccessor MaterialAccessor::GetTextureByName(const wxString& name)
{
    return GetShaderByName(name, MFn::kTextureList, "textures");
}

void MaterialAccessor::GetAllMaterials(MaterialAccessorVector& materials)
{
    materials.clear();

    MItDependencyNodes nodeIt(MFn::kShaderList);
    MFnDependencyNode depNode(nodeIt.item());
    MPlug shaders = depNode.findPlug("shaders");
    unsigned int count = shaders.evaluateNumElements();
    materials.reserve(count);
    for (unsigned int i = 0; i < count; i++)
    {
        MPlug element = shaders.elementByLogicalIndex(i);
        MPlugArray inputs;
        element.connectedTo(inputs, true, false);
        if (inputs.length() > 0)
            materials.push_back(inputs[0].node());
    }
}

void MaterialAccessor::GetSelectedMaterials(MaterialAccessorVector& materials)
{
    materials.clear();

    MaterialAccessorVector allMaterials;
    GetAllMaterials(allMaterials);

    MSelectionList selection;
    MGlobal::getActiveSelectionList(selection);
    materials.reserve(selection.length());
    for (unsigned int i = 0; i < selection.length(); i++)
    {
        MObject obj;
        selection.getDependNode(i, obj);

        MaterialAccessor material(obj);
        if (std::find(allMaterials.begin(), allMaterials.end(), material) != allMaterials.end())
            materials.push_back(material);
    }
}
