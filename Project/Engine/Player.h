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


    //void ChangeState(PlayerState* newState);

    // �⺻ ����
    //void Move(float deltaTime);
    //void Jump();
    void Attack();

    // ���� ����
    //PlayerState* GetCurrentState() { return m_CurrentState; }

private:
    float m_Speed;
    float m_JumpForce;
};

