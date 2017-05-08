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
#include "FinjinSceneSettingsAccessorBase.hpp"
#include "FileUtilities.hpp"
#include "BaseDirectory.hpp"
#include "StringUtilities.hpp"
#include "PropertyValues.hpp"
#include "ApplicationAccessor.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "EnvironmentFar.hpp"
#include "MacroExpander.hpp"
#include "FinjinGlobalSettings.hpp"
#include "Strings.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
void FinjinSceneSettingsAccessorBase::GetSubmeshNamingValue(SubmeshNaming& naming)
{
    naming.custom = GetSubmeshCustomName();

    wxString type = GetSubmeshNaming();
    if (type == wxT("material"))
        naming.type = SubmeshNaming::MATERIAL;
    else if (type == wxT("custom") && !naming.custom.empty())
        naming.type = SubmeshNaming::CUSTOM;
    else
        naming.type = SubmeshNaming::SUBMESH;
}

void FinjinSceneSettingsAccessorBase::GetFlags(std::vector<wxString>& names)
{
    int count = GetFlagCount();
    names.resize(count);
    for (int i = 0; i < count; i++)
        GetFlag(i, names[i]);
}

wxString FinjinSceneSettingsAccessorBase::GetNodeAnimationSampleValueText()
{
    return PropertyValues::ChooseAnimationSampleValue(GetNodeAnimationSampleType(), GetNodeAnimationSampleInterval());
}

void FinjinSceneSettingsAccessorBase::SetNodeAnimationSampling(double sampling, SampleType sampleType, bool sampleTypeValid)
{
    if (!sampleTypeValid)
    {
        SetNodeAnimationSampleInterval(0);
        SetNodeAnimationSampleType(SampleType::INTERVAL);
    }
    else if (sampleType == SampleType::INTERVAL)
    {
        SetNodeAnimationSampleInterval(sampling);
        SetNodeAnimationSampleType(SampleType::INTERVAL);
    }
    else if (sampleType == SampleType::RATE)
    {
        double rate = sampling;
        double interval = 0;
        if (rate != 0)
            interval = 1 / rate;
        SetNodeAnimationSampleInterval(interval);
        SetNodeAnimationSampleType(SampleType::RATE);
    }
}

wxString FinjinSceneSettingsAccessorBase::GetVertexAnimationSampleValueText()
{
    return PropertyValues::ChooseAnimationSampleValue(GetVertexAnimationSampleType(), GetVertexAnimationSampleInterval());
}

void FinjinSceneSettingsAccessorBase::SetVertexAnimationSampling(double sampling, SampleType sampleType, bool sampleTypeValid)
{
    if (!sampleTypeValid)
    {
        SetVertexAnimationSampleInterval(0);
        SetVertexAnimationSampleType(SampleType::INTERVAL);
    }
    else if (sampleType == SampleType::INTERVAL)
    {
        SetVertexAnimationSampleInterval(sampling);
        SetVertexAnimationSampleType(SampleType::INTERVAL);
    }
    else if (sampleType == SampleType::RATE)
    {
        double rate = sampling;
        double interval = 0;
        if (rate != 0)
            interval = 1 / rate;
        SetVertexAnimationSampleInterval(interval);
        SetVertexAnimationSampleType(SampleType::RATE);
    }
}

wxString FinjinSceneSettingsAccessorBase::GetSkeletonAnimationSampleValueText()
{
    return PropertyValues::ChooseAnimationSampleValue(GetSkeletonAnimationSampleType(), GetSkeletonAnimationSampleInterval());
}

void FinjinSceneSettingsAccessorBase::SetSkeletonAnimationSampling(double sampling, SampleType sampleType, bool sampleTypeValid)
{
    if (!sampleTypeValid)
    {
        SetSkeletonAnimationSampleInterval(0);
        SetSkeletonAnimationSampleType(SampleType::INTERVAL);
    }
    else if (sampleType == SampleType::INTERVAL)
    {
        SetSkeletonAnimationSampleInterval(sampling);
        SetSkeletonAnimationSampleType(SampleType::INTERVAL);
    }
    else if (sampleType == SampleType::RATE)
    {
        double rate = sampling;
        double interval = 0;
        if (rate != 0)
            interval = 1 / rate;
        SetSkeletonAnimationSampleInterval(interval);
        SetSkeletonAnimationSampleType(SampleType::RATE);
    }
}

bool FinjinSceneSettingsAccessorBase::CanExportMaterial(MaterialAccessor material)
{
    if (!GetExportMaterials())
        return false;

    int disallowedMaterialCount = GetDisallowedMaterialCount();
    for (int i = 0; i < disallowedMaterialCount; i++)
    {
        auto disallowedMaterial = GetDisallowedMaterial(i);
        if (disallowedMaterial.IsValid() && disallowedMaterial.MaterialEquals(material))
            return false;
    }

    return true;
}

void FinjinSceneSettingsAccessorBase::RemoveDisallowedMaterials(MaterialAccessorSet& materials)
{
    if (!GetExportMaterials())
        materials.clear();
    else
    {
        for (auto materialIterator = materials.begin(); materialIterator != materials.end();)
        {
            auto material = *materialIterator;
            if (!CanExportMaterial(material))
                materialIterator = materials.erase(materialIterator);
            else
                ++materialIterator;
        }
    }
}

