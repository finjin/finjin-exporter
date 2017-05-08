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
#include "FinjinObjectSettingsAccessor.hpp"
#include "FinjinNodeAnimationSettingsAccessor.hpp"
#include "FinjinMeshAnimationSettingsAccessor.hpp"
#include "NodeAttributeAdder.hpp"
#include "MayaUtilities.hpp"
#include "UserDataAttributes.hpp"
#include "ExtendedValueAttributes.hpp"
#include "ExtendedValue.hpp"

using namespace Finjin::Exporter;


//Locals------------------------------------------------------------------------
static const MString OBJECT_SETTINGS_ATTRIBUTE_NAME("FinjinObjectSettings");
FinjinObjectSettingsAccessor::ObjectSettingsAttributes FinjinObjectSettingsAccessor::attributes;


//Implementation----------------------------------------------------------------
IMPLEMENT_DYNAMIC_OR_STATIC_ATTRIBUTE_USER_DATA_ACCESSOR_METHODS(FinjinObjectSettingsAccessor, FinjinObjectSettingsAccessor::attributes)

IMPLEMENT_DYNAMIC_OR_STATIC_ATTRIBUTE_EXTENDED_VALUE_ACCESSOR_METHODS(FinjinObjectSettingsAccessor, FinjinObjectSettingsAccessor::attributes)

wxString FinjinObjectSettingsAccessor::GetType()
{
    return ApplicationStringToWxString(ChildString(attributes.type, "Type"));
}

wxString FinjinObjectSettingsAccessor::GetVisibility()
{
    return ApplicationStringToWxString(ChildString(attributes.visibility, "Visibility"));
}

bool FinjinObjectSettingsAccessor::GetVisibilityAffectObjectOnly()
{
    return ChildBool(attributes.visibilityAffectObjectOnly, "VisibilityAffectObjectOnly");
}

wxString FinjinObjectSettingsAccessor::GetRenderQueueName()
{
    return ApplicationStringToWxString(ChildString(attributes.renderQueueName, "RenderQueueName"));
}

int FinjinObjectSettingsAccessor::GetRenderPriority()
{
    return ChildInt(attributes.renderQueuePriority, "RenderQueuePriority");
}

float FinjinObjectSettingsAccessor::GetRenderingDistance()
{
    return ChildFloat(attributes.renderingDistance, "RenderingDistance");
}

ObjectAccessor FinjinObjectSettingsAccessor::GetExportedParent()
{
    return MayaPlug::GetObjectReference(Child(attributes.exportedParent, "ExportedParent"));
}

bool FinjinObjectSettingsAccessor::GetUseRootAsExportedParent()
{
    return ChildBool(attributes.useRootAsExportedParent, "UseRootAsExportedParent");
}

bool FinjinObjectSettingsAccessor::GetAttachedObject()
{
    return ChildBool(attributes.isAttachedObject, "IsAttachedObject");
}

bool FinjinObjectSettingsAccessor::GetIgnoreChildren()
{
    return ChildBool(attributes.ignoreChildren, "IgnoreChildren");
}

wxString FinjinObjectSettingsAccessor::GetChildSorting()
{
    return ApplicationStringToWxString(ChildString(attributes.childSorting, "ChildSorting"));
}

int FinjinObjectSettingsAccessor::GetChildOrder()
{
    return ChildInt(attributes.childOrder, "ChildOrder");
}

bool FinjinObjectSettingsAccessor::GetIsPrefab()
{
    return ChildBool(attributes.isPrefab, "IsPrefab");
}

bool FinjinObjectSettingsAccessor::GetIsPrefabInstance()
{
    return ChildBool(attributes.isPrefabInstance, "IsPrefabInstance");
}

wxString FinjinObjectSettingsAccessor::GetPrefabName()
{
    return ApplicationStringToWxString(ChildString(attributes.prefabName, "PrefabName"));
}

int FinjinObjectSettingsAccessor::GetNoteTrackNameCount()
{
    return ChildNumElements(attributes.noteTrackNames, "NoteTrackNames");
}

wxString FinjinObjectSettingsAccessor::GetNoteTrackName(int i)
{
    MPlug noteTrackNamesPlug = Child(attributes.noteTrackNames, "NoteTrackNames");
    wxString value;
    if (!noteTrackNamesPlug.isNull())
        value = ApplicationStringToWxString(noteTrackNamesPlug[i].asString());
    return value;
}

bool FinjinObjectSettingsAccessor::GetExportFlags()
{
    return ChildBool(attributes.exportFlags, "ExportFlags");
}

int FinjinObjectSettingsAccessor::GetFlagCount()
{
    return ChildNumElements(attributes.flagNames, "FlagNames");
}

void FinjinObjectSettingsAccessor::GetFlag(int i, wxString& name)
{
    MPlug namesPlug = Child(attributes.flagNames, "FlagNames");
    if (!namesPlug.isNull())
        name = ApplicationStringToWxString(namesPlug[i].asString());
    else
        name.clear();
}

SampleType FinjinObjectSettingsAccessor::GetNodeAnimationSampleType()
{
    return (SampleType)ChildInt(attributes.nodeAnimationSampleType, "NodeAnimationSampleType", (int)SampleType::USE_PARENT);
}

double FinjinObjectSettingsAccessor::GetNodeAnimationSampleInterval()
{
    return ChildDouble(attributes.nodeAnimationSampleInterval, "NodeAnimationSampleInterval");
}

int FinjinObjectSettingsAccessor::GetNodeAnimationCount()
{
    return ChildNumElements(attributes.nodeAnimations, "NodeAnimations");
}

