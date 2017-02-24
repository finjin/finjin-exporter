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

    /** 
     * This class provides generic access to user data stored within an object.
     * User data is the data that users can modify in the object and scene settings dialogs.
     */
    class UserDataSettingsAccessor
    {
    public:
        virtual ~UserDataSettingsAccessor() {}

        virtual wxString GetUserID() = 0;
        virtual wxString GetUserData() = 0;
        virtual wxString GetUserDataClass() = 0;

        virtual void SetUserID(wxString value) = 0;
        virtual void SetUserData(wxString value) = 0;
        virtual void SetUserDataClass(wxString value) = 0;

        bool HasUserData(wxString* id = nullptr, wxString* userData = nullptr)
        {
            wxString tempId = GetUserID();
            wxString tempUserData = GetUserData();

            if (id != nullptr)
                *id = tempId;
            if (userData != nullptr)
                *userData = tempUserData;

            return !tempId.empty() || !tempUserData.empty();
        }
    };

} }


//Macros-----------------------------------------------------------------------

/** Common user data accessor method declarations. */
#define FINJIN_EXPORTER_DECLARE_USER_DATA_ACCESSOR_METHODS \
    wxString GetUserID();\
    wxString GetUserData();\
    wxString GetUserDataClass();\
    \
    void SetUserID(wxString value);\
    void SetUserData(wxString value);\
    void SetUserDataClass(wxString value);

#define FINJIN_EXPORTER_DECLARE_USER_DATA_ACCESSOR_METHODS_OVERRIDE \
    wxString GetUserID() override;\
    wxString GetUserData() override;\
    wxString GetUserDataClass() override;\
    \
    void SetUserID(wxString value) override;\
    void SetUserData(wxString value) override;\
    void SetUserDataClass(wxString value) override;


/**
 * Default common user data accessor method implementation.
 * This is useful during development, before the actual data storage code has been written.
 */
#define FINJIN_EXPORTER_IMPLEMENT_DEFAULT_USER_DATA_ACCESSOR_METHODS(className) \
    wxString className::GetUserID() {return wxEmptyString;}\
    wxString className::GetUserData() {return wxEmptyString;}\
    wxString className::GetUserDataClass() {return wxEmptyString;}\
    \
    void className::SetUserID(wxString value) {}\
    void className::SetUserData(wxString value) {}\
    void className::SetUserDataClass(wxString value) {}
