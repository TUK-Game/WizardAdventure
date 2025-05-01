#include "pch.h"
#include "NPCIdleState.h"
#include "GameObject.h"
#include "Animator.h"
#include "StateManager.h"

void CNPCIdleState::Enter(CGameObject* entity)
{
	std::vector<CGameObject*> objs = entity->GetChild();
	for (const auto o : objs) {
		if (o->GetAnimator())
		{
			CAnimator* ani = o->GetAnimator();
			ani->Play(L"IDLE");
			m_Duration = ani->GetDuration();
		}
	}
	m_ElapsedTime = 0.f;
}

void CNPCIdleState::Update(CGameObject* entity, float deltaTime)
{
	m_ElapsedTime += deltaTime;
	if (m_ElapsedTime > m_Duration)
	{
		entity->GetStateManager()->HandleEvent(entity, "Talk");
	}
}

void CNPCIdleState::Exit(CGameObject* entity)
{
}
