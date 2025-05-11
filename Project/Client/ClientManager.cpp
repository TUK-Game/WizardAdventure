#include "pch.h"
#include "ClientManager.h"
#include "Level_1.h"
#include "Level_Start.h"
#include <Engine/Engine.h>
#include <Engine/NetworkManager.h>
#include <Engine/SaveLoadManager.h>
#include <Engine/ServerSession.h>
#include <Engine/JsonConverter.h>
//#include <ImGui/imgui_impl_win32.h>

// PhysX TEMP ==================================================
#include <PxPhysicsAPI.h>

class UserErrorCallback : public physx::PxErrorCallback
{
public:
    virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
    {
        // error processing implementation
    }
};

static UserErrorCallback            g_DefaultErrorCallback;
static physx::PxDefaultAllocator    g_DefaultAllocatorCallback;

// =============================================================

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

    CLevel_Start* level = new CLevel_Start;
    if (FAILED(InitEngine()))
        return E_FAIL;

    std::cout << "====== 서버 연동(NUMPAD 0) ======" << std::endl;
    std::cout << std::endl;


#ifdef AUTO_SERVER_CONNECT
    std::cout << "====== 서버 연동 자동 ======" << std::endl;
    std::cout << std::endl;

    if (CEngine::GetInst()->GetNetworkType() == ENetwork_Type::Offline)
    {
        CEngine::GetInst()->SetNetworkType(ENetwork_Type::Online);
        CNetworkManager::GetInst()->Init();
    }
#endif

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
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_NUMPAD0:
        {
#ifndef AUTO_SERVER_CONNECT
            if (CEngine::GetInst()->GetNetworkType() == ENetwork_Type::Offline)
            {
                CEngine::GetInst()->SetNetworkType(ENetwork_Type::Online);
                CNetworkManager::GetInst()->Init();
            }
#endif
            break;
        }
        case VK_NUMPAD7:
        {
            CJsonConverter::GetInst()->SaveMapCollision(L"Level_1");
            break;
        }
        case VK_NUMPAD8:
        {
            CSaveLoadManager::GetInst()->Save();
            break;
        }
        case VK_NUMPAD9:
        {
            CSaveLoadManager::GetInst()->Load(L"Level_1");
            break;
        }
        }
    }
    break;
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
    {
        if (CEngine::GetInst()->GetNetworkType() == ENetwork_Type::Online)
        {
            CNetworkManager::GetInst()->s_GameSession->OnDisconnected();
        }
        PostQuitMessage(0);
    }
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
