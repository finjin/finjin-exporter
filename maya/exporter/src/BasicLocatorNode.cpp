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
#include "BasicLocatorNode.hpp"

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
bool BasicLocatorNode::isBounded() const
{
    return true;
}

void BasicLocatorNode::draw(M3dView& view, const MDagPath& dagPath, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{
    view.beginGL();
        glPushAttrib(GL_CURRENT_BIT);
            DrawLocator(view, style, status);
        glPopAttrib();
    view.endGL();
}

void BasicLocatorNode::DrawSphere(float r)
{
    DrawCircleXY(r,30);
    DrawCircleXZ(r,30);
    DrawCircleYZ(r,30);
}

void BasicLocatorNode::DrawCrossXY(float w, float h)
{
    glBegin(GL_LINES);
        glVertex3f( -0.5f*w, 0, 0 );
        glVertex3f(  0.5f*w, 0, 0 );
        glVertex3f( 0,  0.5f*h, 0 );
        glVertex3f( 0, -0.5f*h, 0 );
    glEnd();
}

void BasicLocatorNode::DrawCrossXZ(float w, float h)
{
    glBegin(GL_LINES);
        glVertex3f( -0.5f*w, 0, 0 );
        glVertex3f(  0.5f*w, 0, 0 );
        glVertex3f( 0,0,  0.5f*h );
        glVertex3f( 0,0, -0.5f*h );
    glEnd();
}

void BasicLocatorNode::DrawCrossYZ(float w, float h)
{
    glBegin(GL_LINES);
        glVertex3f( 0, -0.5f*w, 0 );
        glVertex3f( 0,  0.5f*w, 0 );
        glVertex3f( 0, 0,  0.5f*h );
        glVertex3f( 0, 0, -0.5f*h );
    glEnd();
}

void BasicLocatorNode::DrawQuadXY(float w, float h)
{
    glBegin(GL_LINE_LOOP);
        glVertex3f( -0.5f*w, -0.5f*h, 0 );
        glVertex3f(  0.5f*w, -0.5f*h, 0 );
        glVertex3f(  0.5f*w,  0.5f*h, 0 );
        glVertex3f( -0.5f*w,  0.5f*h, 0 );
    glEnd();
}

void BasicLocatorNode::DrawQuadXZ(float w, float h)
{
    glBegin(GL_LINE_LOOP);
        glVertex3f( -0.5f*w, 0, -0.5f*h );
        glVertex3f(  0.5f*w, 0, -0.5f*h );
        glVertex3f(  0.5f*w, 0,  0.5f*h );
        glVertex3f( -0.5f*w, 0,  0.5f*h );
    glEnd();
}

void BasicLocatorNode::DrawQuadYZ(float w, float h)
{
    glBegin(GL_LINE_LOOP);
        glVertex3f( 0, -0.5f*w, -0.5f*h );
        glVertex3f( 0,  0.5f*w, -0.5f*h );
        glVertex3f( 0,  0.5f*w,  0.5f*h );
        glVertex3f( 0, -0.5f*w,  0.5f*h );
    glEnd();
}

void BasicLocatorNode::DrawFilledQuadXY(float w, float h)
{
    glBegin(GL_QUADS);
        glVertex3f( -0.5f*w, -0.5f*h, 0 );
        glVertex3f(  0.5f*w, -0.5f*h, 0 );
        glVertex3f(  0.5f*w,  0.5f*h, 0 );
        glVertex3f( -0.5f*w,  0.5f*h, 0 );
    glEnd();
}

void BasicLocatorNode::DrawFilledQuadXZ(float w, float h)
{
    glBegin(GL_QUADS);
        glVertex3f( -0.5f*w, 0, -0.5f*h );
        glVertex3f(  0.5f*w, 0, -0.5f*h );
        glVertex3f(  0.5f*w, 0,  0.5f*h );
        glVertex3f( -0.5f*w, 0,  0.5f*h );
    glEnd();
}

void BasicLocatorNode::DrawFilledQuadYZ(float w, float h)
{
    glBegin(GL_QUADS);
        glVertex3f( 0, -0.5f*w, -0.5f*h );
        glVertex3f( 0,  0.5f*w, -0.5f*h );
        glVertex3f( 0,  0.5f*w,  0.5f*h );
        glVertex3f( 0, -0.5f*w,  0.5f*h );
    glEnd();
}

void BasicLocatorNode::DrawCube(float w, float h, float d)
{
    glPushMatrix();
        glTranslatef(0,0,d/2);
        DrawQuadXY(w,h);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,0,-d/2);
        DrawQuadXY(w,h);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,h/2,0);
        DrawQuadXZ(w,d);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,-h/2,0);
        DrawQuadXZ(w,d);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(w/2,0,0);
        DrawQuadYZ(h,d);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-w/2,0,0);
        DrawQuadYZ(h,d);
    glPopMatrix();
}

