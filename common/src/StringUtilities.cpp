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
#include "StringUtilities.hpp"

#if wxUSE_UNICODE
    typedef std::wstring stdstring;
    typedef std::wostringstream stdstringstream;
#else
    typedef std::string stdstring;
    typedef std::ostringstream stdstringstream;
#endif

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
bool StringUtilities::IsSameNoCase(const wxString& s1, const wxString& s2)
{
    return s1.IsSameAs(s2.wx_str(), false);
}

bool StringUtilities::IsSameNoCase(const wxString& s1, const wxChar* s2)
{
    return s1.IsSameAs(s2, false);
}

bool StringUtilities::IsSameNoCase(const wxChar* s1, const wxChar* s2)
{
    wxString wxs1(s1 ? s1 : wxEmptyString);
    return wxs1.IsSameAs(s2, false);
}

bool StringUtilities::StartsWithNoCase(const wxString& s1, const wxString& s2)
{
    size_t s1Length = s1.length();
    size_t s2Length = s2.length();
    if (s2Length > s1Length)
        return false;
    else if (s2Length == s1Length)
        return s1.IsSameAs(s2, false);
    else
    {
        wxString start = s1.SubString(0, s2Length - 1);
        return start.IsSameAs(s2, false);
    }
}

bool StringUtilities::EndsWithNoCase(const wxString& s1, const wxString& s2)
{
    size_t s1Length = s1.length();
    size_t s2Length = s2.length();
    if (s2Length > s1Length)
        return false;
    else if (s2Length == s1Length)
        return s1.IsSameAs(s2, false);
    else
    {
        wxString end = s1.Mid(s1Length - s2Length);
        return end.IsSameAs(s2, false);
    }
}

int StringUtilities::FindDivide(const wxString& s)
{
    size_t divideIndex = s.find('/');
    if (divideIndex == wxString::npos)
        divideIndex = s.find('\\');
    return (divideIndex == wxString::npos) ? -1 : (int)divideIndex;
}

wxString StringUtilities::ToString(int value)
{
    wxString str = wxString::Format(wxT("%d"), value);
    return str;
}

#ifndef _SIZE_T_DEFINED
wxString StringUtilities::ToString(size_t value)
{
    wxString str = wxString::Format(wxT("%u"), (unsigned int)value);
    return str;
}
#endif

wxString StringUtilities::ToString(unsigned int value)
{
    wxString str = wxString::Format(wxT("%u"), value);
    return str;
}

wxString StringUtilities::ToString(bool value)
{
    return value ? wxT("true") : wxT("false");
}

wxString StringUtilities::ToString(float value, size_t precision)
{
    stdstringstream stream;
    stream.precision((std::streamsize)precision);
    stream << value;
    stdstring valueText = stream.str();
    return valueText.c_str();
}

wxString StringUtilities::ToString(double value, size_t precision)
{
    stdstringstream stream;
    stream.precision((std::streamsize)precision);
    stream << value;
    stdstring valueText = stream.str();
    return valueText.c_str();
}

wxString StringUtilities::ToString(FinjinColor value, bool includeAlpha)
{
    stdstringstream stream;
    stream << value.r << FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_CHAR << value.g << FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_CHAR << value.b;
    if (includeAlpha)
        stream << FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_CHAR << value.a;
    stdstring valueText = stream.str();
    return valueText.c_str();
}

wxString StringUtilities::ToString(FinjinVector3 value)
{
    stdstringstream stream;
    stream << value.x << FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_CHAR << value.y << FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_CHAR << value.z;
    stdstring valueText = stream.str();
    return valueText.c_str();
}

wxString StringUtilities::ToString(FinjinVector4 value)
{
    stdstringstream stream;
    stream << value.x << FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_CHAR << value.y << FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_CHAR << value.z << FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_CHAR << value.w;
    stdstring valueText = stream.str();
    return valueText.c_str();
}

wxString StringUtilities::ToString(const std::vector<int>& values, const wxString& joinText)
{
    stdstringstream stream;
    for (size_t i = 0; i < values.size(); i++)
    {
        stream << values[i];
        if (i < values.size() - 1)
            stream << joinText.wx_str();
    }
    stdstring valueText = stream.str();
    return valueText.c_str();
}

wxString StringUtilities::ToString(const std::vector<float>& values, const wxString& joinText)
{
    stdstringstream stream;
    for (size_t i = 0; i < values.size(); i++)
    {
        stream << values[i];
        if (i < values.size() - 1)
            stream << joinText.wx_str();
    }
    stdstring valueText = stream.str();
    return valueText.c_str();
}

