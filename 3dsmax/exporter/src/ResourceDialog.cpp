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
#include "ResourceDialog.hpp"
#include "MaxControlUtilities.hpp"
#include "ApplicationAccessor.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
ResourceDialog::ResourceDialog()
{
    this->hDlg = nullptr;
    this->oldUserData = 0;
    this->oldWndProc = nullptr;    
}

ResourceDialog::~ResourceDialog()
{
}

INT_PTR ResourceDialog::DoModal(int dialogID, HWND hWndParent)
{
    if (hWndParent == nullptr)
        hWndParent = (HWND)ApplicationAccessor::GetMainWindow();

    return DialogBoxParam
        (
        wxGetInstance(), 
        MAKEINTRESOURCE(dialogID), 
        hWndParent,
        DialogProcCallback,
        (LPARAM)this
        );
}

void ResourceDialog::Attach(HWND hDlg)
{
    this->hDlg = hDlg;

    //Get old window user data and procedure
    this->oldUserData = GetWindowLongPtr(this->hDlg, GWLP_USERDATA);
    this->oldWndProc = (WNDPROC)GetWindowLongPtr(this->hDlg, GWLP_WNDPROC);

    //Replace user data and procedure
    SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)this);
    SetWindowLongPtr(hDlg, GWLP_WNDPROC, (LONG_PTR)DialogProcCallback);    
}

bool ResourceDialog::GetGUIData()
{
    return true;
}

void ResourceDialog::GetTitle(wxString& text)
{
    MaxControlUtilities::GetWindowText(this->hDlg, text);
}

void ResourceDialog::SetTitle(const wxString& text)
{
    ::SetWindowText(this->hDlg, text.wx_str());
}

INT_PTR CALLBACK ResourceDialog::DialogProcCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    ResourceDialog* dialog;

    if (message == WM_INITDIALOG)
    {
        dialog = (ResourceDialog*)lParam;
        dialog->hDlg = hDlg;
        SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)lParam);

        SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(wxGetInstance(), _T("IDI_FINJIN")));
    }
    else
    {
        dialog = (ResourceDialog*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
        if (dialog == nullptr)
            return FALSE;
    }

    bool handled = false;
    INT_PTR result = dialog->DialogProc(message, wParam, lParam, handled);

    if (!handled && dialog->oldWndProc != nullptr)
    {
        //Restore old window user data and procedure
        SetWindowLongPtr(dialog->hDlg, GWLP_USERDATA, (LONG_PTR)dialog->oldUserData);
        SetWindowLongPtr(dialog->hDlg, GWLP_WNDPROC, (LONG_PTR)dialog->oldWndProc);    
 
        //Call old window procedure
        result = dialog->oldWndProc(dialog->hDlg, message, wParam, lParam);
        
        //Replace user data and procedure
        SetWindowLongPtr(dialog->hDlg, GWLP_USERDATA, (LONG_PTR)dialog);
        SetWindowLongPtr(dialog->hDlg, GWLP_WNDPROC, (LONG_PTR)DialogProcCallback);    
    }

    return result;
}

void ResourceDialog::CheckRadioButton(int idFirstButton, int idLastButton, int idCheckButton)
{
    ::CheckRadioButton(this->hDlg, idFirstButton, idLastButton, idCheckButton);
}

HWND ResourceDialog::GetDlgItem(int idDlgItem)
{
    return ::GetDlgItem(this->hDlg, idDlgItem);
}

void ResourceDialog::CheckDlgButton(int idButton, UINT check)
{
    ::CheckDlgButton(this->hDlg, idButton, check);
}

bool ResourceDialog::IsDlgButtonChecked(int idButton)
{
    return ::IsDlgButtonChecked(this->hDlg, idButton) ? true : false;
}

wxString ResourceDialog::GetDlgItemText(int idControl)
{
    wxString text;
    GetDlgItemText(idControl, text);
    return text;
}

void ResourceDialog::GetDlgItemText(int idControl, wxString& text)
{
    MaxControlUtilities::GetWindowText(GetDlgItem(idControl), text);    
}

void ResourceDialog::SetDlgItemText(int idControl, const wxString& text)
{
    ::SetDlgItemText(this->hDlg, idControl, text.wx_str());
}

void ResourceDialog::CenterWindow()
{
    ::CenterWindow(this->hDlg, GetParent(this->hDlg));
}

void ResourceDialog::End(int result)
{
    EndDialog(this->hDlg, result);
}
