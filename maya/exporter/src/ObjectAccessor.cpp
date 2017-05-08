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
#include "ObjectAccessor.hpp"
#include "MayaUtilities.hpp"
#include "TimeChanger.hpp"

using namespace Finjin::Exporter;

//Disabled since people find it confusing when the node and object have a different name
//It's also tedious having to name both in Maya
#define USE_MAYA_OBJECT_NAME 0


//Local functions---------------------------------------------------------------
static bool IsIntermediate(MFnDagNode& dagNode)
{
    return dagNode.isIntermediateObject() || dagNode.inUnderWorld();
}

static bool IsTraversableObjectType(MFn::Type type)
{
    switch (type)
    {
        case MFn::kNurbsPlane: return true;
        case MFn::kCamera: return true;
        case MFn::kDummy: return true;
        case MFn::kNurbsCurve: return true;
        case MFn::kPlane: return true;
        case MFn::kNurbsSurface: return true;
        case MFn::kMesh: return true;
        case MFn::kLight: return true;
        case MFn::kNonAmbientLight: return true;
        case MFn::kAreaLight: return true;
        case MFn::kLinearLight: return true;
        case MFn::kDirectionalLight: return true;
        case MFn::kPointLight: return true;
        case MFn::kSpotLight: return true;
        case MFn::kPluginDependNode: return true;
        case MFn::kPluginLocatorNode: return true;
        case MFn::kLocator: return true;
        default: return false;
    }
}

static bool IsTraversableNodeType(MFn::Type type)
{
    switch (type)
    {
        case MFn::kWorld: return true;
        case MFn::kTransform: return true;
        case MFn::kJoint: return true;
        case MFn::kLookAt: return true;
        default: return false;
    }
}

/*static MObject GetTraversableInput(MFnDagNode& dagNode)
{
    const char* attributeNames[] = {"inMesh", "create"};
    for (int nameIndex = 0; nameIndex < FINJIN_COUNT_OF(attributeNames); nameIndex++)
    {
        MObject inMeshAttribute = dagNode.attribute(attributeNames[nameIndex]);
        if (!inMeshAttribute.isNull())
        {
            MPlug inMesh(dagNode.object(), inMeshAttribute);
            MPlugArray inputs;
            inMesh.connectedTo(inputs, true, false);
            for (unsigned int i = 0; i < inputs.length(); i++)
            {
                MObject inputObject = inputs[i].node();
                MFn::Type type = inputObject.apiType();
                if (IsTraversableObjectType(type))
                {
                    MFnDagNode inputObjectDagNode(inputObject);
                    if (!IsIntermediate(inputObjectDagNode))
                        return inputObject;
                }
            }
        }
    }

    return MObject();
}*/

bool ObjectAccessor::IsInVisibleLayer() const
{
    //Get all the display layer names
    MStringArray displayLayers;
    MGlobal::executeCommand("string $allDisplayLayers[] = `layout -query -childArray LayerEditorDisplayLayerLayout`;", displayLayers, DISPLAY_MAYA_COMMAND);
    if (displayLayers.length() > 0)
    {
        //Iterate on the object and its ancestors looking for visibility info, stopping at the root
        for (ObjectAccessor object(*this); !object.IsRoot(); object = object.GetParent())
        {
            MString fullNodeName = WxStringToApplicationString(object.GetFullName(false));

            //For each display layer name...
            MStringArray members;
            for (unsigned int displayLayerIndex = 0; displayLayerIndex < displayLayers.length(); displayLayerIndex++)
            {
                //Get all the members in the layer...
                MGlobal::executeCommand("editDisplayLayerMembers -fullNames -q " + displayLayers[displayLayerIndex], members);
                for (unsigned int memberIndex = 0; memberIndex < members.length(); memberIndex++)
                {
                    //If there is a member with a name matching the object name, return the visibility of the layer
                    if (members[memberIndex] == fullNodeName)
                    {
                        int isVisible = 1;
                        MGlobal::executeCommand("getAttr " + displayLayers[displayLayerIndex] + ".visibility", isVisible, DISPLAY_MAYA_COMMAND);
                        return isVisible ? true : false;
                    }
                }
            }
        }
    }

    //Object could not be found in a layer. Assume it's visible
    return true;
}


