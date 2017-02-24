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


//Includes---------------------------------------------------------------------
#ifndef OSMacOSX_
    #include <tchar.h>
#endif

//Standard library includes
#include <algorithm>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <sstream>
#include <list>
#include <cstdlib>
#include <cmath>
#include <memory>

//Eigen
#include <Eigen/Eigen>

namespace Finjin { namespace Exporter {
    typedef Eigen::Matrix3f MathMatrix33;
    typedef Eigen::Matrix4f MathMatrix44;
    typedef Eigen::Vector2f MathVector2;
    typedef Eigen::Vector3f MathVector3;
    typedef Eigen::Vector4f MathVector4;
    typedef Eigen::Quaternionf MathQuaternion;
} }

//wxWidgets includes
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/hyperlink.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/mimetype.h>
#include <wx/msgdlg.h>
#include <wx/radiobut.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/listctrl.h>
#include <wx/filename.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/tokenzr.h>
#include <wx/scrolwin.h>
#include <wx/filesys.h>
#include <wx/panel.h>
#include <wx/event.h>
#include <wx/cursor.h>
#include <wx/defs.h>
#include <wx/control.h>
#include <wx/dcmemory.h>
#include <wx/listbox.h>
#include <wx/help.h>
#include <wx/app.h>    
#include <wx/init.h>
#include <wx/sysopt.h>
#include <wx/gauge.h>
#include <wx/datetime.h>
#include <wx/menu.h>
#include <wx/timer.h>
#include <wx/dir.h>
#include <wx/evtloop.h>
#include <wx/spinctrl.h>
#include <wx/file.h>
#include <wx/regex.h>
#include <wx/sstream.h>
#include <wx/stdpaths.h>
#include <wx/dcclient.h>
#include <wx/uri.h>
#if defined(__WXMAC__)
    #include <wx/nonownedwnd.h>

    //Disable information icon usage since it ends up causing the dialog to show help/yes/no
    #define wxINFORMATION_DIALOG_FLAGS wxCLOSE
    #define STANDARD_BUTTON_WIDTH 115
#endif

#if defined(__WXMSW__)
    WXDLLIMPEXP_BASE void wxSetInstance(HINSTANCE hInstance);
    extern "C"
    {
        WXDLLIMPEXP_BASE HINSTANCE wxGetInstance();
    }

    #define wxINFORMATION_DIALOG_FLAGS wxOK | wxICON_INFORMATION
    #define STANDARD_BUTTON_WIDTH 100
#endif

#include "finjin/common/FinjinCommonLibrary.hpp"
#include "finjin/common/EnumBitwise.hpp"
#include "finjin/common/EnumValues.hpp"
#include "finjin/common/Math.hpp"
#include "finjin/common/Setting.hpp"
#include "finjin/common/StaticVector.hpp"
#include "finjin/common/WxError.hpp"
#include "finjin/common/WxTimeDuration.hpp"

//Undef the Windows RGB macro and redefine it
#if defined(RGB)
    #undef RGB

    //From wingdi.h
    #define WIN_RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#endif


//Simple data types and functions----------------------------------------------
namespace Finjin { namespace Common {

    enum ByteOrder
    {
        NATIVE, //Not an actual byte order
        LITTLE,
        BIG
    };

    inline ByteOrder GetByteOrder()
    {
        uint16_t lowByteSet = 0x00ff;
        uint8_t* someByte = (uint8_t*)&lowByteSet;
        return *someByte != 0 ? ByteOrder::LITTLE : ByteOrder::BIG;
    }

    inline bool IsLittleEndian()
    {
        return GetByteOrder() == ByteOrder::LITTLE;
    }

    inline bool IsBigEndian()
    {
        return GetByteOrder() == ByteOrder::BIG;
    }

    template <typename T>
    void SwapBytes(T& value)
    {
        auto copy = value;

        auto inByte = reinterpret_cast<int8_t*>(&copy) + sizeof(T) - 1;
        auto outByte = reinterpret_cast<int8_t*>(&value);

        for (size_t i = 0; i < sizeof(T); i++)
            *outByte++ = *inByte--;
    }

    inline wxString FormatIso8601(wxDateTime value)
    {
        auto datePart = value.FormatISODate();

        auto timePart = value.FormatISOTime();
        timePart.Replace(wxT(":"), wxT("_"));

        auto result = datePart;
        result += wxT("-");
        result += timePart;
        return result;
    }

