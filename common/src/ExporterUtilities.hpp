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


//Includes----------------------------------------------------------------------
#include "MaterialAccessor.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "SceneExportSettings.hpp"
#include "FinjinSceneDocument.hpp"
#include "FinjinGlobalSettings.hpp"
#include "finjin/common/WxStreamingFileFormat.hpp"
#include "finjin/common/WxDataChunkWriter.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class GeometryState;
    class FinjinSceneDocument;
    class FinjinSceneDocument_Clipping;
    class FinjinSceneDocument_Shadows;

    /**
     * Various utility functions used during export.
     * These methods are primarily used by the Finjin exporter handler and its associated classes.
     */
    class ExporterUtilities
    {
    public:
        static bool ConvertCopyImage(const wxString& sourceFileName, WxDataChunkWriter& writer, const WxChunkPropertyName& sizePropertyName, const WxChunkPropertyName& contentPropertyName, bool convertToTexture);
        static bool ConvertCopyImage(const wxString& sourceFileName, ByteOrder byteOrder, const wxString& destinationFileName, bool convertToTexture);

        static wxString GetMaterialName(MaterialAccessor material, const wxString& defaultName, bool includeParentMaterialName);

        static void FixObjectName(wxString& fileName);

        static wxString GetTextureName(const wxString& relativeFilePath);

        static bool GetPlane(FinjinVector4& plane, GeometryState& geomState);

        static std::unique_ptr<FinjinSceneDocument_Clipping> ExportEnvironmentRange
            (
            FinjinSceneDocument& scene,
            FinjinSceneSettingsAccessor sceneSettings,
            SceneExportSettings& sceneExportSettings
            );

        static std::unique_ptr<FinjinSceneDocument_Shadows> ExportShadows
            (
            FinjinSceneDocument& scene,
            FinjinSceneSettingsAccessor sceneSettings,
            SceneExportSettings& sceneExportSettings
            );

        template <typename T>
        static void ExportFlags(FinjinSceneDocument_Item& item, T settings)
        {
            auto& globalSettings = FinjinGlobalSettings::GetInstance();

            item.namedFlags.clear();

            int flagCount = settings.GetFlagCount();
            if (settings.GetExportFlags() && flagCount > 0)
            {
                item.namedFlags.reserve(flagCount);

                for (int i = 0; i < flagCount; i++)
                {
                    NamedFlag flag;

                    settings.GetFlag(i, flag.name);
                    if (!flag.name.empty())
                    {
                        auto bit = globalSettings.GetFlagBit(flag.name);
                        if (bit >= 0 && bit < FINJIN_EXPORTER_MAX_FLAG_BITS)
                        {
                            flag.bit = bit;
                            item.namedFlags.push_back(flag);
                        }
                    }
                }
            }
        }

        static FinjinSceneDocument_SceneNode::NodeVisibility GetNodeVisibility
            (
            const wxString& visibility,
            FinjinSceneDocument_SceneNode::NodeVisibility defaultValue = FinjinSceneDocument_SceneNode::NODE_VISIBILITY_DEFAULT
            );

        static void WriteTransformProperties(WxDataChunkWriter& writer, const TransformAccessor& transform, WxError& error);
    };

} }
