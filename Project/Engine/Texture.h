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
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return m_SrvHandle; }

public:
	int Init(const std::wstring& path);

public:
	int CreateTexture(const std::wstring& path);
	int CreateView();

public:
	virtual CTexture* Clone() override { return nullptr; }

private:
	ScratchImage			 		m_Image;
	ComPtr<ID3D12Resource>			m_Tex2D;

	ComPtr<ID3D12DescriptorHeap>	m_SrvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		m_SrvHandle;
};

