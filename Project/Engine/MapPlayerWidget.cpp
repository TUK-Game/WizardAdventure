#include "pch.h"
#include "MapPlayerWidget.h"
#include "LevelManager.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "Level.h"
#include "Engine.h"
#include "Player.h"

CMapPlayerWidget::CMapPlayerWidget()
{
}

CMapPlayerWidget::~CMapPlayerWidget()
{
}

bool CMapPlayerWidget::InitPlayer(CPlayer* player, int idx)
{
	m_OwnerPlayer = player;
	m_MapMax = CLevelManager::GetInst()->GetCurrentLevel()->GetMapMaxToCamera();
	m_MapMin = CLevelManager::GetInst()->GetCurrentLevel()->GetMapMinToCamera();

	switch (player->GetAttribute())
	{
	case EPlayerAttribute::Fire:
	{
		SetTexture(L"FireMage_Face");
	}
	break;
	case EPlayerAttribute::Ice:
	{
		SetTexture(L"IceMage_Face");
	}
	break;
	case EPlayerAttribute::Electric:
	{
		SetTexture(L"LightningMage_Face");
	}
	break;
	}
	GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));
	GetTransform()->SetRelativeScale(0.05f, 0.05f, 0.05f);

	return true;
}

void CMapPlayerWidget::Update()
{
	if (m_OwnerPlayer)
	{
		m_PlayerPosition = m_OwnerPlayer->GetTransform()->GetRelativePosition();
		ConvertPositionToMap();
	}
}

void CMapPlayerWidget::ConvertPositionToMap()
{
	Vec2 playerXZ = Vec2(m_PlayerPosition.x, m_PlayerPosition.z);
	Vec2 mapSize = m_MapMax - m_MapMin;
	Vec2 normalizedPos{ 1.0f - (playerXZ.x - m_MapMin.x) / mapSize.x, (playerXZ.y - m_MapMin.y) / mapSize.y };
	Vec2 ndcPos{ normalizedPos.x * 2.0f - 1.0f, 1.0f - normalizedPos.y * 2.0f };

	GetTransform()->SetRelativePosition(ndcPos.x, ndcPos.y, 0.f);
}
