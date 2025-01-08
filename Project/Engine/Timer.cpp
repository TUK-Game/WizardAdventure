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
	// 현재 시간을 기록
	m_CurTime = std::chrono::high_resolution_clock::now();

	// m_DT = (현재 타이머값 - 이전 프레임의 타이머값) / 초당 타이머값
	m_DeltaTime = std::chrono::duration<float, std::chrono::milliseconds::period>(m_CurTime - m_PrevTime).count() / 1000.f;

	// 현재 시간을 이전 시간으로 업데이트
	m_PrevTime = m_CurTime;

	// FPS 계산
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
