#pragma once

#include "Asset.h"
#include "Texture.h"
#include "GraphicShader.h"
#include "ComputeShader.h"

enum
{
};

class CMaterial : public CAsset
{
public:
	CMaterial();
	~CMaterial();

	CSharedPtr<CGraphicShader> GetGraphicsShader() { return m_GraphicsShader; }
	CSharedPtr<CComputeShader> GetComputeShader() { return m_ComputeShader; }

	void SetGraphicsShader(CSharedPtr<CGraphicShader> shader) { m_GraphicsShader = shader; }
	void SetComputeShader(CSharedPtr<CComputeShader> shader) { m_ComputeShader = shader; }
	void SetInt(unsigned char index, int value) { m_Params.SetInt(index, value); }
	void SetFloat(unsigned char index, float value) { m_Params.SetFloat(index, value); }
	void SetTexture(unsigned char index, CSharedPtr<CTexture> texture) { m_arrTexture[index] = texture; }

	void GraphicsBinding();
	void ComputeBinding();
	void Dispatch(UINT32 x, UINT32 y, UINT32 z);

public:
	virtual CMaterial* Clone() override { return new CMaterial(*this); }

private:
	CSharedPtr<CGraphicShader>	m_GraphicsShader;
	CSharedPtr<CComputeShader>	m_ComputeShader;
	MaterialParams				m_Params;
	std::array<CSharedPtr<CTexture>, MATERIAL_TEXTURE_COUNT> m_arrTexture;
};

