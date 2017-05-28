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


//Includes----------------------------------------------------------------------
#include "FinjinPrecompiled.hpp"
#include "UserDataTypes.hpp"
#include "StringUtilities.hpp"
#include "FileUtilities.hpp"
#include "Strings.hpp"
#include "ApplicationAccessor.hpp"

using namespace Finjin::Exporter;


//Local functions---------------------------------------------------------------
static bool IsValidNameChar(wxChar c)
{
    return c != wxT('=') && c != wxT('\r') && c != wxT('\n') && c != wxT('\t');// isalpha(c) || isdigit(c) || c == wxT('_') || c == wxT('-');
}

static void FixName(wxString& name)
{
    for (size_t i = 0; i < name.length(); i++)
    {
        if (!IsValidNameChar(name[i]))
            name[i] = wxT(' ');
    }

    StringUtilities::RemoveSpace(name);
}


//Implementation----------------------------------------------------------------

//UserDataTypes
UserDataTypes::UserDataTypes()
{
}

UserDataTypes::~UserDataTypes()
{
    Clear();
}

void UserDataTypes::Clear()
{
    this->directory.clear();
    this->loadedFiles.clear();
    this->enums.clear();
    this->classes.clear();
}

bool UserDataTypes::Load(const wxString& fileName)
{
    EnsureBaseDirectory();

    auto directory = FileUtilities::GetDirectoryPath(fileName);
    if (!directory.empty())
    {
        //A file path with a directory was specified
        this->directory = FileUtilities::JoinPath(this->directory, directory);
    }

    //Create the full file path
    auto baseFileName = FileUtilities::GetFileName(fileName);
    auto fileNameToLoad = FileUtilities::JoinPath(this->directory, baseFileName);

    //Load the file if it hasn't been loaded already
    if (this->loadedFiles.find(fileNameToLoad) == this->loadedFiles.end())
    {
        WxByteBuffer fileBytes;
        if (FileUtilities::ReadBinaryFile(fileNameToLoad, fileBytes))
        {
            this->loadedFiles.insert(fileNameToLoad);

            WxConfigDocumentReader reader;
            for (auto line = reader.Start(fileBytes); line != nullptr; line = reader.Next())
            {
                switch (line->GetType())
                {
                    case WxConfigDocumentLine::Type::SECTION:
                    {
                        auto value = line->GetSectionName();
                        if (value == wxT("include"))
                            LoadInclude(reader, value);
                        else if (value == wxT("enum"))
                            LoadEnum(reader, value);
                        else if (value == wxT("class"))
                            LoadClass(reader, value);
                        break;
                    }
                    case WxConfigDocumentLine::Type::SCOPE_START:
                    {
                        break;
                    }
                    case WxConfigDocumentLine::Type::SCOPE_END:
                    {
                        break;
                    }
                    case WxConfigDocumentLine::Type::KEY_AND_VALUE:
                    {
                        break;
                    }
                    default: break;
                }
            }

            //File loaded
            return true;
        }
        else
        {
            //File could not be loaded
            return false;
        }
    }
    else
    {
        //File is already loaded
        return true;
    }
}

void UserDataTypes::EnsureBaseDirectory()
{
    //Make sure there is an base directory specified
    if (this->directory.empty())
        this->directory = ApplicationAccessor::GetApplicationDirectory();
}

