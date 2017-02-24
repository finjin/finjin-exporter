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
#include "UserDataClassControls.hpp"
#include "Dialogs.hpp"
#include "StringSplitter.hpp"
#include "StringUtilities.hpp"
#include "ApplicationControls.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "SpinnerControl.hpp"
#include "ApplicationAccessor.hpp"
#include "finjin/common/WxConfigDocumentWriter.hpp"
#include "finjin/common/WxDocumentWriterOutput.hpp"

#define ALLOW_EMPTY_VALUES true

using namespace Finjin::Exporter;


//Locals-----------------------------------------------------------------------
static const int BASE_CONTROL_ID = 10000;

static wxFlexGridSizer* CreateGridSizer()
{
    auto sizer = new wxFlexGridSizer(2, 2, 2);
    sizer->SetFlexibleDirection(wxBOTH);
    sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
    return sizer;
}


//Implementation---------------------------------------------------------------

//UserDataClassControls-----------
UserDataClassControls::UserDataClassControls()
{
    this->classType = nullptr;
    this->parentWindow = nullptr;
    this->topSizer = nullptr;
}

void UserDataClassControls::Initialize(UserDataTypes::Class* classType, wxWindow* parent, const wxString& rawUserData)
{
    //Destroy old controls
    Destroy();

    this->parentWindow = parent;

    this->classType = classType;

    classType->GetDataItems(this->properties);    
    
    this->propertyInstances.resize(this->properties.size());
    for (size_t i = 0; i < this->properties.size(); i++)
        this->propertyInstances[i].name = this->properties[i].name;
    
    //Parse data
    if (!rawUserData.empty())
    {
        wxString sectionName;

        WxConfigDocumentReader reader;
        auto byteBuffer = rawUserData.ToUTF8();
        auto byteBufferLength = strlen(byteBuffer.data());
        for (auto line = reader.Start(byteBuffer.data(), byteBuffer.data() + byteBufferLength); line != nullptr; line = reader.Next())
        {    
            switch (line->GetType())
            {
                case WxConfigDocumentLine::Type::SECTION:
                {
                    sectionName = line->GetSectionName();
                    break;
                }
                case WxConfigDocumentLine::Type::KEY_AND_VALUE:
                {
                    if (!sectionName.empty())
                    {
                        wxString key, value;
                        line->GetKeyAndValue(key, value);
                        auto propInstance = GetPropertyInstance(key);
                        if (propInstance != nullptr)
                            propInstance->value = value;                        
                    }
                    break;
                }
                default: break;
            }
        }
    }

    InitializeControls();   
}

void UserDataClassControls::InitializeControls()
{
    //Synchronize data to class items
    SynchronizePropertyInstancesToClassProperties();
    
    SUSPEND_CUSTOM_CONTROLS;

    //Create controls for each class property
    this->topSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer* controlParentSizer = nullptr;
    wxString previousGroupName;
    int controlID = BASE_CONTROL_ID;
    for (auto item = this->properties.begin(); item != this->properties.end(); ++item)
    {
        //Determine parent window of the control
        if (controlParentSizer == nullptr || item->groupName != previousGroupName)
        {
            if (!item->groupName.empty())
            {
                //The parent sizer is a box sizer
                auto groupSizer = new wxStaticBoxSizer(new wxStaticBox(this->parentWindow, controlID++, item->groupName), wxVERTICAL);
                auto newSizer = CreateGridSizer();
                groupSizer->Add(newSizer);
                this->topSizer->Add(groupSizer, 0, wxEXPAND|wxTOP, 5);
                
                controlParentSizer = newSizer;
            }
            else
            {
                //The parent sizer is the top sizer
                auto newSizer = CreateGridSizer();
                this->topSizer->Add(newSizer, 0, wxEXPAND|wxTOP, 5);
                
                controlParentSizer = newSizer;
            }

            previousGroupName = item->groupName;
        }
        
        //Get current item's value
        auto value = GetPropertyInstanceValue(item->name);

        //Create the controls for the item and its value
        std::unique_ptr<ItemControls> dataControls(new ItemControls(*item, this->parentWindow, controlParentSizer, controlID, value));

        //Add control to collection
        this->controls.push_back(std::move(dataControls));
    }

    //Get 'visibility' controls for each control
    //This is done as a separate step so that controls can reference controls created later
    for (size_t controlIndex = 0; controlIndex < this->controls.size(); controlIndex++)
    {
        auto control = this->controls[controlIndex].get();
        auto& item = control->dataItem;
                
        //Get the parent visibility control and only use it if it's for a boolean value
        auto parentVisibilityControl = GetControlsForName(item.visibilityParentName);
        if (parentVisibilityControl != nullptr && 
            parentVisibilityControl->dataItem.type.type == UserDataTypes::DataType::BOOL_DATA_TYPE &&
            !parentVisibilityControl->CausesVisibilityCircularReference(control))
        {
            control->visibilityControl = parentVisibilityControl;
        }

        //From the perspective of child controls, the current control we're iterating on is the parent
        parentVisibilityControl = control;
        for (size_t i = 0; i < item.visibilityChildrenNames.size(); i++)
        {
            auto childControl = GetControlsForName(item.visibilityChildrenNames[i]);
            if (childControl != nullptr &&
                parentVisibilityControl->dataItem.type.type == UserDataTypes::DataType::BOOL_DATA_TYPE &&
                !parentVisibilityControl->CausesVisibilityCircularReference(childControl))
            {
                childControl->visibilityControl = parentVisibilityControl;
            }
        }
    }

    //Update visibility now that all controls have been fully initialized
    UpdateVisibility();
    
    this->parentWindow->SetSizer(this->topSizer);
    this->parentWindow->Layout();
}

