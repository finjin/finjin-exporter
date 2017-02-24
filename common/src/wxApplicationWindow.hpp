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

#if defined(__WXMAC__)
    typedef wxTopLevelWindow wxApplicationWindowBase;
#else
    typedef wxWindow wxApplicationWindowBase;
#endif


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** 
     * A wrapper for using the application's window as a wxWindow.
     * Creating and destroying instances of this class has no effect on the application window.
     */
    class wxApplicationWindow : public wxApplicationWindowBase
    {
    public:
        /** 
         * Initializes the window wrapper
         * @param modal [in] - Indicates whether the window is going to be used as the parent
         * of a modal dialog.
         */
        wxApplicationWindow(bool modal = true);

        ~wxApplicationWindow();
        
        bool Destroy();
        
    private:
        void DestroyCommon();

    private:
        bool modal;
        bool subclassed;
        bool destroyed;
    };

    struct wxApplicationWindowDeleter
    {
        typedef void result_type;
    
        void operator () (wxApplicationWindow* obj) const
        {
            obj->Destroy();
        }
    };

} }