FinjinNodeAnimationSettingsAccessor FinjinObjectSettingsAccessor::GetNodeAnimation(int i)
{
    MPlug animationsPlug = Child(attributes.nodeAnimations, "NodeAnimations");
    FinjinNodeAnimationSettingsAccessor value;
    if (!animationsPlug.isNull())
        value = FinjinNodeAnimationSettingsAccessor(animationsPlug[i]);
    return value;
}

float FinjinObjectSettingsAccessor::GetPointSize()
{
    return ChildFloat(attributes.pointSize, "PointSize");
}

bool FinjinObjectSettingsAccessor::GetEnableSky()
{
    return ChildBool(attributes.enableSky, "EnableSky", true);
}

bool FinjinObjectSettingsAccessor::GetEmbedSkeletonInMesh()
{
    return ChildBool(attributes.embedSkeletonInMesh, "EmbedSkeletonInMesh", true);
}

bool FinjinObjectSettingsAccessor::GetLinkSkeletonToMesh()
{
    return ChildBool(attributes.linkSkeletonToMesh, "LinkSkeletonToMesh");
}

wxString FinjinObjectSettingsAccessor::GetItemType()
{
    return ApplicationStringToWxString(ChildString(attributes.itemType, "ItemType"));
}

wxString FinjinObjectSettingsAccessor::GetMeshAnimationType()
{
    return ApplicationStringToWxString(ChildString(attributes.meshAnimationType, "MeshAnimationType"));
}

bool FinjinObjectSettingsAccessor::GetExportSkeleton()
{
    return ChildBool(attributes.exportSkeleton, "ExportSkeleton", true);
}

bool FinjinObjectSettingsAccessor::GetExportedMeshDeformedBySkeleton()
{
    return ChildBool(attributes.exportedMeshDeformedBySkeleton, "ExportedMeshDeformedBySkeleton", true);
}

wxString FinjinObjectSettingsAccessor::GetSkeletonReferencePoseType()
{
    return ApplicationStringToWxString(ChildString(attributes.skeletonReferencePoseType, "SkeletonReferencePoseType"));
}

TimeAccessor FinjinObjectSettingsAccessor::GetSkeletonReferencePoseTime()
{
    TimeAccessor value;
    value.SetSeconds(ChildDouble(attributes.skeletonReferencePoseTime, "SkeletonReferencePoseTime"));
    return value;
}

bool FinjinObjectSettingsAccessor::GetExportPoses()
{
    return ChildBool(attributes.exportPoses, "ExportPoses", true);
}

bool FinjinObjectSettingsAccessor::GetRemoveBonesWithNoVertexInfluence()
{
    return ChildBool(attributes.removeBonesWithNoVertexInfluence, "RemoveBonesWithNoVertexInfluence");
}

SampleType FinjinObjectSettingsAccessor::GetSkeletonAnimationSampleType()
{
    return (SampleType)ChildInt(attributes.skeletonAnimationSampleType, "SkeletonAnimationSampleType", (int)SampleType::USE_PARENT);
}

double FinjinObjectSettingsAccessor::GetSkeletonAnimationSampleInterval()
{
    return ChildDouble(attributes.skeletonAnimationSampleInterval, "SkeletonAnimationSampleInterval");
}

SampleType FinjinObjectSettingsAccessor::GetVertexAnimationSampleType()
{
    return (SampleType)ChildInt(attributes.vertexAnimationSampleType, "VertexAnimationSampleType", (int)SampleType::USE_PARENT);
}

double FinjinObjectSettingsAccessor::GetVertexAnimationSampleInterval()
{
    return ChildDouble(attributes.vertexAnimationSampleInterval, "VertexAnimationSampleInterval");
}

ObjectAccessor FinjinObjectSettingsAccessor::GetAnimatedRoot()
{
    return MayaPlug::GetObjectReference(Child(attributes.animatedSkeletonRoot, "AnimatedRoot"));
}

int FinjinObjectSettingsAccessor::GetMeshAnimationCount()
{
    return ChildNumElements(attributes.meshAnimations, "MeshAnimations");
}

FinjinMeshAnimationSettingsAccessor FinjinObjectSettingsAccessor::GetMeshAnimation(int i)
{
    MPlug animationsPlug = Child(attributes.meshAnimations, "MeshAnimations");
    FinjinMeshAnimationSettingsAccessor value;
    if (!animationsPlug.isNull())
        value = FinjinMeshAnimationSettingsAccessor(animationsPlug[i]);
    return value;
}

bool FinjinObjectSettingsAccessor::GetExportMesh()
{
    return ChildBool(attributes.exportMesh, "ExportMesh");
}

wxString FinjinObjectSettingsAccessor::GetMeshName()
{
    return ApplicationStringToWxString(ChildString(attributes.meshName, "MeshName"));
}

wxString FinjinObjectSettingsAccessor::GetSkeletonName()
{
    return ApplicationStringToWxString(ChildString(attributes.skeletonName, "SkeletonName"));
}

wxString FinjinObjectSettingsAccessor::GetPrimitiveType()
{
    return ApplicationStringToWxString(ChildString(attributes.primitiveType, "PrimitiveType"));
}

bool FinjinObjectSettingsAccessor::GetExportVertexColors()
{
    return ChildBool(attributes.exportVertexColors, "ExportVertexColors", false);
}

bool FinjinObjectSettingsAccessor::GetExportTangents()
{
    return ChildBool(attributes.exportTangents, "ExportTangents", false);
}

bool FinjinObjectSettingsAccessor::GetExportBinormals()
{
    return ChildBool(attributes.exportBinormals, "ExportBinormals", false);
}

bool FinjinObjectSettingsAccessor::GetAnimateBoundingVolumes()
{
    return ChildBool(attributes.animateBoundingVolumes, "AnimateBoundingVolumes", false);
}

