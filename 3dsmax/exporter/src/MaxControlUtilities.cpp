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
#include "MaxControlUtilities.hpp"
#include "MaxUtilities.hpp"
#include "StringUtilities.hpp"
#include "resource.h"

using namespace Finjin::Exporter;


//Local functions---------------------------------------------------------------
static int GetValueIndex(const wxString& value, int referenceValueCount, const TCHAR* referenceValues[])
{
    for (int i = 0; i < referenceValueCount; i++)
    {
        if (value == referenceValues[i])
            return i;
    }

    return -1;
}


//Implementation----------------------------------------------------------------
int MaxControlUtilities::ShowMessage(const wxString& message, const wxString& title, UINT type)
{
    return MessageBox(GetCOREInterface()->GetMAXHWnd(), message.wx_str(), title.wx_str(), type);
}

int MaxControlUtilities::ShowMessage(int messageID, int titleID, UINT type)
{
    return MessageBox
        (
        GetCOREInterface()->GetMAXHWnd(),
        MaxUtilities::GetString(messageID),
        MaxUtilities::GetString2(titleID),
        type
        );
}

int MaxControlUtilities::MeasureTextWidth(HDC hdc, const TCHAR* text, bool logicalPixels)
{
    SIZE textSize = MeasureTextSize(hdc, text, logicalPixels);
    return (int)textSize.cx;
}

SIZE MaxControlUtilities::MeasureTextSize(HDC hdc, const TCHAR* text, bool logicalPixels)
{
    SIZE textSize;
    GetTextExtentPoint32(hdc, text, (int)_tcslen(text), &textSize);
    if (logicalPixels)
    {
        textSize.cx = MulDiv(textSize.cx, 72, GetDeviceCaps(hdc, LOGPIXELSX));
        textSize.cy = MulDiv(textSize.cy, 72, GetDeviceCaps(hdc, LOGPIXELSY));
    }
    return textSize;
}

void MaxControlUtilities::GetText(ICustEdit* edit, wxString& text)
{
    std::vector<MCHAR> ttext;

    //Keep getting the text
    int length = 64;
    do
    {
        //Increase maximum length
        length *= 2;
        ttext.resize(length);

        //Set the second to last character to the null terminator
        //If this is non-null after getting the text, we'll assume that more
        //space is needed to get the entire string
        ttext[length - 2] = 0;

        //Get the text
        edit->GetText(&ttext[0], length - 1);
    }while(ttext[length - 2] != 0);

    text = &ttext[0];
}

void MaxControlUtilities::GetWindowText(HWND hwnd, wxString& text)
{
    int length = ::GetWindowTextLength(hwnd);
    std::vector<wxChar> ttext;
    ttext.resize(length + 1);
    ::GetWindowText(hwnd, &ttext[0], length + 1);
    text = &ttext[0];
}

void MaxControlUtilities::GetSelectedComboText(HWND hWndCombo, wxString& text)
{
    int curSel = ComboBox_GetCurSel(hWndCombo);
    int length = ComboBox_GetLBTextLen(hWndCombo, curSel);
    std::vector<wxChar> ttext;
    ttext.resize(length + 1);
    ComboBox_GetLBText(hWndCombo, curSel, &ttext[0]);
    text = &ttext[0];
}

void MaxControlUtilities::SetComboHeight(HWND hWndCombo, int height)
{
    RECT rect;
    GetWindowRect(hWndCombo, &rect);
    rect.bottom = rect.top + height;
    MoveWindow(hWndCombo, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
}

void MaxControlUtilities::FillComboBox(HWND hWnd, const TCHAR* items[], int itemCount, bool adjustWindowWidth)
{
    int comboWidth = 0;

    //Get device context
    HDC hDCCombo = GetDC(hWnd);

    //For all the text items...
    for (int i = 0; i < itemCount; i++)
    {
        //Add text
        ComboBox_AddString(hWnd, items[i]);

        //Measure the text width, keeping the current or new width, whichever is larger
        comboWidth = std::max(comboWidth, MeasureTextWidth(hDCCombo, items[i]));
    }

    //Device context not needed any more
    ReleaseDC(hWnd, hDCCombo);

    //Adjust dropdown width
    SendMessage(hWnd, CB_SETDROPPEDWIDTH, comboWidth + 5, 0);

    //Select first item
    ComboBox_SetCurSel(hWnd, 0);

    //Adjust the window's width if necessary
    if (adjustWindowWidth)
    {
        RECT rect;

        GetWindowRect(hWnd, &rect);
        ScreenToClient(GetParent(hWnd), (POINT*)&rect.left);
        ScreenToClient(GetParent(hWnd), (POINT*)&rect.right);
        rect.right = rect.left + comboWidth + 20;

        MoveWindow(hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
    }
}

void MaxControlUtilities::SelectComboBoxItem(HWND hWnd, const TCHAR* items[], int itemCount, wxString value, wxString defaultValue)
{
    auto result = GetValueIndex(value, itemCount, items);
    if (result == -1)
        result = GetValueIndex(defaultValue, itemCount, items);
    if (result == -1)
        result = 0;

    ComboBox_SetCurSel(hWnd, result);
}
