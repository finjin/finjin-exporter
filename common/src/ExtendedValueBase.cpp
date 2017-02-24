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
#include "ExtendedValueBase.hpp"
#include "StringUtilities.hpp"

using namespace Finjin::Exporter;


//Locals-----------------------------------------------------------------------
static const wxString EXTENDED_INT_TEXT = wxT("int");
static const wxString EXTENDED_FLOAT_TEXT = wxT("float");
static const wxString EXTENDED_COLOR_TEXT = wxT("color");
static const wxString EXTENDED_STRING_TEXT = wxT("string");
static const wxString EXTENDED_OBJECT_TEXT = wxT("object");
static const wxString EXTENDED_MATERIAL_TEXT = wxT("material");


//Implementation---------------------------------------------------------------
ExtendedValueBase::ExtendedValueBase()
{
    this->type = EXTENDED_NONE;
    this->count = 0;    
}

ExtendedValueBase::Type ExtendedValueBase::GetType() const
{
    return this->type;
}

wxString ExtendedValueBase::GetTypeString() const
{
    wxString result;

    switch (this->type)
    {
        case EXTENDED_INT: result = EXTENDED_INT_TEXT; break;
        case EXTENDED_FLOAT: result = EXTENDED_FLOAT_TEXT; break;
        case EXTENDED_COLOR: result = EXTENDED_COLOR_TEXT; break;
        case EXTENDED_STRING: result = EXTENDED_STRING_TEXT; break;
        case EXTENDED_OBJECT: result = EXTENDED_OBJECT_TEXT; break;
        case EXTENDED_MATERIAL: result = EXTENDED_MATERIAL_TEXT; break;
        default: break;
    }

    return result;
}

ExtendedValueBase::Type ExtendedValueBase::ParseType(const wxString& text)
{
    Type type = EXTENDED_NONE;

    if (text == EXTENDED_INT_TEXT)
        type = EXTENDED_INT;
    else if (text == EXTENDED_FLOAT_TEXT)
        type = EXTENDED_FLOAT;
    else if (text == EXTENDED_COLOR_TEXT)
        type = EXTENDED_COLOR;
    else if (text == EXTENDED_STRING_TEXT)
        type = EXTENDED_STRING;
    else if (text == EXTENDED_OBJECT_TEXT)
        type = EXTENDED_OBJECT;
    else if (text == EXTENDED_MATERIAL_TEXT)
        type = EXTENDED_MATERIAL;
    
    return type;
}

int ExtendedValueBase::GetCount() const
{
    if (this->type == EXTENDED_COLOR)
        return this->count / 4;
    else
        return this->count;
}

void ExtendedValueBase::Get(int& value) const
{
    int count = 1;
    Get(&value, count);
    if (count == 0)
        value = 0;
}

void ExtendedValueBase::Get(int* values, int& count) const
{
    if (this->type == EXTENDED_INT || this->type == EXTENDED_FLOAT || this->type == EXTENDED_COLOR)
    {
        count = std::min(count, this->count);
        for (int i = 0; i < count; i++)
            values[i] = RoundToInt(this->floats[i]);
    }
    else if (this->type == EXTENDED_STRING)
    {
        count = std::min(count, this->count);
        for (int i = 0; i < count; i++)
            values[i] = StringUtilities::ToInt(this->strings[i]);
    }
    else
        count = 0;
}

void ExtendedValueBase::Get(std::vector<int>& values) const
{
    int count = GetCount();
    values.resize(count);
    if (count > 0)
        Get(values.data(), count);
}

void ExtendedValueBase::Get(float& value) const
{
    int count = 1;
    Get(&value, count);
    if (count == 0)
        value = 0;
}

void ExtendedValueBase::Get(float* values, int& count) const
{
    if (this->type == EXTENDED_INT || this->type == EXTENDED_FLOAT || this->type == EXTENDED_COLOR)
    {
        count = std::min(count, this->count);
        memcpy(values, &this->floats[0], count * sizeof(float));
    }
    else if (this->type == EXTENDED_STRING)
    {
        count = std::min(count, this->count);
        for (int i = 0; i < count; i++)
            values[i] = StringUtilities::ToFloat(this->strings[i]);
    }
    else
        count = 0;
}

