#pragma once
#include "GameObject.h"

struct GateInfo
{
	Vec3 GatePos;
	Vec3 GateSize;
	Vec3 GateDir;
	float GateYRot = 0.f;
};

class CTriggerBox : public CGameObject
{
public:
	CTriggerBox();
	virtual ~CTriggerBox();

public:
	void Init();

	void SetTriggerBox(const Vec3& pos, const Vec3& size);
	void SetArea(const Vec3& center, const Vec3& size) { m_AreaCenter = center; m_AreaSize = size; }
	void PushGateInfo(const Vec3& center, const Vec3& size, const Vec3& dir, float yRot) { m_GateInfo.emplace_back(GateInfo(center, size, dir, yRot)); }

	bool IsMonsterInArea(const Vec3& pos);

	void AddMonster(CMonsterRef monster) { m_AreaMonsters.push_back(monster); }

	virtual void Update(float deltaTime);
	virtual void CollisionBegin(CBoxCollider* src, CBoxCollider* dest) override;
	virtual void CollisionEnd(CBoxCollider* src, CBoxCollider* dest) override;

private:
	std::vector<CMonsterRef> m_AreaMonsters;
	Vec3 m_AreaCenter;
	Vec3 m_AreaSize;
	std::vector<GateInfo> m_GateInfo;
};

