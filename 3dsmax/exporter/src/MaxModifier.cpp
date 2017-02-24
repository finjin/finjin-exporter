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
#include "MaxModifier.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
MaxModifier::MaxModifier()
{
    Clear();
}

MaxModifier::MaxModifier(Modifier* modifier) 
{
    this->modifier = modifier;
    this->originalState = (this->modifier != nullptr) ? this->modifier->IsEnabled() : FALSE;
}

MaxModifier::MaxModifier(const MaxModifier& maxModifier)
{
    this->modifier = maxModifier.modifier;
    this->originalState = maxModifier.originalState;
}

void MaxModifier::Clear()
{
    this->modifier = nullptr;
    this->originalState = FALSE;
}

bool MaxModifier::IsValid() const
{
    return this->modifier != nullptr;
}

Modifier* MaxModifier::operator -> ()
{
    return this->modifier;
}

MaxModifier::operator Modifier* ()
{
    return this->modifier;
}

void MaxModifier::Enable(bool enable)
{
    if (this->modifier != nullptr)
    {
        if (enable)
            this->modifier->EnableMod();
        else
            this->modifier->DisableMod();
    }
}

void MaxModifier::Disable()
{
    Enable(false);
}

void MaxModifier::Restore()
{
    if (this->modifier != nullptr)
    {
        if (this->originalState)
            this->modifier->EnableMod();
        else
            this->modifier->DisableMod();
    }
}
