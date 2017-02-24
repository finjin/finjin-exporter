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
#include "SpinnerControl-Max.hpp"
#include "ApplicationControls.hpp"
#include "StringUtilities.hpp"
#include "TimeAccessor.hpp"


//Implementation---------------------------------------------------------------
enum {EDIT_HEIGHT = 16};
enum {EDIT_WIDTH = 54};
enum {SPINNER_WIDTH = 10};
enum {SPINNER_HEIGHT = EDIT_HEIGHT};

Finjin::SpinnerControl::SpinnerControl
    (
    wxWindow* parent,
    wxWindowID id,
    wxWindowID textCtrlId,
    const wxPoint& pos,
    const wxSize& sz,
    double initial,
    double minValue, 
    double maxValue, 
    UnitType unitType,
    int precision,
    const wxString& name
    )
    : wxPanel(parent, id, pos, wxSize(sz.x, EDIT_HEIGHT), wxBORDER_NONE | wxTAB_TRAVERSAL, name)
{
    this->unitType = unitType;
    if (this->unitType == UI_TIME_UNIT)
    {
        this->minValue = TimeAccessor::ConvertInternalToUIValue(minValue);
        this->maxValue = TimeAccessor::ConvertInternalToUIValue(maxValue);        
    }
    else
    {
        this->minValue = minValue;
        this->maxValue = maxValue;        
    }
    this->precision = precision;
    
    //Determine edit position
    int editPositionX = 0;
    if (pos.x > 0)
        editPositionX = pos.x;
    int editPositionY = 0;
    if (pos.y > 0)
        editPositionY = pos.y;

    //Determine edit size
    int editWidth = EDIT_WIDTH + SPINNER_WIDTH;
    if (sz.x > SPINNER_WIDTH)
        editWidth = sz.x;
    editWidth -= SPINNER_WIDTH;
        
    //Determine spinner position
    int spinnerPositionX = editPositionX + editWidth;
    int spinnerPositionY = editPositionY;
    
    //Create edit control
    this->editHWnd = CreateWindowA(CUSTEDITWINDOWCLASS, "", WS_CHILD | WS_VISIBLE | WS_TABSTOP, editPositionX, editPositionY, editWidth, EDIT_HEIGHT, (HWND)GetHWND(), nullptr, wxGetInstance(), nullptr);
    if (textCtrlId == wxID_ANY)
    {
        textCtrlId = wxNewId();
        wxRegisterId(textCtrlId);
    }
    SetWindowLong(this->editHWnd, GWL_ID, textCtrlId);
    this->editCtrl = new wxAdoptedWindow(this->editHWnd, false);    
    //this->editCtrl->SetSize(wxSize(editWidth, EDIT_HEIGHT));
        
    //Create spinner control
    this->spinnerHWnd = CreateWindowA(SPINNERWINDOWCLASS, "", WS_CHILD | WS_VISIBLE | WS_TABSTOP, spinnerPositionX, spinnerPositionY, SPINNER_WIDTH, SPINNER_HEIGHT, (HWND)GetHWND(), nullptr, wxGetInstance(), nullptr);
    long spinnerCtrlId = wxNewId();
    wxRegisterId(spinnerCtrlId);
    SetWindowLong(this->spinnerHWnd, GWL_ID, spinnerCtrlId);
    this->spinnerCtrl = new wxAdoptedWindow(this->spinnerHWnd, false);
    //this->spinnerCtrl->SetSize(wxSize(SPINNER_WIDTH, SPINNER_HEIGHT));

    //Create sizers
    wxBoxSizer* box = new wxBoxSizer(wxHORIZONTAL); 
    box->Add(this->editCtrl);
    box->Add(this->spinnerCtrl);
    
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL); 
    topSizer->Add(box, 0, wxTOP, 2);
    SetSizer(topSizer);
    
    //Set up spinner
    this->spin = GetISpinner(this->spinnerHWnd);
    SetUnitType(this->unitType);
    SetValue(initial);
}
                
Finjin::SpinnerControl::~SpinnerControl()
{  
    ReleaseISpinner(this->spin);
    
    delete this->editCtrl;
    DestroyWindow(this->editHWnd);
    
    delete this->spinnerCtrl;
    DestroyWindow(this->spinnerHWnd);
}

int Finjin::SpinnerControl::GetIntValue()
{
    if (this->unitType == UI_TIME_UNIT)
        return RoundToInt(TimeAccessor::ConvertUIToInternalValue(this->spin->GetIVal()));
    else
        return this->spin->GetIVal();
}