void UserDataTypes::LoadInclude(WxConfigDocumentReader& reader, const wxString& initialSectionName)
{
    StaticVector<wxString, 10> sections;
    auto currentSectionName = initialSectionName;

    for (auto line = reader.Next(); line != nullptr; line = reader.Next())
    {
        switch (line->GetType())
        {
            case WxConfigDocumentLine::Type::SECTION:
            {
                currentSectionName = line->GetSectionName();
                break;
            }
            case WxConfigDocumentLine::Type::SCOPE_START:
            {
                if (sections.full())
                {
                    //This is an error condition
                    return;
                }
                else
                    sections.push_back(currentSectionName);
                break;
            }
            case WxConfigDocumentLine::Type::SCOPE_END:
            {
                sections.pop_back();
                if (sections.empty())
                    return;
                else
                    currentSectionName = sections.back();
                break;
            }
            case WxConfigDocumentLine::Type::KEY_AND_VALUE:
            {
                wxString key, fileName;
                line->GetKeyAndValue(key, fileName);
                if (currentSectionName == initialSectionName)
                {
                    if (key == wxT("file"))
                    {
                        if (!fileName.empty())
                        {
                            auto extension = FileUtilities::GetExtension(fileName, true);
                            if (extension.empty())
                            {
                                fileName += wxT(".");
                                fileName += Strings::USER_DATA_TYPES_FILE_EXTENSION;
                            }

                            auto currentDirectory = this->directory;
                            Load(fileName); //Load modifies this->directory
                            this->directory = currentDirectory;
                        }
                    }
                }

                break;
            }
            default: break;
        }
    }
}

void UserDataTypes::LoadEnum(WxConfigDocumentReader& reader, const wxString& initialSectionName)
{
    StaticVector<wxString, 10> sections;
    auto currentSectionName = initialSectionName;

    std::shared_ptr<Enum> enumType(new Enum);
    Enum::Item enumValue;

    for (auto line = reader.Next(); line != nullptr; line = reader.Next())
    {
        switch (line->GetType())
        {
            case WxConfigDocumentLine::Type::SECTION:
            {
                currentSectionName = line->GetSectionName();
                break;
            }
            case WxConfigDocumentLine::Type::SCOPE_START:
            {
                if (sections.full())
                {
                    //This is an error condition
                    return;
                }
                else
                {
                    sections.push_back(currentSectionName);
                }
                break;
            }
            case WxConfigDocumentLine::Type::SCOPE_END:
            {
                sections.pop_back();
                if (sections.empty())
                    return;
                else
                {
                    if (currentSectionName == wxT("enum-value"))
                    {
                        if (enumValue.IsValid())
                        {
                            enumType->items.push_back(enumValue);
                            enumValue.Reset();
                        }
                    }

                    currentSectionName = sections.back();
                }
                break;
            }
            case WxConfigDocumentLine::Type::KEY_AND_VALUE:
            {
                auto sectionName = sections.back();

                wxString key, value;
                line->GetKeyAndValue(key, value);
                if (currentSectionName == initialSectionName)
                {
                    if (key == wxT("name"))
                    {
                        FixName(value);

                        if (!value.empty() && GetClass(value) == nullptr && GetEnum(value) == nullptr)
                        {
                            enumType->name = value;

                            this->enums[value] = enumType;
                        }
                    }
                    else if (key == wxT("type"))
                    {
                        FixName(value);

                        enumType->type = GetDataType(*enumType.get(), value);
                    }
                    else if (key == wxT("super"))
                    {
                        //Get super classes
                        if (!value.empty())
                        {
                            wxStringTokenizer supers(value, wxT(FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_STRING));
                            while (supers.HasMoreTokens())
                            {
                                auto super = supers.GetNextToken();
                                FixName(super);

                                auto superEnum = GetEnum(super);
                                if (superEnum != nullptr)
                                    enumType->supers.push_back(superEnum);
                            }
                        }
                    }
                }
                else if (sectionName == wxT("enum-value"))
                {
                    if (key == wxT("name"))
                    {
                        FixName(value);
                        enumValue.name = value;
                        enumValue.value = value;
                    }
                    else if (key == wxT("value"))
                    {
                        FixName(value);
                        if (!value.empty())
                            enumValue.value = value;
                    }
                }

                break;
            }
            default: break;
        }
    }
}

