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
#include "RaytraceMaterialExporterHandler.hpp"
#include "MaxUtilities.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyValues.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Constants--------------------------------------------------------------------
#define RAYTRACE_MTL_CLASSID Class_ID(655953908, 849023086)


//Implementation---------------------------------------------------------------
RaytraceMaterialExporterHandler::RaytraceMaterialExporterHandler()
{
    for (auto& item : this->texmaps)
        item = nullptr;
    this->texmapCount = 0;
}

bool RaytraceMaterialExporterHandler::CanHandle(MaterialAccessor material)
{
    return material.mtl != nullptr && material.mtl->ClassID() == RAYTRACE_MTL_CLASSID;
}

void RaytraceMaterialExporterHandler::CalculateRequirements()
{
    for (auto& item : this->texmaps)
        item = nullptr;
    this->texmapCount = 0;

    //Texture
    Animatable* anim;
    if (MaxUtilities::GetSubAnimByName(anim, _M("Diffuse:"), this->material.mtl, true))
    {
        auto bitmap = static_cast<BitmapTex*>(anim);
        auto isOn = true; //How to get diffuseMapEnable?
        if (isOn)
        {
            auto diffuseMap = GetDiffuseMap(bitmap);
            if (diffuseMap != nullptr)
            {
                this->texmaps[TexMapIndex::DIFFUSE].texmap = diffuseMap;
                this->texmapCount++;
            }
        }
    }
}

void RaytraceMaterialExporterHandler::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    //Type
    writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_STANDARD, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Name
    writer.WriteString(StandardAssetDocumentPropertyNames::NAME, writer.GetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME), error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    IParamBlock* pblock;
    int paramID;
    IParamBlock* isOnPBlock;
    int isOnParamID;
    
    //Ambient color
    Color ambient(0,0,0);
    if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("Ambient Color On"), this->material.mtl) &&
        isOnPBlock->GetInt(isOnParamID) && 
        MaxUtilities::GetParamIDByName(pblock, paramID, _M("Ambient"), this->material.mtl))
    {
        ambient = pblock->GetColor(paramID);
    }
    else if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("Ambient Amount"), this->material.mtl))
    {
        float amount = pblock->GetFloat(paramID);
        ambient = Color(amount, amount, amount);
    }
    {
        float colorArray[4] = {ambient.r, ambient.g, ambient.b, 1.0f};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::AMBIENT_COLOR, colorArray, 4, error);
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
    
    //Specular color
    if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("Spec. Color"), this->material.mtl))
    {
        Color specular = pblock->GetColor(paramID);
            
        float colorArray[4] = {specular.r, specular.g, specular.b, 1};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::SPECULAR_COLOR, colorArray, 4, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
    
    //Emissive color
    Color selfIllum(0,0,0);
    if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("Luminosity Color On"), this->material.mtl) &&
        isOnPBlock->GetInt(isOnParamID) && 
        MaxUtilities::GetParamIDByName(pblock, paramID, _M("Luminosity"), this->material.mtl))
    {
        selfIllum = pblock->GetColor(paramID);
    }
    else if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("Self-Illum Amount"), this->material.mtl))
    {
        float amount = pblock->GetFloat(paramID);
        selfIllum = Color(amount, amount, amount);
    }
    {
        float colorArray[4] = {selfIllum.r, selfIllum.g, selfIllum.b, 1.0f};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::SELF_ILLUMINATION_COLOR, colorArray, 4, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Shininess
    if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("Glossiness"), this->material.mtl))
    {
        float shininess = pblock->GetFloat(paramID);
            
        writer.WriteFloat(StandardAssetDocumentPropertyNames::SHININESS, shininess, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
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
