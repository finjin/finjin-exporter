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
#include "MayaUtilities.hpp"
#include "ApplicationAccessor.hpp"
#include "TimeChanger.hpp"
#include "StringSplitter.hpp"
#include "TransformAccessor.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
void MayaUtilities::GetLocalMeshIndices
    (
    MIntArray& localIndex,
    const MIntArray& faceIndices,
    const MIntArray& triangleIndices
    )
{
    localIndex.clear();

    for (unsigned int triangleIndex = 0; triangleIndex < triangleIndices.length(); triangleIndex++)
    {
        int value = -1;

        for (unsigned int faceIndex = 0; faceIndex < faceIndices.length(); faceIndex++)
        {
            if (triangleIndices[triangleIndex] == faceIndices[faceIndex])
            {
                value = faceIndex;
                break;
            }
        }

        localIndex.append(value);
    }
}

void MayaUtilities::DeleteObject(MObject& obj)
{
    ObjectAccessor object(obj);
    auto name = object.GetFullName();
    DeleteObject(WxStringToApplicationString(name));
}

void MayaUtilities::DeleteObject(MFnDagNode& dagNode)
{
    DeleteObject(dagNode.fullPathName());
}

void MayaUtilities::DeleteObject(const MString& name)
{
    MGlobal::executeCommand(MString("delete \"") + name + MString("\""), DISPLAY_MAYA_COMMAND);
}

bool MayaUtilities::IsStandardViewportCamera(MObject obj)
{
    if (obj.hasFn(MFn::kCamera))
    {
        MString name = MFnDagNode(obj).name();
        return
            name == "perspShape" ||
            name == "sideShape" ||
            name == "topShape" ||
            name == "rightShape" ||
            name == "bottomShape" ||
            name == "leftShape" ||
            name == "frontShape" ||
            name == "backShape";
    }
    else
        return false;
}

bool MayaUtilities::IsRoot(MObject obj)
{
    MStatus status;
    MFnDagNode dagNode(obj, &status);
    if (status != MS::kSuccess || dagNode.isIntermediateObject())
        return false;

    return dagNode.parentCount() == 0;
}

MObject MayaUtilities::GetParent(MObject obj)
{
    MObject parent;

    //Try to get the first non-intermediate parent transform
    MFnDagNode dagNode(obj);
    unsigned int parentCount = dagNode.parentCount();
    for (unsigned int i = 0; i < parentCount; i++)
    {
        MObject maybeParent = dagNode.parent(i);
        MFnDagNode maybeParentDagNode(parent);
        if (maybeParent.hasFn(MFn::kTransform) && !maybeParentDagNode.isIntermediateObject())
        {
            parent = maybeParent;
            break;
        }
    }

    if (parentCount > 0)
        parent = dagNode.parent(0);

    return parent;
}

MObject MayaUtilities::GetObjectByName(const MString& name)
{
    MSelectionList selection;
    selection.add(name);

    MObject object;
    MDagPath dagPath;
    if (selection.getDagPath(0, dagPath) == MStatus::kSuccess)
        object = dagPath.node();
    else
        selection.getDependNode(0, object);
    return object;
}

bool MayaUtilities::IsEmptyMeshOrCurve(MObject obj)
{
    MObject meshObject;
    MFnMesh mesh;
    MFnNurbsCurve nurbsCurve;
    MFnDagNode tesselatedMeshTransformNode;
    if (obj.hasFn(MFn::kNurbsSurface))
    {
        MFnNurbsSurface nurbsSurface(obj);
        MObject tesselatedMeshTransform = nurbsSurface.tesselate();
        tesselatedMeshTransformNode.setObject(tesselatedMeshTransform);
        meshObject = tesselatedMeshTransformNode.child(0);
    }
    else if (obj.hasFn(MFn::kMesh))
        meshObject = obj;
    else if (obj.hasFn(MFn::kNurbsCurve))
        nurbsCurve.setObject(obj);

    auto isEmpty = true;
    if (!meshObject.isNull())
    {
        mesh.setObject(meshObject);
        isEmpty = mesh.numVertices() == 0 || mesh.numPolygons() == 0;
    }
    else if (!nurbsCurve.object().isNull())
    {
        MPointArray controlPoints;
        nurbsCurve.getCVs(controlPoints);
        isEmpty = controlPoints.length() == 0;
    }

    //Delete the tesselated mesh if necessary
    if (!tesselatedMeshTransformNode.object().isNull())
        DeleteObject(tesselatedMeshTransformNode);

    return isEmpty;
}

