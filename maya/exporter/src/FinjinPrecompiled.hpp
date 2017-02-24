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


#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

//Maya includes
#include <maya/MPxFileTranslator.h>
#include <maya/MDistance.h>
#include <maya/MColor.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MFileIO.h>
#include <maya/MFileObject.h>
#include <maya/MFnAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MFnCamera.h>
#include <maya/MFnLight.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MPxCommand.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MVector.h>
#include <maya/MPointArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MSceneMessage.h>
#include <maya/MDGMessage.h>
#include <maya/MTimerMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MUserEventMessage.h>
#include <maya/MViewportRenderer.h>
#include <maya/MImage.h>
#include <maya/MRenderingInfo.h>
#include <maya/M3dView.h>
#include <maya/MArgList.h>
#include <maya/MPxData.h>
#include <maya/MFnPluginData.h>
#include <maya/MPxNode.h> 
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnGenericAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnStringData.h>
#include <maya/MDagModifier.h>
#include <maya/MDGModifier.h>
#include <maya/MIteratorType.h>
#include <maya/MFStream.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MPxManipContainer.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>
#include <maya/MTime.h>
#include <maya/MFnTransform.h>
#include <maya/MAnimControl.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnGenericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MFnWeightGeometryFilter.h>
#include <maya/MFnSet.h>
#include <maya/MFnIkJoint.h>
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MItGeometry.h>
#include <maya/MCommandResult.h>

namespace Maya
{
    static const float PI = 3.1415926535f;
}

//Finjin includes
#include "CommonTypes.hpp"


//Macros-----------------------------------------------------------------------
#define UI_UNIT_TYPE MDistance::uiUnit()
#define INTERNAL_UNIT_TYPE MDistance::internalUnit()

#define GET_TRANSLATION_SPACE MSpace::kPostTransform
#define SET_TRANSLATION_SPACE MSpace::kPostTransform
#define GET_SCALE_SPACE MSpace::kPostTransform
#define SET_SCALE_SPACE MSpace::kPostTransform
#define GET_POINTS_NORMALS_SPACE MSpace::kTransform

#define EXPAND_APP_XYZ(value) \
    (value).x,\
    (value).y,\
    (value).z

#define APP_VERSION_NAME wxString::Format(wxT("Maya%d-%s"), MAYA_API_VERSION / 100, APPLICATION_PLATFORM)

//Many calls to MGlobal::execute use this value, which aids in debugging
//It should always be false when building for release
#define DISPLAY_MAYA_COMMAND false

#define APP_TIME_RESOLUTION_UNIT WxTimeDurationUnit::MILLISECONDS
