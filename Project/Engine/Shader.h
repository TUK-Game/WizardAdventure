#pragma once

#include "Asset.h"

enum class SHADER_TYPE : UINT8
{
	DEFERRED,
	FORWARD,
	COMPUTE,
};

enum class RASTERIZER_TYPE : UINT8
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK,
	WIREFRAME,
};

enum class DEPTH_STENCIL_TYPE : UINT8
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
};

struct ShaderInfo
{
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
};

class CShader :
    public CAsset
{
public:
    CShader(EAsset_Type type);
    CShader(const CShader& other) = delete;
    ~CShader();

public:
	SHADER_TYPE GetShaderType() { return m_Info.shaderType; }
protected:
	ShaderInfo						m_Info;
    ComPtr<ID3DBlob>                m_ErrBlob;
	ComPtr<ID3D12PipelineState>     m_PipelineState;

protected:
	int CreateShader(const std::wstring& path, const std::string& name, const std::string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);

public:
    virtual CShader* Clone() override   { return nullptr; }
	virtual int Init(const std::wstring& path, const std::string& name, ShaderInfo info = ShaderInfo()) { return 0; };
};