bool FinjinObjectSettingsAccessor::GetCheckMeshInstance()
{
    return ChildBool(attributes.checkMeshInstance, "CheckMeshInstance");
}

bool FinjinObjectSettingsAccessor::GetUseCustomSubmeshes()
{
    return ChildBool(attributes.useCustomSubmeshes, "UseCustomTextureCoordinateSets");
}

void FinjinObjectSettingsAccessor::GetSubmeshesSettings(SubmeshesSettings& value)
{
    value.clear();

    MPlug submeshesPlug = Child(attributes.submeshes, "SubmeshesTextureCoordinateSets");
    if (!submeshesPlug.isNull())
    {
        value.resize(submeshesPlug.evaluateNumElements());
        for (size_t i = 0; i < value.size(); i++)
        {
            MPlug submeshPlug = submeshesPlug[i];

            //Submesh name
            MPlug textureCoordinateSetSubmeshNamePlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureCoordinateSetSubmeshName, "TextureCoordinateSetSubmeshName");
            if (!textureCoordinateSetSubmeshNamePlug.isNull())
                value[i].submeshName = ApplicationStringToWxString(textureCoordinateSetSubmeshNamePlug.asString());

            //Material
            MPlug textureCoordinateSetMaterialPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureCoordinateSetMaterial, "TextureCoordinateSetMaterial");
            MPlug textureCoordinateSetMaterialObjectPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureCoordinateSetMaterialObject, "TextureCoordinateSetMaterialObject");
            MString materialName;
            if (!textureCoordinateSetMaterialPlug.isNull())
                materialName = textureCoordinateSetMaterialPlug.asString();
            if (materialName.length() > 0)
            {
                value[i].material = MaterialAccessor::GetMaterialByName(ApplicationStringToWxString(materialName));
                textureCoordinateSetMaterialPlug.setString("");

                if (!textureCoordinateSetMaterialObjectPlug.isNull())
                    MayaPlug::SetObjectReference(textureCoordinateSetMaterialObjectPlug, value[i].material.obj);
            }
            else
                value[i].material = MayaPlug::GetObjectReference(textureCoordinateSetMaterialObjectPlug);

            //Source/destination mapping
            MPlug textureCoordinateSetSourcesPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureCoordinateSetSources, "TextureCoordinateSetSources");
            MPlug textureCoordinateSetDestinationsPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureCoordinateSetDestinations, "TextureCoordinateSetDestinations");
            MPlug textureCoordinateSetTypesPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureCoordinateSetTypes, "TextureCoordinateSetTypes");
            if (!textureCoordinateSetSourcesPlug.isNull() &&
                !textureCoordinateSetDestinationsPlug.isNull() &&
                !textureCoordinateSetTypesPlug.isNull())
            {
                value[i].textureCoordinateSetMappings.resize(textureCoordinateSetSourcesPlug.evaluateNumElements());
                for (size_t textureCoordinateSetIndex = 0;
                    textureCoordinateSetIndex < value[i].textureCoordinateSetMappings.size();
                    textureCoordinateSetIndex++)
                {
                    TextureCoordinateSetMapping& textureCoordinateSetMapping = value[i].textureCoordinateSetMappings[textureCoordinateSetIndex];

                    textureCoordinateSetMapping.source.name = ApplicationStringToWxString(textureCoordinateSetSourcesPlug[(unsigned int)textureCoordinateSetIndex].asString());
                    textureCoordinateSetMapping.destinationIndex = textureCoordinateSetDestinationsPlug[(unsigned int)textureCoordinateSetIndex].asInt();
                    textureCoordinateSetMapping.SetTextureCoordinateFlagsFromString(ApplicationStringToWxString(textureCoordinateSetTypesPlug[(unsigned int)textureCoordinateSetIndex].asString()));
                }
            }

            //Texture aliases
            MPlug textureAliasesPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureAliases, "SubmeshTextureAliases");
            MPlug textureNamesPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureNames, "SubmeshTextureNames");
            if (!textureAliasesPlug.isNull() &&
                !textureNamesPlug.isNull())
            {
                value[i].textureNameAliases.resize(textureAliasesPlug.evaluateNumElements());
                for (size_t textureAliasesIndex = 0;
                    textureAliasesIndex < value[i].textureNameAliases.size();
                    textureAliasesIndex++)
                {
                    TextureNameAlias& textureNameAlias = value[i].textureNameAliases[(unsigned int)textureAliasesIndex];

                    textureNameAlias.alias = ApplicationStringToWxString(textureAliasesPlug[(unsigned int)textureAliasesIndex].asString());
                    textureNameAlias.textureName = ApplicationStringToWxString(textureNamesPlug[(unsigned int)textureAliasesIndex].asString());
                }
            }

            //Render queue
            MPlug renderQueueNamePlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.renderQueueName, "SubmeshRenderQueueName");
            if (!renderQueueNamePlug.isNull())
                value[i].renderQueueName = ApplicationStringToWxString(renderQueueNamePlug.asString());

            MPlug renderQueuePriorityPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.renderQueuePriority, "SubmeshRenderQueuePriority");
            if (!renderQueuePriorityPlug.isNull())
                value[i].renderPriority = renderQueuePriorityPlug.asInt();
        }
    }
}

void FinjinObjectSettingsAccessor::Upgrade()
{
    //This method is used to upgrade incorrect or outdated values
}

int FinjinObjectSettingsAccessor::GetManualLodCount()
{
    int count = ChildNumElements(attributes.lodObjectReferences, "LODObjectReferences");

    //Make sure there are no empty objects being taken into account
    ObjectAccessor object;
    float distance;
    for (int i = 0; i < count;)
    {
        GetManualLod(i, object, distance);
        if (!object.IsValid())
        {
            RemoveManualLod(i);
            count--;
        }
        else
            i++;
    }

    return count;
}

