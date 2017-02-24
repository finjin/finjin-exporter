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

    /** 
     * A material exporter handler that exports a default material.
     * This is typically used as a fallback whenever an unsupported material is encountered.
     */
    class DefaultMaterialExporterHandler : public MaterialExporterHandler
    {
    public:
        void Write(WxDataChunkWriter& writer, WxError& error) override;
    };

    class DefaultMaterialExporterHandlerFactory : public MaterialExporterHandlerFactory
    {
    public:
        bool CanHandle(MaterialAccessor material) const override
        {
            return true;
        }

        MaterialExporterHandler* CreateHandler() override
        {
            return new DefaultMaterialExporterHandler;
        }

        static DefaultMaterialExporterHandlerFactory& GetInstance()
        {
            static DefaultMaterialExporterHandlerFactory instance;
            return instance;
        }
    };

} }
