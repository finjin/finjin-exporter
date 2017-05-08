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

    /** Strings that appear within the user interface (dialogs, controls, and so on). */
    class Strings
    {
    public:
        static const wxString FINJIN;
        static const wxString SCENE_SETTINGS;

        static const wxString ALL;
        static const wxString NONE;
        static const wxString DEFAULT;
        static const wxString ID;
        static const wxString VALUE;

        static const wxString FLAG_NAME_REQUIRED;
        static const wxString FLAG_NAME_CANT_BE_INTEGER;
        static const wxString UNIQUE_FLAG_NAME_REQUIRED;
        static const wxString UNIQUE_FLAG_BIT_REQUIRED;
        static const wxString DATA_ENTRY_ERROR;

        static const wxString CHOOSE_DIRECTORY;
        static const wxString CHOOSE_FILE;

        static const wxString TEXTURE_FILE_EXTENSION;
        static const wxString USER_DATA_TYPES_FILE_WILDCARD;
        static const wxString USER_DATA_TYPES_FILE_EXTENSION;
        static const wxString USER_DATA_TYPES_FILE_FILTER;
        static const wxString CUSTOM_HELPER_ICON_FILE_FILTER;
        static const wxString MATERIAL_FILE_FILTER;
        static const wxString DDS_FILE_FILTER;
        static const wxString FILE_FILTER_FORMAT;
        static const wxString CFG_FILE_FILTER;
        static const wxString ALL_FILE_FILTER;

        static const wxString DEFAULT_FORMAT;

        static const wxString FROM_FIELD_REQUIRED;
        static const wxString TO_FIELD_REQUIRED;

        static const wxString RENAME_BITMAP;
        static const wxString RENAME_BITMAP_EXTENSION;

        static const wxString ADD_FLAG_ALIAS;

        static const wxString ADD_CUSTOM_PARAMETER;
        static const wxString EDIT_CUSTOM_PARAMETER;

        static const wxString NAME;
        static const wxString TRACK;
        static const wxString FRAME;
        static const wxString FRAMES;
        static const wxString TIME;
        static const wxString LENGTH;
        static const wxString SAMPLE_INTERVAL_RATE;
        static const wxString LOOPED;

        static const wxString ADD_NODE_ANIMATION;
        static const wxString EDIT_NODE_ANIMATION;

        static const wxString ADD_MESH_ANIMATION;
        static const wxString EDIT_MESH_ANIMATION;

        static const wxString YES;
        static const wxString NO;

        static const wxString ANIMATION_NAME_REQUIRED;
        static const wxString ANIMATION_NAME_ALREADY_DEFINED;
        static const wxString START_FRAME_MUST_BE_LESS_THAN_END_FRAME;

        static const wxString OBJECT_REQUIRED;
        static const wxString BONE_REQUIRED;
        static const wxString REMOVE_TRANSLATION_COMPONENT_REQUIRED;

        static const wxString DISTANCE;
        static const wxString OBJECT;

        static const wxString ADD_MANUAL_LOD;
        static const wxString EDIT_MANUAL_LOD;

        static const wxChar* SCENE_SAMPLE_TYPES_TEXT[];
        static const SampleType SCENE_SAMPLE_TYPES_VALUE[];
        static const int SCENE_SAMPLE_TYPE_COUNT;

        static const wxChar* OBJECT_SAMPLE_TYPES_TEXT[];
        static const SampleType OBJECT_SAMPLE_TYPES_VALUE[];
        static const int OBJECT_SAMPLE_TYPE_COUNT;

        static const wxChar* STANDARD_SCENE_SCALES_TEXT[];
        static const wxChar* STANDARD_SCENE_SCALES_VALUE[];
        static const int STANDARD_SCENE_SCALE_COUNT;

        static const wxChar* SKELETON_REFERENCE_POSE_TEXT[];
        static const wxChar* SKELETON_REFERENCE_POSE_VALUE[];
        static const int SKELETON_REFERENCE_POSE_COUNT;

        static const wxString POSE;
        static const wxString MORPH;
        static const wxString COMBINED_MORPH;
        static const wxString SKELETON;
        static const wxString SKELETON_AND_POSE;
        static const wxString SKELETON_AND_MORPH;

        static const wxString SAVE_AS;

        static const wxString COMPANY_NAME_INFO;
        static const wxString COPYRIGHT_INFO;

        static const wxString MORPHER;
        static const wxString PHYSIQUE;
        static const wxString SKIN;
        static const wxString SHAPE;

        static const wxString OBJECT_SETTINGS_DIALOG_TITLE_FORMAT;
        static const wxString SCENE_SETTINGS_DIALOG_TITLE;
        static const wxString USER_DATA_SETTINGS_DIALOG_TITLE_FORMAT;

        static const wxString NO_MATERIAL;
        static const wxString NO_MATERIAL_NAME;

        static const wxString EXPORT_FINISHED;
        static const wxString ERROR_RUNNING_EXPORTER;
        static const wxString ERROR_INITIALIZING_EXPORTER;
        static const wxString EXPORTING_MATERIALS;
        static const wxString EXPORTING_SCENE;

        static const wxString LESS_THAN_A_SECOND;
        static const wxString ONE_SECOND;
        static const wxString INT_SECONDS_FORMAT;
        static const wxString FLOAT_SECONDS_FORMAT;
        static const wxString FLOAT_MINUTES_FORMAT;
        static const wxString STARTED_EXPORT_ON_FORMAT;
        static const wxString FINISHED_EXPORT_ON_ELAPSED_TIME_FORMAT;

        static const wxString COULD_NOT_COPY_IMAGE_FILE_ERROR_CODEC_FORMAT;
        static const wxString COULD_NOT_COPY_IMAGE_FILE_ERROR_SOURCE_FORMAT;
        static const wxString COULD_NOT_COPY_IMAGE_FILE_ERROR_DESTINATION_PERMISSIONS_FORMAT;

        static const wxString OBJECT_SETTINGS_ERROR_NO_SELECTED_OBJECT;
        static const wxString OBJECT_SETTINGS_ERROR_CAPTION;

        static const wxString SELECT_IMAGE_FILE;
        static const wxString ALL_IMAGE_FILES;
        static const wxString FILES;

        static const wxString FINJIN_ERROR;
        static const wxString UNKNOWN_ERROR;
        static const wxString INVALID_RESOURCE_LOCATION;
        static const wxString RESOURCE_LOCATION_NOT_FOUND_FORMAT;
        static const wxString RESOURCE_LOCATION_CORRECTION_INSTRUCTIONS;

        static const wxString VERTEX_FORMAT_LOCATION_NOT_FOUND_FORMAT;
        static const wxString VERTEX_FORMAT_LOCATION_CORRECTION_INSTRUCTIONS;

        static const wxString AT_LEAST_ONE_DIRECTORY_REQUIRED;

        static const wxString OUTPUT_HEADING;
        static const wxString ERRORS_HEADING;

        static const wxString NO_EXPORTED_FILE_TO_VIEW;
        static const wxString NO_EXTERNAL_VIEWER_CONFIGURED;
        static const wxString FINJIN_VIEWER_ERROR;
        static const wxString RUNNING_EXTERNAL_VIEWER_FORMAT;

        static const wxString RENDERING_CUBE_MAP_FORMAT;

        static const wxString EXPORT_MATERIAL_ERROR_CAPTION;
        static const wxString EXPORT_MATERIAL_ERROR_NO_SELECTED_MATERIAL_FORMAT;
        static const wxString EXPORT_MATERIAL_ERROR_NO_SCENE_MATERIALS;

        static const wxString FINJIN_MATERIAL_EXPORTER;

        static const wxString SUBMESH_TEXTURE_COORDINATE_SETS_NOT_SAME_FOR_EDIT;

        static const wxString SECOND_CHARACTER_LOWER;
        static const wxString SAMPLE_LOWER;
        static const wxString SAMPLES_LOWER;

        static const wxString ADD_FLAG;

        static const wxString PROGRESS;
        static const wxString TOTAL_PROGRESS;

        static const wxString PROJECT;
        static const wxString CUSTOM;

        static const wxString ENTERING;
        static const wxString LEAVING;

        static const wxString NO_OBJECTS_HAVE_DUPLICATE_NAMES;
        static const wxString SOME_OBJECTS_HAVE_DUPLICATE_NAMES_FORMAT;
        static const wxString DUPLICATE_OBJECT_NAME_CHECK;

        static const wxString EXPORT_PROGRESS_DIALOG_TITLE_FORMAT;

        static const wxString EXPORT_COMMAND_MACRO_NO_VALUE_FOUND_FOR_SEQUENCE_FORMAT;

        static const wxString EXPORTING_FILE_FORMAT_FORMAT;

        static const wxString PHYSIQUE_DATA_CANNOT_BE_EXPORTED;
        static const wxString ENCOUNTERED_CHUNK_WITHOUT_OBJECT_NAME_FORMAT;

        static wxString FormatTime(wxDateTime time);
        static wxString FormatElapsedTime(wxTimeSpan elapsedTime);
    };

} }
