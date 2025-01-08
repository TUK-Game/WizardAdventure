#pragma once

#include "Shader.h"

class CGraphicShader :
    public CShader
{
public:
	CGraphicShader();
	~CGraphicShader();

public:
	int Init(const std::wstring& path);
	void Update();

private:
	int CreateShader(const std::wstring& path, const std::string& name, const std::string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	int CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version);
	int CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version);

private:
	D3D12_PRIMITIVE_TOPOLOGY_TYPE	m_Topology;
	ComPtr<ID3DBlob>				m_VSBlob;
	ComPtr<ID3DBlob>				m_PSBlob;
};

