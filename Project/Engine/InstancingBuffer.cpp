#include "pch.h"
#include "InstancingBuffer.h"
#include "Device.h"

CInstancingBuffer::CInstancingBuffer()
{
}

CInstancingBuffer::~CInstancingBuffer()
{
}

void CInstancingBuffer::Init(UINT32 maxCount)
{
	m_MaxCount = maxCount;

	const UINT32 bufferSize = sizeof(InstancingParams) * maxCount;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_Buffer));
}

void CInstancingBuffer::Clear()
{
	m_Data.clear();
}

void CInstancingBuffer::AddData(InstancingParams& params)
{
	m_Data.push_back(params);
}

void CInstancingBuffer::PushData()
{
	const UINT32 dataCount = GetCount();
	if (dataCount > m_MaxCount)
		Init(dataCount);

	const UINT32 bufferSize = dataCount * sizeof(InstancingParams);

	void* dataBuffer = nullptr;
	D3D12_RANGE readRange{ 0, 0 };
	m_Buffer->Map(0, &readRange, &dataBuffer);
	memcpy(dataBuffer, &m_Data[0], bufferSize);
	m_Buffer->Unmap(0, nullptr);

	m_BufferView.BufferLocation = m_Buffer->GetGPUVirtualAddress();
	m_BufferView.StrideInBytes = sizeof(InstancingParams);
	m_BufferView.SizeInBytes = bufferSize;
}