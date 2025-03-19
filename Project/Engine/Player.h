#pragma once
#include "GameObject.h"
#include "StateManager.h"

class CPlayer : public CGameObject
{
public:
    CPlayer();
    virtual ~CPlayer();

    virtual void Begin();
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render();

    CStateManager* GetStateMachine() { return m_StateManager; }

    //void ChangeState(PlayerState* newState);

    // �⺻ ����
    //void Move(float deltaTime);
    //void Jump();
    void Attack();

    // ���� ����
    //PlayerState* GetCurrentState() { return m_CurrentState; }

private:
    CStateManager* m_StateManager;
    float m_Speed;
    float m_JumpForce;
};

