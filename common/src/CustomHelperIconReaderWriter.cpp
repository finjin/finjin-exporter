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
#include "CustomHelperIconReaderWriter.hpp"
#include "finjin/common/WxConfigDocumentReader.hpp"
#include "finjin/common/WxConfigDocumentWriter.hpp"
#include "FileUtilities.hpp"

using namespace Finjin::Exporter;

static void WriteVertex(WxConfigDocumentWriter& writer, size_t index, const FinjinVector3& v)
{
    wxString indexString;
    indexString << index;

    wxString vertexString;
    vertexString << v.x;
    vertexString << wxT(" ");
    vertexString << v.y;
    vertexString << wxT(" ");
    vertexString << v.z;
    writer.WriteKeyAndValue(indexString, vertexString);
}

static FinjinVector3 ParseVertex(const wxString& value)
{
    wxStringTokenizer valuesTokenizer(value, wxT(" "));
    wxArrayString values;
    while (valuesTokenizer.HasMoreTokens())
        values.Add(valuesTokenizer.GetNextToken());

    FinjinVector3 result;
    double doubleValue;
    size_t elementCount = std::min((size_t)3, value.size());
    for (size_t i = 0; i < elementCount; i++)
    {
        values[i].ToDouble(&doubleValue);
        result[i] = (float)doubleValue;
    }
    return result;
}

static bool WriteToConfig(CustomHelperIconWriter& iconWriter, WxConfigDocumentWriter& writer, const wxString& vector3Type = wxT("Vector3"))
{
    auto& vertices = iconWriter.vertices;
    auto& edges = iconWriter.edges;

    //Calculate number of vertices
    auto vertexCount = !edges.empty() ? edges.size() * 2 : vertices.size();

    //Make sure there are a valid number of vertices
    if (vertexCount == 0 || vertexCount % 2 != 0)
        return false;

    writer.WriteScopeStart(wxT("vertices"));
    {
        wxString countString;
        countString << vertexCount;
        writer.WriteKeyAndValue(wxT("count"), countString);

        size_t vertexIndex = 0;
        if (!edges.empty())
        {
            //Use edges
            for (size_t i = 0; i < edges.size(); i++)
            {
                WriteVertex(writer, vertexIndex++, vertices[edges[i].v1]);
                WriteVertex(writer, vertexIndex++, vertices[edges[i].v2]);
            }        
        }
        else
        {
            //Use vertices
            for (size_t i = 0; i < vertices.size(); i += 2)
            {
                WriteVertex(writer, vertexIndex++, vertices[i]);
                WriteVertex(writer, vertexIndex++, vertices[i + 1]);
            }        
        }
    }
    writer.WriteScopeEnd();    

    return true;
}

template <typename T>
bool WriteCodeToStream(CustomHelperIconWriter& iconWriter, T& out, const wxString& vector3Type = wxT("Vector3"))
{
    auto& vertices = iconWriter.vertices;
    auto& edges = iconWriter.edges;

    //Calculate number of vertices
    auto vertexCount = !edges.empty() ? edges.size() * 2 : vertices.size();

    //Make sure there are a valid number of vertices
    if (vertexCount == 0 || vertexCount % 2 != 0)
        return false;

    out << "static const " << vector3Type << " ICON_EDGE_VERTICES[] = " << std::endl;
    out << "    {" << std::endl;
        
    if (!edges.empty())
    {
        //Use edges
        for (size_t i = 0; i < edges.size(); i++)
        {
            out 
                << "    " << vector3Type << "(" << vertices[edges[i].v1].x << ", " << vertices[edges[i].v1].y << ", " << vertices[edges[i].v1].z << ", "
                << vector3Type << "(" << vertices[edges[i].v2].x << ", " << vertices[edges[i].v2].y << ", " << vertices[edges[i].v2].z << "";
                
            if (i < edges.size() - 1)
                out << ",";
            out << std::endl;
        }
    }
    else
    {
        //Use vertices
        for (size_t i = 0; i < vertices.size(); i += 2)
        {
            out 
                << "    " << vector3Type << "(" << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << ", "
                << vector3Type << "(" << vertices[i + 1].x << ", " << vertices[i + 1].y << ", " << vertices[i + 1].z << "";
                
            if (i < vertices.size() - 2)
                out << ",";
            out << std::endl;
        }
    }

    out << "    };" << std::endl;

    return true;
}