void UserDataTypes::LoadClass(WxConfigDocumentReader& reader, const wxString& initialSectionName)
{
    StaticVector<wxString, 10> sections;
    auto currentSectionName = initialSectionName;

    std::shared_ptr<Class> userDataClass(new Class);
    Class::Item data;

    wxString minValue, maxValue;

    for (auto line = reader.Next(); line != nullptr; line = reader.Next())
    {
        switch (line->GetType())
        {
            case WxConfigDocumentLine::Type::SECTION:
            {
                currentSectionName = line->GetSectionName();
                break;
            }
            case WxConfigDocumentLine::Type::SCOPE_START:
            {
                if (sections.full())
                {
                    //This is an error condition
                    return;
                }
                else
                {
                    sections.push_back(currentSectionName);
                }
                break;
            }
            case WxConfigDocumentLine::Type::SCOPE_END:
            {
                sections.pop_back();
                if (sections.empty())
                    return;
                else
                {
                    if (currentSectionName == wxT("property"))
                    {
                        if (data.IsValid())
                        {
                            if (data.controlType == UserDataControlType::NONE)
                                data.controlType = GetControlType(data, wxEmptyString);

                            if (data.type.type == DataType::INT_DATA_TYPE)
                            {
                                data.minValue.intValue = minValue.empty() ? 0 : StringUtilities::ToInt(minValue);
                                data.maxValue.intValue = maxValue.empty() ? data.minValue.intValue + 100 : StringUtilities::ToInt(maxValue);
                            }
                            else if (data.type.type == DataType::FLOAT_DATA_TYPE)
                            {
                                data.minValue.floatValue = minValue.empty() ? 0 : StringUtilities::ToFloat(minValue);
                                data.maxValue.floatValue = maxValue.empty() ? data.minValue.floatValue + 100 : StringUtilities::ToFloat(maxValue);
                            }

                            userDataClass->items.push_back(data);
                            data.Reset();

                            minValue.clear();
                            maxValue.clear();
                        }
                    }

                    currentSectionName = sections.back();
                }
                break;
            }
            case WxConfigDocumentLine::Type::KEY_AND_VALUE:
            {
                auto sectionName = sections.back();

                wxString key, value;
                line->GetKeyAndValue(key, value);
                if (currentSectionName == initialSectionName)
                {
                    if (key == wxT("name"))
                    {
                        FixName(value);

                        if (!value.empty() && GetClass(value) == nullptr && GetEnum(value) == nullptr)
                        {
                            userDataClass->name = value;
                            userDataClass->displayName = value;

                            this->classes[value] = userDataClass;
                        }
                    }
                    else if (key == wxT("display-name"))
                    {
                        if (!value.empty())
                            userDataClass->displayName = value;
                    }
                    else if (key == wxT("super"))
                    {
                        if (!value.empty())
                        {
                            wxStringTokenizer supers(value, wxT(FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_STRING));
                            while (supers.HasMoreTokens())
                            {
                                auto super = supers.GetNextToken();
                                FixName(super);
                                auto superClass = GetClass(super);
                                if (superClass != nullptr)
                                    userDataClass->supers.push_back(superClass);
                            }
                        }
                    }
                    else if (key == wxT("usage"))
                    {
                        if (!value.empty())
                        {
                            wxStringTokenizer usages(value, wxT(FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_STRING));
                            while (usages.HasMoreTokens())
                            {
                                auto usage = usages.GetNextToken();
                                if (usage == wxT("scene"))
                                    userDataClass->usage = userDataClass->usage | UserDataUsage::SCENE;
                                else if (usage == wxT("object"))
                                    userDataClass->usage = userDataClass->usage | UserDataUsage::OBJECT;
                                else if (usage == wxT("material"))
                                    userDataClass->usage = userDataClass->usage | UserDataUsage::MATERIAL;
                                else if (usage == wxT("private"))
                                    userDataClass->usage = userDataClass->usage | UserDataUsage::PRIVATE;
                            }
                        }
                    }
                }
                else if (sectionName == wxT("property"))
                {
                    if (key == wxT("group-name"))
                    {
                        data.groupName = value;
                    }
                    else if (key == wxT("name"))
                    {
                        data.name = value;
                        FixName(data.name);

                        data.displayName = data.name;
                    }
                    else if (key == wxT("display-name"))
                    {
                        if (!value.empty())
                            data.displayName = value;
                    }
                    else if (key == wxT("visibility-parent-name"))
                    {
                        FixName(value);
                        if (value != data.name)
                            data.visibilityParentName = value;
                    }
                    else if (key == wxT("visibility-children-names"))
                    {
                        if (!value.empty())
                        {
                            wxStringTokenizer visibilityChildren(value, wxT(FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_STRING));
                            while (visibilityChildren.HasMoreTokens())
                            {
                                auto visibilityChildName = visibilityChildren.GetNextToken();
                                FixName(visibilityChildName);
                                if (visibilityChildName != data.visibilityParentName && visibilityChildName != data.name)
                                    data.visibilityChildrenNames.push_back(visibilityChildName);
                            }
                        }
                    }
                    else if (key == wxT("type"))
                    {
                        wxString dataType = value;
                        FixName(dataType);

                        data.type = GetDataType(data, dataType);
                    }
                    else if (key == wxT("default") || key == wxT("default-value"))
                    {
                        data.defaultValue = value;
                    }
                    else if (key == wxT("control-type"))
                    {
                        data.controlType = GetControlType(data, value);
                    }
                    else if (key == wxT("edit-width"))
                    {
                        data.editWidth = StringUtilities::ToInt(value);
                    }
                    else if (key == wxT("edit-height"))
                    {
                        data.editHeight = StringUtilities::ToInt(value);
                    }
                    else if (key == wxT("min-value"))
                    {
                        minValue = value;
                    }
                    else if (key == wxT("max-value"))
                    {
                        maxValue = value;
                    }
                    else if (key == wxT("increment"))
                    {
                        data.increment = StringUtilities::ToFloat(value);
                    }
                }

                break;
            }
            default: break;
        }
    }
}

