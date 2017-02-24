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

    enum class StringSplitterFlags
    {
        /** Whitespace is trimmed from all the parsed strings. */
        TRIM = 1 << 0,

        /** Empty strings are ignored. */
        IGNORE_EMPTY = 1 << 1
    };
    FINJIN_ENUM_BITWISE_OPERATIONS(StringSplitterFlags)

    /** Class for splitting and rejoining strings. */
    class StringSplitter
    {        
    public:
        /** Default constructor. */
        StringSplitter();

        /** Constructor that splits the input text with a single delimiter. */
        StringSplitter(const wxString& text, wxChar delim, StringSplitterFlags flags = StringSplitterFlags::TRIM);

        /** Constructor that splits the input text with a string of delimiters. */
        StringSplitter(const wxString& text, const wxString& delims, StringSplitterFlags flags = StringSplitterFlags::TRIM);
        
        /** Finds the index of the line that matches the specified text. */
        int FindLine(const wxString& text, int startLine = 0) const;
      
        /** Gets the total length, in characters, of all strings. */
        size_t TotalLength() const;

        /** Gets the total length, in characters, of all strings, starting from the specified line. */
        size_t TotalLength(int startLine, int lineCount) const;

        /** Splits the input text with a single delimiter. */
        size_t Split(const wxString& text, wxChar delim, StringSplitterFlags flags = StringSplitterFlags::TRIM);

        /** Splits the input text with a string of delimiters. */
        size_t Split(const wxString& text, const wxString& delims, StringSplitterFlags flags = StringSplitterFlags::TRIM);
        
        /** 
         * Joins all the split strings.
         * @param joinChar [in] - The character used to join the strings.
         * @param startLine [in] - The index of the string to start with.
         * @param lineCount [in] - The number of lines to join, starting with startLine. If < 0, 
         * all lines starting from startLine are joined.
         * @return A new string with all the joined strings is returned.
         */
        wxString Join(wxChar joinChar, int startLine = 0, int lineCount = -1);

        /** 
         * Joins all the split strings.
         * @param joinText [in] - The string used to join the strings.
         * @param startLine [in] - The index of the string to start with.
         * @param lineCount [in] - The number of lines to join, starting with startLine. If < 0, 
         * all lines starting from startLine are joined.
         * @return A new string with all the joined strings is returned.
         */
        wxString Join(const wxString& joinText, int startLine = 0, int lineCount = -1);

        /** Removes the specified line. */
        void Remove(int line);

        /** Removes all the empty lines at the end of the split strings. */
        void RemoveTrailingEmptyLines();

        /** Gets the number of split strings. */
        size_t size() const {return this->items.size();}

        /** Gets the string at the specified index. */
        wxString& operator [] (size_t i) {return this->items[i];}

        /** Gets the string at the specified index. */
        const wxString& operator [] (size_t i) const {return this->items[i];}
            
        /** Gets all the split strings. */
        std::vector<wxString>& GetItems() {return this->items;}

        /** Gets all the split strings. */
        const std::vector<wxString>& GetItems() const {return this->items;}

    private:
        std::vector<wxString> items;
    };

} }
