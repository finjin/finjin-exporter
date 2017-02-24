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
#include "CubeMapRenderer.hpp"

#if defined(FINJIN_EXPORTER_ENABLE_D3D_FUNCTIONALITY)

#include "Direct3DState.hpp"
#include "FileUtilities.hpp"
#include "MaxMathUtilities.hpp"
#include "MaxUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
CubeMapRenderer::CubeMapRenderer()
{
    this->centerNode = nullptr;
}

void CubeMapRenderer::SetCenterNode(INode* node)
{
    this->centerNode = node;
}

void CubeMapRenderer::RenderToFile
    (
    const wxString& fileName, 
    int dimensions,
    bool separateFiles
    )
{
    enum {FACE_COUNT = 6};

    static const TCHAR* faceSuffixes[FACE_COUNT] = 
        {
        _T("_fr"), 
        _T("_bk"), 
        _T("_up"), 
        _T("_dn"), 
        _T("_lf"), 
        _T("_rt")
        };

    static const Matrix3 faceRotations[FACE_COUNT] = 
        {
        RotateZMatrix(0),
        RotateZMatrix(Max::PI),
        RotateXMatrix(Max::PI / 2),
        RotateXMatrix(-Max::PI / 2),
        RotateZMatrix(Max::PI / 2),
        RotateZMatrix(-Max::PI / 2)
        };

    static const D3DCUBEMAP_FACES d3dFaceIndices[FACE_COUNT] = 
        {
        D3DCUBEMAP_FACE_POSITIVE_Z,
        D3DCUBEMAP_FACE_NEGATIVE_Z,
        D3DCUBEMAP_FACE_POSITIVE_Y,
        D3DCUBEMAP_FACE_NEGATIVE_Y,
        D3DCUBEMAP_FACE_NEGATIVE_X,
        D3DCUBEMAP_FACE_POSITIVE_X
        };

    auto maxInterface = GetCOREInterface();

    Point3 center(0,0,0);
    if (this->centerNode != nullptr)
        center = this->centerNode->GetObjectTM(maxInterface->GetTime()).GetTrans();
    
    auto fileNameNoExt = FileUtilities::RemoveExtension(fileName);
    auto ext = FileUtilities::GetExtension(fileName, true);

    wxString faceFileNames[FACE_COUNT];
    for (int i = 0; i < FACE_COUNT; i++)
        faceFileNames[i] = fileNameNoExt + faceSuffixes[i] + ext;
    
    //Get renderer
    auto renderer = maxInterface->GetProductionRenderer();

    //Get view
    auto baseCameraMatrix = MaxUtilities::GetCameraIdentityMatrix();
    
    ViewParams viewParams;
    viewParams.projType = PROJ_PERSPECTIVE;
    viewParams.hither = .001f;
    viewParams.yon = 1.0e30f;
    viewParams.distance = 1;
    viewParams.zoom = 1;
    viewParams.fov = Max::PI / 2;
    viewParams.nearRange = 0;
    viewParams.farRange = 1.0e30f;

    //Render faces
    BitmapInfo faceInfo[FACE_COUNT];
    Bitmap* faces[FACE_COUNT] = {0};
    for (int i = 0; i < FACE_COUNT; i++)
    {
        auto cameraMatrix = baseCameraMatrix * faceRotations[i];
        cameraMatrix.SetTrans(center);            
        viewParams.prevAffineTM = viewParams.affineTM = Inverse(cameraMatrix);

        //Initialize render system
        if (maxInterface->OpenRenderer(renderer, &viewParams, RENDTYPE_NORMAL, dimensions, dimensions))
        {
            auto& info = faceInfo[i];
            info.SetWidth(dimensions);
            info.SetHeight(dimensions);
            info.SetType(BMM_TRUE_32);
            info.SetFlags(MAP_HAS_ALPHA);                
            info.SetName(faceFileNames[i]);                
            faces[i] = TheManager->Create(&info);                
            faces[i]->CreateChannels(BMM_CHAN_COLOR);
                            
            //Perform render
            maxInterface->RendererRenderFrame(renderer, maxInterface->GetTime(), faces[i]);
            maxInterface->CloseRenderer(renderer);
        }
    }

    //Save
    if (separateFiles)
    {
        //Save individual files
        for (int i = 0; i < FACE_COUNT; i++)
        {
            if (faces[i] != nullptr)
            {
                faces[i]->OpenOutput(&faceInfo[i]);
                faces[i]->WriteAll();
                faces[i]->CloseAll();
            }
        }
    }
    else
    {
        //Create DDS file

        //Initialize Direct3D
        Direct3DState d3d;
        if (d3d.Create())
        {
            //Create Direct3D cube texture
            LPDIRECT3DCUBETEXTURE9 d3dCubeMap;
            auto hr = d3d.d3dDevice->CreateCubeTexture(dimensions, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &d3dCubeMap, nullptr);
            if (SUCCEEDED(hr))
            {
                //Copy render cube faces to the faces of the Direct3D cube texture
                for (int i = 0; i < FACE_COUNT; i++)
                {
                    //Lock the Direct3D cube texture bits
                    D3DLOCKED_RECT lockedRect;
                    hr = d3dCubeMap->LockRect(d3dFaceIndices[i], 0, &lockedRect, nullptr, D3DLOCK_DISCARD);
                    if (FAILED(hr))
                        continue;

                    //Copy rendered cube face to Direct3D cube texture face
                    ULONG channelType;
                    int alphaType;
                    auto d3dFaceBytes = static_cast<uint8_t*>(lockedRect.pBits);
                    auto faceColorPixels = reinterpret_cast<Color24*>(faces[i]->GetChannel(BMM_CHAN_COLOR, channelType));
                    auto faceAlphaBytes = static_cast<uint8_t*>(faces[i]->GetAlphaPtr(&alphaType));
                    for (int row = 0; row < dimensions; row++)
                    {
                        auto d3dFacePixels = reinterpret_cast<uint32_t*>(d3dFaceBytes);
                        
                        for (int column = 0; column < dimensions; column++)
                        {
                            *d3dFacePixels++ =
                                uint32_t((faceAlphaBytes != nullptr) ? *faceAlphaBytes++ : 255) << 24 |
                                uint32_t(faceColorPixels->r) << 16 |
                                uint32_t(faceColorPixels->g) << 8 |
                                uint32_t(faceColorPixels->b)
                                ;
                            faceColorPixels++;
                        }

                        d3dFaceBytes += lockedRect.Pitch;
                    }

                    //Unlock the Direct3D cube texture bits
                    d3dCubeMap->UnlockRect(d3dFaceIndices[i], 0);
                }

                //Save the generated Direct3D cube texture
                D3DXSaveTextureToFile(fileName.wx_str(), D3DXIFF_DDS, d3dCubeMap, nullptr);
            
                //Release the Direct3D cube texture
                d3dCubeMap->Release();
            }

            d3d.Destroy();
        }
    }

    //Delete bitmaps
    for (int i = 0; i < FACE_COUNT; i++)
    {
        if (faces[i] != nullptr)
            faces[i]->DeleteThis();            
    }
}

#endif
