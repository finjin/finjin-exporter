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
#include "SpinnerButton.hpp"
#include "drag_arrows_normal.xpm"
#include "drag_arrows_disabled.xpm"

using namespace Finjin::Exporter;

#if defined(__WXMAC__)
    #define RESIZE_CURSOR wxCURSOR_HAND
#else
    #define RESIZE_CURSOR wxCURSOR_SIZENS
#endif


//Local functions--------------------------------------------------------------
/**
 * Rounds the given value to precision, measured in decimal places.
 * precision < 0 indicates precision < +/- 10^(-precision)
 */
static double Round(double value, int precision)
{
    double exponent = exp(-(double)precision * log(10.0));
    double mantissa = value / exponent;
    return round(mantissa) * exponent;
}


//Implementation---------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(SpinnerButtonEvent, wxEvent)
DEFINE_EVENT_TYPE(Finjin::Exporter::EVT_SPINNER_BUTTON_DRAG)
DEFINE_EVENT_TYPE(Finjin::Exporter::EVT_SPINNER_BUTTON_DOWN)
DEFINE_EVENT_TYPE(Finjin::Exporter::EVT_SPINNER_BUTTON_UP)

IMPLEMENT_DYNAMIC_CLASS(SpinnerButton, wxStaticText)
BEGIN_EVENT_TABLE(SpinnerButton, wxStaticText)
    EVT_ERASE_BACKGROUND(SpinnerButton::OnEraseBackground)
    EVT_PAINT(SpinnerButton::OnPaint)
    EVT_SIZE(SpinnerButton::OnSize)
    EVT_LEFT_DOWN(SpinnerButton::OnLeftDown)
    EVT_LEFT_UP(SpinnerButton::OnLeftUp)
    EVT_MOTION(SpinnerButton::OnMotion)
    EVT_ENTER_WINDOW(SpinnerButton::OnEnterWindow)
    EVT_LEAVE_WINDOW(SpinnerButton::OnLeaveWindow)
    EVT_MOUSE_CAPTURE_LOST(SpinnerButton::OnCaptureLost)    
END_EVENT_TABLE()

SpinnerButton::SpinnerButton()
{
    Init();
}

SpinnerButton::SpinnerButton
    (
    wxWindow* parent,
    wxWindowID id,
    const wxPoint& pos,
    const wxSize& size,
    const wxString& name
    )
{ 
    Init();

    wxSize bs = DoGetBestSize();
    wxSize sz = size == wxDefaultSize ? bs : size;

    if (!wxControl::Create(parent, id, pos, sz, wxBORDER_NONE | wxTAB_TRAVERSAL, wxDefaultValidator, name))
        return;
        
    this->value = 50;
    SetRange(0, 100);

    SetSizeHints(bs.x, bs.y);    
}

void SpinnerButton::SetValue(double value)
{
    this->value = value;
    LimitValue(this->value);
}

void SpinnerButton::SetRange(double minVal, double maxVal)
{
    double range = maxVal - minVal;
    if (range >= 0.0)
    {
        this->minValue = minVal;
        this->maxValue = maxVal;
        this->minPrecision = - static_cast<int>(round(log(fabs(range)) / log(10.0)));
    }
}

void SpinnerButton::OnEraseBackground(wxEraseEvent& event)
{
}

void SpinnerButton::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);  
    DoDrawDragger(dc);
}

void SpinnerButton::OnLeftDown(wxMouseEvent& event)
{
    this->dragLast = this->dragStart = event.GetPosition();
    this->dragStartValue = this->value;

    this->dragging = true;
    CaptureMouse();

    Refresh();

    SpinnerButtonEvent dragEvent(EVT_SPINNER_BUTTON_DOWN, GetId());
    dragEvent.SetValue(this->value);
    dragEvent.SetEventObject(this);
    GetEventHandler()->ProcessEvent(dragEvent);
}

void SpinnerButton::OnLeftUp(wxMouseEvent& event)
{
    this->dragging = false;
    ReleaseMouse();

    Refresh();

    SpinnerButtonEvent dragEvent(EVT_SPINNER_BUTTON_UP, GetId());
    dragEvent.SetValue(this->value);
    dragEvent.SetEventObject(this);
    wxPostEvent(GetParent(),dragEvent);
}

