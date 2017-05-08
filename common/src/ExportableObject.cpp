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
#include "ExportableObject.hpp"
#include "ObjectTypeDetector.hpp"
#include "ApplicationAccessor.hpp"
#include "ChildObjectSorter.hpp"
#include "ExporterContext.hpp"

using namespace Finjin::Exporter;


//Local functions---------------------------------------------------------------
static bool ObjectShouldHaveSettings(ObjectAccessor& object)
{
    //This allows all objects, including bones, to have object settings and be exported
    return true;
}


//Implementation----------------------------------------------------------------
ExportableObject::ExportableObject()
{
    this->startingInitialize = false;
    this->resolvingDependencies = false;
}

ExportableObject::~ExportableObject()
{
}

void ExportableObject::Initialize
    (
    ObjectAccessor object,
    ExporterContext& context
    )
{
    //Start initialization
    InitializeStart(0, object, FinjinObjectSettingsAccessor(), context);

    //Some objects will need their parent to be changed
    for (auto changeParentIterator = this->changeParent.begin();
        changeParentIterator != this->changeParent.end();
        ++changeParentIterator)
    {
        auto newParentIterator = context.GetAllObjects().find(changeParentIterator->second);
        if (newParentIterator != context.GetAllObjects().end())
        {
            ExportableObjectPtr newParent = newParentIterator->second;
            newParent->initializedChildObjects.push_back(changeParentIterator->first->GetSharedPtr());

            changeParentIterator->first->RemoveFromParent();
            changeParentIterator->first->parent = newParent.get();
        }
    }
    this->changeParent.clear();

    //Finish initialization
    InitializeEnd(context);
}

void ExportableObject::Initialize
    (
    ExportableObject* parent,
    ObjectAccessor object,
    FinjinObjectSettingsAccessor objectSettings,
    ExporterContext& context
    )
{
    InitializeStart(parent, object, objectSettings, context);
}

void ExportableObject::InitializeStart
    (
    ExportableObject* parent,
    ObjectAccessor object,
    FinjinObjectSettingsAccessor objectSettings,
    ExporterContext& context
    )
{
    //FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("ExportableObject::InitializeStart(%s)"), object.GetLocalName().wx_str());

    this->startingInitialize = true;

    this->exportCount = 0;
    this->root = (parent != nullptr) ? parent->root : this;
    this->parent = parent;
    this->object = object;
    context.EnsureUniqueExportableObjectName(this->object);
    this->objectSettings = objectSettings;
    if (this->objectSettings.IsValid())
    {
        this->objectSettings.TouchReferences();
        this->objectExportSettings.Initialize(this->object, this->objectSettings, context.GetSceneSettings(), *context.GetSceneExportSettings());
    }

    this->detectedObjectType = nullptr;
    if (this->object.IsValid())
    {
        ObjectTypeDetector typeDetector;
        typeDetector.Detect(this->object, this->objectSettings);
        this->detectedObjectType = typeDetector.selectedType;
        if (this->detectedObjectType != nullptr)
            this->detectedObjectType->GetDependencies(this, context);
    }

    //Handle children--------------------------------------
    //Get children
    ObjectAccessorVector childObjects;
    this->object.GetChildren(childObjects);
    if (!childObjects.empty())
    {
        //Prepare to sort children, ignoring any unnecessary children
        std::vector<ChildObjectSorter::ChildObject> sortedChildObjects;
        sortedChildObjects.reserve(childObjects.size());
        for (size_t i = 0; i < childObjects.size(); i++)
        {
            ObjectAccessor childObject = childObjects[i];
            if (!context.HasExportableObject(childObject))
            {
                ChildObjectSorter::ChildObject sortableChildObject;
                FinjinObjectSettingsAccessor childObjectSettings;
                if (ObjectShouldHaveSettings(childObject))
                    childObjectSettings = context.GetSceneSettings().GetObjectSettings(childObject, true);
                sortableChildObject.Set(childObject, childObjectSettings, 0, (int)i);
                sortedChildObjects.push_back(sortableChildObject);
            }
        }

        //Sort children
        if (this->objectSettings.IsValid())
        {
            ChildObjectSorter sorter(this->objectSettings.GetChildSortingValue());
            std::sort(sortedChildObjects.begin(), sortedChildObjects.end(), sorter);
        }

        //Initialize the children, creating the child graph recursively
        for (size_t i = 0; i < sortedChildObjects.size(); i++)
        {
            ExportableObjectPtr childExportableObject(NewInstance());

            childExportableObject->InitializeStart(this, sortedChildObjects[i].object, sortedChildObjects[i].objectSettings, context);
            this->initializedChildObjects.push_back(childExportableObject);

            context.AddExportableObject(childExportableObject->object, childExportableObject);
        }
    }

    //If object has a different exported parent than the one in the current scene,
    //record this object and its new parent so that the hierarchy can be updated later
    if (this->objectSettings.IsValid())
    {
        ObjectAccessor exportedParent;
        if (!this->objectSettings.GetUseRootAsExportedParent())
            exportedParent = this->objectSettings.GetExportedParent();
        if (exportedParent.IsValid() &&
            exportedParent != this->object &&
            exportedParent != this->object.GetParent() &&
            !this->object.HasDescendant(exportedParent))
        {
            this->root->changeParent[this] = exportedParent;
        }
    }

    this->startingInitialize = false;
}

