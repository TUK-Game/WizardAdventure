#include "pch.h"
#include "CameraScript.h"
#include "InputManager.h"
#include "Transform.h"
#include "Engine.h"
#include "LevelManager.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "GameObject.h"
#include "Level.h"
#include "MeshRenderer.h"
#include "Logger.h"
#include "ImageWidget.h"

CCameraScript::CCameraScript()
	: m_Speed(1000.f)
{
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::Begin()
{
	if(GetOwner()->GetCamera()->GetTarget())
		m_TargetTransform = GetOwner()->GetCamera()->GetTarget()->GetTransform();
	m_Offset = Vec3(462.0f, 1130.2f, -659.8f);
}

void CCameraScript::Update()
{
	// 카메라 컴포넌트가 없다면 종료
	if (!GetCamera())
		return;


	if (KEY_PUSH(EKey::Esc))
	{
		PostQuitMessage(0);
	}

	if (KEY_PUSH(EKey::Num1))
	{
		GetOwner()->GetCamera()->SetCameraType(ECamera_Type::Free);
		GetOwner()->GetCamera()->SetFOV(90.f);
		GetOwner()->GetCamera()->SetFar(10000.f);
	}
	if (KEY_PUSH(EKey::Num2))
	{
		GetOwner()->GetCamera()->SetCameraType(ECamera_Type::Fixed);
		GetOwner()->GetCamera()->SetFOV(60.f);
		GetOwner()->GetCamera()->SetFar(5000.f);
		GetTransform()->SetRelativeRotation(49.f, -34.f, 0.f);
	}
	if (KEY_DOWN(EKey::M))
	{
		CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
		const auto window = level->FindWidgetWindow(EWIDGETWINDOW_TYPE::MAP_WINDOW);
		if (window->GetEnable())
		{
			window->SetEnable(false);
			level->SetWidgetWindowType(EWIDGETWINDOW_TYPE::END);
		}
		else
		{
			window->SetEnable(true);
			level->SetWidgetWindowType(EWIDGETWINDOW_TYPE::MAP_WINDOW);
		}
	/*	CWidget* widget = CLevelManager::GetInst()->GetCurrentLevel()->FindWidget(L"MiniMap");
		if(widget->GetEnable())
			widget->SetEnable(false);
		else
			widget->SetEnable(true);*/
	}
		
	if (GetOwner()->GetCamera()->GetTarget())
	{
		m_TargetTransform = GetOwner()->GetCamera()->GetTarget()->GetTransform();
	}

	ECamera_Type type = GetOwner()->GetCamera()->GetCameraType();

	if (ECamera_Type::Fixed == type)
	{
		FixedMove();
	}
	else if(ECamera_Type::Free == type)
	{
		FreeMove();
	}
	else if(ECamera_Type::Interaction_Start == type)
	{
		MoveToTarget();
	}
	else if (ECamera_Type::Interaction == type)
	{
	}
	else if (ECamera_Type::Interaction_End == type)
	{
		RollBackCamera();
	}

}

void CCameraScript::FreeMove()
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
#ifdef READY_IMGUI
			//CImGuiManager::GetInst()->SetSelectedObject(obj);
			//obj->SetActive(false);
#endif
			std::cout << "My name is " << ws2s(obj->GetName()) << '\n';
		}
	}
}

void CCameraScript::FixedMove()
{
	if (GetOwner()->GetCamera()->GetTarget())
	{
		Vec3 pos = m_TargetTransform->GetRelativePosition();
		GetTransform()->SetRelativePosition(pos + m_Offset);
	}
}

void CCameraScript::MoveToTarget()
{
	m_ElapsedTime += DELTA_TIME;

	float t = std::clamp(m_ElapsedTime / m_MoveDuration, 0.f, 1.f);

	Vec3 newPos = Vec3::Lerp(m_InteractionStartPos, m_InteractionZoomTarget, t);
	newPos.y = max(newPos.y, 200.f);
	GetOwner()->GetTransform()->SetRelativePosition(newPos);

	Vec3 newrot = Vec3::Lerp(m_InteractionStartDir, m_InteractionZoomDir, t);
	GetOwner()->GetTransform()->SetRelativeRotation(newrot);

	if (t >= 1.0f)
	{
		GetOwner()->GetCamera()->SetCameraType(ECamera_Type::Interaction);
		GetOwner()->GetCamera()->CheckLayer(LAYER_PLAYER);
		m_ElapsedTime = 0.f;
	}
}

void CCameraScript::RollBackCamera()
{
	m_ElapsedTime += DELTA_TIME;

	float t = std::clamp(m_ElapsedTime / m_MoveDuration, 0.f, 1.f);

	Vec3 newPos = Vec3::Lerp(m_InteractionZoomTarget, m_InteractionStartPos, t);
	GetOwner()->GetTransform()->SetRelativePosition(newPos);

	Vec3 newrot = Vec3::Lerp(m_InteractionZoomDir, m_InteractionStartDir, t);
	GetOwner()->GetTransform()->SetRelativeRotation(newrot);

	if (t >= 1.0f)
	{
		GetOwner()->GetTransform()->SetRelativePosition(m_InteractionStartPos);
		GetOwner()->GetTransform()->SetRelativeRotation(m_InteractionStartDir);
		GetOwner()->GetCamera()->SetCameraType(ECamera_Type::Fixed);
		m_ElapsedTime = 0.f;
	}
}
