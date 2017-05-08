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
#include "SimpleUri.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
SimpleUri::SimpleUri()
{
}

SimpleUri::SimpleUri(const SimpleUri& other)
{
    this->scheme = other.scheme;
    this->host = other.host;
    this->path = other.path;
    this->fragment = other.fragment;
}

SimpleUri::SimpleUri(SimpleUri&& other)
{
    this->scheme = std::move(other.scheme);
    this->host = std::move(other.host);
    this->path = std::move(other.path);
    this->fragment = std::move(other.fragment);
}

SimpleUri::~SimpleUri()
{
}

SimpleUri& SimpleUri::operator = (const SimpleUri& other)
{
    if (this != &other)
    {
        this->scheme = other.scheme;
        this->host = other.host;
        this->path = other.path;
        this->fragment = other.fragment;
    }

    return *this;
}

SimpleUri& SimpleUri::operator = (SimpleUri&& other)
{
    if (this != &other)
    {
        this->scheme = std::move(other.scheme);
        this->host = std::move(other.host);
        this->path = std::move(other.path);
        this->fragment = std::move(other.fragment);
    }

    return *this;
}

bool SimpleUri::IsValid() const
{
    if (!this->scheme.empty())
        return !this->host.empty() || !this->path.empty();
    else
        return false;
}

const wxString& SimpleUri::GetScheme() const
{
    return this->scheme;
}

void SimpleUri::SetScheme(const wxString& value)
{
    this->scheme = value;
}

const wxString& SimpleUri::GetHost() const
{
    return this->host;
}

void SimpleUri::SetHost(const wxString& value)
{
    this->host = value;
}

const wxString& SimpleUri::GetPath() const
{
    return this->path;
}

void SimpleUri::SetPath(const wxString& value)
{
    this->path = value;
}

const wxString& SimpleUri::GetFragment() const
{
    return this->fragment;
}

void SimpleUri::SetFragment(const wxString& value)
{
    this->fragment = value;
}

wxString SimpleUri::ToString() const
{
    wxString result;

    if (IsValid())
    {
        if (!this->scheme.empty())
        {
            result += this->scheme;
            result += wxT("://");
        }
        result += this->host;
        result += this->path;
        if (!this->fragment.empty())
        {
            result += wxT("#");
            result += this->fragment;
        }
    }

    return result;
}