void UserDataClassControls::Destroy(bool destroyDocument)
{
    //Delete custom controls
    if (this->parentWindow != nullptr)
        this->parentWindow->DestroyChildren();
    this->controls.clear();
    
    //Remove references to class type
    this->classType = nullptr;
    this->properties.clear();

    //Destroy document if necessary
    if (destroyDocument)
        this->propertyInstances.clear();
}

void UserDataClassControls::GetGUIData()
{
    //Loop through all the controls, get the data, and put it into the corresponding property instance
    for (size_t controlIndex = 0; controlIndex < this->controls.size(); controlIndex++)
    {
        auto control = this->controls[controlIndex].get();
        auto& item = control->dataItem;
        
        //Determine if the value if worth saving
        if (!control->CheckVisibility())
            continue;

        //Get the output value
        wxString value;
        if (item.type.enumType != nullptr)
        {
            //It's an enum
            int curSel = ((wxChoice*)control->windows[0])->GetSelection();
            value = control->GetEnumValue(curSel);
        }
        else
        {
            //It's not an enum
            switch (item.type.type)
            {
                case UserDataTypes::DataType::STRING_DATA_TYPE:
                {
                    value = ((ApplicationTextCtrl*)control->windows[0])->GetValue();
                    break;
                }
                case UserDataTypes::DataType::INT_DATA_TYPE:
                {
                    std::vector<int> values;
                    values.resize(item.type.count);

                    for (int i = 0; i < item.type.count; i++)
                    {
                        if (item.controlType == UserDataControlType::EDIT)
                        {
                            auto editValue = ((ApplicationTextCtrl*)control->windows[i])->GetValue();
                            values[i] = StringUtilities::ToInt(editValue);
                        }
                        else if (item.controlType == UserDataControlType::SPINNER)
                            values[i] = ((SpinnerControl*)control->windows[i])->GetIntValue();
                    }

                    value = StringUtilities::ToString(values);

                    break;
                }
                case UserDataTypes::DataType::FLOAT_DATA_TYPE:
                {
                    std::vector<float> values;
                    values.resize(item.type.count);

                    for (int i = 0; i < item.type.count; i++)
                    {
                        if (item.controlType == UserDataControlType::EDIT)
                        {
                            auto editValue = ((ApplicationTextCtrl*)control->windows[i])->GetValue();
                            values[i] = StringUtilities::ToFloat(editValue);
                        }
                        else if (item.controlType == UserDataControlType::SPINNER)
                            values[i] = ((SpinnerControl*)control->windows[i])->GetValue();
                    }

                    value = StringUtilities::ToString(values);

                    break;
                }
                case UserDataTypes::DataType::BOOL_DATA_TYPE:
                {
                    value = ((wxCheckBox*)control->windows[0])->GetValue() ? wxT("true") : wxT("false");
                    break;
                }
                case UserDataTypes::DataType::RGB_DATA_TYPE:
                {
                    FinjinColor color = ((Finjin::Exporter::ColorPickerControl*)control->windows[0])->GetColour();
                    value = StringUtilities::ToString(color, false);
                    break;
                }
                case UserDataTypes::DataType::RGBA_DATA_TYPE:
                {
                    FinjinColor color = ((Finjin::Exporter::ColorPickerControl*)control->windows[0])->GetColour();
                    color.a = ((SpinnerControl*)control->windows[1])->GetValue();
                    value = StringUtilities::ToString(color, true);

                    break;
                }        
            }    
        }

        //Put the value into the appropriate property instance
        SetPropertyInstanceValue(item.name, value);        
    }
}

