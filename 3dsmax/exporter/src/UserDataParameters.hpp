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


//Includes----------------------------------------------------------------------
#include "UserDataSettingsAccessor.hpp"


//Macros------------------------------------------------------------------------

/** This macro should NEVER be modified. If more parameters are needed, create a new macro. */
#define USER_DATA_PARAM_BLOCK_IDS PB_ID, PB_USER_DATA_REFERENCE, PB_USER_DATA, PB_USER_DATA_CLASS

#define USER_DATA_PARAMETER_BLOCK_PARAMETERS(className)\
    className::PB_ID, _T("ID"), TYPE_STRING, 0, IDS_ID,\
        p_end,\
    className::PB_USER_DATA, _T("UserData"), TYPE_STRING, 0, IDS_USER_DATA,\
        p_end,\
    className::PB_USER_DATA_CLASS, _T("UserDataClass"), TYPE_STRING, 0, IDS_USER_DATA_CLASS,\
        p_end

#define DECLARE_USER_DATA_OBJECT_METHODS FINJIN_EXPORTER_DECLARE_USER_DATA_ACCESSOR_METHODS

#define FINJIN_EXPORTER_IMPLEMENT_USER_DATA_OBJECT_METHODS(className)\
    wxString className::GetUserID() {return this->pblock->GetStr(PB_ID);}\
    wxString className::GetUserData() {return this->pblock->GetStr(PB_USER_DATA);}\
    wxString className::GetUserDataClass() {return this->pblock->GetStr(PB_USER_DATA_CLASS);}\
    \
    void className::SetUserID(wxString value) {this->pblock->SetValue(PB_ID, 0, WxStringToApplicationStringM(value));}\
    void className::SetUserData(wxString value) {this->pblock->SetValue(PB_USER_DATA, 0, WxStringToApplicationStringM(value));}\
    void className::SetUserDataClass(wxString value) {this->pblock->SetValue(PB_USER_DATA_CLASS, 0, WxStringToApplicationStringM(value));}

#define FINJIN_EXPORTER_IMPLEMENT_USER_DATA_ACCESSOR_METHODS(className, member)\
    wxString className::GetUserID() {return this->member->GetUserID();}\
    wxString className::GetUserData() {return this->member->GetUserData();}\
    wxString className::GetUserDataClass() {return this->member->GetUserDataClass();}\
    \
    void className::SetUserID(wxString value) {this->member->SetUserID(value);}\
    void className::SetUserData(wxString value) {this->member->SetUserData(value);}\
    void className::SetUserDataClass(wxString value) {this->member->SetUserDataClass(value);}
