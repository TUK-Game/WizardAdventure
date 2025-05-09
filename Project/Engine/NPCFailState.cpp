#include "pch.h"
#include "NPCFailState.h"
#include "GameObject.h"
#include "Animator.h"
#include "StateManager.h"

void CNPCFailState::Enter(CGameObject* entity)
{
	std::vector<CGameObject*> objs = entity->GetChild();
	for (const auto o : objs) {
		if (o->GetAnimator())
		{
			CAnimator* ani = o->GetAnimator();
			ani->Play(L"FAILED");
			m_Duration = ani->GetDuration();
		}
	}
	m_ElapsedTime = 0.f;
}

void CNPCFailState::Update(CGameObject* entity, float deltaTime)
{
	m_ElapsedTime += deltaTime;
	if (m_ElapsedTime > m_Duration)
	{
		entity->GetStateManager()->HandleEvent(entity, "Continue");
	}
}

void CNPCFailState::Exit(CGameObject* entity)
{
}
