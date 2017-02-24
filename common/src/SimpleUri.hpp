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


//Includes---------------------------------------------------------------------


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {
    
    class SimpleUri
    {
    public:
        SimpleUri();
        SimpleUri(const SimpleUri& other);
        SimpleUri(SimpleUri&& other);
        ~SimpleUri();

        SimpleUri& operator = (const SimpleUri& other);
        SimpleUri& operator = (SimpleUri&& other);

        bool IsValid() const;

        const wxString& GetScheme() const;
        void SetScheme(const wxString& value);

        const wxString& GetHost() const;
        void SetHost(const wxString& value);

        const wxString& GetPath() const;
        void SetPath(const wxString& value);

        const wxString& GetFragment() const;
        void SetFragment(const wxString& value);

        wxString ToString() const;

    private:
        wxString scheme;
        wxString host;
        wxString path;
        wxString fragment;
    };

} }
