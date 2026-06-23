#include "pch.h"
#include "GameEngine.h"
#include "DirectoryManager.h"
#include "Device.h"
#include "Defines.h"

void GameEngine::Destroy()
{
    DirectoryManager::Instance().Destroy();
}

bool GameEngine::Init(HINSTANCE inst, const wchar_t* name)
{
    _hInst = inst;
    
    lstrcpy(_className, name);
    lstrcpy(_titleName, name);
    
    RegisterWindowClass();
    
    if (!Create())
        return false;
    
    _hdc = GetDC(_hWnd);
    g_isRun = InitManager();
    
    return true;
}

int GameEngine::Run()
{
    MSG msg = {};
    while(g_isRun)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Logic();
        }
    }
    return (int)msg.wParam;
}

bool GameEngine::Create()
{
    _hWnd = CreateWindow(_className, _titleName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _hInst, nullptr);
    if (!_hWnd)
        return false;
    
    RECT windowRect = { 0, 0, 1920, 1080}; // 해상도
    
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);
    SetWindowPos(_hWnd, HWND_TOPMOST, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, SWP_NOMOVE | SWP_NOZORDER);
    ShowWindow(_hWnd, SW_SHOW);
    UpdateWindow(_hWnd);
    return true;
}

void GameEngine::RegisterWindowClass()
{
    WNDCLASSEX wcex;
        wcex.cbSize        = sizeof(WNDCLASSEX);
        wcex.style         = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc   = WndProc;
        wcex.cbClsExtra    = 0;
        wcex.cbWndExtra    = 0;
        wcex.hInstance     = _hInst;
        wcex.hIcon         = LoadIcon(_hInst, MAKEINTRESOURCE(IDI_MAPLE)); // 아이콘(ICON)
        wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName  = 0;
        wcex.lpszClassName = _className;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MAPLE));
    RegisterClassExW(&wcex);
}

void GameEngine::Logic()
{
    Render(0.f);
}

void GameEngine::Tick(float deltaTime)
{
}

void GameEngine::Collision(float deltaTime)
{
}

void GameEngine::Render(float deltaTime)
{
    Device::Instance().ClearBackBuffer(_clearColor);
    Device::Instance().ClearDepthStencil(1.f, 0);
    Device::Instance().SetTarget();
    Device::Instance().Render();
}

bool GameEngine::InitManager()
{
    DirectoryManager::Instance().Init();
    if (false == Device::Instance().Init(_hWnd, 1920, 1080, true))
    {
        return false;
    }
    
    return true;
}

LRESULT GameEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        {
            g_isRun = false;
            PostQuitMessage(0);
        }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
