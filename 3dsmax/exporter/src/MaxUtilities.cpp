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
#include "ApplicationAccessor.hpp"
#include "MaxUtilities.hpp"
#include "FileUtilities.hpp"
#include "StringUtilities.hpp"
#include "MaxBitmapUtilities.hpp"
#include "MaxMathUtilities.hpp"
#include "MathUtilities.hpp"
#include "MaxControlUtilities.hpp"
#include "MaxMaterialUtilities.hpp"
#include "StringSplitter.hpp"
#include "resource.h"
#include "PropertyValues.hpp"
#include "CoordinateSystemConverter.hpp"
#include "FinjinSceneSettingsObject.hpp"
#include "FinjinObjectSettingsObject.hpp"
#include "FinjinSceneSettingsAccessor.hpp"

using namespace Finjin::Exporter;


//Local values-----------------------------------------------------------------
static const Class_ID MORPHER_MODIFIER_CLASS_ID(0x17bb6854, 0xa5cba2a3);
static const Class_ID MAYBE_OLD_SKIN_CLASS_ID(0, 0); //(0x68477bb4, 0x28cf6b86) //Old skin class ID? Commented out in Max 9 SDK iskin.h
static const Class_ID CURRENT_SKIN_CLASS_ID(9815843, 87654); //Defined value for SKIN_CLASSID in Max 9 SDK iskin.h        
static const Class_ID XFORM_CLASS_ID(0x25215824, 0); //Modifier created when using the "Reset Xform" utility


//Local functions--------------------------------------------------------------
static MSTR FixParameterName(const MCHAR* name)
{
    wxString fixedName(name);
    fixedName.Replace(wxT(" "), wxT("_"));
    fixedName.Replace(wxT("."), wxT("_"));
    fixedName.Replace(wxT("-"), wxT("_"));
    return WxStringToApplicationStringM(fixedName);
}

template <class T>
void AddKeyTimesTemplate(IKeyControl* ikeys, std::vector<TimeValue>& keyTimes)
{
    T key;
    int keyCount = ikeys->GetNumKeys();
    for (int keyIndex = 0; keyIndex < keyCount; keyIndex++)
    {        
        ikeys->GetKey(keyIndex, &key);
        if (std::find(keyTimes.begin(), keyTimes.end(), key.time) == keyTimes.end())
            keyTimes.push_back(key.time);
    }
}

static Box3 ComputeBBox(INode* node, bool transform = false, TimeValue t = -1) 
{ 
    Box3 box;

    if (t < 0)
        t = GetCOREInterface()->GetTime(); 

    auto obj = node->EvalWorldState(t).obj;   

    //Get the scale transform if necessary
    Matrix3 sclMat(1);
    if (transform)
    {
        Matrix3 mat;

        //Determine if the object is in world space or object space 
        //so we can get the correct TM. We can check this by getting 
        //the Object TM after the world space modifiers have been 
        //applied. It the matrix returned is the identity matrix the 
        //points of the object have been transformed into world space. 
        if (node->GetObjTMAfterWSM(t).IsIdentity()) 
        { 
           //It's in world space, so put it back into object 
           //space. We can do this by computing the inverse 
           //of the matrix returned before any world space 
           //modifiers were applied. 
            mat = Inverse(node->GetObjTMBeforeWSM(t)); 
        } 
        else
        { 
            //It's in object space, get the Object TM. 
            mat = node->GetObjectTM(t); 
        }  

        //Extract just the scaling part from the TM 
        AffineParts parts; 
        decomp_affine(mat, &parts); 
        ApplyScaling(sclMat, ScaleValue(parts.k * parts.f, parts.u));   
    }

    //Get the bound box, and affect it by just 
    //the scaling portion 
    obj->GetDeformBBox(t, box, &sclMat);   

    return box;   
}

static float GetUSFractionalScale(int type)
{
    switch (type)
    {
        case UNIT_FRAC_1_1: return 1;
        case UNIT_FRAC_1_2: return 2;
        case UNIT_FRAC_1_4: return 4;
        case UNIT_FRAC_1_8: return 8;
        case UNIT_FRAC_1_10: return 10;
        case UNIT_FRAC_1_16: return 16;
        case UNIT_FRAC_1_32: return 32;
        case UNIT_FRAC_1_64: return 64;
        case UNIT_FRAC_1_100: return 100;
    }
    return 1;
}


