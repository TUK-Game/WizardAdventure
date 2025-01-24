#include "pch.h"
#include "TestDragon.h"
#include "InputManager.h"
#include "Animator.h"

void CTestDragon::Update()
{
	if (KEY_PUSH(EKey::Num1))
	{
		INT32 count = GetAnimator()->GetAnimCount();
		INT32 currentIndex = GetAnimator()->GetCurrentClipIndex();

		INT32 index = (currentIndex + 1) % count;
		GetAnimator()->Play(index);
	}

	if (KEY_PUSH(EKey::Num2))
	{
		INT32 count = GetAnimator()->GetAnimCount();
		INT32 currentIndex = GetAnimator()->GetCurrentClipIndex();

		INT32 index = (currentIndex - 1 + count) % count;
		GetAnimator()->Play(index);
	}
}