void UserDataClassControls::GetGUIData(wxString& rawUserData)
{
    GetGUIData();    

    if (this->classType != nullptr)
    {
        std::stringstream outStream;
        StreamDataChunkWriterOutput output(outStream);
        WxConfigDocumentWriter writer;
        writer.Create(output);

        writer.WriteScopeStart(this->classType->name);
        {
            for (auto& propInstance : this->propertyInstances)
            {
                if (ALLOW_EMPTY_VALUES || !propInstance.value.empty())
                    writer.WriteKeyAndValue(propInstance.name, propInstance.value);
            }
        }
        writer.WriteScopeEnd();
    
        auto outString = outStream.str();
        rawUserData = wxString::FromUTF8(outString.c_str(), outString.length());                
    }
    else
        rawUserData.clear();
}

bool UserDataClassControls::HandleCheckBoxClick(wxCheckBox* checkbox)
{
    for (size_t controlIndex = 0; controlIndex < this->controls.size(); controlIndex++)
    {
        auto control = this->controls[controlIndex].get();
        for (size_t windowIndex = 0; windowIndex < control->windows.size(); windowIndex++)
        {
            if (control->windows[windowIndex] == checkbox)
            {
                UpdateVisibility();
                return true;
            }
        }
    }

    return false;
}

void UserDataClassControls::SynchronizePropertyInstancesToClassProperties()
{
    //Create document items that have no corresponding class items
    for (auto item = this->properties.begin(); item != this->properties.end(); ++item)
    {
        auto propertyInstance = GetPropertyInstance(item->name);
        if (propertyInstance == nullptr)
        {
            //No element, add it
            PropertyInstance newPropertyInstance;
            newPropertyInstance.name = item->name;
            newPropertyInstance.value = item->defaultValue;
            this->propertyInstances.push_back(newPropertyInstance);
        }
        else
        {
            //The element exists            
        }
    }

    //Remove all property instances that have no corresponding class item
    for (auto propInstanceIterator = this->propertyInstances.begin(); propInstanceIterator != this->propertyInstances.end();)
    {
        if (!HasProperty(propInstanceIterator->name))
            propInstanceIterator = this->propertyInstances.erase(propInstanceIterator);
        else
            ++propInstanceIterator;
    }
}

bool UserDataClassControls::HasProperty(wxString name) const
{
    for (auto item = this->properties.begin(); item != this->properties.end(); ++item)
    {
        if (item->name == name)
            return true;
    }
    return false;
}

wxString UserDataClassControls::GetPropertyInstanceValue(const wxString& name)
{
    wxString value;

    auto propInstance = GetPropertyInstance(name);
    if (propInstance != nullptr)
        value = propInstance->value;

    return value;
}

void UserDataClassControls::SetPropertyInstanceValue(const wxString& name, const wxString& value)
{
    auto propInstance = GetPropertyInstance(name);
    if (propInstance != nullptr)
        propInstance->value = value;
}

UserDataClassControls::ItemControls* UserDataClassControls::GetControlsForName(const wxString& name)
{
    for (size_t controlIndex = 0; controlIndex < this->controls.size(); controlIndex++)
    {
        auto control = this->controls[controlIndex].get();
        auto& item = control->dataItem;
        if (item.name == name)
            return control;
    }
    return nullptr;
}

void UserDataClassControls::UpdateVisibility()
{
    for (size_t controlIndex = 0; controlIndex < this->controls.size(); controlIndex++)
        this->controls[controlIndex]->UpdateVisibility();
}

UserDataClassControls::PropertyInstance* UserDataClassControls::GetPropertyInstance(const wxString& name)
{
    for (auto& propInstance : this->propertyInstances)
    {
        if (propInstance.name == name)
            return &propInstance;
    }
    return nullptr;
}


