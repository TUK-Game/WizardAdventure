#pragma once

#include "Shader.h"

class CGraphicShader :
    public CShader
{
public:
	CGraphicShader();
	~CGraphicShader();

public:
	int Init(const std::wstring& path, ShaderInfo info = ShaderInfo(), const std::string& vs = "VS_Main", const std::string& ps = "PS_Main", const std::string& gs = "");
	void Update();

private:
	int CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version);
	int CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version);
	int CreateGeometryShader(const std::wstring& path, const std::string& name, const std::string& version);

	D3D12_PRIMITIVE_TOPOLOGY_TYPE GetTopologyType(D3D_PRIMITIVE_TOPOLOGY topology);
private:
	D3D12_PRIMITIVE_TOPOLOGY_TYPE	m_Topology;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  m_GraphicsPipelineDesc;
	ComPtr<ID3DBlob>				m_VSBlob;
	ComPtr<ID3DBlob>				m_PSBlob;
	ComPtr<ID3DBlob>				m_GSBlob;
};

