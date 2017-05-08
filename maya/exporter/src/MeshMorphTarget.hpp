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
#include "TimeChanger.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class MeshMorphTarget : public MeshMorphTargetBase
    {
    public:
        MeshMorphTarget(MObject blendShapeDeformer, int blendShapeIndex, ObjectAccessor targetObject)
        {
            this->blendShapeDeformer.setObject(blendShapeDeformer);
            this->blendShapeIndex = blendShapeIndex;
            this->name = targetObject.GetLocalName();
            this->object = targetObject;
        }

        float GetInfluence(TimeAccessor t)
        {
            if (!this->blendShapeDeformer.object().isNull())
            {
                TimeChanger timeChanger(t);
                return this->blendShapeDeformer.weight(this->blendShapeIndex);
            }
            else
                return 0;
        }

    public:
        /** The blend shape deformer for the mesh. */
        MFnBlendShapeDeformer blendShapeDeformer;

        /** The index of this blend shape in the deformer. */
        int blendShapeIndex;
    };

} }
