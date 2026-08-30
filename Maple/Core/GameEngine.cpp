#include "pch.h"
#include "GameEngine.h"
#include "DirectoryManager.h"
#include "Device.h"
#include "Defines.h"
#include "TimeManager.h"
#include "AssetManager.h"
#include "Input/Input.h"
#include "Input/InputSystem.h"
#include "Collision/CollisionProfileManager.h"
#include "Shader/ShaderManager.h"
#include "Render/RenderManager.h"
#include "World/World.h"

#include "Game/Map/Boss/LucidEntry.h"

void GameEngine::Destroy()
{
    _World->Destroy();

    AssetManager::Instance().Destroy();

    ShaderManager::Instance().Destroy();

    _Input->Destroy();

    InputSystem::Instance().Destroy();

    DirectoryManager::Instance().Destroy();
}

bool GameEngine::Init(HINSTANCE Instance, const wchar_t* Name)
{
    _hInst = Instance;
    
    lstrcpy(_ClassName, Name);
    
    lstrcpy(_TitleName, Name);
    
    RegisterWindowClass();
    
    if (!Create())
    {
        return false;
    }
    
    _hdc = GetDC(_hWnd);

    g_isRun = InitManager();
    
    _World = New<World>();

    //_World->Init("LucidEntry");
    
    _World->Init("LucidPhase1");
    // Map List : LucidEntry , LucidPhase1 , LucidPhase2 , LucidReward

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

Ptr<class World> GameEngine::GetWorld() const
{
    return _World;
}

bool GameEngine::Create()
{
    _hWnd = CreateWindow(_ClassName, _TitleName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _hInst, nullptr);
    
    if (!_hWnd)
    {
        return false;
    }
    
    RECT windowRect = { 0, 0, 1366, 768}; // 해상도
    
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
    
    wcex.lpszClassName = _ClassName;
    
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MAPLE));
    
    RegisterClassExW(&wcex);
}

void GameEngine::Logic()
{
    float DeltaTime = TimeManager::Instance().Tick();

    Tick(DeltaTime);

    Collision(DeltaTime);

    Render(DeltaTime);
}

void GameEngine::Tick(float DeltaTime)
{
    _Input->Tick(DeltaTime);

    InputSystem::Instance().Tick(DeltaTime);

    // todo : Sound

    _World->Tick(DeltaTime);

    RenderManager::Instance().Tick(DeltaTime);
}

void GameEngine::Collision(float DeltaTime)
{
    _World->Collision(DeltaTime);
}

void GameEngine::Render(float DeltaTime)
{
    Device::Instance().ClearBackBuffer(_ClearColor);
    
    Device::Instance().ClearDepthStencil(1.f, 0);
    
    Device::Instance().SetTarget();
    
    _World->Render(DeltaTime);

    RenderManager::Instance().Render(DeltaTime);

    _World->RenderUI(DeltaTime);

    Device::Instance().Render();
}

bool GameEngine::InitManager()
{
    DirectoryManager::Instance().Init();

    if (false == TimeManager::Instance().Init())
    {
        return false;
    }

    if (false == Device::Instance().Init(_hWnd, 1366, 768, true)) // 해상도 같이 바꿔줘야함
    {
        return false;
    }

    if (false == ShaderManager::Instance().Init())
    {
        return false;
    }

    if (false == AssetManager::Instance().Init())
    {
        return false;
    }

    _Input = New<Input>();

    _Input->Init();

    if (false == InputSystem::Instance().Init(_Input))
    {
        return false;
    }

    if (false == CollisionProfileManager::Instance().Init())
    {
        return false;
    }

    if (false == RenderManager::Instance().Init())
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
