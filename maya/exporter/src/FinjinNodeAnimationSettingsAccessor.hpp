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
#include "FinjinNodeAnimationSettingsAccessorBase.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class NodeAttributeAdder;

    /** Access to a Finjin node animation settings object. */
    class FinjinNodeAnimationSettingsAccessor : public FinjinNodeAnimationSettingsAccessorBase
    {
    public:    
        FinjinNodeAnimationSettingsAccessor();
        FinjinNodeAnimationSettingsAccessor(MPlug rootPlug);
        
        bool IsValid() const {return !this->rootPlug.isNull();}

        bool operator == (const FinjinNodeAnimationSettingsAccessor& other) const;

        wxString GetAnimationName() override;
        wxString GetTimeScaleType() override;
        double GetTimeScale() override;
        bool GetCopyFirstKeyToLast() override;
        bool GetUseAnimationStartTime() override;
        bool GetEmbedAnimation() override;
        bool GetLinkToMainObject() override;
        bool GetEnabled() override;
        bool GetLooped() override;
        double GetSampleInterval() override;
        SampleType GetSampleType() override;
        FinjinTimeInterval GetTimeInterval() override;
        wxString GetTranslationInterpolationType() override;
        wxString GetRotationInterpolationType() override;

        void SetAnimationName(wxString value) override;
        void SetTimeScaleType(wxString value) override;
        void SetTimeScale(double value) override;
        void SetCopyFirstKeyToLast(bool value) override;
        void SetUseAnimationStartTime(bool value) override;
        void SetEmbedAnimation(bool value) override;
        void SetLinkToMainObject(bool value) override;
        void SetEnabled(bool value) override;
        void SetLooped(bool value) override;
        void SetSampleInterval(double value) override;
        void SetSampleType(SampleType value) override;
        void SetTimeInterval(FinjinTimeInterval interval) override;
        void SetTranslationInterpolationType(wxString value) override;
        void SetRotationInterpolationType(wxString value) override;

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

            MObject animationName;
            MObject timeStart;
            MObject timeEnd;
            MObject timeScaleType;
            MObject timeScale;
            MObject copyFirstKeyToLast;
            MObject useAnimationStartTime;
            MObject embedAnimation;
            MObject linkToMainObject;
            MObject enabled;
            MObject looped;
            MObject sampleInterval;
            MObject sampleType;
            MObject translationInterpolationType;
            MObject rotationInterpolationType;
        };
        static Attributes attributes;
    };

} }
