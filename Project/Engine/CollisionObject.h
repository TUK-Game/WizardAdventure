#pragma once
#include "GameObject.h"
class CCollisionObject :
	public CGameObject
{
public:
    CCollisionObject();
    virtual ~CCollisionObject();

    void Init(CGameObject* owner);
    void InitToChild(CGameObject* owner, Vec3 position, Vec3 scale = Vec3(1.f, 1.f, 1.f));
    virtual void FinalUpdate();

private:
    CGameObject* m_Owner;
};

