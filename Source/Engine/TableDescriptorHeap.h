#pragma once

class CTableDescriptorHeap
{
public:
	CTableDescriptorHeap();
	~CTableDescriptorHeap();

public:
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap()	{ return m_DescHeap; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER reg);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(SRV_REGISTER reg);

public:
	int Init(UINT count);

	void Clear();
	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg);

	void CommitTable();

private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(unsigned char reg);

private:
	ComPtr<ID3D12DescriptorHeap> m_DescHeap;
	UINT64	m_HandleSize;
	UINT64	m_GroupSize;
	UINT64	m_GroupCount;

	UINT	m_CurrentGroupIndex;
};

