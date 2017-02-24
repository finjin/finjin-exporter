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
#include "RenderTerrainMapDialog.hpp"

#if defined(FINJIN_EXPORTER_ENABLE_D3D_FUNCTIONALITY)

#include "MaxUtilities.hpp"
#include "FileUtilities.hpp"
#include "MaxControlUtilities.hpp"
#include "Strings.hpp"
#include "wxAdoptedWindow.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
RenderTerrainMapDialog::Settings RenderTerrainMapDialog::lastTerrainMapSettings(512, 512);
RenderTerrainMapDialog::Settings RenderTerrainMapDialog::lastHeightMapSettings(513, 513);


//Implementation---------------------------------------------------------------
RenderTerrainMapDialog::RenderTerrainMapDialog(Mode mode)
{
    this->mode = mode;
    this->useSelectedObjects = false;

    if (this->mode == TerrainMapRenderer::COLOR_MAP)
    {
        this->lastActiveSettings = &lastTerrainMapSettings;
        this->useSelectedObjectsStringID = IDS_TERRAIN_MAP_SELECTED_OBJECTS;
        this->useAllObjectsStringID = IDS_TERRAIN_MAP_ALL_OBJECTS;
        this->titleStringID = IDS_RENDER_TERRAIN_MAP_TITLE;
    }
    else
    {
        this->lastActiveSettings = &lastHeightMapSettings;
        this->useSelectedObjectsStringID = IDS_HEIGHT_MAP_SELECTED_OBJECTS;
        this->useAllObjectsStringID = IDS_HEIGHT_MAP_ALL_OBJECTS;
        this->titleStringID = IDS_RENDER_HEIGHT_MAP_TITLE;
    }

    this->width = this->lastActiveSettings->width;
    this->height = this->lastActiveSettings->height;    
    this->destinationFileName = this->lastActiveSettings->destinationFileName;
    this->bytesPerPixel = this->lastActiveSettings->bytesPerPixel;
    this->isSquare = this->lastActiveSettings->isSquare;
    this->rotate90DegreesClockwise = this->lastActiveSettings->rotate90DegreesClockwise;
}

int RenderTerrainMapDialog::DoModal(HWND hWndParent)
{
    return ResourceDialog::DoModal(IDD_RENDER_TERRAIN_MAP_DIALOG, hWndParent);
}

