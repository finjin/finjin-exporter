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
#include "WindowPlacementManager.hpp"

using namespace Finjin::Exporter;


//Static initialization--------------------------------------------------------
WindowPlacementManager::Placements WindowPlacementManager::placements;


//Implementation---------------------------------------------------------------
void WindowPlacementManager::SavePlacement(wxTopLevelWindow* window)
{   
    Placement placement;
    placement.rect = window->GetScreenRect();
    placement.maximized = window->IsMaximized();
    placements[window->GetName()] = placement;
}

bool WindowPlacementManager::RestorePlacement(wxTopLevelWindow* window)
{
    auto name = window->GetName();

    //Exit early if the window placement can't be found
    Placement placement;
    if (!GetSavedPlacement(name, placement))
        return false;
    
    //Restore the placement
    if (placement.maximized)
        window->Maximize();
    else
    {
        window->Move(placement.rect.x, placement.rect.y);
        window->SetSize(placement.rect.GetSize());
    }
    window->Layout();
    
    return true;
}

bool WindowPlacementManager::GetSavedPlacement(const wxString& name, Placement& placement)
{
    //Exit early if the window placement can't be found
    auto foundAt = placements.find(name);
    if (foundAt == placements.end())
        return false;

    placement = foundAt->second;
    return true;
}

bool WindowPlacementManager::HasSavedPlacement(const wxString& name)
{
    auto foundAt = placements.find(name);
    return foundAt != placements.end();
}
