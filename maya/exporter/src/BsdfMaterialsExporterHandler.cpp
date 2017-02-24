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
#include "BsdfMaterialsExporterHandler.hpp"
#include "ApplicationAccessor.hpp"
#include "MayaPlug.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyValues.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Constants--------------------------------------------------------------------
static const MString BSDF_ARCHITECTURAL_TYPE_NAME("builtin_bsdf_architectural");
static const MString BSDF_ARCHITECTURAL_COMP_TYPE_NAME("builtin_bsdf_architectural_comp");
static const MString BSDF_ASHIKHMIN_TYPE_NAME("builtin_bsdf_ashikhmin");
static const MString BSDF_CARPAINT_TYPE_NAME("builtin_bsdf_carpaint");
static const MString BSDF_LAMBERT_TYPE_NAME("builtin_bsdf_lambert");
static const MString BSDF_PHONG_TYPE_NAME("builtin_bsdf_phong");


//Implementation---------------------------------------------------------------
BsdfMaterialsExporterHandler::BsdfMaterialsExporterHandler()
{
    this->textureIsSpecular = false;
    this->texmapCount = 0;
}

void BsdfMaterialsExporterHandler::CalculateRequirements()
{
    for (auto& texture : this->textures)
        texture.Reset();
    this->texmapCount = 0;
    this->textureIsSpecular = false;

    MFnDependencyNode depNode(this->material.obj);
    MString typeName = depNode.typeName();

    MObject diffuseTexture;

    if (typeName == BSDF_ARCHITECTURAL_TYPE_NAME)
    {
        //Texture
        diffuseTexture = FindFileTexture(depNode, "diffuse_refl");
        
        //Specular color, possibly diffuse texture
        if (diffuseTexture.isNull())
        {
            diffuseTexture = FindFileTexture(depNode, "specular_refl");
            if (!diffuseTexture.isNull())
                this->textureIsSpecular = true;
        }
    }
    else if (typeName == BSDF_ARCHITECTURAL_COMP_TYPE_NAME)
    {
        //Texture
        diffuseTexture = FindFileTexture(depNode, "diffuse");
        
        //Specular color, possibly diffuse texture
        if (diffuseTexture.isNull())
        {
            diffuseTexture = FindFileTexture(depNode, "refl_color");
            if (!diffuseTexture.isNull())
                this->textureIsSpecular = true;
        }
    }
    else if (typeName == BSDF_ASHIKHMIN_TYPE_NAME)
    {
        //Texture
        diffuseTexture = FindFileTexture(depNode, "diffuse");
    }
    else if (typeName == BSDF_CARPAINT_TYPE_NAME)
    {
        //Texture
        diffuseTexture = FindFileTexture(depNode, "metallic_color");
    }
    else if (typeName == BSDF_LAMBERT_TYPE_NAME)
    {
        //Texture
        diffuseTexture = FindFileTexture(depNode, "diffuse");
    }
    else if (typeName == BSDF_PHONG_TYPE_NAME)
    {
        //Texture
        diffuseTexture = FindFileTexture(depNode, "diffuse");
        
        //Specular color, possibly diffuse texture
        if (diffuseTexture.isNull())
        {
            diffuseTexture = FindFileTexture(depNode, "glossy");
            if (!diffuseTexture.isNull())
                this->textureIsSpecular = true;
        }
    }

    //Diffuse texture
    if (!diffuseTexture.isNull())
        this->textures[TexMapIndex::DIFFUSE].obj = diffuseTexture;

    for (auto& texture : this->textures)
    {
        if (!texture.isNull())
            this->texmapCount++;
    }
}

bool BsdfMaterialsExporterHandler::CanHandle(MaterialAccessor material)
{
    static const MString BSDF_TYPE_NAMES[] =
        {
        BSDF_ARCHITECTURAL_TYPE_NAME,
        BSDF_ARCHITECTURAL_COMP_TYPE_NAME,
        BSDF_ASHIKHMIN_TYPE_NAME,
        BSDF_CARPAINT_TYPE_NAME,
        BSDF_LAMBERT_TYPE_NAME,
        BSDF_PHONG_TYPE_NAME
        };

    MFnDependencyNode depNode(material.obj);
    MString typeName = depNode.typeName();
    for (int i = 0; i < FINJIN_COUNT_OF(BSDF_TYPE_NAMES); i++)
    {
        if (typeName == BSDF_TYPE_NAMES[i])
            return true;
    }
    return false;
}

