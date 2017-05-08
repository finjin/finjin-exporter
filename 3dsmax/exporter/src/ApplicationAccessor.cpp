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
#include "FileUtilities.hpp"
#include "MaxUtilities.hpp"
#include "FinjinSceneSettingsObject.hpp"
#include "StringUtilities.hpp"
#include "Strings.hpp"
#include "MaxBitmapUtilities.hpp"
#include "PropertyValues.hpp"
#include <IPathConfigMgr.h>

using namespace Finjin::Exporter;


//Local functions---------------------------------------------------------------
static StdFog* GetStandardFog()
{
    //Look for the first fog atmospheric
    StdFog* fog = nullptr;
    int atmosphericCount = GetCOREInterface()->NumAtmospheric();
    for (int index = 0; index < atmosphericCount; index++)
    {
        auto atmos = GetCOREInterface()->GetAtmospheric(index);
        if (atmos->ClassID() == Class_ID(FOG_CLASS_ID, 0))
        {
            //Found fog atmospheric. Make sure it's of "standard" type
            auto maybeStandardFog = static_cast<StdFog*>(atmos);
            if (maybeStandardFog->GetType() == 0)
            {
                fog = maybeStandardFog;
                break;
            }
        }
    }

    return fog;
}


//Static initialization---------------------------------------------------------
const wxString ApplicationAccessor::APPLICATION_LONG_NAME(wxT("3D Studio Max"));
const wxString ApplicationAccessor::APPLICATION_SHORT_NAME(wxT("3DS Max"));
const wxString ApplicationAccessor::MATERIAL_SELECTOR_WINDOW_NAME(wxT("Material Editor"));
const wxString ApplicationAccessor::BONE_NAME(wxT("Bone"));
const wxString ApplicationAccessor::TEXTURE_COORDINATE_SET_NAME(wxT("Map Channel"));


//Implementation----------------------------------------------------------------
UpAxis ApplicationAccessor::GetUpAxis()
{
    return UpAxis::Z;
}

FinjinTimeInterval ApplicationAccessor::GetAnimationRange()
{
    Interval maxInterval = GetCOREInterface()->GetAnimRange();

    FinjinTimeInterval interval;
    interval.start.SetNativeTime(maxInterval.Start());
    interval.end.SetNativeTime(maxInterval.End());
    return interval;
}

WXWidget ApplicationAccessor::GetMainWindow(bool modal)
{
    return GetCOREInterface()->GetMAXHWnd();
}

void ApplicationAccessor::RegisterModelessDialog(wxWindow* dialog)
{
    GetCOREInterface()->RegisterDlgWnd((HWND)dialog->GetHWND());
}

void ApplicationAccessor::UnregisterModelessDialog(wxWindow* dialog)
{
    GetCOREInterface()->UnRegisterDlgWnd((HWND)dialog->GetHWND());
}

wxString ApplicationAccessor::GetApplicationVersion()
{
    wxString programVersion;

#ifdef MAX_PRODUCT_YEAR_NUMBER
    programVersion << MAX_PRODUCT_YEAR_NUMBER;
#else
    programVersion << MAX_VERSION_MAJOR;
    if (MAX_VERSION_MINOR > 0)
        programVersion << wxT(".") << MAX_VERSION_MINOR;
#endif

    return programVersion;
}

wxString ApplicationAccessor::GetFinjinHomeDirectory(bool writable)
{
    if (writable)
        return GetUserHomeApplicationDirectory();
    else
        return GetApplicationDirectory();
}

wxString ApplicationAccessor::GetApplicationDirectory()
{
    wxString directory;

    directory = GetCOREInterface()->GetDir(APP_MAX_SYS_ROOT_DIR);

    if (!directory.empty())
        FileUtilities::EnsureTrailingPathSeparator(directory);

    return directory;
}

wxString ApplicationAccessor::GetHelpDirectory()
{
    wxString directory = GetCOREInterface()->GetDir(APP_HELP_DIR);
    if (!directory.empty())
        FileUtilities::EnsureTrailingPathSeparator(directory);
    return directory;
}

wxString ApplicationAccessor::GetCurrentProjectDirectory()
{
    wxString directory = GetCOREInterface()->GetDir(APP_SCENE_DIR); //APP_PROJECT_FOLDER_DIR;

    if (!directory.empty())
        FileUtilities::EnsureTrailingPathSeparator(directory);

    return directory;
}

bool ApplicationAccessor::SetCurrentProjectDirectory(wxString directory)
{
    return IPathConfigMgr::GetPathConfigMgr()->SetCurrentProjectFolder(MaxSDK::Util::Path(directory.wx_str()));
}

wxString ApplicationAccessor::GetApplicationHelpLocation()
{
    return FileUtilities::JoinPath(GetHelpDirectory(), wxT("finjin-exporter.chm"));
}

bool ApplicationAccessor::GetWindowBackgroundColor(wxColor& color)
{
    COLORREF c = GetColorManager()->GetColor(kBackground);
    color = wxColor(GetRValue(c), GetGValue(c), GetBValue(c));
    return true;
}

bool ApplicationAccessor::GetTextBackgroundColor(wxColor& color)
{
    COLORREF c = GetColorManager()->GetColor(kWindow);
    color = wxColor(GetRValue(c), GetGValue(c), GetBValue(c));
    return true;
}

bool ApplicationAccessor::GetTextForegroundColor(wxColor& color)
{
    COLORREF c = GetColorManager()->GetColor(kWindowText);
    color = wxColor(GetRValue(c), GetGValue(c), GetBValue(c));
    return true;
}

