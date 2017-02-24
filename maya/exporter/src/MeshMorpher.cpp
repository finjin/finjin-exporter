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
#include "ApplicationAccessor.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
MeshMorpher::MeshMorpher(GeometryStateBase* geometryState) : MeshMorpherBase(geometryState)
{
}

MeshMorpher::~MeshMorpher()
{
    Clear();
}

void MeshMorpher::Clear()
{
    MeshMorpherBase::Clear();

    this->blendShapeDeformer.reset();
    this->originalEnvelope = 0;
}

bool MeshMorpher::Initialize(MObject object, const CoordinateSystemConverter& coordinateConverter, float scale, PrimitiveType meshPrimitiveType)
{
    Clear();
    
    MStatus status;
    MFnMesh mesh(object, &status);
    if (status != MStatus::kSuccess)
        return false;

    if (FindMeshBlendShapeDeformer(mesh))
    {
        //Get the morph targets
        std::vector<MDagModifier*> weightConnectionDagModifiers;
        MIntArray weightIndexList;
        std::vector<float> originalWeights;        
        StartMorphTargets(weightConnectionDagModifiers, weightIndexList, originalWeights);

        MObjectArray baseObjects;
        this->blendShapeDeformer->getBaseObjects(baseObjects);
        unsigned int baseObjectCount = std::min(baseObjects.length(), (unsigned int)1); //Only 1 can be supported
        for (unsigned int baseObjectIndex = 0; baseObjectIndex < baseObjectCount; baseObjectIndex++)
        {
            MObject baseObject = baseObjects[baseObjectIndex];

            for (unsigned int weightIndex = 0; weightIndex < (unsigned int)originalWeights.size(); weightIndex++)
            {
                const unsigned int blendShapeIndex = weightIndexList[weightIndex];

                MObjectArray targets;
                this->blendShapeDeformer->getTargets(baseObject, blendShapeIndex, targets);

                unsigned int targetCount = std::min(targets.length(), (unsigned int)1); //Only 1 can be supported
                for (unsigned int targetIndex = 0; targetIndex < targetCount; targetIndex++)
                {
                    ObjectAccessor targetObject(targets[targetIndex]);
                    if (!HasMorphTarget(targetObject))
                    {
                        MeshMorphTargetPtr morphTarget(new MeshMorphTarget(this->blendShapeDeformer->object(), blendShapeIndex, targetObject));
                        AddMorphTarget(morphTarget);

                        //Get the morph target components
                        if (this->geometryState != nullptr)
                        {
                            //Set weight to 1
                            this->blendShapeDeformer->setWeight(blendShapeIndex, 1);
                            
                            //Positions
                            MPointArray meshPoints;
                            mesh.getPoints(meshPoints, GET_POINTS_NORMALS_SPACE);            
                            morphTarget->creationPositions.resize(meshPoints.length());
                            for (unsigned int pointIndex = 0; pointIndex < meshPoints.length(); pointIndex++)
                            {
                                MPoint point(meshPoints[pointIndex]);
                                morphTarget->creationPositions[pointIndex].Set(point.x * scale, point.y * scale, point.z * scale);
                                coordinateConverter.ConvertPoint(morphTarget->creationPositions[pointIndex]);
                            }
                            
                            //Normals
                            morphTarget->creationNormals.Create(mesh, coordinateConverter);

                            //Set weight to 0
                            this->blendShapeDeformer->setWeight(blendShapeIndex, 0);

                            break;
                        }
                    }
                }
            }
        }

        FinishMorphTargets(weightConnectionDagModifiers, weightIndexList, originalWeights);
    }

    return IsValid();
}

bool MeshMorpher::IsValid() const
{
    return !this->morphTargets.empty();
}

void MeshMorpher::Enable(bool enable)
{
    if (this->blendShapeDeformer != nullptr)
    {
        //Disable auto keyframing state
        this->autoKeyState.Disable();

        //Set envelope
        this->blendShapeDeformer->setEnvelope(enable ? this->originalEnvelope : 0);

        //Set auto keyframing state
        this->autoKeyState.Enable(enable);
    }    
}

void MeshMorpher::Restore()
{
    if (this->blendShapeDeformer != nullptr)
    {
        //Disable auto keyframing state
        this->autoKeyState.Disable();

        //Restore envelope
        this->blendShapeDeformer->setEnvelope(this->originalEnvelope);

        //Restore auto keyframing state
        this->autoKeyState.Restore();
    }        
}