float FinjinSceneSettingsAccessorBase::GetSceneScaleValue()
{
    //Get scale
    auto scaleUnit = GetSceneScaleUnit();
    auto scale = ApplicationAccessor::GetScale(scaleUnit);

    //Multiply in the extra scaling factor
    auto scaleDivide = GetSceneScaleDivide();
    if (scaleDivide != 0)
        scale /= scaleDivide;

    return scale;
}

FinjinColor FinjinSceneSettingsAccessorBase::GetAmbientLightColorValue()
{
    FinjinColor color;

    switch (GetAmbientLightColorSync())
    {
        case ValueSourceSync::FROM_MASTER_TO_SLAVE: color = ApplicationAccessor::GetAmbientLightColor(); break;
        case ValueSourceSync::SEPARATE:
        case ValueSourceSync::FROM_SLAVE_TO_MASTER: color = GetAmbientLightColor(); break;
    }

    return color;
}

FinjinColor FinjinSceneSettingsAccessorBase::GetBackgroundColorValue()
{
    FinjinColor color;

    switch (GetBackgroundColorSync())
    {
        case ValueSourceSync::FROM_MASTER_TO_SLAVE: color = ApplicationAccessor::GetBackgroundColor(); break;
        case ValueSourceSync::SEPARATE:
        case ValueSourceSync::FROM_SLAVE_TO_MASTER: color = GetBackgroundColor(); break;
    }

    return color;
}

void FinjinSceneSettingsAccessorBase::SetAmbientLightColorValue(ValueSourceSync sync, FinjinColor color)
{
    SetAmbientLightColorSync(sync);

    switch (sync)
    {
        case ValueSourceSync::SEPARATE: SetAmbientLightColor(color); break;
        case ValueSourceSync::FROM_MASTER_TO_SLAVE: SetAmbientLightColor(ApplicationAccessor::GetAmbientLightColor()); break;
        case ValueSourceSync::FROM_SLAVE_TO_MASTER:
        {
            ApplicationAccessor::SetAmbientLightColor(color);
            SetAmbientLightColor(color);
            break;
        }
    }
}

void FinjinSceneSettingsAccessorBase::SetBackgroundColorValue(ValueSourceSync sync, FinjinColor color)
{
    SetBackgroundColorSync(sync);

    switch (sync)
    {
        case ValueSourceSync::SEPARATE: SetBackgroundColor(color); break;
        case ValueSourceSync::FROM_MASTER_TO_SLAVE: SetBackgroundColor(ApplicationAccessor::GetBackgroundColor()); break;
        case ValueSourceSync::FROM_SLAVE_TO_MASTER:
        {
            ApplicationAccessor::SetBackgroundColor(color);
            SetBackgroundColor(color);
            break;
        }
    }
}

float FinjinSceneSettingsAccessorBase::GetEnvironmentFarValue()
{
    EnvironmentFar environmentFarObject(GetEnvironmentFarObject(), GetEnvironmentFar());
    return environmentFarObject.value;
}

FogMode FinjinSceneSettingsAccessorBase::GetFogModeValue()
{
    auto fogModeValue = FogMode::NONE;

    auto mode = GetFogMode();
    if (mode == PropertyValues::FogMode::LINEAR)
        fogModeValue = FogMode::LINEAR;
    else if (mode == PropertyValues::FogMode::EXP)
        fogModeValue = FogMode::EXP;
    else if (mode == PropertyValues::FogMode::EXP2)
        fogModeValue = FogMode::EXP2;

    return fogModeValue;
}

void FinjinSceneSettingsAccessorBase::GetFogValues(wxString& mode, FinjinColor& color, float& density, float& start, float& end, float range, TimeAccessor time, bool* applicationHasLinearFog)
{
    mode = GetFogMode();
    color = GetFogColor();
    density = GetFogDensity();
    start = GetFogStart();
    end = GetFogEnd();
    if (applicationHasLinearFog != nullptr)
        *applicationHasLinearFog = false;

    if (GetFogSync() == ValueSourceSync::FROM_MASTER_TO_SLAVE)
    {
        if (ApplicationAccessor::GetLinearFog(start, end, color, (range > 0) ? range : GetEnvironmentFarValue(), time))
        {
            density = GetFogDensity();
            if (applicationHasLinearFog != nullptr)
                *applicationHasLinearFog = true;
        }
    }
}

void FinjinSceneSettingsAccessorBase::SetFogValues(ValueSourceSync sync, wxString mode, FinjinColor color, float density, float start, float end, float range, TimeAccessor time)
{
    SetFogSync(sync);
    SetFogMode(mode);
    SetFogColor(color);
    SetFogDensity(density);
    SetFogStart(start);
    SetFogEnd(end);

    switch (sync)
    {
        case ValueSourceSync::FROM_MASTER_TO_SLAVE:
        {
            if (ApplicationAccessor::GetLinearFog(start, end, color, (range > 0) ? range : GetEnvironmentFarValue(), time))
            {
                SetFogColor(color);
                SetFogStart(start);
                SetFogEnd(end);
            }
            break;
        }
        case ValueSourceSync::FROM_SLAVE_TO_MASTER:
        {
            if (mode == PropertyValues::FogMode::LINEAR)
                ApplicationAccessor::SetLinearFog(start, end, color, (range > 0) ? range : GetEnvironmentFarValue(), time);
            else if (!mode.empty())
            {
                if (range <= 0)
                    range = GetEnvironmentFarValue();
                ApplicationAccessor::SetLinearFog(-1, -1, color, (range > 0) ? range : GetEnvironmentFarValue(), time);
            }
            break;
        }
        default: break;
    }
}
