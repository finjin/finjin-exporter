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
#include "SpinnerControl-Generic.hpp"
#include "ApplicationControls.hpp"
#include "ApplicationAccessor.hpp"
#include "TimeAccessor.hpp"

#define DEFAULT_CONTROL_WIDTH 40
#define DEFAULT_CONTROL_HEIGHT 19


//Implementation----------------------------------------------------------------
Finjin::Exporter::SpinnerControl::SpinnerControl
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
    : wxPanel(parent, id, pos, wxSize(sz.GetWidth() > 0 ? sz.GetWidth() : DEFAULT_CONTROL_WIDTH, sz.GetHeight() > 0 ? sz.GetHeight() : DEFAULT_CONTROL_HEIGHT), wxBORDER_NONE, name)
{
    this->pushed = false;

    this->unitType = unitType;
    switch (this->unitType)
    {
        case WORLD_UNIT:
        {
            break;
        }
        case INT_UNIT:
        {
            precision = 0;
            break;
        }
        case UI_TIME_UNIT:
        {
            if (!TimeAccessor::IsUIUnitTimeBased())
                precision = 0;
        }
        default: break;
    }
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

    wxSize size = sz;
    if (size.GetHeight() < 0)
        size.SetHeight(DEFAULT_CONTROL_HEIGHT);
    if (size.GetWidth() < 0)
       size.SetWidth(DEFAULT_CONTROL_WIDTH);

    //Create the text control
    if (textCtrlId == wxID_ANY)
    {
        textCtrlId = wxNewId();
        wxRegisterId(textCtrlId);
    }

    wxSize textCtrlSize(size.GetWidth() - BUTTON_WIDTH, size.GetHeight());

    long textCtrlStyle = wxTE_PROCESS_ENTER | wxTE_LEFT | wxTAB_TRAVERSAL;
    static int textControlNameCount = 0;
    this->textCtrl = new ApplicationTextCtrl(this, textCtrlId, wxEmptyString, wxPoint(0,0), textCtrlSize, textCtrlStyle, wxDefaultValidator, wxString::Format(wxT("TXTCTRL%d"), textControlNameCount++));

    //Connect event handlers to the text control
    Connect(this->textCtrl->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(Finjin::Exporter::SpinnerControl::OnEnter));
    this->textCtrl->Connect(this->textCtrl->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(Finjin::Exporter::SpinnerControl::OnTextLoseFocus), 0, this);

    //Create the spinner button
    int dragButtonId = wxNewId();
    wxRegisterId(dragButtonId);

    this->spinnerButton = new SpinnerButton
        (
        this,
        dragButtonId,
        wxPoint(size.GetWidth() - BUTTON_WIDTH, 0),
        wxSize(BUTTON_WIDTH, size.GetHeight()),
        wxEmptyString
        );
    this->spinnerButton->SetRange(this->minValue, this->maxValue);
    this->spinnerButton->SetPrecision(precision);

    //Connect event handlers to the spinner button
    Connect(this->spinnerButton->GetId(), EVT_SPINNER_BUTTON_DRAG, SpinnerButtonEventHandler(Finjin::Exporter::SpinnerControl::OnDrag));
    Connect(this->spinnerButton->GetId(), EVT_SPINNER_BUTTON_DOWN, SpinnerButtonEventHandler(Finjin::Exporter::SpinnerControl::OnDown));
    Connect(this->spinnerButton->GetId(), EVT_SPINNER_BUTTON_UP, SpinnerButtonEventHandler(Finjin::Exporter::SpinnerControl::OnUp));

    //Create sizers
    wxBoxSizer* box = new wxBoxSizer(wxHORIZONTAL);
    box->Add(this->textCtrl);
    box->Add(this->spinnerButton);

    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(box);
    SetSizer(topSizer);

    SetSize(size);
    this->spinnerButton->SetSize(wxSize(BUTTON_WIDTH, GetSize().GetHeight()));

    SetValue(initial);

    wxCommandEvent enter_event(wxEVT_COMMAND_TEXT_ENTER, GetId());
    wxPostEvent(GetParent(), enter_event);
}

Finjin::Exporter::SpinnerControl::~SpinnerControl()
{
}

int Finjin::Exporter::SpinnerControl::GetIntValue()
{
    int value = RoundToInt(GetValue());
    return value;
}

double Finjin::Exporter::SpinnerControl::GetValue()
{
    double value = GetUIValue();
    if (this->unitType == WORLD_UNIT)
        value /= ApplicationAccessor::GetInternalToUIScale();
    else if (this->unitType == UI_TIME_UNIT)
        value = TimeAccessor::ConvertUIToInternalValue(value);
    return value;
}

void Finjin::Exporter::SpinnerControl::SetValue(double value, bool generateEvent)
{
    if (this->unitType == WORLD_UNIT)
        value *= ApplicationAccessor::GetInternalToUIScale();
    else if (this->unitType == UI_TIME_UNIT)
        value = TimeAccessor::ConvertInternalToUIValue(value);
    SetUIValue(value, generateEvent);
}

double Finjin::Exporter::SpinnerControl::GetUIValue()
{
    double value = 0;

    //Get text value
    wxString s = this->textCtrl->GetValue();

    //Find the index of the first character that can't be parsed as a number
    const size_t length = s.Length();
    size_t i;
    for (i = 0; i < length; ++i)
    {
        const wxChar c = s.GetChar(i);
        if (isdigit(c) || c == wxT(',') || c == wxT('.') || c == wxT('-'))
        {
            //Do nothing
        }
        else
            break;
    }

    //Get the number
    wxString numericText = s.substr(0, i);

    //Parse the number as a double
    if (!numericText.ToDouble(&value))
    {
        //Parse failed. Try replacing periods with commas, or vice versa
        if (numericText.Find(wxT('.')) != -1)
            numericText.Replace(wxT("."), wxT(","));
        else
            numericText.Replace(wxT(","), wxT("."));

        //Try to parse again
        numericText.ToDouble(&value);
    }

    return value;
}