void ExportableObject::InitializeEnd(ExporterContext& context)
{
    //Add the children to their appropriate parent, taking into account
    //that some objects will have been marked as "ignore" during the previous initialization step
    for (auto childExportableObject : this->initializedChildObjects)
    {
        if (!context.HasIgnoreObject(childExportableObject->object))
            AddBestChild(childExportableObject, context);

        childExportableObject->InitializeEnd(context);
    }
    this->initializedChildObjects.clear();
}

ExportableObject* ExportableObject::NewInstance()
{
    return new ExportableObject;
}

TransformAccessor ExportableObject::GetObjectOffsetTransformation(TimeAccessor time)
{
    return this->object.GetObjectOffsetTransformation(time);
}

TransformAccessor ExportableObject::GetConvertedObjectOffsetTransformation(TimeAccessor time, CoordinateSystemConverter& conversionManager, float scale)
{
    TransformAccessor transformation(GetObjectOffsetTransformation(time), conversionManager, scale);
    return transformation;
}

TransformAccessor ExportableObject::GetNodeTransformation(TimeAccessor time)
{
    return this->object.GetNodeTransformation(time);
}

TransformAccessor ExportableObject::GetFullWorldTransformation(TimeAccessor time)
{
    return this->object.GetFullWorldTransformation(time);
}

TransformAccessor ExportableObject::GetLocalNodeTransformation(TimeAccessor time, const TransformAccessor* parentTransform)
{
    auto transformation = GetNodeTransformation(time);

    if (parentTransform != nullptr)
        transformation = transformation.GetRelativeTo(*parentTransform);
    else if (this->parent != nullptr && !this->parent->object.IsRoot())
    {
        auto parentTransformation = this->parent->GetNodeTransformation(time);
        transformation = transformation.GetRelativeTo(parentTransformation);
    }

    return transformation;
}

TransformAccessor ExportableObject::GetConvertedNodeTransformation(TimeAccessor time, CoordinateSystemConverter& conversionManager, float scale, FinjinVector4* objectRotation)
{
    TransformAccessor transformation(GetNodeTransformation(time), conversionManager, scale);

    if (objectRotation != nullptr)
    {
        objectRotation->Set(0, 0, 0, 1);

        if (this->detectedObjectType->GetDescriptor().IsCamera())
            conversionManager.ConvertCameraRotation(*objectRotation);
        else if (this->detectedObjectType->GetDescriptor().IsLight())
            conversionManager.ConvertLightRotation(*objectRotation);
    }

    return transformation;
}

TransformAccessor ExportableObject::GetConvertedLocalNodeTransformation(TimeAccessor time, CoordinateSystemConverter& conversionManager, float scale, FinjinVector4* objectRotation)
{
    auto transformation = GetConvertedNodeTransformation(time, conversionManager, scale, objectRotation);

    if (this->parent != nullptr && !this->parent->object.IsRoot())
    {
        auto parentTransformation = this->parent->GetConvertedNodeTransformation(time, conversionManager, scale);
        transformation = transformation.GetRelativeTo(parentTransformation);
    }

    return transformation;
}

void ExportableObject::ResolveDependencies(ExporterContext& context)
{
    this->resolvingDependencies = true;

    //Resolve dependencies for this object
    for (auto dependentObject : this->dependentObjects)
    {
        ExportableObjectPtr exportableObject = context.FindExportableObject(dependentObject);
        if (exportableObject != nullptr)
            this->dependentObjectMap[dependentObject] = exportableObject;
    }

    //Perform type-specific dependency resolution
    if (this->detectedObjectType != nullptr)
        this->detectedObjectType->ResolveDependencies(this, context);

    //Resolve dependencies for children
    for (auto childObject : this->childObjects)
    {
        childObject->ResolveDependencies(context);
    }

    //Remove objects from their parents
    //Only perform this step if we're at the root node, since by then all dependencies have been resolved
    if (this->root == this)
    {
        for (auto child : this->objectsForRemovalFromParent)
        {
            if (child->parent != nullptr)
                child->parent->RemoveChild(child);
        }
        this->objectsForRemovalFromParent.clear();
    }

    this->resolvingDependencies = false;
}

ExportableObject* ExportableObject::GetBestContainer()
{
    ExportableObject* object = this;

    while (object != object->root && !object->detectedObjectType->GetDescriptor().IsContainer())
        object = object->parent;

    return object;
}

