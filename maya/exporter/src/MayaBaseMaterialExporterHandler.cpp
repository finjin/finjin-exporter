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
#include "MayaBaseMaterialExporterHandler.hpp"
#include "FileUtilities.hpp"
#include "MathUtilities.hpp"
#include "ExporterUtilities.hpp"
#include "MayaPlug.hpp"
#include "MayaUtilities.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyValues.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
void MayaBaseMaterialExporterHandler::WriteDiffuseMap(WxDataChunkWriter& writer, size_t& mapIndex, TextureSlot textureSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    if (textureSlot.isNull() || !textureSlot.obj.hasFn(MFn::kFileTexture))
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(MFnDependencyNode(textureSlot.obj).name()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, textureSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        HandleTexture(writer, textureSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_DIFFUSE, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

void MayaBaseMaterialExporterHandler::WriteSpecularMap(WxDataChunkWriter& writer, size_t& mapIndex, TextureSlot textureSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    if (textureSlot.isNull() || !textureSlot.obj.hasFn(MFn::kFileTexture))
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(MFnDependencyNode(textureSlot.obj).name()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, textureSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        HandleTexture(writer, textureSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_SPECULAR, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

void MayaBaseMaterialExporterHandler::WriteHeightMap(WxDataChunkWriter& writer, size_t& mapIndex, TextureSlot textureSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    if (textureSlot.isNull() || !textureSlot.obj.hasFn(MFn::kFileTexture))
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(MFnDependencyNode(textureSlot.obj).name()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, textureSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        HandleTexture(writer, textureSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_HEIGHT, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

void MayaBaseMaterialExporterHandler::WriteReflectionMap(WxDataChunkWriter& writer, size_t& mapIndex, TextureSlot textureSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    if (textureSlot.isNull() || !textureSlot.obj.hasFn(MFn::kFileTexture))
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(MFnDependencyNode(textureSlot.obj).name()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, textureSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        HandleTexture(writer, textureSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_REFLECTION, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

void MayaBaseMaterialExporterHandler::WriteBumpMap(WxDataChunkWriter& writer, size_t& mapIndex, TextureSlot textureSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    if (textureSlot.isNull() || !textureSlot.obj.hasFn(MFn::kFileTexture))
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(MFnDependencyNode(textureSlot.obj).name()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, textureSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        HandleTexture(writer, textureSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_BUMP, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

void MayaBaseMaterialExporterHandler::WriteSelfIlluminationMap(WxDataChunkWriter& writer, size_t& mapIndex, TextureSlot textureSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    if (textureSlot.isNull() || !textureSlot.obj.hasFn(MFn::kFileTexture))
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(MFnDependencyNode(textureSlot.obj).name()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, textureSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        HandleTexture(writer, textureSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_SELF_ILLUMINATION, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

void MayaBaseMaterialExporterHandler::WriteOpacityMap(WxDataChunkWriter& writer, size_t& mapIndex, TextureSlot textureSlot, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    if (textureSlot.isNull() || !textureSlot.obj.hasFn(MFn::kFileTexture))
        return;

    writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, ApplicationStringToWxString(MFnDependencyNode(textureSlot.obj).name()));

    WxChunkName mapChunkName(StandardAssetDocumentChunkNames::MAP, mapIndex++);
    writer.WriteChunk(mapChunkName, [this, textureSlot](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        HandleTexture(writer, textureSlot, StandardAssetDocumentPropertyValues::TypeName::MATERIAL_MAP_OPACITY, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

void MayaBaseMaterialExporterHandler::HandleTexture(WxDataChunkWriter& writer, TextureSlot textureSlot, const wxString& type, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, type, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    writer.WriteString(StandardAssetDocumentPropertyNames::NAME, writer.GetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME), error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    if (textureSlot.amount < 1.0f)
    {
        writer.WriteFloat(StandardAssetDocumentPropertyNames::AMOUNT, textureSlot.amount, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    MFnDependencyNode textureDepNode(textureSlot.obj);

    //File name
    MString mayaFileName;
    MRenderUtil::exactFileTextureName(textureSlot.obj, mayaFileName);
    if (mayaFileName.length() > 0)
    {
        auto textureFileName = ApplicationStringToWxString(mayaFileName);

        //Store the file name reference
        AddBitmapFileName(textureFileName);

        //Texture file name
        auto resolvedPaths = this->sceneExportSettings->ResolveAssetPaths(AssetClass::TEXTURE, textureFileName);
        auto assetRef = this->sceneExportSettings->GetAssetFileReference(AssetClass::TEXTURE, resolvedPaths);
        writer.WriteString(StandardAssetDocumentPropertyNames::TEXTURE_REF, assetRef.ToUriString(), error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    MPlug fileHasAlphaPlug = textureDepNode.findPlug("fileHasAlpha");
    if (!fileHasAlphaPlug.isNull() && fileHasAlphaPlug.asBool())
    {
        writer.WriteString(StandardAssetDocumentPropertyNames::TEXTURE_ALPHA, StandardAssetDocumentPropertyValues::TextureAlpha::STANDARD, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Get connections to uvCoord attribute of textureSlot node
    MPlugArray uvCoordInputs;
    textureDepNode.findPlug("uvCoord").connectedTo(uvCoordInputs, true, false);

    //Get place2dtexture node
    MFnDependencyNode place2dTexture;
    for (unsigned int j = 0; j < uvCoordInputs.length(); j++)
    {
        if (uvCoordInputs[j].node().hasFn(MFn::kPlace2dTexture))
        {
            place2dTexture.setObject(uvCoordInputs[j].node());
            break;
        }
    }

    //Get various textureSlot settings and uvChooser
    //MFnDependencyNode uvChooser;
    if (!place2dTexture.object().isNull())
    {
        //UV chooser
        /*MPlugArray placetexSrcPlugs;
        place2dTexture.findPlug("uvCoord").connectedTo(placetexSrcPlugs, true, false);
        for (unsigned int j = 0; j < placetexSrcPlugs.length(); j++)
        {
            if (placetexSrcPlugs[j].node().hasFn(MFn::kUvChooser))
            {
                uvChooser.setObject(placetexSrcPlugs[j].node());
                break;
            }
        }*/

        //Addressing mode
        wxString addressModes[3] =
            {
            StandardAssetDocumentPropertyValues::TextureAddressMode::WRAP,
            StandardAssetDocumentPropertyValues::TextureAddressMode::WRAP,
            StandardAssetDocumentPropertyValues::TextureAddressMode::WRAP
            };
        //U addressing mode
        bool mirrorU;
        //place2dTexture.findPlug("wrapU").getValue(wrapU);
        place2dTexture.findPlug("mirrorU").getValue(mirrorU);
        if (mirrorU)
            addressModes[0] = StandardAssetDocumentPropertyValues::TextureAddressMode::MIRROR;

        //V addressing mode
        bool mirrorV;
        //place2dTexture.findPlug("wrapV").getValue(wrapV);
        place2dTexture.findPlug("mirrorV").getValue(mirrorV);
        if (mirrorV)
            addressModes[1] = StandardAssetDocumentPropertyValues::TextureAddressMode::MIRROR;

        //Texture scaling
        double coverage[2];
        place2dTexture.findPlug("coverageU").getValue(coverage[0]);
        place2dTexture.findPlug("coverageV").getValue(coverage[1]);
        double repeat[2];
        place2dTexture.findPlug("repeatU").getValue(repeat[0]);
        place2dTexture.findPlug("repeatV").getValue(repeat[1]);
        float exportedScale[3];
        if (!MathUtilities::AlmostZero(coverage[0] - 1) || !MathUtilities::AlmostZero(coverage[1] - 1))
        {
            exportedScale[0] = 1/coverage[0];
            exportedScale[1] = 1/coverage[1];
        }
        else
        {
            exportedScale[0] = repeat[0];
            exportedScale[1] = repeat[1];
        }
        exportedScale[2] = 1.0f;

        //Texture scrolling
        double translation[2];
        place2dTexture.findPlug("translateFrameU").getValue(translation[0]);
        place2dTexture.findPlug("translateFrameV").getValue(translation[1]);
        double offset[2];
        place2dTexture.findPlug("offsetU").getValue(offset[0]);
        place2dTexture.findPlug("offsetV").getValue(offset[1]);
        float exportedScroll[3];
        if (!MathUtilities::AlmostZero(translation[0]) || !MathUtilities::AlmostZero(translation[1]))
        {
            exportedScroll[0] = translation[0];
            exportedScroll[1] = -translation[1];
        }
        else
        {
            exportedScroll[0] = offset[0];
            exportedScroll[1] = -offset[1];
        }
        exportedScroll[2] = 0.0f;

        //Texture rotation
        double rotateFrame;
        place2dTexture.findPlug("rotateFrame").getValue(rotateFrame);
        double rotateUV;
        place2dTexture.findPlug("rotateUV").getValue(rotateUV);
        double exportedRotation;
        if (!MathUtilities::AlmostZero(rotateFrame))
            exportedRotation = -rotateFrame;
        else
            exportedRotation = -rotateUV;

        //Finally, set everything
        if (exportedScale[0] != 1 && exportedScale[1] != 1)
        {
            writer.WriteFloats(StandardAssetDocumentPropertyNames::TEXTURE_SCALE, exportedScale, 3, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }

        if (exportedRotation != 0)
        {
            writer.WriteFloat(StandardAssetDocumentPropertyNames::TEXTURE_ROTATION, exportedRotation, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }

        if (exportedScroll[0] != 0 && exportedScroll[1] != 0)
        {
            writer.WriteFloats(StandardAssetDocumentPropertyNames::TEXTURE_OFFSET, exportedScroll, 3, error);
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

MObject MayaBaseMaterialExporterHandler::FindFileTexture(MFnDependencyNode& shader, MString texturePlugName)
{
    MObject textureSlot;

    MPlugArray colorInputs;
    shader.findPlug(texturePlugName).connectedTo(colorInputs, true, false);
    for (unsigned int i = 0; i < colorInputs.length() && textureSlot.isNull(); i++)
    {
        MObject maybeTexture = colorInputs[i].node();
        if (maybeTexture.hasFn(MFn::kFileTexture))
            textureSlot = maybeTexture;
    }

    return textureSlot;
}

void MayaBaseMaterialExporterHandler::FindFileTextures(std::vector<Texture>& textures, MFnDependencyNode& shader, MString texturePlugName)
{
    MPlugArray colorInputs;
    shader.findPlug(texturePlugName).connectedTo(colorInputs, true, false);
    for (unsigned int i = 0; i < colorInputs.length(); i++)
    {
        MObject maybeTexture = colorInputs[i].node();
        if (maybeTexture.hasFn(MFn::kFileTexture))
            textures.push_back(maybeTexture);
        else if (maybeTexture.hasFn(MFn::kLayeredTexture))
        {
            MFnDependencyNode layerShader(maybeTexture);
            MPlug inputsPlug = layerShader.findPlug("inputs");
            for (int inputIndex = (int)inputsPlug.evaluateNumElements() - 1; inputIndex >= 0; inputIndex--)
            {
                MPlug inputPlug = inputsPlug[inputIndex];

                MPlugArray moreColorInputs;
                inputPlug.child(0).connectedTo(moreColorInputs, true, false);
                for (unsigned int colorInputIndex = 0; colorInputIndex < moreColorInputs.length(); colorInputIndex++)
                {
                    if (moreColorInputs[colorInputIndex].node().hasFn(MFn::kFileTexture))
                    {
                        Texture textureSlot(moreColorInputs[colorInputIndex].node());

                        MPlug blendModePlug = MayaPlug::FindChildPlug(inputPlug, "bm");
                        if (!blendModePlug.isNull())
                            textureSlot.blendMode = (MayaBlendMode)blendModePlug.asShort();

                        textures.push_back(textureSlot);

                        break;
                    }
                }
            }
        }
    }
}
