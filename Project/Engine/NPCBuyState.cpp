#include "pch.h"
#include "NPCBuyState.h"
#include "GameObject.h"
#include "Animator.h"
#include "StateManager.h"

void CNPCBuyState::Enter(CGameObject* entity)
{
	std::vector<CGameObject*> objs = entity->GetChild();
	for (const auto o : objs) {
		if (o->GetAnimator())
		{
			CAnimator* ani = o->GetAnimator();
			ani->Play(L"BUYMOTION");
			m_Duration = ani->GetDuration();
		}
	}
	m_ElapsedTime = 0.f;
}

void CNPCBuyState::Update(CGameObject* entity, float deltaTime)
{
	m_ElapsedTime += deltaTime;
	if (m_ElapsedTime > m_Duration)
	{
		entity->GetStateManager()->HandleEvent(entity, "Continue");
	}
}

void CNPCBuyState::Exit(CGameObject* entity)
{
}
