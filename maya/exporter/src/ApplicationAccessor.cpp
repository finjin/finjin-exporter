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
#include "ApplicationAccessor.hpp"
#include "SystemUtilities.hpp"
#include "FileUtilities.hpp"
#include "RegistryKeyEnumerator.hpp"
#include "MayaUtilities.hpp"
#include "StringUtilities.hpp"
#include "TimeChanger.hpp"
#include "PropertyValues.hpp"

using namespace Finjin::Exporter;


//Local functions---------------------------------------------------------------
static MObject GetEnvFogMaterial()
{
    MObject envFog;
    const MFn::Type listType = MFn::kShaderList;
    const MString listPlugName = "shaders";
    MItDependencyNodes nodeIt(listType);
    MFnDependencyNode depNode(nodeIt.item());
    MPlug shaders = depNode.findPlug(listPlugName);
    unsigned int count = shaders.evaluateNumElements();
    for (unsigned int i = 0; i < count; i++)
    {
        MPlug element = shaders.elementByLogicalIndex(i);
        MPlugArray inputs;
        element.connectedTo(inputs, true, false);
        if (inputs.length() > 0)
        {
            MObject object = inputs[0].node();
            MFnDependencyNode depNode(object);
            if (object.apiType() == MFn::kEnvFogMaterial)
            {
                envFog = object;
                break;
            }
        }
    }
    return envFog;
}

static bool GetAmbientLightObject(ObjectAccessor& object)
{
    MObject lightObject;

    MItDag dagIterator(MItDag::kBreadthFirst, MFn::kInvalid);
    for (; !dagIterator.isDone(); dagIterator.next())
    {
        MObject obj = dagIterator.currentItem();
        if (obj.hasFn(MFn::kAmbientLight))
        {
            lightObject = obj;
            break;
        }
    }

    object = lightObject;
    return true;
}


//Static initialization---------------------------------------------------------
const wxString ApplicationAccessor::APPLICATION_LONG_NAME(wxT("Maya"));
const wxString ApplicationAccessor::APPLICATION_SHORT_NAME(wxT("Maya"));
const wxString ApplicationAccessor::MATERIAL_SELECTOR_WINDOW_NAME(wxT("Hypershade"));
const wxString ApplicationAccessor::BONE_NAME(wxT("Joint"));
const wxString ApplicationAccessor::TEXTURE_COORDINATE_SET_NAME(wxT("UV Set"));


//Implementation----------------------------------------------------------------
UpAxis ApplicationAccessor::GetUpAxis()
{
    return MGlobal::isYAxisUp() ? UpAxis::Y : UpAxis::Z;
}

FinjinTimeInterval ApplicationAccessor::GetAnimationRange()
{
    FinjinTimeInterval interval;
    interval.start.SetNativeTime(MAnimControl::minTime());
    interval.end.SetNativeTime(MAnimControl::maxTime());
    return interval;
}

WXWidget ApplicationAccessor::GetMainWindow(bool modal)
{
    return (WXWidget)M3dView::applicationShell();
}

wxString ApplicationAccessor::GetApplicationVersion()
{
    wxString programVersion;

    int majorVersion = MAYA_API_VERSION / 100;
    int minorVersion = (MAYA_API_VERSION % 100) / 10;
    programVersion << majorVersion;
    if (minorVersion > 0)
    {
        programVersion << wxT(".");
        programVersion << minorVersion;
    }

    return programVersion;
}

wxString ApplicationAccessor::GetFinjinHomeDirectory(bool writable)
{
    if (writable)
        return GetUserHomeApplicationDirectory();
    else
        return GetApplicationDirectory() + wxT("bin/");
}

wxString ApplicationAccessor::GetApplicationDirectory()
{
    MString mayaDirectory = getenv("MAYA_LOCATION");
    wxString directory = ApplicationStringToWxString(mayaDirectory);
    if (!directory.empty())
        FileUtilities::EnsureTrailingPathSeparator(directory);
    return directory;
}

wxString ApplicationAccessor::GetHelpDirectory()
{
    wxString directory = GetApplicationDirectory();
    if (!directory.empty())
        directory += wxT("docs/ext/");
    return directory;
}

