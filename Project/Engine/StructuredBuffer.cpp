#include "pch.h"
#include "StructuredBuffer.h"
#include "Device.h"

CStructuredBuffer::CStructuredBuffer()
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

void CStructuredBuffer::Init(UINT32 elementSize, UINT32 elementCount, void* initialData)
{
	m_ElementSize = elementSize;
	m_ElementCount = elementCount;
	m_ResourceState = D3D12_RESOURCE_STATE_COMMON;

	// Buffer
	{
		UINT64 bufferSize = static_cast<UINT64>(m_ElementSize) * m_ElementCount;
		D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
		D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		CDevice::GetInst()->GetDevice()->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			m_ResourceState,
			nullptr,
			IID_PPV_ARGS(&m_Buffer));

		if (initialData)
			CopyInitialData(bufferSize, initialData);
	}

	// SRV
	{
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_SrvHeap));

		m_SrvHeapBegin = m_SrvHeap->GetCPUDescriptorHandleForHeapStart();

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = m_ElementCount;
		srvDesc.Buffer.StructureByteStride = m_ElementSize;
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

		DEVICE->CreateShaderResourceView(m_Buffer.Get(), &srvDesc, m_SrvHeapBegin);
	}

	// UAV
	{
		D3D12_DESCRIPTOR_HEAP_DESC uavheapDesc = {};
		uavheapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		uavheapDesc.NumDescriptors = 1;
		uavheapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DEVICE->CreateDescriptorHeap(&uavheapDesc, IID_PPV_ARGS(&m_UavHeap));

		m_UavHeapBegin = m_UavHeap->GetCPUDescriptorHandleForHeapStart();

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = m_ElementCount;
		uavDesc.Buffer.StructureByteStride = m_ElementSize;
		uavDesc.Buffer.CounterOffsetInBytes = 0;
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

		DEVICE->CreateUnorderedAccessView(m_Buffer.Get(), nullptr, &uavDesc, m_UavHeapBegin);
	}
}

void CStructuredBuffer::PushGraphicsData(SRV_REGISTER reg)
{
	CDevice::GetInst()->GetGraphicsDescHeap()->SetSRV(m_SrvHeapBegin, reg);
}

void CStructuredBuffer::PushComputeSRVData(SRV_REGISTER reg)
{
	CDevice::GetInst()->GetComputeDescHeap()->SetSRV(m_SrvHeapBegin, reg);
}

void CStructuredBuffer::PushComputeUAVData(UAV_REGISTER reg)
{
	CDevice::GetInst()->GetComputeDescHeap()->SetUAV(m_UavHeapBegin, reg);
}

void CStructuredBuffer::CopyInitialData(UINT64 bufferSize, void* InitialData)
{
	ComPtr<ID3D12Resource> readBuffer = nullptr;
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize, D3D12_RESOURCE_FLAG_NONE);
	D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	DEVICE->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&readBuffer));

	UINT8* dataBegin = nullptr;
	D3D12_RANGE readRange{ 0, 0 };
	readBuffer->Map(0, &readRange, reinterpret_cast<void**>(&dataBegin));
	memcpy(dataBegin, InitialData, bufferSize);
	readBuffer->Unmap(0, nullptr);

	// Common -> Copy
	{
		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_Buffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);

		RESOURCE_CMD_LIST->ResourceBarrier(1, &barrier);
	}

	RESOURCE_CMD_LIST->CopyBufferRegion(m_Buffer.Get(), 0, readBuffer.Get(), 0, bufferSize);

	// Copy -> Common
	{
		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_Buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON);
		RESOURCE_CMD_LIST->ResourceBarrier(1, &barrier);
	}


	CDevice::GetInst()->GetCmdQueue()->FlushResourceCommandQueue();

	m_ResourceState = D3D12_RESOURCE_STATE_COMMON;
}
