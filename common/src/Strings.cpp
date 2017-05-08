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
#include "Strings.hpp"

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
const wxString Strings::FINJIN(wxT("Finjin"));
const wxString Strings::SCENE_SETTINGS(wxT("Scene Settings"));

const wxString Strings::ALL(wxT("All"));
const wxString Strings::NONE(wxT("None"));
const wxString Strings::DEFAULT(wxT("Default"));
const wxString Strings::ID(wxT("ID"));
const wxString Strings::VALUE(wxT("Value"));

const wxString Strings::FLAG_NAME_REQUIRED(wxT("You must enter a flag name."));
const wxString Strings::FLAG_NAME_CANT_BE_INTEGER(wxT("The flag name cannot be a number. You must add at least one non-numeric character."));
const wxString Strings::UNIQUE_FLAG_NAME_REQUIRED(wxT("The flag name you have entered is already in use. Please enter a unique name."));
const wxString Strings::UNIQUE_FLAG_BIT_REQUIRED(wxT("The flag bit you have selected is already in use. Please select a unique bit."));
const wxString Strings::DATA_ENTRY_ERROR(wxT("Data Entry Error"));

const wxString Strings::CHOOSE_DIRECTORY(wxT("Choose Directory"));
const wxString Strings::CHOOSE_FILE(wxT("Choose File"));

const wxString Strings::TEXTURE_FILE_EXTENSION(wxT("texture"));
const wxString Strings::USER_DATA_TYPES_FILE_WILDCARD(wxT("*.CFG-USERDATATYPES"));
const wxString Strings::USER_DATA_TYPES_FILE_EXTENSION(wxT("cfg-userdatatypes"));
const wxString Strings::USER_DATA_TYPES_FILE_FILTER(wxT("User Data Types Files (*.cfg-userdatatypes)|*.cfg-userdatatypes"));
const wxString Strings::CUSTOM_HELPER_ICON_FILE_FILTER(wxT("Finjin Custom Helper Icon (*.chi)|*.chi"));
const wxString Strings::MATERIAL_FILE_FILTER(wxT("Finjin Streaming Text Material File (*.fstd-material)|*.fstd-material|Finjin Streaming Binary Material File (*.fsbd-material)|*.fsbd-material|Finjin Streaming JSON Material File (*.json-material)|*.json-material|Finjin Config Material File (*.cfg-material)|*.cfg-material"));
const wxString Strings::DDS_FILE_FILTER(wxT("DDS Files (*.dds)|*.dds"));
const wxString Strings::FILE_FILTER_FORMAT(wxT("%s Files (*.%s)|*.%s"));
const wxString Strings::CFG_FILE_FILTER(wxT("Config Files (*.cfg)|*.cfg"));
const wxString Strings::ALL_FILE_FILTER(wxT("All Files (*.*)|*.*"));

const wxString Strings::DEFAULT_FORMAT(wxT("(Default: %s)"));

const wxString Strings::FROM_FIELD_REQUIRED(wxT("\"From\" field is required"));
const wxString Strings::TO_FIELD_REQUIRED(wxT("\"To\" field is required"));

const wxString Strings::RENAME_BITMAP(wxT("Rename Bitmap"));
const wxString Strings::RENAME_BITMAP_EXTENSION(wxT("Rename Bitmap Extension"));

const wxString Strings::ADD_FLAG(wxT("Add Flag"));

const wxString Strings::ADD_CUSTOM_PARAMETER(wxT("Add Custom Parameter"));
const wxString Strings::EDIT_CUSTOM_PARAMETER(wxT("Edit Custom Parameter"));

const wxString Strings::NAME(wxT("Name"));
const wxString Strings::TRACK(wxT("Track"));
const wxString Strings::FRAME(wxT("Frame"));
const wxString Strings::FRAMES(wxT("Frames"));
const wxString Strings::TIME(wxT("Time"));
const wxString Strings::LENGTH(wxT("Length"));
const wxString Strings::SAMPLE_INTERVAL_RATE(wxT("Sampling"));
const wxString Strings::LOOPED(wxT("Looped"));

const wxString Strings::ADD_NODE_ANIMATION(wxT("Add Node Animation"));
const wxString Strings::EDIT_NODE_ANIMATION(wxT("Edit Node Animation"));

const wxString Strings::ADD_MESH_ANIMATION(wxT("Add Mesh Animation"));
const wxString Strings::EDIT_MESH_ANIMATION(wxT("Edit Mesh Animation"));

