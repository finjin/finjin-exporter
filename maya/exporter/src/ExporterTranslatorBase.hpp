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


//Macros-----------------------------------------------------------------------
#define FINJIN_DECLARE_TRANSLATOR(name, base) \
    class name : public base \
    { \
    public: \
        MString defaultExtension() const {return FILE_EXTENSION;} \
        MString filter() const {return FILE_FILTER;} \
        static void* Creator() {return new name();} \
        static MString GetTranslatorName() {return TRANSLATOR_NAME;} \
    public: \
        static const MString FILE_EXTENSION; \
        static const MString FILE_FILTER; \
        static const MString FILE_VERSION; \
        static const MString TRANSLATOR_NAME; \
    };

#define FINJIN_IMPLEMENT_TRANSLATOR(name, version, extension, filter, desc) \
    const MString name::FILE_VERSION = version; \
    const MString name::FILE_EXTENSION = extension; \
    const MString name::FILE_FILTER = filter; \
    const MString name::TRANSLATOR_NAME = desc;

    
//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {
    
    class ExporterTranslatorBase : public MPxFileTranslator
    {
    public:
        bool haveWriteMethod() const override {return true;}        
    };

} }