UserDataTypes::DataType UserDataTypes::GetDataType(const Class::Item& item, const wxString& type)
{
    UserDataTypes::DataType dt;

    if (type.empty() || type == wxT("string"))
    {
        //Do nothing
    }
    else if (type.StartsWith(wxT("int")))
    {
        dt.type = UserDataTypes::DataType::INT_DATA_TYPE;

        wxString countText = type.substr(3);
        dt.count = std::max(1, StringUtilities::ToInt(countText));
    }
    else if (type.StartsWith(wxT("float")))
    {
        dt.type = UserDataTypes::DataType::FLOAT_DATA_TYPE;

        wxString countText = type.substr(5);
        dt.count = std::max(1, StringUtilities::ToInt(countText));
    }
    else if (type == wxT("bool"))
        dt.type = UserDataTypes::DataType::BOOL_DATA_TYPE;
    else if (type == wxT("rgb"))
        dt.type = UserDataTypes::DataType::RGB_DATA_TYPE;
    else if (type == wxT("rgba"))
        dt.type = UserDataTypes::DataType::RGBA_DATA_TYPE;
    else
    {
        //It's hopefully an enum of some sort
        Enum* enumType = GetEnum(type);
        if (enumType != nullptr)
            dt = GetDataType(*enumType, type);
    }

    return dt;
}

