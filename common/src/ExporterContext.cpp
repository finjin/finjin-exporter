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
#include "ExporterContext.hpp"
#include "ExportableObject.hpp"
#include "ObjectAccessor.hpp"
#include "SceneExportSettings.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "MeshSkeleton.hpp"
#include "MeshMorpher.hpp"
#include "GeometryState.hpp"
#include "DetectableObjectType.hpp"
#include "ApplicationAccessor.hpp"

using namespace Finjin::Exporter;


//Local classes----------------------------------------------------------------

/** Aids in the construction of an exportable hierarchy for the selected scene objects. */
class SelectedHierarchy
{
    class Node
    {
    public:
        Node(ExportableObject* exportableObject, bool isSelected = false)
        {            
            this->exportableObject = exportableObject;
            this->isSelected = isSelected;
        }

        Node& AddChild(ExportableObject* exportableObject, bool isSelected = false)
        {
            for (auto& node : this->children)
            {
                if (node.exportableObject == exportableObject)
                {
                    node.isSelected |= isSelected;
                    return node;
                }
            }

            this->children.push_back(Node(exportableObject, isSelected));

            return this->children.back();
        }

        void Finalize()
        {
            std::list<Node> topNodes;
            GetTopSelectedNodes(topNodes);

            std::list<ExportableObjectPtr> newChildObjects;
            for (auto& node : topNodes)
                node.FinalizeNode(newChildObjects, this->exportableObject);
            this->exportableObject->childObjects = newChildObjects;
        }

    private:
        void GetTopSelectedNodes(std::list<Node>& nodes)
        {
            for (auto& node : this->children)
            {
                if (node.isSelected)
                    nodes.push_back(node);
                else
                    node.GetTopSelectedNodes(nodes);
            }
        }

        void FinalizeNode(std::list<ExportableObjectPtr>& objects, ExportableObject* newParent = nullptr)
        {
            //Set new parent if necessary
            if (newParent != nullptr)
            {
                //The retrieval of the shared pointer will fail when object has previously been removed from its
                //parent, which typically occurs for objects that are NEVER exported, such as bones that are part of a mesh
                ExportableObjectPtr obj = this->exportableObject->GetSharedPtr();
                if (obj != nullptr)
                    objects.push_back(obj);
                this->exportableObject->parent = newParent;                
            }
            
            //Remove child objects should not be exported
            for (auto i = this->exportableObject->childObjects.begin(); i != this->exportableObject->childObjects.end();)
            {
                ExportableObject* childObject = i->get();
                if (!IsChildExportable(childObject))
                    i = this->exportableObject->childObjects.erase(i);
                else
                    ++i;
            }
            
            //Finalize children recursively
            for (auto& node : this->children)
                node.FinalizeNode(objects);
        }

        bool IsChildExportable(ExportableObject* exportableObject)
        {
            for (auto& node : this->children)
            {
                if (node.exportableObject == exportableObject)
                {
                    //Note:
                    //-To have the exporter export all objects extended to the selected descendants, return true
                    //-To have the exporter export only the selected objects connected to the top-most ancestor, return i->isSelected
                    return true;
                }
            }
            return false;
        }

    private:
        std::list<Node> children;
        bool isSelected;

    public:
        ExportableObject* exportableObject;
    };

public:
    SelectedHierarchy(ExporterContext& context, ExportableObject* rootExportableObject) : 
        exporterContext(context), 
        root(rootExportableObject)
    {
    }

    void Add(ExportableObject* exportableObject)
    {
        GetParent(exportableObject).AddChild(exportableObject, true);
    }

    void Finalize()
    {
        this->root.Finalize();        
    }

private:
    Node& GetParent(ExportableObject* exportableObject)
    {
        if (exportableObject->parent == this->root.exportableObject)
            return this->root;
        else
            return GetParent(exportableObject->parent).AddChild(exportableObject->parent);
    }

private:
    ExporterContext& exporterContext;
    Node root;
};


//Implementation---------------------------------------------------------------
ExporterContext::ExporterContext(ExporterHandler* handler, FinjinSceneSettingsAccessor sceneSettings, SceneExportSettings* sceneExportSettings)
{
    this->handler = handler;    
    this->sceneSettings = sceneSettings;
    this->sceneSettings.TouchReferences();
    this->sceneExportSettings = sceneExportSettings;    
}

ExporterContext::~ExporterContext()
{    
}

ExportableObjectMap& ExporterContext::GetAllObjects()
{
    return this->allObjects;
}

void ExporterContext::EnsureUniqueExportableObjectName(ObjectAccessor& object)
{
    //Object name
    wxString objectName = object.GetLocalName(true);
    if (this->objectNames.find(objectName) != this->objectNames.end())
    {
        wxString newObjectName;
        for (int i = 1; ; i++)
        {
            newObjectName = objectName;
            newObjectName += wxT("_");
            newObjectName << i;
            if (this->objectNames.find(newObjectName) == this->objectNames.end())
            {
                objectName = newObjectName;
                object.objectName = newObjectName;
                break;
            }
        }
    }
    this->objectNames.insert(objectName);

    //Node name
    wxString nodeName = object.GetLocalName(false);
    if (this->nodeNames.find(nodeName) != this->nodeNames.end())
    {
        wxString newNodeName;
        for (int i = 1; ; i++)
        {
            newNodeName = nodeName;
            newNodeName += wxT("_");
            newNodeName << i;
            if (this->nodeNames.find(newNodeName) == this->nodeNames.end())
            {
                nodeName = newNodeName;
                object.nodeName = newNodeName;
                break;
            }
        }
    }
    this->nodeNames.insert(nodeName);
}

