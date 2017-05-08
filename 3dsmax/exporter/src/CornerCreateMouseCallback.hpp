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

    /**
     * Base class for a mouse creation callback that creates objects from the corners.
     * The object being created can be 2D (2 corners) or a 3D (3 corners), and can
     * be created from the center or with the center at the bottom of the object.
     */
    class CornerCreateMouseCallback : public CreateMouseCallBack
    {
    public:
        /** Indicates whether a 2D or 3D object is being created. */
        enum Dimension
        {
            DIMENSION_2D,
            DIMENSION_3D
        };

        /**
         * Constructors that initializes the callback to work on 3D objects
         * created from the center.
         */
        CornerCreateMouseCallback()
        {
            this->bottomPivot = false;
            this->dimension = DIMENSION_3D;
        }

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
                        SetWidth(0);
                        SetLength(0);
                        SetHeight(0);
                        SuspendSnap(true);
                        this->p0 = vpt->SnapPoint(m, m, nullptr, SNAP_IN_3D);
                        this->p1 = this->p0 + Point3(.01,.01,.01);
                        mat.SetTrans(.5f * (this->p0+ this->p1));

                        if (this->bottomPivot)
                        {
                            Point3 xyz = mat.GetTrans();
                            xyz.z = p0.z;
                            mat.SetTrans(xyz);
                        }

                        break;
                    }
                    case 1:
                    {
                        this->sp1 = m;
                        this->p1 = vpt->SnapPoint(m, m, nullptr, SNAP_IN_3D);
                        this->p1.z = this->p0.z + .01;
                        mat.SetTrans(.5f * (this->p0 + this->p1));

                        if (this->bottomPivot)
                        {
                            Point3 xyz = mat.GetTrans();
                            xyz.z = p0.z;
                            mat.SetTrans(xyz);
                        }

                        Point3 d = this->p1 - this->p0;
                        SetWidth(fabs(d.x));
                        SetLength(fabs(d.y));
                        SetHeight(fabs(d.z));

                        if (msg == MOUSE_POINT)
                        {
                            if ((Length(this->sp1 - this->sp0) < 3 || Length(d) < 0.1f))
                                return CREATE_ABORT;
                            else if (this->dimension == DIMENSION_2D)
                                return CREATE_STOP;
                        }

                        break;
                    }
                    case 2:
                    {
                        this->p1.z = this->p0.z + vpt->SnapLength(vpt->GetCPDisp(this->p0, Point3(0,0,1), this->sp1, m, TRUE));
                        mat.SetTrans(.5f * (this->p0 + this->p1));

                        if (this->bottomPivot)
                            mat.SetTrans(2, p0.z);

                        Point3 d = this->p1 - this->p0;
                        SetWidth(fabs(d.x));
                        SetLength(fabs(d.y));
                        SetHeight(fabs(d.z));

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
         * @param size [in] - The current width of the object.
         */
        virtual void SetWidth(float size)
        {
        }

        /**
         * Called as the user creates the object.
         * Overridden by derived classes.
         * @param size [in] - The current length of the object.
         */
        virtual void SetLength(float size)
        {
        }

        /**
         * Called as the user creates the object.
         * Overridden by derived classes.
         * @param size [in] - The current height of the object.
         */
        virtual void SetHeight(float size)
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

    protected:
        /** Indicates whether the pivot is at the bottom of the object. */
        bool bottomPivot;

        /** Indicates whether a 2D or 3D object is being created. */
        Dimension dimension;
    };

} }