//Implementation---------------------------------------------------------------
wxString MaxUtilities::GetPropertyString(int propertySet, PROPID propid)
{
    wxString propString;

    PROPSPEC propSpec;
    propSpec.ulKind = PRSPEC_PROPID;
    propSpec.propid = propid;
    int index = GetCOREInterface()->FindProperty(propertySet, &propSpec);    
    if (index != -1) 
    {
        auto prop = GetCOREInterface()->GetPropertyVariant(propertySet, index);
        if (prop != nullptr)
        {
            if (prop->vt == VT_LPWSTR)
                propString = prop->pwszVal;
            else if (prop->vt == VT_LPSTR)
                propString = wxString::FromUTF8(prop->pszVal);
        }
    }

    return propString;
}

/*void MaxUtilities::SetPropertyString(int propertySet, PROPID propid, const wxString& value)
{
    PROPSPEC propSpec;
    propSpec.ulKind = PRSPEC_PROPID;
    propSpec.propid = propid;
    
    PROPVARIANT prop;
    prop.vt = VT_LPSTR;
    MSTR maxValue = WxStringToApplicationStringM(value);
    prop.pszVal = (LPSTR)maxValue.data(); //This cast probably isn't safe for Max2013 and up

    GetCOREInterface()->AddProperty(propertySet, &propSpec, &prop);
}*/

wxString MaxUtilities::GetSceneSummaryInfo()
{
    //The scene summary information appears in two places in 3DS Max:
    // 1)File->Summary Info...->Description
    // 2)File->File Properties->Summary->Comments
    
    return GetPropertyString(PROPSET_SUMMARYINFO, PIDSI_COMMENTS);
}

/*void MaxUtilities::SetSceneSummaryInfo(const wxString& text)
{
    SetPropertyString(PROPSET_SUMMARYINFO, PIDSI_COMMENTS, text);
}
*/

bool MaxUtilities::ContainsClassID(const Class_ID* classIDs, int classIDCount, Class_ID id)
{
    for (int classIDIndex = 0; classIDIndex < classIDCount; classIDIndex++)
    {
        if (classIDs[classIDIndex] == id)
            return true;
    }

    return false;
}

