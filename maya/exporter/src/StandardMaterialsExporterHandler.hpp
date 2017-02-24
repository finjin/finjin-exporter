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
#include "MayaBaseMaterialExporterHandler.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Handler for exporting standard application materials. */
    class StandardMaterialsExporterHandler : public MayaBaseMaterialExporterHandler
    {
    public:
        StandardMaterialsExporterHandler();

        void CalculateRequirements() override;

        void Write(WxDataChunkWriter& writer, WxError& error) override;

        static bool CanHandle(MaterialAccessor material);

        enum class TexMapIndex
        {
            DIFFUSE, //Maya Standard material: Diffuse map
            SPECULAR, //Maya Standard material: Specular map
            HEIGHT, //Maya Standard material: Displacement map
            BUMP, //Maya Standard material: Bump map
            SELF_ILLUMINATION, //Maya Standard material: Self illumination map
            REFLECTION, //Maya Standard material: Reflection map
            OPACITY, //Maya Standard material: Opacity map

            COUNT
        };

    private:
        EnumValues<TexMapIndex, TexMapIndex::COUNT, TextureSlot> textures;
        size_t texmapCount;
        std::vector<Texture> colorTextures; //Textures on the 'color' plug
    };

    /** Handler factory for exporting standard application materials. */
    class StandardMaterialsExporterHandlerFactory : public MaterialExporterHandlerFactory
    {
    public:
        bool CanHandle(MaterialAccessor material) const override
        {
            return StandardMaterialsExporterHandler::CanHandle(material);
        }

        MaterialExporterHandler* CreateHandler() override
        {
            return new StandardMaterialsExporterHandler;
        }

        static StandardMaterialsExporterHandlerFactory& GetInstance()
        {
            static StandardMaterialsExporterHandlerFactory instance;
            return instance;
        }
    };

} }
