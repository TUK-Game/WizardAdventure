#include "pch.h"
#include "TestPlayer.h"
#include "InputManager.h"
#include "Animator.h"
#include "Mesh.h"
#include "Transform.h"
#include "Timer.h"
#include "Engine.h"

void CTestPlayer::Update()
{
	Vec3 Front = GetTransform()->GetRelativeDir(EDir::Front);
	Vec3 Pos = GetTransform()->GetRelativePosition();
	if (KEY_PUSH(EKey::Up))
	{
		GetTransform()->SetRelativeRotation(0.f, 0.f, 0.f);

		if (KEY_PUSH(EKey::Left))
			GetTransform()->SetRelativeRotation(0.f, -45.f, 0.f);
		else if (KEY_PUSH(EKey::Right))
			GetTransform()->SetRelativeRotation(0.f, 45.f, 0.f);

		Pos -= Front * m_Speed * DELTA_TIME;
	}
	else if (KEY_PUSH(EKey::Down))
	{
		GetTransform()->SetRelativeRotation(0.f, 180.f, 0.f);
		if (KEY_PUSH(EKey::Left))
			GetTransform()->SetRelativeRotation(0.f, 180.f + 45.f, 0.f);
		else if (KEY_PUSH(EKey::Right))
			GetTransform()->SetRelativeRotation(0.f, 180.f - 45.f, 0.f);

		Pos -= Front * m_Speed * DELTA_TIME;
	}
	else if (KEY_PUSH(EKey::Right))
	{
		GetTransform()->SetRelativeRotation(0.f, 90.f, 0.f);
		Pos -= Front * m_Speed * DELTA_TIME;
	}
	else if (KEY_PUSH(EKey::Left))
	{
		GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
		Pos -= Front * m_Speed * DELTA_TIME;
	}
	else if (KEY_PUSH(EKey::Ctrl))
	{
		if (m_Prev == 4)
			return;
		m_Prev = 4;
		GetAnimator()->Play(4);
	}

	GetTransform()->SetRelativePosition(Pos);

	if (KEY_PUSH(EKey::Up) || KEY_PUSH(EKey::Left) || KEY_PUSH(EKey::Right) || KEY_PUSH(EKey::Down))
	{
		if (m_Prev == 5)
			return;
		m_Prev = 5;
		GetAnimator()->Play(5);
	}

	if (KEY_UP(EKey::Up) || KEY_UP(EKey::Left) || KEY_UP(EKey::Right) || KEY_UP(EKey::Down))
	{
		m_Prev = 3;
		GetAnimator()->Play(3);
	}

}
