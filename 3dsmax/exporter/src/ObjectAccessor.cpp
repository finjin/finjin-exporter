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
#include "MaxUtilities.hpp"
#include "MaxMathUtilities.hpp"
#include "ApplicationAccessor.hpp"
#include "FinjinSceneSettingsObject.hpp"
#include <ilayer.h>
/*
#include <ilayermanager.h>
#include <ilayerproperties.h>
*/

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
ObjectAccessor::ObjectAccessor()
{
    this->node = nullptr;
}

ObjectAccessor::ObjectAccessor(INode* node)
{
    this->node = node;
}

ObjectAccessor::ObjectAccessor(const ObjectAccessor& object)
{
    this->node = object.node;
    this->objectName = object.objectName;
    this->nodeName = object.nodeName;
}

ObjectAccessor::operator INode* ()
{
    return this->node;
}

bool ObjectAccessor::operator == (const ObjectAccessor other) const
{
    return this->node == other.node;
}

bool ObjectAccessor::operator != (const ObjectAccessor other) const
{
    return this->node != other.node;
}

bool ObjectAccessor::operator < (const ObjectAccessor other) const
{
    return this->node < other.node;
}

wxString ObjectAccessor::GetLocalName(bool useObjectName) const
{
    const wxString& nameOverride = useObjectName ? this->objectName : this->nodeName;
    if (!nameOverride.empty())
        return nameOverride;
    else
        return (this->node != nullptr) ? ApplicationStringToWxString(this->node->GetName()) : wxEmptyString;
}

wxString ObjectAccessor::GetFullName(bool useObjectName) const
{
    return GetLocalName(useObjectName);
}

ObjectAccessor ObjectAccessor::GetParent()
{
    return this->node->GetParentNode();
}

void ObjectAccessor::GetChildren(ObjectAccessorVector& children)
{
    children.clear();

    int childCount = this->node->NumberOfChildren();
    children.reserve(childCount);
    for (int i = 0; i < childCount; i++)
    {
        INode* child = this->node->GetChildNode(i);
        if (!IsSettingsObject(child))
            children.push_back(child);
    }
}

bool ObjectAccessor::HasDescendant(ObjectAccessor other)
{
    for (INode* parent = other.node->GetParentNode();
        parent != nullptr && !parent->IsRootNode();
        parent = parent->GetParentNode())
    {
        if (parent == this->node)
            return true;
    }
    return false;
}

bool ObjectAccessor::IsRoot() const
{
    return this->node->IsRootNode() ? true : false;
}

bool ObjectAccessor::IsValid() const
{
    return this->node != nullptr;
}

bool ObjectAccessor::IsVisible() const
{
    bool hidden = this->node->IsObjectHidden() ? true : false;

    ILayer* layer = (ILayer*)this->node->GetReference(NODE_LAYER_REF);
    hidden |= layer->IsHidden();

    return !hidden;
}

bool ObjectAccessor::GetCastShadows() const
{
    return this->node->CastShadows() ? true : false;
}

void ObjectAccessor::SetCastShadows(bool value)
{
    this->node->SetCastShadows(value);
}

bool ObjectAccessor::GetReceiveShadows() const
{
    return this->node->RcvShadows() ? true : false;
}

bool ObjectAccessor::IsCamera() const
{
    return this->node != nullptr && this->node->GetObjectRef()->SuperClassID() == CAMERA_CLASS_ID;
}

bool ObjectAccessor::IsLight() const
{
    return this->node != nullptr && this->node->GetObjectRef()->SuperClassID() == LIGHT_CLASS_ID;
}

bool ObjectAccessor::IsAmbientLight() const
{
    return false;
}

FinjinColor ObjectAccessor::GetLightColor(TimeAccessor time)
{
    FinjinColor color;
    if (IsValid())
    {
        LightState lightState;
        LightObject* lightObject = (LightObject*)this->node->EvalWorldState(time.GetNativeTime()).obj;
        lightObject->EvalLightState(time.GetNativeTime(), FOREVER, &lightState);

        color.Set(lightState.color.r, lightState.color.g, lightState.color.b);
    }
    return color;
}

Mtl* ObjectAccessor::GetMaterial()
{
    return IsValid() ? this->node->GetMtl() : nullptr;
}

