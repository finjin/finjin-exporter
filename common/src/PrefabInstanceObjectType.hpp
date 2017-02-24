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
#include "DetectableObjectType.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** DetectableObjectType for an object that is an instance of a model. */
    class PrefabInstanceObjectType : public DetectableObjectType
    {
    public:
        DetectableObjectTypeDescriptor& GetDescriptor() override {return GetTypeDescriptor();}

        class Descriptor : public DetectableObjectTypeDescriptor
        {
        public:
            const wxString& GetTypeName() const override {static const wxString name(wxT("prefab-instance")); return name;}
            const wxString& GetDisplayName() const override {static const wxString name(wxT("Prefab Instance")); return name;}
            bool CanAttachToNode() const override {return true;} //Assume that any prefab instance is for an attachable type
            bool IsUniversal() const override {return true;}
            bool CanBePrefab() const override {return false;}
        };

        static Descriptor& GetTypeDescriptor()
        {
            static Descriptor desc;
            return desc;
        }

        static PrefabInstanceObjectType& GetInstance()
        {
            static PrefabInstanceObjectType detector;
            return detector;
        }

        void CreateSettingsPages(TabControlPages& tabPages, ObjectAccessor& object, FinjinObjectSettingsAccessor& objectSettings, FinjinSceneSettingsAccessor& sceneSettings) override;
    };

} }
