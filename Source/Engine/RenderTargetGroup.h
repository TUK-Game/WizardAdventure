#pragma once
#include "Texture.h"

enum class RENDER_TARGET_GROUP_TYPE : UINT8
{
	SWAP_CHAIN, // BACK_BUFFER, FRONT_BUFFER
	SHADOW,		// SHADOW
	G_BUFFER, // POSITION, NORMAL, COLOR
	LIGHTING, // DIFFUSE, SPECULAR
	MAP,
	END,
};

enum
{
	RENDER_TARGET_SHADOW_GROUP_MEMBER_COUNT = 1, 
	RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT = 3,
	RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT = 2,
	RENDER_TARGET_MAP_GROUP_MEMBER_COUNT = 1,
	RENDER_TARGET_GROUP_COUNT = static_cast<UINT8>(RENDER_TARGET_GROUP_TYPE::END)
};

struct RenderTarget
{
	CTexture* target;
	float clearColor[4];
};

class CRenderTargetGroup
{
public:
	void Create(RENDER_TARGET_GROUP_TYPE groupType, std::vector<RenderTarget>& rtVec, CTexture* dsTexture);

	void OMSetRenderTargets(UINT32 count, UINT32 offset);
	void OMSetRenderTargets();

	void ClearRenderTargetView(UINT32 index);
	void ClearRenderTargetView();

	void WaitTargetToResource();
	void WaitResourceToTarget();

	CTexture* GetRTTexture(UINT32 index) { return m_RenderTargetVec[index].target; }
	CTexture* GetDSTexture() { return m_DepthStencilTexture; }

private:
	RENDER_TARGET_GROUP_TYPE		m_GroupType;
	std::vector<RenderTarget>		m_RenderTargetVec;
	UINT32							m_RenderTargetCount;
	CTexture*						m_DepthStencilTexture;
	ComPtr<ID3D12DescriptorHeap>	m_RtvHeap;

private:
	UINT32							m_RtvHeapSize;
	D3D12_CPU_DESCRIPTOR_HANDLE		m_RtvHeapBegin;
	D3D12_CPU_DESCRIPTOR_HANDLE		m_DsvHeapBegin;

private:
	D3D12_RESOURCE_BARRIER			m_TargetToResource[8];
	D3D12_RESOURCE_BARRIER			m_ResourceToTarget[8];
};

