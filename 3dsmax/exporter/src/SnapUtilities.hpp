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

    /** Various functions for performing snapping during an object's creation. */
    class SnapUtilities
    {
    public:
        /**
         * Performs snapping that works best for all Finjin objects.
         * @param t [in] - The current time.
         * @param node [in] - The object being created.
         * @param snap [in] - Snap information.
         * @param p [in] - The cursor position.
         * @param view [in] - The view in which the object creation is occurring.
         * @param objectLocalTM [in] - The object's 'local' transformation matrix.
         * This is applied before the node->GetObjectTM() matrix is applied.
         */
        static void DefaultSnap
            (
            TimeValue t, 
            INode* node, 
            SnapInfo* snap, 
            IPoint2* p, 
            ViewExp* view, 
            Matrix3 objectLocalTM
            );
    };

} }