UserDataTypes::DataType UserDataTypes::GetDataType(Enum& e, const wxString& type)
{
    UserDataTypes::DataType dt;
    dt.enumType = &e;

    if (type.empty() || type == wxT("string"))
    {
        //Do nothing
    }
    else if (type.StartsWith(wxT("int")))
    {
        dt.type = UserDataTypes::DataType::INT_DATA_TYPE;

        wxString countText = type.substr(3);
        dt.count = std::max(1, StringUtilities::ToInt(countText));
    }
    else if (type.StartsWith(wxT("float")))
    {
        dt.type = UserDataTypes::DataType::FLOAT_DATA_TYPE;

        wxString countText = type.substr(5);
        dt.count = std::max(1, StringUtilities::ToInt(countText));
    }
    else if (type == wxT("bool"))
        dt.type = UserDataTypes::DataType::BOOL_DATA_TYPE;
    else if (type == wxT("rgb"))
        dt.type = UserDataTypes::DataType::RGB_DATA_TYPE;
    else if (type == wxT("rgba"))
        dt.type = UserDataTypes::DataType::RGBA_DATA_TYPE;

    return dt;
}

UserDataControlType UserDataTypes::GetControlType(const Class::Item& item, const wxString& controlType) const
{
    if (item.type.enumType != nullptr)
    {
        //Enums always use a combo box
        return UserDataControlType::COMBO;
    }
    else
    {
        switch (item.type.type)
        {
            case DataType::STRING_DATA_TYPE:
            {
                return UserDataControlType::EDIT;
            }
            case DataType::INT_DATA_TYPE:
            {
                if (controlType == wxT("edit"))
                    return UserDataControlType::EDIT;
                else if (controlType == wxT("spinner"))
                    return UserDataControlType::SPINNER;
                else
                    return UserDataControlType::SPINNER;
            }
            case DataType::FLOAT_DATA_TYPE:
            {
                if (controlType == wxT("edit"))
                    return UserDataControlType::EDIT;
                else if (controlType == wxT("spinner"))
                    return UserDataControlType::SPINNER;
                else
                    return UserDataControlType::SPINNER;
            }
            case DataType::BOOL_DATA_TYPE:
            {
                return UserDataControlType::CHECK;
            }
            case DataType::RGB_DATA_TYPE:
            {
                return UserDataControlType::RGB_SWATCH;
            }
            case DataType::RGBA_DATA_TYPE:
            {
                return UserDataControlType::RGBA_SWATCH;
            }
        }
    }

    return UserDataControlType::EDIT;
}

UserDataTypes::Enum* UserDataTypes::GetEnum(const wxString& name)
{
    auto e = this->enums.find(name);
    if (e != this->enums.end())
        return e->second.get();
    else
        return nullptr;
}

const UserDataTypes::Enum* UserDataTypes::GetEnum(const wxString& name) const
{
    auto e = this->enums.find(name);
    if (e != this->enums.end())
        return e->second.get();
    else
        return nullptr;
}

UserDataTypes::Class* UserDataTypes::GetClass(const wxString& name)
{
    auto e = this->classes.find(name);
    if (e != this->classes.end())
        return e->second.get();
    else
        return nullptr;
}

const UserDataTypes::Class* UserDataTypes::GetClass(const wxString& name) const
{
    auto e = this->classes.find(name);
    if (e != this->classes.end())
        return e->second.get();
    else
        return nullptr;
}

UserDataTypes::Class* UserDataTypes::GetClass(int index, UserDataUsage usage)
{
    int counter = 0;
    for (auto c = this->classes.begin(); c != this->classes.end(); ++c)
    {
        if (c->second->IsSupportedUsage(usage))
        {
            if (index == counter)
                return c->second.get();
            counter++;
        }
    }

    return nullptr;
}

