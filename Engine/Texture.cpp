#include "pch.h"
#include "Texture.h"
#include "Engine.h"

void Texture::Init(const wstring& path)
{
	CreateTexture(path);
	CreateView();
}

void Texture::CreateTexture(const wstring& path)
{
    wstring ext = fs::path(path).extension();

    HRESULT hr;
    if (ext == L".dds" || ext == L".DDS")
        hr = ::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, _image);
    else if (ext == L".tga" || ext == L".TGA")
        hr = ::LoadFromTGAFile(path.c_str(), nullptr, _image);
    else // png, jpg, jpeg, bmp
        hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, _image);

    if (FAILED(hr))
    {
        ::MessageBoxA(nullptr, "Failed to load texture file!", nullptr, MB_OK);
        assert(nullptr);
    }

    hr = ::CreateTexture(DEVICE.Get(), _image.GetMetadata(), &_tex2D);
    if (FAILED(hr))
    {
        ::MessageBoxA(nullptr, "Failed to create texture resource!", nullptr, MB_OK);
        assert(nullptr);
    }

    vector<D3D12_SUBRESOURCE_DATA> subResources;
    hr = ::PrepareUpload(DEVICE.Get(),
        _image.GetImages(),
        _image.GetImageCount(),
        _image.GetMetadata(),
        subResources);

    if (FAILED(hr))
    {
        ::MessageBoxA(nullptr, "Failed to prepare texture upload!", nullptr, MB_OK);
        assert(nullptr);
    }

    const uint64 bufferSize = ::GetRequiredIntermediateSize(_tex2D.Get(), 0, static_cast<uint32>(subResources.size()));

    D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

    ComPtr<ID3D12Resource> textureUploadHeap;
    hr = DEVICE->CreateCommittedResource(
        &heapProperty,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(textureUploadHeap.GetAddressOf()));

    if (FAILED(hr))
    {
        ::MessageBoxA(nullptr, "Failed to create upload heap!", nullptr, MB_OK);
        assert(nullptr);
    }

    ::UpdateSubresources(RESOURCE_CMD_LIST.Get(),
        _tex2D.Get(),
        textureUploadHeap.Get(),
        0, 0,
        static_cast<unsigned int>(subResources.size()),
        subResources.data());

    GEngine->GetCmdQueue()->FlushResourceCommandQueue();
}

void Texture::CreateView()
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap));

	_srvHandle = _srvHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = _image.GetMetadata().format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;
	DEVICE->CreateShaderResourceView(_tex2D.Get(), &srvDesc, _srvHandle);
}