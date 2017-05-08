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

    /** A single weighted vertex. */
    struct WeightedVertex
    {
        WeightedVertex()
        {
            this->assignments.reserve(FINJIN_MAX_BLEND_WEIGHTS);
        }

        struct Assignment
        {
            Assignment()
            {
                this->weight = 0;
            }

            Assignment(ObjectAccessor bone, float weight)
            {
                this->bone = bone;
                this->weight = weight;
            }

            ObjectAccessor bone;
            float weight;
        };

        void AddBoneInfluence(ObjectAccessor bone, float weight)
        {
            if (weight > 0)
                this->assignments.push_back(Assignment(bone, weight));
        }

        bool HasBoneInfluence(ObjectAccessor bone) const
        {
            for (size_t i = 0; i < this->assignments.size(); i++)
            {
                if (this->assignments[i].bone == bone)
                    return true;
            }
            return false;
        }

        bool HasSameBones(const WeightedVertex& other) const
        {
            if (this->assignments.size() != other.assignments.size())
                return false;

            for (size_t i = 0; i < other.assignments.size(); i++)
            {
                if (!HasBoneInfluence(other.assignments[i].bone))
                    return false;
            }

            return true;
        }

        float GetTotalWeight() const
        {
            float weight = 0;
            for (size_t i = 0; i < this->assignments.size(); i++)
                weight += this->assignments[i].weight;
            return weight;
        }

        void ScaleWeights(float scale)
        {
            for (size_t i = 0; i < this->assignments.size(); i++)
                this->assignments[i].weight *= scale;
        }

        void RemoveExcessAssignments(size_t maxAssignments = FINJIN_MAX_BLEND_WEIGHTS)
        {
            if (this->assignments.size() > maxAssignments)
            {
                //Sort in weight descending order
                std::sort(this->assignments.begin(), this->assignments.end(), SortBoneInfluenceDescending);

                //Remove excess weights
                this->assignments.resize(maxAssignments);

                //Rescale weights
                float totalWeight = GetTotalWeight();
                if (totalWeight > 0)
                    ScaleWeights(1 / totalWeight);
            }
        }

        void clear()
        {
            this->assignments.clear();
        }

        /** Sorts vertex bone assignments in descending weight order */
        static bool SortBoneInfluenceDescending(const Assignment& a1, const Assignment& a2)
        {
            return a1.weight > a2.weight;
        }

        std::vector<Assignment> assignments;
    };

} }