void BsdfMaterialsExporterHandler::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    //Type
    writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_STANDARD, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Name
    writer.WriteString(StandardAssetDocumentPropertyNames::NAME, writer.GetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME), error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //-------------------------------------------
    MFnDependencyNode depNode(this->material.obj);
    MString typeName = depNode.typeName();

    if (typeName == BSDF_ARCHITECTURAL_TYPE_NAME)
    {
        //Diffuse color
        if (this->textures[TexMapIndex::DIFFUSE].isNull())            
        {
            auto diffuse = MayaPlug::GetColor(depNode.findPlug("diffuse_refl"));
                
            auto colorArray = diffuse.GetRGBAArray();
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray.data(), colorArray.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }

        //Specular color
        if (!this->textureIsSpecular)
        {
            auto specular = MayaPlug::GetColor(depNode.findPlug("specular_refl"));
                
            auto colorArray = specular.GetRGBAArray();
            writer.WriteFloats(StandardAssetDocumentPropertyNames::SPECULAR_COLOR, colorArray.data(), colorArray.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }
    else if (typeName == BSDF_ARCHITECTURAL_COMP_TYPE_NAME)
    {
        //Diffuse color
        if (this->textures[TexMapIndex::DIFFUSE].isNull())            
        {
            auto diffuse = MayaPlug::GetColor(depNode.findPlug("diffuse"));
                
            auto colorArray = diffuse.GetRGBAArray();
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray.data(), colorArray.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }

        //Specular color
        if (!this->textureIsSpecular)
        {
            auto specular = MayaPlug::GetColor(depNode.findPlug("refl_color"));
                
            auto colorArray = specular.GetRGBAArray();
            writer.WriteFloats(StandardAssetDocumentPropertyNames::SPECULAR_COLOR, colorArray.data(), colorArray.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }
    else if (typeName == BSDF_ASHIKHMIN_TYPE_NAME)
    {
        //Diffuse color
        if (this->textures[TexMapIndex::DIFFUSE].isNull())            
        {
            auto diffuse = MayaPlug::GetColor(depNode.findPlug("diffuse"));
                
            auto colorArray = diffuse.GetRGBAArray();
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray.data(), colorArray.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }
    else if (typeName == BSDF_CARPAINT_TYPE_NAME)
    {
        //Diffuse color
        if (this->textures[TexMapIndex::DIFFUSE].isNull())            
        {
            auto diffuse = MayaPlug::GetColor(depNode.findPlug("metallic_color"));
                
            auto colorArray = diffuse.GetRGBAArray();
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray.data(), colorArray.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }
    else if (typeName == BSDF_LAMBERT_TYPE_NAME)
    {
        //Diffuse color
        if (this->textures[TexMapIndex::DIFFUSE].isNull())            
        {
            auto diffuse = MayaPlug::GetColor(depNode.findPlug("diffuse"));
                
            auto colorArray = diffuse.GetRGBAArray();
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray.data(), colorArray.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }
    else if (typeName == BSDF_PHONG_TYPE_NAME)
    {
        //Diffuse color
        if (this->textures[TexMapIndex::DIFFUSE].isNull())            
        {
            auto diffuse = MayaPlug::GetColor(depNode.findPlug("diffuse"));
                
            auto colorArray = diffuse.GetRGBAArray();
            writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray.data(), colorArray.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }

        //Specular color
        if (!this->textureIsSpecular)
        {
            auto specular = MayaPlug::GetColor(depNode.findPlug("glossy"));
                
            auto colorArray = specular.GetRGBAArray();
            writer.WriteFloats(StandardAssetDocumentPropertyNames::SPECULAR_COLOR, colorArray.data(), colorArray.size(), error);
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
            WriteDiffuseMap(writer, mapIndex, this->textures[TexMapIndex::DIFFUSE], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}
