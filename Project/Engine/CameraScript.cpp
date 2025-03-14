#include "pch.h"
#include "CameraScript.h"
#include "InputManager.h"
#include "Transform.h"
#include "Engine.h"
#include "LevelManager.h"

CCameraScript::CCameraScript()
	: m_Speed(1000.f)
{
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::Update()
{
	// 카메라 컴포넌트가 없다면 종료
	if (!GetCamera())
		return;

	Move();
}

void CCameraScript::Move()
{
	Vec3 pos = GetTransform()->GetRelativePosition();

	Vec3 front = GetTransform()->GetRelativeDir(EDir::Front);
	Vec3 right = GetTransform()->GetRelativeDir(EDir::Right);
	Vec3 up = GetTransform()->GetRelativeDir(EDir::Up);

	if (KEY_PUSH(EKey::W))
		pos += front * DELTA_TIME * m_Speed;
	if (KEY_PUSH(EKey::S))
		pos -= front * DELTA_TIME * m_Speed;
	if (KEY_PUSH(EKey::A))
		pos -= right * DELTA_TIME * m_Speed;
	if (KEY_PUSH(EKey::D))
		pos += right * DELTA_TIME * m_Speed;
	if (KEY_PUSH(EKey::Q))
		pos -= up * DELTA_TIME * m_Speed;
	if (KEY_PUSH(EKey::E))
		pos += up * DELTA_TIME * m_Speed;

	GetTransform()->SetRelativePosition(pos);


	if (KEY_PUSH(EKey::RButton))
	{
		Vec2 dragDir = CInputManager::GetInst()->GetDragDir();
		Vec3 rot = GetTransform()->GetRelativeRotation();
		rot.x += dragDir.y * DELTA_TIME * 360.f;
		rot.y += dragDir.x * DELTA_TIME * 360.f;
		GetTransform()->SetRelativeRotation(rot);
	}

	if (KEY_DOWN(EKey::LButton))
	{
		const Vec2& pos = CInputManager::GetInst()->GetMousePosition();
		CGameObject* obj = CLevelManager::GetInst()->Pick(pos.x, pos.y);
		if (obj)
		{
			obj->SetActive(false);
		}
	}

	if (KEY_PUSH(EKey::Esc))
	{
		PostQuitMessage(0);
	}
}
	