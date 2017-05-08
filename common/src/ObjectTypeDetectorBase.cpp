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
#include "ObjectTypeDetectorBase.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "StringUtilities.hpp"
#include "FinjinObjectSettingsAccessor.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
ObjectTypeDetectorBase::Detectors ObjectTypeDetectorBase::detectors;


//Implementation----------------------------------------------------------------
void ObjectTypeDetectorBase::AddDisallowedType(DetectableObjectType* type)
{
    this->disallowedTypes.insert(type);
}

void ObjectTypeDetectorBase::AddMappedType(DetectableObjectType* source, DetectableObjectType* destination)
{
    this->mappedTypes[source] = destination;
}

DetectableObjectType* ObjectTypeDetectorBase::Detect(ObjectAccessor object, FinjinObjectSettingsAccessorBase& objectSettings)
{
    return Detect(object, 0, false, objectSettings);
}

DetectableObjectType* ObjectTypeDetectorBase::Detect(ObjectAccessor object, const wxString& typeName, FinjinObjectSettingsAccessorBase& objectSettings)
{
    return Detect(object, &typeName, true, objectSettings);
}

DetectableObjectType* ObjectTypeDetectorBase::Detect(ObjectAccessor object, const wxString* typeNamePtr, bool useTypeNamePtr, FinjinObjectSettingsAccessorBase& objectSettings)
{
    //Initialize context
    ObjectTypeDetectionContext context;
    context.object = object;

    if (useTypeNamePtr)
    {
        if (typeNamePtr != nullptr)
            context.typeName = *typeNamePtr;
    }
    else if (((FinjinObjectSettingsAccessor&)objectSettings).IsValid())
        context.typeName = objectSettings.GetType();
    context.disallowedTypes = this->disallowedTypes;
    context.mappedTypes = this->mappedTypes;

    PrepareContext(context);

    //Check universal types. Universal types are always added
    for (auto detector : detectors)
    {
        if (detector->GetDescriptor().IsUniversal())
            context.AddType(detector, detector->GetDescriptor().GetTypeName() == context.typeName);
    }

    //Check type names. The first match is added
    if (!context.typeName.empty())
    {
        for (auto detector : detectors)
        {
            if (detector->GetDescriptor().GetTypeName() == context.typeName)
            {
                context.AddType(detector, true);
                break;
            }
        }
    }

    //Perform final detection
    for (auto detector : detectors)
        detector->Detect(context);

    //Collect results
    this->detectedTypes.resize(context.types.size());
    std::copy(context.types.begin(), context.types.end(), this->detectedTypes.begin());
    this->selectedType = ObjectTypeDetectionContext::GetSelectedType(this->detectedTypes);
    if (this->selectedType == 0 && !this->detectedTypes.empty())
    {
        //Try to get the first non-universal type
        for (size_t i = 0; i < this->detectedTypes.size(); i++)
        {
            if (!this->detectedTypes[i].type->GetDescriptor().IsUniversal() &&
                this->disallowedTypes.find(this->detectedTypes[i].type) == this->disallowedTypes.end())
            {
                this->selectedType = this->detectedTypes[i].type;
                break;
            }
        }

        //If no type was found, get the first universal type
        if (this->selectedType == 0)
        {
            for (size_t i = 0; i < this->detectedTypes.size(); i++)
            {
                if (this->detectedTypes[i].type->GetDescriptor().IsUniversal() &&
                    this->disallowedTypes.find(this->detectedTypes[i].type) == this->disallowedTypes.end())
                {
                    this->selectedType = this->detectedTypes[i].type;
                    break;
                }
            }
        }

        //If no type was found, just take the first one
        if (this->selectedType == 0)
            this->selectedType = this->detectedTypes.front().type;
    }

    //Sort the types
    std::sort(this->detectedTypes.begin(), this->detectedTypes.end());

    return this->selectedType;
}

void ObjectTypeDetectorBase::RegisterType(DetectableObjectType& detector)
{
    if (std::find(detectors.begin(), detectors.end(), &detector) == detectors.end())
        detectors.push_back(&detector);
}

int ObjectTypeDetectorBase::GetTypeCount()
{
    return (int)detectors.size();
}

void ObjectTypeDetectorBase::ClearTypes()
{
    detectors.clear();
}
