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
#include "MaxBaseMaterialExporterHandler.hpp"
#include "FileUtilities.hpp"
#include "MathUtilities.hpp"
#include "MaxUtilities.hpp"
#include "ExporterUtilities.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyValues.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Local functions---------------------------------------------------------------
static float ConvertMaxTiling(float tiling)
{
    float scale = 1;
    if (!MathUtilities::AlmostZero(tiling))
        scale = 1 / tiling;
    return scale;
}

static bool IsValidReflectionOrRefractionMap(Texmap* texmap)
{
    if (texmap != nullptr)
    {
        if (texmap->ClassID() == Class_ID(ACUBIC_CLASS_ID, 0))
        {
            //It's a reflect/refract map
            auto stdCubic = static_cast<StdCubic*>(texmap);

            auto pblock = stdCubic->GetParamBlock(0);
            ParamID paramID;
            if (MaxUtilities::GetParamIDByName(paramID, wxT("bitmapName"), pblock))
            {
                //Up, down, left, right, front, back
                for (int i = 0; i < 6; i++)
                {
                    auto asset = pblock->GetAssetUser(paramID, 0, i);
                    if (asset.GetFileName().isNull())
                        return false;
                }
            }

            return true;
        }
        else if (texmap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
        {
            auto bitmapTex = static_cast<BitmapTex*>(texmap);
            //auto mapping = bitmapTex->GetUVGen()->GetCoordMapping(0)); //UVMAP_EXPLICIT, UVMAP_SPHERE_ENV, etc

            MSTR maxTextureFileName = bitmapTex->GetMapName();
            BitmapInfo bitmapInfo(maxTextureFileName);
            if (BMMGetFullFilename(&bitmapInfo))
                maxTextureFileName = bitmapInfo.Name();

            return !maxTextureFileName.isNull();
        }
    }

    return false;
}


//Implementation----------------------------------------------------------------
Texmap* MaxBaseMaterialExporterHandler::GetDiffuseMap(Texmap* texmap)
{
    return GetExportableBitmapTexmap(texmap);
}

void MaxBaseMaterialExporterHandler::WriteDiffuseMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    texmapSlot.texmap = GetExportableBitmapTexmap(texmapSlot.texmap);
    if (texmapSlot.texmap == nullptr)
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(texmapSlot.texmap->GetName()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, texmapSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        WriteTexmap(writer, texmapSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_DIFFUSE, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

Texmap* MaxBaseMaterialExporterHandler::GetSpecularMap(Texmap* texmap)
{
    if (texmap != nullptr)
    {
        if (texmap->ClassID() == Class_ID(COMPOSITE_CLASS_ID, 0) && texmap->NumSubTexmaps() > 0)
        {
            //It's a composite map. Look for the first non-null texmap
            for (int i = 2; i < texmap->NumSubTexmaps(); i += 2) //Skip the 'mask' texmaps
            {
                if (texmap->GetSubTexmap(i) != nullptr)
                {
                    texmap = texmap->GetSubTexmap(i);
                    break;
                }
            }
        }
        else
        {
            //The map isn't supported at all
            texmap = nullptr;
        }
    }

    return texmap;
}

void MaxBaseMaterialExporterHandler::WriteSpecularMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    texmapSlot.texmap = GetExportableBitmapTexmap(texmapSlot.texmap);
    if (texmapSlot.texmap == nullptr)
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(texmapSlot.texmap->GetName()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, texmapSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        WriteTexmap(writer, texmapSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_SPECULAR, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

Texmap* MaxBaseMaterialExporterHandler::GetHeightMap(Texmap* texmap)
{
    return GetExportableBitmapTexmap(texmap);
}

void MaxBaseMaterialExporterHandler::WriteHeightMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    texmapSlot.texmap = GetExportableBitmapTexmap(texmapSlot.texmap);
    if (texmapSlot.texmap == nullptr)
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(texmapSlot.texmap->GetName()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, texmapSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        WriteTexmap(writer, texmapSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_HEIGHT, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

Texmap* MaxBaseMaterialExporterHandler::GetReflectionMap(Texmap* texmap)
{
    if (IsValidReflectionOrRefractionMap(texmap))
    {
        //Do nothing
    }
    else
        texmap = GetExportableBitmapTexmap(texmap);

    return texmap;
}

void MaxBaseMaterialExporterHandler::WriteReflectionMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    texmapSlot.texmap = GetExportableBitmapTexmap(texmapSlot.texmap);
    if (texmapSlot.texmap == nullptr)
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(texmapSlot.texmap->GetName()));

    if (IsValidReflectionOrRefractionMap(texmapSlot.texmap))
    {
        WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
        writer.WriteChunk(mapChunkName, [this, texmapSlot](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            WriteTexmap(writer, texmapSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_REFLECTION, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}

Texmap* MaxBaseMaterialExporterHandler::GetRefractionMap(Texmap* texmap)
{
    if (IsValidReflectionOrRefractionMap(texmap))
    {
        //Do nothing
    }
    else
        texmap = GetExportableBitmapTexmap(texmap);

    return texmap;
}

void MaxBaseMaterialExporterHandler::WriteRefractionMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    texmapSlot.texmap = GetExportableBitmapTexmap(texmapSlot.texmap);
    if (texmapSlot.texmap == nullptr)
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(texmapSlot.texmap->GetName()));

    if (IsValidReflectionOrRefractionMap(texmapSlot.texmap))
    {
        WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
        writer.WriteChunk(mapChunkName, [this, texmapSlot](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            WriteTexmap(writer, texmapSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_REFRACTION, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}

Texmap* MaxBaseMaterialExporterHandler::GetBumpMap(Texmap* texmap)
{
    return GetExportableBitmapTexmap(texmap);
}

void MaxBaseMaterialExporterHandler::WriteBumpMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    texmapSlot.texmap = GetExportableBitmapTexmap(texmapSlot.texmap);
    if (texmapSlot.texmap == nullptr)
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(texmapSlot.texmap->GetName()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, texmapSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        WriteTexmap(writer, texmapSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_BUMP, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

Texmap* MaxBaseMaterialExporterHandler::GetSelfIlluminationMap(Texmap* texmap)
{
    return GetExportableBitmapTexmap(texmap);
}

void MaxBaseMaterialExporterHandler::WriteSelfIlluminationMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    texmapSlot.texmap = GetExportableBitmapTexmap(texmapSlot.texmap);
    if (texmapSlot.texmap == nullptr)
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(texmapSlot.texmap->GetName()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, texmapSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        WriteTexmap(writer, texmapSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_SELF_ILLUMINATION, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

void MaxBaseMaterialExporterHandler::WriteTexmap(WxDataChunkWriter& writer, TexMapSlot texmapSlot, const wxString& type, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    texmapSlot.texmap = GetExportableBitmapTexmap(texmapSlot.texmap);
    if (texmapSlot.texmap == nullptr)
        return;

    writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, type, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    writer.WriteString(StandardAssetDocumentPropertyNames::NAME, writer.GetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME), error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    if (texmapSlot.amount < 1.0f)
    {
        writer.WriteFloat(StandardAssetDocumentPropertyNames::AMOUNT, texmapSlot.amount, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    if (texmapSlot.texmap->ClassID() == Class_ID(ACUBIC_CLASS_ID, 0))
    {
        //It's a reflect/refract map
        auto stdCubic = static_cast<StdCubic*>(texmapSlot.texmap);

        auto pblock = stdCubic->GetParamBlock(0);
        ParamID paramID;
        if (MaxUtilities::GetParamIDByName(paramID, wxT("bitmapName"), pblock))
        {
            //Up, down, left, right, front, back
            int i = 2; //Left
            auto asset = pblock->GetAssetUser(paramID, 0, i);
            auto maxTextureFileName = asset.GetFileName();
            if (!maxTextureFileName.isNull())
            {
                //File name
                BitmapInfo bitmapInfo(maxTextureFileName);
                if (BMMGetFullFilename(&bitmapInfo))
                    maxTextureFileName = bitmapInfo.Name();
                auto textureFileName = ApplicationStringToWxString(maxTextureFileName);

                //Store the file name reference
                AddBitmapFileName(textureFileName);

                //Texture file name
                auto resolvedPaths = this->sceneExportSettings->ResolveAssetPaths(AssetClass::TEXTURE, textureFileName);
                auto assetRef = this->sceneExportSettings->GetAssetFileReference(AssetClass::TEXTURE, resolvedPaths);
                writer.WriteString(StandardAssetDocumentPropertyNames::TEXTURE_REF, assetRef.ToUriString(), error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }
    }
    else
    {
        auto bitmapTex = static_cast<BitmapTex*>(texmapSlot.texmap);

        //File name
        MSTR maxTextureFileName = bitmapTex->GetMapName();
        BitmapInfo bitmapInfo(maxTextureFileName);
        if (BMMGetFullFilename(&bitmapInfo))
            maxTextureFileName = bitmapInfo.Name();
        auto textureFileName = ApplicationStringToWxString(maxTextureFileName);

        //Store the file name reference
        AddBitmapFileName(textureFileName);

        //Texture file name
        auto resolvedPaths = this->sceneExportSettings->ResolveAssetPaths(AssetClass::TEXTURE, textureFileName);
        auto assetRef = this->sceneExportSettings->GetAssetFileReference(AssetClass::TEXTURE, resolvedPaths);
        writer.WriteString(StandardAssetDocumentPropertyNames::TEXTURE_REF, assetRef.ToUriString(), error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Channel
        if (texmapSlot.texmap->GetUVWSource() == UVWSRC_EXPLICIT)
        {
            auto mapChannel = texmapSlot.texmap->GetMapChannel() - 1;
            if (mapChannel != 0)
            {
                writer.WriteUInt8(StandardAssetDocumentPropertyNames::TEXTURE_COORDINATE_SET, mapChannel, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }

        //Blending
        auto bitmap = bitmapTex->GetBitmap(0);
        if (bitmap != nullptr && bitmap->HasAlpha())
        {
            if (bitmap->PreMultipliedAlpha())
            {
                writer.WriteString(StandardAssetDocumentPropertyNames::TEXTURE_ALPHA, StandardAssetDocumentPropertyValues::TextureAlpha::PREMULTIPLIED, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
            else
            {
                writer.WriteString(StandardAssetDocumentPropertyNames::TEXTURE_ALPHA, StandardAssetDocumentPropertyValues::TextureAlpha::STANDARD, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }

        //Texture coordinate transform
        auto uvGen = bitmapTex->GetUVGen();
        if (uvGen != nullptr)
        {
            enum
            {
                TILING_U = 1 << 0,
                TILING_V = 1 << 1,
                MIRROR_U = 1 << 2,
                MIRROR_V = 1 << 3
            };
            float scaleU = ConvertMaxTiling(uvGen->GetUScl(this->sceneExportSettings->time.GetNativeTime()));
            float scaleV = ConvertMaxTiling(uvGen->GetVScl(this->sceneExportSettings->time.GetNativeTime()));

            auto addressModeU = StandardAssetDocumentPropertyValues::TextureAddressMode::WRAP;
            auto addressModeV = StandardAssetDocumentPropertyValues::TextureAddressMode::WRAP;

            if (!(uvGen->GetTextureTiling() & TILING_U))
                addressModeU = StandardAssetDocumentPropertyValues::TextureAddressMode::WRAP;
            if (uvGen->GetTextureTiling() & MIRROR_U)
            {
                addressModeU = StandardAssetDocumentPropertyValues::TextureAddressMode::MIRROR;
                scaleU = -scaleU;
            }

            if (!(uvGen->GetTextureTiling() & TILING_V))
                addressModeV = StandardAssetDocumentPropertyValues::TextureAddressMode::CLAMP;
            if (uvGen->GetTextureTiling() & MIRROR_V)
            {
                addressModeV = StandardAssetDocumentPropertyValues::TextureAddressMode::MIRROR;
                scaleV = -scaleV;
            }
            wxString addressModes[3] = {addressModeU, addressModeV, wxString(StandardAssetDocumentPropertyValues::TextureAddressMode::WRAP)};

            auto offsetX = -uvGen->GetUOffs(this->sceneExportSettings->time.GetNativeTime());
            auto offsetY = uvGen->GetVOffs(this->sceneExportSettings->time.GetNativeTime());

            auto rotation = -uvGen->GetWAng(this->sceneExportSettings->time.GetNativeTime());

            //Finally, set everything
            if (scaleU != 1 && scaleV != 1)
            {
                float scaling[3] = {scaleU, scaleV, 1};
                writer.WriteFloats(StandardAssetDocumentPropertyNames::TEXTURE_SCALE, scaling, 3, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }

            if (rotation != 0)
            {
                writer.WriteFloat(StandardAssetDocumentPropertyNames::TEXTURE_ROTATION, rotation, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }

            if (offsetX != 0 && offsetY != 0)
            {
                float offset[3] = {offsetX, offsetY, 0};
                writer.WriteFloats(StandardAssetDocumentPropertyNames::TEXTURE_OFFSET, offset, 3, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }

            if (addressModes[0] != StandardAssetDocumentPropertyValues::TextureAddressMode::WRAP ||
                addressModes[1] != StandardAssetDocumentPropertyValues::TextureAddressMode::WRAP ||
                addressModes[2] != StandardAssetDocumentPropertyValues::TextureAddressMode::WRAP)
            {
                writer.WriteStrings(StandardAssetDocumentPropertyNames::TEXTURE_ADDRESS_MODE, addressModes, 3, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }
    }
}

Texmap* MaxBaseMaterialExporterHandler::GetOpacityMap(Texmap* texmap)
{
    return GetExportableBitmapTexmap(texmap);
}

void MaxBaseMaterialExporterHandler::WriteOpacityMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    texmapSlot.texmap = GetExportableBitmapTexmap(texmapSlot.texmap);
    if (texmapSlot.texmap == nullptr)
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(texmapSlot.texmap->GetName()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, texmapSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        WriteTexmap(writer, texmapSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_OPACITY, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

Texmap* MaxBaseMaterialExporterHandler::GetShininessMap(Texmap* texmap)
{
    return GetExportableBitmapTexmap(texmap);
}

void MaxBaseMaterialExporterHandler::WriteShininessMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    texmapSlot.texmap = GetExportableBitmapTexmap(texmapSlot.texmap);
    if (texmapSlot.texmap == nullptr)
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(texmapSlot.texmap->GetName()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, texmapSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        WriteTexmap(writer, texmapSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_SHININESS, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

Texmap* MaxBaseMaterialExporterHandler::GetEnvironmentMap(Texmap* texmap)
{
    return GetExportableBitmapTexmap(texmap);
}

void MaxBaseMaterialExporterHandler::WriteEnvironmentMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    texmapSlot.texmap = GetExportableBitmapTexmap(texmapSlot.texmap);
    if (texmapSlot.texmap == nullptr)
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(texmapSlot.texmap->GetName()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, texmapSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        WriteTexmap(writer, texmapSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_ENVIRONMENT, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

Texmap* MaxBaseMaterialExporterHandler::GetExportableBitmapTexmap(Texmap* texmap)
{
    //Iterate until there's nothing left to look at
    while (texmap != nullptr)
    {
        if (texmap->ClassID() == Class_ID(TINT_CLASS_ID, 0))
        {
            //It's a tint map. Get the sub-texmap
            texmap = texmap->GetSubTexmap(0);
        }
        else if (texmap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
        {
            //It's a bitmap. Break out of the loop and return it
            break;
        }
        else if (texmap->ClassID() == Class_ID(COMPOSITE_CLASS_ID, 0) && texmap->NumSubTexmaps() > 0)
        {
            //It's a composite map. Look for the first non-null texmap
            for (int i = 0; i < texmap->NumSubTexmaps(); i += 2) //Skip the 'mask' texmaps
            {
                if (texmap->GetSubTexmap(i) != nullptr)
                {
                    texmap = texmap->GetSubTexmap(i);
                    break;
                }
            }
        }
        else if (texmap->ClassID() == Class_ID(ACUBIC_CLASS_ID, 0))
        {
            if (!IsValidReflectionOrRefractionMap(texmap))
                texmap = nullptr;
            break;
        }
        else
        {
            //The map isn't supported at all
            texmap = nullptr;
        }
    }

    return texmap;
}
