#include "pch.h"
#include "RenderTargetGroup.h"
#include "Engine.h"
#include "Device.h"


void CRenderTargetGroup::Create(RENDER_TARGET_GROUP_TYPE groupType, std::vector<RenderTarget>& rtVec, CTexture* dsTexture)
{
	m_GroupType = groupType;
	m_RenderTargetVec = rtVec;
	m_RenderTargetCount = static_cast<UINT32>(rtVec.size());
	m_DepthStencilTexture = dsTexture;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = m_RenderTargetCount;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.NodeMask = 0;

	DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_RtvHeap));

	m_RtvHeapSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_RtvHeapBegin = m_RtvHeap->GetCPUDescriptorHandleForHeapStart();
	m_DsvHeapBegin = m_DepthStencilTexture->GetDSV()->GetCPUDescriptorHandleForHeapStart();

	for (UINT32 i = 0; i < m_RenderTargetCount; i++)
	{
		UINT32 destSize = 1;
		D3D12_CPU_DESCRIPTOR_HANDLE destHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RtvHeapBegin, i * m_RtvHeapSize);

		UINT32 srcSize = 1;
		ComPtr<ID3D12DescriptorHeap> srcRtvHeapBegin = m_RenderTargetVec[i].target->GetRTV();
		D3D12_CPU_DESCRIPTOR_HANDLE srcHandle = srcRtvHeapBegin->GetCPUDescriptorHandleForHeapStart();

		DEVICE->CopyDescriptors(1, &destHandle, &destSize, 1, &srcHandle, &srcSize, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	for (int i = 0; i < m_RenderTargetCount; ++i)
	{
		m_TargetToResource[i] = CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargetVec[i].target->GetTex2D().Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COMMON);

		m_ResourceToTarget[i] = CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargetVec[i].target->GetTex2D().Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}
}

void CRenderTargetGroup::OMSetRenderTargets(UINT32 count, UINT32 offset)
{
	D3D12_VIEWPORT vp = D3D12_VIEWPORT{ 0.f, 0.f, m_RenderTargetVec[0].target->GetWidth() , m_RenderTargetVec[0].target->GetHeight(), 0.f, 1.f };
	D3D12_RECT rect = D3D12_RECT{ 0, 0, static_cast<LONG>(m_RenderTargetVec[0].target->GetWidth()),  static_cast<LONG>(m_RenderTargetVec[0].target->GetHeight()) };

	GRAPHICS_CMD_LIST->RSSetViewports(1, &vp);
	GRAPHICS_CMD_LIST->RSSetScissorRects(1, &rect);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RtvHeapBegin, offset * m_RtvHeapSize);
	GRAPHICS_CMD_LIST->OMSetRenderTargets(count, &rtvHandle, FALSE/*1°³*/, &m_DsvHeapBegin);
	
}

void CRenderTargetGroup::OMSetRenderTargets()
{
	D3D12_VIEWPORT vp = D3D12_VIEWPORT{ 0.f, 0.f, m_RenderTargetVec[0].target->GetWidth() , m_RenderTargetVec[0].target->GetHeight(), 0.f, 1.f };
	D3D12_RECT rect = D3D12_RECT{ 0, 0, static_cast<LONG>(m_RenderTargetVec[0].target->GetWidth()),  static_cast<LONG>(m_RenderTargetVec[0].target->GetHeight()) };

	GRAPHICS_CMD_LIST->RSSetViewports(1, &vp);
	GRAPHICS_CMD_LIST->RSSetScissorRects(1, &rect);

	GRAPHICS_CMD_LIST->OMSetRenderTargets(m_RenderTargetCount, &m_RtvHeapBegin, TRUE/*´ÙÁß*/, &m_DsvHeapBegin);
}

void CRenderTargetGroup::ClearRenderTargetView(UINT32 index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RtvHeapBegin, index * m_RtvHeapSize);
	GRAPHICS_CMD_LIST->ClearRenderTargetView(rtvHandle, m_RenderTargetVec[index].clearColor, 0, nullptr);

	GRAPHICS_CMD_LIST->ClearDepthStencilView(m_DsvHeapBegin, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
}

void CRenderTargetGroup::ClearRenderTargetView()
{
	WaitResourceToTarget();

	for (UINT32 i = 0; i < m_RenderTargetCount; i++)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RtvHeapBegin, i * m_RtvHeapSize);
		GRAPHICS_CMD_LIST->ClearRenderTargetView(rtvHandle, m_RenderTargetVec[i].clearColor, 0, nullptr);
	}

	GRAPHICS_CMD_LIST->ClearDepthStencilView(m_DsvHeapBegin, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
}

void CRenderTargetGroup::WaitTargetToResource()
{
	GRAPHICS_CMD_LIST->ResourceBarrier(m_RenderTargetCount, m_TargetToResource);
}

void CRenderTargetGroup::WaitResourceToTarget()
{
	GRAPHICS_CMD_LIST->ResourceBarrier(m_RenderTargetCount, m_ResourceToTarget);
}