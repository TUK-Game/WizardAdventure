#include "pch.h"
#include "MapPlayerWidget.h"
#include "LevelManager.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "Level.h"
#include "Engine.h"

CMapPlayerWidget::CMapPlayerWidget()
{
}

CMapPlayerWidget::~CMapPlayerWidget()
{
}

bool CMapPlayerWidget::Init()
{
	m_OwnerPlayer = CLevelManager::GetInst()->GetPlayer();
	m_MapCenter = CLevelManager::GetInst()->GetCurrentLevel()->GetMapCenter();
	m_MapSize = CLevelManager::GetInst()->GetCurrentLevel()->GetMapSize() * 2;
	AddComponent(new CMeshRenderer);
	AddComponent(new CTransform);
	SetTexture(L"Kita");
	GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Sphere"));
	GetTransform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SetEnable(false);

	float MapAspectRatio = m_MapSize.x / m_MapSize.y;
	float TextureAspectRatio = (float)(CEngine::GetInst()->GetWindowInfo().Width) / (float)(CEngine::GetInst()->GetWindowInfo().Height);
	m_AspectRatio = MapAspectRatio / TextureAspectRatio;

	return true;
}

void CMapPlayerWidget::Update()
{
	m_PlayerPosition = m_OwnerPlayer->GetTransform()->GetRelativePosition();
	ConvertPositionToMap();
}

void CMapPlayerWidget::ConvertPositionToMap()
{
	Vec3 NormalizedPosition;

	float RelativeX = m_PlayerPosition.x - m_MapCenter.x;
	float Relativez = m_PlayerPosition.z - m_MapCenter.z;

	NormalizedPosition.x = ((RelativeX / (m_MapSize.x / 2)));
	NormalizedPosition.y = ((Relativez / (m_MapSize.z / 2)));

	if (m_AspectRatio > 1.0f)
	{
		NormalizedPosition.x /= m_AspectRatio;
		NormalizedPosition.y *= m_AspectRatio;
	}
	else
	{
		NormalizedPosition.x *= m_AspectRatio;
		NormalizedPosition.y /= m_AspectRatio;
	}

	std::cout << "X : " << NormalizedPosition.x << " , Y : " << NormalizedPosition.y << '\n';

	GetTransform()->SetRelativePosition(NormalizedPosition);
}
