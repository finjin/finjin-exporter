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


//Includes---------------------------------------------------------------------
#include <wx/combobox.h>
#include "ColorPickerControl.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** A collection of utility functions used to modify window properties. */
    class ApplicationControls
    {    
    public:    
        /**
         * To be called for dialogs and notebooks.
         */
        static void AdjustContainerBackgroundColor(wxWindow* window);

        /**
         * To be called for text boxes, URL controls, combo boxes, list controls, and choices.
         */
        static void AdjustTextColors(wxWindow* window);

        /**
         * To be called for static text controls.
         */
        static void AdjustStaticTextForegroundColor(wxWindow* window);

        /** 
         * To be called for windows that have a title bar and can display an icon. 
         */
        static void SetFinjinIcon(wxTopLevelWindowBase* window);
    };

    /**
     * A text control.
     * This should ALWAYS be used in place of the standard wxTextCtrl control. This is because
     * some applications require specific functionality in order for the text controls
     * to look as they should.
     */
    class ApplicationTextCtrl : public wxTextCtrl
    {
    public:
        static const long EXTRA_TEXT_CTRL_FLAGS;
        static const long DISALLOWED_TEXT_CTRL_FLAGS;

        ApplicationTextCtrl
            (
            wxWindow* parent, 
            wxWindowID id, 
            const wxString& value = wxEmptyString, 
            const wxPoint& pos = wxDefaultPosition, 
            const wxSize& size = wxDefaultSize, 
            long style = 0, 
            const wxValidator& validator = wxDefaultValidator, 
            const wxString& name = wxTextCtrlNameStr
            ) : wxTextCtrl(parent, id, value, pos, size, (style | EXTRA_TEXT_CTRL_FLAGS) & ~DISALLOWED_TEXT_CTRL_FLAGS, validator, name)
        {
            ApplicationControls::AdjustTextColors(this);
        }
    };

    /**
     * A choice (combo box) control.
     * This should ALWAYS be used in place of the standard wxChoice control. This is because
     * some applications require specific functionality in order for the choice controls
     * to look as they should.
     */
    class ApplicationChoiceCtrl : public wxChoice
    {
    public:
        ApplicationChoiceCtrl
            (
            wxWindow* parent, 
            wxWindowID id, 
            const wxPoint& pos, 
            const wxSize& size, 
            const wxArrayString& choices, 
            long style = 0, 
            const wxValidator& validator = wxDefaultValidator, 
            const wxString& name = wxChoiceNameStr
            )
            : wxChoice(parent, id, pos, size, choices, style, validator, name)
        {
            ApplicationControls::AdjustTextColors(this);        
        }

        /*
        There's a bug (???) with wxWidgets 2.9.x that causes the dropdown height to be incorrect.
        Below is my failed attempt to work around the bug
        void Thaw()
        {
            //This overload is necessary since wxWidgets 2.9 doesn't seem to properly calculate the dropdown height
            //if items are added during a Freeze()
            //Note that since Thaw() is no longer virtual all choice controls should be stored as ApplicationChoiceCtrl*, NOT
            //as wxChoice*!!!

            //Thaw as usual
            wxChoice::Thaw();

        #if defined(__WXMSW__)
            //MSWUpdateDropDownHeight();
            //MSWUpdateVisibleHeight();            
        #endif            
        }
        */
    };

    /**
     * A static text control.
     * This should ALWAYS be used in place of the standard wxStaticText control. This is because
     * some applications require specific functionality in order for the static text controls
     * to look as they should.
     */
    class ApplicationStaticTextCtrl : public wxStaticText
    {
    public:
        ApplicationStaticTextCtrl
            (
            wxWindow* parent, 
            wxWindowID id, 
            const wxString& label,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = 0, 
            const wxString& name = wxStaticTextNameStr
            ) : wxStaticText(parent, id, label, pos, size, style, name)
        {
            ApplicationControls::AdjustStaticTextForegroundColor(this);
        }
    };

    /**
     * A list control.
     * This should ALWAYS be used in place of the standard wxListCtrl control. This is because
     * some applications require specific functionality in order for the list controls
     * to look as they should.
     */
    class ApplicationListCtrl : public wxListCtrl
    {
    public:
        ApplicationListCtrl
            (
            wxWindow *parent,
            wxWindowID id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxLC_ICON,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxListCtrlNameStr
            ) : wxListCtrl(parent, id, pos, size, style, validator, name)
        {
            ApplicationControls::AdjustTextColors(this);    
        }

        bool GetSelection(std::vector<int>& selection)
        {
            selection.clear();

            int selectedItemCount = GetSelectedItemCount();
            if (selectedItemCount > 0)
            {
                selection.reserve(selectedItemCount);

                int selectedIndex = -1;
                for (int i = 0; i < selectedItemCount; i++)
                {
                    selectedIndex = (int)GetNextItem(selectedIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
                    selection.push_back(selectedIndex);                    
                }
            }

            return !selection.empty();
        }

        int GetSelection() const
        {
            return (int)GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        }

        void SetSelection(int index)
        {
            if (index >= 0 && index < GetItemCount())
            {
                if (GetWindowStyleFlag() & wxLC_SINGLE_SEL)
                {
                    //Single selection. Clear item state
                    SetItemState(index, 0, wxLIST_STATE_SELECTED|wxLIST_STATE_FOCUSED);
                }
                else
                {
                    //Multi-selection list. Clear all other item states
                    for (int i = 0; i < GetItemCount(); i++)
                        SetItemState(i, 0, wxLIST_STATE_SELECTED|wxLIST_STATE_FOCUSED);
                }
                
                //Select
                SetItemState(index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
            }
        }

        void SetSequentialItemData()
        {
            for (int i = 0; i < GetItemCount(); i++)
                SetItemData(i, i);
        }
    };

    /**
     * A list box control.
     * This should ALWAYS be used in place of the standard wxListBox control. This is because
     * some applications require specific functionality in order for the list box controls
     * to look as they should.
     */
    class ApplicationListBox : public wxListBox
    {
    public:
        ApplicationListBox
            (
            wxWindow* parent, 
            wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            int n = 0, 
            const wxString choices[] = nullptr,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxListBoxNameStr
            ) : wxListBox(parent, id, pos, size, n, choices, style, validator, name)
        {
            ApplicationControls::AdjustTextColors(this);
        }

        ApplicationListBox
            (
            wxWindow *parent, 
            wxWindowID id,
            const wxPoint& pos,
            const wxSize& size,
            const wxArrayString& choices,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxListBoxNameStr) : wxListBox(parent, id, pos, size, choices, style, validator, name)
        {
            ApplicationControls::AdjustTextColors(this);
        }
    };

} }
