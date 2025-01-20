#pragma once

#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "RenderTargetGroup.h"

class CDevice :
	public CSingleton<CDevice>
{
	DECLARE_SINGLETON(CDevice)

public:
	ComPtr<IDXGIFactory> GetDXGI()		{ return m_DXGI; }
	ComPtr<ID3D12Device> GetDevice()	{ return m_Device; }

	std::shared_ptr<CGraphicsCommandQueue> GetCmdQueue()	{ return m_GraphicsCmdQueue; }
	std::shared_ptr<CComputeCommandQueue> GetComputeCmdQueue()	{ return m_ComputeCmdQueue; }
	std::shared_ptr<CSwapChain> GetSwapChain()						{ return m_SwapChain; }
	std::shared_ptr<CRootSignature> GetRootSignature()				{ return m_RootSignature; }
	std::shared_ptr<CGraphicsDescriptorHeap> GetGraphicsDescHeap()		{ return m_GraphicsDescHeap; }
	std::shared_ptr<CComputeDescriptorHeap> GetComputeDescHeap()		{ return m_ComputeDescHeap; }

	std::shared_ptr<CConstantBuffer> GetConstantBuffer(EConstantBuffer_Type type) { return m_ConstantBuffers[static_cast<unsigned char>(type)]; }
	std::shared_ptr<CRenderTargetGroup> GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE type) { return m_RenderTargetGroups[static_cast<unsigned char>(type)]; }
public:
	int Init();

	void RenderBegin();
	void RenderEnd();

private:
	void CreateConstantBuffer(CBV_REGISTER reg, UINT bufferSize, UINT count);
	void CreateRenderTargetGroups();

private:
	D3D12_VIEWPORT	m_Viewport;
	D3D12_RECT		m_ScissorRect;

	ComPtr<ID3D12Debug>			m_DebugController;
	ComPtr<IDXGIFactory>		m_DXGI;
	ComPtr<ID3D12Device>		m_Device;

	std::shared_ptr<CGraphicsCommandQueue>		m_GraphicsCmdQueue;
	std::shared_ptr<CComputeCommandQueue>		m_ComputeCmdQueue;
	std::shared_ptr<CSwapChain>					m_SwapChain;
	std::shared_ptr<CRootSignature>				m_RootSignature;
	std::shared_ptr<CGraphicsDescriptorHeap>	m_GraphicsDescHeap;
	std::shared_ptr<CComputeDescriptorHeap>		m_ComputeDescHeap;
	

	std::vector<std::shared_ptr<CConstantBuffer>>	m_ConstantBuffers;
	std::array<std::shared_ptr<CRenderTargetGroup>, RENDER_TARGET_GROUP_COUNT> m_RenderTargetGroups;
};