const wxString Strings::YES(wxT("Yes"));
const wxString Strings::NO(wxT("No"));

const wxString Strings::ANIMATION_NAME_REQUIRED(wxT("Animation name required."));
const wxString Strings::ANIMATION_NAME_ALREADY_DEFINED(wxT("There is already an animation with this name. Please enter a unique name."));
const wxString Strings::START_FRAME_MUST_BE_LESS_THAN_END_FRAME(wxT("The start frame must be less than the end frame."));

const wxString Strings::OBJECT_REQUIRED(wxT("You must choose an object."));
const wxString Strings::BONE_REQUIRED(wxT("You must choose a bone."));
const wxString Strings::REMOVE_TRANSLATION_COMPONENT_REQUIRED(wxT("You must check at least one translation component to remove."));

const wxString Strings::DISTANCE(wxT("Distance"));
const wxString Strings::OBJECT(wxT("Object"));

const wxString Strings::ADD_MANUAL_LOD(wxT("Add Manual LOD"));
const wxString Strings::EDIT_MANUAL_LOD(wxT("Edit Manual LOD"));

const wxChar* Strings::SCENE_SAMPLE_TYPES_TEXT[] = {wxT("Use Global Setting"), wxT("Seconds Between Samples"), wxT("Samples Per Second")};
const SampleType Strings::SCENE_SAMPLE_TYPES_VALUE[] = {SampleType::USE_PARENT, SampleType::INTERVAL, SampleType::RATE};
const int Strings::SCENE_SAMPLE_TYPE_COUNT = FINJIN_COUNT_OF(Strings::SCENE_SAMPLE_TYPES_VALUE);

const wxChar* Strings::OBJECT_SAMPLE_TYPES_TEXT[] = {wxT("Use Scene Setting"), wxT("Seconds Between Samples"), wxT("Samples Per Second")};
const SampleType Strings::OBJECT_SAMPLE_TYPES_VALUE[] = {SampleType::USE_PARENT, SampleType::INTERVAL, SampleType::RATE};
const int Strings::OBJECT_SAMPLE_TYPE_COUNT = FINJIN_COUNT_OF(Strings::OBJECT_SAMPLE_TYPES_VALUE);

const wxChar* Strings::STANDARD_SCENE_SCALES_TEXT[] = {wxT("Active"), wxT("Meters"), wxT("Kilometers"), wxT("Feet"), wxT("Miles")};
const wxChar* Strings::STANDARD_SCENE_SCALES_VALUE[] = {wxT(""), wxT("meters"), wxT("kilometers"), wxT("feet"), wxT("miles")};
const int Strings::STANDARD_SCENE_SCALE_COUNT = FINJIN_COUNT_OF(STANDARD_SCENE_SCALES_VALUE);

const wxChar* Strings::SKELETON_REFERENCE_POSE_TEXT[] = {wxT("Bind Pose"), wxT("Time")};
const wxChar* Strings::SKELETON_REFERENCE_POSE_VALUE[] = {wxT(""), wxT("time")};
const int Strings::SKELETON_REFERENCE_POSE_COUNT = FINJIN_COUNT_OF(SKELETON_REFERENCE_POSE_VALUE);

const wxString Strings::POSE(wxT("Pose"));
const wxString Strings::MORPH(wxT("Morph"));
const wxString Strings::COMBINED_MORPH(wxT("Combined Morph"));
const wxString Strings::SKELETON(wxT("Skeleton"));
const wxString Strings::SKELETON_AND_POSE(wxT("Skeleton + Pose"));
const wxString Strings::SKELETON_AND_MORPH(wxT("Skeleton + Morph"));

const wxString Strings::SAVE_AS(wxT("Save As"));

const wxString Strings::COMPANY_NAME_INFO(wxT("Finjin"));
const wxString Strings::COPYRIGHT_INFO(wxT("Copyright 2016 Finjin"));

const wxString Strings::MORPHER(wxT("Morpher"));
const wxString Strings::PHYSIQUE(wxT("Physique"));
const wxString Strings::SKIN(wxT("Skin"));
const wxString Strings::SHAPE(wxT("Shape"));

const wxString Strings::OBJECT_SETTINGS_DIALOG_TITLE_FORMAT(wxT("Finjin Object Settings - %s"));
const wxString Strings::SCENE_SETTINGS_DIALOG_TITLE(wxT("Finjin Scene Settings"));
const wxString Strings::USER_DATA_SETTINGS_DIALOG_TITLE_FORMAT(wxT("Finjin User Data Settings - %s"));

