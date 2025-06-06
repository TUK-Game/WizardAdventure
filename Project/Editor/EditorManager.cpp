#include "pch.h"
#include "EditorManager.h"
#include <Engine/Engine.h>

CEditorManager::CEditorManager()
    : m_hInstance(nullptr)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(0);
}

CEditorManager::~CEditorManager()
{
}

int CEditorManager::Init(HINSTANCE instance)
{
    m_hInstance = instance;

    if (FAILED(InitEngine()))
        return E_FAIL;

    return S_OK;
}

int CEditorManager::Run()
{
    return CEngine::GetInst()->Run();
}

LRESULT CEditorManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int CEditorManager::InitEngine()
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_EDITOR));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = L"";//MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.lpszClassName = L"WIZARD ADVENTURE EDITOR";

    HACCEL hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_EDITOR));

    return CEngine::GetInst()->Init(m_hInstance, hAccelTable, wcex, L"Wizard Adventure Editor", 1280, 720);
}
