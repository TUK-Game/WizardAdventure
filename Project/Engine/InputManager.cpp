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

	// �ʱ� ���콺��ǥ ����
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
	// ���� �����찡 ��Ŀ�� ���� ��
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		return;
#endif

	if (GetFocus() == CEngine::GetInst()->GetWindowInfo().hWnd)
	{
		for (size_t i = 0; i < m_vecKeyInfo.size(); ++i)
		{
			// ���� �ش� Ű�� �����ִ�.
			if (GetAsyncKeyState(m_arrKeyValue[i]) & 0x8001)
			{
				// �������� �����־���.
				if (m_vecKeyInfo[i].bPrevPressed)
				{
					m_vecKeyInfo[i].State = EKeyState::Push;
				}

				// ������ �������� �ʾҴ�.
				else
				{
					m_vecKeyInfo[i].State = EKeyState::Down;
				}

				m_vecKeyInfo[i].bPrevPressed = true;
			}

			// ���� �ش�Ű�� �ȴ����ִ�.
			else
			{
				// �������� �����־���.
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

		// ���콺 ��ǥ ����
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(CEngine::GetInst()->GetWindowInfo().hWnd, &pt);

		// ���� ������ ���콺��ǥ ���
		m_MousePrevPosition = m_MousePosition;

		// ���� ���콺 ��ǥ ���
		m_MousePosition = Vec2((float)pt.x, (float)pt.y);

		// ���콺 Ŀ���� �����ϴ� ���⺤�� ���
		m_DragDir = m_MousePosition - m_MousePrevPosition;
	}

	// ���� �����찡 ��Ȱ��ȭ �Ǿ� ���� ��
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

		// ���콺 ��ǥ ����
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

