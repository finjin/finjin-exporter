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


#include <Windows.h>

//Max includes
#define MAX_VERSION_MAJOR_2017 19
#define MAX_VERSION_MAJOR_2016 18
#define MAX_VERSION_MAJOR_2015 17
#define MAX_VERSION_MAJOR_2014 16
#define MAX_VERSION_MAJOR_2013 15
#define MAX_VERSION_MAJOR_2012 14
#define MAX_VERSION_MAJOR_2011 13
#define MAX_VERSION_MAJOR_2010 12
#define MAX_VERSION_MAJOR_2009 11
#define MAX_VERSION_MAJOR_2008 10
#define MAX_VERSION_MAJOR_9 9
#define MAX_VERSION_MAJOR_8 8

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#include <Max.h>
#include <plugapi.h>
#include <stdmat.h>
#include <impexp.h>
#include <iparamb2.h>
#include <modstack.h>
#include <CS/IMixer8.h> 
#include <CS/BipedApi.h>
#include <CS/KeyTrack.h>
#include <CS/phyexp.h>
#include <iskin.h>
#include <tab.h>
#include <notetrck.h>
#include <istdplug.h>
#include <iparamm2.h>
#include <guplib.h>
#include <imenuman.h>
#include <maxscript\maxscript.h>
#include <maxscript\util\listener.h>
#include <Notify.h>
#include <iFnPub.h>
#include <ActionTable.h>
#include <iMenuMan.h>
#include <ParticleFlow/PFClassIDs.h>
#include <gport.h>
#include <hsv.h>
#include <iColorMan.h>
#include <bmmlib.h>
#include <mnmath.h>
#include <decomp.h>
#include <MeshNormalSpec.h> 
#include <simpmod.h>
#undef min
#undef PI

namespace Max
{
    static const float PI = 3.1415926535f;
}

//#define FINJIN_EXPORTER_ENABLE_D3D_FUNCTIONALITY 1

#if defined(FINJIN_EXPORTER_ENABLE_D3D_FUNCTIONALITY)
    #include <d3d9.h>
    #include <d3dx9.h>
#endif

//Finjin includes
#include "CommonTypes.hpp"


//Macros-----------------------------------------------------------------------
#define EXPAND_APP_XYZ(value) \
    (value).x,\
    (value).y,\
    (value).z

#define APP_VERSION_NAME wxString::Format(wxT("3DSMax%d-%s"), MAX_PRODUCT_YEAR_NUMBER, APPLICATION_PLATFORM)

#define APP_TIME_RESOLUTION_UNIT WxTimeDurationUnit::MILLISECONDS

