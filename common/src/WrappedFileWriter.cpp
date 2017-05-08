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
#include "WrappedFileWriter.hpp"
#include "FileUtilities.hpp"


//Local types-------------------------------------------------------------------
struct Header
{
    enum class FileFormat : uint32_t
    {
        EMBEDDED = 0
    };

    enum class FileFormatClass : uint32_t
    {
        GENERIC = 0,
        IMAGE = 1,
        SOUND = 2
    };

    Header()
    {
        this->magic = 0;
        this->fileFormat = FileFormat::EMBEDDED;
        this->fileFormatVersion = 0;
        this->fileFormatClass = FileFormatClass::GENERIC;
        this->fileFormatClassVersion = 0;
        this->fileExtensionLength = 0;
        this->fileLength = 0;
    }

    static FileFormatClass GetFileFormatClass(const wxString& ext)
    {
        if (ext == wxT("astc") || ext == wxT("bmp") || ext == wxT("dds") || ext == wxT("jpg") || ext == wxT("jpeg") || ext == wxT("ktx") || ext == wxT("pkm") || ext == wxT("png") || ext == wxT("tga"))
            return FileFormatClass::IMAGE;
        else if (ext == wxT("ogg") || ext == wxT("wav"))
            return FileFormatClass::SOUND;
        else
            return FileFormatClass::GENERIC;
    }

    uint32_t magic;
    FileFormat fileFormat;
    uint32_t fileFormatVersion;
    FileFormatClass fileFormatClass;
    uint32_t fileFormatClassVersion;
    uint32_t fileExtensionLength;
    wxString fileExtension;
    uint64_t fileLength;
};

using namespace Finjin::Exporter;


//Local functions---------------------------------------------------------------
template <typename T>
void WriteHeader(T& outFile, const wxString& inFileExtension, uint64_t inFileLength, ByteOrder byteOrder)
{
    if (byteOrder == ByteOrder::NATIVE)
        byteOrder = ::GetByteOrder();
    auto swapBytes = byteOrder != ::GetByteOrder();

    //uint32_t: Finjin magic number
    uint32_t magic = FINJIN_SIGNATURE_FOURCC;
    if (swapBytes)
        SwapBytes(magic);
    outFile.Write(&magic, sizeof(magic));

    //uint32_t: Format
    auto fileFormat = Header::FileFormat::EMBEDDED;
    if (swapBytes)
        SwapBytes(fileFormat);
    outFile.Write(&fileFormat, sizeof(fileFormat));

    //uint32_t: Format version
    uint32_t fileFormatVersion = 1;
    if (swapBytes)
        SwapBytes(fileFormatVersion);
    outFile.Write(&fileFormatVersion, sizeof(fileFormatVersion));

    //uint32_t: File format class
    auto fileFormatClass = Header::GetFileFormatClass(inFileExtension);
    if (swapBytes)
        SwapBytes(fileFormatClass);
    outFile.Write(&fileFormatClass, sizeof(fileFormatClass));

    //uint32_t: File format class version
    uint32_t fileFormatClassVersion = 1;
    if (swapBytes)
        SwapBytes(fileFormatClassVersion);
    outFile.Write(&fileFormatClassVersion, sizeof(fileFormatClassVersion));

    //uint32_t: File extension length
    auto inFileExtensionBytes = inFileExtension.ToUTF8();
    auto inFileExtensionByteCount = strlen(inFileExtensionBytes.data());
    uint32_t fileExtensionLength = static_cast<uint32_t>(inFileExtensionByteCount);
    if (swapBytes)
        SwapBytes(fileExtensionLength);
    outFile.Write(&fileExtensionLength, sizeof(fileExtensionLength));

    //UTF-8[File extension length]: File extension (without leading dot)
    outFile.Write(inFileExtensionBytes.data(), inFileExtensionByteCount);

    //uint64_t: Embedded file length
    if (swapBytes)
        SwapBytes(inFileLength);
    outFile.Write(&inFileLength, sizeof(inFileLength));
}


//Implementation----------------------------------------------------------------
void WrappedFileWriter::Wrap(const wxString& inFilePath, ByteOrder byteOrder, WxByteBuffer& bytes, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    bytes.clear();

    //Get input file extension
    auto inFileExtension = FileUtilities::GetExtension(inFilePath, false);
    if (inFileExtension.empty())
    {
        FINJIN_WX_SET_ERROR(error, wxT("Input file must have an extension."));
        return;
    }

    std::vector<uint8_t> fileContent;
    if (!FileUtilities::ReadBinaryFile(inFilePath, fileContent) || fileContent.empty())
    {
        FINJIN_WX_SET_ERROR(error, wxString::Format(wxT("Failed to read input file '%s' or it was zero length."), inFilePath.wx_str()));
        return;
    }

    WriteHeader(bytes, inFileExtension, fileContent.size(), byteOrder);
    bytes.Write(fileContent.data(), fileContent.size());
}

void WrappedFileWriter::Wrap(const wxString& inFilePath, ByteOrder byteOrder, const wxString& outFilePath, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    //Get input file extension
    auto inFileExtension = FileUtilities::GetExtension(inFilePath, false);
    if (inFileExtension.empty())
    {
        FINJIN_WX_SET_ERROR(error, wxT("Input file must have an extension."));
        return;
    }

    //Get input file length
    uint64_t inFileLength = wxFileName::GetSize(inFilePath).GetValue();
    if (inFileLength == 0)
    {
        FINJIN_WX_SET_ERROR(error, wxString::Format(wxT("Failed to read input file '%s' or it was zero length."), inFilePath.wx_str()));
        return;
    }

    //Write output file----------------------
    wxFile outFile(outFilePath, wxFile::write);
    if (!outFile.IsOpened())
    {
        FINJIN_WX_SET_ERROR(error, wxString::Format(wxT("Failed to open output file '%s'."), outFilePath.wx_str()));
        return;
    }

    WriteHeader(outFile, inFileExtension, inFileLength, byteOrder);

    //Write input file----------------
    wxFile inFile(inFilePath, wxFile::read);
    if (!inFile.IsOpened())
    {
        FINJIN_WX_SET_ERROR(error, wxString::Format(wxT("Failed to open input file '%s'."), inFilePath.wx_str()));
        return;
    }
    FileUtilities::WriteFileTo(inFile, outFile);
}
