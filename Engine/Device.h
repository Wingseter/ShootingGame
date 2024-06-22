#pragma once

class Device
{
public:
    // Initializes the device
    void Init();

    // Returns the DXGI factory interface
    ComPtr<IDXGIFactory> GetDXGI() { return _dxgi; }

    // Returns the D3D12 device interface
    ComPtr<ID3D12Device> GetDevice() { return _device; }

private:
    // COM (Component Object Model)
    // - A technology that enables language independence and backward compatibility in DirectX
    // - Uses COM objects (COM interfaces). Details are hidden from us
    // - ComPtr is a kind of smart pointer
    ComPtr<ID3D12Debug> _debugController; // Debug controller for D3D12
    ComPtr<IDXGIFactory> _dxgi; // Handles screen-related functionalities
    ComPtr<ID3D12Device> _device; // Creates various objects
};
