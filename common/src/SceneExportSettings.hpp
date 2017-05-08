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
#include "CoordinateSystemConverter.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "finjin/common/WxStreamingFileFormat.hpp"
#include "finjin/common/WxDataChunkWriterController.hpp"
#include "ExportableObject.hpp"
#include "GeometryStateBase.hpp"
#include "AssetClass.hpp"
#include "AssetReference.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    enum class SceneExportSettingsFlags
    {
        NONE = 0,

        /** Use the current application animation time. */
        USE_CURRENT_TIME = 1 << 1
    };
    FINJIN_ENUM_BITWISE_OPERATIONS(SceneExportSettingsFlags)

    /** Collects some common scene settings used during the export process. */
    class SceneExportSettings
    {
    public:
        SceneExportSettings();

        void Initialize(FinjinSceneSettingsAccessor sceneSettings, SceneExportSettingsFlags flags = SceneExportSettingsFlags::NONE);

        void MakeExportPaths(AssetClass defaultAssetType, const wxString& exportFilePath);

        bool HasCurrentFileFormat() const;
        void NextFileFormat();
        WxStreamingFileFormat GetCurrentFileFormat() const;
        wxString GetCurrentAssetDirectory(AssetClass assetType) const;

        wxString GetMaterialName(MaterialAccessor material) const;
        wxString GetAssetName(ExportableObject* exportableObject, GeometryStateBase* geometryState) const;
        wxString GetAssetName(AssetClass assetType, ExportableObject* exportableObject) const;
        wxString GetAssetName(AssetClass assetType, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings) const;

        struct EmbeddedAndLinkingHint
        {
            EmbeddedAndLinkingHint();
            EmbeddedAndLinkingHint(const wxString& isEmbedded);
            EmbeddedAndLinkingHint(const wxString& isEmbedded, const wxString& linkToMainObject);
            EmbeddedAndLinkingHint(bool isEmbedded, bool linkToMainObject);

            Setting<bool> isEmbedded;
            Setting<bool> linkToMainObject;
        };

        bool IsEmbeddedAssetType(AssetClass assetType, EmbeddedAndLinkingHint hint = EmbeddedAndLinkingHint()) const;

        AssetReference GetAssetReference(MaterialAccessor material, EmbeddedAndLinkingHint hint = EmbeddedAndLinkingHint()) const;
        AssetReference GetAssetReference(ExportableObject* exportableObject, GeometryStateBase* geometryState, EmbeddedAndLinkingHint hint = EmbeddedAndLinkingHint()) const;
        AssetReference GetAssetReference(AssetClass assetType, ExportableObject* exportableObject, EmbeddedAndLinkingHint hint = EmbeddedAndLinkingHint()) const;
        AssetReference GetAssetReference(AssetClass assetType, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, EmbeddedAndLinkingHint hint = EmbeddedAndLinkingHint()) const;
        AssetReference GetAssetReference(AssetClass assetType, const wxString& objectName, EmbeddedAndLinkingHint hint = EmbeddedAndLinkingHint()) const;
        AssetReference GetAssetFileReference(AssetClass assetType, const AssetPaths& fileName, EmbeddedAndLinkingHint hint = EmbeddedAndLinkingHint()) const;

        void AppendAssetExtension(wxString& s, AssetClass assetType) const;

        AssetPaths GetAssetFilePaths(AssetClass assetType, const wxString& objectName, EmbeddedAndLinkingHint hint = EmbeddedAndLinkingHint()) const;
        AssetPaths ResolveAssetPaths(AssetClass assetType, const wxString& fileName, EmbeddedAndLinkingHint hint = EmbeddedAndLinkingHint()) const;

        static bool ChunkNameToAssetType(const WxChunkName& chunkName, AssetClass& assetType);
        static WxChunkPropertyName AssetTypeToAssetReferencePropertyName(AssetClass assetType);

    public:

        struct FormatData
        {
            WxStreamingFileFormat fileFormat;

            EnumArray<AssetClass, AssetClass::COUNT, wxString> assetDirectories;
        };

        AssetClass defaultAssetType;
        StaticVector<FormatData, (size_t)WxStreamingFileFormat::COUNT> fileFormats;

        /** The export time. */
        TimeAccessor time;

        /** Converts coordinates. */
        CoordinateSystemConverter conversionManager;

        /** Scale to use when exporting. */
        float scale;

        /** The number of world units per meter, relative to the scaling factor. */
        float scaledWorldUnitsPerMeter;

        /** Name of material to use when no material is applied to a mesh. */
        wxString noMaterialName;

        bool copyBitmaps;
        bool convertBitmapsToTextures;

        StaticVector<AssetClass, (size_t)AssetClass::COUNT> embeddedAssetTypes;

        bool includeParentMaterialInName;

        ByteOrder byteOrder;

    private:
        FormatData* currentFormatData;
    };

    class SceneDataChunkWriterController : public WxDataChunkWriterController
    {
    public:
        SceneDataChunkWriterController(SceneExportSettings* sceneExportSettings);
        bool RequiresNewOutput(const WxDataChunkWriter& writer, const WxChunkName& chunkName) const override;
        std::unique_ptr<WxDocumentWriterOutput> AddOutput(WxDataChunkWriter& writer, const WxChunkName& chunkName, WxError& error) override;

    public:
        size_t objectIndex;
        SceneExportSettings* sceneExportSettings;
    };

} }
