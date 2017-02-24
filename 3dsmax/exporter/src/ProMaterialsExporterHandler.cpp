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
#include "ProMaterialsExporterHandler.hpp"
#include "MaxUtilities.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyValues.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Constants--------------------------------------------------------------------
#define PRO_PLASTIC_VINYL_MTL_CLASSID Class_ID(2004030991, -1033983941)
#define PRO_METALLIC_PAINT_MTL_CLASSID Class_ID(2004030991, -1184796640)
#define PRO_METAL_MTL_CLASSID Class_ID(2004030991, -1185657837)
#define PRO_SOLID_GLASS_MTL_CLASSID Class_ID(2004030991, 1297002876)
#define PRO_MIRROR_MTL_CLASSID Class_ID(2004030991, -1156690138) 
#define PRO_GENERIC_MTL_CLASSID Class_ID(2004030991, -1520410832)


//Implementation---------------------------------------------------------------
ProMaterialsExporterHandler::ProMaterialsExporterHandler()
{
    for (auto& item : this->texmaps)
        item = nullptr;
    this->texmapCount = 0;
}

bool ProMaterialsExporterHandler::CanHandle(MaterialAccessor material)
{
    static const Class_ID PRO_MTL_CLASSIDS[] =
        {
        PRO_PLASTIC_VINYL_MTL_CLASSID,
        PRO_METALLIC_PAINT_MTL_CLASSID,
        PRO_METAL_MTL_CLASSID,
        PRO_SOLID_GLASS_MTL_CLASSID,
        PRO_MIRROR_MTL_CLASSID,
        PRO_GENERIC_MTL_CLASSID
        };

    if (material.mtl != nullptr)
    {
        for (int i = 0; i < FINJIN_COUNT_OF(PRO_MTL_CLASSIDS); i++)
        {
            if (material.mtl->ClassID() == PRO_MTL_CLASSIDS[i])
                return true;
        }        
    }
    
    return false;
}

void ProMaterialsExporterHandler::CalculateRequirements()
{
    for (auto& item : this->texmaps)
        item = nullptr;
    this->texmapCount = 0;
    
    IParamBlock2* pblock;
    ParamID paramID;
    IParamBlock2* isOnPBlock;
    ParamID isOnParamID;

    if (this->material.mtl->ClassID() == PRO_PLASTIC_VINYL_MTL_CLASSID ||
        this->material.mtl->ClassID() == PRO_METALLIC_PAINT_MTL_CLASSID)
    {
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("diffuse_color.shader"), this->material.mtl))
        {
            auto isOn = true;
            if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("diffuse_color.connected"), this->material.mtl))
                isOn = isOnPBlock->GetInt(isOnParamID) ? true : false;
            if (isOn)
            {
                Texmap* diffuseMap = GetDiffuseMap(pblock->GetTexmap(paramID));
                if (diffuseMap != nullptr)
                {
                    this->texmaps[TexMapIndex::DIFFUSE].texmap = diffuseMap;
                    this->texmapCount++;
                }
            }
        }        
    }
    else if (this->material.mtl->ClassID() == PRO_SOLID_GLASS_MTL_CLASSID)
    {
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("transmittance_map.shader"), this->material.mtl))
        {
            auto isOn = true;
            if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("transmittance_map.connected"), this->material.mtl))
                isOn = isOnPBlock->GetInt(isOnParamID) ? true : false;
            if (isOn)
            {
                Texmap* selfIlluminationMap = GetSelfIlluminationMap(pblock->GetTexmap(paramID));
                if (selfIlluminationMap != nullptr)
                {
                    this->texmaps[TexMapIndex::SELF_ILLUMINATION].texmap = selfIlluminationMap;
                    this->texmapCount++;
                }
            }
        }
    }
    else if (this->material.mtl->ClassID() == PRO_GENERIC_MTL_CLASSID)
    {
        //Texture
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("diffuse.shader"), this->material.mtl))
        {
            auto isOn = true;
            if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("diffuse.connected"), this->material.mtl))
                isOn = isOnPBlock->GetInt(isOnParamID) ? true : false;
            if (isOn)
            {
                Texmap* diffuseMap = GetDiffuseMap(pblock->GetTexmap(paramID));
                if (diffuseMap != nullptr)
                {
                    this->texmaps[TexMapIndex::DIFFUSE].texmap = diffuseMap;
                    this->texmapCount++;
                }
            }
        }
        
        //Self illumination
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("self_illum_filter_map.shader"), this->material.mtl))
        {
            auto isOn = true;
            if (MaxUtilities::GetParamIDByName(isOnPBlock, isOnParamID, _M("self_illum_filter_map.connected"), this->material.mtl))
                isOn = isOnPBlock->GetInt(isOnParamID) ? true : false;
            if (isOn)
            {
                Texmap* selfIlluminationMap = GetSelfIlluminationMap(pblock->GetTexmap(paramID));
                if (selfIlluminationMap != nullptr)
                {
                    this->texmaps[TexMapIndex::SELF_ILLUMINATION].texmap = selfIlluminationMap;
                    this->texmapCount++;
                }
            }
        }        
    }
}

