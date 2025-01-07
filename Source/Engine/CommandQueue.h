#pragma once

class CSwapChain;
class CDescriptorHeap;

class CCommandQueue
{
public:
	CCommandQueue();
	~CCommandQueue();

public:
	ComPtr<ID3D12CommandQueue> GetCmdQueue()				{ return m_CmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList()			{ return m_CmdList; }
	ComPtr<ID3D12GraphicsCommandList> GetResourceCmdList()	{ return m_ResCmdList; }

public:
	int Init(ComPtr<ID3D12Device> device, std::shared_ptr<CSwapChain> swapChain);
	void WaitSync();

	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	void FlushResourceCommandQueue();

private:
	ComPtr<ID3D12CommandQueue>			m_CmdQueue;
	ComPtr<ID3D12CommandAllocator>		m_CmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	m_CmdList;

	ComPtr<ID3D12CommandAllocator>		m_ResCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	m_ResCmdList;

	ComPtr<ID3D12Fence>					m_Fence;
	UINT								m_FenceValue;
	HANDLE								m_FenceEvent;

	std::shared_ptr<CSwapChain>			m_SwapChain;
};

