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
#include "FileUtilities.hpp"
#include "MathUtilities.hpp"
#include "FinjinSceneDocument.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyValues.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Locals-----------------------------------------------------------------------
struct MaxMapToTexMapIndex
{
    int maxID;
    StandardMaterialsExporterHandler::TexMapIndex texmapIndex;
};
static const MaxMapToTexMapIndex maxMapToTexMapIndex[] = 
{
    {ID_DI, StandardMaterialsExporterHandler::TexMapIndex::DIFFUSE},
    {ID_SP, StandardMaterialsExporterHandler::TexMapIndex::SPECULAR},
    {ID_DP, StandardMaterialsExporterHandler::TexMapIndex::HEIGHT},
    {ID_BU, StandardMaterialsExporterHandler::TexMapIndex::BUMP},
    {ID_SI, StandardMaterialsExporterHandler::TexMapIndex::SELF_ILLUMINATION},
    {ID_RL, StandardMaterialsExporterHandler::TexMapIndex::REFLECTION},
    {ID_RR, StandardMaterialsExporterHandler::TexMapIndex::REFRACTION},    
    {ID_OP, StandardMaterialsExporterHandler::TexMapIndex::OPACITY},
    {ID_SS, StandardMaterialsExporterHandler::TexMapIndex::SHININESS}, //3DS Max indicates glossiness map is ID_SH, but it is ID_SS
    {ID_AM, StandardMaterialsExporterHandler::TexMapIndex::ENVIRONMENT}
};


//Implementation---------------------------------------------------------------
StandardMaterialsExporterHandler::StandardMaterialsExporterHandler()
{
    for (auto& item : this->texmaps)
        item = nullptr;
    this->texmapCount = 0;
}

bool StandardMaterialsExporterHandler::CanHandle(MaterialAccessor material)
{
    if (material.mtl == nullptr)
        return false;
    else
    {
        return
            material.mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0) ||
            material.mtl->ClassID() == Class_ID(DMTL2_CLASS_ID, 0);
    }
}

