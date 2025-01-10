#pragma once

#include "Asset.h"
#include "Texture.h"
#include "GraphicShader.h"

enum
{
};

class CMaterial : public CAsset
{
public:
	CMaterial();
	~CMaterial();

	CSharedPtr<CGraphicShader> GetShader() { return m_Shader; }

	void SetShader(CSharedPtr<CGraphicShader> shader) { m_Shader = shader; }
	void SetInt(unsigned char index, int value) { m_Params.SetInt(index, value); }
	void SetFloat(unsigned char index, float value) { m_Params.SetFloat(index, value); }
	void SetTexture(unsigned char index, CSharedPtr<CTexture> texture) { m_arrTexture[index] = texture; }

	void Binding();

public:
	virtual CMaterial* Clone() override { return new CMaterial(*this); }

private:
	CSharedPtr<CGraphicShader>	m_Shader;
	MaterialParams				m_Params;
	std::array<CSharedPtr<CTexture>, MATERIAL_TEXTURE_COUNT> m_arrTexture;
};

