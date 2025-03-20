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

    void Move(Vec3 moveDir, bool shouldRotate = true);
    void Attack();

    void CreateStateManager();

    Vec3 GetCurrentMoveDir() { return m_currentMoveDir; }
private:
    Vec3 m_currentMoveDir;
};