INT_PTR RenderTerrainMapDialog::DialogProc(UINT message, WPARAM wParam, LPARAM lParam, bool& handled)
{
    switch (message)
    {
        case WM_INITDIALOG:
        {
            wxString infoText;
            if (this->useSelectedObjects)
                infoText = MaxUtilities::GetString(this->useSelectedObjectsStringID);
            else
                infoText = MaxUtilities::GetString(this->useAllObjectsStringID);
            SetDlgItemText(IDC_SELECTED_OBJECTS_INFORMATION_TEXT, infoText);

            SetTitle(MaxUtilities::GetString(this->titleStringID));

            if (this->isSquare)
                this->height = this->width;

            this->widthSpin = SetupIntSpinner(this->hDlg, IDC_WIDTH_SPIN, IDC_WIDTH_EDIT, 1, 4096, this->width);
            this->heightSpin = SetupIntSpinner(this->hDlg, IDC_HEIGHT_SPIN, IDC_HEIGHT_EDIT, 1, 4096, this->height);
            this->bytesPerPixelSpin = SetupIntSpinner(this->hDlg, IDC_BYTES_PER_PIXEL_SPIN, IDC_BYTES_PER_PIXEL_EDIT, 1, 2, this->bytesPerPixel);
            
            CheckDlgButton(IDC_SQUARE_CHECK, this->isSquare);
            CheckDlgButton(IDC_ROTATE_90_DEGREES_COUNTERCLOCKWISE, this->rotate90DegreesClockwise);

            SetDlgItemText(IDC_DESTINATION_FILE_EDIT, this->destinationFileName);

            //Hide bytes per pixel controls
            if (this->mode == TerrainMapRenderer::COLOR_MAP)
            {
                ShowWindow(GetDlgItem(IDC_BYTES_PER_PIXEL_TEXT), SW_HIDE);
                ShowWindow(GetDlgItem(IDC_BYTES_PER_PIXEL_EDIT), SW_HIDE);
                ShowWindow(GetDlgItem(IDC_BYTES_PER_PIXEL_SPIN), SW_HIDE);
            }

            //Center in parent window
            CenterWindow();

            handled = true;

            break;
        }
        case WM_DESTROY:
        {
            handled = true;

            ReleaseISpinner(this->widthSpin);
            ReleaseISpinner(this->heightSpin);
            ReleaseISpinner(this->bytesPerPixelSpin);            

            break;
        }
        case CC_SPINNER_CHANGE:
        case CC_SPINNER_BUTTONDOWN:
        case WM_CUSTEDIT_ENTER:
        case CC_SPINNER_BUTTONUP:
        {
            if (this->isSquare)
                this->heightSpin->SetValue(this->widthSpin->GetIVal(), FALSE);
            break;
        }
        case WM_COMMAND:
        {
            int id = LOWORD(wParam);
            int code = HIWORD(wParam);

            switch (id)
            {
                case IDOK:
                {
                    handled = true;
                    
                    //Get values
                    GetDlgItemText(IDC_DESTINATION_FILE_EDIT, this->destinationFileName);
                    this->width = this->widthSpin->GetIVal();
                    this->height = this->heightSpin->GetIVal();
                    this->bytesPerPixel = this->bytesPerPixelSpin->GetIVal();
                    
                    this->isSquare = IsDlgButtonChecked(IDC_SQUARE_CHECK);
                    if (this->isSquare)
                        this->height = this->width;

                    this->rotate90DegreesClockwise = IsDlgButtonChecked(IDC_ROTATE_90_DEGREES_COUNTERCLOCKWISE) ? true : false;
                    
                    bool success = true;

                    if (this->destinationFileName.empty())
                    {
                        MaxControlUtilities::ShowMessage(MaxUtilities::GetString(IDS_DESTINATION_FILE_REQUIRED), MaxUtilities::GetString2(IDS_DATA_ENTRY_ERROR), MB_ICONINFORMATION | MB_OK);
                        SetFocus(GetDlgItem(IDC_DESTINATION_FILE_EDIT));
                        success = false;
                    }
                    
                    if (success)
                    {                    
                        //Store values for the next launch of the dialog
                        this->lastActiveSettings->width = this->width;
                        this->lastActiveSettings->height = this->height;    
                        this->lastActiveSettings->destinationFileName = this->destinationFileName;
                        this->lastActiveSettings->bytesPerPixel = this->bytesPerPixel;
                        this->lastActiveSettings->isSquare = this->isSquare;
                        this->lastActiveSettings->rotate90DegreesClockwise = this->rotate90DegreesClockwise;

                        EndDialog(this->hDlg, TRUE);
                        return TRUE;
                    }
                    break;
                }
                case IDCANCEL:
                {
                    handled = true;
                    EndDialog(this->hDlg, FALSE);
                    return FALSE;
                }
                case IDC_SQUARE_CHECK:
                {
                    handled = true;
                    this->isSquare = IsDlgButtonChecked(IDC_SQUARE_CHECK);
                    if (this->isSquare)
                        this->heightSpin->SetValue(this->widthSpin->GetIVal(), FALSE);

                    break;
                }
                case IDC_DESTINATION_FILE_BUTTON:
                {
                    handled = true;

                    GetDlgItemText(IDC_DESTINATION_FILE_EDIT, this->destinationFileName);

                    if (this->mode == TerrainMapRenderer::COLOR_MAP)
                    {
                        //Any file type is allowed

                        BitmapInfo bitmapInfo;
                        bitmapInfo.SetName(this->destinationFileName);
                        if (TheManager->SelectFileOutput(&bitmapInfo, this->hDlg))
                            SetDlgItemText(IDC_DESTINATION_FILE_EDIT, bitmapInfo.Name());
                    }
                    else
                    {   
                        //Only DDS files are allowed because the 3DS Max bitmap functions 
                        //seem to be incapable of saving a 8-bit or 16-bit per channel bitmap

                        wxAdoptedWindow window(this->hDlg);
                        wxFileDialog dialog(&window, Strings::CHOOSE_FILE, wxEmptyString, wxEmptyString, Strings::DDS_FILE_FILTER, wxFD_SAVE);
                        if (dialog.ShowModal() == wxID_OK)
                        {
                            wxString file = dialog.GetPath();
                            FileUtilities::EnsureExtension(file, _T(".dds"));
                            SetDlgItemText(IDC_DESTINATION_FILE_EDIT, file);
                        }
                    }
                        
                    break;
                }
            }

            break;
        }
    }

    return 0;
}

#endif
