#pragma once

#include "Shader.h"

class CGraphicShader :
    public CShader
{
public:
	CGraphicShader();
	~CGraphicShader();

public:
	virtual int Init(const std::wstring& path, const std::string& name, ShaderInfo info = ShaderInfo());
	void Update();

private:
	int CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version);
	int CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version);

private:
	D3D12_PRIMITIVE_TOPOLOGY_TYPE	m_Topology;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  m_GraphicsPipelineDesc;
	ComPtr<ID3DBlob>				m_VSBlob;
	ComPtr<ID3DBlob>				m_PSBlob;
};

