#pragma once

#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "DepthStencilBuffer.h"

class CDevice :
	public CSingleton<CDevice>
{
	DECLARE_SINGLETON(CDevice)

public:
	ComPtr<IDXGIFactory> GetDXGI()		{ return m_DXGI; }
	ComPtr<ID3D12Device> GetDevice()	{ return m_Device; }

	std::shared_ptr<CCommandQueue> GetCmdQueue()					{ return m_CmdQueue; }
	std::shared_ptr<CSwapChain> GetSwapChain()						{ return m_SwapChain; }
	std::shared_ptr<CRootSignature> GetRootSignature()				{ return m_RootSignature; }
	std::shared_ptr<CTableDescriptorHeap> GetTableDescHeap()		{ return m_TableDescHeap; }
	std::shared_ptr<CDepthStencilBuffer> GetDepthStencilBuffer()	{ return m_DepthStencilBuffer; }

	std::shared_ptr<CConstantBuffer> GetConstantBuffer(EConstantBuffer_Type type) { return m_ConstantBuffers[static_cast<unsigned char>(type)]; }

public:
	int Init();

	void RenderBegin();
	void RenderEnd();

private:
	void CreateConstantBuffer(CBV_REGISTER reg, UINT bufferSize, UINT count);

private:
	D3D12_VIEWPORT	m_Viewport;
	D3D12_RECT		m_ScissorRect;

	ComPtr<ID3D12Debug>			m_DebugController;
	ComPtr<IDXGIFactory>		m_DXGI;
	ComPtr<ID3D12Device>		m_Device;

	std::shared_ptr<CCommandQueue>			m_CmdQueue;
	std::shared_ptr<CSwapChain>				m_SwapChain;
	std::shared_ptr<CRootSignature>			m_RootSignature;
	std::shared_ptr<CTableDescriptorHeap>	m_TableDescHeap;
	std::shared_ptr<CDepthStencilBuffer>	m_DepthStencilBuffer;

	std::vector<std::shared_ptr<CConstantBuffer>>	m_ConstantBuffers;
};