void FinjinObjectSettingsAccessor::GetManualLod(int i, ObjectAccessor& object, float& distance)
{
    MPlug lodObjectsPlug = Child(attributes.lodObjectReferences, "LODObjectReferences");
    MPlug distancesPlug = Child(attributes.lodDistances, "LODDistances");
    if (!lodObjectsPlug.isNull() && !distancesPlug.isNull())
    {
        std::vector<int> deletedIndices;
        MayaPlug::GetObjectReferenceCount(lodObjectsPlug, &deletedIndices);
        MayaPlug::RemoveAttributeElements(distancesPlug, &deletedIndices);

        object = ObjectAccessor(MayaPlug::GetObjectReference(lodObjectsPlug[i]));

        distance = distancesPlug[i].asFloat();
    }
}

bool FinjinObjectSettingsAccessor::GetCalculateManualLodDistances()
{
    return ChildBool(attributes.calculateManualLodDistances, "CalculateManualLODDistances");
}

void FinjinObjectSettingsAccessor::SetType(wxString value)
{
    Child(attributes.type, "Type").setValue(WxStringToApplicationString(value));
}

void FinjinObjectSettingsAccessor::SetVisibility(wxString value)
{
    Child(attributes.visibility, "Visibility").setValue(WxStringToApplicationString(value));
}

void FinjinObjectSettingsAccessor::SetVisibilityAffectObjectOnly(bool value)
{
    Child(attributes.visibilityAffectObjectOnly, "VisibilityAffectObjectOnly").setValue(value);
}

void FinjinObjectSettingsAccessor::SetRenderQueueName(wxString value)
{
    Child(attributes.renderQueueName, "RenderQueueName").setValue(WxStringToApplicationString(value));
}

void FinjinObjectSettingsAccessor::SetRenderPriority(int value)
{
    Child(attributes.renderQueuePriority, "RenderQueuePriority").setValue(value);
}

void FinjinObjectSettingsAccessor::SetRenderingDistance(float value)
{
    Child(attributes.renderingDistance, "RenderingDistance").setValue(value);
}

void FinjinObjectSettingsAccessor::SetExportedParent(ObjectAccessor value)
{
    MayaPlug::SetObjectReference(Child(attributes.exportedParent, "ExportedParent"), value.obj);
}

void FinjinObjectSettingsAccessor::SetUseRootAsExportedParent(bool value)
{
    Child(attributes.useRootAsExportedParent, "UseRootAsExportedParent").setValue(value);
}

void FinjinObjectSettingsAccessor::SetAttachedObject(bool value)
{
    Child(attributes.isAttachedObject, "IsAttachedObject").setValue(value);
}

void FinjinObjectSettingsAccessor::SetIgnoreChildren(bool value)
{
    Child(attributes.ignoreChildren, "IgnoreChildren").setValue(value);
}

void FinjinObjectSettingsAccessor::SetChildSorting(wxString value)
{
    Child(attributes.childSorting, "ChildSorting").setValue(WxStringToApplicationString(value));
}

void FinjinObjectSettingsAccessor::SetChildOrder(int value)
{
    Child(attributes.childOrder, "ChildOrder").setValue(value);
}

void FinjinObjectSettingsAccessor::SetIsPrefab(bool value)
{
    Child(attributes.isPrefab, "IsPrefab").setValue(value);
}

void FinjinObjectSettingsAccessor::SetIsPrefabInstance(bool value)
{
    Child(attributes.isPrefabInstance, "IsPrefabInstance").setValue(value);
}

void FinjinObjectSettingsAccessor::SetPrefabName(wxString value)
{
    Child(attributes.prefabName, "PrefabName").setValue(WxStringToApplicationString(value));
}

void FinjinObjectSettingsAccessor::ClearNoteTrackNames()
{
    MayaPlug::RemoveAttributeElements(Child(attributes.noteTrackNames, "NoteTrackNames"));
}

void FinjinObjectSettingsAccessor::AddNoteTrackName(wxString value)
{
    MayaPlug::AddAttributeElementValue(Child(attributes.noteTrackNames, "NoteTrackNames"), value);
}

void FinjinObjectSettingsAccessor::SetExportFlags(bool value)
{
    Child(attributes.exportFlags, "ExportFlags").setValue(value);
}

void FinjinObjectSettingsAccessor::ClearFlags()
{
    MayaPlug::RemoveAttributeElements(Child(attributes.flagNames, "FlagNames"));
}

void FinjinObjectSettingsAccessor::AddFlag(wxString name)
{
    MayaPlug::AddAttributeElementValue(Child(attributes.flagNames, "FlagNames"), name);
}

void FinjinObjectSettingsAccessor::SetNodeAnimationSampleType(SampleType value)
{
    Child(attributes.nodeAnimationSampleType, "NodeAnimationSampleType").setValue((int)value);
}

void FinjinObjectSettingsAccessor::SetNodeAnimationSampleInterval(double value)
{
    Child(attributes.nodeAnimationSampleInterval, "NodeAnimationSampleInterval").setValue(value);
}

FinjinNodeAnimationSettingsAccessor FinjinObjectSettingsAccessor::AddNewNodeAnimation()
{
    MPlug animationsPlug = Child(attributes.nodeAnimations, "NodeAnimations");
    int i = MayaPlug::AddAttributeElement(animationsPlug, wxT("NodeAnimation_AnimationName"));
    return FinjinNodeAnimationSettingsAccessor(animationsPlug[i]);
}