void ProMaterialsExporterHandler::Write(WxDataChunkWriter& writer, WxError& error)
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
    
    //Ambient color
    {
        Color ambient(0, 0, 0);

        float colorArray[4] = {ambient.r, ambient.g, ambient.b, 1.0f};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::AMBIENT_COLOR, colorArray, 4, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    if (this->material.mtl->ClassID() == PRO_PLASTIC_VINYL_MTL_CLASSID ||
        this->material.mtl->ClassID() == PRO_METALLIC_PAINT_MTL_CLASSID)
    {
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("diffuse_color"), this->material.mtl))
        {
            AColor diffuse = pblock->GetAColor(paramID);
                
            float colorArray[4] = {diffuse.r, diffuse.g, diffuse.b, 1.0f};
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray, 4, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }        
    }
    else if (this->material.mtl->ClassID() == PRO_METAL_MTL_CLASSID)
    {
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("diffuse_color"), this->material.mtl))
        {
            AColor diffuse = pblock->GetAColor(paramID);
                
            float colorArray[4] = {diffuse.r, diffuse.g, diffuse.b, 1.0f};
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray, 4, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }
    else if (this->material.mtl->ClassID() == PRO_SOLID_GLASS_MTL_CLASSID)
    {
        enum
        {
            CLEAR_COLOR = 0,
            GREEN_COLOR = 1,
            GRAY_COLOR = 2,
            BLUE_COLOR = 3,
            BLUE_GREEN_COLOR = 4,
            BRONZE_COLOR = 5,
            CUSTOM_COLOR = 6
        };
        int colorIndex = -1;
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("transmittance_color"), this->material.mtl))
        {
            colorIndex = pblock->GetInt(paramID);
        }
        auto diffuseValid = true;
        AColor diffuse(1,1,1);
        switch (colorIndex)
        {
            case CLEAR_COLOR: diffuse = Color(.3f,.3f,.3f); break;
            case GREEN_COLOR: diffuse = Color(0.0f,.5f,0.0f); break;
            case GRAY_COLOR: diffuse = Color(.7f,.7f,.7f); break;
            case BLUE_COLOR: diffuse = Color(0.0f,0.0f,.5f); break;
            case BLUE_GREEN_COLOR: diffuse = Color(0.0f,.5f,.5f); break;
            case BRONZE_COLOR: diffuse = Color(1,1,0); break;
            case CUSTOM_COLOR:
            {
                if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("transmittance_map"), this->material.mtl))
                    diffuse = pblock->GetAColor(paramID);
                break;
            }
            default: diffuseValid = false;
        }
        if (diffuseValid)
        {
            float colorArray[4] = {diffuse.r, diffuse.g, diffuse.b, 1.0f};
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray, 4, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
        
        //It's glass. Enable some type of transparency
        //pass->setSceneBlending(FinjinObsolete::SBF_ONE, FinjinObsolete::SBF_ONE);                
    }
    else if (this->material.mtl->ClassID() == PRO_MIRROR_MTL_CLASSID)
    {
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("tint_color"), this->material.mtl))
        {
            AColor diffuse = pblock->GetAColor(paramID);
                
            float colorArray[4] = {diffuse.r, diffuse.g, diffuse.b, 1.0f};
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray, 4, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }
    else if (this->material.mtl->ClassID() == PRO_GENERIC_MTL_CLASSID)
    {
        //Diffuse color
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("diffuse"), this->material.mtl))
        {
            AColor diffuse = pblock->GetAColor(paramID);
                
            float colorArray[4] = {diffuse.r, diffuse.g, diffuse.b, 1.0f};
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray, 4, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }

        //Emissive color
        //This looks terrible. It always comes out at full intensity?
        //if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("self_illum_filter_map"), this->material.mtl))
        //{
            //AColor emissive = pblock->GetAColor(paramID);
            //if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("self_illum_luminance"), this->material.mtl))
            //{
                //float intensity = std::max(1.0f, pblock->GetFloat(paramID));
                //emissive *= intensity;
                //pass->setSelfIllumination(emissive.r, emissive.g, emissive.b);
            //}            
        //}
                
        //Culling mode
        bool backfaceCullingEnabled = false;
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("backface_cull"), this->material.mtl))
            backfaceCullingEnabled = pblock->GetInt(paramID) ? true : false;
        if (!backfaceCullingEnabled)
        {
            writer.WriteString(StandardAssetDocumentPropertyNames::CULL_MODE, wxT("none"), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }        

        //Transparency
        if (MaxUtilities::GetParamIDByName(pblock, paramID, _M("transparency.connected"), this->material.mtl))
        {
            if (pblock->GetInt(paramID))
            {
                //Some amount of blending. Just use additive
                //pass->setSceneBlending(FinjinObsolete::SBF_ONE, FinjinObsolete::SBF_ONE);                
            }
        }
    }

    //Maps
    if (this->texmapCount > 0)
    {
        writer.WriteChunk(StandardAssetDocumentChunkNames::MAPS, [this](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            size_t mapIndex = 0;

            //Diffuse
            WriteDiffuseMap(writer, mapIndex, this->texmaps[TexMapIndex::DIFFUSE], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Self illumination
            WriteSelfIlluminationMap(writer, mapIndex, this->texmaps[TexMapIndex::SELF_ILLUMINATION], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}
