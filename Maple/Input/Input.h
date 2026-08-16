#pragma once
#pragma comment(lib, "dinput8.lib")
#include "Core/Object.h"
#include "dinput.h"
#define DIRECTINPUT_VERSION 0x0800
#define DIK_MOUSELBUTTON    0xfc
#define DIK_MOUSERBUTTON    0xfd
#define DIK_MOUSEWHEEL      0xfe
#define KEY_PUSH            0x80

class Input : public Object
{
    friend class InputSystem;

public:
    Input() = default;
    virtual ~Input() = default;
    Input(const Input&) = delete;
    Input(Input&&) = delete;
    Input& operator=(const Input&) = delete;
    Input& operator=(Input&&) = delete;
    
private:
    HINSTANCE _hInst = 0;
    HWND      _hWnd  = 0;
    ComPtr<IDirectInput8>       _Input    = nullptr;
    ComPtr<IDirectInputDevice8> _Keyboard = nullptr;
    ComPtr<IDirectInputDevice8> _Mouse    = nullptr;
    eInputSystemType _Type = eInputSystemType::DINPUT;
    uint8 _KeyState[256] = {};
    DIMOUSESTATE _MouseState = {};
    bool _MouseDown[MOUSE_BUTTON_TYPE::End] = {};
    bool _MouseHold[MOUSE_BUTTON_TYPE::End] = {};
    bool _MouseUp[MOUSE_BUTTON_TYPE::End] = {};
    FVector2D _MousePosition;
    FVector2D _MouseWorldPosition;
    FVector2D _MouseMove;
    bool _MouseCompute = true;

public:
    bool GetMouseDown(MOUSE_BUTTON_TYPE::Type Type) { return _MouseDown[Type]; }
    bool GetMouseHold(MOUSE_BUTTON_TYPE::Type Type) { return _MouseHold[Type]; }
    bool GetMouseUp(MOUSE_BUTTON_TYPE::Type Type)   { return _MouseUp[Type];   }
    const FVector2D& GetMouseWorldPosition() const  { return _MouseWorldPosition;   }
    const FVector2D& GetMousePosition() const       { return _MousePosition;        }
    const FVector2D& GetMouseMove() const           { return _MouseMove;       }

public:
    bool Init();
    void Tick(float DeltaTime);
    bool GetKeyState(uint8 key);
    virtual void Destroy() override;

private:
    bool InitInput();
    bool UpdateKeyBoard();
    bool UpdateMouse();
    void UpdateInput(float DeltaTime);
    void UpdateMousePos(float delatTime);
    uint8 ConvertKey(uint8 key);
};