bool MayaUtilities::IsPlane(MObject obj, std::vector<MPlug>* axisPlugs)
{
    MFnDagNode dagNode(obj);

    //See if it's a nurbs plane
    MObject createAttribute = dagNode.attribute("create");
    if (!createAttribute.isNull())
    {
        MPlug inMesh(obj, createAttribute);
        MPlugArray inputs;
        inMesh.connectedTo(inputs, true, false);
        for (unsigned int i = 0; i < inputs.length(); i++)
        {
            MObject planeObject = inputs[i].node();
            MFn::Type type = planeObject.apiType();
            if (type == MFn::kNurbsPlane)
            {
                if (axisPlugs != nullptr)
                {
                    MFnDependencyNode depNode(planeObject);
                    axisPlugs->resize(3);
                    (*axisPlugs)[0] = depNode.findPlug("axisX");
                    (*axisPlugs)[1] = depNode.findPlug("axisY");
                    (*axisPlugs)[2] = depNode.findPlug("axisZ");
                }
                return true;
            }
        }
    }

    //See if it's a polygon plane
    //Make sure it's not a cube because the way we're testing for polygon planes passes for cubes
    MString fullName = dagNode.fullPathName();
    if (fullName.length() > 0 && !IsCubeMesh(obj))
    {
        MPlugArray inputs;
        dagNode.findPlug("inMesh").connectedTo(inputs, true, false);
        for (unsigned int i = 0; i < inputs.length(); i++)
        {
            MFnDependencyNode depNode(inputs[i].node());
            if (!depNode.findPlug("width").isNull() &&
                !depNode.findPlug("height").isNull() &&
                !depNode.findPlug("subdivisionsWidth").isNull() &&
                !depNode.findPlug("subdivisionsHeight").isNull())
            {
                if (axisPlugs != nullptr)
                {
                    axisPlugs->resize(3);
                    (*axisPlugs)[0] = depNode.findPlug("axisX");
                    (*axisPlugs)[1] = depNode.findPlug("axisY");
                    (*axisPlugs)[2] = depNode.findPlug("axisZ");
                }
                return true;
            }
        }
    }

    return false;
}

bool MayaUtilities::IsSphereMesh(MObject obj)
{
    const char* attributeNames[] = {"inMesh", "create"};
    for (int nameIndex = 0; nameIndex < FINJIN_COUNT_OF(attributeNames); nameIndex++)
    {
        MObject inMeshAttribute = MFnDagNode(obj).attribute(attributeNames[nameIndex]);
        if (!inMeshAttribute.isNull())
        {
            MPlug inMesh(obj, inMeshAttribute);
            MPlugArray inputs;
            inMesh.connectedTo(inputs, true, false);
            for (unsigned int i = 0; i < inputs.length(); i++)
            {
                MFn::Type type = inputs[i].node().apiType();
                if (type == MFn::kPolySphere || type == MFn::kSphere)
                    return true;
            }
        }
    }

    return false;
}

bool MayaUtilities::IsCubeMesh(MObject obj)
{
    MObject inMeshAttribute = MFnDagNode(obj).attribute("inMesh");
    if (!inMeshAttribute.isNull())
    {
        MPlug inMesh(obj, inMeshAttribute);
        MPlugArray inputs;
        inMesh.connectedTo(inputs, true, false);
        for (unsigned int i = 0; i < inputs.length(); i++)
        {
            MFn::Type type = inputs[i].node().apiType();
            if (type == MFn::kPolyCube)
                return true;
        }
    }

    return false;
}

