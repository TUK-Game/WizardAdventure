#include "pch.h"
#include "ConstantBuffer.h"
#include "Device.h"

CConstantBuffer::CConstantBuffer()
	: m_MappedBuffer(nullptr)
	, m_ElementSize(0)
	, m_ElementCount(0)
	, m_CPUHandleBegin{}
	, m_HandleIncrementSize(0)
	, m_CurrentIndex(0)
	, m_Register{}
{
}

CConstantBuffer::~CConstantBuffer()
{
	if (m_CBVBuffer)
	{
		if (m_CBVBuffer != nullptr)
			m_CBVBuffer->Unmap(0, nullptr);

		m_CBVBuffer = nullptr;
	}
}

int CConstantBuffer::Init(CBV_REGISTER reg, UINT size, UINT count)
{
	m_Register = reg;

	// 상수 버퍼는 256 바이트 배수로 만들어야 한다
	// 0 256 512 768
	m_ElementSize = (size + 255) & ~255;
	m_ElementCount = count;

	if (FAILED(CreateBuffer()))
		return false;

	if (FAILED(CreateView()))
		return false;

	return S_OK;
}

int CConstantBuffer::CreateBuffer()
{
	UINT bufferSize = m_ElementSize * m_ElementCount;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	if (FAILED(DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_CBVBuffer))))
	{
		return E_FAIL;
	}

	m_CBVBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedBuffer));
	// We do not need to unmap until we are done with the resource.  However, we must not write to
	// the resource while it is in use by the GPU (so we must use synchronization techniques).

	return S_OK;
}

int CConstantBuffer::CreateView()
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc = {};
	cbvDesc.NumDescriptors = m_ElementCount;
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	if (FAILED(DEVICE->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&m_CBVHeap))))
		return E_FAIL;

	m_CPUHandleBegin = m_CBVHeap->GetCPUDescriptorHandleForHeapStart();
	m_HandleIncrementSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (UINT i = 0; i < m_ElementCount; ++i)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = GetSRVCpuHandle(i);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_CBVBuffer->GetGPUVirtualAddress() + static_cast<UINT64>(m_ElementSize) * i;
		cbvDesc.SizeInBytes = m_ElementSize;   // CB size is required to be 256-byte aligned.

		DEVICE->CreateConstantBufferView(&cbvDesc, cbvHandle);
	}

	return S_OK;
}

void CConstantBuffer::Clear()
{
	m_CurrentIndex = 0;
}

void CConstantBuffer::PushGraphicsData(void* buffer, UINT size)
{
	assert(m_CurrentIndex < m_ElementCount);
	assert(m_ElementSize == ((size + 255) & ~255));

	::memcpy(&m_MappedBuffer[m_CurrentIndex * m_ElementSize], buffer, size);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = GetSRVCpuHandle(m_CurrentIndex);
	CDevice::GetInst()->GetGraphicsDescHeap()->SetCBV(cpuHandle, m_Register);

	m_CurrentIndex++;
}

void CConstantBuffer::PushComputeData(void* buffer, UINT size)
{
	assert(m_CurrentIndex < m_ElementCount);
	assert(m_ElementSize == ((size + 255) & ~255));

	::memcpy(&m_MappedBuffer[m_CurrentIndex * m_ElementSize], buffer, size);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = GetSRVCpuHandle(m_CurrentIndex);
	CDevice::GetInst()->GetComputeDescHeap()->SetCBV(cpuHandle, m_Register);

	m_CurrentIndex++;
}

void CConstantBuffer::SetGlobalData(void* buffer, UINT size)
{
	assert(m_ElementSize == ((size + 255) & ~255));

	::memcpy(&m_MappedBuffer[0], buffer, size);

	CMD_LIST->SetGraphicsRootConstantBufferView(0, GetGpuVirtualAddress(0));
}


D3D12_GPU_VIRTUAL_ADDRESS CConstantBuffer::GetGpuVirtualAddress(UINT index)
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = m_CBVBuffer->GetGPUVirtualAddress();
	objCBAddress += index * m_ElementSize;
	return objCBAddress;
}

D3D12_CPU_DESCRIPTOR_HANDLE CConstantBuffer::GetSRVCpuHandle(UINT index)
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_CPUHandleBegin, index * m_HandleIncrementSize);
}