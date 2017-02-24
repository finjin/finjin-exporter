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
#include "ExtendedValueParameters.hpp"
#include "ExtendedValueObject.hpp"

using namespace Finjin::Exporter;


//Local functions--------------------------------------------------------------
static ExtendedValueObject* FindExtendedValue(IParamBlock2* pblock, int paramID, int id, int* foundIndex = nullptr)
{
    if (foundIndex != nullptr)
        *foundIndex = -1;
    ExtendedValueObject* foundObject = nullptr;

    int count = pblock->Count(paramID);
    for (int valueIndex = 0; valueIndex < count && foundObject == nullptr; valueIndex++)
    {
        ExtendedValueObject* extendedValueObject = (ExtendedValueObject*)pblock->GetReferenceTarget(paramID, 0, valueIndex);        
        if (id == extendedValueObject->GetID())
        {
            if (foundIndex != nullptr)
                *foundIndex = valueIndex;
            foundObject = extendedValueObject;
        }
    }

    return foundObject;
}


//Implementation---------------------------------------------------------------
void ExtendedValueParameters::GetAllExtendedValues(IParamBlock2* pblock, int paramID, std::vector<ExtendedValue>& values, std::vector<int>* ids)
{
    values.resize(pblock->Count(paramID));
    if (ids != nullptr)
        ids->resize(values.size());
    for (size_t i = 0; i < values.size(); i++)
    {
        ExtendedValueObject* extendedValueObject = (ExtendedValueObject*)pblock->GetReferenceTarget(paramID, 0, (int)i);
        
        if (ids != nullptr)
            (*ids)[i] = extendedValueObject->GetID();

        extendedValueObject->GetValue(values[i]);
    }
}

bool ExtendedValueParameters::GetExtendedValue(IParamBlock2* pblock, int paramID, int id, ExtendedValue& value)
{
    ExtendedValueObject* extendedValueObject = FindExtendedValue(pblock, paramID, id);
    if (extendedValueObject != nullptr)
        extendedValueObject->GetValue(value);

    return extendedValueObject != nullptr;
}

void ExtendedValueParameters::SetExtendedValue(IParamBlock2* pblock, int paramID, int id, const ExtendedValue& value)
{
    ExtendedValueObject* extendedValueObject = FindExtendedValue(pblock, paramID, id);
    if (extendedValueObject == nullptr)
    {
        extendedValueObject = new ExtendedValueObject;
        
        ReferenceTarget* extendedValueObjects[1] = {extendedValueObject};
        pblock->Append(paramID, 1, extendedValueObjects);
    }
    extendedValueObject->SetID(id);
    extendedValueObject->SetValue(value);
}

bool ExtendedValueParameters::RemoveExtendedValue(IParamBlock2* pblock, int paramID, int id)
{
    int index;
    ExtendedValueObject* extendedValueObject = FindExtendedValue(pblock, paramID, id, &index);
    if (extendedValueObject != nullptr)
        pblock->Delete(paramID, index, 1);

    return extendedValueObject != nullptr;
}

void ExtendedValueParameters::ClearExtendedValues(IParamBlock2* pblock, int paramID)
{
    pblock->ZeroCount(paramID);
}
