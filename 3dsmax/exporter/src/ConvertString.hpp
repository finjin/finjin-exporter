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
inline wxString ApplicationStringToWxString(const TSTR& maxString)
{
#if wxUSE_UNICODE
    return maxString.ToUTF16().data();
#else
    return wxString::FromUTF8(maxString.ToUTF8().data());
#endif
}

inline wxString ApplicationStringToWxString(const TCHAR* maxString)
{
    if (maxString == nullptr)
        return wxEmptyString;

#if wxUSE_UNICODE
    return maxString;
#else
    return wxString::FromUTF8(maxString);
#endif
}

/** Converts a wxString to an application string. */
inline TSTR WxStringToApplicationString(const wxString& wx)
{
    return WxStringAsInternal(wx);
}

inline MSTR WxStringToApplicationStringM(const wxString& wx)
{
#if wxUSE_UNICODE
    return MSTR::FromUTF16(wx.wx_str());
#else
    return MSTR::FromUTF8(wx.ToUTF8().data());
#endif
}
