#include "pch.h"
#include "SwapChain.h"

CSwapChain::CSwapChain()
	: m_BackBufferIndex(0)
{
}

CSwapChain::~CSwapChain()
{
}

int CSwapChain::Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	if (FAILED(CreateSwapChain(info, dxgi, cmdQueue)))
		return E_FAIL;
	return S_OK;
}

void CSwapChain::Present()
{
	// Present the frame.
	m_SwapChain->Present(0, 0);
}

void CSwapChain::SwapIndex()
{
	m_BackBufferIndex = (m_BackBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

int CSwapChain::CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	// 이전에 만든 정보 날린다
	m_SwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = static_cast<UINT>(info.Width); // 버퍼의 해상도 너비
	sd.BufferDesc.Height = static_cast<UINT>(info.Height); // 버퍼의 해상도 높이
	sd.BufferDesc.RefreshRate.Numerator = 60; // 화면 갱신 비율
	sd.BufferDesc.RefreshRate.Denominator = 1; // 화면 갱신 비율
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 버퍼의 디스플레이 형식
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // 멀티 샘플링 OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 후면 버퍼에 렌더링할 것 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // 전면 + 후면 버퍼
	sd.OutputWindow = info.hWnd;
	sd.Windowed = info.bWindowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 전면 후면 버퍼 교체 시 이전 프레임 정보 버림
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (FAILED(dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &m_SwapChain)))
		return E_FAIL;

	return S_OK;
}