void Finjin::Exporter::SpinnerControl::SetUIValue(double value, bool generateEvent)
{
    this->spinnerButton->SetValue(value);

    //Convert the current button value to text
    wxString textValue = SpinnerButton::ToString(this->spinnerButton->GetValue(), this->spinnerButton->GetPrecision());
    if (this->unitType == WORLD_UNIT)
        textValue += !this->unit.empty() ? this->unit : ApplicationAccessor::GetDefaultUnitType(true);

    //Put the text value into the text control
    this->textCtrl->SetValue(textValue);

    //Put the current value back into the button
    double v = GetUIValue();
    this->spinnerButton->SetValue(v);

    if (generateEvent)
        SendValueChangedEvent();
}

Finjin::Exporter::SpinnerControl::UnitType Finjin::Exporter::SpinnerControl::GetUnitType() const
{
    return this->unitType;
}

void Finjin::Exporter::SpinnerControl::SetUnitType(UnitType unitType)
{
    this->unitType = unitType;
}

int Finjin::Exporter::SpinnerControl::GetPrecision() const
{
    return this->spinnerButton->GetPrecision();
}

void Finjin::Exporter::SpinnerControl::SetPrecision(int precision)
{
    this->spinnerButton->SetPrecision(precision);
}

double Finjin::Exporter::SpinnerControl::GetAcceleration() const
{
    return this->spinnerButton->GetAcceleration();
}

void Finjin::Exporter::SpinnerControl::SetAcceleration(double a)
{
    this->spinnerButton->SetAcceleration(a);
}

void Finjin::Exporter::SpinnerControl::SetRange(double a, double b)
{
    this->spinnerButton->SetRange(a, b);
}

double Finjin::Exporter::SpinnerControl::GetMin() const
{
    return this->spinnerButton->GetMin();
}

double Finjin::Exporter::SpinnerControl::GetMax() const
{
    return this->spinnerButton->GetMax();
}

void Finjin::Exporter::SpinnerControl::OnEnter(wxCommandEvent& WXUNUSED(event))
{
    HandleTextChange(true);
}

void Finjin::Exporter::SpinnerControl::OnTextLoseFocus(wxFocusEvent& event)
{
    HandleTextChange(true);
}

void Finjin::Exporter::SpinnerControl::OnDrag(SpinnerButtonEvent& event)
{
    //Convert the current button value to text
    wxString textValue = SpinnerButton::ToString(this->spinnerButton->GetValue(), this->spinnerButton->GetPrecision());
    if (this->unitType == WORLD_UNIT)
        textValue += !this->unit.empty() ? this->unit : ApplicationAccessor::GetDefaultUnitType(true);

    //Put the text value into the text control
    this->textCtrl->SetValue(textValue);

    //Put the current value back into the button
    this->spinnerButton->SetValue(GetUIValue());

    //Generate value changed event
    SendValueChangedEvent();
}

void Finjin::Exporter::SpinnerControl::OnDown(SpinnerButtonEvent& event)
{
    this->pushed = true;
    this->textCtrl->SetFocus();
    event.Skip();
}

void Finjin::Exporter::SpinnerControl::OnUp(SpinnerButtonEvent& event)
{
    this->pushed = false;
    event.Skip();
}

void Finjin::Exporter::SpinnerControl::OnPrecision(SpinnerButtonEvent& event)
{
    event.Skip();
}

void Finjin::Exporter::SpinnerControl::HandleTextChange(bool generateValueChangeEvent)
{
    //Put the new value into the drag button
    this->spinnerButton->SetValue(GetUIValue());

    //Convert the value to text
    wxString textValue = SpinnerButton::ToString(this->spinnerButton->GetValue(), this->spinnerButton->GetPrecision());
    if (this->unitType == WORLD_UNIT)
        textValue += !this->unit.empty() ? this->unit : ApplicationAccessor::GetDefaultUnitType(true);

    //Put the text value back into the text control
    this->textCtrl->SetValue(textValue);

    //Generate value changed event
    if (generateValueChangeEvent)
        SendValueChangedEvent();
}

bool Finjin::Exporter::SpinnerControl::IsEnabled() const
{
    return this->textCtrl->IsEnabled();
}

bool Finjin::Exporter::SpinnerControl::Enable(bool enable)
{
    this->textCtrl->Enable(enable);
    this->spinnerButton->Enable(enable);
    return true;
}

void Finjin::Exporter::SpinnerControl::Disable()
{
    Enable(false);
}

bool Finjin::Exporter::SpinnerControl::HasWindowID(wxWindowID id) const
{
    return
        GetId() == id ||
        this->textCtrl->GetId() == id ||
        this->spinnerButton->GetId() == id;
}

wxSize Finjin::Exporter::SpinnerControl::DoGetBestSize() const
{
    return GetSize();
}

void Finjin::Exporter::SpinnerControl::SendValueChangedEvent()
{
    SpinnerControlEvent event(EVT_SPINNER_CONTROL_VALUE_CHANGED, GetId());
    event.SetEventObject(this);
    event.SetValue(GetValue());
    GetEventHandler()->ProcessEvent(event);
}
