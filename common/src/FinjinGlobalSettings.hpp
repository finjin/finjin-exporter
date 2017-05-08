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
#include "finjin/common/WxDataChunkWriter.hpp"
#include "BaseDirectory.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class FinjinSceneSettingsAccessor;

    class FinjinGlobalSettings
    {
    public:
        FinjinGlobalSettings();
        ~FinjinGlobalSettings();

        void GetSceneSettingsDefaults(FinjinSceneSettingsAccessor& sceneSettings);

        static FinjinGlobalSettings& GetInstance();

        void Save();

        bool HasFlags() const;
        void GetFlags(std::vector<wxString>& names, std::vector<int>& bits);
        void GetFlag(int i, wxString& name, int& bit);
        int GetFlagBit(const wxString& name);

        wxString GetWorkingBaseUserDataTypesDirectory() const;
        wxString GetWorkingVertexFormatFilePath() const;

        bool GetExportAnyDocumentCreationData() const;

        static const double DEFAULT_SAMPLE_INTERVAL;

    private:
        void Load();

    public:
        bool loaded;
        bool detailedLogging;
        WxDataChunkBlobTextFormat blobTextFormat;
        ByteOrder binaryByteOrder;
        bool additionalExportJsonFiles;
        bool additionalExportTextFiles;
        bool additionalExportBinaryFiles;
        bool additionalExportConfigFiles;
        bool embedMeshes;
        bool embedMaterials;
        bool embedTextures;
        bool embedPrefabs;
        bool exportAuthor;
        bool exportApplicationName;
        bool exportExporterVersion;
        bool exportExportTime;
        wxString exportDirectory;
        size_t maxBytesPerLine;
        wxString sceneScaleUnit;
        float sceneScaleDivide;
        SampleType skeletonAnimationSampleType;
        double skeletonAnimationSampleInterval;
        SampleType vertexAnimationSampleType;
        double vertexAnimationSampleInterval;
        SampleType nodeAnimationSampleType;
        double nodeAnimationSampleInterval;
        wxString submeshNaming;
        wxString submeshCustomName;

        BaseDirectory::Type baseVertexFormatDirectoryType;
        wxString baseVertexFormatDirectory;
        wxString vertexFormatPath;

        bool includeParentMaterialName;
        bool createExtraMaterials;
        wxString noMaterialName;
        bool copyBitmaps;
        bool convertBitmapsToTextures;
        wxString nodeTranslationInterpolationType;
        wxString nodeRotationInterpolationType;

        enum ExternalViewer
        {
            FINJIN_VIEWER
        };
        ExternalViewer externalViewer;

        bool runExternalViewerAfterExport;

        BaseDirectory::Type baseUserDataTypesDirectoryType;
        wxString baseUserDataTypesDirectory;
        std::vector<UserDataTypesLocation> userDataTypesLocations;

        std::vector<NamedFlag> flags;
    };

} }
