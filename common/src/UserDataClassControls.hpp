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
#include "UserDataTypes.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * All the controls for a single user data class.
     */
    class UserDataClassControls
    {
    public:
        /** All the controls that are used for a single data item. */
        struct ItemControls
        {
        public:
            /**
             * Constructor.
             * @param dataItem [in] - The data item for which the controls will be created.
             * @param parent [in] - The parent window of the created controls.
             * @param parentSizer [in] - The sizer into which the controls will be added.
             * @param controlID [in/out] - The start control identifier. This is incremented
             * as controls are created and assigned new identifiers.
             * @param value [in] - The current value for the data item. It will be parsed as
             * required by the data item type.
             */
            ItemControls
                (
                UserDataTypes::Class::Item& dataItem,
                wxWindow* parent,
                wxSizer* parentSizer,
                int& controlID,
                const wxString& value
                );

            ~ItemControls();

            /** Determines whether this ItemControls has a control with the specified identifier. */
            bool HasControlID(int id) const;

            /** Gets the enum value at the specified index. */
            wxString GetEnumValue(int index) const;

            /** Gets the index of the enum with the specified value. */
            int GetEnumIndex(const wxString& value) const;

            /** Determines whether the control and its data is visible, as determined by the visibilityControl. */
            bool CheckVisibility();

            /** Updates the visibility of all controls. */
            void UpdateVisibility();

            /**
             * Determines whether a possible new reference to this set of controls will cause a circular reference.
             * @param possibleReference [in] - The set of controls that wants to use this object as visibility control.
             */
            bool CausesVisibilityCircularReference(ItemControls* possibleReference) const;

        public:
            UserDataTypes::Class::Item& dataItem;
            UserDataTypes::Enum::Items enumItems;

            /** The control that controls the visibility of this control. */
            ItemControls* visibilityControl;

            wxStaticText* labelWindow;
            std::vector<wxWindow*> windows;
        };

        UserDataClassControls();

        void Initialize(UserDataTypes::Class* classType, wxWindow* parent, const wxString& rawUserData);

        /**
         * Destroys all data members, including controls and possibly data.
         * @param destroyDocument [in] - Indicates whether the internal XML document should
         * be destroyed. External callers should never use this argument, it is for internal
         * use only.
         */
        void Destroy(bool destroyDocument = true);

        /** Gets all the data from the controls and puts it into the internal XML document. */
        void GetGUIData();

        /**
         * Gets all the data from the controls, puts it into the internal XML document, and
         * writes to the specified text buffer.
         * @param rawUserData [out] - The XML text.
         */
        void GetGUIData(wxString& rawUserData);

        /**
         * This should be called when the parent window detects a checkbox was clicked.
         * @param checkbox [in] - The check box that was clicked.
         * @return Indicates whether the checkbox was a user data control. If it was, the
         * user interface should probably be refreshed.
         */
        bool HandleCheckBoxClick(wxCheckBox* checkbox);

    protected:
        /** Create controls for each class data item .*/
        void InitializeControls();

        /** Synchronizes the property instances so that it contains data for each property. */
        void SynchronizePropertyInstancesToClassProperties();

        /** Determines if the specified property has been encountered. */
        bool HasProperty(wxString name) const;

        /** Gets the value for the specified XML element. */
        wxString GetPropertyInstanceValue(const wxString& name);

        /** Sets the value for the specified XML element. */
        void SetPropertyInstanceValue(const wxString& name, const wxString& value);

        /** Gets the object that contains the controls for the specified data item. */
        ItemControls* GetControlsForName(const wxString& name);

        struct PropertyInstance
        {
            wxString name;
            wxString value;
        };
        PropertyInstance* GetPropertyInstance(const wxString& name);

        /** Updates the visibility of all controls. */
        void UpdateVisibility();

    public:
        /** The class which the controls are linked. */
        UserDataTypes::Class* classType;

        /** The properties contained in the class. */
        UserDataTypes::Class::Items properties;

        /** The controls, one for each item in the class. */
        std::vector<std::unique_ptr<ItemControls> > controls;

        std::vector<PropertyInstance> propertyInstances;

        wxWindow* parentWindow;
        wxSizer* topSizer;

        enum {EDIT_HEIGHT = 16};
        enum {SPACE = 8};
    };

} }
