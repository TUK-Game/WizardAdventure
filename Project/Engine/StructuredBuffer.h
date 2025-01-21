#pragma once

class CStructuredBuffer
{
public:
	CStructuredBuffer();
	~CStructuredBuffer();

	void Init(UINT32 elementSize, UINT32 elementCount);

	void PushGraphicsData(SRV_REGISTER reg);
	void PushComputeSRVData(SRV_REGISTER reg);
	void PushComputeUAVData(UAV_REGISTER reg);

	ComPtr<ID3D12DescriptorHeap> GetSRV() { return m_SrvHeap; }
	ComPtr<ID3D12DescriptorHeap> GetUAV() { return m_UavHeap; }

	void SetResourceState(D3D12_RESOURCE_STATES state) { m_ResourceState = state; }
	D3D12_RESOURCE_STATES GetResourceState() { return m_ResourceState; }
	ComPtr<ID3D12Resource> GetBuffer() { return m_Buffer; }

private:
	ComPtr<ID3D12Resource>			m_Buffer;
	ComPtr<ID3D12DescriptorHeap>	m_SrvHeap;
	ComPtr<ID3D12DescriptorHeap>	m_UavHeap;

	UINT32						m_ElementSize = 0;
	UINT32						m_ElementCount = 0;
	D3D12_RESOURCE_STATES		m_ResourceState = {};

private:
	D3D12_CPU_DESCRIPTOR_HANDLE m_SrvHeapBegin = {};
	D3D12_CPU_DESCRIPTOR_HANDLE m_UavHeapBegin = {};
};