double Finjin::SpinnerControl::GetValue()  
{
    if (this->unitType == INT_UNIT)
        return this->spin->GetIVal();
    else if (this->unitType == UI_TIME_UNIT)
        return TimeAccessor::ConvertUIToInternalValue(this->spin->GetIVal());
    else
        return this->spin->GetFVal();
}

void Finjin::SpinnerControl::SetValue(double value, bool generateEvent)
{
    if (this->unitType == INT_UNIT)
        this->spin->SetValue((int)value, FALSE);
    else if (this->unitType == UI_TIME_UNIT)
        this->spin->SetValue((int)TimeAccessor::ConvertInternalToUIValue(value), FALSE);
    else
        this->spin->SetValue((float)value, FALSE);

    if (generateEvent)
        SendValueChangedEvent();
}

Finjin::SpinnerControl::UnitType Finjin::SpinnerControl::GetUnitType() const
{
    return this->unitType;
}

void Finjin::SpinnerControl::SetUnitType(UnitType unitType)
{
    this->unitType = unitType;    

    //Determine spinner type
    EditSpinnerType spinnerType = EDITTYPE_FLOAT;
    if (this->unitType == INT_UNIT || this->unitType == UI_TIME_UNIT)
        spinnerType = EDITTYPE_INT;
    else if (this->unitType == WORLD_UNIT)
        spinnerType = EDITTYPE_UNIVERSE;    

    //Link edit control to spinner
    this->spin->LinkToEdit(this->editHWnd, spinnerType);
    
    //Set limits
    if (this->unitType == INT_UNIT || this->unitType == UI_TIME_UNIT)
        this->spin->SetLimits((int)this->minValue, (int)this->maxValue, FALSE);
    else
        this->spin->SetLimits((float)this->minValue, (float)this->maxValue, FALSE);
    
    //Set scale
    if (this->unitType == INT_UNIT || this->unitType == UI_TIME_UNIT)
        this->spin->SetScale(this->precision);
    else
        this->spin->SetScale(pow(.1, this->precision));
}

int Finjin::SpinnerControl::GetPrecision() const 
{
    return this->precision;
}

void Finjin::SpinnerControl::SetPrecision(int precision)
{
    if (precision != this->precision)
    {
        this->precision = precision;

        if (this->unitType == INT_UNIT || this->unitType == UI_TIME_UNIT)
            this->spin->SetScale(this->precision);
        else
            this->spin->SetScale(pow(.1, this->precision));
    }
}

double Finjin::SpinnerControl::GetAcceleration() const
{
    //Acceleration not supported
    return 1;
}

void Finjin::SpinnerControl::SetAcceleration(double a)
{
    //Acceleration not supported
}

void Finjin::SpinnerControl::SetRange(double a, double b)
{
    this->minValue = a;
    this->maxValue = b;
    
    if (this->unitType == INT_UNIT || this->unitType == UI_TIME_UNIT)
        this->spin->SetLimits((int)this->minValue, (int)this->maxValue, FALSE);
    else
        this->spin->SetLimits((float)this->minValue, (float)this->maxValue, FALSE);
}

double Finjin::SpinnerControl::GetMin() const 
{
    return this->minValue;
}

double Finjin::SpinnerControl::GetMax() const 
{
    return this->maxValue;
}

bool Finjin::SpinnerControl::IsEnabled() const
{
    return IsWindowEnabled(this->editHWnd) ? true : false;
}

bool Finjin::SpinnerControl::Enable(bool enable) 
{   
    EnableWindow(this->editHWnd, enable);
    EnableWindow(this->spinnerHWnd, enable);        
    return true;    
}

void Finjin::SpinnerControl::Disable()
{
    Enable(false);
}

wxSize Finjin::SpinnerControl::DoGetBestSize() const
{
    return GetSize();    
}

WXLRESULT Finjin::SpinnerControl::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    bool handled = false;

    switch (nMsg)
    {
        case WM_SETFOCUS:
        {
            this->editCtrl->SetFocus();
            handled = true;
            break;
        }
        case CC_SPINNER_CHANGE: 
        case CC_SPINNER_BUTTONDOWN:            
        case CC_SPINNER_BUTTONUP: 
        {
            handled = true;
            SendValueChangedEvent();
            break;
        }
    }

    if (handled)
        return 0;
    else
        return wxPanel::MSWWindowProc(nMsg, wParam, lParam);
}

void Finjin::SpinnerControl::SendValueChangedEvent()
{
    SpinnerControlEvent event(EVT_SPINNER_CONTROL_VALUE_CHANGED, GetId());
    event.SetEventObject(this);
    event.SetValue(GetValue());
    GetEventHandler()->ProcessEvent(event);  
}
