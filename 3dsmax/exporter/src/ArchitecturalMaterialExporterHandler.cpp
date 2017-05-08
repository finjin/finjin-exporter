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
#include "ArchitecturalMaterialExporterHandler.hpp"
#include "MaxUtilities.hpp"
#include "MathUtilities.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyValues.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Constants---------------------------------------------------------------------
#define ARCH_MTL_CLASSID Class_ID(332471230, 1763586103)


//Implementation----------------------------------------------------------------
ArchitecturalMaterialExporterHandler::ArchitecturalMaterialExporterHandler()
{
    for (auto& item : this->texmaps)
        item = nullptr;
    this->texmapCount = 0;
}

bool ArchitecturalMaterialExporterHandler::CanHandle(MaterialAccessor material)
{
    return material.mtl != nullptr && material.mtl->ClassID() == ARCH_MTL_CLASSID;
}

void ArchitecturalMaterialExporterHandler::CalculateRequirements()
{
    for (auto& item : this->texmaps)
        item = nullptr;
    this->texmapCount = 0;

    IParamBlock2* pblock;
    ParamID paramID;
    IParamBlock2* isOnPBlock;
    ParamID isOnParamID;

    //Diffuse texture
    if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("diffuseMap"), this->material.mtl))
    {
        auto isOn = true;
        if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("diffuseMapEnable"), this->material.mtl))
            isOn = isOnPBlock->GetInt(isOnParamID) ? true : false;
        if (isOn)
        {
            auto diffuseMap = pblock->GetTexmap(paramID);
            if (diffuseMap != nullptr)
            {
                this->texmaps[TexMapIndex::DIFFUSE].texmap = diffuseMap;
                this->texmapCount++;
            }
        }
    }

    //Intensity
    if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("intensityMap"), this->material.mtl))
    {
        auto isOn = true;
        if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("intensityMapEnable"), this->material.mtl))
            isOn = isOnPBlock->GetInt(isOnParamID) ? true : false;
        if (isOn)
        {
            auto intensityMap = GetOpacityMap(pblock->GetTexmap(paramID));
            if (intensityMap != nullptr)
            {
                this->texmaps[TexMapIndex::INTENSITY].texmap = intensityMap;
                this->texmapCount++;
            }
        }
    }

    //Self illumination
    if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("luminanceMap"), this->material.mtl))
    {
        auto isOn = true;
        if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("luminanceMapEnable"), this->material.mtl))
            isOn = isOnPBlock->GetInt(isOnParamID) ? true : false;
        if (isOn)
        {
            auto selfIlluminationMap = GetSelfIlluminationMap(pblock->GetTexmap(paramID));
            if (selfIlluminationMap != nullptr)
            {
                this->texmaps[TexMapIndex::SELF_ILLUMINATION].texmap = selfIlluminationMap;
                this->texmapCount++;
            }
        }
    }

    //Specular
    if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("shininessMap"), this->material.mtl))
    {
        auto isOn = true;
        if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("shininessMapEnable"), this->material.mtl))
            isOn = isOnPBlock->GetInt(isOnParamID) ? true : false;
        if (isOn)
        {
            auto shininessMap = GetSpecularMap(pblock->GetTexmap(paramID));
            if (shininessMap != nullptr)
            {
                this->texmaps[TexMapIndex::SPECULAR].texmap = shininessMap;
                this->texmapCount++;
            }
        }
    }
}

void ArchitecturalMaterialExporterHandler::Write(WxDataChunkWriter& writer, WxError& error)
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
    IParamBlock2* isOnPBlock;
    ParamID isOnParamID;

    //Ambient color
    {
        Color ambient(0, 0, 0);

        float colorArray[4] = {ambient.r, ambient.g, ambient.b, 1.0f};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::AMBIENT_COLOR, colorArray, 4, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Culling
    bool isTwoSided = false;
    if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("twoSided"), this->material.mtl))
        isTwoSided = isOnPBlock->GetInt(isOnParamID) ? true : false;
    if (isTwoSided)
    {
        writer.WriteString(StandardAssetDocumentPropertyNames::CULL_MODE, wxT("none"), error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Diffuse color
    if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("Diffuse"), this->material.mtl))
    {
        Color diffuse = pblock->GetColor(paramID);

        float colorArray[4] = {diffuse.r, diffuse.g, diffuse.b, 1.0f};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray, 4, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Transparency
    if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("transparency"), this->material.mtl))
    {
        float opacity = 1 - pblock->GetFloat(paramID);
        auto transparencyEnabled = !MathUtilities::AlmostOne(opacity);
        if (transparencyEnabled)
        {
            writer.WriteFloat(StandardAssetDocumentPropertyNames::OPACITY, opacity, error);
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

            //Intensity
            WriteOpacityMap(writer, mapIndex, this->texmaps[TexMapIndex::INTENSITY], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Self illumination
            WriteSelfIlluminationMap(writer, mapIndex, this->texmaps[TexMapIndex::SELF_ILLUMINATION], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Specular
            WriteSpecularMap(writer, mapIndex, this->texmaps[TexMapIndex::SPECULAR], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}