void ExtendedValueBase::Get(std::vector<float>& values) const
{
    int count = GetCount();
    values.resize(count);
    if (count > 0)
        Get(values.data(), count);
}

void ExtendedValueBase::Get(FinjinColor& value) const
{
    int count = 1;
    Get(&value, count);
    if (count == 0)
        value.Set(0, 0, 0, 1);
}

void ExtendedValueBase::Get(FinjinColor* values, int& count) const
{
    if (this->type == EXTENDED_INT || this->type == EXTENDED_FLOAT || this->type == EXTENDED_COLOR)
    {
        int actualCount = this->count / 4;
        count = std::min(count, actualCount);
        for (int i = 0; i < count; i++)
        {
            values[i].r = this->floats[i*4 + 0];
            values[i].g = this->floats[i*4 + 1];
            values[i].b = this->floats[i*4 + 2];
            values[i].a = this->floats[i*4 + 3];
        }
    }
    else
        count = 0;
}

void ExtendedValueBase::Get(std::vector<FinjinColor>& values) const
{
    int count = GetCount();
    values.resize(count);
    if (count > 0)
        Get(values.data(), count);
}

void ExtendedValueBase::Get(wxString& value) const
{
    value = ToString(false);
}

void ExtendedValueBase::Get(wxString* values, int& count) const
{
    switch (this->type)
    {
        case EXTENDED_STRING:
        {
            count = std::min(count, this->count);
            for (int i = 0; i < this->count; i++)
                values[i] = this->strings[i];
            break;
        }
        case EXTENDED_INT:
        {
            count = std::min(count, this->count);
            for (int i = 0; i < this->count; i++)
                values[i] = wxString::Format(wxT("%d"), RoundToInt(this->floats[i]));

            break;
        }
        case EXTENDED_FLOAT:
        case EXTENDED_COLOR:
        {
            count = std::min(count, this->count);
            for (int i = 0; i < this->count; i++)
                values[i] = wxString::Format(wxT("%f"), this->floats[i]);

            break;
        }
        case EXTENDED_OBJECT:
        {
            count = std::min(count, this->count);
            for (int i = 0; i < this->count; i++)
                values[i] = this->objects[i].GetLocalName();
            break;
        }
        case EXTENDED_MATERIAL:
        {
            count = std::min(count, this->count);
            for (int i = 0; i < this->count; i++)
                values[i] = this->materials[i].GetLocalName();
            break;
        }
        default:
        {
            count = 0;
            break;
        }
    }
}

void ExtendedValueBase::Get(std::vector<wxString>& values) const
{
    int count = GetCount();
    values.resize(count);
    if (count > 0)
        Get(values.data(), count);
}

void ExtendedValueBase::Get(ObjectAccessor& value) const
{
    int count = 1;
    Get(&value, count);
    if (count == 0)
        value = ObjectAccessor();
}

void ExtendedValueBase::Get(ObjectAccessor* values, int& count) const
{
    if (this->type == EXTENDED_OBJECT)
    {
        count = std::min(count, this->count);
        for (int i = 0; i < this->count; i++)
            values[i] = this->objects[i];
    }
    else if (this->type == EXTENDED_STRING)
    {
        count = std::min(count, this->count);
        for (int i = 0; i < this->count; i++)
            values[i] = ObjectAccessor::GetObjectByName(this->strings[i]);
    }
    else
        count = 0;
}

void ExtendedValueBase::Get(std::vector<ObjectAccessor>& values) const
{
    int count = GetCount();
    values.resize(count);
    if (count > 0)
        Get(values.data(), count);
}

void ExtendedValueBase::Get(MaterialAccessor& value) const
{
    int count = 1;
    Get(&value, count);
    if (count == 0)
        value = MaterialAccessor();
}

void ExtendedValueBase::Get(MaterialAccessor* values, int& count) const
{
    if (this->type == EXTENDED_MATERIAL)
    {
        count = std::min(count, this->count);
        for (int i = 0; i < this->count; i++)
            values[i] = this->materials[i];
    }
    else if (this->type == EXTENDED_STRING)
    {
        count = std::min(count, this->count);
        for (int i = 0; i < this->count; i++)
            values[i] = MaterialAccessor::GetMaterialByName(this->strings[i]);
    }
    else
        count = 0;
}