    template <typename T>
    std::string ConvertToUtf8String(const T& value)
    {
        wxString s;
        s << value;
        return s.ToUTF8().data();
    }

    inline std::string ConvertToUtf8String(int8_t value)
    {
        wxString s;
        s << (int)value;
        return s.ToUTF8().data();
    }

    inline std::string ConvertToUtf8String(uint8_t value)
    {
        wxString s;
        s << (unsigned)value;
        return s.ToUTF8().data();
    }

    inline std::string ConvertToUtf8String(float value)
    {
        auto s = wxString::Format(wxT("%.10f"), value);
        s.Replace(wxT(","), wxT("."));

        while (s.length() > 1 && s.Last() == wxT('0'))
            s.RemoveLast();
        if (s.Last() == wxT('.'))
            s.RemoveLast();

        return s.ToUTF8().data();
    }

    inline std::string ConvertToUtf8String(double value)
    {
        auto s = wxString::Format(wxT("%.10f"), value);
        s.Replace(wxT(","), wxT("."));

        while (s.length() > 1 && s.Last() == wxT('0'))
            s.RemoveLast();
        if (s.Last() == wxT('.'))
            s.RemoveLast();

        return s.ToUTF8().data();
    }

    template <typename T>
    std::string ConvertToUtf8String(const wxString& s)
    {
        return s.ToUTF8().data();
    }

}}

namespace Finjin { namespace Exporter {

    using namespace Finjin::Common;

    /** A boolean that can inherit from its parent. */
    enum InheritedBool
    {
        /** False */
        INHERITED_FALSE,

        /** True */
        INHERITED_TRUE,

        /** Use the related parent boolean value */
        INHERITED_USE_PARENT_BOOL
    };

    /** 
     * Generates a true boolean value. 
     * @param value [in] - The inherited boolean value.
     * @param parentBool [in] - The parent boolean value.
     */
    inline bool FromInheritedBool(InheritedBool value, bool parentBool)
    {
        switch (value)
        {
            case INHERITED_FALSE: return false;
            case INHERITED_TRUE: return true;
            default: break;
        }
        return parentBool;
    }

    /** Up axis. */
    enum class UpAxis
    {
        Y,
        Z,
        COUNT,
    };

    /** Primitive types. */
    enum class PrimitiveType
    {
        DEFAULT,
        TRIANGLES,
        POINTS,
        LINES
    };

    /** Time sampling types. */
    enum class SampleType
    {
        INTERVAL,
        RATE,
        USE_PARENT
    };

    /** Types of sorting for an object's children. */
    enum class ChildSortType
    {
        NONE,
        BY_NAME,
        BY_ORDER
    };

    enum class LightType
    {
        POINT_LIGHT,
        DIRECTIONAL_LIGHT,
        SPOT_LIGHT
    };

    /** Mesh animation track. */
    struct MeshAnimationTrack
    {
        /** An application-specific value indicating which 'track' is referred to. */
        int location;

        /** The internal name of the track. */
        wxString name;

        /** The display name of the track. */
        wxString displayName;
    };

    struct UserDataTypesLocation
    {
        enum Type
        {
            FILE,
            DIRECTORY
        };
        
        UserDataTypesLocation()
        {
            this->type = DIRECTORY;
        }

        UserDataTypesLocation(Type type, const wxString& name)
        {
            this->name = name;
            this->type = type;            
        }

        wxString GetTypeString() const
        {
            if (this->type == FILE)
                return wxT("file");
            else
                return wxT("directory");
        }

        Type type;
        wxString name;        
    };

    /** A bit flag that has a name. */
    struct NamedFlag
    {
        NamedFlag()
        {
            this->bit = 0;
        }

        NamedFlag(const wxString& name, int bit)
        {
            this->name = name;
            this->bit = bit;
        }
        
        wxString name;
        uint8_t bit;
    };

    /** Determines how submeshes are named. */
    struct SubmeshNaming
    {
        enum Type
        {
            SUBMESH,
            MATERIAL,
            CUSTOM
        };

        SubmeshNaming()
        {
            this->type = SUBMESH;
        }

