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
#include "MeshAnimationTypeDetector.hpp"
#include "Strings.hpp"
#include "PropertyValues.hpp"
#include "StringUtilities.hpp"
#include "MeshSkeleton.hpp"
#include "MeshMorpher.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
MeshAnimationTypeDetector::MeshAnimationTypeDetector() 
{
    this->selectedType = nullptr;
    this->types.reserve((size_t)MeshAnimationTypes::COUNT);
    this->hasSkeleton = this->hasMorpher = false;
}

void MeshAnimationTypeDetector::Detect(ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings) 
{
    auto typeName = objectSettings.GetMeshAnimationType();
    Detect(object, &typeName, true, objectSettings);
}

void MeshAnimationTypeDetector::Detect(ObjectAccessor object, const wxString* typeNamePtr, bool useTypeNamePtr, FinjinObjectSettingsAccessor objectSettings)
{
    this->hasSkeleton = MeshSkeleton::HasSkeleton(object);
    this->hasMorpher = MeshMorpher::HasMorpher(object);
        
    //Get mesh animation type text
    wxString typeName;
    if (useTypeNamePtr)
    {        
        if (typeNamePtr != nullptr)
            typeName = *typeNamePtr;
    }
    else
        typeName = objectSettings.GetMeshAnimationType();

    DetectTypes(typeName);
}

int MeshAnimationTypeDetector::FindTypeIndex(const wxString& name) const
{
    int index = -1;
    for (size_t i = 0; i < this->types.size(); i++)
    {
        if (this->types[i].name == name)
        {
            index = (int)i;
            break;
        }
    }
    return index;
}

MeshAnimationType* MeshAnimationTypeDetector::FindType(MeshAnimationTypes type)
{
    for (size_t i = 0; i < this->types.size(); i++)
    {
        if (this->types[i].type == type)
            return &this->types[i];
    }
    return nullptr;
}

void MeshAnimationTypeDetector::AddType(MeshAnimationTypes type, const wxString& name, const wxString& displayName)
{
    MeshAnimationType animationType;            
    animationType.type = type;
    animationType.name = name;
    animationType.displayName = displayName;
    this->types.push_back(animationType);
}

void MeshAnimationTypeDetector::DetectTypes(const wxString& typeName)
{
    //Determine the type
    if (this->hasSkeleton && this->hasMorpher)
    {
        AddType(MeshAnimationTypes::SKELETON_AND_POSE, wxEmptyString, Strings::SKELETON_AND_POSE);
        AddType(MeshAnimationTypes::SKELETON_AND_MORPH, PropertyValues::MeshAnimationType::MORPH, Strings::SKELETON_AND_MORPH);
        AddType(MeshAnimationTypes::MORPH_ONLY, PropertyValues::MeshAnimationType::MORPH_ONLY, Strings::MORPH);
        AddType(MeshAnimationTypes::COMBINED_MORPH_ONLY, PropertyValues::MeshAnimationType::COMBINED_MORPH_ONLY, Strings::COMBINED_MORPH);

        if (typeName == PropertyValues::MeshAnimationType::MORPH_ONLY)
            this->selectedType = FindType(MeshAnimationTypes::MORPH_ONLY);            
        else if (typeName == PropertyValues::MeshAnimationType::COMBINED_MORPH_ONLY)
            this->selectedType = FindType(MeshAnimationTypes::COMBINED_MORPH_ONLY);            
        else if (typeName == PropertyValues::MeshAnimationType::MORPH)
            this->selectedType = FindType(MeshAnimationTypes::SKELETON_AND_MORPH);            
        else
            this->selectedType = FindType(MeshAnimationTypes::SKELETON_AND_POSE);
    }
    else if (this->hasSkeleton)
    {
        AddType(MeshAnimationTypes::SKELETON_ONLY, wxEmptyString, Strings::SKELETON);
        AddType(MeshAnimationTypes::SKELETON_AND_MORPH, PropertyValues::MeshAnimationType::MORPH, Strings::SKELETON_AND_MORPH);
        AddType(MeshAnimationTypes::MORPH_ONLY, PropertyValues::MeshAnimationType::MORPH_ONLY, Strings::MORPH);
        AddType(MeshAnimationTypes::COMBINED_MORPH_ONLY, PropertyValues::MeshAnimationType::COMBINED_MORPH_ONLY, Strings::COMBINED_MORPH);

        if (typeName == PropertyValues::MeshAnimationType::MORPH)
            this->selectedType = FindType(MeshAnimationTypes::SKELETON_AND_MORPH);
        else if (typeName == PropertyValues::MeshAnimationType::MORPH_ONLY)
            this->selectedType = FindType(MeshAnimationTypes::MORPH_ONLY);
        else if (typeName == PropertyValues::MeshAnimationType::COMBINED_MORPH_ONLY)
            this->selectedType = FindType(MeshAnimationTypes::COMBINED_MORPH_ONLY);            
        else
            this->selectedType = FindType(MeshAnimationTypes::SKELETON_ONLY);
    }
    else if (this->hasMorpher)
    {
        AddType(MeshAnimationTypes::POSE_ONLY, wxEmptyString, Strings::POSE);
        AddType(MeshAnimationTypes::MORPH_ONLY, PropertyValues::MeshAnimationType::MORPH, Strings::MORPH);
        AddType(MeshAnimationTypes::COMBINED_MORPH_ONLY,PropertyValues::MeshAnimationType::COMBINED_MORPH_ONLY, Strings::COMBINED_MORPH);

        if (typeName == PropertyValues::MeshAnimationType::MORPH || typeName == PropertyValues::MeshAnimationType::MORPH_ONLY)
            this->selectedType = FindType(MeshAnimationTypes::MORPH_ONLY);            
        else if (typeName == PropertyValues::MeshAnimationType::COMBINED_MORPH_ONLY)
            this->selectedType = FindType(MeshAnimationTypes::COMBINED_MORPH_ONLY);            
        else
            this->selectedType = FindType(MeshAnimationTypes::POSE_ONLY);
    }
    else
    {
        AddType(MeshAnimationTypes::MORPH_ONLY, PropertyValues::MeshAnimationType::MORPH_ONLY, Strings::MORPH);
        AddType(MeshAnimationTypes::COMBINED_MORPH_ONLY, PropertyValues::MeshAnimationType::COMBINED_MORPH_ONLY, Strings::COMBINED_MORPH);
        this->selectedType = FindType(MeshAnimationTypes::MORPH_ONLY);
    }
}
