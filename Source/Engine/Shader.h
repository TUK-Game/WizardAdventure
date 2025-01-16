#pragma once

#include "Asset.h"

enum class RASTERIZER_TYPE
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK,
	WIREFRAME,
};

enum class DEPTH_STENCIL_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
};

struct ShaderInfo
{
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
};

class CShader :
    public CAsset
{
public:
    CShader(EAsset_Type type);
    CShader(const CShader& other) = delete;
    ~CShader();

protected:
    ComPtr<ID3DBlob>                    m_ErrBlob;
    ComPtr<ID3D12PipelineState>         m_PipelineState;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC  m_PipelineDesc;

public:
    virtual CShader* Clone() override   { return nullptr; }
	virtual int Init(const std::wstring& path, ShaderInfo info = ShaderInfo()) { return 0; };
};

