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

    /** Handler for exporting Raytrace materials. */
    class RaytraceMaterialExporterHandler : public MaxBaseMaterialExporterHandler
    {
    public:
        RaytraceMaterialExporterHandler();

        void CalculateRequirements() override;

        void Write(WxDataChunkWriter& writer, WxError& error) override;

        static bool CanHandle(MaterialAccessor material);

    private:
        enum class TexMapIndex
        {
            DIFFUSE,

            COUNT
        };
        EnumArray<TexMapIndex, TexMapIndex::COUNT, TexMapSlot> texmaps;
        size_t texmapCount;
    };

    /** Handler factory for exporting Raytrace materials. */
    class RaytraceMaterialExporterHandlerFactory : public MaterialExporterHandlerFactory
    {
    public:
        bool CanHandle(MaterialAccessor material) const override
        {
            return RaytraceMaterialExporterHandler::CanHandle(material);
        }

        MaterialExporterHandler* CreateHandler() override
        {
            return new RaytraceMaterialExporterHandler;
        }

        static RaytraceMaterialExporterHandlerFactory& GetInstance()
        {
            static RaytraceMaterialExporterHandlerFactory instance;
            return instance;
        }
    };

} }