bool MayaUtilities::GetPlaneSizeAndSegmentCount(FinjinVector2& size, FinjinIVector2& segmentCount, MObject object)
{
    size.x = size.y = 0;
    segmentCount.x = segmentCount.y = 0;

    MFnDagNode dagNode(object);

    //See if it's a nurbs plane
    MObject inMeshAttribute = dagNode.attribute("create");
    if (!inMeshAttribute.isNull())
    {
        MPlug inMesh(object, inMeshAttribute);
        MPlugArray inputs;
        inMesh.connectedTo(inputs, true, false);
        for (unsigned int i = 0; i < inputs.length(); i++)
        {
            MObject planeObject = inputs[i].node();
            MFn::Type type = planeObject.apiType();
            if (type == MFn::kNurbsPlane)
            {
                size.x = MayaPlug::GetFloatAttribute(planeObject, "width");
                size.y = size.x * MayaPlug::GetFloatAttribute(planeObject, "lengthRatio");
                segmentCount.x = MayaPlug::GetIntAttribute(planeObject, "patchesU");
                segmentCount.y = MayaPlug::GetIntAttribute(planeObject, "patchesV");
                return segmentCount.x > 0 && segmentCount.y > 0;
            }
        }
    }

    //See if it's a polygon plane
    //Make sure it's not a cube because the way we're testing for polygon planes passes for cubes
    MString fullName = dagNode.fullPathName();
    if (fullName.length() > 0 && !IsCubeMesh(object))
    {
        MPlugArray inputs;
        dagNode.findPlug("inMesh").connectedTo(inputs, true, false);
        for (unsigned int i = 0; i < inputs.length(); i++)
        {
            MFnDependencyNode depNode(inputs[i].node());
            MPlug widthPlug = depNode.findPlug("width");
            MPlug heightPlug = depNode.findPlug("height");
            MPlug subdivisionsWidthPlug = depNode.findPlug("subdivisionsWidth");
            MPlug subdivisionsHeightPlug = depNode.findPlug("subdivisionsHeight");
            if (!widthPlug.isNull() &&
                !heightPlug.isNull() &&
                !subdivisionsWidthPlug.isNull() &&
                !subdivisionsHeightPlug.isNull())
            {
                widthPlug.getValue(size.x);
                heightPlug.getValue(size.y);
                subdivisionsWidthPlug.getValue(segmentCount.x);
                subdivisionsHeightPlug.getValue(segmentCount.y);
                return segmentCount.x > 0 && segmentCount.y > 0;
            }
        }
    }

    return false;
}

bool MayaUtilities::GetSphereRadiusAndSegmentCount(float& radius, FinjinIVector2& segmentCount, MObject object)
{
    radius = 0;
    segmentCount.x = segmentCount.y = 0;

    const char* attributeNames[] = {"inMesh", "create"};
    for (int nameIndex = 0; nameIndex < FINJIN_COUNT_OF(attributeNames) && radius == 0; nameIndex++)
    {
        MObject inMeshAttribute = MFnDagNode(object).attribute(attributeNames[nameIndex]);
        if (!inMeshAttribute.isNull())
        {
            MPlug inMesh(object, inMeshAttribute);
            MPlugArray inputs;
            inMesh.connectedTo(inputs, true, false);
            for (unsigned int i = 0; i < inputs.length(); i++)
            {
                MObject sphereObject = inputs[i].node();
                MFn::Type type = sphereObject.apiType();
                if (type == MFn::kPolySphere)
                {
                    radius = MayaPlug::GetFloatAttribute(sphereObject, "radius");
                    segmentCount.x = MayaPlug::GetFloatAttribute(sphereObject, "subdivisionsAxis");
                    segmentCount.y = MayaPlug::GetFloatAttribute(sphereObject, "subdivisionsHeight");
                    break;
                }
                else if (type == MFn::kSphere)
                {
                    radius = MayaPlug::GetFloatAttribute(sphereObject, "radius");
                    segmentCount.x = MayaPlug::GetFloatAttribute(sphereObject, "sections");
                    segmentCount.y = MayaPlug::GetFloatAttribute(sphereObject, "spans");
                    break;
                }
            }
        }
    }

    return segmentCount.x > 0 && segmentCount.y > 0;
}

bool MayaUtilities::GetBoxSize(FinjinVector3& size, MObject object)
{
    size.SetZero();

    MObject inMeshAttribute = MFnDagNode(object).attribute("inMesh");
    if (!inMeshAttribute.isNull())
    {
        MPlug inMesh(object, inMeshAttribute);
        MPlugArray inputs;
        inMesh.connectedTo(inputs, true, false);
        for (unsigned int i = 0; i < inputs.length(); i++)
        {
            MObject cubeObject = inputs[i].node();
            MFn::Type type = cubeObject.apiType();
            if (type == MFn::kPolyCube)
            {
                size.x = MayaPlug::GetFloatAttribute(cubeObject, "width");
                size.y = MayaPlug::GetFloatAttribute(cubeObject, "height");
                size.z = MayaPlug::GetFloatAttribute(cubeObject, "depth");
                break;
            }
        }
    }

    return !size.IsZero();
}

