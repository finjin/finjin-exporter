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
    
    /** 3DS Max's entry point for the Finjin Exporter. */
    class FinjinCustomHelperIconExporter : public SceneExport
    {
    public:
        FinjinCustomHelperIconExporter();
        
        int ExtCount();
        const TCHAR* Ext(int n);

        const TCHAR* LongDesc();
        const TCHAR* ShortDesc();

        const TCHAR* AuthorName();
        
        const TCHAR* CopyrightMessage();

        const TCHAR* OtherMessage1(); 
        const TCHAR* OtherMessage2(); 

        unsigned int Version();

        void ShowAbout(HWND hWnd);

        int DoExport
            (
            const TCHAR* fileName, 
            ExpInterface* exportInterface, 
            Interface* maxInterface, 
            BOOL suppressPrompts = FALSE, 
            DWORD options = 0
            );
        
        BOOL SupportsOptions(int ext, DWORD options);

        static ClassDesc* GetClassDesc();
        
    protected:
        void SaveIcon(INode* node, const wxString& fileName);
        bool IsNodeMesh(INode* node);
        
    protected:
        TimeValue time;                //the current time slider value
        Interface* maxInterface;
    };

} }
