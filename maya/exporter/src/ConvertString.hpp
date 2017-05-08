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


//Inline functions--------------------------------------------------------------

/** Converts an application string to a wxString. */
inline wxString ApplicationStringToWxString(const MString& maya)
{
#if wxUSE_UNICODE
    return maya.asWChar();
#else
    return wxString::FromUTF8(maya.asUTF8());
#endif
}

/** Converts a wxString to an application. */
inline MString WxStringToApplicationString(const wxString& wx)
{
#if wxUSE_UNICODE
    return MString(WxStringAsInternal(wx));
#else
    return MString(wx.ToUTF8().data());
#endif
}
