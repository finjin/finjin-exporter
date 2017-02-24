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

    /** Various utility functions for dealing with controls. */
    class MaxControlUtilities
    {
    public:
        /** 
         * Displays a message box with the specified message.
         * @param message [in] - The message.
         * @param title [in] - The text shown in the title bar of the displayed message box.
         * @param type [in] - Message box flags.
         * @return The message box result is returned.
         */
        static int ShowMessage(const wxString& message, const wxString& title = _T("Message"), UINT type = MB_OK);

        /** 
         * Displays a message box with the specified message.
         * @param messageID [in] - Resource identifier of the message string.
         * @param title [in] - The text shown in the title bar of the displayed message box.
         * @param type [in] - Message box flags.
         * @return The message box result is returned.
         */
        static int ShowMessage(int messageID, int titleID, UINT type = MB_OK);

        /** 
         * Measuress the width of the specified text.
         * @param hdc [in] - The device context being used to draw the text.
         * @param text [in] - The text being measured.
         * @param logicalPixels [in] - Indicates whether the width being returned should
         * be calculated in logical pixels. This will usually be true.
         * @return The width of the specified text is returned.
         */
        static int MeasureTextWidth(HDC hdc, const TCHAR* text, bool logicalPixels = true);

        /** 
         * Measuress the size of the specified text.
         * @param hdc [in] - The device context being used to draw the text.
         * @param text [in] - The text being measured.
         * @param logicalPixels [in] - Indicates whether the size being returned should
         * be calculated in logical pixels. This will usually be true.
         * @return The width and height of the specified text is returned.
         */
        static SIZE MeasureTextSize(HDC hdc, const TCHAR* text, bool logicalPixels = true);

        /** 
         * Gets the text from a 3DS Max custom edit control.
         * @param edit [in] - The 3DS Max custom edit control.
         * @param text [out] - The retrieved text.
         */
        static void GetText(ICustEdit* edit, wxString& text);

        /** 
         * Gets the text from a Windows edit control.
         * @param edit [in] - The edit control window handle.
         * @param text [out] - The retrieved text.
         */
        static void GetWindowText(HWND hwnd, wxString& text);

        /** Gets the text of the selected combo box item. */
        static void GetSelectedComboText(HWND hWndCombo, wxString& text);

        /** Sets the height of the combo box dropdown. */
        static void SetComboHeight(HWND hWndCombo, int height);

        /** 
         * Fills the combo box with the specified items.
         * @param hWnd [in] - The combo box window handle.
         * @param items [in] - Array of strings to put into the combo box.
         * @param itemCount [in] - Number of strings to put into the combo box.
         * @param adjustWindowWidth [in] - Indicates whether the combo box width is adjusted to 
         * match the width of the largest string being added.
         */
        static void FillComboBox(HWND hWnd, const TCHAR* items[], int itemCount, bool adjustWindowWidth = false);

        /** 
         * Selects the appropriate combo box item. 
         * The selection is done with an array of strings that is assumed to have
         * the same number of items in the combo box.
         * @param hWnd [in] - The combo box window handle.
         * @param items [in] - Array of string values. These strings don't have to be
         * the same strings that were used to fill the combo box, but it is expected that
         * the number of strings matches the number of items in the combo box.
         * @param itemCount [in] - Number of string values.
         * @param value [in] - The value to select.
         * @param defaultValue [in] - The value to use if the main value could not be found.
         */
        static void SelectComboBoxItem(HWND hWnd, const TCHAR* items[], int itemCount, wxString value, wxString defaultValue = _T(""));
    };

} }
