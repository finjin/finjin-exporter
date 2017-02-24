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
#include "ConvertToFramesDialog.hpp"
#include "MaxUtilities.hpp"
#include "FileUtilities.hpp"
#include "MaxControlUtilities.hpp"
#include "wxAdoptedWindow.hpp"
#include "Strings.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
ConvertToFramesDialog::ConvertToFramesDialog()
{
    this->sizeType = ORIGINAL_SIZE;
    this->width = 256;
    this->height = 256;
}

int ConvertToFramesDialog::DoModal(HWND hWndParent)
{
    return ResourceDialog::DoModal(IDD_CONVERT_TO_FRAMES_DIALOG, hWndParent);
}

INT_PTR ConvertToFramesDialog::DialogProc(UINT message, WPARAM wParam, LPARAM lParam, bool& handled)
{
    switch (message)
    {
        case WM_INITDIALOG:
        {
            CheckRadioButton(IDC_ORIGINAL_SIZE_RADIO, IDC_CUSTOM_SIZE_RADIO, this->sizeType == ORIGINAL_SIZE ? IDC_ORIGINAL_SIZE_RADIO : IDC_CUSTOM_SIZE_RADIO);

            this->widthSpin = SetupIntSpinner(this->hDlg, IDC_WIDTH_SPIN, IDC_WIDTH_EDIT, 1, 4096, this->width);
            this->heightSpin = SetupIntSpinner(this->hDlg, IDC_HEIGHT_SPIN, IDC_HEIGHT_EDIT, 1, 4096, this->height);

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

                    GetDlgItemText(IDC_SOURCE_FILE_EDIT, this->sourceFileName);
                    GetDlgItemText(IDC_DESTINATION_DIRECTORY_EDIT, this->destinationDirectory);
                    GetDlgItemText(IDC_DESTINATION_BASE_FILE_NAME_EDIT, this->destinationBaseFileName);
                    if (IsDlgButtonChecked(IDC_ORIGINAL_SIZE_RADIO))
                        this->sizeType = ORIGINAL_SIZE;
                    else
                        this->sizeType = CUSTOM_SIZE;
                    this->width = this->widthSpin->GetIVal();
                    this->height = this->heightSpin->GetIVal();

                    auto extension = FileUtilities::GetExtension(this->destinationBaseFileName, true);

                    if (sourceFileName.empty())
                    {
                        MaxControlUtilities::ShowMessage(MaxUtilities::GetString(IDS_SOURCE_FILE_NAME_REQUIRED), MaxUtilities::GetString2(IDS_DATA_ENTRY_ERROR), MB_ICONINFORMATION | MB_OK);
                        SetFocus(GetDlgItem(IDC_SOURCE_FILE_EDIT));
                    }
                    else if (destinationBaseFileName.empty())
                    {
                        MaxControlUtilities::ShowMessage(MaxUtilities::GetString(IDS_DESTINATION_BASE_FILE_NAME_REQUIRED), MaxUtilities::GetString2(IDS_DATA_ENTRY_ERROR), MB_ICONINFORMATION | MB_OK);
                        SetFocus(GetDlgItem(IDC_DESTINATION_BASE_FILE_NAME_EDIT));
                    }
                    else if (extension.empty())
                    {
                        MaxControlUtilities::ShowMessage(MaxUtilities::GetString(IDS_DESTINATION_BASE_FILE_NAME_EXTENSION_REQUIRED), MaxUtilities::GetString2(IDS_DATA_ENTRY_ERROR), MB_ICONINFORMATION | MB_OK);
                        SetFocus(GetDlgItem(IDC_DESTINATION_BASE_FILE_NAME_EDIT));
                    }
                    else
                    {                    
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
                case IDC_SOURCE_FILE_BUTTON:
                {
                    BitmapInfo sourceBitmapInfo;
                    if (TheManager->SelectFileInput(&sourceBitmapInfo, this->hDlg))
                        SetDlgItemText(IDC_SOURCE_FILE_EDIT, sourceBitmapInfo.Name());
                    break;
                }
                case IDC_DESTINATION_DIRECTORY_BUTTON:
                {
                    GetDlgItemText(IDC_DESTINATION_DIRECTORY_EDIT, this->destinationDirectory);

                    wxAdoptedWindow window(this->hDlg);
                    wxDirDialog dialog(&window, Strings::CHOOSE_DIRECTORY, wxEmptyString);
                    if (dialog.ShowModal() == wxID_OK)
                    {
                        auto directory = dialog.GetPath();
                        SetDlgItemText(IDC_DESTINATION_DIRECTORY_EDIT, directory.wx_str());
                    }
                    break;
                }
            }

            break;
        }
    }

    return 0;
}
