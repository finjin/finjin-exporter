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
#include "DetectableObjectTypeDescriptor.hpp"
#include "TabControl.hpp"
#include "ExportedObject.hpp"
#include "ExportableObject.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {
    
    class ObjectTypeDetectionContext;
    class ObjectAccessor;
    class FinjinObjectSettingsAccessor;
    class FinjinSceneSettingsAccessor;
    class ExporterHandler;
        
    /** 
     * This class specifies the base implementation for a type that can be applied to objects in the scene.
     * It is "detectable" in the sense that while objects have a specific type in an application, any one of
     * a number types can be applied to them when exporting. So, there is a process to detect which types
     * are allowed on a particular object and then the best type is assigned, either automatically or by the user.
     */
    class DetectableObjectType
    {
    public:
        virtual ~DetectableObjectType() {}
        
        /** 
         * Assists in detecting the type.
         * @param context [in/out] - Contains information about the object whose type is being determined.
         */
        virtual void Detect(ObjectTypeDetectionContext& context) {}

        /** Gets the type descriptor. */
        virtual DetectableObjectTypeDescriptor& GetDescriptor() = 0;        

        /** Creates the object settings pages for the type. */
        virtual void CreateSettingsPages(TabControlPages& tabPages, ObjectAccessor& object, FinjinObjectSettingsAccessor& objectSettings, FinjinSceneSettingsAccessor& sceneSettings) {}

        /** 
         * Determines whether the specified child is consumed by its parent.
         * This method will always be called on the parent's type.
         */
        virtual bool IsConsumedChild(ExportableObject* child) {return GetDescriptor().ConsumesAllChildren();}

        /** Gets the objects that the specified object dependends on. */
        virtual void GetDependencies(ExportableObject* exportableObject, ExporterContext& context) {}

        /** Performs some final resolution on the objects that the specified object depends on. */
        virtual void ResolveDependencies(ExportableObject* exportableObject, ExporterContext& context) {}

        /** 
         * Generates an ExportedObject for the specified exportable object.
         * Implementations are free to create just one ExportedObject that contains everything, or a hierarchy
         * of ExportedObjects.
         */
        virtual std::unique_ptr<ExportedObject> Export(ExportableObjectPtr& exportableObject, ExporterContext& context) {return nullptr;}
    };

} }
