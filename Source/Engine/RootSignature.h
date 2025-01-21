#pragma once

class CRootSignature
{
public:
	CRootSignature();
	~CRootSignature();

public:
	ComPtr<ID3D12RootSignature>	GetGraphicsRootSignature() { return m_GraphicsRootSignature; }
	ComPtr<ID3D12RootSignature>	GetComputeRootSignature()	{ return m_ComputeRootSignature; }

public:
	int Init();

private:
	int CreateGraphicsRootSignature();
	int CreateComputeRootSignature();

private:
	D3D12_STATIC_SAMPLER_DESC	m_SamplerDesc;
	ComPtr<ID3D12RootSignature>	m_GraphicsRootSignature;
	ComPtr<ID3D12RootSignature>	m_ComputeRootSignature;
};

