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


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * Manages access to a Win32 list control.
     * This class does not create the control. The control will typically be
     * created automatically as part of the creation of a dialog window from
     * a resource.
     */
    class ListControl
    {
    public:
        ListControl();

        /**
         * Attaches to the specifed list control.
         * @param hList [in] - The list control. This control is modified
         * to have the LVS_EX_GRIDLINES and LVS_EX_FULLROWSELECT properties.
         */
        void Initialize(HWND hList);

        /** Removes all the items from the list control. */
        void Reset();

        /**
         * Inserts a column into the list control.
         * @param columnStringID [in] - Resource identifier of the string used
         * as the column's label.
         * @param column [in] - Index of the column to insert. This index should exist,
         * which means that during initialization columns should be created
         * sequentially: 0, 1, and so on.
         * @param width [in] - The width of the column.
         */
        void InsertColumn(int columnStringID, int column, int width = 50);

        /**
         * Inserts a new item into the specified row, possibly creating a new row.
         * @param row [in] - The index of the row to modify/add.
         * @param column [in] - The index of the column to insert. If this is 0,
         * a new row is inserted.
         * @param text [in] - The text to set/add.
         * @param data [in] - Data to associate with the item. This is ignored if
         * column isn't 0.
         */
        void InsertItem(int row, int column, const wxString& text = wxEmptyString, LPARAM data = 0);

        /**
         * Adds a new row.
         * @param text [in] - The text to set into the first column.
         * @param data [in] - Data to associate with the item.
         * @return Index of the added row.
         */
        int AddItem(const wxString& text = wxEmptyString, LPARAM data = 0);

        /** Deletes the specified row. */
        void DeleteItem(int row);

        /** Gets the number of rows in the control. */
        int GetItemCount();

        /** Gets the text for the specified item. */
        void GetItem(int row, int column, wxString& text);

        /** Sets the text for the specified item. */
        void SetItem(int row, int column, const wxString& text);

        /** Gets the currently selected row. */
        int GetCurSel();

        /** Sets the currently selected row. */
        void SetCurSel(int row);

        /** Gets the data associated with the specified row. */
        LPARAM GetData(int row);

        /** Sets the data associated with the specified row. */
        void SetData(int row, LPARAM data);

        /** Determines whether the notification data indicates that the item selection changed. */
        static bool IsSelectedItemChange(NMLISTVIEW* listViewNotify);

    protected:
        /** The list control. */
        HWND hList;
    };

} }