void ExtendedValueBase::Get(std::vector<MaterialAccessor>& values) const
{
    int count = GetCount();
    values.resize(count);
    if (count > 0)
        Get(values.data(), count);
}

void ExtendedValueBase::Set(int value)
{
    Set(&value, 1);
}

void ExtendedValueBase::SetNone()
{
    this->type = EXTENDED_NONE;
    this->count = 0;
    Clean();
}

void ExtendedValueBase::Set(const int* values, int count)
{
    if (values != nullptr && count > 0)
    {
        this->type = EXTENDED_INT;
        this->count = count;
        if (this->count > (int)this->floats.size())
            this->floats.resize(this->count);
        for (int i = 0; i < this->count; i++)
            this->floats[i] = values[i];
    }
    else
    {
        this->type = EXTENDED_NONE;
        this->count = 0;
    }
    Clean();
}

void ExtendedValueBase::Set(const std::vector<int>& values)
{
    Set(values.empty() ? nullptr : values.data(), (int)values.size());
}

void ExtendedValueBase::Set(float value)
{
    Set(&value, 1);
}

void ExtendedValueBase::Set(const float* values, int count)
{
    if (values != nullptr && count > 0)
    {
        this->type = EXTENDED_FLOAT;
        this->count = count;
        if (this->count > (int)this->floats.size())
            this->floats.resize(this->count);
        memcpy(&this->floats[0], values, this->count * sizeof(float));
    }
    else
    {
        this->type = EXTENDED_NONE;
        this->count = 0;
    }
    Clean();
}

void ExtendedValueBase::Set(const std::vector<float>& values)
{
    Set(values.empty() ? nullptr : values.data(), (int)values.size());
}

void ExtendedValueBase::Set(const FinjinColor& value)
{
    Set(&value, 1);
}

void ExtendedValueBase::Set(const FinjinColor* values, int count)
{
    if (values != nullptr && count > 0)
    {
        this->type = EXTENDED_COLOR;
        this->count = count * 4;
        if (this->count > (int)this->floats.size())
            this->floats.resize(this->count);
        for (int i = 0; i < count; i++)
        {
            this->floats[i*4 + 0] = values[i].r;
            this->floats[i*4 + 1] = values[i].g;
            this->floats[i*4 + 2] = values[i].b;
            this->floats[i*4 + 3] = values[i].a;
        }
    }
    else
    {
        this->type = EXTENDED_NONE;
        this->count = 0;
    }
    Clean();
}

void ExtendedValueBase::Set(const std::vector<FinjinColor>& values)
{
    Set(values.empty() ? nullptr : values.data(), (int)values.size());
}

void ExtendedValueBase::Set(const wxString& value)
{
    Set(&value, 1);
}

void ExtendedValueBase::Set(const wxString* values, int count)
{
    if (values != nullptr && count > 0)
    {
        this->type = EXTENDED_STRING;
        this->count = count;
        if (this->count > (int)this->strings.size())
            this->strings.resize(this->count);
        for (int i = 0; i < this->count; i++)
            this->strings[i] = values[i];
    }
    else
    {
        this->type = EXTENDED_NONE;
        this->count = 0;
    }
    Clean();
}

void ExtendedValueBase::Set(const std::vector<wxString>& values)
{
    Set(values.empty() ? nullptr : values.data(), (int)values.size());
}

void ExtendedValueBase::Set(ObjectAccessor value)
{
    Set(&value, 1);
}

void ExtendedValueBase::Set(const ObjectAccessor* values, int count)
{
    if (values != nullptr && count > 0)
    {
        this->type = EXTENDED_OBJECT;
        this->count = count;
        if (this->count > (int)this->objects.size())
            this->objects.resize(this->count);
        for (int i = 0; i < this->count; i++)
            this->objects[i] = values[i];
    }
    else
    {
        this->type = EXTENDED_NONE;
        this->count = 0;
    }
    Clean();
}

