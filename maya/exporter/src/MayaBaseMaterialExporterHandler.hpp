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


#pragma once


//Includes----------------------------------------------------------------------
#include "MaterialExporterHandler.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Base handler for exporting materials. */
    class MayaBaseMaterialExporterHandler : public MaterialExporterHandler
    {
    protected:
        struct TextureSlot
        {
            TextureSlot()
            {
                this->amount = 1.0f;
            }

            void Reset()
            {
                this->obj = MObject();
                this->amount = 1.0f;
            }

            bool isNull() const
            {
                return this->obj.isNull();
            }

            MObject obj;
            float amount;
        };

        enum MayaBlendMode
        {
            MAYA_BLEND_NONE = 0,
            MAYA_BLEND_OVER = 1,
            MAYA_BLEND_IN = 2,
            MAYA_BLEND_OUT = 3,
            MAYA_BLEND_ADD = 4,
            MAYA_BLEND_SUBTRACT = 5,
            MAYA_BLEND_MULTIPLY = 6,
            MAYA_BLEND_DIFFERENCE = 7,
            MAYA_BLEND_LIGHTEN = 8,
            MAYA_BLEND_DARKEN = 9,
            MAYA_BLEND_SATURATE = 10,
            MAYA_BLEND_DESATURATE = 11,
            MAYA_BLEND_ILLUMINATE = 12
        };

        struct Texture
        {
            MayaBlendMode blendMode;
            MObject obj;

            Texture()
            {
                blendMode = MAYA_BLEND_NONE;
            }

            Texture(MObject o) : obj(o)
            {
                blendMode = MAYA_BLEND_NONE;
            }
        };

        void WriteDiffuseMap(WxDataChunkWriter& writer, size_t& mapIndex, TextureSlot textureSlot, WxError& error);
        void WriteSpecularMap(WxDataChunkWriter& writer, size_t& mapIndex, TextureSlot textureSlot, WxError& error);
        void WriteHeightMap(WxDataChunkWriter& writer, size_t& mapIndex, TextureSlot textureSlot, WxError& error);
        void WriteReflectionMap(WxDataChunkWriter& writer, size_t& mapIndex, TextureSlot textureSlot, WxError& error);
        void WriteBumpMap(WxDataChunkWriter& writer, size_t& mapIndex, TextureSlot textureSlot, WxError& error);
        void WriteSelfIlluminationMap(WxDataChunkWriter& writer, size_t& mapIndex, TextureSlot textureSlot, WxError& error);
        void WriteOpacityMap(WxDataChunkWriter& writer, size_t& mapIndex, TextureSlot textureSlot, WxError& error);

        void HandleTexture(WxDataChunkWriter& writer, TextureSlot textureSlot, const wxString& type, WxError& error);

        static MObject FindFileTexture(MFnDependencyNode& shader, MString texturePlugName);
        static void FindFileTextures(std::vector<Texture>& textures, MFnDependencyNode& shader, MString texturePlugName);
    };

} }
