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
#include "FinjinMeshAnimationSettingsAccessorBase.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class NodeAttributeAdder;

    /** Access to a Finjin mesh animation settings object. */
    class FinjinMeshAnimationSettingsAccessor : public FinjinMeshAnimationSettingsAccessorBase
    {
    public:    
        FinjinMeshAnimationSettingsAccessor();
        FinjinMeshAnimationSettingsAccessor(MPlug rootPlug);
        
        bool IsValid() const {return !this->rootPlug.isNull();}

        bool operator == (const FinjinMeshAnimationSettingsAccessor& other) const;

        wxString GetTrack() override;
        wxString GetAnimationName() override;
        wxString GetTimeScaleType() override;
        double GetTimeScale() override;
        bool GetCopyFirstKeyToLast() override;
        bool GetExportAsMorph() override;
        InheritedBool GetMorphWholeObject() override;
        bool GetOverrideAnimatedRootStartTime() override;
        TimeAccessor GetAnimatedRootStartTime() override;
        bool GetUseAnimationStartTime() override;
        bool GetLooped() override;
        double GetSampleInterval() override;
        SampleType GetSampleType() override;
        FinjinTimeInterval GetTimeInterval() override;
        bool GetEmbedAnimation() override;
        bool GetLinkToMainObject() override;
        int GetBoneTranslationMaskCount() override;
        void GetBoneTranslationMask(int index, ObjectAccessor& object, FinjinVector3& mask) override;
        FinjinVector3 GetBoneTranslationMask(ObjectAccessor object) override;
        int GetAllowExportBoneCount() override;
        void GetAllowExportBone(int i, ObjectAccessor& object, bool& allow, bool& recursive) override;
        
        void SetTrack(wxString value) override;
        void SetAnimationName(wxString value) override;
        void SetTimeScaleType(wxString value) override;
        void SetTimeScale(double value) override;
        void SetCopyFirstKeyToLast(bool value) override;
        void SetExportAsMorph(bool value) override;
        void SetMorphWholeObject(InheritedBool value) override;        
        void SetOverrideAnimatedRootStartTime(bool value) override;
        void SetAnimatedRootStartTime(TimeAccessor value) override;
        void SetUseAnimationStartTime(bool value) override;
        void SetLooped(bool value) override;
        void SetSampleInterval(double value) override;
        void SetSampleType(SampleType value) override;
        void SetTimeInterval(FinjinTimeInterval interval) override;
        void SetEmbedAnimation(bool value) override;
        void SetLinkToMainObject(bool value) override;
        void ClearBoneTranslationMasks() override;
        void AddBoneTranslationMask(ObjectAccessor object, FinjinVector3 mask) override;
        void ClearAllowExportBones() override;
        void AddAllowExportBone(ObjectAccessor object, bool allow, bool recursive) override;

        static MObject CreateSettingsArrayAttribute(NodeAttributeAdder& adder, const MString& name);

    private:
        MPlug Child(MObject attribute, const MString& name);
        int ChildInt(MObject attribute, const MString& name, int defaultValue = 0);
        bool ChildBool(MObject attribute, const MString& name, bool defaultValue = false);
        float ChildFloat(MObject attribute, const MString& name, float defaultValue = 0);
        double ChildDouble(MObject attribute, const MString& name, double defaultValue = 0);
        MString ChildString(MObject attribute, const MString& name);
        int ChildNumElements(MObject attribute, const MString& name);

    public:
        MPlug rootPlug;

        struct Attributes
        {        
            MObject updateCounter;

            MObject trackName;
            MObject animationName;
            MObject timeStart;
            MObject timeEnd;
            MObject timeScaleType;
            MObject timeScale;
            MObject copyFirstKeyToLast;
            MObject exportAsMorph;
            MObject morphWholeObject;
            MObject overrideAnimatedRootStartTime;
            MObject animatedRootStartTime;
            MObject useAnimationStartTime;
            MObject looped;
            MObject sampleInterval;
            MObject sampleType;
            MObject embedAnimation;
            MObject linkToMainObject;
            
            MObject boneTranslationMaskObjects;
            MObject boneTranslationMasks;
            MObject allowExportBonesObjects;
            MObject allowExportBoneAllows;
            MObject allowExportBoneRecursives;
        };
        static Attributes attributes;
    };

} }
