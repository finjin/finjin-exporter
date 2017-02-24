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
#include "StandardMaterialsExporterHandler.hpp"
#include "ApplicationAccessor.hpp"
#include "MathUtilities.hpp"
#include <maya/MFnPhongShader.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MFnLambertShader.h>
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyValues.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
StandardMaterialsExporterHandler::StandardMaterialsExporterHandler()
{
}

void StandardMaterialsExporterHandler::CalculateRequirements()
{
    for (auto& texture : this->textures)
        texture.Reset();
    this->texmapCount = 0;
    this->colorTextures.clear();

    MObject incandescenceTexture;
    if (this->material.obj.hasFn(MFn::kPhong))
    {
        MFnPhongShader shader(this->material.obj);
        
        //Texture
        FindFileTextures(this->colorTextures, shader, "color");

        //Self illumination texture
        incandescenceTexture = FindFileTexture(shader, "incandescence");        
    }
    else if (this->material.obj.hasFn(MFn::kBlinn))
    {
        MFnBlinnShader shader(this->material.obj);
        
        //Texture
        FindFileTextures(this->colorTextures, shader, "color");
        
        //Self illumination texture
        incandescenceTexture = FindFileTexture(shader, "incandescence");
    }
    else if (this->material.obj.hasFn(MFn::kLambert))
    {
        MFnLambertShader shader(this->material.obj);
        
        //Texture
        FindFileTextures(this->colorTextures, shader, "color");
        
        //Self illumination texture
        incandescenceTexture = FindFileTexture(shader, "incandescence");
    }

    //Diffuse texture
    if (this->colorTextures.size() == 1)
    {
        this->textures[TexMapIndex::DIFFUSE].obj = this->colorTextures[0].obj;
    }

    //Self illumination texture
    if (!incandescenceTexture.isNull())
    {
        this->textures[TexMapIndex::SELF_ILLUMINATION].obj = incandescenceTexture;
    }

    //Handled layered textures, if any
    for (size_t i = 0; i < this->colorTextures.size(); i++)
    {
        switch (this->colorTextures[i].blendMode)
        {
            case MAYA_BLEND_NONE:
            {
                if (this->textures[TexMapIndex::DIFFUSE].isNull())
                    this->textures[TexMapIndex::DIFFUSE].obj = this->colorTextures[i].obj;
                break;
            }
            case MAYA_BLEND_IN: break; //Not handled
            case MAYA_BLEND_OUT: break; //Not handled
            case MAYA_BLEND_SUBTRACT: break; //Not handled
            case MAYA_BLEND_OVER: break; //Not handled
            case MAYA_BLEND_DARKEN: break; //Not handled
            case MAYA_BLEND_MULTIPLY: break; //Not handled
            case MAYA_BLEND_DIFFERENCE: break; //Not handled
            case MAYA_BLEND_SATURATE: break; //Not handled
            case MAYA_BLEND_DESATURATE: break; //Not handled
            case MAYA_BLEND_ADD: //Fall through
            case MAYA_BLEND_LIGHTEN: //Fall through
            case MAYA_BLEND_ILLUMINATE: 
            {
                if (this->textures[TexMapIndex::SELF_ILLUMINATION].isNull())
                    this->textures[TexMapIndex::SELF_ILLUMINATION].obj = incandescenceTexture;
                break;
            }
        }
    }

    for (auto& texture : this->textures)
    {
        if (!texture.isNull())
            this->texmapCount++;
    }
}

bool StandardMaterialsExporterHandler::CanHandle(MaterialAccessor material)
{
    auto result = false;

    if (material.IsValid())
    {
        if (material.obj.hasFn(MFn::kPhong))
            result = true;
        else if (material.obj.hasFn(MFn::kBlinn))
            result = true;
        else if (material.obj.hasFn(MFn::kLambert))
            result = true;                        
    }

    return result;
}

