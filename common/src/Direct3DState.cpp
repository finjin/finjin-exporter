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
#include "Direct3DState.hpp"
#include "ApplicationAccessor.hpp"

#if defined(FINJIN_EXPORTER_ENABLE_D3D_FUNCTIONALITY)

using namespace Finjin::Exporter;


//Implementation----------------------------------------------------------------
Direct3DState::Direct3DState()
{
    this->d3d = nullptr;
    this->d3dDevice = nullptr;
}

Direct3DState::~Direct3DState()
{
    Destroy();
}

bool Direct3DState::Create()
{
    auto hWnd = (HWND)ApplicationAccessor::GetMainWindow();

    //Initialize D3D
    this->d3d = Direct3DCreate9(D3D_SDK_VERSION);

    //Initialized D3D device
    D3DPRESENT_PARAMETERS d3dPresent;
    FINJIN_ZERO_ITEM(d3dPresent);
    d3dPresent.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dPresent.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dPresent.hDeviceWindow = hWnd;
    d3dPresent.Windowed = TRUE;
    auto hr = this->d3d->CreateDevice
        (
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE,
        &d3dPresent,
        &this->d3dDevice
        );

    return SUCCEEDED(hr) ? true : false;
}

void Direct3DState::Destroy()
{
    FINJIN_SAFE_RELEASE(this->d3dDevice);
    FINJIN_SAFE_RELEASE(this->d3d);
}

#endif