void ExporterContext::AddExportableObject(ObjectAccessor object, ExportableObjectPtr exportableObject)
{
    this->allObjects[object] = exportableObject;    
}

ExportableObjectPtr ExporterContext::FindExportableObject(ObjectAccessor object)
{
    ExportableObjectPtr result;
    auto foundIterator = this->allObjects.find(object);    
    if (foundIterator != this->allObjects.end())
        result = foundIterator->second;
    return result;
}

bool ExporterContext::HasExportableObject(ObjectAccessor object) const
{
    return this->allObjects.find(object) != this->allObjects.end();
}

void ExporterContext::AddIgnoreObject(ObjectAccessor object)
{
    this->ignoreObjects.insert(object);
}

bool ExporterContext::HasIgnoreObject(ObjectAccessor object) const
{
    return this->ignoreObjects.find(object) != this->ignoreObjects.end();
}

bool ExporterContext::ShouldIgnoreObject(ExportableObject* exportableObject, bool rebuildingObjectGraph)
{
    return false;
}

bool ExporterContext::ShouldIgnoreChildren(ExportableObject* exportableObject)
{
    return false;
}

void ExporterContext::AddMeshDependencies(ExportableObject* exportableObject)
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("ExporterContext::AddMeshDependencies(%s)"), exportableObject->object.GetLocalName().wx_str());

    //Bones
    FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Adding bones"));
    MeshSkeleton skeleton(nullptr);
    CoordinateSystemConverter nullConverter(ApplicationAccessor::GetUpAxis());
    skeleton.Initialize(exportableObject->object, nullConverter, 1, 0);
    for (auto& bone : skeleton.bones)
    {
        exportableObject->AddDependency(bone->object, wxT("bone"));
        AddIgnoreObject(bone->object);
    }
    InsertAll(this->ignoreObjects, skeleton.auxiliaryBoneObjects);

    //Morph targets
    FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Adding morph targets"));
    MeshMorpher morpher(nullptr);
    morpher.Initialize(exportableObject->object, nullConverter, 1, exportableObject->objectSettings.GetPrimitiveTypeValue());
    for (auto& target : morpher.morphTargets)
    {
        if (target->object.IsValid())
            AddIgnoreObject(target->object);
    }

    //Manual LODs
    FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Adding manual LODs"));
    for (size_t i = 0; i < exportableObject->objectExportSettings.manualLods.size(); i++)
    {
        auto& object = exportableObject->objectExportSettings.manualLods[i].object;    
        exportableObject->AddDependency(object, wxT("manualLod"));
    }
}

void ExporterContext::ResolveMeshDependencies(ExportableObject* exportableObject)
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("ExporterContext::ResolveMeshDependencies(%s)"), exportableObject->object.GetLocalName().wx_str());

    for (size_t i = 0; i < exportableObject->objectExportSettings.manualLods.size(); i++)
    {
        auto& object = exportableObject->objectExportSettings.manualLods[i].object;
        ExportableObjectPtr manualLodPtr = exportableObject->dependentObjectMap[object];
        manualLodPtr->RemoveFromParent();
    }        
}

void ExporterContext::GetBoneAttachments(AllBonesWithAttachments& allBonesWithAttachments, ExportableObject* exportableObject, GeometryState* geometryState)
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("ExporterContext::GetBoneAttachments(%s)"), exportableObject->object.GetLocalName().wx_str());

    allBonesWithAttachments.clear();
    allBonesWithAttachments.reserve(geometryState->meshSkeleton.bones.size());

    //Get skeleton's attached objects
    for (auto& meshBone : geometryState->meshSkeleton.bones)
    {
        BoneAndAttachments boneAndAttachments;
        boneAndAttachments.bone = FindExportableObject(meshBone->object);
        if (boneAndAttachments.bone != nullptr)
        {        
            ObjectAccessorVector attachedObjects;
            geometryState->meshSkeleton.GetBoneAttachedObjects(meshBone.get(), attachedObjects);
            for (size_t attachedObjectIndex = 0; attachedObjectIndex < attachedObjects.size(); attachedObjectIndex++)
            {
                if (!HasIgnoreObject(attachedObjects[attachedObjectIndex]))
                {
                    ExportableObjectPtr exportableObject = FindExportableObject(attachedObjects[attachedObjectIndex]);
                    if (exportableObject != nullptr)
                    {
                        if (exportableObject->detectedObjectType->GetDescriptor().IsMovableObject())
                            boneAndAttachments.attachedObjects.push_back(exportableObject);
                        else
                            FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Found exportable attached object %s but it is not a movable"), attachedObjects[attachedObjectIndex].GetLocalName().wx_str());
                    }
                    else
                        FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Could not find exportable attached object %s"), attachedObjects[attachedObjectIndex].GetLocalName().wx_str());
                }
            }

            if (!boneAndAttachments.attachedObjects.empty())
                allBonesWithAttachments.push_back(boneAndAttachments);
        }
        else
            FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Could not find exportable bone %s"), meshBone->object.GetLocalName().wx_str());
    }
}

void ExporterContext::ReplaceChildrenWithSelectedObjects(ExportableObject* exportableObject)
{
    //Get the selected objects
    ObjectAccessorVector selectedObjects;
    ObjectAccessor::GetSelectedObjects(selectedObjects);

    //Build the hierarchy of selected objects
    SelectedHierarchy selectedHierarchy(*this, exportableObject);
    for (size_t i = 0; i < selectedObjects.size(); i++)
    {
        ExportableObjectMap::iterator foundObject = this->allObjects.find(selectedObjects[i]);
        if (foundObject != allObjects.end() && !ShouldIgnoreObject(foundObject->second.get(), true))
        {            
            selectedHierarchy.Add(foundObject->second.get());
        }
    }

    selectedHierarchy.Finalize();
}
