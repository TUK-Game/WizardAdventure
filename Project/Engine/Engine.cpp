#include "pch.h"
#include "Engine.h"

CEngine::CEngine()
    : m_hInstance(nullptr)
    , m_hAccelTable(nullptr)
    , m_hMainWnd(nullptr)
    , m_hMainDC(nullptr)
    , m_Resolution{}
{
}

CEngine::~CEngine()
{
}

int CEngine::Init(HINSTANCE hInstance, HACCEL hAccelTable, const WNDCLASSEXW& wcex, const std::wstring& titleName, int width, int height)
{
    m_hInstance = hInstance;
    m_hAccelTable = hAccelTable;
    m_TitleName = titleName;
    m_Resolution = { width, height };

    if (FAILED(CreateMainWindow(wcex)))
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

            if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
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

int CEngine::CreateMainWindow(const WNDCLASSEXW& wcex)
{
    RegisterClassExW(&wcex);

    m_hMainWnd = CreateWindowW(wcex.lpszClassName, m_TitleName.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInstance, nullptr);

    if (!m_hMainWnd)
        return E_FAIL;

    ShowWindow(m_hMainWnd, SW_SHOW);
    UpdateWindow(m_hMainWnd);

    m_hMainDC = GetDC(m_hMainWnd);

    // 입력된 해상도에 맞는 윈도우 크기 설정

    // 실제 해상도에 맞는 크기로 다시 계산
    RECT rc = { 0, 0, (int)m_Resolution.x, (int)m_Resolution.y };
    AdjustWindowRect(&rc, WS_EX_OVERLAPPEDWINDOW, !!GetMenu(m_hMainWnd));
    SetWindowPos(m_hMainWnd, nullptr, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0);

    // 화면 중앙으로 이동
    POINT pos = { ::GetDeviceCaps(m_hMainDC, HORZRES) / 2, ::GetDeviceCaps(m_hMainDC, VERTRES) / 2 };
    pos.x -= (int)m_Resolution.x / 2;
    pos.y -= (int)m_Resolution.y / 2 - rc.top;
    SetWindowPos(m_hMainWnd, nullptr, pos.x, pos.y, rc.right - rc.left, rc.bottom - rc.top, 0);

    return S_OK;
}

void CEngine::Progress()
{
}