wxString ApplicationAccessor::GetCurrentProjectDirectory()
{
    wxString directory;
    MString workspace;
    MStatus status = MGlobal::executeCommand("workspace -q -rd", workspace, DISPLAY_MAYA_COMMAND);
    if (status == MS::kSuccess)
        directory = ApplicationStringToWxString(workspace);
    if (!directory.empty())
        FileUtilities::EnsureTrailingPathSeparator(directory);
    return directory;
}

bool ApplicationAccessor::SetCurrentProjectDirectory(wxString directory)
{
    auto command = wxString::Format(wxT("workspace -rd \"%s\""), directory.wx_str());
    return MGlobal::executeCommand(WxStringToApplicationString(command), DISPLAY_MAYA_COMMAND) == MStatus::kSuccess;
}

wxString ApplicationAccessor::GetApplicationHelpLocation()
{
    wxString location;

#if defined(__WXMSW__)
    location = FileUtilities::JoinPath(GetHelpDirectory(), wxT("finjin-exporter.chm"));
#elif defined(__WXMAC__)
    location = FileUtilities::JoinPath(GetApplicationDirectory(), wxString::Format(wxT("docs/Maya%d/en_US/Nodes/FinjinDocuments/index.html"), MAYA_API_VERSION / 100));

    if (wxFileExists(location) || wxDirExists(location))
        location = wxT("file://") + location;
    else
        location = wxT("http://finjin.com/docs/finjin-exporter-maya");
#else
    Not implemented!
#endif

    return location;
}

bool ApplicationAccessor::GetWindowBackgroundColor(wxColor& color)
{
    return false;
}

bool ApplicationAccessor::GetTextBackgroundColor(wxColor& color)
{
    return false;
}

bool ApplicationAccessor::GetTextForegroundColor(wxColor& color)
{
    return false;
}

float ApplicationAccessor::GetInternalToUIScale()
{
    return MayaUtilities::GetActiveScale();
}

float ApplicationAccessor::GetMasterScaleMeters()
{
    return MDistance(1, INTERNAL_UNIT_TYPE).asMeters();
}

float ApplicationAccessor::GetScale(wxString& scaleUnit)
{
    float scale = 1;

    if (scaleUnit.empty())
        scale = MayaUtilities::GetActiveScale();
    else if (scaleUnit == PropertyValues::SceneScale::METERS)
        scale = MDistance(1, INTERNAL_UNIT_TYPE).asMeters();
    else if (scaleUnit == PropertyValues::SceneScale::KILOMETERS)
        scale = MDistance(1, INTERNAL_UNIT_TYPE).asKilometers();
    else if (scaleUnit == PropertyValues::SceneScale::FEET)
        scale = MDistance(1, INTERNAL_UNIT_TYPE).asFeet();
    else if (scaleUnit == PropertyValues::SceneScale::MILES)
        scale = MDistance(1, INTERNAL_UNIT_TYPE).asMiles();
    else
    {
        scale = MayaUtilities::GetActiveScale();
        scaleUnit = wxEmptyString;
    }

    return scale;
}

wxString ApplicationAccessor::GetDefaultUnitType(bool abbreviate)
{
    wxString name;
    switch (MDistance::uiUnit())
    {
        case MDistance::kInches: name = abbreviate ? wxT("in") : wxT("inches"); break;
        case MDistance::kFeet: name = abbreviate ? wxT("ft") : wxT("feet"); break;
        case MDistance::kYards: name = abbreviate ? wxT("yd") : wxT("yards"); break;
        case MDistance::kMiles: name = abbreviate ? wxT("mi") : wxT("miles"); break;
        case MDistance::kMillimeters: name = abbreviate ? wxT("mm") : wxT("millimeters"); break;
        case MDistance::kCentimeters: name = abbreviate ? wxT("cm") : wxT("centimeters"); break;
        case MDistance::kKilometers: name = abbreviate ? wxT("km") : wxT("kilometers"); break;
        case MDistance::kMeters: name = abbreviate ? wxT("m") : wxT("meters"); break;
        default: break;
    }
    return name;
}