//Implementation---------------------------------------------------------------
bool CustomHelperIconWriter::IsValid() const
{
    return !this->vertices.empty();
}

void CustomHelperIconWriter::Normalize()
{
    //Get maximum length
    auto maxLength = 0.0f;
    for (size_t i = 0; i < this->vertices.size(); i++)
    {
        auto length = this->vertices[i].LengthSquared();
        if (length > maxLength)
            maxLength = length;
    }
    
    //Normalize
    if (maxLength > 0)
    {
        maxLength = sqrtf(maxLength);
        auto oneOverMaxLength = 1.0f/maxLength;
        for (size_t i = 0; i < this->vertices.size(); i++)
            this->vertices[i] *= oneOverMaxLength;
    }
}

bool CustomHelperIconWriter::Write(const wxString& fileName, const wxString& vector3Type)
{
    //Calculate number of vertices
    size_t vertexCount = !this->edges.empty() ? this->edges.size() * 2 : this->vertices.size();

    //Make sure there are a valid number of vertices
    if (vertexCount == 0 || vertexCount % 2 != 0)
        return false;

    //Determine if XML should be written
    wxFileName fileNameObject(fileName);
    wxString baseFileName = fileNameObject.GetName();
    auto saveAsConfig = !baseFileName.empty() && baseFileName[0] != wxT('_');

    if (saveAsConfig)
    {
        wxFile outFile(fileName, wxFile::write);
        if (!outFile.IsOpened())
            return false;

        WxFileDataChunkWriterOutput output(outFile);
        WxConfigDocumentWriter writer;
        writer.Create(output);    
    
        return WriteToConfig(*this, writer, vector3Type);
    }
    else
    {
    #if wxUSE_UNICODE    
        std::wofstream outFile;    
    #else
        std::ofstream outFile; 
    #endif
        
    #if defined(__WXMSW__)
        outFile.open(fileName.wx_str(), std::ios::out);
    #else
        auto fileNameUtf8 = fileName.ToUTF8();
        outFile.open(fileNameUtf8.data(), std::ios::out);
    #endif
        if (!outFile.is_open())
            return false;

        return WriteCodeToStream(*this, outFile, vector3Type);
    }
}

bool CustomHelperIconReader::Read(const wxString& fileName)
{
    this->vertices.clear();

    auto result = false;

    std::vector<uint8_t> fileContent;
    if (!FileUtilities::ReadBinaryFile(fileName, fileContent))
        return false;
    
    WxConfigDocumentReader reader;
    wxString currentSectionName;
    FinjinVector3 tempVertex;
    for (auto line = reader.Start(fileContent); line != nullptr; line = line = reader.Next())
    {
        switch (line->GetType())
        {
            case WxConfigDocumentLine::Type::SECTION: 
            {
                currentSectionName = line->GetSectionName();
                break;
            }
            case WxConfigDocumentLine::Type::SCOPE_START:
            {
                break;
            }
            case WxConfigDocumentLine::Type::SCOPE_END:
            {
                currentSectionName.clear();
                break;
            }
            case WxConfigDocumentLine::Type::KEY_AND_VALUE:
            {
                auto key = line->GetKey();
                auto value = line->GetValue();

                if (currentSectionName == wxT("vertices"))
                {
                    if (key == wxT("count"))
                    {
                        unsigned long count;
                        value.ToULong(&count);
                        this->vertices.reserve((size_t)count);
                    }
                    else
                    {
                        tempVertex = ParseVertex(value);
                        this->vertices.push_back(tempVertex);
                    }
                }

                break;
            }
            default: break;
        }
    }

    //The load is considered successful if there was a non-zero, even number of vertices
    result = !this->vertices.empty() && this->vertices.size() % 2 == 0;

    //Clear out the vertices if there was a failure
    if (!result)
        this->vertices.clear();
    
    return result;
}
