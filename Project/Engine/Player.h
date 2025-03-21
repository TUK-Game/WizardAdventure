#pragma once
#include "GameObject.h"

class CPlayer : public CGameObject
{
public:
    CPlayer();
    virtual ~CPlayer();

    virtual void Begin();
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render();


    void Move(Vec3 moveDir);
    //void Jump();
    void Attack();

    // 상태 관련
    //PlayerState* GetCurrentState() { return m_CurrentState; }

public:
    virtual void CollisionBegin(CBaseCollider* src, CBaseCollider* dest) override;

private:
    Vec3 m_currentMoveDir;
    float m_Speed = 1000.f;
};