FinjinNodeAnimationSettingsAccessor FinjinObjectSettingsAccessor::AddNodeAnimationCopy(int i)
{
    FinjinNodeAnimationSettingsAccessor sourceAccessor = GetNodeAnimation(i);
    FinjinNodeAnimationSettingsAccessor accessor = AddNewNodeAnimation();
    accessor.CopyFrom(sourceAccessor);
    return accessor;
}

void FinjinObjectSettingsAccessor::RemoveNodeAnimation(FinjinNodeAnimationSettingsAccessor value)
{
    MayaPlug::RemoveAttributeElement(value.rootPlug);
}

void FinjinObjectSettingsAccessor::SetPointSize(float value)
{
    Child(attributes.pointSize, "PointSize").setValue(value);
}

void FinjinObjectSettingsAccessor::SetEnableSky(bool value)
{
    Child(attributes.enableSky, "EnableSky").setValue(value);
}

void FinjinObjectSettingsAccessor::SetEmbedSkeletonInMesh(bool value)
{
    Child(attributes.embedSkeletonInMesh, "EmbedSkeletonInMesh").setValue(value);
}

void FinjinObjectSettingsAccessor::SetLinkSkeletonToMesh(bool value)
{
    Child(attributes.linkSkeletonToMesh, "LinkSkeletonToMesh").setValue(value);
}

void FinjinObjectSettingsAccessor::SetItemType(wxString value)
{
    Child(attributes.itemType, "ItemType").setValue(WxStringToApplicationString(value));
}

void FinjinObjectSettingsAccessor::SetMeshAnimationType(wxString value)
{
    Child(attributes.meshAnimationType, "MeshAnimationType").setValue(WxStringToApplicationString(value));
}

void FinjinObjectSettingsAccessor::SetExportSkeleton(bool value)
{
    Child(attributes.exportSkeleton, "ExportSkeleton").setValue(value);
}

void FinjinObjectSettingsAccessor::SetExportedMeshDeformedBySkeleton(bool value)
{
    Child(attributes.exportedMeshDeformedBySkeleton, "ExportedMeshDeformedBySkeleton").setValue(value);
}

void FinjinObjectSettingsAccessor::SetSkeletonReferencePoseType(wxString value)
{
    Child(attributes.skeletonReferencePoseType, "SkeletonReferencePoseType").setValue(WxStringToApplicationString(value));
}

void FinjinObjectSettingsAccessor::SetSkeletonReferencePoseTime(TimeAccessor value)
{
    Child(attributes.skeletonReferencePoseTime, "SkeletonReferencePoseTime").setValue((double)value.GetSeconds());
}

void FinjinObjectSettingsAccessor::SetExportPoses(bool value)
{
    Child(attributes.exportPoses, "ExportPoses").setValue(value);
}

void FinjinObjectSettingsAccessor::SetRemoveBonesWithNoVertexInfluence(bool value)
{
    Child(attributes.removeBonesWithNoVertexInfluence, "RemoveBonesWithNoVertexInfluence").setValue(value);
}

void FinjinObjectSettingsAccessor::SetSkeletonAnimationSampleType(SampleType value)
{
    Child(attributes.skeletonAnimationSampleType, "SkeletonAnimationSampleType").setValue((int)value);
}

void FinjinObjectSettingsAccessor::SetSkeletonAnimationSampleInterval(double value)
{
    Child(attributes.skeletonAnimationSampleInterval, "SkeletonAnimationSampleInterval").setValue(value);
}

void FinjinObjectSettingsAccessor::SetVertexAnimationSampleType(SampleType value)
{
    Child(attributes.vertexAnimationSampleType, "VertexAnimationSampleType").setValue((int)value);
}

void FinjinObjectSettingsAccessor::SetVertexAnimationSampleInterval(double value)
{
    Child(attributes.vertexAnimationSampleInterval, "VertexAnimationSampleInterval").setValue(value);
}

void FinjinObjectSettingsAccessor::SetAnimatedRoot(ObjectAccessor value)
{
    MayaPlug::SetObjectReference(Child(attributes.animatedSkeletonRoot, "AnimatedRoot"), value.obj);
}

FinjinMeshAnimationSettingsAccessor FinjinObjectSettingsAccessor::AddNewMeshAnimation()
{
    MPlug animationsPlug = Child(attributes.meshAnimations, "MeshAnimations");
    int i = MayaPlug::AddAttributeElement(animationsPlug, wxT("MeshAnimation_AnimationName"));
    return FinjinMeshAnimationSettingsAccessor(animationsPlug[i]);
}

FinjinMeshAnimationSettingsAccessor FinjinObjectSettingsAccessor::AddMeshAnimationCopy(int i)
{
    FinjinMeshAnimationSettingsAccessor sourceAccessor = GetMeshAnimation(i);
    FinjinMeshAnimationSettingsAccessor accessor = AddNewMeshAnimation();
    accessor.CopyFrom(sourceAccessor);
    return accessor;
}

void FinjinObjectSettingsAccessor::RemoveMeshAnimation(FinjinMeshAnimationSettingsAccessor value)
{
    MayaPlug::RemoveAttributeElement(value.rootPlug);
}

void FinjinObjectSettingsAccessor::SetExportMesh(bool value)
{
    Child(attributes.exportMesh, "ExportMesh").setValue(value);
}

void FinjinObjectSettingsAccessor::SetMeshName(wxString value)
{
    Child(attributes.meshName, "MeshName").setValue(WxStringToApplicationString(value));
}

void FinjinObjectSettingsAccessor::SetSkeletonName(wxString value)
{
    Child(attributes.skeletonName, "SkeletonName").setValue(WxStringToApplicationString(value));
}

void FinjinObjectSettingsAccessor::SetPrimitiveType(wxString value)
{
    Child(attributes.primitiveType, "PrimitiveType").setValue(WxStringToApplicationString(value));
}

