#pragma once
#include "GameObject.h"

class CTriggerBox : public CGameObject
{
public:
	CTriggerBox();
	virtual ~CTriggerBox();

public:
	virtual void SetTriggerBox(const Vec3& pos, const Vec3& size);

	virtual void Update(float deltaTime);
	virtual void CollisionBegin(CBoxCollider* src, CBoxCollider* dest) override;
	virtual void CollisionEnd(CBoxCollider* src, CBoxCollider* dest) override;

protected:
};
