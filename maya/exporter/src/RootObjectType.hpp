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
#include "DetectableObjectType.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** DetectableObjectType for an object that is the scene root. */
    class RootObjectType : public DetectableObjectType
    {
    public:
        void Detect(ObjectTypeDetectionContext& context) override;

        DetectableObjectTypeDescriptor& GetDescriptor() override {return GetTypeDescriptor();}

        void GetDependencies(ExportableObject* exportableObject, ExporterContext& context) override;

        std::unique_ptr<ExportedObject> Export(ExportableObjectPtr& exportableObject, ExporterContext& context) override;

        class Descriptor : public DetectableObjectTypeDescriptor
        {
        public:
            const wxString& GetTypeName() const override {static const wxString name(wxT("root")); return name;}
            const wxString& GetDisplayName() const override {static const wxString name(wxT("Root")); return name;}
            bool IsContainer() const override {return true;}
            bool CanBePrefab() const override {return false;}
        };

        static Descriptor& GetTypeDescriptor()
        {
            static Descriptor desc;
            return desc;
        }

        static RootObjectType& GetInstance()
        {
            static RootObjectType detector;
            return detector;
        }
    };

} }