Modifier* MaxUtilities::FindModifier(INode* node, const Class_ID* classIDs, int cidCount)
{
    //Get object from node. Abort if no object.
    if (node == nullptr) 
        return nullptr;

    auto obj = node->GetObjectRef();
    while (obj != nullptr  && obj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
    {
        auto derivedObj = static_cast<IDerivedObject*>(obj);
            
        //Iterate over all entries of the modifier stack.
        int modStackIndex = 0;
        
        while (modStackIndex < derivedObj->NumModifiers())
        {
            //Get current modifier
            auto modifier = derivedObj->GetModifier(modStackIndex);

            //Is this the right modifier
            for (int i = 0; i < cidCount; i++)
            {
                if (modifier->ClassID() == classIDs[i])
                    return modifier;
            }
            modStackIndex++;
        }
        obj = derivedObj->GetObjRef();
    }

    //Not found.
    return nullptr;
}

Modifier* MaxUtilities::FindModifier(INode* node, Class_ID id)
{
    Class_ID classIDs[1] = {id};
    return FindModifier(node, classIDs, 1);
}

bool MaxUtilities::VerifyMeshModifierStack(INode* node)
{
    auto objectName = ApplicationStringToWxString(node->GetName());

    std::vector<Modifier*> modifiers;
    modifiers.reserve(10);

    auto obj = node->GetObjectRef();
    while (obj != nullptr  && obj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
    {
        auto derivedObj = static_cast<IDerivedObject*>(obj);            
        int modStackIndex = 0;        
        while (modStackIndex < derivedObj->NumModifiers())
        {
            auto modifier = derivedObj->GetModifier(modStackIndex);
            modifiers.push_back(modifier);
            modStackIndex++;
        }
        obj = derivedObj->GetObjRef();
    }

    auto result = true;

    //Find the modifiers
    int skinFoundAt = -1;
    int morpherFoundAt = -1;
    for (size_t i = 0; i < modifiers.size(); i++)
    {
        auto modifier = modifiers[i];
        if (modifier->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B) ||
            modifier->ClassID() == MAYBE_OLD_SKIN_CLASS_ID ||
            modifier->ClassID() == CURRENT_SKIN_CLASS_ID)
        {
            //Found skin/physique
            if (skinFoundAt != -1)
            {
                FINJIN_EXPORTER_LOG_MESSAGE(ERROR_LOG_MESSAGE, wxT("'%s' has more than one Skin/Physique modifier. Only one Skin/Physique modifier is allowed."), objectName.wx_str());
                result = false;
            }
            else
                skinFoundAt = (int)i;
        }
        else if (modifier->ClassID() == MORPHER_MODIFIER_CLASS_ID)
        {
            //Found morpher
            if (morpherFoundAt != -1)
            {
                FINJIN_EXPORTER_LOG_MESSAGE(ERROR_LOG_MESSAGE, wxT("'%s' has more than one Morpher modifier. Only one Morpher modifier is allowed."), objectName.wx_str());
                result = false;
            }
            else
                morpherFoundAt = (int)i;            
        }
    }

    //Ensure the ordering
    if (result)
    {
        if (skinFoundAt != -1 && morpherFoundAt != -1)
        {
            if (skinFoundAt != morpherFoundAt - 1)
            {
                FINJIN_EXPORTER_LOG_MESSAGE(ERROR_LOG_MESSAGE, wxT("'%s' has a Skin/Physique modifier and a Morpher modifier, but Skin/Physique modifier is not above the Morpher modifier."), objectName.wx_str());
                result = false;
            }            
        }
    }

    //Ensure the correct modifier is at the top of the stack
    if (result)
    {
        if (skinFoundAt != -1)
        {
            if (skinFoundAt > 0)
            {
                FINJIN_EXPORTER_LOG_MESSAGE(ERROR_LOG_MESSAGE, wxT("'%s' has a Skin/Physique modifier, but it's not at the top of the stack."), objectName.wx_str());
                result = false;
            }
        }
        else if (morpherFoundAt != -1)
        {
            if (morpherFoundAt > 0)
            {
                FINJIN_EXPORTER_LOG_MESSAGE(ERROR_LOG_MESSAGE, wxT("'%s' has a Morpher modifier, but it's not at the top of the stack."), objectName.wx_str());
                result = false;
            }
        }
    }

    return result;
}

/*bool MaxUtilities::IsBipedBone(INode* node)
{
    if (!node->IsRootNode())
    {
        Class_ID objectClassID = node->GetObjectRef()->ClassID();
        return objectClassID == BIPED_CLASS_ID ? true : false;
    }
    else
        return false;
}*/

bool MaxUtilities::IsBipedBone(INode* node)
{
    bool isBipedBone = false;

    Control* control = node->GetTMController();
    if (control != nullptr)
    {
        isBipedBone =  
            control->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||
            control->ClassID() == BIPBODY_CONTROL_CLASS_ID;
    }

    return isBipedBone;
}

bool MaxUtilities::IsNonBipedBone(INode* node)
{
    auto obj = node->GetObjectRef();
    return
        obj->ClassID() == Class_ID(BONE_CLASS_ID,0) || //Standard bone
        obj->ClassID() == BONE_OBJ_CLASSID; //Newer bone
}

bool MaxUtilities::IsCATBone(INode* node)
{
    auto obj = node->GetObjectRef();
    return
        obj->ClassID() == Class_ID(778701833, 1138084288) || //Standard bone
        obj->ClassID() == Class_ID(1943816243, 1707687082); //Root bone

    //Class_ID(131020930, 1206916674) //CAT muscle strand
    //Class_ID(401557208, 470706893) //CAT muscle
    //Class_ID(1454273253, 949708377) //CAT parent (on the ground plane)
}

bool MaxUtilities::IsAnyBone(INode* node)
{
    return IsBipedBone(node) || IsNonBipedBone(node) || IsCATBone(node);
}

bool MaxUtilities::IsUnknownObject(INode* node)
{
    return (node->GetObjectRef()->ClassID() == Class_ID(-1, 0)) ? true : false;
}

INode* MaxUtilities::FindRootmostBipedBone(INode* bone)
{
    while (bone->GetParentNode() != nullptr && IsBipedBone(bone->GetParentNode()))
        bone = bone->GetParentNode();
    
    return bone;
}

INode* MaxUtilities::FindBipedRootBone(INode* node, bool& usesPhysiqueModifier)
{
    INode* rootBone = nullptr;
    usesPhysiqueModifier = false;

    auto physiqueModifier = FindPhysiqueModifier(node);
    auto skinModifier = FindSkinModifier(node);    

    if (physiqueModifier != nullptr)
    {
        usesPhysiqueModifier = true;

        auto physiqueExport = static_cast<IPhysiqueExport*>(physiqueModifier->GetInterface(I_PHYINTERFACE));
        auto physiqueContextExport = static_cast<IPhyContextExport*>(physiqueExport->GetContextInterface(node));
        physiqueContextExport->ConvertToRigid(TRUE);
        physiqueContextExport->AllowBlending(TRUE);
        int skinnedVertexCount = physiqueContextExport->GetNumberVertices();
        for (int vertexIndex = 0; vertexIndex < skinnedVertexCount && rootBone == nullptr; vertexIndex++)
        {
            auto vertexExport = physiqueContextExport->GetVertexInterface(vertexIndex);

            if (vertexExport != nullptr)
            {                
                if (vertexExport->GetVertexType() == RIGID_NON_BLENDED_TYPE)
                {
                    auto rigidVertexExport = static_cast<IPhyRigidVertex*>(vertexExport);
                    auto bone = rigidVertexExport->GetNode();
                    rootBone = FindRootmostBipedBone(bone);                    
                }
                else if (vertexExport->GetVertexType() == RIGID_BLENDED_TYPE)
                {
                    auto blendedVertexExport = static_cast<IPhyBlendedRigidVertex*>(vertexExport);                    
                    for (int boneIndex = 0; boneIndex < blendedVertexExport->GetNumberNodes() && rootBone == nullptr; boneIndex++)
                    {
                        INode* bone = blendedVertexExport->GetNode(boneIndex);                        
                        rootBone = FindRootmostBipedBone(bone);                        
                    }
                }

                physiqueContextExport->ReleaseVertexInterface(vertexExport);
            }
        }

        physiqueExport->ReleaseContextInterface(physiqueContextExport);
        physiqueModifier->ReleaseInterface(I_PHYINTERFACE, physiqueExport);
    }
    else if (skinModifier != nullptr)
    {
        usesPhysiqueModifier = false;

        auto iskin = static_cast<ISkin*>(skinModifier->GetInterface(I_SKIN));
        if (iskin != nullptr)
        {
            for (int boneIndex = 0; boneIndex < iskin->GetNumBones() && rootBone == nullptr; boneIndex++)
            {
                INode* bone = iskin->GetBone(boneIndex);
                rootBone = FindRootmostBipedBone(bone);
            }

            skinModifier->ReleaseInterface(I_SKIN, iskin);
        }        
    }

    return rootBone;
}

Modifier* MaxUtilities::FindPhysiqueModifier(INode* node)
{
    return FindModifier(node, Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B));
}

