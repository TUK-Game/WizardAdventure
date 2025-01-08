#pragma once

#include "Asset.h"

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
};

