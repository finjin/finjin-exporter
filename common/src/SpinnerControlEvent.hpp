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

    /** Spinner control event passed into event handlers. */
    class SpinnerControlEvent : public wxCommandEvent 
    {
    public:
        SpinnerControlEvent(wxEventType e = wxEVT_NULL, int id = 0) : wxCommandEvent(e, id) 
        {
            this->value = 0;
        }   

        SpinnerControlEvent(const SpinnerControlEvent& event) : wxCommandEvent(event) 
        {
            this->value = event.GetValue();
        }
        
        wxEvent* Clone() const {return new SpinnerControlEvent(*this);}

        float GetValue() const {return this->value;}
        void SetValue(double value) {this->value = value;}
            
        DECLARE_DYNAMIC_CLASS(SpinnerControlEvent)

    private:
        double value;
    };

    //"Value changed" event
    DECLARE_EVENT_TYPE(EVT_SPINNER_CONTROL_VALUE_CHANGED, 13000)

    typedef void (wxEvtHandler::*SpinnerControlEventFunction)(SpinnerControlEvent& event);

    #define SpinnerControlEventHandler(func) \
        (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(SpinnerControlEventFunction, &func)

    #define SPINNER_CONTROL_TEXT_CHANGED_EVENT(id, fn) \
        DECLARE_EVENT_TABLE_ENTRY(EVT_SPINNER_CONTROL_VALUE_CHANGED, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction)(SpinnerControlEventFunction)&fn, \
        (wxObject*)NULL \
        ),

    #define EVT_SPINNER_VALUE_CHANGED(id, fn) SPINNER_CONTROL_TEXT_CHANGED_EVENT(id, fn)

} }