Modifier* MaxUtilities::FindSkinModifier(INode* node)
{
    static const Class_ID skinClassIDs[] =
        {
        MAYBE_OLD_SKIN_CLASS_ID,
        CURRENT_SKIN_CLASS_ID 
        };    

    return FindModifier(node, skinClassIDs, FINJIN_COUNT_OF(skinClassIDs));
}

Modifier* MaxUtilities::FindMorpherModifier(INode* node)
{
    return FindModifier(node, MORPHER_MODIFIER_CLASS_ID);
}

void MaxUtilities::RemoveBipedNonuniformScale(INode* node, bool remove)
{
    Control* c = node->GetTMController();
    if ((c->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
        (c->ClassID() == BIPBODY_CONTROL_CLASS_ID) ||
        (c->ClassID() == FOOTPRINT_CLASS_ID))
    {
        auto bipedExport = static_cast<IBipedExport*>(c->GetInterface(I_BIPINTERFACE));
        if (bipedExport != nullptr)
        {
            bipedExport->RemoveNonUniformScale(remove ? TRUE : FALSE);
            c->ReleaseInterface(I_BIPINTERFACE, bipedExport);
        }
    }
}

void MaxUtilities::GetAllNodes
    (
    std::unordered_set<INode*>& nodes, 
    INode* rootNode, 
    bool ignoreSettings
    )
{
    if (rootNode == nullptr)
        rootNode = GetCOREInterface()->GetRootNode();
    else if (!ignoreSettings || ObjectAccessor::IsSettingsObject(rootNode))
        nodes.insert(rootNode);

    for (int i = 0; i < rootNode->NumberOfChildren(); i++)
        GetAllNodes(nodes, rootNode->GetChildNode(i), ignoreSettings);
}

int MaxUtilities::GetSelectedNodes(std::unordered_set<INode*>& nodes)
{
    int selectedNodeCount = GetCOREInterface()->GetSelNodeCount();
    for (int nodeIndex = 0; nodeIndex < selectedNodeCount; nodeIndex++)
        nodes.insert(GetCOREInterface()->GetSelNode(nodeIndex));
    return selectedNodeCount;
}

void MaxUtilities::GetChildNodes(INode* node, INodeTab& childNodes)
{
    int childNodeCount = node->NumberOfChildren();
    childNodes.SetCount(childNodeCount);

    for (int i = 0; i < childNodeCount; i++)
        childNodes[i] = node->GetChildNode(i);
}

bool MaxUtilities::GetParamIDByName(ParamID& paramID, const MCHAR* name, IParamBlock2* pblock) 
{
    //FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("MaxUtilities::GetParamIDByName(ParamID& paramID)"));

    int count = pblock->NumParams();
    for (int i = 0; i < count; ++i) 
    {
        ParamID id = pblock->IndextoID(i);
        const ParamDef& paramDef = pblock->GetParamDef(id);
        //wxString debugName = ApplicationStringToWxString(paramDef.int_name);
        //FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Param %d name: %s"), i, debugName.wx_str());
        if (_tcsicmp(name, paramDef.int_name) == 0) 
        {
            paramID = id;
            return true;
        }
    }

    return false;
}

bool MaxUtilities::GetParamIDByName(IParamBlock2*& pblock, ParamID& paramID, const MCHAR* name, Animatable* a)
{
    //FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("MaxUtilities::GetParamIDByName(IParamBlock*& pblock)"));

    for (int i = 0; i < a->NumParamBlocks(); i++)
    {
        pblock = a->GetParamBlock(i);
        if (GetParamIDByName(paramID, name, pblock))
            return true;
    }
    return false;
}

bool MaxUtilities::GetParamIDByName(IParamBlock*& pblock, int& paramID, const MCHAR* name, Animatable* a, bool nameStartsWith)
{
    //FINJIN_EXPORTER_LOG_METHOD_ENTRY(wxT("MaxUtilities::GetParamIDByName(IParamBlock*& pblock, bool nameStartsWith)"));

    //The printing capability is useful for determining the parameter block layout for
    //old plugins during development
    auto print = false;

    auto wxname = ApplicationStringToWxString(name);

    if (print)
        FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Animatable has %d subanims"), a->NumSubs());

    for (int i = 0; i < a->NumSubs(); i++) 
    {
        Animatable* subAnim = a->SubAnim(i);
        if (subAnim == nullptr)
            continue;
        
        if (subAnim->SuperClassID() == PARAMETER_BLOCK_CLASS_ID)
        {
            if (print)
                FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Subanim %d is a paramter block"), i);

            pblock = static_cast<IParamBlock*>(subAnim);
            
            if (print)
                FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("There are %d parameters"), pblock->NumParams());

            for (int paramIndex = 0; paramIndex < pblock->NumParams(); paramIndex++)
            {
                int animNumber = pblock->GetAnimNum(paramIndex);
                if (animNumber >= 0)
                {
                    wxString paramName = ApplicationStringToWxString(pblock->SubAnimName(animNumber));

                    if (print)
                        FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("  Parameter %d name: %s"), paramIndex, paramName.wx_str());

                    if (nameStartsWith)
                    {
                        if (paramName.StartsWith(wxname))
                        {
                            paramID = paramIndex;
                            return true;
                        }
                    }
                    else
                    {
                        if (wxname == paramName)
                        {
                            paramID = paramIndex;
                            return true;
                        }
                    }
                }
                else
                {
                    if (print)
                        FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("  Parameter undefined for %d"), paramIndex);
                }
            }
        }
        else
        {
            if (print)
            {
                auto subName = ApplicationStringToWxString(a->SubAnimName(i));

                MSTR className;
                subAnim->GetClassName(className);
                
                FINJIN_EXPORTER_LOG_MESSAGE(DEBUG_LOG_MESSAGE, wxT("Subanim %d is something else. Name='%s' classid=(0x%x, 0x%x) classname='%s'"), i, subName.wx_str(), subAnim->ClassID().PartA(), subAnim->ClassID().PartB(), ApplicationStringToWxString(className).wx_str());
            }
            
            if (GetParamIDByName(pblock, paramID, name, subAnim, nameStartsWith))
                return true;
        }
    }    

    return false;
} 