const wxString Strings::NO_MATERIAL(wxT("No Material"));
const wxString Strings::NO_MATERIAL_NAME(wxT("no-material"));

const wxString Strings::EXPORT_FINISHED(wxT("Export finished"));
const wxString Strings::ERROR_RUNNING_EXPORTER(wxT("Error running exporter"));
const wxString Strings::ERROR_INITIALIZING_EXPORTER(wxT("Error initializing exporter"));
const wxString Strings::EXPORTING_MATERIALS(wxT("Exporting materials"));
const wxString Strings::EXPORTING_SCENE(wxT("Exporting scene"));

const wxString Strings::LESS_THAN_A_SECOND(wxT("< 1 second"));
const wxString Strings::ONE_SECOND(wxT("1 second"));
const wxString Strings::INT_SECONDS_FORMAT(wxT("%d seconds"));
const wxString Strings::FLOAT_SECONDS_FORMAT(wxT("%.3f seconds"));
const wxString Strings::FLOAT_MINUTES_FORMAT(wxT("%.2f minutes"));
const wxString Strings::STARTED_EXPORT_ON_FORMAT(wxT("Started export on %s"));
const wxString Strings::FINISHED_EXPORT_ON_ELAPSED_TIME_FORMAT(wxT("Finished export on %s. Total time: %s"));

const wxString Strings::COULD_NOT_COPY_IMAGE_FILE_ERROR_CODEC_FORMAT(wxT("Could not copy image file from '%s' to '%s'. Codec not supported."));
const wxString Strings::COULD_NOT_COPY_IMAGE_FILE_ERROR_SOURCE_FORMAT(wxT("Could not copy image file '%s'. File does not exist."));
const wxString Strings::COULD_NOT_COPY_IMAGE_FILE_ERROR_DESTINATION_PERMISSIONS_FORMAT(wxT("Could not copy image file from '%s' to '%s'. Destination file exists already and is read-only."));

const wxString Strings::OBJECT_SETTINGS_ERROR_NO_SELECTED_OBJECT(wxT("No valid object is currently selected.\n\nYou must select a valid object before editing its settings."));
const wxString Strings::OBJECT_SETTINGS_ERROR_CAPTION(wxT("Finjin Object Settings Error"));

const wxString Strings::SELECT_IMAGE_FILE(wxT("Select Image File"));
const wxString Strings::ALL_IMAGE_FILES(wxT("All Image Files"));
const wxString Strings::FILES(wxT("Files"));

const wxString Strings::FINJIN_ERROR(wxT("Finjin Error"));
const wxString Strings::UNKNOWN_ERROR(wxT("Unknown error"));
const wxString Strings::INVALID_RESOURCE_LOCATION(wxT("Finjin Invalid Resource Locations"));
const wxString Strings::RESOURCE_LOCATION_NOT_FOUND_FORMAT(wxT("The following resource locations could not be found:\n\n"));
const wxString Strings::RESOURCE_LOCATION_CORRECTION_INSTRUCTIONS(wxT("Please verify that the locations you have entered are correct."));

const wxString Strings::VERTEX_FORMAT_LOCATION_NOT_FOUND_FORMAT(wxT("The following vertex format location could not be found:\n\n"));
const wxString Strings::VERTEX_FORMAT_LOCATION_CORRECTION_INSTRUCTIONS(wxT("Please verify that the location you entered is correct."));

const wxString Strings::AT_LEAST_ONE_DIRECTORY_REQUIRED(wxT("At least one directory is required."));

const wxString Strings::OUTPUT_HEADING(wxT("Output:"));
const wxString Strings::ERRORS_HEADING(wxT("Errors:"));

const wxString Strings::NO_EXPORTED_FILE_TO_VIEW(wxT("Nothing has been exported, so there is no file to view."));
const wxString Strings::NO_EXTERNAL_VIEWER_CONFIGURED(wxT("No external viewer is currently configured.\n\nTry configuring a viewer in the Finjin Global Settings dialog."));
const wxString Strings::FINJIN_VIEWER_ERROR(wxT("Finjin Viewer Error"));
const wxString Strings::RUNNING_EXTERNAL_VIEWER_FORMAT(wxT("Running external viewer (%s) with command: %s"));

