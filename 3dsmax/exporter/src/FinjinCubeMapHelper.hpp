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

#if defined(FINJIN_EXPORTER_ENABLE_D3D_FUNCTIONALITY)

//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {
    
    /** The Finjin cube map helper object. */
    class FinjinCubeMapHelper : public HelperObject
    {
    public:
        FinjinCubeMapHelper();
        ~FinjinCubeMapHelper();

        //From HelperObject
        int UsesWireColor() override { return TRUE; }

        //From BaseObject
        int HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2* p, ViewExp* vpt) override;
        void Snap(TimeValue t, INode* inode, SnapInfo* snap, IPoint2* p, ViewExp* vpt) override;
        int Display(TimeValue t, INode* inode, ViewExp* vpt, int flags) override;
        CreateMouseCallBack* GetCreateMouseCallBack() override;
        void BeginEditParams(IObjParam* ip, ULONG flags, Animatable* prev) override;
        void EndEditParams(IObjParam* ip, ULONG flags, Animatable* next) override;

        //From Object
        ObjectState Eval(TimeValue time) override;
        void InitNodeName(MSTR& s) override;
        Interval ObjectValidity(TimeValue t) override        {return FOREVER;}
        int CanConvertToType(Class_ID obtype) override        {return FALSE;}
        Object* ConvertToType(TimeValue t, Class_ID obtype) override {return nullptr;}        
        void GetWorldBoundBox(TimeValue t, INode* inode, ViewExp* vpt, Box3& box) override;
        void GetLocalBoundBox(TimeValue t, INode* inode, ViewExp* vpt, Box3& box) override;
        int DoOwnSelectHilite() override                        {return TRUE;}
        
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

        float GetIconSize(TimeValue t = 0);
        void SetIconSize(float value, TimeValue t = 0);

        BOOL GetAlwaysFaceView();

        int GetCubeFaceSize(TimeValue t = 0);
        void SetCubeFaceSize(int value, TimeValue t = 0);

        bool GetRenderToSeparateFiles();
        bool GetRenderToDDSFile();
        
        wxString GetBaseFileName(TimeValue t = 0);
        void SetBaseFileName(wxString value, TimeValue t = 0);

        bool GetRenderOnExport(TimeValue t = 0);

        Matrix3 GetIconSizeMatrix(TimeValue t = 0);

        std::vector<Point3>& GetEdgeVertices();

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
            PB_ICON_SIZE,
            PB_RENDER_ON_EXPORT,
            PB_CUBE_FACE_SIZE,
            PB_FILE_TYPE,
            PB_BASE_FILE_NAME
        };

        /** Reference 0 */
        IParamBlock2* pblock;

        IObjParam* ip;

        bool suspendSnap;        
    };

} }

#endif
