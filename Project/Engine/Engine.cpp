#include "pch.h"
#include "Engine.h"
#include "Device.h"
#include "Timer.h"
#include "PathManager.h"
#include "AssetManager.h"
#include "LevelManager.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "CollisionManager.h"
CEngine::CEngine()
    : m_WindowInfo{}
    , m_Timer(nullptr)
    , m_DeltaTime(0.f)
{
}

CEngine::~CEngine()
{
    delete m_Timer;
}

int CEngine::Init(HINSTANCE hInstance, HACCEL hAccelTable, const WNDCLASSEXW& wcex, const std::wstring& titleName, int width, int height, bool bWindowed)
{
    m_WindowInfo.hInstance = hInstance;
    m_WindowInfo.hAccelTable = hAccelTable;
    m_WindowInfo.TitleName = titleName;
    m_WindowInfo.Width = width;
    m_WindowInfo.Height = height;
    m_WindowInfo.bWindowed = bWindowed;

    if (FAILED(CreateMainWindow(wcex)))
        return E_FAIL;

    ResizeWindow(m_WindowInfo.Width, m_WindowInfo.Height);

    if (FAILED(CDevice::GetInst()->Init()))
        return E_FAIL;  


    m_Timer = new CTimer;
    m_Timer->Init();

    CPathManager::GetInst()->Init();

    if (FAILED(CAssetManager::GetInst()->Init()))
        return E_FAIL;

    if (FAILED(CInputManager::GetInst()->Init()))
        return E_FAIL;

    if (FAILED(CCollisionManager::GetInst()->Init()))
        return E_FAIL;

    if (FAILED(CLevelManager::GetInst()->Init()))
        return E_FAIL;

    return S_OK;
}

int CEngine::Run()
{
    MSG msg;

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            if (!TranslateAccelerator(msg.hwnd, m_WindowInfo.hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            CEngine::GetInst()->Progress();
        }
    }

    return (int)msg.wParam;
}

void CEngine::ResizeWindow(int width, int height)
{
    // 실제 해상도에 맞는 크기로 다시 계산
    RECT rc = { 0, 0, m_WindowInfo.Width, m_WindowInfo.Height };
    AdjustWindowRect(&rc, WS_EX_OVERLAPPEDWINDOW, !!GetMenu(m_WindowInfo.hWnd));
    SetWindowPos(m_WindowInfo.hWnd, nullptr, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0);

    // 화면 중앙으로 이동
    POINT pos = { ::GetDeviceCaps(m_WindowInfo.hDC, HORZRES) / 2, ::GetDeviceCaps(m_WindowInfo.hDC, VERTRES) / 2 };
    pos.x -= m_WindowInfo.Width / 2;
    pos.y -= m_WindowInfo.Height / 2 - rc.top;
    SetWindowPos(m_WindowInfo.hWnd, nullptr, pos.x, pos.y, rc.right - rc.left, rc.bottom - rc.top, 0);

    //CDevice::GetInst()->GetDepthStencilBuffer()->Init(m_WindowInfo);
}

int CEngine::CreateMainWindow(const WNDCLASSEXW& wcex)
{
    RegisterClassExW(&wcex);

    m_WindowInfo.hWnd = CreateWindowW(wcex.lpszClassName, m_WindowInfo.TitleName.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_WindowInfo.hInstance, nullptr);

    if (!m_WindowInfo.hWnd)
        return E_FAIL;

    m_WindowInfo.hDC = GetDC(m_WindowInfo.hWnd);

    // ResizeWindow
    RECT rc = { 0, 0, m_WindowInfo.Width, m_WindowInfo.Height };
    AdjustWindowRect(&rc, WS_EX_OVERLAPPEDWINDOW, !!GetMenu(m_WindowInfo.hWnd));
    SetWindowPos(m_WindowInfo.hWnd, nullptr, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0);

    // 화면 중앙으로 이동
    POINT pos = { ::GetDeviceCaps(m_WindowInfo.hDC, HORZRES) / 2, ::GetDeviceCaps(m_WindowInfo.hDC, VERTRES) / 2 };
    pos.x -= m_WindowInfo.Width / 2;
    pos.y -= m_WindowInfo.Height / 2 - rc.top;
    SetWindowPos(m_WindowInfo.hWnd, nullptr, pos.x, pos.y, rc.right - rc.left, rc.bottom - rc.top, 0);

    ShowWindow(m_WindowInfo.hWnd, SW_SHOW);
    UpdateWindow(m_WindowInfo.hWnd);

    return S_OK;
}

void CEngine::Progress()
{
    m_Timer->Update();
    m_DeltaTime = m_Timer->GetDeltaTime();

    CInputManager::GetInst()->Update();
    CLevelManager::GetInst()->Progress();

    CRenderManager::GetInst()->Render();
}
