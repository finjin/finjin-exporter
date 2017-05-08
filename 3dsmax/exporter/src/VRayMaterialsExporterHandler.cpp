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
#include "VRayMaterialsExporterHandler.hpp"
#include "MaxUtilities.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyValues.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Constants---------------------------------------------------------------------
#define VRAY_FASTSSSMTL_CLASSID Class_ID(1543535727, 281901923)
#define VRAY_LIGHTMTL_CLASSID Class_ID(2093950526, 1062943161)
#define VRAY_MTL_CLASSID Class_ID(935280431, 1882483036)


//Implementation----------------------------------------------------------------
VRayMaterialsExporterHandler::VRayMaterialsExporterHandler()
{
    for (auto& item : this->texmaps)
        item = nullptr;
    this->texmapCount = 0;
}

bool VRayMaterialsExporterHandler::CanHandle(MaterialAccessor material)
{
    static const Class_ID VRAY_CLASSIDS[] =
        {
        VRAY_FASTSSSMTL_CLASSID,
        VRAY_LIGHTMTL_CLASSID,
        VRAY_MTL_CLASSID
        };

    if (material.mtl != nullptr)
    {
        for (int i = 0; i < FINJIN_COUNT_OF(VRAY_CLASSIDS); i++)
        {
            if (material.mtl->ClassID() == VRAY_CLASSIDS[i])
                return true;
        }
    }

    return false;
}

void VRayMaterialsExporterHandler::CalculateRequirements()
{
    for (auto& item : this->texmaps)
        item = nullptr;
    this->texmapCount = 0;

    int mapCount = 0;

    IParamBlock2* pblock;
    ParamID paramID;
    IParamBlock2* isOnPBlock;
    ParamID isOnParamID;

    if (this->material.mtl->ClassID() == VRAY_FASTSSSMTL_CLASSID)
    {
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("shallow_texmap"), this->material.mtl))
        {
            auto isOn = true;
            if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("shallow_texmap_on"), this->material.mtl))
                isOn = isOnPBlock->GetInt(isOnParamID) ? true : false;
            if (isOn)
            {
                auto diffuseMap = GetDiffuseMap(pblock->GetTexmap(paramID));
                if (diffuseMap != nullptr)
                {
                    this->texmaps[TexMapIndex::DIFFUSE].texmap = diffuseMap;
                    this->texmapCount++;
                }
            }
        }
        if (this->texmaps[TexMapIndex::DIFFUSE] == nullptr && MaxUtilities::GetParamIDByName(pblock, paramID, _M("deep_texmap"), this->material.mtl))
        {
            auto isOn = true;
            if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("deep_texmap_on"), this->material.mtl))
                isOn = isOnPBlock->GetInt(isOnParamID) ? true : false;
            if (isOn)
            {
                auto diffuseMap = GetDiffuseMap(pblock->GetTexmap(paramID));
                if (diffuseMap != nullptr)
                {
                    this->texmaps[TexMapIndex::DIFFUSE].texmap = diffuseMap;
                    this->texmapCount++;
                }
            }
        }
    }
    else if (this->material.mtl->ClassID() == VRAY_LIGHTMTL_CLASSID)
    {
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("texmap"), this->material.mtl))
        {
            auto isOn = true;
            if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("texmap_on"), this->material.mtl))
                isOn = isOnPBlock->GetInt(isOnParamID) ? true : false;
            if (isOn)
            {
                auto diffuseMap = GetDiffuseMap(pblock->GetTexmap(paramID));
                if (diffuseMap != nullptr)
                {
                    this->texmaps[TexMapIndex::DIFFUSE].texmap = diffuseMap;
                    this->texmapCount++;
                }
            }
        }
    }
    else if (this->material.mtl->ClassID() == VRAY_MTL_CLASSID)
    {
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("texmap_diffuse"), this->material.mtl))
        {
            auto isOn = true;
            if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("texmap_diffuse_on"), this->material.mtl))
                isOn = isOnPBlock->GetInt(isOnParamID) ? true : false;
            if (isOn)
            {
                auto diffuseMap = GetDiffuseMap(pblock->GetTexmap(paramID));
                if (diffuseMap != nullptr)
                {
                    this->texmaps[TexMapIndex::DIFFUSE].texmap = diffuseMap;
                    this->texmapCount++;
                }
            }
        }
    }
}

void VRayMaterialsExporterHandler::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    //Type
    writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_STANDARD, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Name
    writer.WriteString(StandardAssetDocumentPropertyNames::NAME, writer.GetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME), error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    IParamBlock2* pblock;
    ParamID paramID;

    if (this->material.mtl->ClassID() == VRAY_FASTSSSMTL_CLASSID)
    {
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("shallow_color"), this->material.mtl))
        {
            Color diffuse = pblock->GetColor(paramID);

            float colorArray[4] = {diffuse.r, diffuse.g, diffuse.b, 1.0f};
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray, 4, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }
    else if (this->material.mtl->ClassID() == VRAY_LIGHTMTL_CLASSID)
    {
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("color"), this->material.mtl))
        {
            Color diffuse = pblock->GetColor(paramID);

            float colorArray[4] = {diffuse.r, diffuse.g, diffuse.b, 1.0f};
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray, 4, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }
    else if (this->material.mtl->ClassID() == VRAY_MTL_CLASSID)
    {
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("diffuse"), this->material.mtl))
        {
            Color diffuse = pblock->GetColor(paramID);

            float colorArray[4] = {diffuse.r, diffuse.g, diffuse.b, 1.0f};
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray, 4, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("reflection_glossiness"), this->material.mtl))
        {
            float shininess = pblock->GetFloat(paramID);

            writer.WriteFloat(StandardAssetDocumentPropertyNames::SHININESS, shininess, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }

    //Maps
    if (this->texmapCount > 0)
    {
        writer.WriteChunk(StandardAssetDocumentChunkNames::MAPS, [this](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->texmapCount, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            size_t mapIndex = 0;

            //Diffuse
            WriteDiffuseMap(writer, mapIndex, this->texmaps[TexMapIndex::DIFFUSE], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}