        wxString GetSubmeshName(const wxString& materialName) const
        {
            wxString name;
            switch (this->type)
            {
                case SUBMESH: name = wxT("submesh"); break;
                case MATERIAL: name = materialName; break;
                case CUSTOM: name = this->custom; break;
            }
            return name;
        }

        Type type;
        wxString custom;
    };

    /** Synchronization type for a value that has a potential source. */
    enum class ValueSourceSync
    {
        SEPARATE,
        FROM_MASTER_TO_SLAVE,
        FROM_SLAVE_TO_MASTER
    };
    
    /** The number of items that can be chosen. */
    enum class HowMany
    {
        /** Choose many items. */
        CHOOSE_MANY,

        /** Choose one item. */
        CHOOSE_ONE        
    };

    enum class FogMode
    {
        NONE,
        EXP, //Density increases  exponentially from the camera (fog = 1/e^(distance * density))        
        EXP2, //Density increases at the square of FOG_EXP, i.e. even quicker (fog = 1/e^(distance * density)^2)
        LINEAR //Density increases linearly between the start and end distances
    };

    template <typename T>
    struct DeleteThisDeleter
    {
        typedef void result_type;
    
        void operator () (T* obj) const
        {
            obj->DeleteThis();
        }
    };

    /** Rounds a floating point number to an integer. */
    inline int RoundToInt(double v)
    {
        return (v >= 0) ? (int)(v + .5) : (int)(v - .5);
    }

    /** Hashes a sequence of bytes. */
    inline size_t HashBytes(const void* s, size_t count)
    {
        auto str = static_cast<const uint8_t*>(s);

        size_t hash = 0;
        if (count > 0)
        {
            hash = 5381;
            for (size_t i = 0; i < count; i++)
                hash = ((hash << (size_t)5) + hash) + *str++; //hash * 33 + c
        }
        return hash;
    }
        
    /** Sorts all the values and removes the duplicates. */
    template <class T>
    void SortAndRemoveDuplicates(std::vector<T>& values)
    {
        std::vector<T> uniqueValues;
        uniqueValues.reserve(values.size());

        //Sort the times
        std::sort(values.begin(), values.end());

        //Remove duplicates
        for (size_t i = 0; i < values.size();)
        {
            uniqueValues.push_back(values[i]);

            size_t currentIndex = i++;
            while (i < values.size() && values[i] == values[currentIndex])
                i++;
        }

        //Return the unique times
        values.swap(uniqueValues);
    }

    /** Each item in the source collection is added to the destination by calling push_back() on the destination. */
    template <class T, class U>
    void PushAll(T& destination, const U& source)
    {
        for (auto& item : source)
            destination.push_back(item);
    }

    /** Each item in the source collection is added to the destination by calling insert() on the destination. */
    template <class T, class U>
    void InsertAll(T& destination, const U& source)
    {
        for (auto& item : source)
            destination.insert(item);
    }

    /** Determines if two sets of strings contain the same strings. */
    template <typename SetA, typename SetB>
    inline bool StringSetsSame(const SetA& a, const SetB& b)
    {
        if (a.size() != b.size())
            return false;

        for (auto i = a.begin(); i != a.end(); ++i)
        {
            if (b.find(*i) == b.end())
                return false;
        }

        return true;
    }

    class ApplicationListCtrl;
    class ApplicationChoiceCtrl;
    class ColorPickerControl;

} }


//Macros-----------------------------------------------------------------------
#define FINJIN_EXPORTER_NO_COPY_OR_MOVE(cls) \
    private: \
    cls(const cls& other) {} \
    cls& operator = (const cls& other) {return *this;} \
    cls(cls&& other) {} \
    cls& operator = (cls&& other) {return *this;}

/** Rounds a floating point number. */
#if defined(__WXMSW__)
    #define round(x) (floor((x) + .5))
#endif

#define FLOAT_EPSILON 1.1929093e-5


#if wxMAJOR_VERSION == 2 && wxMINOR_VERSION >= 9
    #define WxStringAsInternal(wx) (wx).wx_str().AsInternal()
#else
    #define WxStringAsInternal(wx) (wx).wx_str()
#endif

#if (wxMAJOR_VERSION > 2) || (wxMAJOR_VERSION == 2 && wxMINOR_VERSION >= 9)
    #define WxStringData(wx) (wx).data()
#else
    #define WxStringData(wx) (wx)
#endif

