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
#include "MaxBitmapUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
void MaxBitmapUtilities::CopyBitmapInfo(BitmapInfo& destinationBitmapInfo, BitmapInfo& sourceBitmapInfo)
{
    destinationBitmapInfo.SetWidth(sourceBitmapInfo.Width());
    destinationBitmapInfo.SetHeight(sourceBitmapInfo.Height());
    destinationBitmapInfo.SetAspect(sourceBitmapInfo.Aspect());
    destinationBitmapInfo.SetGamma(sourceBitmapInfo.Gamma());
    destinationBitmapInfo.SetType(sourceBitmapInfo.Type());
    destinationBitmapInfo.SetFlags(sourceBitmapInfo.Flags());
    destinationBitmapInfo.SetCustomFlag(sourceBitmapInfo.GetCustomFlags());    
}

void MaxBitmapUtilities::ResizeImage(const wxString& sourceFileName, const wxString& destinationFileName, int width, int height)
{
    BMM_Color_fl black;

    MSTR maxSourceFileName = WxStringToApplicationStringM(sourceFileName);
    BitmapInfo sourceBitmapInfo(maxSourceFileName);
    std::unique_ptr<Bitmap, DeleteThisDeleter<Bitmap> > sourceBitmap(TheManager->Load(&sourceBitmapInfo));
    if (sourceBitmap != nullptr)
    {
        BitmapInfo destinationBitmapInfo;
        CopyBitmapInfo(destinationBitmapInfo, sourceBitmapInfo);
        destinationBitmapInfo.SetWidth(width);
        destinationBitmapInfo.SetHeight(height);        
        destinationBitmapInfo.SetName(destinationFileName.wx_str());
        std::unique_ptr<Bitmap, DeleteThisDeleter<Bitmap> > destinationBitmap(TheManager->Create(&destinationBitmapInfo));
        
        //Locate the device for the destination file format
        int sourceDeviceIndex = TheManager->ioList.ResolveDevice(&sourceBitmapInfo);
        int destinationDeviceIndex = TheManager->ioList.ResolveDevice(&destinationBitmapInfo);        
        if (sourceDeviceIndex != -1 && destinationDeviceIndex != -1)
        {
            //Create the device so that we can get its default settings
            std::shared_ptr<BitmapIO> sourceIO(TheManager->ioList.CreateDevInstance(sourceDeviceIndex));
            std::shared_ptr<BitmapIO> destinationIO;
            if (sourceDeviceIndex == destinationDeviceIndex) 
                destinationIO = sourceIO;
            else
                destinationIO.reset(TheManager->ioList.CreateDevInstance(destinationDeviceIndex));
            if (sourceIO != nullptr && destinationIO != nullptr)
            {              
                //Get the number of bytes required by the configuration
                DWORD piSizeSource = sourceIO->EvaluateConfigure();
                DWORD piSizeDestination = (destinationDeviceIndex == sourceDeviceIndex) ? piSizeSource : destinationIO->EvaluateConfigure();
                if (piSizeSource > 0 && piSizeDestination > 0)
                {
                    //Set the configuration info into the source and destination bitmap info
                    sourceBitmapInfo.AllocPiData(piSizeSource);
                    sourceIO->ReadCfg();
                    sourceIO->SaveConfigure(sourceBitmapInfo.GetPiData());

                    if (destinationDeviceIndex != sourceDeviceIndex)
                    {
                        destinationBitmapInfo.AllocPiData(piSizeDestination);
                        destinationIO->ReadCfg();
                    }
                    destinationIO->SaveConfigure(sourceBitmapInfo.GetPiData());

                    destinationBitmap->CopyImage(sourceBitmap.get(), COPY_IMAGE_RESIZE_HI_QUALITY, black);

                    //Open the output file and write to it
                    if (destinationBitmap->OpenOutput(&destinationBitmapInfo) == BMMRES_SUCCESS)
                    {
                        destinationBitmap->WriteAll();
                        destinationBitmap->CloseAll();
                    }
                }                
            }
        }
    }
}

void MaxBitmapUtilities::SaveBitmap(Bitmap* bitmap, BitmapInfo& bitmapInfo, bool configure)
{
    if (configure)
    {
        //Locate the device for the destination file format
        int deviceIndex = TheManager->ioList.ResolveDevice(&bitmapInfo);
        if (deviceIndex != -1)
        {
            //Create the device so that we can get its default settings
            std::unique_ptr<BitmapIO> io(TheManager->ioList.CreateDevInstance(deviceIndex));
            if (io != nullptr)
            {              
                //Get the number of bytes required by the configuration
                DWORD piSize = io->EvaluateConfigure();
                if (piSize > 0)
                {
                    //Set the configuration info into the destination bitmap info
                    bitmapInfo.AllocPiData(piSize);
                    io->ReadCfg();
                    io->SaveConfigure(bitmapInfo.GetPiData());

                    //Open the output file and write to it
                    if (bitmap->OpenOutput(&bitmapInfo) == BMMRES_SUCCESS)
                    {
                        //Write the frames
                        bitmap->WriteAll();
                        
                        //Close the output
                        bitmap->CloseAll();
                    }
                }
            }
        }
    }
    else
    {
        if (bitmap->OpenOutput(&bitmapInfo) == BMMRES_SUCCESS)
        {
            //Write the frames
            bitmap->WriteAll();
            
            //Close the output
            bitmap->CloseAll();
        }
    }
}
