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
#include "FinjinCustomDummy.hpp"
#include "FinjinMayaTypeID.hpp"
#include "NodeAttributeAdder.hpp"
#include "MayaUtilities.hpp"
#include "CustomHelperIconReaderWriter.hpp"
#include "Strings.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "wxApplicationWindow.hpp"
#include "ApplicationAccessor.hpp"

using namespace Finjin::Exporter;


//Macros-----------------------------------------------------------------------
#define CUSTOM_DUMMY_LOCK_EDGE_VERTICES wxCriticalSectionLocker customDummyEdgeVerticesLocker(edgeVerticesCriticalSection);


//Static initialization--------------------------------------------------------
FinjinCustomDummy::Attributes FinjinCustomDummy::attributes;
wxCriticalSection FinjinCustomDummy::edgeVerticesCriticalSection;

const MString FinjinCustomDummy::TYPE_NAME("FinjinCustomDummy");
const MTypeId FinjinCustomDummy::TYPE_ID(FinjinMayaTypeID::CUSTOM_DUMMY_NODE);


//Implementation---------------------------------------------------------------
FinjinCustomDummy::FinjinCustomDummy() 
{
}

FinjinCustomDummy::~FinjinCustomDummy() 
{
}

MStatus FinjinCustomDummy::compute(const MPlug& plug, MDataBlock& data)
{
    return MStatus::kSuccess;
}