void SpinnerButton::OnMotion(wxMouseEvent& event)
{
    if (this->dragging)
    {
        wxPoint pos = event.GetPosition();

        //The accelerated mouse movement.
        double h;      
      
        if (this->acceleration == 1.0)
        {
            //Linear motion, no acceleration.  Base motion on the start
            //position of the drag, which gives nice clean motion.    
            h = static_cast<double>(GetTotalDrag(pos) / this->quantum);
        }
        else
        {
            //Nonlinear motion, i.e., mouse acceleration. Base motion
            //on the last position of the cursor.          
            int d = GetLastDrag(pos) / this->quantum;
            double m = pow(static_cast<double>(abs(d)), this->acceleration);
            h = d >= 0 ? m : - m;
        }
    
        if (h != 0.0)
        {
            this->value = this->dragStartValue + h * this->scale;
            
            this->dragLast = pos;
            if (this->acceleration > 1.0)
                this->dragStartValue = this->value;

            LimitValue(this->value);
            
            SpinnerButtonEvent dragEvent(EVT_SPINNER_BUTTON_DRAG, GetId());
            dragEvent.SetValue(GetValue());
            wxPostEvent(GetParent(),dragEvent);
        }
    }
}

void SpinnerButton::OnSize(wxSizeEvent& event)
{
    event.Skip();
}

void SpinnerButton::OnEnterWindow(wxMouseEvent& event)
{
    this->lastCursor = GetCursor();     
    SetCursor(wxCursor(RESIZE_CURSOR));
}

void SpinnerButton::OnLeaveWindow(wxMouseEvent &event)
{
    SetCursor(this->lastCursor);
}

void SpinnerButton::OnCaptureLost(wxMouseCaptureLostEvent& WXUNUSED(event))
{
}

wxString SpinnerButton::ToString(double value, int precision)
{
    if (precision > 0)
    {
        wxString format = wxString::Format(wxT("%%.%df"), precision);
        return wxString::Format(format, value);
    }
    else
    {
        wxString format = wxString::Format(wxT("%%.%dd"), - precision + 1);
        return wxString::Format(format, static_cast<int>(round(value)));
    }
}

wxSize SpinnerButton::DoGetBestSize() const
{
    return wxSize(16, 25);
}

void SpinnerButton::DoDrawDragger(wxPaintDC& dc)
{
    wxBitmap* buttonBitmap;

    if (this->dragging)
        buttonBitmap = &GetStyledBitmap(this->downBitmap, this->downStyle);
    else if (!IsEnabled())
        buttonBitmap = &GetStyledBitmap(this->disabledBitmap, this->disabledStyle);
    else
        buttonBitmap = &GetStyledBitmap(this->normalBitmap, this->normalStyle);
    
    dc.DrawBitmap(*buttonBitmap, 0, 0, false);
}

void SpinnerButton::Init()
{
    this->maxSigFigs = 15;
    this->quantum =  2;
    this->precision = 0;
    this->dragging = false;
    this->logSteps = 10;
    this->dragging = false;
    this->acceleration = 1.5;

    static const DragButtonStyle NormalStyle =
    {
        wxColor(165, 165, 165),
        wxColor(210, 210, 212),
        wxColor(255, 255, 255),
        wxColor(210, 210, 212),
        drag_arrows_normal,
        wxColor(255, 255, 255),
        wxColor(255, 255, 255),
        wxColor(218, 218, 220),
        false
    };

    static const DragButtonStyle DownStyle =
    {
        wxColor(133, 136, 144),
        wxColor(194, 196, 200),
        wxColor(255, 255, 255),
        wxColor(194, 196, 200),
        drag_arrows_normal,
        wxColor(255, 255, 255),
        wxColor(176, 179, 186),
        wxColor(210, 211, 215),
        true
    };

    static const DragButtonStyle DisabledStyle =
    {
        wxColor(133, 136, 144),
        wxColor(194, 196, 200),
        wxColor(255, 255, 255),
        wxColor(194, 196, 200),
        drag_arrows_disabled,
        wxColor(0, 0, 0),
        wxColor(255, 255, 255),
        wxColor(218, 218, 220),
        false
    };

    this->normalStyle = NormalStyle;
    this->downStyle = DownStyle;
    this->disabledStyle = DisabledStyle;
}

bool SpinnerButton::Hit(int x, int y) const
{
    int w;
    int h;
    GetClientSize(&w, &h);

    return x >= 0 && y >= 0 && x < w && y < h;
}

