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
#include "PropertyValues.hpp"
#include "StringUtilities.hpp"
#include "GeometryStateBase.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
const wxString PropertyValues::MeshAnimationType::MORPH_ONLY(wxT("morph-only"));
const wxString PropertyValues::MeshAnimationType::COMBINED_MORPH_ONLY(wxT("combined-morph-only"));
const wxString PropertyValues::MeshAnimationType::MORPH(wxT("morph"));
const wxString PropertyValues::MeshAnimationType::POSE(wxT("pose"));

const wxString PropertyValues::Visibility::VISIBLE(wxT("visible"));
const wxString PropertyValues::Visibility::HIDDEN(wxT("hidden"));
const wxString PropertyValues::Visibility::TREE_VISIBLE(wxT("tree-visible"));
const wxString PropertyValues::Visibility::TREE_HIDDEN(wxT("tree-hidden"));

const wxString PropertyValues::ChildSorting::NAME(wxT("name"));
const wxString PropertyValues::ChildSorting::CHILD_ORDER(wxT("child-order"));

const wxString PropertyValues::SceneScale::ACTIVE(wxT("active"));
const wxString PropertyValues::SceneScale::METERS(wxT("meters"));
const wxString PropertyValues::SceneScale::KILOMETERS(wxT("kilometers"));
const wxString PropertyValues::SceneScale::FEET(wxT("feet"));
const wxString PropertyValues::SceneScale::MILES(wxT("miles"));

const wxString PropertyValues::TimeScale::NORMALIZE(wxT("normalize"));
const wxString PropertyValues::TimeScale::LENGTH(wxT("length"));

const wxString PropertyValues::FogMode::LINEAR(wxT("linear"));
const wxString PropertyValues::FogMode::EXP(wxT("exp"));
const wxString PropertyValues::FogMode::EXP2(wxT("exp2"));

const wxString PropertyValues::PrimitiveType::POINTS(wxT("points"));
const wxString PropertyValues::PrimitiveType::LINES(wxT("lines"));

const wxString PropertyValues::MeshAnimationTrack::MORPHER(wxT("morpher"));
const wxString PropertyValues::MeshAnimationTrack::PHYSIQUE(wxT("physique"));
const wxString PropertyValues::MeshAnimationTrack::SKIN(wxT("skin"));

const wxString PropertyValues::UserDataTypesLocation::ZIP(wxT("zip"));
const wxString PropertyValues::UserDataTypesLocation::DIRECTORY(wxT("directory"));
const wxString PropertyValues::UserDataTypesLocation::FILE(wxT("file"));

const wxString PropertyValues::TextureCoordinateType::NONE(wxT("none"));

wxString PropertyValues::ChooseAnimationSampleValue(SampleType sampleType, double interval)
{
    if (sampleType == SampleType::INTERVAL)
        return StringUtilities::ToString(interval);
    else if (sampleType == SampleType::RATE && interval != 0)
        return StringUtilities::ToString(1 / interval);
    else
        return wxEmptyString;
}