bool MaxUtilities::GetSubAnimByName(Animatable*& subAnim, const MCHAR* name, Animatable* a, bool nameStartsWith)
{
    auto print = true;

    auto wxname = ApplicationStringToWxString(name);

    for (int i = 0; i < a->NumSubs(); i++) 
    {
        subAnim = a->SubAnim(i);
        if (subAnim == nullptr)
            continue;

        auto subName = ApplicationStringToWxString(a->SubAnimName(i));
        
        if (nameStartsWith)
        {
            if (subName.StartsWith(wxname))
                return true;
        }
        else
        {
            if (wxname == subName)
                return true;            
        }   

        if (GetSubAnimByName(subAnim, name, subAnim, nameStartsWith))
            return true;
    }    

    return false;
} 

void MaxUtilities::GetCameraIdentityMatrix(Matrix3& m)
{
    m = ScaleMatrix(Point3(1, -1, 1));
    MaxMathUtilities::SwapRow(m, 1, 2);
}

Matrix3 MaxUtilities::GetCameraIdentityMatrix()
{
    Matrix3 m;
    GetCameraIdentityMatrix(m);
    return m;
}

bool MaxUtilities::GetBoxSize
    (
    FinjinVector3& size, 
    INode* node, 
    bool transform, 
    TimeValue time
    )
{
    size.Zero();

    auto object = node->GetObjectRef();
    if (object->ClassID() == Class_ID(BOXOBJ_CLASS_ID,0))
    {
        Box3 box = ComputeBBox(node, transform, time);
        Point3 boxSize = box.Width();
        size.Set(boxSize.x, boxSize.y, boxSize.z);
    }
    
    return !size.IsZero();
}