bool MeshMorpher::FindMeshBlendShapeDeformer(MFnMesh& mesh)
{
    MItDependencyNodes blendShapeDepNodeIterator(MFn::kBlendShape);
    for (; !blendShapeDepNodeIterator.isDone() && this->blendShapeDeformer == 0; blendShapeDepNodeIterator.next()) 
    {   
        MObject blendShapeObject = blendShapeDepNodeIterator.item();
        MItDependencyGraph itGraph(blendShapeObject, MFn::kMesh, MItDependencyGraph::kDownstream, MItDependencyGraph::kDepthFirst);
        for (; !itGraph.isDone() && this->blendShapeDeformer == nullptr; itGraph.next())
        {
            MFnMesh connectedMesh(itGraph.thisNode());
            if (connectedMesh.fullPathName() == mesh.fullPathName())
            {
                this->blendShapeDeformer.reset(new MFnBlendShapeDeformer(blendShapeObject));
                return true;
            }
        }
    }

    return false;
}

bool MeshMorpher::HasMorphTarget(ObjectAccessor target) const
{
    for (MeshMorphTargets::const_iterator i = this->morphTargets.begin();
        i != this->morphTargets.end();
        ++i)
    {
        MeshMorphTargetPtr morphTarget = *i;
        if (morphTarget->object == target)
            return true;
    }

    return false;
}

void MeshMorpher::StartMorphTargets
    (
    std::vector<MDagModifier*>& weightConnectionDagModifiers,
    MIntArray& weightIndexList,
    std::vector<float>& originalWeights
    )
{
    //Break node connections, saving the modifier for later so they can be restored
    MPlug weightsPlug = this->blendShapeDeformer->findPlug("weight", true);
    unsigned int weightCount = weightsPlug.evaluateNumElements();
    for (unsigned int weightIndex = 0; weightIndex < weightCount; weightIndex++)
    {
        MPlug wPlug = weightsPlug.elementByPhysicalIndex(weightIndex);
        
        MPlugArray sourceConnections;
        wPlug.connectedTo(sourceConnections, false, true);
        for (unsigned int connectionIndex = 0; connectionIndex < sourceConnections.length(); connectionIndex++)
        {
            MDagModifier* dagModifier = new MDagModifier;
            dagModifier->disconnect(wPlug, sourceConnections[connectionIndex]);
            dagModifier->doIt();
            weightConnectionDagModifiers.push_back(dagModifier);
        }        
        
        MPlugArray destinationConnections;
        wPlug.connectedTo(destinationConnections, true, false);
        for (unsigned int connectionIndex = 0; connectionIndex < destinationConnections.length(); connectionIndex++)
        {
            MDagModifier* dagModifier = new MDagModifier;
            dagModifier->disconnect(destinationConnections[connectionIndex], wPlug);
            dagModifier->doIt();
            weightConnectionDagModifiers.push_back(dagModifier);
        }
    }

    //Save original envelope value for the deformer
    this->originalEnvelope = this->blendShapeDeformer->envelope();

    Disable();

    //Fully enable deformer
    this->blendShapeDeformer->setEnvelope(1);

    //Save original target weights while zeroing out the current ones
    this->blendShapeDeformer->weightIndexList(weightIndexList);

    originalWeights.resize(weightIndexList.length());
    for (unsigned int index = 0; index < weightIndexList.length(); index++)
    {
        originalWeights[index] = this->blendShapeDeformer->weight(weightIndexList[index]);
        this->blendShapeDeformer->setWeight(weightIndexList[index], 0);
    }
}

void MeshMorpher::FinishMorphTargets
    (
    const std::vector<MDagModifier*>& weightConnectionDagModifiers,
    const MIntArray& weightIndexList,
    const std::vector<float>& originalWeights    
    )
{
    MDagModifier dagModifier;

    this->blendShapeDeformer->setEnvelope(this->originalEnvelope);

    //Restore weight connections
    for (int i = weightConnectionDagModifiers.size() - 1; i >= 0; i--)
    {
        weightConnectionDagModifiers[i]->undoIt();
        delete weightConnectionDagModifiers[i];
    }
    
    for (unsigned int index = 0; index < weightIndexList.length(); index++)
        this->blendShapeDeformer->setWeight(weightIndexList[index], originalWeights[index]);

    Restore();
}

bool MeshMorpher::HasMorpher(ObjectAccessor object)
{
    MeshMorpher meshMorpher(0);
    CoordinateSystemConverter nullConverter(ApplicationAccessor::GetUpAxis());
    return meshMorpher.Initialize(object, nullConverter, 1, PrimitiveType::DEFAULT);
}