bool MayaUtilities::GetCylinderRadiusAndLength(float& radius, float& length, MObject object)
{
    radius = length = 0;

    const char* attributeNames[] = {"inMesh", "create"};
    for (int nameIndex = 0; nameIndex < FINJIN_COUNT_OF(attributeNames) && (radius == 0 || length == 0); nameIndex++)
    {
        MObject inMeshAttribute = MFnDagNode(object).attribute(attributeNames[nameIndex]);
        if (!inMeshAttribute.isNull())
        {
            MPlug inMesh(object, inMeshAttribute);
            MPlugArray inputs;
            inMesh.connectedTo(inputs, true, false);
            for (unsigned int i = 0; i < inputs.length(); i++)
            {
                MObject cylinderObject = inputs[i].node();
                MFn::Type type = cylinderObject.apiType();
                if (type == MFn::kPolyCylinder)
                {
                    radius = MayaPlug::GetFloatAttribute(cylinderObject, "radius");
                    length = MayaPlug::GetFloatAttribute(cylinderObject, "height");
                    break;
                }
                else if (type == MFn::kCylinder)
                {
                    radius = MayaPlug::GetFloatAttribute(cylinderObject, "radius");
                    length = radius * MayaPlug::GetFloatAttribute(cylinderObject, "heightRatio");
                    break;
                }
            }
        }
    }

    return radius > 0 && length > 0;
}

bool MayaUtilities::GetFullWorldMatrix(MObject& object, MMatrix& worldMatrix)
{
    MStatus status;

    MFnDependencyNode depNode(object, &status);
    if (status != MStatus::kSuccess)
        return false;

    MObject worldMatrixAttr = depNode.attribute("worldMatrix");
    MPlug matrixPlug(object, worldMatrixAttr);
    matrixPlug = matrixPlug.elementByLogicalIndex(0);

    MObject matrixObject;
    status = matrixPlug.getValue(matrixObject);
    if (status != MStatus::kSuccess)
        return false;

    MFnMatrixData worldMatrixData(matrixObject, &status);
    if (status != MStatus::kSuccess)
        return false;

    worldMatrix = worldMatrixData.matrix(&status);
    if (status != MStatus::kSuccess)
        return false;

    /*if (addPivotOffset)
    {
        MTransformationMatrix pivotMatrix = MTransformationMatrix::identity;
        pivotMatrix.setTranslation(GetPivotPoint(object), SET_TRANSLATION_SPACE);

        worldMatrix = worldMatrix * pivotMatrix.asMatrix();
    }*/

    return true;
}

MVector MayaUtilities::GetPivotPoint(MObject obj, TimeAccessor time)
{
    TimeChanger timeChanger(time);
    return GetPivotPoint(obj);
}

static FinjinVector3 GetLocalPivot(MObject obj)
{
    MFnDependencyNode depNode(obj);
    FinjinVector3 rotatePivot = MayaPlug::GetVector3(depNode.findPlug("rotatePivot"));
    //FinjinVector3 scalePivot = MayaPlug::GetVector3(depNode.findPlug("scalePivot"));
    //FinjinVector3 rotatePivotTranslate = MayaPlug::GetVector3(depNode.findPlug("rotatePivotTranslate"));
    //return (rotatePivot + scalePivot) / 2.0f;
    return rotatePivot;
}

MVector MayaUtilities::GetPivotPoint(MObject obj)
{
    FinjinVector3 pivot = GetLocalPivot(obj);
    return MVector(pivot.x, pivot.y, pivot.z);
}


static MObject GetContainerChildWithFn(MObject container, MFn::Type type)
{
    MFnDagNode dagNode(container);
    unsigned int childCount = dagNode.childCount();
    MObject target;
    if (childCount > 0)
    {
        //See if a child without a transform can be found
        for (unsigned int i = 0; i < childCount; i++)
        {
            MObject child = dagNode.child(i);
            if (child.hasFn(type))
                return child;
        }
    }

    return MObject();
}

