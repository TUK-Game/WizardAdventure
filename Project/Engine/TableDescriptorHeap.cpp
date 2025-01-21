#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Device.h"

CGraphicsDescriptorHeap::CGraphicsDescriptorHeap()
	: m_HandleSize(0)
	, m_GroupSize(0)
	, m_GroupCount(0)
	, m_CurrentGroupIndex(0)
{
}

CGraphicsDescriptorHeap::~CGraphicsDescriptorHeap()
{
}

int CGraphicsDescriptorHeap::Init(UINT count)
{
	m_GroupCount = count;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = count * (CBV_SRV_REGISTER_COUNT - 1);
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	if (FAILED(DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_DescHeap))))
		return E_FAIL;

	m_HandleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_GroupSize = m_HandleSize * (CBV_SRV_REGISTER_COUNT - 1);

	return S_OK;
}

void CGraphicsDescriptorHeap::Clear()
{
	m_CurrentGroupIndex = 0;
}

void CGraphicsDescriptorHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	UINT destRange = 1;
	UINT srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CGraphicsDescriptorHeap::SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	UINT destRange = 1;
	UINT srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CGraphicsDescriptorHeap::CommitTable()
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = m_DescHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += m_CurrentGroupIndex * m_GroupSize;
	GRAPHICS_CMD_LIST->SetGraphicsRootDescriptorTable(1, handle);

	m_CurrentGroupIndex++;
}

D3D12_CPU_DESCRIPTOR_HANDLE CGraphicsDescriptorHeap::GetCPUHandle(CBV_REGISTER reg)
{
	return GetCPUHandle(static_cast<unsigned char>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE CGraphicsDescriptorHeap::GetCPUHandle(SRV_REGISTER reg)
{
	return GetCPUHandle(static_cast<unsigned char>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE CGraphicsDescriptorHeap::GetCPUHandle(unsigned char reg)
{
	assert(reg > 0);
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_DescHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += m_CurrentGroupIndex * m_GroupSize;
	handle.ptr += (reg - 1) * m_HandleSize;
	return handle;
}

// ************************
// ComputeDescriptorHeap
// ************************

CComputeDescriptorHeap::CComputeDescriptorHeap()
	: m_HandleSize(0)
{
}

CComputeDescriptorHeap::~CComputeDescriptorHeap()
{
}

int CComputeDescriptorHeap::Init()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = TOTAL_REGISTER_COUNT;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	if (FAILED(DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_DescHeap))))
		return E_FAIL;

	m_HandleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return S_OK;
}

void CComputeDescriptorHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	UINT destRange = 1;
	UINT srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CComputeDescriptorHeap::SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	UINT destRange = 1;
	UINT srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CComputeDescriptorHeap::SetUAV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, UAV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	UINT destRange = 1;
	UINT srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// TODO : ���ҽ� ���� ����
}

void CComputeDescriptorHeap::CommitTable()
{
	ID3D12DescriptorHeap* descHeap = m_DescHeap.Get();
	COMPUTE_CMD_LIST->SetDescriptorHeaps(1, &descHeap);

	D3D12_GPU_DESCRIPTOR_HANDLE handle = descHeap->GetGPUDescriptorHandleForHeapStart();
	COMPUTE_CMD_LIST->SetComputeRootDescriptorTable(0, handle);
}

D3D12_CPU_DESCRIPTOR_HANDLE CComputeDescriptorHeap::GetCPUHandle(CBV_REGISTER reg)
{
	return GetCPUHandle(static_cast<unsigned char>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE CComputeDescriptorHeap::GetCPUHandle(SRV_REGISTER reg)
{
	return GetCPUHandle(static_cast<unsigned char>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE CComputeDescriptorHeap::GetCPUHandle(UAV_REGISTER reg)
{
	return GetCPUHandle(static_cast<unsigned char>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE CComputeDescriptorHeap::GetCPUHandle(unsigned char reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_DescHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += reg * m_HandleSize;
	return handle;
}
