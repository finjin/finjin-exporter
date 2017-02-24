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
#include "FinjinVector.hpp"
#include "MatrixAccessor.hpp"
#include "CoordinateSystemConverter.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class CoordinateSystemConverter;

    /** A mesh vertex normal, used by the MeshNormalsBase class. */
    class VertexNormal 
    {
    public:
        VertexNormal();
        VertexNormal(const FinjinVector3& n, unsigned int s);
        ~VertexNormal();

        void Add(FinjinVector3& n, unsigned int smoothingGroups);
        const FinjinVector3& Get(unsigned int smoothingGroups) const;

        void Normalize();

    public:
        FinjinVector3 norm;
        unsigned int smooth;
        VertexNormal* next;
        std::list<FinjinVector3> accum;
    };

    /** A mesh vertex tangent, used by the MeshNormalsBase class. */
    class VertexTangent
    {
    public:
        VertexTangent();
        VertexTangent(const FinjinVector4& t, unsigned int s);
        ~VertexTangent();

        void Add(FinjinVector4& t, unsigned int smoothingGroups);
        const FinjinVector4& Get(unsigned int smoothingGroups) const;

        void Normalize();

    public:
        FinjinVector4 tangent;
        unsigned int smooth;
        VertexTangent* next;
        std::list<FinjinVector4> accum;
    };

    /** Base class for application-specific MeshNormals class. */
    class MeshNormalsBase
    {
    public:
        virtual ~MeshNormalsBase() {}

        void Destroy();

        FinjinVector3 GetNormal(int cornerIndex, int vertexIndex, int faceIndex, unsigned int smoothingGroups);

        FinjinVector4 GetTangent(int cornerIndex, int vertexIndex, int faceIndex, unsigned int smoothingGroups);

    protected:
        void Convert(const CoordinateSystemConverter& coordinateConverter);        

    public:
        //Calculated vertex values
        std::vector<VertexNormal> vertexNormals;
        std::vector<VertexTangent> vertexTangents;

        //Calculated face values
        std::vector<FinjinVector3> faceNormals;
        std::vector<FinjinVector4> faceTangents;

        //Smoothing groups, one entry per face
        std::vector<unsigned int> faceSmoothingGroups;

        /* Normals for a face. Used for holding explicitly defined normals */
        struct NormalFace
        {
            NormalFace()
            {                
            }

            void SetDegree(int degree)
            {
                this->v.resize(degree);
                this->normals.resize(degree);

                for (int i = 0; i < degree; i++)
                {
                    this->v[i] = -1;
                    this->normals[i].Zero();
                }
            }

            void Set(int cornerIndex, int normalIndex, const FinjinVector3& normal)
            {
                this->v[cornerIndex] = normalIndex;
                this->normals[cornerIndex] = normal;
            }

            int GetDegree() const
            {
                return (int)this->v.size();
            }

            std::vector<int> v;
            std::vector<FinjinVector3> normals;
        };

        /* Tangents for a face. Used for holding explicitly defined tangents */
        struct TangentFace
        {
            TangentFace()
            {                
            }

            void SetDegree(int degree)
            {
                this->v.resize(degree);
                this->tangents.resize(degree);

                for (int i = 0; i < degree; i++)
                {
                    this->v[i] = -1;
                    this->tangents[i].Zero();
                }
            }

            void Set(int cornerIndex, int normalIndex, const FinjinVector4& tangent)
            {
                this->v[cornerIndex] = normalIndex;
                this->tangents[cornerIndex] = tangent;
            }

            int GetDegree() const
            {
                return (int)this->v.size();
            }

            std::vector<int> v;
            std::vector<FinjinVector4> tangents;
        };

        std::vector<NormalFace> normalFaces;    
        std::vector<TangentFace> tangentFaces;    
    };

} }