wxString ApplicationAccessor::FormatWorldValue(float value)
{
    return ApplicationAccessorBase::FormatWorldValue(value * GetInternalToUIScale()) + GetDefaultUnitType(true);
}

void ApplicationAccessor::LogMessage(const wxChar* format, ...)
{
    wxString message;
    FINJIN_EXPORTER_FORMAT_STRING_VA(message, format);
    LogMessage(message);
}

void ApplicationAccessor::LogMessage(const wxString& message)
{
    auto mesageUtf8 = message.ToUTF8();
    std::cout << mesageUtf8.data() << std::endl;
}

bool ApplicationAccessor::Supports3DTextureCoordinates()
{
    return false;
}

bool ApplicationAccessor::SupportsNestedMaterials()
{
    return false;
}

bool ApplicationAccessor::NeedsExtraMaterials()
{
    return true;
}

FinjinColor ApplicationAccessor::GetAmbientLightColor(TimeAccessor time)
{
    ObjectAccessor lightObject;
    GetAmbientLightObject(lightObject);
    FinjinColor ambientLightColor;
    if (lightObject.IsValid())
    {
        MFnLight light(lightObject);

        TimeChanger timeChanger(time);
        MColor mayaColor = light.color();
        ambientLightColor.Set(mayaColor.r, mayaColor.g, mayaColor.b, mayaColor.a);
    }
    return ambientLightColor;
}

bool ApplicationAccessor::SetAmbientLightColor(FinjinColor value, TimeAccessor time)
{
    MObject lightObject;

    MItDag dagIterator(MItDag::kBreadthFirst, MFn::kInvalid);
    for (; !dagIterator.isDone(); dagIterator.next())
    {
        MObject obj = dagIterator.currentItem();
        if (obj.hasFn(MFn::kAmbientLight))
        {
            lightObject = obj;
            break;
        }
    }

    if (!lightObject.isNull())
    {
        MFnLight light(lightObject);
        light.setColor(MColor(value.r, value.g, value.b));
        return true;
    }
    else
        return false;
}

FinjinColor ApplicationAccessor::GetBackgroundColor(TimeAccessor time)
{
    FinjinColor backgroundColor;

    TimeChanger timeChanger(time);

    M3dView view = M3dView::active3dView();
    MDagPath cameraDagPath;
    view.getCamera(cameraDagPath);

    MayaPlug::GetColor(cameraDagPath.node(), "backgroundColor", backgroundColor);

    return backgroundColor;
}

bool ApplicationAccessor::SetBackgroundColor(FinjinColor value, TimeAccessor time)
{
    TimeChanger timeChanger(time);

    M3dView view = M3dView::active3dView();
    MDagPath cameraDagPath;
    view.getCamera(cameraDagPath);

    MayaPlug::SetColor(cameraDagPath.node(), "backgroundColor", value);

    return true;
}

bool ApplicationAccessor::GetLinearFog(float& start, float& end, FinjinColor& color, float range, TimeAccessor time)
{
    MObject envFog = GetEnvFogMaterial();
    if (!envFog.isNull())
    {
        MFnDependencyNode depNode(envFog);
        TimeChanger timeChanger(time);

        start = 0;
        end = Limited(depNode.findPlug("saturationDistance").asFloat() / range, 0.0f, 1.0f);
        color = MayaPlug::GetColor(depNode.findPlug("color"));
    }
    return !envFog.isNull();
}

bool ApplicationAccessor::SetLinearFog(float start, float end, FinjinColor color, float range, TimeAccessor time)
{
    MObject envFog = GetEnvFogMaterial();
    if (!envFog.isNull())
    {
        MFnDependencyNode depNode(envFog);
        TimeChanger timeChanger(time);

        if (end >= 0)
            depNode.findPlug("saturationDistance").setValue(end * range);
        MayaPlug::SetColor(depNode.findPlug("color"), color);
    }
    return !envFog.isNull();
}

void ApplicationAccessor::EnableExport(bool enable)
{
    TimeChanger::Enable(enable);
    ApplicationAccessorBase::EnableExport(enable);
}