template <class T, class T_ITEM>
void GetItems(T object, std::vector<T_ITEM>& items)
{
    //Visited objects
    std::unordered_set<T> visited;

    //After the main loop completes, this list contains an in-order walk of the class tree
    std::list<T> inorderObjects;

    //Objects to visit
    std::list<T> toVisit;

    //Prime the list with the first object
    toVisit.push_front(object);

    //The main loop
    while (!toVisit.empty())
    {
        //Get last object as the current one and remove it from list
        T current = toVisit.back();
        toVisit.pop_back();

        //Record the object as visited
        visited.insert(current);

        //Put the current object at the front of the list. As parents are added, the current
        //object will pushed further down the list
        inorderObjects.push_front(current);

        //Add the object's super items
        //This is done in reverse order so that when in-order list is walked, the objects
        //will be in the correct order
        for (auto superIterator = current->supers.rbegin();
            superIterator != current->supers.rend();
            ++superIterator)
        {
            //Add the super enum only if the enum hasn't already been added
            if (visited.find(*superIterator) == visited.end())
                toVisit.push_front(*superIterator);
        }
    }

    //Walk all the objects in order
    int order = 0;
    for (auto inorderObjectIterator = inorderObjects.begin();
        inorderObjectIterator != inorderObjects.end();
        ++inorderObjectIterator)
    {
        T inorderObject = *inorderObjectIterator;

        //Add all the items in the current object
        for (auto itemIterator = inorderObject->items.begin();
            itemIterator != inorderObject->items.end();
            ++itemIterator)
        {
            auto foundAt = std::find(items.begin(), items.end(), *itemIterator);
            if (foundAt == items.end())
            {
                //Item with the same name hasn't been added yet. Add it
                T_ITEM item = *itemIterator;
                if (item.order == std::numeric_limits<int>::max())
                    item.order = order++;
                items.push_back(item);
            }
            else
            {
                //Item with the same name has already been added. Update it
                int order = (itemIterator->order != std::numeric_limits<int>::max()) ? itemIterator->order : foundAt->order;
                *foundAt = *itemIterator;
                foundAt->order = order;
            }
        }
    }
}

void UserDataTypes::Enum::GetEnumItems(Items& items)
{
    items.reserve(100);
    GetItems(this, items);
    std::sort(items.begin(), items.end());
}

void UserDataTypes::Class::GetDataItems(Items& items)
{
    items.reserve(100);
    GetItems(this, items);
    std::sort(items.begin(), items.end());
}

//UserDataTypes::Enum::Item
UserDataTypes::Enum::Item::Item()
{
    this->order = std::numeric_limits<int>::max();
}

void UserDataTypes::Enum::Item::Reset()
{
    this->name.clear();
    this->value.clear();
    this->order = std::numeric_limits<int>::max();
}

bool UserDataTypes::Enum::Item::IsValid() const
{
    return !this->name.empty() && !this->value.empty();
}

bool UserDataTypes::Enum::Item::operator < (const Item& item) const
{
    if (this->order != std::numeric_limits<int>::max() || item.order != std::numeric_limits<int>::max())
        return this->order < item.order;

    return this->name < item.name;
}

bool UserDataTypes::Enum::Item::operator == (const Item& item) const
{
    return this->name == item.name;
}

//UserDataTypes::Class::Item
UserDataTypes::Class::Item::Item()
{
    Reset();
}

void UserDataTypes::Class::Item::Reset()
{
    this->groupName.clear();
    this->order = std::numeric_limits<int>::max();
    this->name.clear();
    this->displayName.clear();
    this->visibilityParentName.clear();
    this->visibilityChildrenNames.clear();
    this->type.Reset();
    this->defaultValue.clear();
    this->controlType = UserDataControlType::NONE;
    this->editWidth = 0;
    this->editHeight = 0;
    this->minValue.intValue = 0;
    this->maxValue.intValue = 0;
    this->increment = 0;
}

bool UserDataTypes::Class::Item::IsValid() const
{
    return true;
}

bool UserDataTypes::Class::Item::operator < (const Item& item) const
{
    if (!this->groupName.empty() || !item.groupName.empty())
        return (this->groupName < item.groupName) ? true : false;

    if (this->order != std::numeric_limits<int>::max() || item.order != std::numeric_limits<int>::max())
        return this->order < item.order;

    return this->displayName < item.displayName;
}

bool UserDataTypes::Class::Item::operator == (const Item& item) const
{
    return (this->name == item.name) ? true : false;
}
