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

    class NodeAttributeAdder;

    /** Creates user data Maya attributes. */
    struct UserDataAttributes
    {
        /**
         * Creates the user data attributes.
         * @param adder [in] - Used to create the attributes.
         * @param attributeNamePrefix [in] - Prefix to prepend to attribute names.
         */
        void CreateUserDataAttributes(NodeAttributeAdder& adder, const MString& attributeNamePrefix = "");

        /**
         * Creates the user data attributes and puts the created attributes into the specified list.
         * @param attributesList [out] - Receives the created attributes.
         * @param adder [in] - Used to create the attributes.
         * @param attributeNamePrefix [in] - Prefix to prepend to attribute names.
         */
        void CreateUserDataAttributes(std::list<MObject>& attributesList, NodeAttributeAdder& adder, const MString& attributeNamePrefix = "");

        //User data attributes---------------------
        MObject id;
        MObject userData;
        MObject userDataClass;

        static const MString ID_ATTRIBUTE_NAME;
        static const MString USER_DATA_ATTRIBUTE_NAME;
        static const MString USER_DATA_CLASS_ATTRIBUTE_NAME;
    };

} }


//Macros------------------------------------------------------------------------
#define IMPLEMENT_STATIC_ATTRIBUTE_USER_DATA_ACCESSOR_METHODS(className, member, attributes) \
    wxString className::GetUserID() {return ApplicationStringToWxString(MPlug(this->member, attributes.id).asString());}\
    wxString className::GetUserData() {return ApplicationStringToWxString(MPlug(this->member, attributes.userData).asString());}\
    wxString className::GetUserDataClass() {return ApplicationStringToWxString(MPlug(this->member, attributes.userDataClass).asString());}\
    \
    void className::SetUserID(wxString value) {MPlug(this->member, attributes.id).setValue(WxStringToApplicationString(value));}\
    void className::SetUserData(wxString value) {MPlug(this->member, attributes.userData).setValue(WxStringToApplicationString(value));}\
    void className::SetUserDataClass(wxString value) {MPlug(this->member, attributes.userDataClass).setValue(WxStringToApplicationString(value));}

#define IMPLEMENT_DYNAMIC_ATTRIBUTE_USER_DATA_ACCESSOR_METHODS_PREFIXED(className, attributeNamePrefix) \
    wxString className::GetUserID() {return ApplicationStringToWxString(Child(attributeNamePrefix + UserDataAttributes::ID_ATTRIBUTE_NAME).asString());}\
    wxString className::GetUserData() {return ApplicationStringToWxString(Child(attributeNamePrefix + UserDataAttributes::USER_DATA_ATTRIBUTE_NAME).asString());}\
    wxString className::GetUserDataClass() {return ApplicationStringToWxString(Child(attributeNamePrefix + UserDataAttributes::USER_DATA_CLASS_ATTRIBUTE_NAME).asString());}\
    \
    void className::SetUserID(wxString value) {Child(attributeNamePrefix + UserDataAttributes::ID_ATTRIBUTE_NAME).setValue(WxStringToApplicationString(value));}\
    void className::SetUserData(wxString value) {Child(attributeNamePrefix + UserDataAttributes::USER_DATA_ATTRIBUTE_NAME).setValue(WxStringToApplicationString(value));}\
    void className::SetUserDataClass(wxString value) {Child(attributeNamePrefix + UserDataAttributes::USER_DATA_CLASS_ATTRIBUTE_NAME).setValue(WxStringToApplicationString(value));}

#define IMPLEMENT_DYNAMIC_ATTRIBUTE_USER_DATA_ACCESSOR_METHODS(className) \
    IMPLEMENT_DYNAMIC_ATTRIBUTE_USER_DATA_ACCESSOR_METHODS_PREFIXED(className, "")

#define IMPLEMENT_DYNAMIC_OR_STATIC_ATTRIBUTE_USER_DATA_ACCESSOR_METHODS_PREFIXED(className, attributeNamePrefix, attributes) \
    wxString className::GetUserID() {return ApplicationStringToWxString(Child(attributes.id, attributeNamePrefix + UserDataAttributes::ID_ATTRIBUTE_NAME).asString());}\
    wxString className::GetUserData() {return ApplicationStringToWxString(Child(attributes.userData, attributeNamePrefix + UserDataAttributes::USER_DATA_ATTRIBUTE_NAME).asString());}\
    wxString className::GetUserDataClass() {return ApplicationStringToWxString(Child(attributes.userDataClass, attributeNamePrefix + UserDataAttributes::USER_DATA_CLASS_ATTRIBUTE_NAME).asString());}\
    \
    void className::SetUserID(wxString value) {Child(attributes.id, attributeNamePrefix + UserDataAttributes::ID_ATTRIBUTE_NAME).setValue(WxStringToApplicationString(value));}\
    void className::SetUserData(wxString value) {Child(attributes.userData, attributeNamePrefix + UserDataAttributes::USER_DATA_ATTRIBUTE_NAME).setValue(WxStringToApplicationString(value));}\
    void className::SetUserDataClass(wxString value) {Child(attributes.userDataClass, attributeNamePrefix + UserDataAttributes::USER_DATA_CLASS_ATTRIBUTE_NAME).setValue(WxStringToApplicationString(value));}

#define IMPLEMENT_DYNAMIC_OR_STATIC_ATTRIBUTE_USER_DATA_ACCESSOR_METHODS(className, attributes) \
    IMPLEMENT_DYNAMIC_OR_STATIC_ATTRIBUTE_USER_DATA_ACCESSOR_METHODS_PREFIXED(className, "", attributes)