void SpinnerButton::SetPrecision(int precision)
{
    //Ensure that the new precision does not cause either extreme
    //of the range to exceed the maximum number of significant figures.
    int magnitude = GetMagnitude(wxMax(fabs(this->maxValue), fabs(this->minValue)));
    int sigFigs = precision + magnitude;
    if (sigFigs > this->maxSigFigs)
        precision = this->maxSigFigs - magnitude;
    
    this->precision = precision;
    if (this->precision < this->minPrecision)
        this->precision = this->minPrecision;

    this->scale = exp(-this->precision * log(10.0));

    this->value = Round(this->value, precision);
}

void SpinnerButton::SetMaxSigFigs(int n)
{
    if (n < 1)
        n = 1;
    
    this->maxSigFigs = n;

    SetPrecision(this->precision);
}

void SpinnerButton::SetAcceleration(double acceleration)
{
    if (acceleration >= 1)
        this->acceleration = acceleration;
}

int SpinnerButton::GetTotalDrag(const wxPoint& mousePosition) const
{
    return this->dragStart.y - mousePosition.y;
}

int SpinnerButton::GetLastDrag(const wxPoint& mousePosition) const
{
    return this->dragLast.y - mousePosition.y;
}

static void DrawTopLeft(wxDC& dc, const wxColor& color, int w, int h)
{
    dc.SetPen(*wxThePenList->FindOrCreatePen(color, 1, wxPENSTYLE_SOLID));
    dc.DrawLine(    0,     1, w - 1,     1);
    dc.DrawLine(    1,     0,     1, h - 1);
}

static void DrawBottomRight(wxDC& dc, const wxColor& color, int w, int h)
{
    dc.SetPen(*wxThePenList->FindOrCreatePen(color, 1, wxPENSTYLE_SOLID));
    dc.DrawLine(    0, h - 2, w - 1, h - 2);
    dc.DrawLine(w - 2,     0, w - 2, h - 1);
}

wxBitmap SpinnerButton::RenderBitmap(const SpinnerButton::DragButtonStyle& style, int w, int h)
{
    //Create a bitmap
    wxBitmap bitmap(w, h);

    //Select the bitmap into the DC
    wxMemoryDC bdc;
    bdc.SelectObject(bitmap);
    bdc.Clear();

    //Draw corners
    if (style.active)
    {
        DrawBottomRight(bdc, style.hilight, w, h);
        DrawTopLeft(bdc, style.shadow, w, h);
    }
    else
    {
        DrawTopLeft(bdc, style.hilight, w, h);
        DrawBottomRight(bdc, style.shadow, w, h);
    }

    //Draw outer border
    bdc.SetPen(*wxThePenList->FindOrCreatePen(style.border, 1, wxPENSTYLE_SOLID));
    bdc.DrawLine(    0,     0, w - 1,     0);
    bdc.DrawLine(w - 1,     0, w - 1, h - 1);
    bdc.DrawLine(w - 1, h - 1,     0, h - 1);
    bdc.DrawLine(0,     h - 1,     0,     0);

    //Draw edges
    bdc.SetPen(*wxThePenList->FindOrCreatePen(style.corner, 1, wxPENSTYLE_SOLID));
    bdc.DrawPoint(    0,     0);
    bdc.DrawPoint(w - 1,     0);
    bdc.DrawPoint(w - 1, h - 1);
    bdc.DrawPoint(    0, h - 1);

    //Draw arrows
    wxBitmap arrows(style.arrowXpm);
    arrows.SetMask(new wxMask(arrows, style.mask));
    wxPoint pos = wxPoint(w/2 - arrows.GetWidth()/2, h/2 - arrows.GetHeight()/2);
    bdc.DrawBitmap(arrows, pos.x, pos.y, true);

    return bitmap;
}

wxBitmap& SpinnerButton::GetStyledBitmap(wxBitmap& bitmap, const DragButtonStyle& style)
{
    int w;
    int h;
    GetClientSize(&w, &h);

    if (bitmap.Ok() && bitmap.GetWidth() == w && bitmap.GetHeight() == h)    
        return bitmap;
    
    bitmap = RenderBitmap(style, w, h);

    return bitmap;
}

int SpinnerButton::GetMagnitude(double x)
{
    double absX = fabs(x);
    if (absX < 10.0)
        return 0;

    return static_cast<int>(log(absX) / log(10.0));
}

void SpinnerButton::LimitValue(double& value)
{
    if (value < this->minValue)
        value = this->minValue;
    else if (value > this->maxValue)
        value = this->maxValue;    
}
