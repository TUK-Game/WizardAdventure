#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Device.h"

CTableDescriptorHeap::CTableDescriptorHeap()
	: m_HandleSize(0)
	, m_GroupSize(0)
	, m_GroupCount(0)
	, m_CurrentGroupIndex(0)
{
}

CTableDescriptorHeap::~CTableDescriptorHeap()
{
}

int CTableDescriptorHeap::Init(UINT count)
{
	m_GroupCount = count;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = count * REGISTER_COUNT;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	if (FAILED(DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_DescHeap))))
		return E_FAIL;

	m_HandleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_GroupSize = m_HandleSize * REGISTER_COUNT;

	return S_OK;
}

void CTableDescriptorHeap::Clear()
{
	m_CurrentGroupIndex = 0;
}

void CTableDescriptorHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	UINT destRange = 1;
	UINT srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CTableDescriptorHeap::SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	UINT destRange = 1;
	UINT srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CTableDescriptorHeap::CommitTable()
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = m_DescHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += m_CurrentGroupIndex * m_GroupSize;
	CMD_LIST->SetGraphicsRootDescriptorTable(0, handle);

	m_CurrentGroupIndex++;
}

D3D12_CPU_DESCRIPTOR_HANDLE CTableDescriptorHeap::GetCPUHandle(CBV_REGISTER reg)
{
	return GetCPUHandle(static_cast<unsigned char>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE CTableDescriptorHeap::GetCPUHandle(SRV_REGISTER reg)
{
	return GetCPUHandle(static_cast<unsigned char>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE CTableDescriptorHeap::GetCPUHandle(unsigned char reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_DescHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += m_CurrentGroupIndex * m_GroupSize;
	handle.ptr += reg * m_HandleSize;
	return handle;
}