// main.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "EditorManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    if (FAILED(CEditorManager::GetInst()->Init(hInstance)))
    {
        MessageBox(nullptr, L"Init Failed!", L"Init Error", MB_OK);
    }
    else
    {
        CEditorManager::GetInst()->Run();
    }
}