bool MaxUtilities::GetSphereRadiusAndSegmentCount
    (
    float& radius, 
    FinjinIVector2& segmentCount, 
    INode* node, 
    TimeValue time
    )
{
    //Defined in maxsdk\samples\objects\sphere.cpp
    enum
    {
        PB_RADIUS = 0,
        PB_SEGS = 1
    };

    if (time < 0)
        time = 0;

    radius = 0;
    segmentCount.x = segmentCount.y = 0;

    auto object = node->GetObjectRef();
    if (object->ClassID() == Class_ID(SPHERE_CLASS_ID,0))
    {
        //It's a sphere object
        auto params = object->GetParamBlock();
        
        params->GetValue(PB_RADIUS, time, radius, FOREVER);

        int segments;
        params->GetValue(PB_SEGS, time, segments, FOREVER);
        segmentCount.x = segments;
        segmentCount.y = segments / 2 - 1;
    }
    
    return radius > 0 && segmentCount.x > 0 && segmentCount.y > 0;
}

bool MaxUtilities::GetCylinderRadiusAndLength(float& radius, float& length, INode* node, TimeValue time)
{
    //Defined in maxsdk\samples\objects\cyl.cpp
    enum
    {
        PB_RADIUS = 0,
        PB_HEIGHT = 1
    };

    if (time < 0)
        time = 0;

    radius = length = 0;

    auto object = node->GetObjectRef();
    if (object->ClassID() == Class_ID(CYLINDER_CLASS_ID, 0))
    {
        //It's a cylinder object
        auto params = object->GetParamBlock();        
        params->GetValue(PB_RADIUS, time, radius, FOREVER);
        params->GetValue(PB_HEIGHT, time, length, FOREVER);
    }
    
    return radius > 0 && length > 0;
}

