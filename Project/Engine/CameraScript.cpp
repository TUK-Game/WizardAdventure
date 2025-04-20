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
		m_TargetTransform = GetOwner()->GetCamera()->GetTarget()->GetTransform();	
	}
	if (KEY_DOWN(EKey::M))
	{
		const auto window = CLevelManager::GetInst()->GetCurrentLevel()->FindWidgetWindow(EWIDGETWINDOW_TYPE::MAP_WINDOW);
		if (window->GetEnable())
		{
			window->SetEnable(false);
		}
		else
		{
			window->SetEnable(true);
		}
	/*	CWidget* widget = CLevelManager::GetInst()->GetCurrentLevel()->FindWidget(L"MiniMap");
		if(widget->GetEnable())
			widget->SetEnable(false);
		else
			widget->SetEnable(true);*/
	}

	if (GetOwner()->GetCamera()->GetCameraType() == ECamera_Type::Fixed)
		FixedMove();
	else
		FreeMove();
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
	Vec3 pos = m_TargetTransform->GetRelativePosition();
	GetTransform()->SetRelativePosition(pos + m_Offset);
}