PrimitiveType ObjectAccessor::GetDefaultPrimitiveType()
{
    auto primitiveType = PrimitiveType::DEFAULT;

    auto object = this->node->EvalWorldState(0).obj;
    if (object != nullptr)
    {
        if (object->SuperClassID() == SHAPE_CLASS_ID)
            primitiveType = PrimitiveType::LINES;
        else if (object->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
            primitiveType = PrimitiveType::TRIANGLES;
    }

    return primitiveType;
}

TransformAccessor ObjectAccessor::GetObjectOffsetTransformation(TimeAccessor time)
{
    Matrix3 transformation(TRUE);

    if (this->node != nullptr)
        transformation = MaxMathUtilities::GetObjectOffsetTM(this->node, time.GetNativeTime());

    return transformation;
}

TransformAccessor ObjectAccessor::GetNodeTransformation(TimeAccessor time)
{
    Matrix3 transformation(TRUE);

    if (this->node != nullptr && !this->node->IsRootNode())
        transformation = this->node->GetNodeTM(time.GetNativeTime());

    return transformation;
}

TransformAccessor ObjectAccessor::GetFullWorldTransformation(TimeAccessor time)
{
    Matrix3 transformation(TRUE);

    if (this->node != nullptr && !this->node->IsRootNode())
    {
        transformation = this->node->GetObjectTM(time.GetNativeTime());
    }

    return transformation;
}

void ObjectAccessor::GetNoteTracks(std::vector<FinjinNoteTrack>& noteTracks)
{
    //Export note tracks
    int noteTrackCount = this->node->NumNoteTracks();
    noteTracks.resize(noteTrackCount);
    for (int trackIndex = 0; trackIndex < noteTrackCount; trackIndex++)
    {
        //Get the note track
        DefNoteTrack* noteTrack = (DefNoteTrack*)this->node->GetNoteTrack(trackIndex);

        //Set the note track name
        noteTracks[trackIndex].name = wxString::Format(wxT("%d"), trackIndex);

        //Export notes for the track
        noteTracks[trackIndex].keys.resize(noteTrack->keys.Count());
        for (int keyIndex = 0; keyIndex < noteTrack->keys.Count(); keyIndex++)
        {
            TimeAccessor t;
            t.SetNativeTime(noteTrack->keys[keyIndex]->time);

            noteTracks[trackIndex].keys[keyIndex].time = t.GetDuration();
            noteTracks[trackIndex].keys[keyIndex].text = ApplicationStringToWxString(noteTrack->keys[keyIndex]->note);
        }
    }
}

ObjectAccessor ObjectAccessor::GetLookAtObject()
{
    return MaxUtilities::GetLookAtNode(this->node);
}

ObjectAccessor ObjectAccessor::GetRootObject()
{
    return GetCOREInterface()->GetRootNode();
}

ObjectAccessor ObjectAccessor::GetObjectByName(const wxString& name)
{
    MSTR maxName = WxStringToApplicationString(name);
    return GetCOREInterface()->GetINodeByName(maxName);
}

void ObjectAccessor::GetSelectedObjects(ObjectAccessorVector& selectedObjects)
{
    selectedObjects.clear();

    int count = GetCOREInterface()->GetSelNodeCount();
    selectedObjects.reserve(count);
    for (int i = 0; i < count; i++)
    {
        INode* node = GetCOREInterface()->GetSelNode(i);
        if (!IsSettingsObject(node))
            selectedObjects.push_back(node);
    }
}

void ObjectAccessor::SetSelectedObjects(const ObjectAccessorVector& selectedObjects)
{
    if (!selectedObjects.empty())
    {
        for (size_t i = 0; i < selectedObjects.size(); i++)
            GetCOREInterface()->SelectNode(selectedObjects[i].node, i == 0);

        if (selectedObjects.size() > 1)
            GetCOREInterface()->RedrawViews(GetCOREInterface()->GetTime());
    }
    else
        ClearSelectedObjects();
}

void ObjectAccessor::ClearSelectedObjects()
{
    GetCOREInterface()->ClearNodeSelection();
}

bool ObjectAccessor::IsBone(ObjectAccessor object)
{
    return MaxUtilities::IsAnyBone(object.node);
}

bool ObjectAccessor::IsSettingsObject(ObjectAccessor object)
{
    if (object.node->IsRootNode())
        return false;
    else
    {
        Class_ID objectClassID = object.node->GetObjectRef()->ClassID();
        Control* control = object.node->GetTMController();

        return
            objectClassID == FinjinSceneSettingsObject::GetClassClassID() ||
            objectClassID == ParticleView_Class_ID ||
            objectClassID == FOOTPRINT_CLASS_ID ||
            objectClassID == Class_ID(1738878752, 783031169) || //CAT platform
            objectClassID == Class_ID(1205540079, 1318803856) || //Slider helper
            (control != nullptr && control->ClassID() == FOOTPRINT_CLASS_ID);
    }
}