bool MaxUtilities::GetCapsuleRadiusAndLength(float& radius, float& length, INode* node, TimeValue time)
{
    enum
    {
        PB_RADIUS = 0,
        PB_HEIGHT = 1
    };

    if (time < 0)
        time = 0;

    radius = length = 0;

    auto object = node->GetObjectRef();
    if (object->ClassID() == Class_ID(1832744876, 2043230633))
    {
        //It's a capsule object
        auto params = object->GetParamBlock();        
        params->GetValue(PB_RADIUS, time, radius, FOREVER);
        params->GetValue(PB_HEIGHT, time, length, FOREVER);
    }
    
    return radius > 0 && length > 0;
}

bool MaxUtilities::GetPlaneSegmentCount
    (
    FinjinIVector2& segmentCount, 
    INode* node, 
    TimeValue time
    )
{
    if (time < 0)
        time = 0;

    //Reset segment counts
    segmentCount.x = segmentCount.y = 0;

    auto object = node->GetObjectRef();
    if (object->ClassID() == PLANE_CLASS_ID)
    {
        auto pblock = static_cast<IParamBlock2*>(object->GetReference(0));
        ParamID widthSegsID, lengthSegsID;    
        if (pblock != nullptr && 
            MaxUtilities::GetParamIDByName(lengthSegsID, _M("lengthsegs"), pblock) &&
            MaxUtilities::GetParamIDByName(widthSegsID, _M("widthsegs"), pblock))
        {
            segmentCount.x = pblock->GetInt(widthSegsID);
            segmentCount.y = pblock->GetInt(lengthSegsID);
        }
    }

    return segmentCount.x > 0 && segmentCount.y > 0;
}

void MaxUtilities::GetCurrentUnitInfo(wxString& name, float& fractional, bool abbreviate)
{
    DispInfo unitDisplayInfo;
    GetUnitDisplayInfo(&unitDisplayInfo);

    name.clear();
    fractional = 1;

    switch (unitDisplayInfo.dispType)
    {
        case UNITDISP_METRIC:
        {
            switch (unitDisplayInfo.metricDisp)
            {
                case UNIT_METRIC_DISP_MM: name = abbreviate ? wxT("mm") : wxT("millimeters"); break;
                case UNIT_METRIC_DISP_CM: name = abbreviate ? wxT("cm") : wxT("centimeters"); break;
                case UNIT_METRIC_DISP_M: name = abbreviate ? wxT("m") : wxT("meters"); break;
                case UNIT_METRIC_DISP_KM: name = abbreviate ? wxT("km") : wxT("kilometers"); break;
            }
            break;
        }
        case UNITDISP_US:
        {
            switch (unitDisplayInfo.usDisp)
            {
                case UNIT_US_DISP_FRAC_IN: name = abbreviate ? wxT("in") : wxT("inches"); fractional = GetUSFractionalScale(unitDisplayInfo.usFrac); break;
                case UNIT_US_DISP_DEC_IN: name = abbreviate ? wxT("in") : wxT("inches"); break;
                case UNIT_US_DISP_FRAC_FT: name = abbreviate ? wxT("ft") : wxT("feet"); fractional = GetUSFractionalScale(unitDisplayInfo.usFrac); break;
                case UNIT_US_DISP_DEC_FT: name = abbreviate ? wxT("ft") : wxT("feet"); break;
                case UNIT_US_DISP_FT_FRAC_IN: name = abbreviate ? wxT("ft") : wxT("feet"); break;
                case UNIT_US_DISP_FT_DEC_IN: name = abbreviate ? wxT("ft") : wxT("feet"); break;
            }
            break;
        }
        case UNITDISP_CUSTOM:
        {
            name = unitDisplayInfo.customName;
            fractional = GetMasterScale(unitDisplayInfo.customUnit) * unitDisplayInfo.customValue;
            break;
        }
    }

    fractional = 1 / fractional;
}

