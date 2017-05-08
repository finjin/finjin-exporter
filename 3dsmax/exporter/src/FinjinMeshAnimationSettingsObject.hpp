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
#include "FinjinObjectSettingsObject.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** The Finjin mesh animation settings object. */
    class FinjinMeshAnimationSettingsObject : public Object
    {
    public:
        FinjinMeshAnimationSettingsObject();
        ~FinjinMeshAnimationSettingsObject();

        //From BaseObject
        CreateMouseCallBack* GetCreateMouseCallBack() override;

        //From Object
        ObjectState Eval(TimeValue time) override;
        void InitNodeName(MSTR& s) override;
        Interval ObjectValidity(TimeValue t) override        {return FOREVER;}
        BOOL IsRenderable() override                          {return FALSE;}

        Class_ID ClassID() override;
        SClass_ID SuperClassID() override;
        void GetClassName(MSTR& s) override;

        RefTargetHandle Clone(RemapDir& remap) override;
        RefResult NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate) override;

        int NumSubs() override;
        MSTR SubAnimName(int i) override;
        Animatable* SubAnim(int i) override;

        int NumRefs() override;
        RefTargetHandle GetReference(int i) override;
        void SetReference(int i, RefTargetHandle rtarg) override;

        int NumParamBlocks() override { return 1; }
        IParamBlock2* GetParamBlock(int i) override { return this->pblock; }
        IParamBlock2* GetParamBlockByID(BlockID id) override { return (this->pblock->ID() == id) ? this->pblock : nullptr; }

        void DeleteThis() override { delete this; }

        static Class_ID GetClassClassID();
        static ClassDesc* GetClassDesc();

        wxString GetTrack();
        wxString GetAnimationName();
        wxString GetTimeScaleType();
        double GetTimeScale();
        bool GetCopyFirstKeyToLast();
        bool GetExportAsMorph();
        bool GetUseAnimationStartTime();
        bool GetLooped();
        double GetSampleInterval();
        SampleType GetSampleType();
        Interval GetTimeInterval();
        int GetBoneTranslationMaskCount();
        void GetBoneTranslationMask(int index, INode*& node, Point3& mask);
        Point3 GetBoneTranslationMask(INode* node);
        void ClearBoneTranslationMasks();
        int GetAllowExportBoneCount();
        void GetAllowExportBone(int index, INode*& node, bool& allow, bool& recursive);
        void ClearAllowExportBones();
        InheritedBool GetMorphWholeObject();
        bool GetOverrideAnimatedRootStartTime();
        TimeValue GetAnimatedRootStartTime();
        bool GetEmbedAnimation();
        bool GetLinkToMainObject();

        void SetTrack(wxString value);
        void SetAnimationName(wxString value);
        void SetTimeScaleType(wxString value);
        void SetTimeScale(double value);
        void SetCopyFirstKeyToLast(bool value);
        void SetExportAsMorph(bool value);
        void SetUseAnimationStartTime(bool value);
        void SetLooped(bool value);
        void SetSampleInterval(double value);
        void SetSampleType(SampleType value);
        void SetTimeInterval(Interval interval);
        void AddBoneTranslationMask(INode* node, Point3 mask);
        void AddAllowExportBone(INode* node, bool allow, bool recursive);
        void SetMorphWholeObject(InheritedBool value);
        void SetOverrideAnimatedRootStartTime(bool value);
        void SetAnimatedRootStartTime(TimeValue value);
        void SetTimeInterval(FinjinTimeInterval interval);
        void SetEmbedAnimation(bool value);
        void SetLinkToMainObject(bool value);

    public:
        enum
        {
            PARAM_BLOCK_REF,
            NUM_REFS
        };

        /** Parameter block parameters */
        enum
        {
            PB_TRACK,
            PB_ANIMATION_NAME,
            PB_TIME_START,
            PB_TIME_END,
            PB_TIME_SCALE_TYPE,
            PB_TIME_SCALE,
            PB_COPY_FIRST_KEY_TO_LAST,
            PB_USE_ANIMATION_START_TIME,
            PB_LOOPED,
            PB_SAMPLE_INTERVAL,
            PB_SAMPLE_TYPE,
            PB_BONE_TRANSLATION_MASK_NODES,
            PB_BONE_TRANSLATION_MASKS,
            PB_ALLOW_EXPORT_BONE_NODES,
            PB_ALLOW_EXPORT_BONE_ALLOWS,
            PB_ALLOW_EXPORT_BONE_RECURSIVES,
            PB_EXPORT_AS_MORPH,
            PB_MORPH_WHOLE_OBJECT,
            PB_LINK_TO_MAIN_OBJECT,
            PB_EMBED_ANIMATION,
            PB_OVERRIDE_ANIMATED_ROOT_START_TIME,
            PB_ANIMATED_ROOT_START_TIME
        };

        /** Reference 0 */
        IParamBlock2* pblock;

        IObjParam* ip;
    };

} }
