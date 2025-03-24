#include "pch.h"
#include "InputManager.h"
#include "Engine.h"

CInputManager::CInputManager()
{
	m_arrKeyValue =
	{
		'Q', 'W', 'E', 'R', 'T', 'Y',
		'A', 'S', 'D', 'F', 'G', 'H',
		'Z', 'X', 'C',  'V',  'B',  'N', 'M',

		VK_RETURN,
		VK_ESCAPE,
		VK_LSHIFT,
		VK_MENU,
		VK_CONTROL,
		VK_SPACE,
		VK_TAB,

		VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT,

		'0',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',

		//VK_NUMPAD0,
		//VK_NUMPAD1,
		//VK_NUMPAD2,
		//VK_NUMPAD3,
		//VK_NUMPAD4,
		//VK_NUMPAD5,
		//VK_NUMPAD6,
		//VK_NUMPAD7,
		//VK_NUMPAD8,
		//VK_NUMPAD9,

		VK_LBUTTON,
		VK_RBUTTON,
	};

}

CInputManager::~CInputManager()
{
}

int CInputManager::Init()
{
	for (size_t i = 0; i < (int)EKey::Max; ++i)
	{
		m_vecKeyInfo.push_back(KeyInfo{ EKeyState::None, false });
	}

	// 초기 마우스좌표 세팅
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(CEngine::GetInst()->GetWindowInfo().hWnd, &pt);

	m_MousePosition = Vec2((float)pt.x, (float)pt.y);
	m_MousePrevPosition = m_MousePosition;

	return S_OK;
}

void CInputManager::Update()
{
#ifdef READY_IMGUI
	// 게임 윈도우가 포커싱 중일 때
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		return;
#endif

	if (GetFocus() == CEngine::GetInst()->GetWindowInfo().hWnd)
	{
		for (size_t i = 0; i < m_vecKeyInfo.size(); ++i)
		{
			// 현재 해당 키가 눌려있다.
			if (GetAsyncKeyState(m_arrKeyValue[i]) & 0x8001)
			{
				// 이전에도 눌려있었다.
				if (m_vecKeyInfo[i].bPrevPressed)
				{
					m_vecKeyInfo[i].State = EKeyState::Push;
				}

				// 이전에 눌려있지 않았다.
				else
				{
					m_vecKeyInfo[i].State = EKeyState::Down;
				}

				m_vecKeyInfo[i].bPrevPressed = true;
			}

			// 현재 해당키가 안눌려있다.
			else
			{
				// 이전에는 눌려있었다.
				if (m_vecKeyInfo[i].bPrevPressed)
				{
					m_vecKeyInfo[i].State = EKeyState::Up;
				}

				else
				{
					m_vecKeyInfo[i].State = EKeyState::None;
				}

				m_vecKeyInfo[i].bPrevPressed = false;
			}
		}

		// 마우스 좌표 갱신
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(CEngine::GetInst()->GetWindowInfo().hWnd, &pt);

		// 이전 프레임 마우스좌표 기록
		m_MousePrevPosition = m_MousePosition;

		// 현재 마우스 좌표 기록
		m_MousePosition = Vec2((float)pt.x, (float)pt.y);

		// 마우스 커서가 진행하는 방향벡터 계산
		m_DragDir = m_MousePosition - m_MousePrevPosition;
	}

	// 게임 윈도우가 비활성화 되어 있을 때
	else
	{
		for (KeyInfo keyInfo : m_vecKeyInfo)
		{
			if (EKeyState::Down == keyInfo.State
				|| EKeyState::Push == keyInfo.State)
			{
				keyInfo.State = EKeyState::Up;
			}

			else if (EKeyState::Up == keyInfo.State)
			{
				keyInfo.State = EKeyState::None;
			}

			keyInfo.bPrevPressed = false;
		}

		// 마우스 좌표 갱신
		*((int*)&m_MousePosition.x) = 0xffffffff;
		*((int*)&m_MousePosition.y) = 0xffffffff;
	}
}

bool CInputManager::IsMouseOnClient()
{
	Vec2 resolution = CEngine::GetInst()->GetResolution();

	if (resolution.x <= m_MousePosition.x || resolution.y <= m_MousePosition.y ||
		m_MousePosition.x < 0 || m_MousePosition.y < 0)
	{
		return false;
	}

	return true;
}

