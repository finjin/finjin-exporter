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


#pragma once


//Includes---------------------------------------------------------------------
#include "ObjectFilterType.hpp"
#include "ObjectAccessor.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** 
     * Prompts the user to choose one or more objects. 
     * This prompt uses the 3DS Max object selection dialog.
     */
    class ChooseObjectsPromptMax
    {
    public:
        /** 
         * Initializes the prompt.
         * @param parent [in] - The parent window.
         * @param howMany [in] - The number of objects that can be chosen.
         * @param types [in] - The types of objects to choose from.
         * @param chooseObjects [in] - An explicit set of objects to choose from.
         * @param excludeObjects [in] - A set of objects that cannot be chosen.
         */
        ChooseObjectsPromptMax
            (
            wxWindow* parent, 
            HowMany howMany = HowMany::CHOOSE_MANY, 
            ObjectFilterType types = ObjectFilterType::ALL, 
            const ObjectAccessorSet* chooseObjects = nullptr,
            const ObjectAccessorSet* excludeObjects = nullptr
            );
        
        /** 
         * Shows the dialog.
         * @return If the user selects at least one object, true is returned. 
         * Otherwise, false is returned.
         */
        bool ShowPrompt();

        /** Gets the object that was selected. */
        ObjectAccessor GetObject();

        /** Gets the objects that were selected. */
        const ObjectAccessorVector& GetObjects() {return this->objects;}

    private:

        /** Callback used by 3DS Max when determining which objects to show. */
        class SelectObjectCallback : public HitByNameDlgCallback 
        {
        public:
            SelectObjectCallback(ChooseObjectsPromptMax* prompt) {this->prompt = prompt;}

            const MCHAR* dialogTitle();
            const MCHAR* buttonText();

            BOOL singleSelect()    {return this->prompt->howMany == HowMany::CHOOSE_ONE;}

            int filter(INode* node);
            void proc(INodeTab& nodeTab);

        private:
            ChooseObjectsPromptMax* prompt;
        };

        wxWindow* parent;
        ObjectAccessorSet chooseObjects;
        ObjectAccessorSet excludeObjects;
        HowMany howMany;
        ObjectFilterType types;
        ObjectAccessorVector objects;
    };

} }
