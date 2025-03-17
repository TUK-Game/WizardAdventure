#include "pch.h"
#include "SubLevel.h"
#include "GameObject.h"
#include "BaseCollider.h"
#include "BoxCollider.h"
#include "OrientedBoxCollider.h"
#include "Layer.h"
#include "Frustum.h"

std::vector<int> layerIndex({ 3, 10 });

CSubLevel::CSubLevel()
{
}

CSubLevel::~CSubLevel()
{
}

void CSubLevel::Init()
{
	
}

void CSubLevel::Begin()
{
	if (m_SubLevels.empty())
	{
		for (int idx : layerIndex)
		{
			GetLayer(idx)->Begin();
		}
	}
	else
	{
		for (const auto& level : m_SubLevels)
			level->Begin();
	}
}

void CSubLevel::Update()
{
	if (m_SubLevels.empty())
	{
		for (int idx : layerIndex)
		{
			GetLayer(idx)->Update();
		}
	}
	else
	{
		for (const auto& level : m_SubLevels)
			level->Update();
	}
}

void CSubLevel::FinalUpdate()
{
	if (m_SubLevels.empty())
	{
		for (int idx : layerIndex)
		{
			GetLayer(idx)->FinalUpdate();
		}
	}
	else
	{
		for (const auto& level : m_SubLevels)
			level->FinalUpdate();
	}
}

void CSubLevel::Deregister()
{
	if (m_SubLevels.empty())
	{
		for (int idx : layerIndex)
		{
			GetLayer(idx)->m_vecObjects.clear();
		}
	}
	else
	{
		for (const auto& level : m_SubLevels)
			level->Deregister();
	}
}

void CSubLevel::End()
{
	if (m_SubLevels.empty())
	{
		for (int idx : layerIndex)
		{
			GetLayer(idx)->m_vecObjects.clear();
		}
	}
	else
	{
		for (const auto& level : m_SubLevels)
			level->End();
	}
}

bool CSubLevel::AddGameObject(CGameObject* object, int layerIndex, bool bChildMove)
{
	if (m_SubLevels.empty())
	{
		switch (object->GetCollider()->GetColliderType())
		{
		case ECollider_Type::Sphere:
		{
			break;
		}
		case ECollider_Type::Box:
		{
			const auto& collider = (CBoxCollider*)(object->GetCollider());
			if (m_BoundingBox.Intersects(collider->GetBoundingBox()))
			{
				GetLayer(layerIndex)->AddGameObject(object, bChildMove);
				return true;
			}
			break;
		}
		case ECollider_Type::OrientedBox:
		{
			break;
		}
		}
		return false;
	}
	else
	{
		for (const auto& level : m_SubLevels)
		{
			if (level->AddGameObject(object, layerIndex, bChildMove))
				return true;
		}
	}
	return false;
}

void CSubLevel::RemoveGameObject(CGameObject* object)
{
	if (m_SubLevels.empty())
	{
		for (int idx : layerIndex)
		{
			GetLayer(idx)->RemoveGameObject(object);
		}
	}
	else
	{
		for (const auto& level : m_SubLevels)
			level->RemoveGameObject(object);
	}
}

void CSubLevel::RegisterGameObject(CGameObject* object, int layer)
{
	if(m_SubLevels.empty())
		GetLayer(layer)->RegisterGameObject(object);
	else
	{
		for (const auto& level : m_SubLevels)
			level->RegisterGameObject(object, layer);
	}
}

void CSubLevel::PickGameObject(CFrustum& frustum, std::vector<CGameObject*>& objects)
{
	if (!frustum.IsInFrustum(m_BoundingBox))
		return;

	if (m_SubLevels.empty())
	{
		for (int idx : layerIndex)
		{
			for (const auto& object : GetLayer(idx)->GetParentObjects())
			{
				objects.push_back(object);
			}
		}
	}
	else
	{
		for (const auto& level : m_SubLevels)
			level->PickGameObject(frustum, objects);
	}
}

void CSubLevel::SplitSubScene(int splitLevels)
{
	if (splitLevels > 0)
	{
		for(int i = 0; i < 4; ++i)
			m_SubLevels.push_back(std::make_shared<CSubLevel>());

		float xHalf = m_BoundingBox.Extents.x * 0.5f;
		float yHalf = m_BoundingBox.Extents.y;
		float zHalf = m_BoundingBox.Extents.z * 0.5f;

		m_SubLevels[0]->SetBoundingBox(Vec3(m_BoundingBox.Center.x - xHalf, m_BoundingBox.Center.y, m_BoundingBox.Center.z - zHalf), Vec3(xHalf, yHalf, zHalf));
		m_SubLevels[1]->SetBoundingBox(Vec3(m_BoundingBox.Center.x - xHalf, m_BoundingBox.Center.y, m_BoundingBox.Center.z + zHalf), Vec3(xHalf, yHalf, zHalf));
		m_SubLevels[2]->SetBoundingBox(Vec3(m_BoundingBox.Center.x + xHalf, m_BoundingBox.Center.y, m_BoundingBox.Center.z - zHalf), Vec3(xHalf, yHalf, zHalf));
		m_SubLevels[3]->SetBoundingBox(Vec3(m_BoundingBox.Center.x + xHalf, m_BoundingBox.Center.y, m_BoundingBox.Center.z + zHalf), Vec3(xHalf, yHalf, zHalf));

		m_SubLevels[0]->SplitSubScene(splitLevels - 1);
		m_SubLevels[1]->SplitSubScene(splitLevels - 1);
		m_SubLevels[2]->SplitSubScene(splitLevels - 1);
		m_SubLevels[3]->SplitSubScene(splitLevels - 1);
	}
	else
	{

	}
}
