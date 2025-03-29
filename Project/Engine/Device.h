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
	Vec2 GetViewportSize() { return Vec2(m_Viewport.Width, m_Viewport.Height); }
	std::shared_ptr<CGraphicsCommandQueue> GetCmdQueue()	{ return m_GraphicsCmdQueue; }
	std::shared_ptr<CComputeCommandQueue> GetComputeCmdQueue()	{ return m_ComputeCmdQueue; }
	std::shared_ptr<CSwapChain> GetSwapChain()						{ return m_SwapChain; }
	std::shared_ptr<CRootSignature> GetRootSignature()				{ return m_RootSignature; }
	std::shared_ptr<CGraphicsDescriptorHeap> GetImGuiDescHeap()		{ return m_ImGuiDescHeap; }
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
	void Create2DDevice();

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
	std::shared_ptr<CGraphicsDescriptorHeap>	m_ImGuiDescHeap;
	std::shared_ptr<CComputeDescriptorHeap>		m_ComputeDescHeap;
	

	std::vector<std::shared_ptr<CConstantBuffer>>	m_ConstantBuffers;
	std::array<std::shared_ptr<CRenderTargetGroup>, RENDER_TARGET_GROUP_COUNT> m_RenderTargetGroups;

public:
	// Text
	ComPtr<ID3D11On12Device>	m_d3d11On12Device;
	ComPtr<ID2D1Device2>		m_d2dDevice = NULL;
	ComPtr<ID2D1Factory3>		m_d2dFactory;
	ComPtr<ID2D1DeviceContext2> m_d2dDeviceContext = NULL;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	ComPtr<IDWriteFactory>		m_WriteFactory = NULL;

	ComPtr<ID2D1SolidColorBrush> m_d2dbrBackground = NULL;
	ComPtr<ID2D1SolidColorBrush> m_d2dbrBorder = NULL;
	ComPtr<IDWriteTextFormat>	 m_dwFont = NULL;
	ComPtr<IDWriteTextLayout>	 m_dwTextLayout = NULL;
	ComPtr<ID2D1SolidColorBrush> m_d2dbrText = NULL;
	ComPtr<ID3D11Resource> m_d3d11WrappedBackBuffers[SWAP_CHAIN_BUFFER_COUNT];
	ComPtr<ID2D1Bitmap1> m_d2dRenderTargets[SWAP_CHAIN_BUFFER_COUNT];

};

