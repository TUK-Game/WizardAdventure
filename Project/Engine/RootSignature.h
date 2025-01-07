#pragma once

class CRootSignature
{
public:
	CRootSignature();
	~CRootSignature();

public:
	ComPtr<ID3D12RootSignature>	GetSignature()	{ return m_Signature; }

public:
	int Init();

private:
	void CreateSamplerDesc();
	int CreateRootSignature();

private:
	ComPtr<ID3D12RootSignature>	m_Signature;
	D3D12_STATIC_SAMPLER_DESC	m_SamplerDesc;
};

