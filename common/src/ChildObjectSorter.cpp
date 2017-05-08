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
#include "ChildObjectSorter.hpp"
#include "StringUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
ChildObjectSorter::ChildObject::ChildObject()
{
    this->order = 0;
    this->defaultOrder = 0;
    this->data = nullptr;
}

ChildObjectSorter::ChildObject::ChildObject(ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, void* data, int defaultOrder)
{
    Set(object, objectSettings, data, defaultOrder);
}

void ChildObjectSorter::ChildObject::Set(ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, void* data, int defaultOrder)
{
    this->object = object;
    this->objectSettings = objectSettings;
    this->data = data;

    this->name = this->object.GetLocalName();

    this->order = this->objectSettings.IsValid() ? this->objectSettings.GetChildOrder() : defaultOrder;
    this->defaultOrder = defaultOrder;
}

ChildObjectSorter::ChildObjectSorter(ChildSortType sortType)
{
    this->sortType = sortType;
}

bool ChildObjectSorter::operator () (const ChildObject& a, const ChildObject& b) const
{
    switch (this->sortType)
    {
        case ChildSortType::BY_NAME: return a.name.CmpNoCase(b.name) < 0;
        case ChildSortType::BY_ORDER: return a.order < b.order;
        default: return a.defaultOrder < b.defaultOrder;
    }
}
