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
#include "AssetClass.hpp"
#include "FileUtilities.hpp"
#include "ExporterUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
wxString AssetClassUtilities::ToString(size_t value, bool directoryName)
{
    return ToString((AssetClass)value, directoryName);
}

wxString AssetClassUtilities::ToString(AssetClass value, bool directoryName)
{
    wxString extString;

    switch (value)
    {
        case AssetClass::MATERIAL: extString = directoryName ? wxT("materials") : wxT("material"); break;
        case AssetClass::MESH: extString = directoryName ? wxT("meshes") : wxT("mesh"); break;
        case AssetClass::MORPH_ANIMATION: extString = directoryName ? wxT("morphanims") : wxT("morphanim"); break;
        case AssetClass::NODE_ANIMATION: extString = directoryName ? wxT("nodeanims") : wxT("nodeanim"); break;
        case AssetClass::POSE_ANIMATION: extString = directoryName ? wxT("poseanims") : wxT("poseanim"); break;
        case AssetClass::PREFAB: extString = directoryName ? wxT("prefabs") : wxT("prefab"); break;
        case AssetClass::SCENE: extString = directoryName ? wxT("scenes") : wxT("scene"); break;
        case AssetClass::SKELETON: extString = directoryName ? wxT("skeletons") : wxT("skeleton"); break;
        case AssetClass::SKELETON_ANIMATION: extString = directoryName ? wxT("skeletonanims") : wxT("skeletonanim"); break;
        case AssetClass::SOUND: extString = directoryName ? wxT("sounds") : wxT("sound"); break;
        case AssetClass::TEXTURE: extString = directoryName ? wxT("textures") : wxT("texture"); break;
        default: break;
    }

    return extString;
}

wxString AssetClassUtilities::CreateFileName(AssetClass assetClass, const wxString& assetName, WxStreamingFileFormat extensionFormat)
{
    //File base name
    auto fileName = assetName;
    ExporterUtilities::FixObjectName(fileName);

    //Add asset extension
    auto assetTypeExtension = ToString(assetClass);
    wxString assetTypeAndFileTypeExtension = wxT(".");
    assetTypeAndFileTypeExtension += WxStreamingFileFormatUtilities::MakeExtension(assetTypeExtension, extensionFormat);

    //Finish
    return FileUtilities::CreateFileName(fileName, assetTypeAndFileTypeExtension);
}
