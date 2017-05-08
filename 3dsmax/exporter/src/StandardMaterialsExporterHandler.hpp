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
#include "MaxBaseMaterialExporterHandler.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Handler for exporting standard application materials. */
    class StandardMaterialsExporterHandler : public MaxBaseMaterialExporterHandler
    {
    public:
        StandardMaterialsExporterHandler();

        void CalculateRequirements() override;

        void Write(WxDataChunkWriter& writer, WxError& error) override;

        static bool CanHandle(MaterialAccessor material);

        enum class TexMapIndex
        {
            DIFFUSE, //3DS Max Standard material: Diffuse map
            SPECULAR, //3DS Max Standard material: Specular map
            HEIGHT, //3DS Max Standard material: Displacement map
            BUMP, //3DS Max Standard material: Bump map
            SELF_ILLUMINATION, //3DS Max Standard material: Self illumination map
            REFLECTION, //3DS Max Standard material: Reflection map
            REFRACTION, //3DS Max Standard material: Refraction map
            OPACITY, //3DS Max Standard material: Opacity map
            SHININESS, //3DS Max Standard material: Glossiness
            ENVIRONMENT, //3DS Max Standard material: Ambient

            COUNT
        };

    private:
        EnumArray<TexMapIndex, TexMapIndex::COUNT, TexMapSlot> texmaps;
        size_t texmapCount;
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