void StandardMaterialsExporterHandler::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    //Type
    writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_STANDARD, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Name
    writer.WriteString(StandardAssetDocumentPropertyNames::NAME, writer.GetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME), error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //-------------------------------------------
    MColor diffuse(1, 1, 1);
    MColor ambient(0, 0, 0);
    MColor specular(0, 0, 0);
    float shininess = 0;
    MColor emissive(0, 0, 0);
    float transparency = 1;
    bool transparencyEnabled = false;

    if (this->material.obj.hasFn(MFn::kPhong))
    {
        MFnPhongShader shader(this->material.obj);
        
        //Diffuse color
        if (this->textures[TexMapIndex::DIFFUSE].isNull())
            diffuse = shader.color();
        
        //Ambient color
        ambient = shader.ambientColor();
        
        //Specular color and shininess
        specular = shader.specularColor();
        shininess = shader.cosPower() * 1.28;

        //Emissive color
        if (this->textures[TexMapIndex::SELF_ILLUMINATION].isNull())
            emissive = shader.incandescence();

        //Transparency
        transparency = shader.transparency().r;
        float opacity = 1 - transparency;
        transparencyEnabled = !MathUtilities::AlmostOne(opacity);
    }
    else if (this->material.obj.hasFn(MFn::kBlinn))
    {
        MFnBlinnShader shader(this->material.obj);
        
        //Diffuse color
        if (this->textures[TexMapIndex::DIFFUSE].isNull())
            diffuse = shader.color();
        
        //Ambient color
        ambient = shader.ambientColor();
        
        //Specular color and shininess
        specular = shader.specularColor();
        shininess = 1 - shader.eccentricity();

        //Emissive color
        if (this->textures[TexMapIndex::SELF_ILLUMINATION].isNull())
            emissive = shader.incandescence();

        //Transparency
        transparency = shader.transparency().r;
        float opacity = 1 - transparency;
        transparencyEnabled = !MathUtilities::AlmostOne(opacity);
    }
    else if (this->material.obj.hasFn(MFn::kLambert))
    {
        MFnLambertShader shader(this->material.obj);
        
        //Diffuse color
        if (this->textures[TexMapIndex::DIFFUSE].isNull())
            diffuse = shader.color();
        
        //Ambient color
        ambient = shader.ambientColor();
        
        //Emissive color
        if (this->textures[TexMapIndex::SELF_ILLUMINATION].isNull())
            emissive = shader.incandescence();

        //Transparency
        transparency = shader.transparency().r;
        float opacity = 1 - transparency;
        transparencyEnabled = !MathUtilities::AlmostOne(opacity);
    }
    
    //Ambient color
    {
        float colorArray[4] = {ambient.r, ambient.g, ambient.b, 1};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::AMBIENT_COLOR, colorArray, 4, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Diffuse color
    {
        float colorArray[4] = {diffuse.r, diffuse.g, diffuse.b, diffuse.a};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray, 4, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
    
    //Specular color
    {
        float colorArray[4] = {specular.r, specular.g, specular.b, 1};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::SPECULAR_COLOR, colorArray, 4, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Shininess
    writer.WriteFloat(StandardAssetDocumentPropertyNames::SHININESS, shininess, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        
    //Emissive color
    if (emissive.r != 0 || emissive.g != 0 || emissive.b != 0)
    {
        float colorArray[4] = {emissive.r, emissive.g, emissive.b, 1.0f};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::SELF_ILLUMINATION_COLOR, colorArray, 4, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
        
    //Culling mode
    if (this->material.culling == MaterialAccessor::NO_CULLING)
    {
        writer.WriteString(StandardAssetDocumentPropertyNames::CULL_MODE, wxT("none"), error);            
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
    else if (this->material.culling == MaterialAccessor::OPPOSITE_CULLING)
    {
        writer.WriteString(StandardAssetDocumentPropertyNames::CULL_MODE, wxT("opposite"), error);            
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
            WriteDiffuseMap(writer, mapIndex, this->textures[TexMapIndex::DIFFUSE], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Specular
            WriteSpecularMap(writer, mapIndex, this->textures[TexMapIndex::SPECULAR], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Height
            WriteHeightMap(writer, mapIndex, this->textures[TexMapIndex::HEIGHT], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Bump
            WriteBumpMap(writer, mapIndex, this->textures[TexMapIndex::BUMP], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Self illumination
            WriteSelfIlluminationMap(writer, mapIndex, this->textures[TexMapIndex::SELF_ILLUMINATION], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Reflection
            WriteReflectionMap(writer, mapIndex, this->textures[TexMapIndex::REFLECTION], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Opacity
            WriteOpacityMap(writer, mapIndex, this->textures[TexMapIndex::OPACITY], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}
