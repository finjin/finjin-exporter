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
#include "TerrainMapRenderer.hpp"

#if defined(FINJIN_EXPORTER_ENABLE_D3D_FUNCTIONALITY)

#include "Direct3DState.hpp"
#include "MaxBitmapUtilities.hpp"
#include "MaxUtilities.hpp"
#include "MathUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
TerrainMapRenderer::TerrainMapRenderer(Mode mode)
{
    this->mode = mode;
}

void TerrainMapRenderer::AddSelectedNodes()
{
    int selectedNodeCount = GetCOREInterface()->GetSelNodeCount();
    for (int i = 0; i < selectedNodeCount; i++)
        this->nodesToRender.insert(GetCOREInterface()->GetSelNode(i));
}

void TerrainMapRenderer::AddNode(INode* node)
{
    this->nodesToRender.insert(node);
}

void TerrainMapRenderer::RenderToFile
    (
    const wxString& fileName, 
    int width,
    int height,
    int bytesPerPixel,
    bool rotate90DegreesClockwise
    )
{
    auto maxInterface = GetCOREInterface();

    auto renderingSelectedNodes = !this->nodesToRender.empty();

    //Get all the scene nodes
    std::unordered_set<INode*> allNodes;
    MaxUtilities::GetAllNodes(allNodes);
    
    //Prepare the scene
    std::unordered_set<INode*> hiddenNodes;
    if (renderingSelectedNodes)
    {
        //Hide all unselected, visible nodes
        for (auto node : allNodes)
        {
            if (this->nodesToRender.find(node) == this->nodesToRender.end() && !node->IsObjectHidden())
            {
                node->Hide(TRUE);
                hiddenNodes.insert(node);
            }
        }
    }
    else
    {
        //Rendering all of the scene nodes
        this->nodesToRender = allNodes;
    }

    //Get the world bounding box for all selected objects
    auto currentTime = maxInterface->GetTime();

    //Get the bounding box for the world to be rendered
    Box3 worldBoundingBox;
    for (auto node : this->nodesToRender)
    {
        auto obj = node->EvalWorldState(currentTime).obj;
        auto worldTM = node->GetObjectTM(currentTime);
        Box3 box;
        obj->GetDeformBBox(currentTime, box, &worldTM, FALSE);
        if (node == *this->nodesToRender.begin())
            worldBoundingBox = box;
        else
            worldBoundingBox += box;
    }
    auto worldSize = worldBoundingBox.Width();

    //Get renderer
    auto renderer = maxInterface->GetProductionRenderer();

    //Calculate camera
    auto baseCameraMatrix = MaxUtilities::GetCameraIdentityMatrix();
    auto cameraMatrix = baseCameraMatrix * RotateXMatrix(-Max::PI / 2); //Looking down
    if (rotate90DegreesClockwise)
    {
        //To rotate the image 90 degrees clockwise, rotate the camera 90 degrees counter-clockwise
        cameraMatrix = cameraMatrix * RotateZMatrix(Max::PI / 2); 
    }

    auto worldCenter = worldBoundingBox.Center();
    Point3 cameraCenter(worldCenter.x, worldCenter.y, worldBoundingBox.Max().z);
    cameraMatrix.SetTrans(cameraCenter);
    
    //Initialize view parameters
    ViewParams viewParams;
    viewParams.projType = PROJ_PARALLEL;
    viewParams.hither = .001f;
    viewParams.yon = 1.0e30f;
    viewParams.distance = 1;
    viewParams.zoom = std::max(worldSize.x, worldSize.y) / 400; //The scale is relative to a 400 unit wide viewport
    viewParams.nearRange = 0;
    viewParams.farRange = 1.0e30f;
    viewParams.prevAffineTM = viewParams.affineTM = Inverse(cameraMatrix);

    //Initialize render system
    if (maxInterface->OpenRenderer(renderer, &viewParams, RENDTYPE_NORMAL, width, height))
    {
        //Create bitmap to render into
        BitmapInfo bitmapInfo;
        bitmapInfo.SetWidth(width);
        bitmapInfo.SetHeight(height);
        bitmapInfo.SetType(this->mode == COLOR_MAP ? BMM_TRUE_32 : BMM_GRAY_8);
        bitmapInfo.SetName(fileName.wx_str());        
        auto bitmap = TheManager->Create(&bitmapInfo);
        bitmap->CreateChannels(BMM_CHAN_COLOR | BMM_CHAN_Z);
                                        
        //Perform render
        maxInterface->RendererRenderFrame(renderer, maxInterface->GetTime(), bitmap);
        maxInterface->CloseRenderer(renderer);
        
        //Process the rendered bitmap
        if (this->mode == COLOR_MAP)
        {
            //Standard bitmap
            MaxBitmapUtilities::SaveBitmap(bitmap, bitmapInfo, true);
        }
        else
        {
            //Height map
            //Note that in 3DS Max, all depth values are negative. Near depths are closer 
            //to 0, and far depths are closer to negative infinity                

            ULONG depthType;
            auto bitmapDepths = (float*)bitmap->GetChannel(BMM_CHAN_Z, depthType);
            float maxDepth = 0; 
            float minDepth = FLT_MAX;
            const float infiniteDepth = -1000000000000000000000000000000.0f;
            if (bitmapDepths != nullptr)
            {
                //Get the depth range
                for (int row = 0; row < height; row++)
                {
                    for (int column = 0; column < width; column++)
                    {
                        float depth = *bitmapDepths++;

                        //Ignore infinite depth values since we only want to consider
                        //values in which objects are contained within the view
                        if (depth > infiniteDepth)
                        {
                            depth = -depth;
                            maxDepth = std::max(maxDepth, depth);
                            minDepth = std::min(minDepth, depth);
                        }
                    }
                }
                float depthRange = maxDepth - minDepth;
                float oneOverDepthRange = MathUtilities::AlmostZero(depthRange, (float)FLOAT_EPSILON) ? 0 : 1 / depthRange;                    

                //Initialize Direct3D
                Direct3DState d3d;
                if (d3d.Create())
                {                
                    //Create Direct3D texture
                    LPDIRECT3DTEXTURE9 d3dTexture;
                    D3DFORMAT format = bytesPerPixel == 1 ? D3DFMT_L8 : D3DFMT_L16;
                    auto hr = d3d.d3dDevice->CreateTexture(width, height, 1, 0, format, D3DPOOL_SYSTEMMEM, &d3dTexture, nullptr);
                    if (SUCCEEDED(hr))
                    {
                        //Lock the Direct3D texture bits
                        D3DLOCKED_RECT lockedRect;
                        hr = d3dTexture->LockRect(0, &lockedRect, nullptr, D3DLOCK_DISCARD);
                        if (SUCCEEDED(hr))
                        {
                            auto d3dFaceBytes = (uint8_t*)lockedRect.pBits;
                
                            //Convert the render bitmap's depths to heights
                            auto bitmapDepths = (float*)bitmap->GetChannel(BMM_CHAN_Z, depthType);
                            for (int row = 0; row < height; row++)
                            {
                                int rowByteOffset = 0;

                                for (int column = 0; column < width; column++)
                                {
                                    //Convert depth to normalized height
                                    float depth = *bitmapDepths++;
                                    float height = 0;
                                    if (depth > infiniteDepth)
                                    {
                                        depth = -depth;
                                        height = Limited((maxDepth - depth) * oneOverDepthRange, 0.0f, 1.0f);                        
                                    }
                                                
                                    //Put the height into the Direct3D texture
                                    if (bytesPerPixel == 1)
                                        d3dFaceBytes[rowByteOffset++] = (uint8_t)(height * 255);
                                    else //if (bytesPerPixel == 2)
                                    {
                                        *(unsigned short*)&d3dFaceBytes[rowByteOffset] = (unsigned short)(height * 65535);
                                        rowByteOffset += 2;
                                    }
                                }

                                d3dFaceBytes += lockedRect.Pitch;
                            }                        
                
                            //Unlock the Direct3D texture bits
                            d3dTexture->UnlockRect(0);

                            //Save the generated Direct3D texture
                            D3DXSaveTextureToFile(fileName.wx_str(), D3DXIFF_DDS, d3dTexture, nullptr);
                        }

                        //Release the Direct3D texture
                        d3dTexture->Release();
                    }

                    d3d.Destroy();
                }
            }
        }

        bitmap->DeleteThis();
    }

    if (renderingSelectedNodes)
    {
        //Show all hidden nodes
        for (auto node : hiddenNodes)
            node->Hide(FALSE);
    }
}

#endif
