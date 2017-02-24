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
#include "FileUtilities.hpp"
#include "MayaPlug.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Constants--------------------------------------------------------------------
static const MString VRAY_FASTSSSMTL_TYPE_NAME("VRayFastSSS2");
static const MString VRAY_LIGHTMTL_TYPE_NAME("VRayLightMtl");
static const MString VRAY_MTL_TYPE_NAME("VRayMtl");


//Implementation---------------------------------------------------------------
VRayMaterialsExporterHandler::VRayMaterialsExporterHandler()
{
    this->textureIsSpecular = false;
    this->texmapCount = 0;
}

void VRayMaterialsExporterHandler::CalculateRequirements()
{
    for (auto& texture : this->textures)
        texture.Reset();
    this->textureIsSpecular = false;
    this->texmapCount = 0;

    MFnDependencyNode depNode(this->material.obj);
    MString typeName = depNode.typeName();

    MObject diffuseTexture;

    if (typeName == VRAY_FASTSSSMTL_TYPE_NAME)
    {
        //Texture
        if (depNode.findPlug("diffuseAmount").asFloat() > 0)
            diffuseTexture = FindFileTexture(depNode, "diffuseTex");
        
        //Specular color, possibly diffuse texture
        if (diffuseTexture.isNull())
        {
            if (depNode.findPlug("reflectionAmount").asFloat() > 0)
            {
                diffuseTexture = FindFileTexture(depNode, "reflection");
                if (!diffuseTexture.isNull())
                    this->textureIsSpecular = true;
            }
        }
    }
    else if (typeName == VRAY_LIGHTMTL_TYPE_NAME)
    {
        //Texture
        diffuseTexture = FindFileTexture(depNode, "color");
    }
    else if (typeName == VRAY_MTL_TYPE_NAME)
    {
        //Texture
        if (depNode.findPlug("diffuseColorAmount").asFloat() > 0)
            diffuseTexture = FindFileTexture(depNode, "diffuseColor");
        
        //Specular color, possibly diffuse texture
        if (diffuseTexture.isNull())
        {
            if (depNode.findPlug("reflectionColorAmount").asFloat() > 0)
            {
                diffuseTexture = FindFileTexture(depNode, "reflectionColor");
                if (!diffuseTexture.isNull())
                    textureIsSpecular = true;
            }
        }
    }    

    //Diffuse texture
    if (!diffuseTexture.isNull())
    {
        this->textures[TexMapIndex::DIFFUSE].obj = diffuseTexture;
    }

    for (auto& texture : this->textures)
    {
        if (!texture.isNull())
            this->texmapCount++;
    }
}

bool VRayMaterialsExporterHandler::CanHandle(MaterialAccessor material)
{
    static const MString VRAY_TYPE_NAMES[] =
        {
        VRAY_FASTSSSMTL_TYPE_NAME,
        VRAY_LIGHTMTL_TYPE_NAME,
        VRAY_MTL_TYPE_NAME
        };

    MFnDependencyNode depNode(material.obj);
    MString typeName = depNode.typeName();
    for (int i = 0; i < FINJIN_COUNT_OF(VRAY_TYPE_NAMES); i++)
    {
        if (typeName == VRAY_TYPE_NAMES[i])
            return true;
    }
    return false;
}

void VRayMaterialsExporterHandler::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    MFnDependencyNode depNode(this->material.obj);
    MString typeName = depNode.typeName();

    if (typeName == VRAY_FASTSSSMTL_TYPE_NAME)
    {
        //Diffuse color
        if (this->textures[TexMapIndex::DIFFUSE].isNull())            
        {
            auto diffuse = MayaPlug::GetColor(depNode.findPlug("diffuseTex"));
                
            auto colorArray = diffuse.GetRGBAArray();
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray.data(), colorArray.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }

        //Specular color
        if (!this->textureIsSpecular)
        {
            auto specular = MayaPlug::GetColor(depNode.findPlug("reflection"));
                
            auto colorArray = specular.GetRGBAArray();
            writer.WriteFloats(StandardAssetDocumentPropertyNames::SPECULAR_COLOR, colorArray.data(), colorArray.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
        
        //Shininess
        float shininess = depNode.findPlug("glossiness").asFloat() * 100;
        writer.WriteFloat(StandardAssetDocumentPropertyNames::SHININESS, shininess, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
    else if (typeName == VRAY_LIGHTMTL_TYPE_NAME)
    {
        //Diffuse color
        if (this->textures[TexMapIndex::DIFFUSE].isNull())            
        {
            auto diffuse = MayaPlug::GetColor(depNode.findPlug("color"));
            diffuse.a = depNode.findPlug("opacity").asFloat();
                
            auto colorArray = diffuse.GetRGBAArray();
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray.data(), colorArray.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }
    else if (typeName == VRAY_MTL_TYPE_NAME)
    {
        //Diffuse color
        if (this->textures[TexMapIndex::DIFFUSE].isNull())            
        {
            auto diffuse = MayaPlug::GetColor(depNode.findPlug("diffuseColor"));
            diffuse.a = depNode.findPlug("opacityMap").asFloat();
                
            auto colorArray = diffuse.GetRGBAArray();
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray.data(), colorArray.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }

        //Specular color
        if (!this->textureIsSpecular)
        {
            auto specular = MayaPlug::GetColor(depNode.findPlug("reflectionColor"));
                
            auto colorArray = specular.GetRGBAArray();
            writer.WriteFloats(StandardAssetDocumentPropertyNames::SPECULAR_COLOR, colorArray.data(), colorArray.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
            
        //Shininess
        float shininess = depNode.findPlug("reflectionGlossiness").asFloat() * 100;
        writer.WriteFloat(StandardAssetDocumentPropertyNames::SHININESS, shininess, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }    

    if (this->texmapCount > 0)
    {
        writer.WriteChunk(StandardAssetDocumentChunkNames::MAPS, [this](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->texmapCount, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            size_t mapIndex = 0;
            WriteDiffuseMap(writer, mapIndex, this->textures[TexMapIndex::DIFFUSE], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}
