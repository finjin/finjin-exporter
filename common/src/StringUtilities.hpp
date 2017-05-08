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
#include "FinjinColor.hpp"
#include "FinjinVector.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Varioius string utility functions. */
    class StringUtilities
    {
    public:
        /** Determines if two strings are the same, ignore case sensitivity. */
        static bool IsSameNoCase(const wxString& s1, const wxString& s2);

        /** Determines if two strings are the same, ignore case sensitivity. */
        static bool IsSameNoCase(const wxString& s1, const wxChar* s2);

        /** Determines if two strings are the same, ignore case sensitivity. */
        static bool IsSameNoCase(const wxChar* s1, const wxChar* s2);

        /** Performs a case-insensitive comparison to determine if s1 starts with s2. */
        static bool StartsWithNoCase(const wxString& s1, const wxString& s2);

        /** Performs a case-insensitive comparison to determine if s1 ends with s2. */
        static bool EndsWithNoCase(const wxString& s1, const wxString& s2);

        /** Finds the index of the 'divide' character in the specified string. */
        static int FindDivide(const wxString& s);

        /** Converts an integer to a string. */
        static wxString ToString(int value);

#ifndef _SIZE_T_DEFINED
        /** Converts a size_t to a string. */
        static wxString ToString(size_t value);
#endif

        /** Converts a unsigned int to a string. */
        static wxString ToString(unsigned int value);

        /** Converts a bool to a string. */
        static wxString ToString(bool value);

        /** Converts a float to a string. */
        static wxString ToString(float value, size_t precision = 6);

        /** Converts a double to a string. */
        static wxString ToString(double value, size_t precision = 6);

        /** Converts a color to a string. */
        static wxString ToString(FinjinColor value, bool includeAlpha = false);

        /** Converts a 3-float to a string. */
        static wxString ToString(FinjinVector3 value);

        /** Converts a 4-float to a string. */
        static wxString ToString(FinjinVector4 value);

        /** Converts an array of integers to a string. */
        static wxString ToString(const std::vector<int>& values, const wxString& joinText = wxT(FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_STRING));

        /** Converts an array of floats to a string. */
        static wxString ToString(const std::vector<float>& values, const wxString& joinText = wxT(FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_STRING));

        /** Converts an array of doubles to a string. */
        static wxString ToString(const std::vector<double>& values, const wxString& joinText = wxT(FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_STRING));

        /** Converts an 3-float to a string that is used to indicate which X/Y/Z components to remove. */
        static wxString ToRemoveXYZString(FinjinVector3 mask);

        /** Converts a string that is used to indicate which X/Y/Z components to remove to a 3-float. */
        static FinjinVector3 ToRemoveXYZVector3(const wxString& text);

        /**
         * Converts a string to a boolean value.
         * @param value [in] - The string to convert to a boolean. The string is case insensitive
         * @return If the input string is 'false', 'no' or '0', false is returned.
         * Otherwise true is returned.
         */
        static bool ToBool(const wxString& value);

        /** Converts a string to double value. */
        static double ToDouble(const wxString& value);

        /** Converts a string to float value. */
        static float ToFloat(const wxString& value);

        /** Converts a string to an unsigned int value. */
        static unsigned int ToUInt(const wxString& value);

        /** Converts a string to an int value. */
        static int ToInt(const wxString& value);

        /** Converts a string to an 2-float value. */
        static FinjinVector2 ToVector2(const wxString& text, float defaultValue = 0);

        /** Converts a string to an 3-float value. */
        static FinjinVector3 ToVector3(const wxString& text, float defaultValue = 0);

        /** Converts a string to an 4-float value. */
        static FinjinVector4 ToVector4(const wxString& text, float defaultValue = 0);

        /** Trims whitespace from the start and end of the specified string */
        static void TrimWhitespace(wxString& text);

        /**
         * Determines if the specified character is a newline character
         * Newline characters are \r (0x0d) and \n (0x0a)
         */
        static bool IsNewline(wxChar c);

        /**
         * Finds the index of the first character in the next new line of text.
         * @param text [in] - The text to search
         * @param start [in] - The start index at which searching begins
         * @return The index of the first character in the next new line. If no newline
         * could be found, the length of the text is returned.
         */
        static int FindNextLineStart(const wxString& text, int start, bool skipExtraNewlines = true);

        /**
         * Parses a key=value pair from the line starting at 'start'
         * @param text [in] - The text from which the key=value pair is parsed
         * @param start [in] - The start index in the text
         * @param nextStart [out] - The index of the next line
         * @param key [out] - The parsed key, with its whitespace trimmed
         * @param value [out] - The parsed value, with its whitespace trimmed
         * @return Returns true if a key=value pair was found, false otherwise. Regardless,
         * the nextStart output parameter is always set.
         */
        static bool ParseKeyValueLine(const wxString& text, int start, int& nextStart, wxString& key, wxString& value, bool skipExtraNewlines = true);
        static bool ParseKeyValueLine(const wxString& text, wxString& key, wxString& value);

        static bool ParsePrefixedKeyValueLine(const wxString& text, wxString& key, wxString& value, wxString* prefix = nullptr);

        static int ParseArrayIndex(const wxString& s);

        static void AppendKeyAndValue(wxString& text, const wxString& key, const wxString& value, const wxString& newline);
        static void AppendPrefixedKeyAndValue(wxString& text, const wxString& prefix, const wxString& key, const wxString& value, const wxString& newline);

        /** Removes all whitespace from the specified text. */
        static void RemoveSpace(wxString& text);

        /** Determines if the specified text contains only integers. */
        static bool IsInteger(const wxString& text);

        /** Determines if the specified text contains a version of the form '1' or '1.2'. */
        static bool IsVersion(const wxString& version);

        /** Formats a string in the form of '1.2'. */
        static wxString FormatVersion(int major, int minor);

        /**
         * Remove the first instance of the name
         * The string comparison is case insensitive
         */
        static void RemoveName(std::vector<wxString>& names, const wxString& nameToRemove);

        /**
         * Determines whether the collection of names contains the specific name
         * The string comparison is case insensitive
         * @param names [in] - The collection to search
         * @param name [in] - The name to search for
         */
        static bool ContainsName(const std::vector<wxString>& names, const wxString& name);

        /** Determines whether the specified name contains 'color' or 'colour' (case-insensitive) */
        static bool NameHasColor(const wxString& name);

        /**
         * Performs a find/replace on the specified text.
         * @param text [in/out] - The text to perform the find/replace on.
         * @param findText [in] - The text to find.
         * @param replaceText [in] - The text that is replaced.
         */
        static void Replace(std::wstring& text, const wchar_t* findText, const wchar_t* replaceText);
    };

} }
