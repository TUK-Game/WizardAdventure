#include "pch.h"
#include "ClientManager.h"
#include <Engine/Engine.h>
//#include <ImGui/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CClientManager::CClientManager()
    : m_hInstance(nullptr)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(100);
    //if (GetModuleHandle(L"WinPixGpuCapturer.dll") == 0)
    //{
    //    LoadLibrary(GetLatestWinPixGpuCapturerPath_Cpp17().c_str());
    //}
}

CClientManager::~CClientManager()
{
}

int CClientManager::Init(HINSTANCE instance)
{
    m_hInstance = instance;

    if (FAILED(InitEngine()))
        return E_FAIL;

    return S_OK;
}

int CClientManager::Run()
{
    return CEngine::GetInst()->Run();
}

LRESULT CClientManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
            if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
                return true; // ImGui가 메시지를 처리했으면 OS 기본 처리 없이 반환

            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
            return true; // ImGui가 메시지를 처리했으면 OS 기본 처리 없이 반환

        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int CClientManager::InitEngine()
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = L"";//MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.lpszClassName = L"WIZARD ADVENTURE";

    HACCEL hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    return CEngine::GetInst()->Init(m_hInstance, hAccelTable, wcex, L"Wizard Adventure", 1280, 720);
}