//Implementation----------------------------------------------------------------
ObjectAccessor::ObjectAccessor()
{
}

ObjectAccessor::ObjectAccessor(MObject obj)
{
    this->obj = obj;
    this->container = obj;

    //FINJIN_EXPORTER_METHOD_ENTRY_FORMAT("ObjectAccessor::ObjectAccessor(%s)", GetLocalName().wx_str());

    MStatus status;
    MFnDagNode dagNode(obj, &status);
    if (this->obj.hasFn(MFn::kTransform))
    {
        //The object has a transform.
        if (!this->obj.hasFn(MFn::kJoint) && status == MStatus::kSuccess)
        {
            unsigned int childCount = dagNode.childCount();
            if (childCount > 0)
            {
                //See if a child without a transform can be found
                for (unsigned int i = 0; i < childCount; i++)
                {
                    MObject child = dagNode.child(i);
                    if (IsTraversableAsObject(child))
                    {
                        //Child doesn't have a transform. Probably a mesh, camera, light, etc
                        //It's the real object we want to consider
                        this->obj = child;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        //The object doesn't have a transform.
        //See if a parent with a transform can be found
        if (status == MStatus::kSuccess && dagNode.parentCount() > 0)
        {
            MObject parent = dagNode.parent(0);
            if (parent.hasFn(MFn::kTransform))
                this->container = parent;
        }
    }

    /*
    if (this->obj == this->container)
    {
        MObject betterObject = GetTraversableInput(dagNode);
        if (!betterObject.isNull())
        {
            this->obj = betterObject;
            //FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, "Has better object: %s (%s)", GetLocalName().wx_str(), this->obj.apiTypeStr());
        }
    }
    */
}

ObjectAccessor::ObjectAccessor(const ObjectAccessor& object)
{
    this->obj = object.obj;
    this->container = object.container;
    this->objectName = object.objectName;
    this->nodeName = object.nodeName;
}

ObjectAccessor::operator MObject ()
{
    return this->obj;
}

bool ObjectAccessor::operator == (const ObjectAccessor& other) const
{
    return this->container == other.container;
}

bool ObjectAccessor::operator != (const ObjectAccessor& other) const
{
    return this->container != other.container;
}

bool ObjectAccessor::operator < (const ObjectAccessor& other) const
{
    return GetFullName() < other.GetFullName();
}

wxString ObjectAccessor::GetLocalName(bool useObjectName) const
{
#if !USE_MAYA_OBJECT_NAME
    useObjectName = false;
#endif

    const wxString& nameOverride = useObjectName ? this->objectName : this->nodeName;
    if (!nameOverride.empty())
        return nameOverride;
    else
    {
        MObject obj = useObjectName ? this->obj : this->container;
        MString name;
        if (obj.hasFn(MFn::kDependencyNode))
        {
            MFnDependencyNode depNode(obj);
            name = depNode.name();
        }

        return ApplicationStringToWxString(name);
    }
}

wxString ObjectAccessor::GetFullName(bool useObjectName) const
{
#if !USE_MAYA_OBJECT_NAME
    useObjectName = false;
#endif

    MObject obj = useObjectName ? this->obj : this->container;
    MString name;
    if (obj.hasFn(MFn::kDagNode))
    {
        MFnDagNode dagNode(obj);
        name = dagNode.fullPathName();
    }
    else if (obj.hasFn(MFn::kDependencyNode))
    {
        MFnDependencyNode depNode(obj);
        name = depNode.name();
    }

    return ApplicationStringToWxString(name);
}

ObjectAccessor ObjectAccessor::GetParent()
{
    return MayaUtilities::GetParent(this->container);
}

void ObjectAccessor::GetChildren(ObjectAccessorVector& children)
{
    children.clear();

    if (this->container.hasFn(MFn::kDagNode))
    {
        MFnDagNode dagNode(this->container);
        unsigned int childCount = dagNode.childCount();
        children.reserve(childCount);

        for (unsigned int i = 0; i < childCount; i++)
        {
            MObject child = dagNode.child(i);
            MFnDependencyNode depNode(child);
            bool traversable = IsTraversableAsNode(child);
            if (traversable)
                children.push_back(child);
        }
    }
}

bool ObjectAccessor::HasDescendant(ObjectAccessor other)
{
    for (MObject parent = MayaUtilities::GetParent(other.obj);
        !parent.isNull() && !MayaUtilities::IsRoot(parent);
        parent = MayaUtilities::GetParent(parent))
    {
        if (parent == this->obj)
            return true;
    }
    return false;
}

bool ObjectAccessor::IsValid() const
{
    return !this->obj.isNull();
}

bool ObjectAccessor::IsRoot() const
{
    return MayaUtilities::IsRoot(this->obj);
}

bool ObjectAccessor::IsVisible() const
{
    if (MayaPlug::GetBoolAttribute(this->container, "visibility", true) &&
        MayaPlug::GetBoolAttribute(this->obj, "visibility", true))
    {
        return IsInVisibleLayer();
    }
    else
        return false;
}

bool ObjectAccessor::GetCastShadows() const
{
    return MayaPlug::GetBoolAttribute(this->obj, "castsShadows", true);
}

void ObjectAccessor::SetCastShadows(bool value)
{
    MayaPlug::SetBoolAttribute(this->obj, "castsShadows", value);
}

bool ObjectAccessor::GetReceiveShadows() const
{
    return MayaPlug::GetBoolAttribute(this->obj, "receiveShadows", true);
}

bool ObjectAccessor::IsCamera() const
{
    return this->obj.hasFn(MFn::kCamera);
}

bool ObjectAccessor::IsLight() const
{
    return this->obj.hasFn(MFn::kLight) && !this->obj.hasFn(MFn::kAmbientLight);
}

bool ObjectAccessor::IsAmbientLight() const
{
    return this->obj.hasFn(MFn::kAmbientLight);
}

FinjinColor ObjectAccessor::GetLightColor(TimeAccessor time)
{
    TimeChanger timeChanger(time);
    MFnLight light(this->obj);
    MColor color = light.color();
    return FinjinColor(color.r, color.g, color.b, color.a);
}

MObject ObjectAccessor::GetMaterial()
{
    MObject material;

    if (this->obj.hasFn(MFn::kMesh))
    {
        MFnMesh mesh(this->obj);

        MObjectArray shaders;
        MIntArray indices;
        mesh.getConnectedShaders(0, shaders, indices);
        if (shaders.length() > 0)
            material = MayaUtilities::GetBestMaterial(shaders[0]);
    }

    return material;
}

PrimitiveType ObjectAccessor::GetDefaultPrimitiveType()
{
    MStatus status;
    MFnDagNode dagNode(this->obj, &status);
    if (status != MS::kSuccess || IsIntermediate(dagNode))
        return PrimitiveType::DEFAULT;
    else if (this->obj.hasFn(MFn::kMesh) || this->obj.hasFn(MFn::kNurbsSurface))
        return PrimitiveType::TRIANGLES;
    else if (this->obj.hasFn(MFn::kNurbsCurve))
        return PrimitiveType::LINES;
    else
        return PrimitiveType::DEFAULT;
}

TransformAccessor ObjectAccessor::GetObjectOffsetTransformation(TimeAccessor time)
{
    TransformAccessor transformation;

    if (this->container.hasFn(MFn::kTransform))
    {
        MVector pivot = MayaUtilities::GetPivotPoint(this->container, time);
        transformation.SetTranslation(FinjinVector3(-pivot.x, -pivot.y, -pivot.z));
    }

    return transformation;
}

TransformAccessor ObjectAccessor::GetNodeTransformation(TimeAccessor time)
{
    TimeChanger timeChanger(time);

    TransformAccessor result;
    if (IsValid() && !IsRoot())
    {
        MMatrix matrix;
        MayaUtilities::GetFullWorldMatrix(this->container, matrix);

        MTransformationMatrix transformation(matrix);
        result = transformation;
    }

    return result;
}

TransformAccessor ObjectAccessor::GetFullWorldTransformation(TimeAccessor time)
{
    TimeChanger timeChanger(time);

    TransformAccessor result;
    if (IsValid() && !IsRoot())
    {
        MMatrix matrix;
        MayaUtilities::GetFullWorldMatrix(this->container, matrix);

        MTransformationMatrix transformation(matrix);
        result = transformation;
    }

    return result;
}

void ObjectAccessor::GetNoteTracks(std::vector<FinjinNoteTrack>& noteTracks)
{
}

ObjectAccessor ObjectAccessor::GetLookAtObject()
{
    return ObjectAccessor();
}

ObjectAccessor ObjectAccessor::GetRootObject()
{
    MObject root;

    MItDag dagIterator(MItDag::kBreadthFirst, MFn::kInvalid);
    for (; !dagIterator.isDone(); dagIterator.next())
    {
        if (dagIterator.depth() == 0)
        {
            root = dagIterator.currentItem();
            break;
        }
    }

    return root;
}

ObjectAccessor ObjectAccessor::GetObjectByName(const wxString& name)
{
    MObject object;

    if (!name.empty())
    {
        MSelectionList selection;
        selection.add(WxStringToApplicationString(name));

        MDagPath dagPath;
        if (selection.getDagPath(0, dagPath) == MStatus::kSuccess)
            object = dagPath.node();
        else
            selection.getDependNode(0, object);
    }

    return object;
}

void ObjectAccessor::GetSelectedObjects(ObjectAccessorVector& selectedObjects)
{
    selectedObjects.clear();

    MSelectionList selection;
    MGlobal::getActiveSelectionList(selection);
    selectedObjects.reserve(selection.length());
    for (unsigned int i = 0; i < selection.length(); i++)
    {
        MObject object;
        selection.getDependNode(i, object);
        if (IsTraversableAsNode(object) && !MayaUtilities::IsRoot(object))
            selectedObjects.push_back(object);
    }
}

void ObjectAccessor::SetSelectedObjects(const ObjectAccessorVector& selectedObjects)
{
    //Selects the objects in the way that Finjin expects
    MSelectionList selection;
    for (size_t i = 0; i < selectedObjects.size(); i++)
        selection.add(selectedObjects[i].container, true);
    MGlobal::setActiveSelectionList(selection);

    //This is necessary to select the objects in the Maya viewports
    MString name;
    for (size_t i = 0; i < selectedObjects.size(); i++)
    {
        name = WxStringToApplicationString(selectedObjects[i].GetFullName());
        MGlobal::selectByName(name, MGlobal::kAddToList);
    }
}

void ObjectAccessor::ClearSelectedObjects()
{
    MSelectionList selection;
    MGlobal::setActiveSelectionList(selection);
}

bool ObjectAccessor::IsBone(ObjectAccessor object)
{
    return object.obj.hasFn(MFn::kJoint);
}

bool ObjectAccessor::IsSettingsObject(ObjectAccessor object)
{
    return MayaUtilities::IsStandardViewportCamera(object);
}

bool ObjectAccessor::IsTraversableAsObject(MObject& object)
{
    if (IsSettingsObject(object))
        return false;

    //Make sure object is a non-intermediate DAG node
    MStatus status;
    MFnDagNode dagNode(object, &status);
    if (status != MStatus::kSuccess || IsIntermediate(dagNode))
        return false;

    return IsTraversableObjectType(object.apiType());
}

bool ObjectAccessor::IsTraversableAsNode(MObject& object)
{
    if (IsSettingsObject(object) || !IsTraversableNodeType(object.apiType()))
        return false;

    //Make sure object is a non-intermediate DAG node
    MStatus status;
    MFnDagNode dagNode(object, &status);
    if (status != MStatus::kSuccess || IsIntermediate(dagNode))
        return false;

    unsigned int childCount = dagNode.childCount();
    if (childCount > 0)
    {
        for (unsigned int i = 0; i < childCount; i++)
        {
            MObject child = dagNode.child(i);
            if (IsTraversableAsObject(child) || IsTraversableAsNode(child))
                return true;
        }

        return false;
    }
    else
        return true;
}
