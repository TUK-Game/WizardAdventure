#include "pch.h"
#include "SkillWidget.h"
#include "LevelManager.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "Engine.h"

void CSkillWidget::SetTexture(const std::wstring& name)
{
	CMaterial* material = new CMaterial;
	CTexture* texture = CAssetManager::GetInst()->FindAsset<CTexture>(name);

	CGraphicShader* shader = CAssetManager::GetInst()->FindAsset<CGraphicShader>(L"SkillUI");
	material->SetTexture(0, texture);
	material->SetGraphicsShader(shader);

	GetMeshRenderer()->SetMaterial(material);
}

bool CSkillWidget::Init()
{
	m_OwnerPlayer = CLevelManager::GetInst()->GetPlayer();
	AddComponent(new CMeshRenderer);
	AddComponent(new CTransform);
	GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));

	return true;
}

void CSkillWidget::Update()
{
	// temp
	// 나중에 스킬 생기면 수정
	if (m_CoolTime >= 0.f)
	{
		m_CoolTime -= DELTA_TIME;
	}

	GetMeshRenderer()->GetMaterial()->SetFloat(0, m_CoolTime / m_OriginCoolTime);
}
