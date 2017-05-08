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
#include "MeshMorphTargetBase.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class MeshMorphTarget : public MeshMorphTargetBase
    {
    public:
        MeshMorphTarget(IParamBlock* pblock)
        {
            this->pblock = pblock;
            this->pblock2 = nullptr;
            this->control = nullptr;
        }

        MeshMorphTarget(IParamBlock2* pblock2)
        {
            this->pblock = nullptr;
            this->pblock2 = pblock2;
            this->control = nullptr;
        }

        MeshMorphTarget(Control* control)
        {
            this->pblock = nullptr;
            this->pblock2 = nullptr;
            this->control = control;
        }

        float GetInfluence(TimeAccessor t)
        {
            if (this->pblock != nullptr)
                return this->pblock->GetFloat(0, t.GetNativeTime())/100.0f;
            else if (this->pblock2 != nullptr)
                return this->pblock2->GetFloat(0, t.GetNativeTime())/100.0f;
            else if (this->control != nullptr)
            {
                float value = 0;
                this->control->GetValue(t.GetNativeTime(), &value, FOREVER);
                return value/100.0f;
            }
            else
                return 0;
        }

    private:
        IParamBlock* pblock;
        IParamBlock2* pblock2;
        Control* control;
    };

} }