wxString StringUtilities::ToString(const std::vector<double>& values, const wxString& joinText)
{
    stdstringstream stream;
    for (size_t i = 0; i < values.size(); i++)
    {
        stream << values[i];
        if (i < values.size() - 1)
            stream << joinText.wx_str();
    }
    stdstring valueText = stream.str();
    return valueText.c_str();
}

wxString StringUtilities::ToRemoveXYZString(FinjinVector3 mask)
{
    wxString text;
    if (mask.x == 0) text += wxT("x");
    if (mask.y == 0) text += wxT("y");
    if (mask.z == 0) text += wxT("z");
    return text;
}

FinjinVector3 StringUtilities::ToRemoveXYZVector3(const wxString& text)
{
    FinjinVector3 mask;
    mask.x = (text.find(wxT('x')) != (size_t)-1) ? 0 : 1;
    mask.y = (text.find(wxT('y')) != (size_t)-1) ? 0 : 1;
    mask.z = (text.find(wxT('z')) != (size_t)-1) ? 0 : 1;
    return mask;
}

bool StringUtilities::ToBool(const wxString& value)
{
    if (value.empty() ||
        value == wxT("f") ||
        value == wxT("F") ||
        value == wxT("false") ||
        value == wxT("no") ||
        value == wxT("0"))
    {
        return false;
    }
    else
        return true;
}

double StringUtilities::ToDouble(const wxString& value)
{
    return wxAtof(value.wx_str()); //wxAtof returns a double
}

float StringUtilities::ToFloat(const wxString& value)
{
    return wxAtof(value.wx_str());
}

unsigned int StringUtilities::ToUInt(const wxString& value)
{
    return (unsigned int)ToInt(value.wx_str());
}

int StringUtilities::ToInt(const wxString& value)
{
    return wxAtoi(value);
}

template <class T, int elementCount>
T ToVector(const wxString& text, float defaultValue)
{
    T v(defaultValue);

    if (!text.empty())
    {
        int i = 0;
        wxStringTokenizer split(text, wxT(","));
        while (split.HasMoreTokens() && i < elementCount)
        {
            auto token = split.GetNextToken();
            StringUtilities::TrimWhitespace(token);
            ((float*)&v)[i++] = StringUtilities::ToFloat(token);
        }
    }

    return v;
}

FinjinVector2 StringUtilities::ToVector2(const wxString& text, float defaultValue)
{
    return ToVector<FinjinVector2, 2>(text, defaultValue);
}

FinjinVector3 StringUtilities::ToVector3(const wxString& text, float defaultValue)
{
    return ToVector<FinjinVector3, 3>(text, defaultValue);
}

FinjinVector4 StringUtilities::ToVector4(const wxString& text, float defaultValue)
{
    return ToVector<FinjinVector4, 4>(text, defaultValue);
}

void StringUtilities::TrimWhitespace(wxString& text)
{
    text.Trim(true);
    text.Trim(false);
}

bool StringUtilities::IsNewline(wxChar c)
{
    return c == wxT('\r') || c == wxT('\n');
}

int StringUtilities::FindNextLineStart(const wxString& text, int start, bool skipExtraNewlines)
{
    int index = start;
    int length = (int)text.length();

    //Skip past all non-newline characters
    while (index < length && !IsNewline(text[index]))
        index++;

    //Skip all the remaining newlines
    while (index < length && IsNewline(text[index]))
    {
        index++;

        if (!skipExtraNewlines && text[index - 1] == wxT('\n'))
        {
            //Caller wants to quit after the first newline is found
            break;
        }
    }

    return index;
}

bool StringUtilities::ParseKeyValueLine(const wxString& text, int start, int& nextStart, wxString& key, wxString& value, bool skipExtraNewlines)
{
    int index = start;
    int length = (int)text.length();

    //Exit early if the end has already been reached, the leading text is whitespace,
    //or the leading text is a # (comment character)
    if (index >= length || isspace(text[start]) || text[start] == wxT('#'))
    {
        nextStart = FindNextLineStart(text, index, skipExtraNewlines);
        return false;
    }

    //Find the key. This is the text to the left of the '=' character
    while (index < length && !IsNewline(text[index]))
    {
        if (text[index] == wxT('='))
            break;
        index++;
    }

    //A key has been found if the current index has moved past the start index,
    //the index didn't reach the end of the text, and the current character is the
    //'=' character
    if (index == start || index == length || text[index] != wxT('='))
    {
        //No key found
        nextStart = FindNextLineStart(text, index, skipExtraNewlines);
        return false;
    }
    else
    {
        //A key was found
        key = text.substr(start, index - start);
        TrimWhitespace(key);
    }

    //If we're this far, we have the index of the '=' character. Everything to the right is the value
    index++; //Skip the '=' character
    int valueStart = index;
    while (index < length && !IsNewline(text[index]))
        index++;

    //Get the value
    value = text.substr(valueStart, index - valueStart + 1);
    TrimWhitespace(value);

    //Get next start location
    nextStart = FindNextLineStart(text, index, skipExtraNewlines);

    return true;
}

