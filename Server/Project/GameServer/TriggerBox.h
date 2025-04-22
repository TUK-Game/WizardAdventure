#pragma once
#include "GameObject.h"

class CTriggerBox : public CGameObject
{
public:
	CTriggerBox();
	virtual ~CTriggerBox();

public:
	void Init();

	void SetTriggerBox(const Vec3& pos, const Vec3& size);
	void SetArea(const Vec3& center, const Vec3& size) { m_AreaCenter = center; m_AreaSize = size; }

	bool IsMonsterInArea(const Vec3& pos);

	void AddMonster(CMonsterRef monster) { m_AreaMonsters.push_back(monster); }

	virtual void Update(float deltaTime);
	virtual void CollisionBegin(CBoxCollider* src, CBoxCollider* dest);

private:
	std::vector<CMonsterRef> m_AreaMonsters;
	Vec3 m_AreaCenter;
	Vec3 m_AreaSize;
};