void FinjinObjectSettingsAccessor::SetExportVertexColors(bool value)
{
    Child(attributes.exportVertexColors, "ExportVertexColors").setValue(value);
}

void FinjinObjectSettingsAccessor::SetExportTangents(bool value)
{
    Child(attributes.exportTangents, "ExportTangents").setValue(value);
}

void FinjinObjectSettingsAccessor::SetExportBinormals(bool value)
{
    Child(attributes.exportBinormals, "ExportBinormals").setValue(value);
}

void FinjinObjectSettingsAccessor::SetAnimateBoundingVolumes(bool value)
{
    Child(attributes.animateBoundingVolumes, "AnimateBoundingVolumes").setValue(value);
}

void FinjinObjectSettingsAccessor::SetCheckMeshInstance(bool value)
{
    Child(attributes.checkMeshInstance, "CheckMeshInstance").setValue(value);
}

void FinjinObjectSettingsAccessor::SetUseCustomSubmeshes(bool value)
{
    Child(attributes.useCustomSubmeshes, "UseCustomTextureCoordinateSets").setValue(value);
}

void FinjinObjectSettingsAccessor::SetSubmeshesSettings(const SubmeshesSettings& value)
{
    MPlug submeshesPlug = Child(attributes.submeshes, "SubmeshesTextureCoordinateSets");
    submeshesPlug.evaluateNumElements();

    MayaPlug::RemoveAttributeElements(submeshesPlug);

    for (size_t i = 0; i < value.size(); i++)
    {
        int addedIndex = MayaPlug::AddAttributeElement(submeshesPlug, wxT("TextureCoordinateSetSubmeshName"));
        MPlug submeshPlug = submeshesPlug[addedIndex];

        //Submesh name
        MPlug textureCoordinateSetSubmeshNamePlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureCoordinateSetSubmeshName, "TextureCoordinateSetSubmeshName");
        textureCoordinateSetSubmeshNamePlug.setString(WxStringToApplicationString(value[i].submeshName));

        //Material
        MPlug textureCoordinateSetMaterialObjectPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureCoordinateSetMaterialObject, "TextureCoordinateSetMaterialObject");
        MObject materialObject = value[i].material.obj;
        MayaPlug::SetObjectReference(textureCoordinateSetMaterialObjectPlug, materialObject);

        //Source/destination mapping
        MPlug textureCoordinateSetSourcesPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureCoordinateSetSources, "TextureCoordinateSetSources");
        MPlug textureCoordinateSetDestinationsPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureCoordinateSetDestinations, "TextureCoordinateSetDestinations");
        MPlug textureCoordinateSetTypesPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureCoordinateSetTypes, "TextureCoordinateSetTypes");
        for (size_t textureCoordinateSetIndex = 0;
            textureCoordinateSetIndex < value[i].textureCoordinateSetMappings.size();
            textureCoordinateSetIndex++)
        {
            auto& textureCoordinateSetMapping = value[i].textureCoordinateSetMappings[textureCoordinateSetIndex];

            MayaPlug::AddAttributeElementValue(textureCoordinateSetSourcesPlug, textureCoordinateSetMapping.source.name);
            MayaPlug::AddAttributeElementValue(textureCoordinateSetDestinationsPlug, textureCoordinateSetMapping.destinationIndex);
            MayaPlug::AddAttributeElementValue(textureCoordinateSetTypesPlug, textureCoordinateSetMapping.GetTextureCoordinateFlagsString());
        }

        //Texture aliases
        MPlug submeshTextureAliasesPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureAliases, "SubmeshTextureAliases");
        MPlug submeshTextureNamesPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.textureNames, "SubmeshTextureNames");
        for (size_t textureAliasesIndex = 0;
            textureAliasesIndex < value[i].textureNameAliases.size();
            textureAliasesIndex++)
        {
            auto& textureNameAlias = value[i].textureNameAliases[textureAliasesIndex];

            MayaPlug::AddAttributeElementValue(submeshTextureAliasesPlug, textureNameAlias.alias);
            MayaPlug::AddAttributeElementValue(submeshTextureNamesPlug, textureNameAlias.textureName);
        }

        //Render queue
        MPlug renderQueueNamePlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.renderQueueName, "SubmeshRenderQueueName");
        renderQueueNamePlug.setString(WxStringToApplicationString(value[i].renderQueueName));

        MPlug renderQueuePriorityPlug = MayaPlug::FindChildPlug(submeshPlug, attributes.submeshesAttributes.renderQueuePriority, "SubmeshRenderQueuePriority");
        renderQueuePriorityPlug.setInt(value[i].renderPriority);
    }
}

void FinjinObjectSettingsAccessor::AddManualLod(ObjectAccessor object, float distance)
{
    MayaPlug::AddObjectReference(Child(attributes.lodObjectReferences, "LODObjectReferences"), object.obj);
    MayaPlug::AddAttributeElementValue(Child(attributes.lodDistances, "LODDistances"), distance);
}

void FinjinObjectSettingsAccessor::SetManualLod(int i, ObjectAccessor object, float distance)
{
    MayaPlug::SetObjectReference(Child(attributes.lodObjectReferences, "LODObjectReferences")[i], object.obj);
    Child(attributes.lodDistances, "LODDistances")[i].setValue(distance);
}

void FinjinObjectSettingsAccessor::RemoveManualLod(int i)
{
    MayaPlug::RemoveAttributeElement(Child(attributes.lodObjectReferences, "LODObjectReferences")[i]);
    MayaPlug::RemoveAttributeElement(Child(attributes.lodDistances, "LODDistances")[i]);
}

