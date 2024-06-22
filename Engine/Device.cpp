#include "pch.h"
#include "Device.h"

void Device::Init()
{
    // Enable the D3D12 debug layer
    // - Outputs detailed debugging messages to the VC++ output window
    // - riid : COM ID of the device
    // - ppDevice : The created device is set in the parameter
#ifdef _DEBUG
    ::D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
    _debugController->EnableDebugLayer();
#endif

    // DXGI (DirectX Graphics Infrastructure)
    // API used with Direct3D
    // - Fullscreen mode switching
    // - Enumerating supported display modes, etc.
    // CreateDXGIFactory
    // - riid : COM ID of the device
    // - ppDevice : The created device is set in the parameter
    ::CreateDXGIFactory(IID_PPV_ARGS(&_dxgi));

    // CreateDevice
    // - Represents an object for the display adapter (graphics card)
    // - pAdapter : Specifies nullptr to use the system's default display adapter
    // - MinimumFeatureLevel : The minimum feature level required by the application (filters out older hardware)
    // - riid : COM ID of the device
    // - ppDevice : The created device is set in the parameter
    ::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));
}