void FinjinCustomDummy::DrawLocator(M3dView& view, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{
    MObject object = thisMObject();
    float size = MPlug(object, attributes.size).asMDistance().as(UI_UNIT_TYPE);
    bool alwaysFaceViewer = MPlug(object, attributes.alwaysFaceView).asBool();
    MString text = MPlug(object, attributes.text).asString();
    FinjinColor color = MayaPlug::GetColor(MPlug(object, attributes.color));
    
    if (status == M3dView::kLead)
        glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
    else
    {
        //MColor outlineColor = colorRGB(status);
        //glColor4f(outlineColor.r, outlineColor.g, outlineColor.b, 0.5f);
        glColor4f(color.r, color.g, color.b, .5f);
    }

    if (alwaysFaceViewer)
    {
        //Need to remove the rotation from the current model-view matrix

        //Get model-view matrix
        MMatrix modelViewMatrix;
        view.modelViewMatrix(modelViewMatrix);

        //Invert and get counter-rotation
        MMatrix viewModelMatrix = modelViewMatrix.inverse();        
        MQuaternion viewModelQuaternion;
        viewModelQuaternion = viewModelMatrix;

        //OpenGL takes an angle and axis, so get those
        MVector axis;
        double angle;
        viewModelQuaternion.getAxisAngle(axis, angle);

        //Save OpenGL's transformation matrix, then modify to remove rotation
        glPushMatrix();
        glRotated(RadiansToDegrees(angle), axis.x, axis.y, axis.z);        
    }

    {
        CUSTOM_DUMMY_LOCK_EDGE_VERTICES

        FinjinVector3 axis = MayaPlug::GetVector3(MPlug(object, attributes.axis));
        UpAxis upAxis = axis.AlmostEquals(FinjinVector3(0,1,0)) ? UpAxis::Y : UpAxis::Z;

        MPlug edgeVerticesPlug = MPlug(object, attributes.edgeVertices);
        int edgeVerticesCount = edgeVerticesPlug.evaluateNumElements();
        if (edgeVerticesCount > 0)
        {
            glBegin(GL_LINES);

            FinjinVector3 v;
            if (alwaysFaceViewer)
            {
                //The data needs to be oriented in a Z-up fashion in order for
                //the above modal-view rotation code to work correctly
                if (upAxis == UpAxis::Z)
                {
                    //The icon data is Z-up, no need to convert
                    for (int i = 0; i < edgeVerticesCount; i += 2)
                    {            
                        v = MayaPlug::GetVector3(edgeVerticesPlug[i]) * size;
                        glVertex3f(v.x, v.y, v.z);

                        v = MayaPlug::GetVector3(edgeVerticesPlug[i + 1]) * size;
                        glVertex3f(v.x, v.y, v.z);
                    }
                }
                else
                {
                    //The icon data is Y-up, so convert to Z-up
                    for (int i = 0; i < edgeVerticesCount; i += 2)
                    {            
                        v = MayaPlug::GetVector3(edgeVerticesPlug[i]) * size;
                        v.Set(v.x, -v.z, v.y);
                        glVertex3f(v.x, v.y, v.z);

                        v = MayaPlug::GetVector3(edgeVerticesPlug[i + 1]) * size;
                        v.Set(v.x, -v.z, v.y);
                        glVertex3f(v.x, v.y, v.z);
                    }
                }
            }
            else
            {
                //Just draw as normal
                for (int i = 0; i < edgeVerticesCount; i += 2)
                {            
                    v = MayaPlug::GetVector3(edgeVerticesPlug[i]) * size;
                    glVertex3f(v.x, v.y, v.z);

                    v = MayaPlug::GetVector3(edgeVerticesPlug[i + 1]) * size;
                    glVertex3f(v.x, v.y, v.z);
                }
            }

            glEnd();
        }
        else
            DrawCube(size, size, size);
    }

    if (alwaysFaceViewer)
        glPopMatrix();

    if (text.length() > 0)
        view.drawText(text, MPoint(0,0,0), M3dView::kLeft);
}

MBoundingBox FinjinCustomDummy::boundingBox() const 
{
    MBoundingBox bbox;

    MObject object = thisMObject();
    float size = MPlug(object, attributes.size).asMDistance().as(UI_UNIT_TYPE);
    bool alwaysFaceViewer = MPlug(object, attributes.alwaysFaceView).asBool();
    
    CUSTOM_DUMMY_LOCK_EDGE_VERTICES

    auto axis = MayaPlug::GetVector3(MPlug(object, attributes.axis));
    auto upAxis = axis.AlmostEquals(FinjinVector3(0,1,0)) ? UpAxis::Y : UpAxis::Z;

    MPlug edgeVerticesPlug(object, attributes.edgeVertices);
    int edgeVerticesCount = edgeVerticesPlug.evaluateNumElements();
    if (edgeVerticesCount > 0)
    {
        FinjinVector3 v;
        if (alwaysFaceViewer)
        {
            if (upAxis == UpAxis::Z)
            {
                //The icon data is Z-up, no need to convert
                for (int i = 0; i < edgeVerticesCount; i += 2)
                {            
                    v = MayaPlug::GetVector3(edgeVerticesPlug[i]) * size;
                    bbox.expand(MPoint(v.x, v.y, v.z));

                    v = MayaPlug::GetVector3(edgeVerticesPlug[i + 1]) * size;
                    bbox.expand(MPoint(v.x, v.y, v.z));
                }
            }
            else
            {
                //The icon data is Y-up, so convert to Z-up
                for (int i = 0; i < edgeVerticesCount; i += 2)
                {            
                    v = MayaPlug::GetVector3(edgeVerticesPlug[i]) * size;
                    v.Set(v.x, -v.z, v.y);
                    bbox.expand(MPoint(v.x, v.y, v.z));

                    v = MayaPlug::GetVector3(edgeVerticesPlug[i + 1]) * size;
                    v.Set(v.x, -v.z, v.y);
                    bbox.expand(MPoint(v.x, v.y, v.z));
                }
            }
        }
        else
        {
            //Just draw as normal
            for (int i = 0; i < edgeVerticesCount; i += 2)
            {            
                v = MayaPlug::GetVector3(edgeVerticesPlug[i]) * size;
                bbox.expand(MPoint(v.x, v.y, v.z));

                v = MayaPlug::GetVector3(edgeVerticesPlug[i + 1]) * size;
                bbox.expand(MPoint(v.x, v.y, v.z));
            }
        }
    }
    else
    {
        bbox.expand(MPoint(-0.5f * size, -0.5f * size, -0.5f * size));
        bbox.expand(MPoint(0.5f * size, 0.5f * size, 0.5f * size));
    }

    return bbox;
}

MStatus FinjinCustomDummy::Initialize()
{
    return AddAttributes() == MStatus::kSuccess ? MStatus::kSuccess : MS::kFailure;
}

void* FinjinCustomDummy::Creator()
{
    return new FinjinCustomDummy();
}

void FinjinCustomDummy::ChooseCustomIcon(MObject& object)
{
    MStatus status;
    MFnDependencyNode depNode(object, &status);
    if (status == MStatus::kSuccess && depNode.typeId() == TYPE_ID)
    {
        CUSTOM_DUMMY_LOCK_EDGE_VERTICES

        SUSPEND_CUSTOM_CONTROLS;
        wxApplicationWindow appWindow;
        wxFileDialog dialog(&appWindow, Strings::CHOOSE_FILE, wxEmptyString, wxEmptyString, Strings::CUSTOM_HELPER_ICON_FILE_FILTER);
        if (dialog.ShowModal() == wxID_OK)
        {
            wxString file = dialog.GetPath();

            CustomHelperIconReader reader;
            if (reader.Read(file) && reader.vertices.size() % 2 == 0)
            {
                UpAxis upAxis = ApplicationAccessor::GetUpAxis();

                //Set axis
                MPlug axisPlug(object, attributes.axis);
                MayaPlug::SetVector3(axisPlug, upAxis == UpAxis::Y ? FinjinVector3(0,1,0) : FinjinVector3(0,0,1));

                //Remove existing vertices
                MPlug edgeVerticesPlug(object, attributes.edgeVertices);
                MayaPlug::RemoveAttributeElements(edgeVerticesPlug);

                //Add new vertices
                FinjinVector3 v;
                for (size_t i = 0; i < reader.vertices.size(); i++)
                {
                    v = reader.vertices[i];
                    if (upAxis == UpAxis::Y)
                        v.Set(v.x, v.z, -v.y);
                    MayaPlug::AddAttributeElementValue(edgeVerticesPlug, v);
                }
            }
        }
    }        
}

void FinjinCustomDummy::UseDefaultIcon(MObject& object)
{
    MStatus status;
    MFnDependencyNode depNode(object, &status);
    if (status == MStatus::kSuccess && depNode.typeId() == TYPE_ID)
    {
        CUSTOM_DUMMY_LOCK_EDGE_VERTICES

        MayaPlug::RemoveAttributeElements(object, attributes.edgeVertices);
    }
}

MStatus FinjinCustomDummy::AddAttributes()
{
    NodeAttributeAdder adder;        
    
    attributes.alwaysFaceView = adder.AddBool("AlwaysFaceView");
    attributes.size = adder.AddDistance("IconSize", MDistance(5, INTERNAL_UNIT_TYPE), 100);
    attributes.color = adder.AddColor("IconColor", MColor(0.0f, 1.0f, 0.0f, 0.5f));
    attributes.text = adder.AddString("Text");
        
    adder.hidden = true;
    attributes.edgeVertices = adder.AddFloat3Array("EdgeVertices");
    attributes.axis = adder.AddFloat3("Axis");

    return MS::kSuccess;
}
