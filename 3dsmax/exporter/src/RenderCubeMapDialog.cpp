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
#include "RenderCubeMapDialog.hpp"
#include "MaxUtilities.hpp"
#include "FileUtilities.hpp"
#include "MaxControlUtilities.hpp"
#include "Strings.hpp"
#include "wxAdoptedWindow.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
int RenderCubeMapDialog::lastDimensions = 512;        
RenderCubeMapDialog::Type RenderCubeMapDialog::lastType = RenderCubeMapDialog::SEPARATE_FILES;
wxString RenderCubeMapDialog::lastDestinationDirectory;
wxString RenderCubeMapDialog::lastDestinationBaseFileName;
wxString RenderCubeMapDialog::lastDestinationFileName;


//Implementation---------------------------------------------------------------
RenderCubeMapDialog::RenderCubeMapDialog()
{
    this->type = lastType;
    this->dimensions = lastDimensions;
    this->centerNode = nullptr;
    this->destinationDirectory = lastDestinationDirectory;
    this->destinationBaseFileName = lastDestinationBaseFileName;
    this->destinationFileName = lastDestinationFileName;
}

int RenderCubeMapDialog::DoModal(HWND hWndParent)
{
    return ResourceDialog::DoModal(IDD_RENDER_CUBE_MAP_DIALOG, hWndParent);
}

INT_PTR RenderCubeMapDialog::DialogProc(UINT message, WPARAM wParam, LPARAM lParam, bool& handled)
{
    switch (message)
    {
        case WM_INITDIALOG:
        {
            wxString infoText;
            if (this->centerNode != nullptr)
                infoText = MaxUtilities::GetString(IDS_CUBE_MAP_CENTERED_AROUND_SELECTED_OBJECT);
            else
                infoText = MaxUtilities::GetString(IDS_CUBE_MAP_CENTERED_AROUND_ORIGIN);
            SetDlgItemText(IDC_CENTER_INFORMATION_TEXT, infoText);

            CheckRadioButton(IDC_SEPARATE_FILES_RADIO, IDC_DDS_FILE_RADIO, this->type == SEPARATE_FILES ? IDC_SEPARATE_FILES_RADIO : IDC_DDS_FILE_RADIO);

            this->dimensionSpin = SetupIntSpinner(this->hDlg, IDC_DIMENSIONS_SPIN, IDC_DIMENSIONS_EDIT, 1, 4096, this->dimensions);

            SetDlgItemText(IDC_DESTINATION_DIRECTORY_EDIT, this->destinationDirectory);
            SetDlgItemText(IDC_DESTINATION_BASE_FILE_NAME_EDIT, this->destinationBaseFileName);
            SetDlgItemText(IDC_DESTINATION_FILE_EDIT, this->destinationFileName);

            //Center in parent window
            CenterWindow();

            handled = true;

            break;
        }
        case WM_DESTROY:
        {
            handled = true;

            ReleaseISpinner(this->dimensionSpin);

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
                    
                    GetDlgItemText(IDC_DESTINATION_DIRECTORY_EDIT, this->destinationDirectory);
                    GetDlgItemText(IDC_DESTINATION_BASE_FILE_NAME_EDIT, this->destinationBaseFileName);
                    GetDlgItemText(IDC_DESTINATION_FILE_EDIT, this->destinationFileName);
                    this->dimensions = this->dimensionSpin->GetIVal();

                    if (IsDlgButtonChecked(IDC_SEPARATE_FILES_RADIO))
                        this->type = SEPARATE_FILES;
                    else
                        this->type = DDS_FILE;

                    bool success = true;

                    if (this->type == SEPARATE_FILES)
                    {
                        auto extension = FileUtilities::GetExtension(this->destinationBaseFileName, true);

                        if (this->destinationDirectory.empty())
                        {
                            MaxControlUtilities::ShowMessage(MaxUtilities::GetString(IDS_DESTINATION_DIRECTORY_REQUIRED), MaxUtilities::GetString2(IDS_DATA_ENTRY_ERROR), MB_ICONINFORMATION | MB_OK);
                            SetFocus(GetDlgItem(IDC_DESTINATION_DIRECTORY_EDIT));
                            success = false;
                        }
                        else if (destinationBaseFileName.empty())
                        {
                            MaxControlUtilities::ShowMessage(MaxUtilities::GetString(IDS_DESTINATION_BASE_FILE_NAME_REQUIRED), MaxUtilities::GetString2(IDS_DATA_ENTRY_ERROR), MB_ICONINFORMATION | MB_OK);
                            SetFocus(GetDlgItem(IDC_DESTINATION_BASE_FILE_NAME_EDIT));
                            success = false;
                        }
                        else if (extension.empty())
                        {
                            MaxControlUtilities::ShowMessage(MaxUtilities::GetString(IDS_DESTINATION_BASE_FILE_NAME_EXTENSION_REQUIRED), MaxUtilities::GetString2(IDS_DATA_ENTRY_ERROR), MB_ICONINFORMATION | MB_OK);
                            SetFocus(GetDlgItem(IDC_DESTINATION_BASE_FILE_NAME_EDIT));
                            success = false;
                        }
                    }
                    else
                    {
                        if (this->destinationFileName.empty())
                        {
                            MaxControlUtilities::ShowMessage(MaxUtilities::GetString(IDS_DESTINATION_FILE_REQUIRED), MaxUtilities::GetString2(IDS_DATA_ENTRY_ERROR), MB_ICONINFORMATION | MB_OK);
                            SetFocus(GetDlgItem(IDC_DESTINATION_FILE_EDIT));
                            success = false;
                        }
                    }                    
                    
                    if (success)
                    {                    
                        lastType = this->type;
                        lastDimensions = this->dimensions;
                        lastDestinationDirectory = this->destinationDirectory;
                        lastDestinationBaseFileName = this->destinationBaseFileName;
                        lastDestinationFileName = this->destinationFileName;

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
                case IDC_DESTINATION_DIRECTORY_BUTTON:
                {
                    GetDlgItemText(IDC_DESTINATION_DIRECTORY_EDIT, this->destinationDirectory);

                    wxAdoptedWindow window(this->hDlg);
                    wxDirDialog dialog(&window, Strings::CHOOSE_DIRECTORY, this->destinationDirectory);
                    if (dialog.ShowModal() == wxID_OK)
                    {
                        auto directory = dialog.GetPath();
                        SetDlgItemText(IDC_DESTINATION_DIRECTORY_EDIT, directory.wx_str());
                    }
                    
                    break;
                }
                case IDC_DESTINATION_FILE_BUTTON:
                {
                    wxAdoptedWindow window(this->hDlg);
                    wxFileDialog dialog(&window, Strings::CHOOSE_FILE, wxEmptyString, wxEmptyString, Strings::DDS_FILE_FILTER, wxFD_SAVE);
                    if (dialog.ShowModal() == wxID_OK)
                    {
                        wxString file = dialog.GetPath();
                        FileUtilities::EnsureExtension(file, _T(".dds"));
                        SetDlgItemText(IDC_DESTINATION_FILE_EDIT, file);
                    }
                        
                    break;
                }
            }

            break;
        }
    }

    return 0;
}
