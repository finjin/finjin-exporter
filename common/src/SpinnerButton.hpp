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

    /**
     * Implements a spinner button as a static text control
     * Based on wxDragCtrl implementation by A.J. Lavin and Vctor Alejandro Gil Seplveda
     */
    class SpinnerButton : public wxStaticText
    {
    public:
        SpinnerButton();
        SpinnerButton
            (
            wxWindow* parent,
            wxWindowID id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            const wxString& name = wxEmptyString
            );
      
        double GetValue() const { return this->value; }
        void SetValue(double val);
        
        double GetMin() const { return this->minValue; }
        double GetMax() const { return this->maxValue; }
        
        /** 
         * Set the range of values through which the button can drag.
         * maxVal must be greater than minVal.
         */
        void SetRange(double minVal, double maxVal);

        int GetPrecision() const { return this->precision ; }

        /** Set the number digits precision. Least significant digit will be 10^(-decimal) */
        void SetPrecision(int precision);

        bool AcceptsFocus() const {return false;}  
        
        /** Set the maximum number of significant figures */
        void SetMaxSigFigs(int n);

        double GetAcceleration() const { return this->acceleration; }

        /** 
         * Set mouse acceleration. 1 <-> linear, > 1 accelerated. 
         * The acceleration argument must be 1 or greater
         */
        void SetAcceleration(double acceleration);

        void OnEraseBackground(wxEraseEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnPaint(wxPaintEvent& event);

        void OnLeftDown(wxMouseEvent& event);
        void OnLeftUp(wxMouseEvent& event);
        void OnMotion(wxMouseEvent& event);
        void OnKeyDown(wxKeyEvent& event);
        void OnEnterWindow(wxMouseEvent& event);
        void OnLeaveWindow(wxMouseEvent& event);
        void OnCaptureLost(wxMouseCaptureLostEvent& event);

        static wxString ToString(double value, int precision);
        
        // Returns the magnitude of the argument in base 10.
        static int GetMagnitude(double x);

    protected:
        virtual wxSize DoGetBestSize() const;
        virtual void DoDrawDragger(wxPaintDC& dc);

        struct DragButtonStyle
        {
            wxColour border;
            wxColour corner;
            wxColour hilight;
            wxColour shadow;
            const char** arrowXpm;
            wxColour mask;
            wxColour top;
            wxColour bottom;
            bool active;
        };

        static wxBitmap RenderBitmap(const DragButtonStyle& style, int w, int h);

        wxBitmap& GetStyledBitmap(wxBitmap& bitmap, const DragButtonStyle& style);    

        void LimitValue(double& value);

    private:
        DECLARE_EVENT_TABLE()
        DECLARE_DYNAMIC_CLASS(SpinnerButton)

        void Init();

        bool Hit(int x, int y) const;
        
        int GetTotalDrag(const wxPoint& mousePosition) const;
        int GetLastDrag(const wxPoint& mousePosition) const;
        
    protected:
        wxCursor lastCursor;

        wxPoint dragStart;
        wxPoint dragLast;

        bool dragging;

        double dragStartValue;

        double minValue;
        double maxValue;
        double value;
        double acceleration;

        int minPrecision;
        int maxSigFigs;

        // Smallest change in cursor position that affects the value.
        int quantum;

        // Change to this->value per quantum change in cursor position.
        double scale;

        // Precision of this->value.  If mode is linear then precision is the
        // number of digits after the decimal point, if greater than 0. 0
        // precision indicates an integer. Precision < 0 indicates the
        // number is a multiple of 10^(-this->precision).  
        int precision;

        unsigned int logSteps;

        DragButtonStyle normalStyle;
        DragButtonStyle downStyle;
        DragButtonStyle disabledStyle;

        wxBitmap normalBitmap;
        wxBitmap downBitmap;
        wxBitmap disabledBitmap;
    };

    /** Spinner button event */
    class SpinnerButtonEvent : public wxCommandEvent
    {
    public:  
        SpinnerButtonEvent(wxEventType commandType = wxEVT_NULL, int id = 0) : wxCommandEvent(commandType, id)
        {
            this->value = 0;
        }

        SpinnerButtonEvent(const SpinnerButtonEvent& event) : wxCommandEvent(event)
        {
            this->value = event.GetValue();
        }

        wxEvent* Clone() const {return new SpinnerButtonEvent(*this);}

        double GetValue() const { return this->value; }
        void SetValue(double value) { this->value = value; }

        DECLARE_DYNAMIC_CLASS(SpinnerButtonEvent)

    private:
        double value;
    };


    DECLARE_EVENT_TYPE(EVT_SPINNER_BUTTON_DRAG, 133001)
    DECLARE_EVENT_TYPE(EVT_SPINNER_BUTTON_UP, 133002)
    DECLARE_EVENT_TYPE(EVT_SPINNER_BUTTON_DOWN, 133003)
    
    typedef void (wxEvtHandler::*wxDragButtonEventFunction)(SpinnerButtonEvent&);

    #define SpinnerButtonEventHandler(func) \
        (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxDragButtonEventFunction, &func)

    #define EVT_SPINNER_BUTTON_DRAG(id, func) \
        DECLARE_EVENT_TABLE_ENTRY(EVT_SPINNER_BUTTON_DRAG, id, wxID_ANY, (wxObjectEventFunction)(wxEventFunction)(wxDragButtonEventFunction)& func, (wxObject*)NULL),

    #define EVT_SPINNER_BUTTON_UP(id, func) \
        DECLARE_EVENT_TABLE_ENTRY(EVT_SPINNER_BUTTON_UP, id, wxID_ANY, (wxObjectEventFunction)(wxEventFunction)(wxDragButtonEventFunction)& func, (wxObject*)NULL),

    #define EVT_SPINNER_BUTTON_DOWN(id, func) \
        DECLARE_EVENT_TABLE_ENTRY(EVT_SPINNER_BUTTON_DOWN, id, wxID_ANY, (wxObjectEventFunction)(wxEventFunction)(wxDragButtonEventFunction)& func, (wxObject*)NULL),

} }
