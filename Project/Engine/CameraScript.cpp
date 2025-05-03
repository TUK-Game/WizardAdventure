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
#include "Light.h"

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
		GetOwner()->GetCamera()->SetFOV(75.f);
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
	if (GetOwner()->GetCamera()->GetTarget())
	{
		Vec3 pos = m_TargetTransform->GetRelativePosition();
		GetTransform()->SetRelativePosition(pos + m_Offset);

		//UpdateDirectionalLight();
	}
}

void CCameraScript::UpdateDirectionalLight()
{
	if (!m_TargetTransform)
		return;

	CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
	CGameObject* lightObj = level->FindObjectByName(L"DirectionalLight");

	if (!lightObj || !lightObj->GetLight())
		return;

	Vec3 cameraPos = GetTransform()->GetRelativePosition();
	Vec3 targetPos = m_TargetTransform->GetRelativePosition();

	// 타겟 기준으로 카메라 위치의 대칭점 계산
	Vec3 mirroredLightPos;
	mirroredLightPos.x = targetPos.x * 2.f - cameraPos.x;
	mirroredLightPos.z = targetPos.z * 2.f - cameraPos.z;
	mirroredLightPos.y = lightObj->GetTransform()->GetRelativePosition().y;

	lightObj->GetTransform()->SetRelativePosition(mirroredLightPos);

	// 타겟을 향한 방향 벡터 계산 및 적용
	Vec3 lightDir = targetPos - mirroredLightPos;
	lightDir.Normalize();
	lightObj->GetLight()->SetLightDirection(lightDir);
}