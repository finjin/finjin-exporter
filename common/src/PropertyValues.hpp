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


#pragma once


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Values that are used in scene and object settings. */
    struct PropertyValues
    {
        struct MeshAnimationType
        {
            static const wxString MORPH_ONLY;
            static const wxString COMBINED_MORPH_ONLY;
            static const wxString MORPH;
            static const wxString POSE;
        };

        struct Visibility
        {
            static const wxString VISIBLE;
            static const wxString HIDDEN;
            static const wxString TREE_VISIBLE;
            static const wxString TREE_HIDDEN;
        };

        struct ChildSorting
        {
            static const wxString NAME;
            static const wxString CHILD_ORDER;
        };

        struct SceneScale
        {
            static const wxString ACTIVE;
            static const wxString METERS;
            static const wxString KILOMETERS;
            static const wxString FEET;
            static const wxString MILES;
        };

        struct TimeScale
        {
            static const wxString NORMALIZE;
            static const wxString LENGTH;
        };

        struct FogMode
        {
            static const wxString LINEAR;
            static const wxString EXP;
            static const wxString EXP2;
        };

        struct PrimitiveType
        {
            static const wxString POINTS;
            static const wxString LINES;
        };

        struct MeshAnimationTrack
        {
            static const wxString MORPHER;
            static const wxString PHYSIQUE;
            static const wxString SKIN;
        };

        struct UserDataTypesLocation
        {
            static const wxString ZIP;
            static const wxString DIRECTORY;
            static const wxString FILE;
        };

        struct TextureCoordinateType
        {
            static const wxString NONE;
        };

        static wxString ChooseAnimationSampleValue(SampleType sampleType, double interval);
    };

} }