float MaxUtilities::GetActiveScale()
{
    float scale = 1;
    DispInfo unitDisplayInfo;
    GetUnitDisplayInfo(&unitDisplayInfo);
    switch (unitDisplayInfo.dispType)
    {
        case UNITDISP_METRIC:
        {
            switch (unitDisplayInfo.metricDisp)
            {
                case UNIT_METRIC_DISP_MM: scale = GetMasterScale(UNITS_MILLIMETERS); break;
                case UNIT_METRIC_DISP_CM: scale = GetMasterScale(UNITS_CENTIMETERS); break;
                case UNIT_METRIC_DISP_M: scale = GetMasterScale(UNITS_METERS); break;
                case UNIT_METRIC_DISP_KM: scale = GetMasterScale(UNITS_KILOMETERS); break;
            }
            break;
        }
        case UNITDISP_US:
        {
            switch (unitDisplayInfo.usDisp)
            {
                case UNIT_US_DISP_FRAC_IN: scale = GetMasterScale(UNITS_INCHES); break;
                case UNIT_US_DISP_DEC_IN: scale = GetMasterScale(UNITS_INCHES); break;
                case UNIT_US_DISP_FRAC_FT: scale = GetMasterScale(UNITS_FEET); break;
                case UNIT_US_DISP_DEC_FT: scale = GetMasterScale(UNITS_FEET); break;
                case UNIT_US_DISP_FT_FRAC_IN: scale = GetMasterScale(UNITS_FEET); break;
                case UNIT_US_DISP_FT_DEC_IN: scale = GetMasterScale(UNITS_FEET); break;
            }
            break;
        }
        case UNITDISP_CUSTOM:
        {
            scale = GetMasterScale(unitDisplayInfo.customUnit) / unitDisplayInfo.customValue;
            break;
        }
    }

    return scale;
}

INode* MaxUtilities::GetLookAtNode(INode* node, bool* gotFromController)
{
    auto fromController = false;

    auto lookAtNode = node->GetTarget();
    if (lookAtNode == nullptr)
    {
        auto transformController = node->GetTMController();
        auto rotationController = transformController->GetRotationController();
        if (rotationController != nullptr)
        {
            if (rotationController->ClassID() == Class_ID(LOOKAT_CONSTRAINT_CLASS_ID,0))
            {
                auto lookatControl = static_cast<ILookAtConstRotation*>(rotationController);            
                if (lookatControl->GetNumTargets() > 0)
                {
                    lookAtNode = lookatControl->GetNode(0);
                    fromController = true;
                }
            }
        }
    }

    if (gotFromController != nullptr)
        *gotFromController = fromController;

    return lookAtNode;
}

bool MaxUtilities::GetLookAtDirection
    (
    INode* node, 
    FinjinVector3& direction, 
    const CoordinateSystemConverter& converter
    )
{       
    auto result = false;

    auto transformController = node->GetTMController();
    auto rotationController = transformController->GetRotationController();
    if (rotationController)
    {
        if (rotationController->ClassID() == Class_ID(LOOKAT_CONTROL_CLASS_ID, 0))
        {
            //Old style look at controller
            
            auto lookatControl = static_cast<ILookatControl*>(rotationController);
            auto localDirection = converter.GetAxisVector(lookatControl->GetAxis());
            if (lookatControl->GetFlip())
                localDirection = -localDirection;
            direction.Set(localDirection.x, localDirection.y, localDirection.z);
            
            result = true;
        }
        else if (rotationController->ClassID() == Class_ID(LOOKAT_CONSTRAINT_CLASS_ID, 0))
        {
            //The more recent look at controller
            
            auto lookatControl = static_cast<ILookAtConstRotation*>(rotationController);            
            auto localDirection = converter.GetAxisVector(lookatControl->GetTargetAxis());
            if (lookatControl->GetTargetAxisFlip())
                localDirection = -localDirection;
            direction.Set(localDirection.x, localDirection.y, localDirection.z);
            
            result = true;
        }
    }

    return result;
}

TCHAR* MaxUtilities::GetString(int id)
{
    static TCHAR buffer[MAX_GET_STRING_LENGTH + 1];

    if (LoadString(wxGetInstance(), id, buffer, sizeof(buffer)/sizeof(buffer[0])))
        return buffer;

    return nullptr;
}

TCHAR* MaxUtilities::GetString2(int id)
{
    static TCHAR buffer[MAX_GET_STRING_LENGTH + 1];

    if (LoadString(wxGetInstance(), id, buffer, sizeof(buffer)/sizeof(buffer[0])))
        return buffer;
    
    return nullptr;
}

MCHAR* MaxUtilities::GetStringM(int id)
{
    static MCHAR buffer[MAX_GET_STRING_LENGTH + 1];

    if (LoadString(wxGetInstance(), id, buffer, sizeof(buffer)/sizeof(buffer[0])))
        return buffer;

    return nullptr;
}
