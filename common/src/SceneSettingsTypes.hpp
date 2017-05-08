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


//Includes----------------------------------------------------------------------
#include "SceneSettingsType.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** A global collection of scene settings types. */
    class SceneSettingsTypes
    {
    public:
        /** Registers a scene settings type. */
        static void RegisterType(SceneSettingsType& type);

        /** Gets the number of registered scene settings types. */
        static int GetTypeCount();

        /** Gets the scene settings type at the specified index. */
        static SceneSettingsType* GetType(int i);

        /** Clears the registered types. */
        static void ClearTypes();

    private:
        typedef std::vector<SceneSettingsType*> Types;
        static Types types;
    };

} }