void ExtendedValueBase::Set(const std::vector<ObjectAccessor>& values)
{
    Set(values.empty() ? nullptr : values.data(), (int)values.size());
}

void ExtendedValueBase::Set(MaterialAccessor value)
{
    Set(&value, 1);
}

void ExtendedValueBase::Set(const MaterialAccessor* values, int count)
{
    if (values != nullptr && count > 0)
    {
        this->type = EXTENDED_MATERIAL;
        this->count = count;
        if (this->count > (int)this->materials.size())
            this->materials.resize(this->count);
        for (int i = 0; i < this->count; i++)
            this->materials[i] = values[i];
    }
    else
    {
        this->type = EXTENDED_NONE;
        this->count = 0;
    }
    Clean();
}

void ExtendedValueBase::Set(const std::vector<MaterialAccessor>& values)
{
    Set(values.empty() ? nullptr : values.data(), (int)values.size());
}

wxString ExtendedValueBase::ToString(bool addPrefix) const
{
    wxString s;

    switch (this->type)
    {
        case EXTENDED_INT:
        {
            if (addPrefix)
                s = wxString::Format(wxT("%s[%d]: "), EXTENDED_INT_TEXT.wx_str(), this->count);
            for (int i = 0; i < this->count; i++)
            {
                if (i > 0)
                    s += wxT(",");

                s += wxString::Format(wxT("%d"), RoundToInt(this->floats[i]));
            }
            break;
        }
        case EXTENDED_FLOAT:
        {
            if (addPrefix)
                s = wxString::Format(wxT("%s[%d]: "), EXTENDED_FLOAT_TEXT.wx_str(), this->count);
            
            for (int i = 0; i < this->count; i++)
            {
                if (i > 0)
                    s += wxT(",");

                s += wxString::Format(wxT("%f"), this->floats[i]);
            }
            break;
        }
        case EXTENDED_COLOR:
        {
            int colorCount = this->count / 4;

            if (addPrefix)
                s = wxString::Format(wxT("%s[%d]: "), EXTENDED_COLOR_TEXT.wx_str(), colorCount);

            for (int i = 0; i < colorCount; i++)
            {
                if (i > 0)
                    s += wxT(",");

                s += wxString::Format
                    (
                    wxT("(%f,%f,%f,%f)"), 
                    this->floats[i*4 + 0],
                    this->floats[i*4 + 1],
                    this->floats[i*4 + 2],
                    this->floats[i*4 + 3]
                    );
            }
            break;
        }
        case EXTENDED_STRING:
        {
            if (addPrefix)
                s = wxString::Format(wxT("%s[%d]: "), EXTENDED_STRING_TEXT.wx_str(), this->count);

            for (int i = 0; i < this->count; i++)
            {
                if (i > 0)
                    s += wxT(",");

                s += this->strings[i];
            }
            break;
        }
        case EXTENDED_OBJECT:
        {
            if (addPrefix)
                s = wxString::Format(wxT("%s[%d]: "), EXTENDED_STRING_TEXT.wx_str(), this->count);
            
            for (int i = 0; i < this->count; i++)
            {
                if (i > 0)
                    s += wxT(",");

                s += this->objects[i].GetLocalName();
            }
            break;
        }
        case EXTENDED_MATERIAL:
        {
            if (addPrefix)
                s = wxString::Format(wxT("%s[%d]: "), EXTENDED_MATERIAL_TEXT.wx_str(), this->count);
            
            for (int i = 0; i < this->count; i++)
            {
                if (i > 0)
                    s += wxT(",");

                s += this->materials[i].GetLocalName();
            }
            break;
        }
        default: break;
    }

    return s;
}

void ExtendedValueBase::Clean()
{
    if (this->type != EXTENDED_INT && this->type != EXTENDED_FLOAT && this->type != EXTENDED_COLOR)
        this->floats.clear();
    if (this->type != EXTENDED_STRING)
        this->strings.clear();
    if (this->type != EXTENDED_OBJECT)
        this->objects.clear();
    if (this->type != EXTENDED_MATERIAL)
        this->materials.clear();
}
