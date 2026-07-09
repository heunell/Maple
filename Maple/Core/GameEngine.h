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
    TCHAR     _className[256] = {};
    TCHAR     _titleName[256] = {};
    float     _clearColor[4]  = { 1, 1, 1, 1 }; // 바탕색 : 하얀색
    
    Ptr<class World> _world;
    Ptr<class Input> _input;
    
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
    void Tick(float deltaTime);
    void Collision(float deltaTime);
    void Render(float deltaTime);
    bool InitManager();
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#define GET_WORLD GameEngine::Instance().GetWorld()