void StandardMaterialsExporterHandler::CalculateRequirements()
{
    auto stdMtl2 = material.mtl->ClassID() == Class_ID(DMTL2_CLASS_ID, 0) ? static_cast<StdMat2*>(this->material.mtl) : nullptr;
    auto stdMtl = static_cast<StdMat*>(this->material.mtl);
        
    for (auto& item : this->texmaps)
        item = nullptr;
    this->texmapCount = 0;

    for (int i = 0; i < FINJIN_COUNT_OF(maxMapToTexMapIndex); i++)
    {
        auto mapID = stdMtl2 != nullptr ? stdMtl2->StdIDToChannel(maxMapToTexMapIndex[i].maxID) : maxMapToTexMapIndex[i].maxID;
        if (mapID != -1)
        {            
            auto texmap = GetExportableBitmapTexmap(stdMtl->GetSubTexmap(mapID));
            if (texmap != nullptr && stdMtl->SubTexmapOn(mapID))
            {
                auto& slot = this->texmaps[maxMapToTexMapIndex[i].texmapIndex];

                slot.amount = stdMtl->GetTexmapAmt(mapID, this->sceneExportSettings->time.GetNativeTime());
                slot.texmap = texmap;
                
                this->texmapCount++;
            }
        }
    }
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
    auto stdMtl = static_cast<StdMat*>(this->material.mtl);
    auto stdMtl2 = this->material.mtl->ClassID() == Class_ID(DMTL2_CLASS_ID, 0) ? static_cast<StdMat2*>(stdMtl) : nullptr;
    
    //Ambient color
    {
        Color ambient = this->material.mtl->GetAmbient();

        float colorArray[4] = {ambient.r, ambient.g, ambient.b, 1.0f};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::AMBIENT_COLOR, colorArray, 4, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
    
    //Diffuse color
    {
        Color diffuse = this->material.mtl->GetDiffuse();

        float colorArray[4] = {diffuse.r, diffuse.g, diffuse.b, 1.0f};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::DIFFUSE_COLOR, colorArray, 4, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
            
    //Specular color
    {
        Color specular = this->material.mtl->GetSpecular();
        float gloss = this->material.mtl->GetShinStr();

        float colorArray[4] = {specular.r, specular.g, specular.b, gloss};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::SPECULAR_COLOR, colorArray, 4, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
        
    //Shininess
    float shininess = this->material.mtl->GetShininess();
    writer.WriteFloat(StandardAssetDocumentPropertyNames::SHININESS, shininess, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    
    //Emissive color
    {
        Color emissive;
        if (stdMtl->GetSelfIllumColorOn())
            emissive = stdMtl->GetSelfIllumColor();
        else
        {
            float intensity = stdMtl->GetSelfIllum(this->sceneExportSettings->time.GetNativeTime());
            emissive.r = intensity;
            emissive.g = intensity;
            emissive.b = intensity;
        }
            
        if (emissive.r != 0 || emissive.g != 0 || emissive.b != 0)
        {
            float colorArray[4] = {emissive.r, emissive.g, emissive.b, 1.0f};
            writer.WriteFloats(StandardAssetDocumentPropertyNames::SELF_ILLUMINATION_COLOR, colorArray, 4, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }

    //Culling mode
    if (stdMtl->GetTwoSided())
    {
        writer.WriteString(StandardAssetDocumentPropertyNames::CULL_MODE, StandardAssetDocumentPropertyValues::CullMode::NONE, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
        
    //Wireframe
    if (stdMtl->GetWire())
    {
        writer.WriteString(StandardAssetDocumentPropertyNames::POLYGON_MODE, StandardAssetDocumentPropertyValues::PolygonMode::SOLID, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
    
    //Shading mode
    if (stdMtl2 != nullptr && stdMtl2->IsFaceted())
    {
        writer.WriteString(StandardAssetDocumentPropertyNames::SHADING_MODE, StandardAssetDocumentPropertyValues::ShadingMode::FLAT, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
    
    //Transparency
    float opacity = stdMtl->GetOpacity(this->sceneExportSettings->time.GetNativeTime());
    auto transparencyEnabled = !MathUtilities::AlmostOne(opacity);
    if (transparencyEnabled)
    {
        wxString transparencyType;
        switch (stdMtl->GetTransparencyType())
        {
            case TRANSP_ADDITIVE: transparencyType = StandardAssetDocumentPropertyValues::Transparency::ADDITIVE; break;
            case TRANSP_FILTER: transparencyType = StandardAssetDocumentPropertyValues::Transparency::FILTER; break;
            case TRANSP_SUBTRACTIVE: transparencyType = StandardAssetDocumentPropertyValues::Transparency::SUBTRACTIVE; break;
        }
        if (!transparencyType.empty())
        {
            writer.WriteString(StandardAssetDocumentPropertyNames::TRANSPARENCY, transparencyType, error);
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

            //Specular
            WriteSpecularMap(writer, mapIndex, this->texmaps[TexMapIndex::SPECULAR], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Self illumination
            WriteSelfIlluminationMap(writer, mapIndex, this->texmaps[TexMapIndex::SELF_ILLUMINATION], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Bump
            WriteBumpMap(writer, mapIndex, this->texmaps[TexMapIndex::BUMP], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Height
            WriteHeightMap(writer, mapIndex, this->texmaps[TexMapIndex::HEIGHT], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Reflection
            WriteReflectionMap(writer, mapIndex, this->texmaps[TexMapIndex::REFLECTION], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Refraction
            WriteRefractionMap(writer, mapIndex, this->texmaps[TexMapIndex::REFRACTION], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Shininess
            WriteShininessMap(writer, mapIndex, this->texmaps[TexMapIndex::SHININESS], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Environment
            WriteEnvironmentMap(writer, mapIndex, this->texmaps[TexMapIndex::ENVIRONMENT], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Opacity
            WriteOpacityMap(writer, mapIndex, this->texmaps[TexMapIndex::OPACITY], error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}