//UserDataClassControls::ItemControls-----------
UserDataClassControls::ItemControls::ItemControls
    (
    UserDataTypes::Class::Item& item, 
    wxWindow* parent, 
    wxSizer* parentSizer,
    int& controlID, 
    const wxString& value    
    ) : 
    dataItem(item)
{
    const long style = 0;
    enum {EDIT_WIDTH = 54};
    enum {COLOR_WIDTH = 40};
    enum {COLOR_HEIGHT = 20};
    enum {ALPHA_EDIT_WIDTH = EDIT_WIDTH / 2 + 10};
    
    this->visibilityControl = 0;

    //Create the label
    this->labelWindow = new ApplicationStaticTextCtrl(parent, controlID++, this->dataItem.displayName);
    parentSizer->Add(this->labelWindow, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    //Create the horizontal sizer for the data controls
    auto dataSizer = new wxBoxSizer(wxHORIZONTAL);
    parentSizer->Add(dataSizer);
    parentSizer = dataSizer;
    
    //Create the data controls
    switch (this->dataItem.controlType)
    {
        case UserDataControlType::EDIT:             
        {
            //Parse values
            StringSplitter splitValues(value, wxT(FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_CHAR));

            //Create controls
            int defaultEditWidth = this->dataItem.type.count == 1 ? 150 : EDIT_WIDTH;
            int editWidth = this->dataItem.editWidth > 0 ? this->dataItem.editWidth : defaultEditWidth;
            int editHeight = this->dataItem.editHeight > 0 ? this->dataItem.editHeight : wxDefaultSize.y;
            wxSize editSize(editWidth, editHeight);
            for (int columnIndex = 0; columnIndex < this->dataItem.type.count; columnIndex++)
            {
                //Create edit control
                ApplicationTextCtrl* editWindow;
                if (this->dataItem.type.type == UserDataTypes::DataType::INT_DATA_TYPE ||
                    this->dataItem.type.type == UserDataTypes::DataType::FLOAT_DATA_TYPE)
                {
                    editWindow = new ApplicationTextCtrl(parent, controlID++, wxEmptyString, wxDefaultPosition, editSize);
                }
                else
                {
                    long editStyle = style;
                    if (this->dataItem.editHeight != 0)
                        editStyle |= wxTE_MULTILINE;
                    editWindow = new ApplicationTextCtrl(parent, controlID++, wxEmptyString, wxDefaultPosition, editSize, editStyle);                    
                }
                parentSizer->Add(editWindow);
                this->windows.push_back(editWindow);
                
                //Set value
                if (columnIndex < (int)splitValues.size())
                {
                    const wxString& theValue = (this->dataItem.type.count == 1) ? value : splitValues[columnIndex];
                    editWindow->SetValue(theValue);
                }
            }
                        
            break;
        }
        case UserDataControlType::COMBO:
        {
            //Create control
            auto comboWindow = new ApplicationChoiceCtrl(parent, controlID++, wxDefaultPosition, wxDefaultSize, wxArrayString()); 
            this->windows.push_back(comboWindow);

            //Fill combo box
            if (this->dataItem.type.enumType != nullptr)
            {
                //Get all the enumeration items
                this->dataItem.type.enumType->GetEnumItems(this->enumItems);

                //Put all the combo strings into an array and into the control
                std::vector<wxString> comboItems;
                comboItems.reserve(this->enumItems.size());
                comboWindow->Freeze();
                for (auto enumIterator = this->enumItems.begin(); enumIterator != this->enumItems.end(); ++enumIterator)
                {
                    comboItems.push_back(enumIterator->name);
                    comboWindow->Append(enumIterator->name);
                }
                comboWindow->Thaw();
                
                //Select current item
                comboWindow->SetSelection(GetEnumIndex(value));
            }

            parentSizer->Add(comboWindow);

            break;
        }
        case UserDataControlType::CHECK:
        {
            //Create control
            auto checkWindow = new wxCheckBox(parent, controlID++, wxEmptyString);
            this->windows.push_back(checkWindow);

            //Set default value
            if (!value.empty() && StringUtilities::ToBool(value))
                checkWindow->SetValue(true);

            parentSizer->Add(checkWindow, wxSizerFlags(0).Top().Border(wxTOP, 4));

            break;
        }
        case UserDataControlType::RGB_SWATCH:
        {
            FinjinColor color = StringUtilities::ToVector4(value, 1);
            auto colorWindow = new Finjin::Exporter::ColorPickerControl(parent, controlID++, color);
            this->windows.push_back(colorWindow);

            parentSizer->Add(colorWindow);
            
            break;
        }
        case UserDataControlType::RGBA_SWATCH:
        {
            FinjinColor color = StringUtilities::ToVector4(value, 1);
            auto colorWindow = new Finjin::Exporter::ColorPickerControl(parent, controlID++, color);
            this->windows.push_back(colorWindow);            
            parentSizer->Add(colorWindow);

            wxSize spinnerSize(ALPHA_EDIT_WIDTH + SpinnerControl::BUTTON_WIDTH, wxDefaultSize.y);
            auto id1 = controlID++;
            auto id2 = controlID++;
            auto spinner = new SpinnerControl(parent, id1, id2, wxDefaultPosition, spinnerSize, color.a, 0, 1, SpinnerControl::FLOAT_UNIT, 2);
            this->windows.push_back(spinner);
            parentSizer->Add(spinner, 0, wxALIGN_CENTER_VERTICAL);

            break;
        }
        case UserDataControlType::SPINNER:
        {
            //Parse values
            StringSplitter splitValues(value, wxT(FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_CHAR));

            //Create controls
            int editWidth = this->dataItem.editWidth > 0 ? this->dataItem.editWidth : EDIT_WIDTH;
            wxSize spinnerSize(editWidth + SpinnerControl::BUTTON_WIDTH, wxDefaultSize.y);
            for (int columnIndex = 0; columnIndex < this->dataItem.type.count; columnIndex++)
            {
                //Determine spinner unit type and default values
                auto unitType = SpinnerControl::FLOAT_UNIT;
                double value = 0;
                double minValue = -1000000;
                double maxValue = 1000000;                                
                if (this->dataItem.type.type == UserDataTypes::DataType::FLOAT_DATA_TYPE)
                {
                    if (columnIndex < (int)splitValues.size())
                        value = StringUtilities::ToFloat(splitValues[columnIndex]);

                    if (this->dataItem.minValue.floatValue <= this->dataItem.maxValue.floatValue)
                    {
                        minValue = this->dataItem.minValue.floatValue;
                        maxValue = this->dataItem.maxValue.floatValue;
                    }
                }
                else if (this->dataItem.type.type == UserDataTypes::DataType::INT_DATA_TYPE)
                {
                    if (columnIndex < (int)splitValues.size())
                        value = StringUtilities::ToInt(splitValues[columnIndex]);

                    unitType = SpinnerControl::INT_UNIT;
                    if (this->dataItem.minValue.intValue <= this->dataItem.maxValue.intValue)
                    {
                        minValue = this->dataItem.minValue.intValue;
                        maxValue = this->dataItem.maxValue.intValue;
                    }
                }

                //Create spinner control
                auto id1 = controlID++;
                auto id2 = controlID++;
                auto spinner = new SpinnerControl(parent, id1, id2, wxDefaultPosition, spinnerSize, value, minValue, maxValue, unitType);
                this->windows.push_back(spinner);
                
                parentSizer->Add(spinner, 0, wxALIGN_CENTER_VERTICAL);
            }
            
            break;
        }
        default: break;
    }
}

UserDataClassControls::ItemControls::~ItemControls()
{
    this->windows.clear();
}

bool UserDataClassControls::ItemControls::HasControlID(int id) const
{
    for (size_t i = 0; i < this->windows.size(); i++)
    {
        if (this->windows[i]->GetId() == id)
            return true;
    }
    return false;
}

wxString UserDataClassControls::ItemControls::GetEnumValue(int index) const
{
    int count = 0;

    for (auto item = this->enumItems.begin(); item != this->enumItems.end(); ++item)
    {
        if (count == index)
            return item->value;
        count++;
    }

    return wxEmptyString;
}

int UserDataClassControls::ItemControls::GetEnumIndex(const wxString& value) const
{
    int index = 0;

    for (auto item = this->enumItems.begin(); item != this->enumItems.end(); ++item)
    {
        if (item->value == value)
            return index;
        index++;
    }

    return 0;
}

bool UserDataClassControls::ItemControls::CheckVisibility()
{
    auto valueVisible = true;
    if (this->visibilityControl != nullptr)
    {
        //Check this object's visibility
        valueVisible = ((wxCheckBox*)this->visibilityControl->windows[0])->GetValue();
        if (valueVisible)
        {
            //Check parent's visibility
            valueVisible &= this->visibilityControl->CheckVisibility();
        }
    }

    return valueVisible;
}

void UserDataClassControls::ItemControls::UpdateVisibility()
{
    auto visible = CheckVisibility();

    //Label
    this->labelWindow->Show(visible);

    //Other controls
    for (size_t i = 0; i < this->windows.size(); i++)
        this->windows[i]->Show(visible);
}

bool UserDataClassControls::ItemControls::CausesVisibilityCircularReference(ItemControls* possibleReference) const
{
    auto parent = this->visibilityControl;
    while (parent != nullptr)
    {
        if (parent == possibleReference)
            return true;
        parent = parent->visibilityControl;
    }
    return false;
}
