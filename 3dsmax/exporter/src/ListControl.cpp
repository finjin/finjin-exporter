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
#include "ListControl.hpp"
#include "MaxUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
ListControl::ListControl()
{
    this->hList = nullptr;
}

void ListControl::Initialize(HWND hList)
{
    this->hList = hList;

    DWORD style = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT; 
    ListView_SetExtendedListViewStyleEx(this->hList, style, style);
}

void ListControl::Reset()
{
    ListView_DeleteAllItems(this->hList);
}

void ListControl::InsertColumn(int columnStringID, int column, int width)
{
    LVCOLUMN columnData;
    columnData.mask = LVCF_WIDTH | LVCF_TEXT;
    columnData.cx = width;
    columnData.pszText = MaxUtilities::GetString(columnStringID);
    ListView_InsertColumn(this->hList, column, &columnData);
}

void ListControl::InsertItem(int row, int column, const wxString& text, LPARAM data)
{
    LVITEM itemData;
    itemData.mask = LVIF_TEXT;
    itemData.pszText = (TCHAR*)WxStringAsInternal(text);
    itemData.iItem = row;
    itemData.iSubItem = column;
    if (column == 0)
    {
        itemData.mask |= LVIF_PARAM;
        itemData.lParam = data;
        ListView_InsertItem(this->hList, &itemData);
    }
    else
        ListView_SetItem(this->hList, &itemData);
}

int ListControl::AddItem(const wxString& text, LPARAM data)
{
    int itemCount = GetItemCount();
    InsertItem(itemCount, 0, text, data);
    return itemCount;
}

void ListControl::DeleteItem(int row)
{
    ListView_DeleteItem(this->hList, row);
}

int ListControl::GetItemCount()
{
    return ListView_GetItemCount(this->hList);
}

void ListControl::GetItem(int row, int column, wxString& text)
{
    std::vector<wxChar> ttext;
    int maxLength = 128;
    ttext.resize(maxLength + 1);

    LVITEM itemData;
    itemData.mask = LVIF_TEXT;
    itemData.pszText = &ttext[0];
    itemData.cchTextMax = maxLength;
    itemData.iItem = row;
    itemData.iSubItem = column;
    ListView_GetItem(this->hList, &itemData);

    text = &ttext[0];
}

void ListControl::SetItem(int row, int column, const wxString& text)
{
    LVITEM itemData;
    itemData.mask = LVIF_TEXT;
    itemData.pszText = (TCHAR*)WxStringAsInternal(text);
    itemData.iItem = row;
    itemData.iSubItem = column;
    ListView_SetItem(this->hList, &itemData);
}

int ListControl::GetCurSel()
{
    return ListView_GetSelectionMark(this->hList);
}

void ListControl::SetCurSel(int row)
{
    //Deselect the current row
    int currentRow = GetCurSel();
    if (currentRow >= 0)
        ListView_SetItemState(this->hList, currentRow, 0, LVIS_SELECTED);

    //Select the new row
    if (row >= 0)
    {
        ListView_SetSelectionMark(this->hList, row);
        ListView_SetItemState(this->hList, row, LVIS_SELECTED, LVIS_SELECTED);
    }
}

LPARAM ListControl::GetData(int row)
{
    LVITEM itemData;
    itemData.mask = LVIF_PARAM;
    itemData.iItem = row;
    itemData.iSubItem = 0;
    ListView_GetItem(this->hList, &itemData);
    return itemData.lParam;
}

void ListControl::SetData(int row, LPARAM data)
{
    LVITEM itemData;
    itemData.mask = LVIF_PARAM;
    itemData.iItem = row;
    itemData.iSubItem = 0;
    itemData.lParam = data;
    ListView_SetItem(this->hList, &itemData);
}

bool ListControl::IsSelectedItemChange(NMLISTVIEW* listViewNotify)
{
    if (listViewNotify->hdr.code == LVN_ITEMCHANGED)
    {  
        //Has the item's state changed?
        if ((listViewNotify->uChanged & LVIF_STATE) != LVIF_STATE)
        {
            //No, it hasn't
            return false;
        }

        //Has the selected state changed?
        if (!(listViewNotify->uNewState & LVIS_SELECTED) || (listViewNotify->uOldState & LVIS_SELECTED))
        {
            //No, it hasn't
            return false; 
        }

        return true;
    }

    return false;
}
