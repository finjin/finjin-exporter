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
#include "StringSplitter.hpp"
#include "StringUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
StringSplitter::StringSplitter()
{    
}

StringSplitter::StringSplitter(const wxString& text, wxChar delim, StringSplitterFlags flags)
{
    Split(text, delim, flags);
}

StringSplitter::StringSplitter(const wxString& text, const wxString& delims, StringSplitterFlags flags)
{
    Split(text, delims, flags);
}

int StringSplitter::FindLine(const wxString& text, int startLine) const
{
    for (int index = startLine; index < (int)this->items.size(); index++)
    {
        if (this->items[index] == text)
            return index;
    }

    return -1;
}

size_t StringSplitter::TotalLength() const
{
    size_t length = 0;

    for (size_t index = 0; index < this->items.size(); index++)
        length += this->items[index].Length();
    
    return length;
}

size_t StringSplitter::TotalLength(int startLine, int lineCount) const
{
    size_t length = 0;

    for (int index = startLine; index < startLine + lineCount; index++)
        length += this->items[index].Length();
    
    return length;
}

size_t StringSplitter::Split(const wxString& text, wxChar delim, StringSplitterFlags flags)
{
    const wxChar delims[] = {delim, 0};
    return Split(text, delims, flags);    
}

size_t StringSplitter::Split(const wxString& text, const wxString& delims, StringSplitterFlags flags)
{
    this->items.clear();

    wxStringTokenizer tokens(text, delims);
    this->items.reserve(tokens.CountTokens());
    while (tokens.HasMoreTokens())
    {
        auto token = tokens.GetNextToken();
        if (AnySet(flags & StringSplitterFlags::TRIM))
            StringUtilities::TrimWhitespace(token);    
        if (!token.empty() || NoneSet(flags & StringSplitterFlags::IGNORE_EMPTY))
            this->items.push_back(token);
    }            

    return this->items.size();
}

wxString StringSplitter::Join(wxChar joinChar, int startLine, int lineCount)
{
    wxChar joinText[2] = {joinChar, 0};
    return Join(joinText, startLine, lineCount);
}

wxString StringSplitter::Join(const wxString& joinText, int startLine, int lineCount)
{
    wxString joined;

    if (lineCount < 0)
        lineCount = (int)this->items.size();

    if (lineCount > 0 && startLine < (int)this->items.size())
    {
        joined += this->items[startLine];

        for (int index = startLine + 1; 
            index < startLine + lineCount && index < (int)this->items.size(); 
            index++)
        {
            joined += joinText;
            joined += this->items[index];
        }
    }

    return joined;
}

void StringSplitter::Remove(int line)
{
    this->items.erase(this->items.begin() + line);
}

void StringSplitter::RemoveTrailingEmptyLines()
{
    while (!this->items.empty() && this->items.back().empty())
        this->items.pop_back();
}
