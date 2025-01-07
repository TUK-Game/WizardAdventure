#pragma once

class CSwapChain
{
public:
	CSwapChain();
	~CSwapChain();

public:
	ComPtr<IDXGISwapChain> GetSwapChain() { return m_SwapChain; }
	ComPtr<ID3D12Resource> GetRenderTarget(int index) { return m_RTVBuffer[index]; }

	ComPtr<ID3D12Resource> GetBackRTVBuffer() { return m_RTVBuffer[m_BackBufferIndex]; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTV() { return m_RTVHandle[m_BackBufferIndex]; }

public:
	int Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void Present();
	void SwapIndex();

private:
	int CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	int CreateRTV(ComPtr<ID3D12Device> device);

private:
	ComPtr<IDXGISwapChain>	m_SwapChain;

	ComPtr<ID3D12Resource>			m_RTVBuffer[(UINT)SWAP_CHAIN_BUFFER_COUNT];
	ComPtr<ID3D12DescriptorHeap>	m_RTVHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		m_RTVHandle[(UINT)SWAP_CHAIN_BUFFER_COUNT];

	UINT					m_BackBufferIndex = 0;
};

