#include "pch.h"
#include "SkillWidget.h"
#include "LevelManager.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "Engine.h"

void CSkillWidget::SetTexture(const std::wstring& name)
{
	CTexture* texture = CAssetManager::GetInst()->FindAsset<CTexture>(name);
	CGraphicShader* shader = CAssetManager::GetInst()->FindAsset<CGraphicShader>(L"SkillUI");
	GetMeshRenderer()->GetMaterial()->SetTexture(0, texture);
	GetMeshRenderer()->GetMaterial()->SetGraphicsShader(shader);
}

void CSkillWidget::SetMesh(const std::wstring& name)
{
	CMesh* mesh = CAssetManager::GetInst()->FindAsset<CMesh>(name);
	GetMeshRenderer()->SetMesh(mesh);
}

bool CSkillWidget::Init(CPlayer* player)
{
	CImageWidget::Init(player);
	CMaterial* material = new CMaterial;
	GetMeshRenderer()->SetMaterial(material);

	return true;
}

void CSkillWidget::Update()
{
	// temp
	// 나중에 스킬 생기면 수정
	CMaterial* material = GetMeshRenderer()->GetMaterial();
	if (material)
	{
		if (m_CoolTime >= 0.f)
		{
			m_CoolTime -= DELTA_TIME;
		}

		material->SetFloat(0, m_CoolTime / m_OriginCoolTime);
	}
}
