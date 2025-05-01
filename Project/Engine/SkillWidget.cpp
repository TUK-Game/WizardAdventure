#include "pch.h"
#include "SkillWidget.h"
#include "LevelManager.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "Engine.h"
#include "Player.h"

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
		float cooldown = m_OwnerPlayer->GetSkillManager()->GetSkillCooldown(m_slot);
		material->SetFloat(0, cooldown / m_OriginCoolTime);
	}
}
