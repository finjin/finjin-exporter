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
#include "ObjectFilterType.hpp"
#include "FilteredSceneObjects.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** 
     * A dialog that displays files and allows the user to select them
     * This is useful in situations where you don't want the 
     */
    class ChooseFilesDialog : public FinjinDialog
    {
    private:
        DECLARE_EVENT_TABLE()
    
    protected:
        wxStaticText* nameLabel;
        ApplicationTextCtrl* nameTextBox;
        wxListBox* filesListBox;
        wxButton* okButton;
        wxButton* cancelButton;
    
    public:
        struct FileName
        {
            wxString baseName;
            wxString fullPath;
        };

        ChooseFilesDialog
            (
            wxWindow* parent, 
            bool singleSelection = true, 
            const std::vector<FileName>* chooseFiles = nullptr,
            const wxString* chooseDirectory = nullptr,
            const std::vector<wxString>* chooseExtensions = nullptr,
            wxWindowID id = wxID_ANY, 
            const wxString& title = wxEmptyString, 
            const wxPoint& pos = wxDefaultPosition
            );
        ~ChooseFilesDialog();        

        enum
        {
            NAME_TEXT,
            FILES_LIST
        };

        void OnOK(wxCommandEvent& event);
        void OnNameTextChanged(wxCommandEvent& event);
        void OnFilesListDClick(wxCommandEvent& event);
        
        void SetGUIData();

    private:
        void FilterFiles(bool saveSelection = false);
        void HandleOK();

    public:
        std::vector<FileName> chosenFileNames;

    private:
        std::vector<FileName> allFileNames;        
    };

} }
