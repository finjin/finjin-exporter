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


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Various functions for dealing with bitmaps in 3DS Max. */
    class MaxBitmapUtilities
    {
    public:
        /** 
         * Copies various bitmap properties from one location to another.
         * This is needed because the default '=' operator doesn't copy
         * everything that is needed.
         */
        static void CopyBitmapInfo(BitmapInfo& destinationBitmapInfo, BitmapInfo& sourceBitmapInfo);

        /**
         * Resizes an image file and saves the resized image to a new file.
         * @param sourceFileName [in] - The file to resize.
         * @param destinationFileName [in] - The destination for the resized image. 
         * This file name can have a different type (extension) than the source.
         * @param width [in] - The new resized width.
         * @param height [in] - The new resized height.
         */
        static void ResizeImage(const wxString& sourceFileName, const wxString& destinationFileName, int width, int height);

        /** 
         * Saves the specified bitmap to a file.
         * @param bitmap [in] - The bitmap to save.
         * @param bitmapInfo [in] - Bitmap information, including the destination file path.
         * @param configure [in] - Indicates whether the configuration information for
         * the bitmap's type should be loaded. This only needs to be true if saving the first
         * of a sequence of bitmaps with a particular type.
         */
        static void SaveBitmap(Bitmap* bitmap, BitmapInfo& bitmapInfo, bool configure);
    };

} }