#if defined(__WXMSW__)
    #define APPLICATION_EXTENSION wxT(".exe")
    #define SHARED_LIBRARY_EXTENSION wxT(".dll")
#elif defined(__WXMAC__)
    #define APPLICATION_EXTENSION wxT(".app")
    #define SHARED_LIBRARY_EXTENSION wxT(".dylib")
#else
    #define APPLICATION_EXTENSION wxT("")
    #define SHARED_LIBRARY_EXTENSION wxT(".dso")
#endif

#if defined(_WIN64) || defined(__LP64__)
    #define APPLICATION_PLATFORM wxT("x64")
#else
    #if defined(__WXMSW__)
        #define APPLICATION_PLATFORM wxT("Win32")
    #else
        #define APPLICATION_PLATFORM wxT("x86")
    #endif
#endif

/** 
 * Creates a formatted string from a variable argument list. 
 * The formatting string's arguments are assumed to be in a variable argument list that starts after the 'format' parameter in a function signature.
 * @param result [out] - A previously declared string variable.
 * @param format [in] - A format string.
 */
#define FINJIN_EXPORTER_FORMAT_STRING_VA(result, format) \
    {\
        va_list args;\
        va_start(args, format);\
        result = wxString::FormatV(format, args);\
        va_end(args);\
    }

/** Same as FINJIN_EXPORTER_FORMAT_STRING_VA, except the result string is declared in this macro. */
#define FINJIN_EXPORTER_NEW_FORMAT_STRING_VA(result, format) \
    wxString result;\
    FINJIN_EXPORTER_FORMAT_STRING_VA(result, format)

/** Always use the built-in runtime type info. */
#define FINJIN_EXPORTER_DEFINE_SUPERCLASS(superClass) typedef superClass Super;
#define FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(object, class) (dynamic_cast<const class*>(object) != nullptr)


//Utilities--------------------------------------------------------------------
#include "ConvertString.hpp"

//Useful for debugging
#include "MethodEntry.hpp"
#include "Logger.hpp"

//Determine the host application
#define FINJIN_EXPORTER_HOST_3DSMAX 1
#define FINJIN_EXPORTER_HOST_MAYA 2
#define FINJIN_EXPORTER_HOST_MOTIONBUILDER 3

#if defined(MAX_VERSION_MAJOR)
    #define FINJIN_EXPORTER_HOST FINJIN_EXPORTER_HOST_3DSMAX
#elif defined(_MGlobal)
    #define FINJIN_EXPORTER_HOST FINJIN_EXPORTER_HOST_MAYA
#elif defined(MOTIONBUILDER_PROGRAM_VERSION)
    #define FINJIN_EXPORTER_HOST FINJIN_EXPORTER_HOST_MOTIONBUILDER
#else
    #error Unable to detect Finjin Exporter's host application type
#endif

#define FINJIN_EXPORTER_GLOBAL_SETTINGS_FILE_NAME wxT("finjin-exporter-global-settings.cfg")
#define FINJIN_EXPORTER_LOG_FILE_NAME wxT("finjin-exporter.log")

#define FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_STRING " "
#define FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_CHAR ' '

#define FINJIN_EXPORTER_MAX_COLOR_SETS 4 //If changed, must also change WxGpuVertexFormatStructMetadata (WxNumericStruct.hpp) and ExportedGeometry (ExportedGeometry.cpp)
#define FINJIN_EXPORTER_MAX_TEXTURE_COORD_SETS 8 //If changed, must also change WxGpuVertexFormatStructMetadata (WxNumericStruct.hpp) and ExportedGeometry (ExportedGeometry.cpp)

#define FINJIN_MAX_BLEND_WEIGHTS 16
    
/** Minimum bytes per line in exported files. */
#define FINJIN_EXPORTER_MIN_BYTES_PER_LINE 1024
#define FINJIN_EXPORTER_MAX_BYTES_PER_LINE (1024 * 1024)

/** Default bytes per line in exported files. */
#define FINJIN_EXPORTER_DEFAULT_BYTES_PER_LINE 1024

/** Default maximum distance. */
#define FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE 100000.0f

/** Default maximum distance used in spinner controls. */
#define FINJIN_EXPORTER_DEFAULT_MAX_DISTANCE_RANGE  10000000.0f

#define FINJIN_EXPORTER_MAX_FLAG_BITS 256
