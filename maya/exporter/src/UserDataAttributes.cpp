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
#include "UserDataAttributes.hpp"
#include "NodeAttributeAdder.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
const MString UserDataAttributes::ID_ATTRIBUTE_NAME = "ID";
const MString UserDataAttributes::USER_DATA_ATTRIBUTE_NAME = "UserData";
const MString UserDataAttributes::USER_DATA_CLASS_ATTRIBUTE_NAME = "UserDataClass";


//Implementation---------------------------------------------------------------
void UserDataAttributes::CreateUserDataAttributes(NodeAttributeAdder& adder, const MString& attributeNamePrefix)
{
    bool oldHidden = adder.hidden;
    adder.hidden = true;

    this->id = adder.AddString(attributeNamePrefix + ID_ATTRIBUTE_NAME);
    this->userData = adder.AddString(attributeNamePrefix + USER_DATA_ATTRIBUTE_NAME);
    this->userDataClass = adder.AddString(attributeNamePrefix + USER_DATA_CLASS_ATTRIBUTE_NAME);

    adder.hidden = oldHidden;
}

void UserDataAttributes::CreateUserDataAttributes(std::list<MObject>& attributesList, NodeAttributeAdder& adder, const MString& attributeNamePrefix)
{
    CreateUserDataAttributes(adder, attributeNamePrefix);
    
    attributesList.push_back(this->id);
    attributesList.push_back(this->userData);
    attributesList.push_back(this->userDataClass);
}