void BasicLocatorNode::DrawFilledCube(float w, float h, float d)
{
    glPushMatrix();
        glTranslatef(0,0,d/2);
        DrawFilledQuadXY(w,h);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,0,-d/2);
        DrawFilledQuadXY(w,h);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,h/2,0);
        DrawFilledQuadXZ(w,d);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,-h/2,0);
        DrawFilledQuadXZ(w,d);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(w/2,0,0);
        DrawFilledQuadYZ(h,d);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-w/2,0,0);
        DrawFilledQuadYZ(h,d);
    glPopMatrix();
}

void BasicLocatorNode::DrawCylinderXY(float r, float h)
{
    glPushMatrix();
        glTranslatef(0,0,h/2);
        DrawCircleXY(r,30);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,0,-h/2);
        DrawCircleXY(r,30);
    glPopMatrix();

    glBegin(GL_LINES);
        glVertex3f(r,0,-h/2);
        glVertex3f(r,0,h/2);

        glVertex3f(-r,0,-h/2);
        glVertex3f(-r,0,h/2);

        glVertex3f(0,r,-h/2);
        glVertex3f(0,r,h/2);

        glVertex3f(0,-r,-h/2);
        glVertex3f(0,-r,h/2);
    glEnd();
}

void BasicLocatorNode::DrawCone(float r, float h)
{
    glPushMatrix();
        glTranslatef(0,-h/2,0);
        DrawCircleXZ(r,30);
    glPopMatrix();

    glBegin(GL_LINES);
        glVertex3f(r,-h/2,0);
        glVertex3f(0,h/2,0);

        glVertex3f(-r,-h/2,0);
        glVertex3f(0,h/2,0);

        glVertex3f(0,-h/2,r);
        glVertex3f(0,h/2,0);

        glVertex3f(0,-h/2,-r);
        glVertex3f(0,h/2,0);
    glEnd();
}

void BasicLocatorNode::DrawCylinderXZ(float r, float h)
{
    glPushMatrix();
        glTranslatef(0,h/2,0);
        DrawCircleXZ(r,30);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,-h/2,0);
        DrawCircleXZ(r,30);
    glPopMatrix();

    glBegin(GL_LINES);
        glVertex3f(r,-h/2,0);
        glVertex3f(r,h/2,0);

        glVertex3f(-r,-h/2,0);
        glVertex3f(-r,h/2,0);

        glVertex3f(0,-h/2,r);
        glVertex3f(0,h/2,r);

        glVertex3f(0,-h/2,-r);
        glVertex3f(0,h/2,-r);
    glEnd();
}

void BasicLocatorNode::DrawCylinderYZ(float r, float h)
{
    glPushMatrix();
        glTranslatef(h/2,0,0);
        DrawCircleYZ(r,30);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-h/2,0,0);
        DrawCircleYZ(r,30);
    glPopMatrix();

    glBegin(GL_LINES);
        glVertex3f(-h/2,r,0);
        glVertex3f(h/2,r,0);

        glVertex3f(-h/2,-r,0);
        glVertex3f(h/2,-r,0);

        glVertex3f(-h/2,0,r);
        glVertex3f(h/2,0,r);

        glVertex3f(-h/2,0,-r);
        glVertex3f(h/2,0,-r);
    glEnd();
}

