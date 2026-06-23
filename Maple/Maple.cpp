// Maple.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Maple.h"
#include "Core/Defines.h"
#include "Core/GameEngine.h"

#define MAX_LOADSTRING 100

bool g_isRun = false;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GameEngine::Instance().Init(hInstance,L"MapleStory");
    GameEngine::Instance().Run();
    GameEngine::Instance().Destroy();
    return TRUE;
}