static bool GetConstraintTargets(MObject constraint, MObjectArray& targets)
{
    MFnDependencyNode depNode(constraint);
    MPlug targetArrayPlug = depNode.findPlug("target");
    for (unsigned int i = 0; i < targetArrayPlug.numElements(); i++)
    {
        MPlug targetPlug = targetArrayPlug[i];

        //Get the first input to the first valid child plug
        for (unsigned int childIndex = 0; childIndex < targetPlug.numChildren(); childIndex++)
        {
            MPlug childPlug = targetPlug.child(childIndex);
            MPlugArray inputs;
            childPlug.connectedTo(inputs, true, false);
            if (inputs.length() > 0)
            {
                targets.append(inputs[0].node());
                break;
            }
        }
    }

    return targets.length() > 0;
}

bool MayaUtilities::GetAimTarget(MObject obj, AimTarget& aimTarget)
{
    aimTarget.targetObject = MObject();

    MObject aimConstraint;
    MObjectArray targets;
    float aimScale = 1;

    //If the object is a light/camera and the parent group is a "look at", we'll get the target from the parent
    ObjectAccessor fullObject(obj);
    if (fullObject.IsLight() || fullObject.IsCamera())
    {
        ObjectAccessor parent = fullObject.GetParent();
        if (parent.IsValid() && parent.container.hasFn(MFn::kLookAt))
        {
            aimConstraint = parent.container;
            GetConstraintTargets(aimConstraint, targets);
        }
    }

    //If no target found yet, try to find the the target from the actual constraint
    if (targets.length() == 0)
    {
        aimConstraint = GetContainerChildWithFn(obj, MFn::kAimConstraint);
        GetConstraintTargets(aimConstraint, targets);

        aimScale = -1; //The aim comes back in the opposite direction that we want, so flip it
    }

    if (targets.length() > 0)
    {
        MFnDependencyNode aimConstraintDepNode(aimConstraint);
        aimTarget.targetObject = targets[0];
        aimTarget.aimVector = MayaPlug::GetVector3(aimConstraintDepNode.findPlug("aimVector"), FinjinVector3(0,0,1)) * aimScale;
        aimTarget.upVector = MayaPlug::GetVector3(aimConstraintDepNode.findPlug("upVector"), FinjinVector3(0,1,0));
        aimTarget.worldUpType = (AimTarget::WorldUpType)aimConstraintDepNode.findPlug("worldUpType").asShort();
        aimTarget.worldUpVector = MayaPlug::GetVector3(aimConstraintDepNode.findPlug("worldUpVector"), FinjinVector3(0,1,0));
        //aimTarget.worldUpObject = aimConstraintDepNode.findPlug("worldUpObject").asMObject(); //This doesn't work. There is no obvious way to get the world up object
    }

    return aimTarget.IsValid();
}

MMatrix MayaUtilities::InverseTranspose(const MMatrix& m)
{
    MMatrix inverse = m.inverse();
    return inverse.transpose();
}

MObject MayaUtilities::GetBestMaterial(MObject shader)
{
    return shader;
}

float MayaUtilities::GetActiveScale()
{
    return MDistance(1, INTERNAL_UNIT_TYPE).as(UI_UNIT_TYPE);
}

int MayaUtilities::FindString(const MStringArray& stringArray, const MString& s)
{
    for (unsigned int i = 0; i < stringArray.length(); i++)
    {
        if (stringArray[i] == s)
            return (int)i;
    }
    return -1;
}

bool MayaUtilities::StartsWith(const char* s1, const char* s2)
{
    size_t s1Length = strlen(s1);
    size_t s2Length = strlen(s2);
    if (s2Length > s1Length)
        return false;

    return strncmp(s1, s2, s2Length) == 0;
}

bool MayaUtilities::EndsWith(const char* s1, const char* s2)
{
    size_t s1Length = strlen(s1);
    size_t s2Length = strlen(s2);
    if (s2Length > s1Length)
        return false;

    return strncmp(s1 + s1Length - s2Length, s2, s2Length) == 0;
}

bool MayaUtilities::StartsWith(const MString& s1, const MString& s2)
{
    return StartsWith(s1.asUTF8(), s2.asUTF8());
}

bool MayaUtilities::EndsWith(const MString& s1, const MString& s2)
{
    return EndsWith(s1.asUTF8(), s2.asUTF8());
}