void BasicLocatorNode::DrawCircleXY(float r, int divs)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i != divs; i++)
    {
        float angle = (i*360.0f/divs) * 3.1415926535898f/180.0f;
        float ca = cos(angle);
        float sa = sin(angle);
        glVertex3f(ca*r,sa*r,0.0f);
    }
    glEnd();
}

void BasicLocatorNode::DrawCircleXZ(float r, int divs)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i != divs; i++)
    {
        float angle = (i*360.0f/divs) * 3.1415926535898f/180.0f;
        float ca = cos(angle);
        float sa = sin(angle);
        glVertex3f(ca*r,0.0f,sa*r);
    }
    glEnd();
}

void BasicLocatorNode::DrawCircleYZ(float r, int divs)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i != divs; i++)
    {
        float angle = (i*360.0f/divs) * (3.1415926535898f/180.0f);
        float ca = cos(angle);
        float sa = sin(angle);
        glVertex3f(0.0f,ca*r,sa*r);
    }
    glEnd();
}

void BasicLocatorNode::DrawFilledCircleXY(float r, int divs)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i != divs; i++)
    {
        float angle = (i*360.0f/divs) * 3.1415926535898f/180.0f;
        float ca = cos(angle);
        float sa = sin(angle);
        glVertex3f(ca*r,sa*r,0.0f);
    }
    glEnd();
}

void BasicLocatorNode::DrawFilledCircleXZ(float r, int divs)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i != divs; i++)
    {
        float angle = (i*360.0f/divs) * 3.1415926535898f/180.0f;
        float ca = cos(angle);
        float sa = sin(angle);
        glVertex3f(ca*r,0.0f,sa*r);
    }
    glEnd();
}

void BasicLocatorNode::DrawFilledCircleYZ(float r, int divs)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i != divs; i++)
    {
        float angle = (i*360.0f/divs) * (3.1415926535898f/180.0f);
        float ca = cos(angle);
        float sa = sin(angle);
        glVertex3f(0.0f,ca*r,sa*r);
    }
    glEnd();
}

void BasicLocatorNode::DrawSpiralXY(float sr, float er, float sa, float ea, float h, int divs)
{
    glPushMatrix();
        glTranslatef(0,0,-h/2);
        DrawCircleXY(sr,30);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,0,h/2);
        DrawCircleXY(er,30);
    glPopMatrix();

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i != divs; i++)
    {
        float t = (float) i / (divs-1);
        float r = (1-t)*sr + t*er;
        float angle = (i*(ea-sa)/divs) * 3.1415926535898f/180.0f;
        float ca = cos(angle);
        float sa = sin(angle);
        glVertex3f(ca*r,sa*r,t*h -h/2);
    }
    glEnd();
}

void BasicLocatorNode::DrawSpiralXZ(float sr, float er, float sa, float ea, float h, int divs)
{
    glPushMatrix();
        glTranslatef(0,-h/2,0);
        DrawCircleXZ(sr,30);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,h/2,0);
        DrawCircleXZ(er,30);
    glPopMatrix();

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i != divs; i++)
    {
        float t = (float) i / (divs-1);
        float r = (1-t)*sr + t*er;
        float angle = (i*(ea-sa)/divs) * 3.1415926535898f/180.0f;
        float ca = cos(angle);
        float sa = sin(angle);
        glVertex3f(ca*r,t*h -h/2,sa*r);
    }
    glEnd();
}

void BasicLocatorNode::DrawSpiralYZ(float sr, float er, float sa, float ea, float h, int divs)
{
    glPushMatrix();
        glTranslatef(-h/2,0,0);
        DrawCircleXZ(sr,30);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(h/2,0,0);
        DrawCircleXZ(er,30);
    glPopMatrix();

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i != divs; i++)
    {
        float t = (float) i / (divs-1);
        float r = (1-t)*sr + t*er;
        float angle = (i*(ea-sa)/divs) * 3.1415926535898f/180.0f;
        float ca = cos(angle);
        float sa = sin(angle);
        glVertex3f(t*h -h/2,ca*r,sa*r);
    }
    glEnd();
}
