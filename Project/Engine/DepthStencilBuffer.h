#pragma once

class CDepthStencilBuffer
{
public:
	CDepthStencilBuffer();
	~CDepthStencilBuffer();
	
public:
	D3D12_CPU_DESCRIPTOR_HANDLE	GetDSVCpuHandle()	{ return m_DSVHandle; }
	DXGI_FORMAT GetDSVFormat()						{ return m_DSVFormat; }

public:
	int Init(const WindowInfo& window, DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT);

private:
	// Depth Stencil View
	ComPtr<ID3D12Resource>				m_DSVBuffer;
	ComPtr<ID3D12DescriptorHeap>		m_DSVHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			m_DSVHandle;
	DXGI_FORMAT							m_DSVFormat;
};

