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
#include "WeightedVertex.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Maps an original vertex to one or more other vertices. */
    struct ReindexedVertex
    {
        ReindexedVertex()
        {
            this->pointIndices.reserve(3);            
        }

        typedef std::vector<unsigned int> PointIndices;

        bool empty() const {return this->pointIndices.empty();}
        void clear() {this->pointIndices.clear(); this->weightedVertex.clear();}
        size_t size() const {return this->pointIndices.size();}
        void resize(size_t newSize) {this->pointIndices.resize(newSize);}
        unsigned int& operator [] (size_t i) {return this->pointIndices[i];}
        const unsigned int& operator [] (size_t i) const {return this->pointIndices[i];}
        PointIndices::iterator begin() {return this->pointIndices.begin();}
        PointIndices::iterator end() {return this->pointIndices.end();}
        void push_back(unsigned int value) {this->pointIndices.push_back(value);}

        /** Maps an original vertex to one or more exported vertices. */
        PointIndices pointIndices;

        /** Connects an origianl vertex to zero or more bones. */
        WeightedVertex weightedVertex;
    };

    typedef std::vector<ReindexedVertex> ReindexedVertices;

} }
