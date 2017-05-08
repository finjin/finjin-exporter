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


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Base class for a mouse creation callback that creates objects from a center point. */
    class CenteredCreateMouseCallback : public CreateMouseCallBack
    {
    public:
        /** The callback procedure. */
        int proc(ViewExp* vpt, int msg, int point, int flags, IPoint2 m, Matrix3& mat)
        {
            vpt->SnapPreview(m, m, nullptr, SNAP_IN_3D);

            if (msg == MOUSE_POINT || msg == MOUSE_MOVE)
            {
                switch (point)
                {
                    case 0:
                    {
                        this->sp0 = m;
                        SuspendSnap(true);
                        this->p0 = vpt->SnapPoint(m, m, nullptr, SNAP_IN_3D);
                        mat.SetTrans(this->p0);
                        break;
                    }
                    case 1:
                    {
                        this->sp1 = m;
                        this->p1 = vpt->SnapPoint(m, m, nullptr, SNAP_IN_3D);

                        float scale = std::max(Length(this->p1 - this->p0), 0.05f);
                        SetCreationSize(scale);

                        if (msg == MOUSE_POINT)
                        {
                            SuspendSnap(false);
                            return CREATE_STOP;
                        }
                        break;
                    }
                }
            }
            else if (msg == MOUSE_ABORT)
            {
                return CREATE_ABORT;
            }

            return CREATE_CONTINUE;
        }

    protected:
        /**
         * Called as the user creates the object.
         * Overridden by derived classes.
         * @param size [in] - The current size of the object.
         */
        virtual void SetCreationSize(float size)
        {
        }

        /**
         * Called when the snapping should be suspended/enabled.
         * Overridden by derived classes.
         * @param suspend [in] - Indicates whether snapping is suspended/enabled.
         */
        virtual void SuspendSnap(bool suspend)
        {
        }

    private:
        /** First world coordinate. */
        Point3 p0;

        /** Final world coordinate. */
        Point3 p1;

        /** First screen coordinate. */
        IPoint2 sp0;

        /** Final screen coordinate. */
        IPoint2 sp1;
    };

} }
