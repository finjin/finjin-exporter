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


//Includes---------------------------------------------------------------------
#include "MaterialExporterHandler.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Base handler for exporting materials. */
    class MaxBaseMaterialExporterHandler : public MaterialExporterHandler
    {
    protected:
        struct TexMapSlot
        {
            TexMapSlot()
            {
                this->texmap = nullptr;
                this->amount = 1.0f;
            }

            void operator = (nullptr_t)
            {
                this->texmap = nullptr;
                this->amount = 1.0f;
            }

            bool operator == (nullptr_t) const
            {
                return this->texmap == nullptr;
            }

            Texmap* texmap;
            float amount;
        };
        
        Texmap* GetDiffuseMap(Texmap* texmap);
        void WriteDiffuseMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error);

        Texmap* GetSpecularMap(Texmap* texmap);
        void WriteSpecularMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error);

        Texmap* GetHeightMap(Texmap* texmap);
        void WriteHeightMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error);

        Texmap* GetReflectionMap(Texmap* texmap);
        void WriteReflectionMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error);

        Texmap* GetRefractionMap(Texmap* texmap);
        void WriteRefractionMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error);

        Texmap* GetBumpMap(Texmap* texmap);
        void WriteBumpMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error);

        Texmap* GetSelfIlluminationMap(Texmap* texmap);
        void WriteSelfIlluminationMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error);
        
        Texmap* GetOpacityMap(Texmap* texmap);
        void WriteOpacityMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error);        

        Texmap* GetShininessMap(Texmap* texmap);
        void WriteShininessMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error);

        Texmap* GetEnvironmentMap(Texmap* texmap);
        void WriteEnvironmentMap(WxDataChunkWriter& writer, size_t& mapIndex, TexMapSlot texmapSlot, WxError& error);

        void WriteTexmap(WxDataChunkWriter& writer, TexMapSlot texmapSlot, const wxString& type, WxError& error);        
        
        Texmap* GetExportableBitmapTexmap(Texmap* texmap);
    };    

} }
