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
#include "MeshMorpher.hpp"
#include "MaxUtilities.hpp"
#include "MorpherApi.h"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
MeshMorpher::MeshMorpher(GeometryStateBase* geometryState) : MeshMorpherBase(geometryState)
{
}

void MeshMorpher::Clear()
{
    MeshMorpherBase::Clear();

    this->morpherModifier.Clear();
}

bool MeshMorpher::Initialize(INode* maxNode, const CoordinateSystemConverter& coordinateConverter, float scale, PrimitiveType meshPrimitiveType)
{
    Clear();

    this->morpherModifier = MaxUtilities::FindMorpherModifier(maxNode);
    if (this->morpherModifier.IsValid())
    {    
        //Get the morph targets
        MaxMorphModifier actualMorpherModifier(this->morpherModifier.modifier);
        
        auto channelCount = (size_t)actualMorpherModifier.NumMorphChannels();
        for (size_t channelIndex = 0; channelIndex < channelCount; channelIndex++)
        {
            auto channel = actualMorpherModifier.GetMorphChannel((int)channelIndex);
            auto morphPointCount = (size_t)channel.NumMorphPoints();
            auto morphTargetNode = channel.GetMorphTarget();
            
            if (channel.IsActive() && morphTargetNode != nullptr)
            {
                MeshMorphTargetPtr morphTarget(new MeshMorphTarget(channel.GetMorphWeightController()));
                AddMorphTarget(morphTarget);
                
                if (morphTargetNode != nullptr)
                {
                    //The channel references an object in the scene

                    auto morphObject = morphTargetNode->EvalWorldState(0).obj;
                    morphTarget->object = morphTargetNode;

                    //Get morph target name
                    morphTarget->name = morphTarget->object.GetLocalName();

                    //Get the morph target components
                    if (this->geometryState != nullptr && morphObject->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
                    {
                        auto morphTriObject = static_cast<TriObject*>(morphObject->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0)));
                        auto& morphMesh = morphTriObject->GetMesh();

                        //Positions
                        morphTarget->creationPositions.resize(morphMesh.numVerts);
                        for (int pointIndex = 0; pointIndex < morphMesh.numVerts; pointIndex++)
                        {
                            Point3 point = morphMesh.verts[pointIndex] * scale;
                            morphTarget->creationPositions[pointIndex].Set(point.x, point.y, point.z);
                            coordinateConverter.ConvertPoint(morphTarget->creationPositions[pointIndex]);
                        }

                        //Normals
                        morphTarget->creationNormals.Create(morphMesh, coordinateConverter, meshPrimitiveType);

                        if (morphTriObject != morphObject)
                            morphTriObject->DeleteThis();
                    }
                }
                else
                {
                    //The channel has the offsets stored within it

                    //Get morph target name
                    morphTarget->name = wxString::Format(wxT("%d"), this->morphTargets.size() - 1);

                    //Get the morph target points
                    if (this->geometryState != nullptr)
                    {
                        morphTarget->creationPositions.resize(morphPointCount);
                        for (size_t pointIndex = 0; pointIndex < morphPointCount; pointIndex++)
                        {
                            Point3 point = channel.GetMorphPoint((int)pointIndex);
                            point *= scale;

                            morphTarget->creationPositions[pointIndex].Set(point.x, point.y, point.z);
                            coordinateConverter.ConvertPoint(morphTarget->creationPositions[pointIndex]);                                                    
                        }
                    }
                }
            }
        }
    }

    return IsValid();
}

bool MeshMorpher::IsValid() const
{
    return !this->morphTargets.empty();
}

void MeshMorpher::Enable(bool enable)
{
    this->morpherModifier.Enable(enable);
}

void MeshMorpher::Restore()
{
    this->morpherModifier.Restore();
}

bool MeshMorpher::HasMorpher(ObjectAccessor object)
{
    return MaxUtilities::FindMorpherModifier(object.node) != nullptr;
}
