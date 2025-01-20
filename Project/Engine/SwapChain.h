#pragma once

class CSwapChain
{
public:
	CSwapChain();
	~CSwapChain();

public:
	ComPtr<IDXGISwapChain> GetSwapChain() { return m_SwapChain; }
	UINT GetBackBufferIndex() { return m_BackBufferIndex; }
public:
	int Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void Present();
	void SwapIndex();

private:
	int CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);

private:
	ComPtr<IDXGISwapChain>	m_SwapChain;

	UINT					m_BackBufferIndex = 0;
};