bool ApplicationAccessor::GetStaticTextForegroundColor(wxColor& color)
{
    COLORREF c = GetColorManager()->GetColor(kText);
    color = wxColor(GetRValue(c), GetGValue(c), GetBValue(c));
    return true;
}

float ApplicationAccessor::GetInternalToUIScale()
{
    return MaxUtilities::GetActiveScale();
}

float ApplicationAccessor::GetMasterScaleMeters()
{
    return GetMasterScale(UNITS_METERS);
}

float ApplicationAccessor::GetScale(wxString& scaleUnit)
{
    float scale = 1;

    if (scaleUnit.empty())
        scale = MaxUtilities::GetActiveScale();
    else if (scaleUnit == PropertyValues::SceneScale::METERS)
        scale = GetMasterScale(UNITS_METERS);
    else if (scaleUnit == PropertyValues::SceneScale::KILOMETERS)
        scale = GetMasterScale(UNITS_KILOMETERS);
    else if (scaleUnit == PropertyValues::SceneScale::FEET)
        scale = GetMasterScale(UNITS_FEET);
    else if (scaleUnit == PropertyValues::SceneScale::MILES)
        scale = GetMasterScale(UNITS_MILES);
    else
    {
        scale = MaxUtilities::GetActiveScale();
        scaleUnit = wxEmptyString;
    }

    return scale;
}

wxString ApplicationAccessor::GetDefaultUnitType(bool abbreviate)
{
    wxString name;
    float fractional;
    MaxUtilities::GetCurrentUnitInfo(name, fractional, abbreviate);
    return name;
}

wxString ApplicationAccessor::FormatWorldValue(float value)
{
    return ApplicationStringToWxString(FormatUniverseValue(value));
}

void ApplicationAccessor::LogMessage(const wxChar* format, ...)
{
    FINJIN_EXPORTER_NEW_FORMAT_STRING_VA(message, format);
    LogMessage(message);
}

void ApplicationAccessor::LogMessage(const wxString& message)
{
    if (the_listener != nullptr)
    {
        MSTR maxMessage = WxStringToApplicationString(message);
        the_listener->edit_stream->printf(_M("%s\n"), maxMessage.data());
        the_listener->edit_stream->flush();
    }
}

bool ApplicationAccessor::SupportsProjects()
{
    return true;
}

bool ApplicationAccessor::SupportsNestedMaterials()
{
    return true;
}

bool ApplicationAccessor::SupportsNoteTracks()
{
    return true;
}

wxString ApplicationAccessor::GetSceneAuthor()
{
    return MaxUtilities::GetPropertyString(PROPSET_SUMMARYINFO, PIDSI_AUTHOR);
}

wxString ApplicationAccessor::GetLastAuthor()
{
    return MaxUtilities::GetPropertyString(PROPSET_SUMMARYINFO, PIDSI_LASTAUTHOR);
}

FinjinColor ApplicationAccessor::GetAmbientLightColor(TimeAccessor time)
{
    Point3 ambientColor = GetCOREInterface()->GetAmbient(time.GetNativeTime(), FOREVER);
    return FinjinColor(ambientColor.x, ambientColor.y, ambientColor.z);
}

bool ApplicationAccessor::SetAmbientLightColor(FinjinColor value, TimeAccessor time)
{
    GetCOREInterface()->SetAmbient(time.GetNativeTime(), Point3(value.r, value.g, value.b));
    return true;
}

FinjinColor ApplicationAccessor::GetBackgroundColor(TimeAccessor time)
{
    Point3 backgroundColor = GetCOREInterface()->GetBackGround(time.GetNativeTime(), FOREVER);
    return FinjinColor(backgroundColor.x, backgroundColor.y, backgroundColor.z);
}

bool ApplicationAccessor::SetBackgroundColor(FinjinColor value, TimeAccessor time)
{
    GetCOREInterface()->SetBackGround(time.GetNativeTime(), Point3(value.r, value.g, value.b));
    return true;
}

bool ApplicationAccessor::GetLinearFog(float& start, float& end, FinjinColor& color, float range, TimeAccessor time)
{
    StdFog* fog = GetStandardFog();
    if (fog != nullptr)
    {
        start = Limited(fog->GetNear(time.GetNativeTime()), 0.0f, 1.0f);
        end = Limited(fog->GetFar(time.GetNativeTime()), 0.0f, 1.0f);
        Color maxFogColor = fog->GetColor(time.GetNativeTime());
        color.Set(maxFogColor.r, maxFogColor.g, maxFogColor.b);
    }
    return fog != nullptr;
}

bool ApplicationAccessor::SetLinearFog(float start, float end, FinjinColor color, float range, TimeAccessor time)
{
    StdFog* fog = GetStandardFog();
    if (fog != nullptr)
    {
        if (start >= 0)
            fog->SetNear(Limited(start, 0.0f, 1.0f), time.GetNativeTime());
        if (end >= 0)
            fog->SetFar(Limited(end, 0.0f, 1.0f), time.GetNativeTime());
        fog->SetColor(Color(color.r, color.g, color.b), time.GetNativeTime());
    }
    return fog != nullptr;
}

void ApplicationAccessor::EnableShortcutKeys(bool enable)
{
    if (enable)
        EnableAccelerators();
    else
        DisableAccelerators();
}

bool ApplicationAccessor::UsesFullNames()
{
    return false;
}

bool ApplicationAccessor::InQuietMode()
{
    return GetCOREInterface()->GetQuietMode() ? true : false;
}
