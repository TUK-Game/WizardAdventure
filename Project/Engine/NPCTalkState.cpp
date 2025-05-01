#include "pch.h"
#include "NPCTalkState.h"
#include "GameObject.h"
#include "Animator.h"
#include "StateManager.h"

void CNPCTalkState::Enter(CGameObject* entity)
{
	std::vector<CGameObject*> objs = entity->GetChild();
	for (const auto o : objs) {
		if (o->GetAnimator())
		{
			CAnimator* ani = o->GetAnimator();
			ani->Play(L"TALK");
			m_Duration = ani->GetDuration();
		}
	}
	m_ElapsedTime = 0.f;
}

void CNPCTalkState::Update(CGameObject* entity, float deltaTime)
{
	m_ElapsedTime += deltaTime;
	if (m_ElapsedTime > m_Duration)
	{
		entity->GetStateManager()->HandleEvent(entity, "Buy");
	}
}

void CNPCTalkState::Exit(CGameObject* entity)
{
}
