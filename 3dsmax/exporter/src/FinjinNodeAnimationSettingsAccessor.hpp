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
#include "FinjinNodeAnimationSettingsAccessorBase.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class FinjinNodeAnimationSettingsObject;

    /** Access to a Finjin node animation settings object. */
    class FinjinNodeAnimationSettingsAccessor : public FinjinNodeAnimationSettingsAccessorBase
    {
    public:
        FinjinNodeAnimationSettingsAccessor() {this->object = nullptr;}
        FinjinNodeAnimationSettingsAccessor(FinjinNodeAnimationSettingsObject* object) {this->object = object;}

        bool IsValid() const {return this->object != nullptr;}

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

    public:
        FinjinNodeAnimationSettingsObject* object;
    };

} }
