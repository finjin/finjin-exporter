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
#include "ChooseObjectsPrompt-Generic.hpp"
#include "ChooseObjectsDialog.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "ModalDialogFix.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
ChooseObjectsPrompt::ChooseObjectsPrompt
    (
    wxWindow* parent,
    HowMany howMany,
    ObjectFilterType types,
    const ObjectAccessorSet* chooseObjects,
    const ObjectAccessorSet* excludeObjects
    )
{
    this->parent = parent;
    this->howMany = howMany;
    this->types = types;
    if (chooseObjects != nullptr)
        this->chooseObjects = *chooseObjects;
    if (excludeObjects != nullptr)
        this->excludeObjects = *excludeObjects;
}

bool ChooseObjectsPrompt::ShowPrompt()
{
    SUSPEND_CUSTOM_CONTROLS;

    ObjectAccessorSet* chooseObjects = nullptr;
    if (!this->chooseObjects.empty())
        chooseObjects = &this->chooseObjects;

    ObjectAccessorSet* excludeObjects = nullptr;
    if (!this->excludeObjects.empty())
        excludeObjects = &this->excludeObjects;

    ChooseObjectsDialog dialog(this->parent, this->howMany == HowMany::CHOOSE_ONE, this->types, chooseObjects, excludeObjects);
    ModalDialogFix modalDialogFix(&dialog);
    bool result = dialog.ShowModal() == wxID_OK;
    if (result)
        this->objects = dialog.chosenObjects;
    return result;
}

ObjectAccessor ChooseObjectsPrompt::GetObject()
{
    ObjectAccessor object;
    if (!this->objects.empty())
        object = this->objects[0];
    return object;
}
