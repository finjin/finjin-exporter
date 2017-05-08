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
#include "ExporterHandler.hpp"
#include "FileUtilities.hpp"
#include "ExporterUtilities.hpp"
#include "Strings.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
bool ExporterHandler::Initialize(AssetClass assetType, const wxString& filePath, bool exportSelected, FinjinSceneSettingsAccessor sceneSettings)
{
    this->exportFilePath = filePath;
    this->exportSelected = exportSelected;
    this->sceneSettings = sceneSettings;

    //Initialize export settings
    this->sceneExportSettings.Initialize(this->sceneSettings);
    this->sceneExportSettings.MakeExportPaths(assetType, filePath);

    return true;
}

void ExporterHandler::RunExportCommands(const std::vector<wxString>& commands)
{
    FINJIN_EXPORTER_METHOD_ENTRY_FORMAT(wxT("ExporterHandler::RunExportCommands(%d)"), (int)commands.size());

    //Fill in command macros
    /*this->exportCommandMacros.exportFilePath = this->exportFilePath;
    this->exportCommandMacros.exportFileName = FileUtilities::GetFileName(this->exportFilePath);
    this->exportCommandMacros.exportDirectory = this->exportDirectoryPath;*/

    for (size_t i = 0; i < commands.size(); i++)
    {
        //Expand command macros
        auto command = commands[i];
        this->exportCommandMacros.Expand(command);

        FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Executing command: %s"), command.wx_str());

        //Execute
        wxArrayString output;
        wxArrayString errors;
        wxExecute(command, output, errors, wxEXEC_SYNC | wxEXEC_NOHIDE | wxEXEC_NODISABLE);
        if (!output.empty())
            Logger::LogResults(INFO_LOG_MESSAGE, output, Strings::OUTPUT_HEADING);
        if (!errors.empty())
            Logger::LogResults(INFO_LOG_MESSAGE, errors, Strings::ERRORS_HEADING);
    }
}

void ExporterHandler::PreparePrefabAssetReference(std::shared_ptr<FinjinSceneDocument_SceneNode> exportedNode)
{
    auto prefabName = exportedNode->prefabName;

    wxURI prefabAssetUri;
    if (prefabAssetUri.Create(exportedNode->prefabName) && !prefabAssetUri.GetScheme().empty() && !prefabAssetUri.GetPath().empty())
    {
        //User specified a URI

        if (FileUtilities::HasExtension(prefabAssetUri.GetPath()))
        {
            //It has an extension. Just use the URI directly
            auto assetRef = AssetReference::Parse(prefabName);
            if (assetRef.objectName.empty())
                assetRef.objectName = FileUtilities::GetFileNameNoExtension(prefabAssetUri.GetPath());
            exportedNode->prefabRef = [assetRef]()
            {
                return assetRef;
            };
        }
        else
        {
            //No extension
            exportedNode->prefabRef = [this, prefabName, prefabAssetUri]()
            {
                //Add extension it if it's not an 'object'
                if (prefabAssetUri.GetScheme() != wxT("object"))
                {
                    auto filePathWithoutExtension = prefabAssetUri.GetPath();

                    auto filePathWithExtension = filePathWithoutExtension;
                    this->sceneExportSettings.AppendAssetExtension(filePathWithExtension, AssetClass::PREFAB);

                    auto formattedPrefabRef = prefabName;
                    formattedPrefabRef.Replace(filePathWithoutExtension, filePathWithExtension);

                    auto assetRef = AssetReference::Parse(formattedPrefabRef);
                    if (assetRef.objectName.empty())
                        assetRef.objectName = filePathWithoutExtension;

                    return assetRef;
                }
                else
                    return AssetReference::Parse(prefabName);
            };
        }
    }
    else
    {
        if (FileUtilities::HasExtension(exportedNode->prefabName))
        {
            //Assume it's a file and format a reference to it
            exportedNode->prefabRef = [this, prefabName]()
            {
                auto objectName = FileUtilities::RemoveExtension(prefabName);
                ExporterUtilities::FixObjectName(objectName);

                return AssetReference::ForObjectInLocalFile(prefabName, objectName);
            };
        }
        else
        {
            //It's a regular name
            ExporterUtilities::FixObjectName(prefabName);
            exportedNode->prefabRef = [this, prefabName]()
            {
                return this->sceneExportSettings.GetAssetReference(AssetClass::PREFAB, prefabName);
            };
        }
    }
}
