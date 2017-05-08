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
#include "finjin/common/WxConfigDocumentReader.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** The different ways the class can be used. */
    enum class UserDataUsage
    {
        /** Use the class for anything. */
        ANY = 0,

        /** Use the class with object settings. */
        OBJECT = 1 << 0,

        /** Use the class with material settings. */
        MATERIAL = 1 << 1,

        /** Use the class with scene settings. */
        SCENE = 1 << 2,

        /** Do not allow the class to be seen. */
        PRIVATE = 1 << 3
    };
    FINJIN_ENUM_BITWISE_OPERATIONS(UserDataUsage)

    /** A control type. */
    enum class UserDataControlType
    {
        NONE,
        EDIT,
        COMBO,
        CHECK,
        RGB_SWATCH,
        RGBA_SWATCH,
        SPINNER
    };

    /**
     * Manages and parses user data classes from XML files.
     * This is used to present custom controls for user defined data.
     */
    class UserDataTypes
    {
    public:
        UserDataTypes();
        ~UserDataTypes();

        /**
         * Loads the specified file into the current collection of user types.
         * @param fileName [in] - The file to load.
         * @return Returns true if the file is or was already loaded, false otherwise.
         */
        bool Load(const wxString& fileName);

        /** Clears the home directory, loaded file names, and all user types. */
        void Clear();

        struct Enum;

        /** A data type. */
        struct DataType
        {
        public:
            enum Type
            {
                STRING_DATA_TYPE,
                INT_DATA_TYPE,
                FLOAT_DATA_TYPE,
                BOOL_DATA_TYPE,
                RGB_DATA_TYPE,
                RGBA_DATA_TYPE
            };

            DataType()
            {
                Reset();
            }

            void Reset()
            {
                this->type = STRING_DATA_TYPE;
                this->count = 1;
                this->enumType = nullptr;
            }

        public:
            /** The basic type. */
            Type type;

            /** The number of values. Only relevant for INT_DATA_TYPE and FLOAT_DATA_TYPE. */
            int count;

            /** Pointer to an enum that constrains the values. May be null. */
            Enum* enumType;
        };

        /**
         * An enum is a list of predefined named values that constrain the values
         * that a data type can take on.
         */
        struct Enum
        {
        public:
            /** A single enumeration item. */
            struct Item
            {
            public:
                Item();

                void Reset();

                bool IsValid() const;

                /** Determines if this item is less than the other item. Used for sorting. */
                bool operator < (const Item& item) const;

                /** Determines if this item is equal to the other item. */
                bool operator == (const Item& item) const;

            public:
                /** Explicit order for the enum. Ignored by default. */
                int order;

                /** The name of the item. This is displayed. */
                wxString name;

                /** The value of the item. */
                wxString value;
            };

            typedef std::vector<Item> Items;

            /** Gets all the enum items for this enum, including the items in supers */
            void GetEnumItems(Items& items);

        public:
            /** The name of the enum. */
            wxString name;

            /** The enum data type. */
            DataType type;

            /** Pointers to enums that this enum inherits from. */
            std::list<Enum*> supers;

            /** All the enum items. Does not include the items from supers. */
            std::list<Item> items;
        };

        /**
         * A class is a collection of data items.
         */
        struct Class
        {
        public:
            Class()
            {
                this->usage = UserDataUsage::ANY;
            }

            /** Determines whether the class can be used in the specified way. */
            bool IsSupportedUsage(UserDataUsage usage) const
            {
                if (AnySet(this->usage & UserDataUsage::PRIVATE))
                    return false;
                else
                    return (this->usage == UserDataUsage::ANY || usage == UserDataUsage::ANY) ? true : AnySet(this->usage & usage);
            }

            /** A data item */
            struct Item
            {
            public:
                Item();

                void Reset();

                bool IsValid() const;

                /** Determines if this item is less than the other item. Used for sorting. */
                bool operator < (const Item& item) const;

                /** Determines if this item is equal to the other item. */
                bool operator == (const Item& item) const;

            public:
                /** The group that the item is assigned to. */
                wxString groupName;

                /** Explicit order for the item. Ignored by default. */
                int order;

                /** The internal name of the item. */
                wxString name;

                /** The display name of the item. */
                wxString displayName;

                /** Name of another item (presumably a boolean value) whose value determines whether this item is visible. */
                wxString visibilityParentName;

                /** Names of other items whoses visibility is determined by this item (presumed to be a boolean value). */
                std::vector<wxString> visibilityChildrenNames;

                /** The item data type. */
                DataType type;

                /** Default value, formatted as a string. */
                wxString defaultValue;

                /** The type of control to be used to display/edit item. */
                UserDataControlType controlType;

                /** The width of the edit box used to edit item. Only relevant for some types. */
                int editWidth;

                /** The height of the edit box used to edit item. Only relevant for some types. */
                int editHeight;

                union Value
                {
                    int intValue;
                    float floatValue;
                };

                /** The minimum allowed value. Only relevant for INT_DATA_TYPE and FLOAT_DATA_TYPE. */
                Value minValue;

                /** The maximum allowed value. Only relevant for INT_DATA_TYPE and FLOAT_DATA_TYPE. */
                Value maxValue;

                /** The spinner increment. Only relevant for INT_DATA_TYPE and FLOAT_DATA_TYPE with a SPINNER_CONTROL_TYPE. */
                float increment;
            };

            typedef std::vector<Item> Items;

            /** Gets all the data items for this class, including the items in supers. */
            void GetDataItems(Items& items);

        public:
            /** The class usage. */
            UserDataUsage usage;

            /** The internal class name. */
            wxString name;

            /** The display name. */
            wxString displayName;

            /** Pointers to classes that this class inherits from. */
            std::list<Class*> supers;

            /** All the items. Does not include the items from supers. */
            std::list<Item> items;
        };

        /** Gets the enum with the specified name. */
        Enum* GetEnum(const wxString& name);

        /** Gets the enum with the specified name. */
        const Enum* GetEnum(const wxString& name) const;

        /** Gets the class with the specified name. */
        Class* GetClass(const wxString& name);

        /** Gets the class with the specified name. */
        const Class* GetClass(const wxString& name) const;

        /** Gets the class at the specified index with the specified usage. */
        Class* GetClass(int index, UserDataUsage usage);

    protected:
        /** Ensures that the 'directory' member is set. */
        void EnsureBaseDirectory();

        /** Loads an 'include' element. */
        void LoadInclude(WxConfigDocumentReader& reader, const wxString& initialSectionName);

        /** Loads an 'enum' element. */
        void LoadEnum(WxConfigDocumentReader& reader, const wxString& initialSectionName);

        /** Loads a 'class' element. */
        void LoadClass(WxConfigDocumentReader& reader, const wxString& initialSectionName);

        /** Determines a class item's data type. */
        DataType GetDataType(const Class::Item& item, const wxString& type);

        /** Determines an enum's data type. */
        DataType GetDataType(Enum& e, const wxString& type);

        /** Determines the appropriate control type for a class item. */
        UserDataControlType GetControlType(const Class::Item& item, const wxString& controlType) const;

    protected:
        /** The current base directory. */
        wxString directory;

        /** Full path of all files that have been loaded. */
        std::set<wxString> loadedFiles;

    public:
        typedef std::unordered_map<wxString, std::shared_ptr<Enum> > Enums;
        typedef std::unordered_map<wxString, std::shared_ptr<Class> > Classes;

        /** All the enums. */
        Enums enums;

        /** All the classes. */
        Classes classes;
    };

} }
