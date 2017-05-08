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


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class ExtendedValue;

    /** The Finjin extended value object. */
    class ExtendedValueObject : public Object
    {
    public:
        ExtendedValueObject();
        ~ExtendedValueObject();

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

        int NumSubs() override;
        MSTR SubAnimName(int i) override;
        Animatable* SubAnim(int i) override;

        RefResult NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate) override;

        int NumRefs() override;
        RefTargetHandle GetReference(int i) override;
        void SetReference(int i, RefTargetHandle rtarg) override;

        int NumParamBlocks() override { return 1; }
        IParamBlock2* GetParamBlock(int i) override { return this->pblock; }
        IParamBlock2* GetParamBlockByID(BlockID id) override { return (this->pblock->ID() == id) ? this->pblock : nullptr; }

        void DeleteThis() override { delete this; }

        int GetID();
        void SetID(int id);

        void GetValue(ExtendedValue& value);
        void SetValue(const ExtendedValue& value);

        static Class_ID GetClassClassID();
        static ClassDesc* GetClassDesc();

    public:
        enum
        {
            PARAM_BLOCK_REF,
            NUM_REFS
        };

        /** Parameter block parameters */
        enum
        {
            PB_ID,
            PB_TYPE,
            PB_FLOATS,
            PB_STRINGS,
            PB_NODES,
            PB_MATERIALS
        };

        /** Reference 0 */
        IParamBlock2* pblock;

        IObjParam* ip;
    };

} }