bool StringUtilities::ParseKeyValueLine(const wxString& text, wxString& key, wxString& value)
{
    int nextIndex;
    return ParseKeyValueLine(text, 0, nextIndex, key, value);
}

bool StringUtilities::ParsePrefixedKeyValueLine(const wxString& text, wxString& key, wxString& value, wxString* prefix)
{
    int index = 0;
    int length = (int)text.length();

    //Move to first space
    while (index < length && !isspace(text[index]))
        index++;
    if (index >= length)
        return false;

    if (prefix != nullptr)
        *prefix = text.substr(0, index);

    //Move to first non-space
    while (index < length && isspace(text[index]))
        index++;
    if (index >= length)
        return false;

    int nextIndex;
    return ParseKeyValueLine(text, index, nextIndex, key, value);
}

int StringUtilities::ParseArrayIndex(const wxString& s)
{
    int index = -1;

    size_t openBracket = s.find_first_of(wxT('['));
    if (openBracket != wxString::npos)
    {
        size_t closeBracket = s.find_first_of(wxT(']'), openBracket + 1);
        if (closeBracket != wxString::npos)
        {
            wxString indexString = s.SubString(openBracket + 1, closeBracket - 1);

            long lValue;
            if (indexString.ToLong(&lValue))
                index = (int)lValue;
        }
    }

    return index;
}

void StringUtilities::AppendKeyAndValue(wxString& text, const wxString& key, const wxString& value, const wxString& newline)
{
    text += key;
    text += wxT("=");
    text += value;
    text += newline;
}

void StringUtilities::AppendPrefixedKeyAndValue(wxString& text, const wxString& prefix, const wxString& key, const wxString& value, const wxString& newline)
{
    text += prefix;
    text += wxT(" ");
    AppendKeyAndValue(text, key, value, newline);
}

void StringUtilities::RemoveSpace(wxString& text)
{
    std::vector<wxChar> chars;
    chars.resize(text.length() + 1);

    int outputLength = 0;
    for (size_t i = 0; i < text.length(); i++)
    {
        if (!isspace(text[i]))
            chars[outputLength++] = text[i];
    }
    chars[outputLength] = 0;
    text = &chars[0];
}

bool StringUtilities::IsInteger(const wxString& text)
{
    if (text.empty())
        return false;
    else
    {
        for (size_t charIndex = 0; charIndex < text.length(); charIndex++)
        {
            if (!isdigit(text[charIndex]))
                return false;
        }

        return true;
    }
}

bool StringUtilities::IsVersion(const wxString& version)
{
    int periodFoundAt = version.Find(wxT('.')) > 0;
    if (periodFoundAt > 0)
    {
        wxString first = version.Left(periodFoundAt);
        wxString last = version.Mid(periodFoundAt + 1);
        return IsInteger(first) && IsInteger(last);
    }
    else if (StringUtilities::IsInteger(version))
        return true;

    return false;
}

wxString StringUtilities::FormatVersion(int major, int minor)
{
    wxString version = StringUtilities::ToString(major);
    version += wxT('.');
    version += StringUtilities::ToString(minor);
    return version;
}

void StringUtilities::RemoveName(std::vector<wxString>& names, const wxString& nameToRemove)
{
    for (size_t i = 0; i < names.size(); i++)
    {
        if (IsSameNoCase(names[i], nameToRemove))
        {
            names.erase(names.begin() + i);
            break;
        }
    }
}

bool StringUtilities::ContainsName(const std::vector<wxString>& names, const wxString& name)
{
    for (size_t i = 0; i < names.size(); i++)
    {
        if (IsSameNoCase(names[i], name))
            return true;
    }
    return false;
}

bool StringUtilities::NameHasColor(const wxString& name)
{
    wxString nameLower = name;
    nameLower.MakeLower();
    return
        nameLower.Find(wxT("color")) >= 0 ||
        nameLower.Find(wxT("colour")) >= 0;
}

void StringUtilities::Replace(std::wstring& text, const wchar_t* findText, const wchar_t* replaceText)
{
    size_t findLength = wcslen(findText);
    size_t foundAt = text.find(findText);
    while (foundAt != std::wstring::npos)
    {
        text.replace(foundAt, findLength, replaceText);
        foundAt = text.find(findText, foundAt);
    }
}
