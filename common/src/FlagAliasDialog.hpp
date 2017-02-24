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
     * A modal dialog used to add a visibility or query flag alias.
     * This dialog can be seen within the application by clicking the 'Add' button
     * in the 'Visibility Flags' or 'Query Flags' section of the 'General' page in the 
     * scene settings dialog.
     */
    class FlagAliasDialog : public FinjinDialog
    {
    private:
        DECLARE_EVENT_TABLE()
    
    protected:
        wxStaticText* nameLabel;
        ApplicationTextCtrl* nameText;
        wxStaticText* bitLabel;
        SpinnerControl* bitSpinner;
        wxButton* okButton;
        wxButton* cancelButton;
    
    public:
        /** 
         * Constructor.
         * @param parent [in] - The parent window.
         * @param title [in] - The dialog's title text.
         * @param usedNames [in] - The flag names already in use. This is used to prevent
         * another flag of the same name from being added.
         * @param usedBits [in] - The bits already in use. This is used to prevent another
         * flag with the same bit from being added.
         * @param name [in] - The name of the dialog.
         * @param bit [in] - The default bit.
         */
        FlagAliasDialog
            (
            wxWindow* parent, 
            const wxString& title, 
            const std::set<wxString>& usedNames, 
            const std::set<int>& usedBits, 
            const wxString& name = wxEmptyString, 
            int bit = 0
            );
        ~FlagAliasDialog();        

        void OnOK(wxCommandEvent& event);

        void SetGUIData();

        /** The name of the added flag. */
        wxString name;

        /** The index of the added bit. */
        int bit;

    private:
        std::set<wxString> usedNames;
        std::set<int> usedBits;
    };

} }