const wxString Strings::RENDERING_CUBE_MAP_FORMAT(wxT("Rendering cube map to %s"));

const wxString Strings::EXPORT_MATERIAL_ERROR_CAPTION(wxT("Finjin Material Export Error"));
const wxString Strings::EXPORT_MATERIAL_ERROR_NO_SELECTED_MATERIAL_FORMAT(wxT("No material is currently selected.\n\nTry opening the %s window and selecting a material."));
const wxString Strings::EXPORT_MATERIAL_ERROR_NO_SCENE_MATERIALS(wxT("No scene materials are present."));

const wxString Strings::FINJIN_MATERIAL_EXPORTER(wxT("Finjin Material Exporter"));

const wxString Strings::SUBMESH_TEXTURE_COORDINATE_SETS_NOT_SAME_FOR_EDIT(wxT("Texture coordinate sets must be the same among all submeshes in order to edit them at the same time."));

const wxString Strings::SECOND_CHARACTER_LOWER(wxT("s"));
const wxString Strings::SAMPLE_LOWER(wxT("sample"));
const wxString Strings::SAMPLES_LOWER(wxT("samples"));

const wxString Strings::ADD_FLAG_ALIAS(wxT("Add Flag Alias"));

const wxString Strings::PROGRESS(wxT("Progress"));
const wxString Strings::TOTAL_PROGRESS(wxT("Total progress"));

const wxString Strings::PROJECT(wxT("Project"));
const wxString Strings::CUSTOM(wxT("Custom"));

const wxString Strings::ENTERING(wxT("Entering"));
const wxString Strings::LEAVING(wxT("Leaving"));

const wxString Strings::NO_OBJECTS_HAVE_DUPLICATE_NAMES(wxT("No objects have duplicate names."));
const wxString Strings::SOME_OBJECTS_HAVE_DUPLICATE_NAMES_FORMAT(wxT("The following duplicate object names were found:\n\n%s\nWould you like to select the object(s) with duplicate names?"));
const wxString Strings::DUPLICATE_OBJECT_NAME_CHECK(wxT("Duplicate Object Name Check"));

const wxString Strings::EXPORT_PROGRESS_DIALOG_TITLE_FORMAT(wxT("%s Exporter"));

const wxString Strings::EXPORT_COMMAND_MACRO_NO_VALUE_FOUND_FOR_SEQUENCE_FORMAT(wxT("Export command macro: No value was found for the escaped sequence '%s'"));

const wxString Strings::EXPORTING_FILE_FORMAT_FORMAT(wxT("Exporting '%s' format files-----------------"));

const wxString Strings::PHYSIQUE_DATA_CANNOT_BE_EXPORTED(wxT("Physique data cannot be exported for this object, probably because it is not a mesh. Try performing a 'Convert to Mesh' before applying the Physique modifier."));
const wxString Strings::ENCOUNTERED_CHUNK_WITHOUT_OBJECT_NAME_FORMAT(wxT("Unexpectedly encountered object without a data chunk writer object name. Using '%s' instead."));


//Implementation----------------------------------------------------------------
wxString Strings::FormatTime(wxDateTime time)
{
    //Example: August 24, 2008 at 12:34:56 PM
    static const wxChar* TIME_FORMAT = wxT("%B %d, %Y at %I:%M:%S %p");
    return time.Format(TIME_FORMAT);
}

wxString Strings::FormatElapsedTime(wxTimeSpan elapsedTime)
{
    wxString elapsedTimeText;

    float elapsedMinutes = elapsedTime.GetMinutes();
    if (elapsedMinutes == 0)
    {
        //Less than a minute
        unsigned int seconds = elapsedTime.GetSeconds().GetLo();
        if (seconds == 0)
            elapsedTimeText = LESS_THAN_A_SECOND;
        else if (seconds == 1)
            elapsedTimeText = ONE_SECOND;
        else
            elapsedTimeText = wxString::Format(INT_SECONDS_FORMAT, (int)seconds);
    }
    else
    {
        //Subtract out the whole minutes and add the fractional minutes
        elapsedTime -= wxTimeSpan(0, elapsedTime.GetMinutes());
        elapsedMinutes += elapsedTime.GetSeconds().GetLo()/60.0f;

        //Format as floating point minutes
        elapsedTimeText = wxString::Format(FLOAT_MINUTES_FORMAT, elapsedMinutes);
    }

    return elapsedTimeText;
}
