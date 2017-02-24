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
#include "ImageFilePrompt.hpp"
#include "Strings.hpp"
#include "SuspendCustomControlsHook.hpp"
#include <FreeImage.h>

using namespace Finjin::Exporter;


//Local functions--------------------------------------------------------------
static wxString& GetImageFileFilter()
{
    static wxString IMAGE_FILE_FILTER;
    if (!IMAGE_FILE_FILTER.empty())
        return IMAGE_FILE_FILTER;

    FREE_IMAGE_FORMAT formats[] = { FIF_DDS, FIF_PNG, FIF_TARGA };
    for (size_t i = 0; i < FINJIN_COUNT_OF(formats); i++)
    {
        FREE_IMAGE_FORMAT format = formats[i];
        FreeImage_SetPluginEnabled(format, TRUE);
        if (FreeImage_IsPluginEnabled(format) && FreeImage_FIFSupportsWriting(format))
        {
            if (!IMAGE_FILE_FILTER.empty())
                IMAGE_FILE_FILTER += wxT("|");

            //Split extensions and add 
            wxString extensionsString(FreeImage_GetFIFExtensionList(format), wxMBConvUTF8());
            wxStringTokenizer extensionsTokenizer(extensionsString, wxT(","));
            wxArrayString extensions;
            while (extensionsTokenizer.HasMoreTokens())
                extensions.Add(extensionsTokenizer.GetNextToken());

            //Add to display string
            auto extensionUpper = extensions[0];
            extensionUpper.MakeUpper();
            IMAGE_FILE_FILTER += extensionUpper;
            IMAGE_FILE_FILTER += wxT(" Files (");
            for (size_t i = 0; i < extensions.size(); i++)
            {
                auto& extension = extensions[i];

                if (i > 0)
                    IMAGE_FILE_FILTER += wxT(",");
                IMAGE_FILE_FILTER += wxT("*.");
                IMAGE_FILE_FILTER += extension;
            }
            IMAGE_FILE_FILTER += wxT(")|");

            //Add to filter string
            for (size_t i = 0; i < extensions.size(); i++)
            {
                auto& extension = extensions[i];

                if (i > 0)
                    IMAGE_FILE_FILTER += wxT(",");
                IMAGE_FILE_FILTER += wxT("*.");
                IMAGE_FILE_FILTER += extension;
            }
        }
    }

    return IMAGE_FILE_FILTER;
}


//Implementation---------------------------------------------------------------
ImageFilePrompt::ImageFilePrompt(wxWindow* parent, bool saving)
{
    this->parent = parent;
    this->saving = saving;
}

const wxString& ImageFilePrompt::GetFileName() const
{
    return this->fileName;
}

bool ImageFilePrompt::ShowPrompt()
{
    SUSPEND_CUSTOM_CONTROLS;

    wxFileDialog dialog(this->parent, Strings::SELECT_IMAGE_FILE, wxEmptyString, wxEmptyString, GetImageFileFilter(), saving ? wxFD_SAVE : wxFD_OPEN);
    bool result = dialog.ShowModal() == wxID_OK;
    if (result)
        this->fileName = dialog.GetPath();    
    return result;
}