void FinjinObjectSettingsAccessor::SetCalculateManualLodDistances(bool value)
{
    Child(attributes.calculateManualLodDistances, "CalculateManualLODDistances").setValue(value);
}

static MObject CreateSubmeshesAttribute
    (
    FinjinObjectSettingsAccessor::ObjectSettingsAttributes::SubmeshesAttributes& attributes,
    NodeAttributeAdder& adder,
    const MString& name
    )
{
    std::list<MObject> attributesList;

    attributesList.push_back(attributes.textureCoordinateSetSubmeshName = adder.AddString("TextureCoordinateSetSubmeshName"));
    attributesList.push_back(attributes.textureCoordinateSetMaterial = adder.AddString("TextureCoordinateSetMaterial"));
    attributesList.push_back(attributes.textureCoordinateSetMaterialObject = adder.AddObjectReference("TextureCoordinateSetMaterialObject"));
    attributesList.push_back(attributes.textureCoordinateSetSources = adder.AddStringArray("TextureCoordinateSetSources"));
    attributesList.push_back(attributes.textureCoordinateSetDestinations = adder.AddIntArray("TextureCoordinateSetDestinations"));
    attributesList.push_back(attributes.textureCoordinateSetTypes = adder.AddStringArray("TextureCoordinateSetTypes"));
    attributesList.push_back(attributes.textureAliases = adder.AddStringArray("SubmeshTextureAliases"));
    attributesList.push_back(attributes.textureNames = adder.AddStringArray("SubmeshTextureNames"));
    attributesList.push_back(attributes.renderQueueName = adder.AddString("SubmeshRenderQueueName"));
    attributesList.push_back(attributes.renderQueuePriority = adder.AddInt("SubmeshRenderQueuePriority"));

    return adder.AddCompoundArray(name, attributesList);
}

MObject FinjinObjectSettingsAccessor::CreateObjectSettingsArrayAttribute(NodeAttributeAdder& adder, const MString& name)
{
    std::list<MObject> attributesList;

    //User data settings
    attributes.CreateUserDataAttributes(attributesList, adder, "Object_");

    //Extended values
    attributes.CreateExtendedValueAttributes(attributesList, adder, "Object_");

    attributesList.push_back(attributes.object = adder.AddObjectReference("Object_Object"));

    attributesList.push_back(attributes.type = adder.AddString("Object_Type"));
    attributesList.push_back(attributes.visibility = adder.AddString("Object_Visibility"));
    attributesList.push_back(attributes.visibilityAffectObjectOnly = adder.AddBool("Object_VisibilityAffectObjectOnly"));
    attributesList.push_back(attributes.targetType = adder.AddString("Object_TargetType"));
    attributesList.push_back(attributes.isPrefab = adder.AddBool("Object_IsPrefab"));
    attributesList.push_back(attributes.isPrefabInstance = adder.AddBool("Object_IsPrefabInstance"));
    attributesList.push_back(attributes.prefabName = adder.AddString("Object_PrefabName"));
    attributesList.push_back(attributes.nodeAnimationSampleInterval = adder.AddDouble("Object_NodeAnimationSampleInterval"));
    attributesList.push_back(attributes.nodeAnimationSampleType = adder.AddInt("Object_NodeAnimationSampleType", (int)SampleType::USE_PARENT));

    attributesList.push_back(attributes.noteTrackNames = adder.AddStringArray("Object_NoteTrackNames"));
    attributesList.push_back(attributes.renderQueueName = adder.AddString("Object_RenderQueueName"));
    attributesList.push_back(attributes.renderQueuePriority = adder.AddInt("Object_RenderQueuePriority"));
    attributesList.push_back(attributes.exportFlags = adder.AddBool("Object_ExportFlags", true));
    attributesList.push_back(attributes.flagNames = adder.AddStringArray("Object_FlagNames"));

    attributesList.push_back(attributes.exportMesh = adder.AddBool("Object_ExportMesh", true));
    attributesList.push_back(attributes.meshName = adder.AddString("Object_MeshName"));
    attributesList.push_back(attributes.animateBoundingVolumes = adder.AddBool("Object_AnimateBoundingVolumes", false));
    attributesList.push_back(attributes.pointSize = adder.AddFloat("Object_PointSize", 10.0f));
    attributesList.push_back(attributes.checkMeshInstance = adder.AddBool("Object_CheckMeshInstance"));
    attributesList.push_back(attributes.exportedMeshDeformedBySkeleton = adder.AddBool("Object_ExportedMeshDeformedBySkeleton", true));
    attributesList.push_back(attributes.exportSkeleton = adder.AddBool("Object_ExportSkeleton", true));
    attributesList.push_back(attributes.skeletonReferencePoseType = adder.AddString("Object_SkeletonReferencePoseType"));
    attributesList.push_back(attributes.skeletonReferencePoseTime = adder.AddDouble("Object_SkeletonReferencePoseTime"));
    attributesList.push_back(attributes.exportPoses = adder.AddBool("Object_ExportPoses", true));
    attributesList.push_back(attributes.removeBonesWithNoVertexInfluence = adder.AddBool("Object_RemoveBonesWithNoVertexInfluence"));

    attributesList.push_back(attributes.skeletonName = adder.AddString("Object_SkeletonName"));
    attributesList.push_back(attributes.meshAnimationType = adder.AddString("Object_MeshAnimationType"));
    attributesList.push_back(attributes.skeletonAnimationSampleInterval = adder.AddDouble("Object_SkeletonAnimationSampleInterval"));
    attributesList.push_back(attributes.skeletonAnimationSampleType = adder.AddInt("Object_SkeletonAnimationSampleType", (int)SampleType::USE_PARENT));
    attributesList.push_back(attributes.vertexAnimationSampleInterval = adder.AddDouble("Object_VertexAnimationSampleInterval"));
    attributesList.push_back(attributes.vertexAnimationSampleType = adder.AddInt("Object_VertexAnimationSampleType", (int)SampleType::USE_PARENT));
    attributesList.push_back(attributes.animatedSkeletonRoot = adder.AddObjectReference("Object_AnimatedRoot"));
    attributesList.push_back(attributes.exportMeshAnimationsToSeparateFile = adder.AddBool("Object_ExportMeshAnimationsToSeparateFile"));
    attributesList.push_back(attributes.linkMeshAnimationsToMainObject = adder.AddBool("Object_LinkMeshAnimationsToMainObject"));

    attributesList.push_back(attributes.primitiveType = adder.AddString("Object_PrimitiveType"));
    attributesList.push_back(attributes.exportVertexColors = adder.AddBool("Object_ExportVertexColors"));
    attributesList.push_back(attributes.exportTangents = adder.AddBool("Object_ExportTangents"));
    attributesList.push_back(attributes.exportBinormals = adder.AddBool("Object_ExportBinormals"));
    attributesList.push_back(attributes.useCustomSubmeshes = adder.AddBool("Object_UseCustomTextureCoordinateSets"));
    attributesList.push_back(attributes.submeshes = CreateSubmeshesAttribute(attributes.submeshesAttributes, adder, "Object_SubmeshesTextureCoordinateSets"));

    attributesList.push_back(attributes.lodObjectNames = adder.AddStringArray("Object_LODObjects"));
    attributesList.push_back(attributes.lodObjectReferences = adder.AddObjectReferenceArray("Object_LODObjectReferences"));
    attributesList.push_back(attributes.lodDistances = adder.AddFloatArray("Object_LODDistances"));
    attributesList.push_back(attributes.calculateManualLodDistances = adder.AddBool("Object_CalculateManualLodDistances"));

    attributesList.push_back(attributes.itemType = adder.AddString("Object_ItemType"));

    attributesList.push_back(attributes.enableSky = adder.AddBool("Object_EnableSky", true));

    attributesList.push_back(attributes.scale = adder.AddFloat("Object_Scale", 1.0));

    attributesList.push_back(attributes.embedSkeletonInMesh = adder.AddBool("Object_EmbedSkeletonInMesh", true));
    attributesList.push_back(attributes.linkSkeletonToMesh = adder.AddBool("Object_LinkSkeletonToMesh", true));

    attributesList.push_back(attributes.exportedParent = adder.AddBool("Object_ExportedParent"));
    attributesList.push_back(attributes.useRootAsExportedParent = adder.AddBool("Object_UseRootAsExportedParent"));
    attributesList.push_back(attributes.isAttachedObject = adder.AddBool("Object_IsAttachedObject"));
    attributesList.push_back(attributes.ignoreChildren = adder.AddBool("Object_IgnoreChildren"));
    attributesList.push_back(attributes.childSorting = adder.AddString("Object_ChildSorting"));
    attributesList.push_back(attributes.childOrder = adder.AddInt("Object_ChildOrder"));

    attributesList.push_back(attributes.renderingDistance = adder.AddFloat("Object_RenderingDistance"));

    attributesList.push_back(attributes.nodeAnimations = FinjinNodeAnimationSettingsAccessor::CreateSettingsArrayAttribute(adder, "Object_NodeAnimations"));
    attributesList.push_back(attributes.meshAnimations = FinjinMeshAnimationSettingsAccessor::CreateSettingsArrayAttribute(adder, "Object_MeshAnimations"));

    return adder.AddCompoundArray(name, attributesList);
}

