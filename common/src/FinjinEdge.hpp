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


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** The vertex indices for a visible edge. */
    struct FinjinEdge
    {
        /** Default constructor. */
        FinjinEdge()
        {
            this->e1 = this->e2 = 0;
        }

        /** Constructor that takes two edge indices, sorting them if necessary. */
        FinjinEdge(uint32_t e1, uint32_t e2)
        {
            //Put the edges into order for easier comparison
            if (e1 <= e2)
            {
                this->e1 = e1;
                this->e2 = e2;
            }
            else
            {
                this->e1 = e2;
                this->e2 = e1;
            }
        }

        size_t GetHash() const
        {
            return HashBytes(reinterpret_cast<const uint8_t*>(&this->e1), sizeof(e1) + sizeof(e2));
        }

        bool operator < (const FinjinEdge& other) const
        {
            return GetHash() < other.GetHash();
        }

        /** Gets the edge specified by index. */
        uint32_t& operator [] (size_t i)
        {
            return (&this->e1)[i];
        }

        /** Gets the edge specified by index. */
        uint32_t operator [] (size_t i) const
        {
            return (&this->e1)[i];
        }

        /** Sorts the edge indices. */
        void Sort()
        {
            if (this->e2 > this->e1)
                std::swap(this->e1, this->e2);
        }

        uint32_t e1;
        uint32_t e2;
    };

    /** A collection of edges. */
    typedef std::set<FinjinEdge> FinjinEdgeSet;

} }