bool ExportableObject::IsConsumedByParent()
{
    if (this->parent != nullptr && this->parent->detectedObjectType != nullptr)
        return this->parent->detectedObjectType->IsConsumedChild(this);
    else
        return false;
}

void ExportableObject::RemoveFromParent()
{
    if (this->resolvingDependencies)
        this->root->objectsForRemovalFromParent.push_back(this);
    else if (this->parent != nullptr)
        this->parent->RemoveChild(this);
}

void ExportableObject::AddBestChild(ExportableObjectPtr child, ExporterContext& context)
{
    auto parent = this;
    auto childObjects = &this->childObjects; //Default to child list

    if (!context.GetSceneSettings().GetExportHierarchy() || child->objectSettings.GetUseRootAsExportedParent())
    {
        //No hierarchy at all
        parent = this->root;
        childObjects = &parent->childObjects;
    }
    else
    {
        if (child->detectedObjectType != nullptr &&
            child->detectedObjectType->GetDescriptor().IsSky())
        {
            //Sky objects are always children of the "best" container, which is usually the root
            parent = GetBestContainer();
            childObjects = &parent->childObjects;
        }
        if (child->detectedObjectType != nullptr &&
            child->detectedObjectType->GetDescriptor().IsMerged())
        {
            //Object is to be merged into another object (typically the parent)
            childObjects = &this->mergedObjects;
        }
        else if (child->objectSettings.IsValid() && child->objectSettings.GetAttachedObject())
        {
            //The object is attached to its parent, not as a child, but as a sibling of its main object
            childObjects = &this->siblingObjects;
        }
    }

    child->parent = parent;
    childObjects->push_back(child);
}

void ExportableObject::AddChild(ExportableObjectPtr child)
{
    if (this->startingInitialize)
        this->initializedChildObjects.push_back(child);
    else
        this->childObjects.push_back(child);
}

void ExportableObject::RemoveChild(ExportableObject* child)
{
    ExportableObjectPtr childPtr = GetChildPtr(child);
    if (childPtr != nullptr)
    {
        this->initializedChildObjects.remove(childPtr);
        this->childObjects.remove(childPtr);
    }
}

void ExportableObject::RemoveChildren()
{
    this->childObjects.clear();
}

ExportableObjectPtr ExportableObject::GetSharedPtr()
{
    ExportableObjectPtr result;

    if (this->parent != nullptr)
        result = this->parent->GetChildPtr(this);

    return result;
}

ExportableObjectPtr ExportableObject::GetChildPtr(ExportableObject* child)
{
    for (auto childObject : this->childObjects)
    {
        if (childObject.get() == child)
            return childObject;
    }

    for (auto childObject : this->initializedChildObjects)
    {
        if (childObject.get() == child)
            return childObject;
    }

    return ExportableObjectPtr();
}

int ExportableObject::GetChildCount(bool recursive) const
{
    int count = 0;

    for (auto childObject : this->childObjects)
    {
        count++;

        if (recursive)
            count += childObject->GetChildCount(true);
    }

    return count;
}

ExportableObjectPtr ExportableObject::GetDependentExportableObject(ObjectAccessor dependentObject)
{
    ExportableObjectPtr exportableObject;

    auto exportableObjectIterator = this->dependentObjectMap.find(dependentObject);
    if (exportableObjectIterator != this->dependentObjectMap.end())
        exportableObject = exportableObjectIterator->second;

    return exportableObject;
}

bool ExportableObject::AddDependency(ObjectAccessor object, const wxString& tag)
{
    if (object.IsValid() && !HasDependency(object))
    {
        this->dependentObjects.push_back(Dependency(object, tag));
        return true;
    }
    else
        return false;
}

bool ExportableObject::HasDependency(ObjectAccessor object) const
{
    return
        std::find(this->dependentObjects.begin(), this->dependentObjects.end(), object) !=
        this->dependentObjects.end();
}

ExportableObject::IterateCallback::Result ExportableObject::IterateChildren(IterateCallback& callback, bool recursive)
{
    for (auto childObject : this->childObjects)
    {
        switch (callback.HandleObject(childObject))
        {
            case IterateCallback::CONTINUE_ITERATION:
            {
                if (recursive && childObject->IterateChildren(callback, true) == IterateCallback::END_ITERATION)
                    return IterateCallback::END_ITERATION;
                break;
            }
            case IterateCallback::SKIP_ITERATION:
            {
                //Do nothing. By doing nothing, we are effectively skipping the object.
                //This is only useful if 'recursive' is true
                break;
            }
            case IterateCallback::END_ITERATION:
            {
                return IterateCallback::END_ITERATION;
            }
        }
    }

    return IterateCallback::CONTINUE_ITERATION;
}