MPlug FinjinObjectSettingsAccessor::GetObjectSettings(ObjectAccessor object)
{
    MObject objectWithSettings = object.obj;

    //Try to find the object settings plug/attribute
    MPlug objectSettingsPlug;
    if (objectWithSettings.hasFn(MFn::kDependencyNode))
    {
        MFnDependencyNode depNode(objectWithSettings);
        objectSettingsPlug = depNode.findPlug(OBJECT_SETTINGS_ATTRIBUTE_NAME);
    }

    return objectSettingsPlug;
}

MPlug FinjinObjectSettingsAccessor::Child(MObject attribute, const MString& name)
{
    MPlug result;

    if (this->rootPlug.isDynamic())
    {
        MString prefixedName = "Object_" + name;

        MString plugName;
        unsigned int count = this->rootPlug.numChildren();
        for (unsigned int i = 0; i < count; i++)
        {
            MPlug plug = this->rootPlug.child(i);
            MayaPlug::GetBaseAttributeName(plug, plugName);
            if (plugName == name || plugName == prefixedName)
            {
                result = plug;
                break;
            }
        }
    }
    else
        result = this->rootPlug.child(attribute);

    return result;
}

int FinjinObjectSettingsAccessor::ChildInt(MObject attribute, const MString& name, int defaultValue)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? defaultValue : plug.asInt();
}

bool FinjinObjectSettingsAccessor::ChildBool(MObject attribute, const MString& name, bool defaultValue)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? defaultValue : plug.asBool();
}

float FinjinObjectSettingsAccessor::ChildFloat(MObject attribute, const MString& name, float defaultValue)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? defaultValue : plug.asFloat();
}

double FinjinObjectSettingsAccessor::ChildDouble(MObject attribute, const MString& name, double defaultValue)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? defaultValue : plug.asDouble();
}

MString FinjinObjectSettingsAccessor::ChildString(MObject attribute, const MString& name)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? "" : plug.asString();
}

int FinjinObjectSettingsAccessor::ChildNumElements(MObject attribute, const MString& name)
{
    MPlug plug = Child(attribute, name);
    return plug.isNull() ? 0 : plug.evaluateNumElements();
}
