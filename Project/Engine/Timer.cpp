#include "pch.h"
#include "Timer.h"
#include "Engine.h"

CTimer::CTimer()
	: m_FPS(0)
	, m_DeltaTime(0.f)
	, m_Time(0.f)
{
}

CTimer::~CTimer()
{
}

void CTimer::Init()
{
	m_CurTime = std::chrono::high_resolution_clock::now();
	m_PrevTime = m_CurTime;
}

void CTimer::Update()
{
	// ���� �ð��� ���
	m_CurTime = std::chrono::high_resolution_clock::now();

	// m_DT = (���� Ÿ�̸Ӱ� - ���� �������� Ÿ�̸Ӱ�) / �ʴ� Ÿ�̸Ӱ�
	m_DeltaTime = std::chrono::duration<float, std::chrono::milliseconds::period>(m_CurTime - m_PrevTime).count() / 1000.f;

	// ���� �ð��� ���� �ð����� ������Ʈ
	m_PrevTime = m_CurTime;

	// FPS ���
	++m_FPS;
	m_Time += m_DeltaTime;

	if (m_Time >= 1.f)
	{
		std::wstring str = std::format(L"FPS : {}", m_FPS);
		SetWindowText(CEngine::GetInst()->GetWindowInfo().hWnd, str.c_str());

		m_FPS = 0;
		m_Time -= 1.f;
	}
}
