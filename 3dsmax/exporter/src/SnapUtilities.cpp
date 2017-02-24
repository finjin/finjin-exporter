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
#include "SnapUtilities.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
void SnapUtilities::DefaultSnap
    (
    TimeValue t, 
    INode* node, 
    SnapInfo* snap, 
    IPoint2* p, 
    ViewExp* view, 
    Matrix3 objectLocalTM
    )
{
    //Get viewport graphics window
    auto gw = view->getGW();    

    //Set node transform into graphics window
    auto tm = objectLocalTM * node->GetObjectTM(t);            
    gw->setTransform(tm);

    auto invPlane = Inverse(snap->plane);

    //Make sure the vertex priority is active and at least as important as the best snap so far
    if (snap->vertPriority > 0 && snap->vertPriority <= snap->priority) 
    {
        Point2 fp((float)p->x, (float)p->y);
        Point2 screen2;
        IPoint3 pt3;

        Point3 thePoint(0,0,0);

        //If constrained to the plane, make sure this point is in it
        if (snap->snapType == SNAP_2D || snap->flags & SNAP_IN_PLANE) 
        {
            auto test = thePoint * tm * invPlane;
            if (fabs(test.z) > 0.0001)
                return;
        }
        gw->wTransPoint(&thePoint, &pt3);
        screen2.x = (float)pt3.x;
        screen2.y = (float)pt3.y;

        //Are we within the snap radius?
        int len = (int)Length(screen2 - fp);
        if (len <= snap->strength) 
        {
            //Is this priority better than the best so far?
            if (snap->vertPriority < snap->priority) 
            {
                snap->priority = snap->vertPriority;
                snap->bestWorld = thePoint * tm;
                snap->bestScreen = screen2;
                snap->bestDist = len;
            }
            else if (len < snap->bestDist) 
            {
                snap->priority = snap->vertPriority;
                snap->bestWorld = thePoint * tm;
                snap->bestScreen = screen2;
                snap->bestDist = len;
            }
        }
    }
}
