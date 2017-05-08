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


//Includes----------------------------------------------------------------------
#include "SpinnerButton.hpp"
#include "SpinnerControlEvent.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    class ApplicationTextCtrl;

    /**
     * Implements a spinner control as a composite control.
     * The embedded edit control is a standard text control.
     * The spinner control is a custom control.
     *
     * Based on wxDragCtrl implementation by A.J. Lavin and Vctor Alejandro Gil Seplveda
     */
    class SpinnerControl : public wxPanel
    {
    public:
        enum {BUTTON_WIDTH = 13};

        enum UnitType
        {
            FLOAT_UNIT,
            INT_UNIT,
            WORLD_UNIT,
            UI_TIME_UNIT
        };

        SpinnerControl
            (
            wxWindow* parent,
            wxWindowID id = wxID_ANY,
            wxWindowID textCtrlId = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            double initial = 0,
            double minValue = 0,
            double maxValue = 1,
            UnitType unitType = FLOAT_UNIT,
            int precision = 5,
            const wxString& name = wxEmptyString
            );

        ~SpinnerControl();

        int GetIntValue();
        double GetValue();
        void SetValue(double value, bool generateEvent = false);

        UnitType GetUnitType() const;
        void SetUnitType(UnitType unitType);

        int GetPrecision() const;
        void SetPrecision(int precision);

        double GetMin() const;
        double GetMax() const;
        void SetRange(double a, double b);

        double GetAcceleration() const;
        void SetAcceleration(double a);

        bool IsEnabled() const;
        bool Enable(bool enable = true);
        void Disable();

        bool HasWindowID(wxWindowID id) const;

    protected:
        double GetUIValue();
        void SetUIValue(double value, bool generateEvent = false);

        virtual wxSize DoGetBestSize() const;

    private:
        void OnEnter(wxCommandEvent& event);
        void OnTextLoseFocus(wxFocusEvent& event);
        void OnDrag(SpinnerButtonEvent& event);
        void OnDown(SpinnerButtonEvent& event);
        void OnUp(SpinnerButtonEvent& event);
        void OnPrecision(SpinnerButtonEvent& event);
        void OnClick(wxMouseEvent& event);

        void HandleTextChange(bool generateValueChangeEvent);
        void SendValueChangedEvent();

    private:
        wxString value;
        wxString unit;
        UnitType unitType;

        ApplicationTextCtrl* textCtrl;
        SpinnerButton* spinnerButton;

        double minValue;
        double maxValue;

        bool pushed;
    };

} }
