#pragma once

#include "Asset.h"

class CTexture :
	public CAsset
{
public:
	CTexture();
	CTexture(const CTexture& other) = delete;
	~CTexture();

public:
	

	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCpuHandle() { return m_SrvHandle; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetUAVCpuHandle() { return m_UavHandle; }

	ComPtr<ID3D12Resource> GetTex2D() { return m_Tex2D; }
	ComPtr<ID3D12DescriptorHeap> GetSRV() { return m_SrvHeap; }
	ComPtr<ID3D12DescriptorHeap> GetRTV() { return m_RtvHeap; }
	ComPtr<ID3D12DescriptorHeap> GetDSV() { return m_DsvHeap; }
	ComPtr<ID3D12DescriptorHeap> GetUAV() { return m_UavHeap; }

	float GetWidth() { return static_cast<float>(m_Desc.Width); }
	float GetHeight() { return static_cast<float>(m_Desc.Height); }

public:
	int Init(const std::wstring& path, UINT textureType = RESOURCE_TEXTURE2D);

	void Create(DXGI_FORMAT format, UINT32 width, UINT32 height,
		const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags,
		D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor = Vec4());

	void CreateFromResource(ComPtr<ID3D12Resource> tex2D);

public:
	virtual CTexture* Clone() override { return nullptr; }

private:
	UINT							m_TextureType = RESOURCE_TEXTURE2D;

	ScratchImage			 		m_Image;
	D3D12_RESOURCE_DESC				m_Desc;
	ComPtr<ID3D12Resource>			m_Tex2D;

	ComPtr<ID3D12DescriptorHeap>	m_SrvHeap;
	ComPtr<ID3D12DescriptorHeap>	m_DsvHeap;
	ComPtr<ID3D12DescriptorHeap>	m_RtvHeap;
	ComPtr<ID3D12DescriptorHeap>	m_UavHeap;

	D3D12_CPU_DESCRIPTOR_HANDLE		m_SrvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE		m_UavHandle;
};

