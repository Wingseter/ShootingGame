#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
	if (_cbvBuffer)
	{
		if (_cbvBuffer != nullptr)
			_cbvBuffer->Unmap(0, nullptr);

		_cbvBuffer = nullptr;
	}
}


void ConstantBuffer::Init(uint32 size, uint32 count)
{
	// 상수 버퍼는 256 바이트 배수로 만들어야 한다
	// 0 256 512 768
	_elementSize = (size + 255) & ~255;
	_elementCount = count;

	CreateBuffer();
	CreateView();
}

void ConstantBuffer::CreateBuffer()
{
	// Calculate the buffer size (element size * element count)
	uint32 bufferSize = _elementSize * _elementCount;

	// Set heap properties (upload heap type)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// Set resource description (buffer type, buffer size)
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	// Create a committed resource (create buffer resource in upload heap)
	DEVICE->CreateCommittedResource(
		&heapProperty,                     // Heap properties
		D3D12_HEAP_FLAG_NONE,              // Heap flags (none)
		&desc,                             // Resource description
		D3D12_RESOURCE_STATE_GENERIC_READ, // Initial resource state (generic read)
		nullptr,                           // Clear value (none)
		IID_PPV_ARGS(&_cbvBuffer));        // Store created resource in _cbvBuffer

	// Map the buffer (map to CPU memory space)
	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));

	// No need to unmap until done with the resource. However, we must not write to
	// the resource while it is in use by the GPU (synchronization techniques must be used).
}



void ConstantBuffer::CreateView()
{
	// Create a descriptor heap for the constant buffer views (CBVs)
	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc = {};
	cbvDesc.NumDescriptors = _elementCount;  // Number of descriptors to allocate
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;  // No specific flags
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;  // Descriptor heap type for CBV, SRV, and UAV
	DEVICE->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&_cbvHeap));  // Create the descriptor heap

	// Get the CPU descriptor handle for the start of the heap
	_cpuHandleBegin = _cbvHeap->GetCPUDescriptorHandleForHeapStart();
	// Get the size of each descriptor in the heap
	_handleIncrementSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Loop through each element to create individual constant buffer views (CBVs)
	for (uint32 i = 0; i < _elementCount; ++i)
	{
		// Get the CPU descriptor handle for the current index
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = GetCpuHandle(i);

		// Set up the constant buffer view description
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = _cbvBuffer->GetGPUVirtualAddress() + static_cast<uint64>(_elementSize) * i;  // GPU virtual address for the buffer
		cbvDesc.SizeInBytes = _elementSize;  // Size of the buffer in bytes

		// Create the constant buffer view in the descriptor heap
		DEVICE->CreateConstantBufferView(&cbvDesc, cbvHandle);
	}
}

void ConstantBuffer::Clear()
{
	_currentIndex = 0;
}

D3D12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{
	// Ensure the current index is within the valid range (bounds check)
	assert(_currentIndex < _elementSize);

	// Copy the data to the mapped buffer at the current index position
	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	// Get the GPU virtual address for the current index position
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = GetCpuHandle(_currentIndex);

	// Increment the current index (move to the next position)
	_currentIndex++;

	return cpuHandle;
}



D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * _elementSize;
	return objCBAddress;
}

D3D12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::GetCpuHandle(uint32 index)
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(_cpuHandleBegin, index * _handleIncrementSize);
}
