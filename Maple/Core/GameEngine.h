#pragma once
#include "Defines.h"
#include "Maple.h"
#include "frameWork.h"
#include "Common/Singleton.h"

class GameEngine : public Singleton<GameEngine>
{
public:
    GameEngine()  = default;
    ~GameEngine() = default;
    GameEngine(const GameEngine&) = delete;
    GameEngine(GameEngine&&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;
    GameEngine& operator=(GameEngine&&) = delete;
    
private:
    HINSTANCE _hInst          = nullptr;
    HWND      _hWnd           = nullptr;
    HDC       _hdc            = nullptr;
    TCHAR     _ClassName[256] = {};
    TCHAR     _TitleName[256] = {};
    float     _ClearColor[4]  = { 0, 0, 0, 1 }; // 바탕색 : 하얀색
    
    Ptr<class World> _World;
    Ptr<class Input> _Input;
    
public:
    virtual void Destroy() override;
    
    bool Init(HINSTANCE inst, const wchar_t* name);
    int  Run();
    
    HDC       GetHDC()       const { return _hdc;   }
    HINSTANCE GetHINSTANCE() const { return _hInst; }
    HWND      GetHWND()      const { return _hWnd;  }
    
    Ptr<class World> GetWorld() const;
    
private:
    bool Create();
    void RegisterWindowClass();
    void Logic();
    void Tick(float DeltaTime);
    void Collision(float DeltaTime);
    void Render(float DeltaTime);
    bool InitManager();
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#define GET_WORLD GameEngine::Instance().GetWorld()
