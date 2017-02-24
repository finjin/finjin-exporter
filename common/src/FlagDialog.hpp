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
#include "FinjinDialog.hpp"
#include "SpinnerControl.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** 
     * A modal dialog used to add a visibility or query flag.
     * This dialog can be seen within the application by clicking the 'Add' button
     * in the 'Visibility Flags' or 'Query Flags' section of the 'General' page in the 
     * object settings dialog.
     */
    class FlagDialog : public FinjinDialog
    {
    private:
        DECLARE_EVENT_TABLE()
    
    protected:
        wxStaticText* nameText;
        ApplicationChoiceCtrl* nameChoice;
        wxButton* okButton;
        wxButton* cancelButton;
    
    public:
        /** 
         * Constructor.
         * @param parent [in] - The parent window.
         * @param title [in] - The dialog's title text.
         * @param flagNames [in] - The flag names configured at the scene level.
         * @param flagBits [in] - The bits configured at the scene level. Each bit corresponds to a flag name.
         */
        FlagDialog
            (
            wxWindow* parent, 
            const wxString& title, 
            const std::vector<wxString>& flagNames, 
            const std::vector<int>& flagBits
            );
        ~FlagDialog();

        void SetGUIData();

        void OnOK(wxCommandEvent& event);

    private:
        wxString GetFlagNameFromBit(int bit);

    public:
        /** The name of the selected flag. */
        wxString name;
        
        /** The index of the selected bit. */
        int bit;

    private:
        std::vector<wxString> flagNames;
        std::vector<int> flagBits;
    };

} }
