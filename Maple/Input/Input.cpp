#include "pch.h"
#include "Input.h"
#include "Core/Device.h"
#include "Core/GameEngine.h"
#include "World/World.h"
#include "World/Level.h"

bool Input::Init()
{
    _hInst = GameEngine::Instance().GetHINSTANCE();
   
	_hWnd  = GameEngine::Instance().GetHWND();
    
	if (FAILED(DirectInput8Create(_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)_Input.GetAddressOf(), nullptr)))
	{
		_Type = eInputSystemType::WINDOW;
	}
    else
	{
		_Type = eInputSystemType::DINPUT;
	}
	
	if (_Type == eInputSystemType::DINPUT)
	{
		if (false == InitInput())
		{
			return false;
		}
    }

    return true;
}

void Input::Tick(float DeltaTime)
{
    if (_Type == eInputSystemType::DINPUT)
    {
        UpdateKeyBoard();
       
		UpdateMouse();
    }
   
	UpdateMousePos(DeltaTime);
   
	UpdateInput(DeltaTime);
}

bool Input::GetKeyState(uint8 key)
{
    return _KeyState[key] & KEY_PUSH;
}

void Input::Destroy()
{}

bool Input::InitInput()
{
    if (FAILED(_Input->CreateDevice(GUID_SysKeyboard, _Keyboard.GetAddressOf(), nullptr)))
	{
		return false;
	}

    if (FAILED(_Keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}

    if (Device::Instance().GetWindowMode())
    {
        if (FAILED(_Keyboard->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			return false;
		}
    }
    else
    {
        if (FAILED(_Keyboard->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		{
			return false;
		}
    }
    
	if (FAILED(_Keyboard->Acquire()))
	{
		return false;
	}

    if (FAILED(_Input->CreateDevice(GUID_SysMouse, _Mouse.GetAddressOf(), nullptr)))
	{
		return false;
	}

    if (Device::Instance().GetWindowMode())
    {
        if (FAILED(_Mouse->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		{
			return false;
		}
    }
    else
    {
        if (FAILED(_Mouse->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_EXCLUSIVE)))
		{
			return false;
		}
    }

    if (FAILED(_Mouse->Acquire()))
	{
		return false;
	}

    return true;
}

bool Input::UpdateKeyBoard()
{
    if (!_Keyboard)
	{
		return false;
	}

    HRESULT ret = _Keyboard->GetDeviceState(256, (LPVOID)&_KeyState);
   
	if (FAILED(ret))
    {
        if (ret == DIERR_INPUTLOST || ret == DIERR_NOTACQUIRED)
		{
			_Keyboard->Acquire();
		}
    }

    return true;
}

bool Input::UpdateMouse()
{
	if (!_Mouse)
	{
		return false;
	}

	HRESULT ret = _Mouse->GetDeviceState(sizeof(_MouseState), (LPVOID)&_MouseState);
	
	if (FAILED(ret))
	{
		if (ret == DIERR_INPUTLOST || ret == DIERR_NOTACQUIRED)
		{
			_Mouse->Acquire();
		}
	}
	
	return true;
}

void Input::UpdateInput(float DeltaTime)
{
	if (_Type == eInputSystemType::DINPUT)
	{
		for (int i = 0; i < MOUSE_BUTTON_TYPE::Type::End; ++i)
		{
			if (_MouseState.rgbButtons[i] & KEY_PUSH)
			{
				if (!_MouseDown[i] && !_MouseHold[i])
				{
					_MouseDown[i] = true;
					
					_MouseHold[i] = true;
				}
				else
				{
					_MouseDown[i] = false;
				}
			}
			else
			{
				if (_MouseHold[i])
				{
					_MouseDown[i] = false;
					
					_MouseHold[i] = false;
					
					_MouseUp[i]   = true;
				}
				else
				{
					_MouseUp[i]   = false;
				}
			}
		}
	}
}

void Input::UpdateMousePos(float delatTime)
{
	POINT mousePointPos;
	GetCursorPos(&mousePointPos);
	ScreenToClient(_hWnd, &mousePointPos);
	FVector2D   ratio         = Device::Instance().GetRSRate();
	FResolution viewPortRS    = Device::Instance().GetRS();
	FVector2D   mousePosition;
	mousePosition._x          = mousePointPos.x * ratio._x;
	mousePosition._y          = mousePointPos.y * ratio._y;
	mousePosition._y          = viewPortRS._Height - mousePosition._y;
	if (_MouseCompute)
	{
		_MouseMove = mousePosition - _MousePosition;
	}
	else
	{
		_MouseCompute = true;
	}
	_MousePosition = mousePosition;
	Ptr<World> world = GameEngine::Instance().GetWorld();
	Ptr<Level> level = world->GetCurLevel();
	if (nullptr == level)
		return;
	FVector3D cameraPos = level->GetCameraWorldPosition();
	_MouseWorldPosition._x = cameraPos._x + _MousePosition._x - viewPortRS._Width * 0.5f;
	_MouseWorldPosition._y = cameraPos._y + _MousePosition._y - viewPortRS._Height * 0.5f;
}

uint8 Input::ConvertKey(uint8 key)
{
    if (_Type == eInputSystemType::DINPUT)
	{
		switch (key)
		{
		case VK_LBUTTON:
			return DIK_MOUSELBUTTON;
		case VK_RBUTTON:
			return DIK_MOUSERBUTTON;
		case VK_MBUTTON:
			return DIK_MOUSEWHEEL;
		case VK_BACK:
			return DIK_BACK;
		case VK_TAB:
			return DIK_TAB;
		case VK_RETURN:
			return DIK_RETURN;
		case VK_LCONTROL:
			return DIK_LCONTROL;
		case VK_RCONTROL:
			return DIK_RCONTROL;
		case VK_LMENU:
			return DIK_LALT;
		case VK_RMENU:
			return DIK_RALT;
		case VK_LSHIFT:
			return DIK_LSHIFT;
		case VK_RSHIFT:
			return DIK_RSHIFT;
		case VK_PAUSE:
			return DIK_PAUSE;
		case VK_CAPITAL:
			return DIK_CAPSLOCK;
		case VK_ESCAPE:
			return DIK_ESCAPE;
		case VK_SPACE:
			return DIK_SPACE;
		case VK_NEXT:
			return DIK_PGDN;
		case VK_PRIOR:
			return DIK_PGUP;
		case VK_END:
			return DIK_END;
		case VK_HOME:
			return DIK_HOME;
		case VK_LEFT:
			return DIK_LEFT;
		case VK_UP:
			return DIK_UP;
		case VK_RIGHT:
			return DIK_RIGHT;
		case VK_DOWN:
			return DIK_DOWN;
		case VK_PRINT:
			return DIK_SYSRQ;
		case VK_INSERT:
			return DIK_INSERT;
		case VK_DELETE:
			return DIK_DELETE;
		case VK_HELP:
			return 0;
		case '0':
			return DIK_0;
		case '1':
			return DIK_1;
		case '2':
			return DIK_2;
		case '3':
			return DIK_3;
		case '4':
			return DIK_4;
		case '5':
			return DIK_5;
		case '6':
			return DIK_6;
		case '7':
			return DIK_7;
		case '8':
			return DIK_8;
		case '9':
			return DIK_9;
		case 'A':
			return DIK_A;
		case 'B':
			return DIK_B;
		case 'C':
			return DIK_C;
		case 'D':
			return DIK_D;
		case 'E':
			return DIK_E;
		case 'F':
			return DIK_F;
		case 'G':
			return DIK_G;
		case 'H':
			return DIK_H;
		case 'I':
			return DIK_I;
		case 'J':
			return DIK_J;
		case 'K':
			return DIK_K;
		case 'L':
			return DIK_L;
		case 'M':
			return DIK_M;
		case 'N':
			return DIK_N;
		case 'O':
			return DIK_O;
		case 'P':
			return DIK_P;
		case 'Q':
			return DIK_Q;
		case 'R':
			return DIK_R;
		case 'S':
			return DIK_S;
		case 'T':
			return DIK_T;
		case 'U':
			return DIK_U;
		case 'V':
			return DIK_V;
		case 'W':
			return DIK_W;
		case 'X':
			return DIK_X;
		case 'Y':
			return DIK_Y;
		case 'Z':
			return DIK_Z;
		case VK_OEM_3:
			return DIK_GRAVE;
		case VK_OEM_MINUS:
			return DIK_MINUS;
		case VK_OEM_NEC_EQUAL:
			return DIK_EQUALS;
		case VK_OEM_4:
			return DIK_LBRACKET;
		case VK_OEM_6:
			return DIK_RBRACKET;
		case VK_OEM_5:
			return DIK_BACKSLASH;
		case VK_OEM_1:
			return DIK_SEMICOLON;
		case VK_OEM_7:
			return DIK_APOSTROPHE;
		case VK_OEM_COMMA:
			return DIK_COMMA;
		case VK_OEM_PERIOD:
			return DIK_PERIOD;
		case VK_OEM_2:
			return DIK_SLASH;
		case VK_NUMPAD0:
			return DIK_NUMPAD0;
		case VK_NUMPAD1:
			return DIK_NUMPAD1;
		case VK_NUMPAD2:
			return DIK_NUMPAD2;
		case VK_NUMPAD3:
			return DIK_NUMPAD3;
		case VK_NUMPAD4:
			return DIK_NUMPAD4;
		case VK_NUMPAD5:
			return DIK_NUMPAD5;
		case VK_NUMPAD6:
			return DIK_NUMPAD6;
		case VK_NUMPAD7:
			return DIK_NUMPAD7;
		case VK_NUMPAD8:
			return DIK_NUMPAD8;
		case VK_NUMPAD9:
			return DIK_NUMPAD9;
		case VK_MULTIPLY:
			return DIK_MULTIPLY;
		case VK_ADD:
			return DIK_ADD;
		case VK_SEPARATOR:
			return DIK_NUMPADCOMMA;
		case VK_SUBTRACT:
			return DIK_SUBTRACT;
		case VK_DECIMAL:
			return DIK_DECIMAL;
		case VK_DIVIDE:
			return DIK_DIVIDE;
			//case VK_RETURN:		
			//return DIK_NUMPADENTER;
		case VK_F1:
			return DIK_F1;
		case VK_F2:
			return DIK_F2;
		case VK_F3:
			return DIK_F3;
		case VK_F4:
			return DIK_F4;
		case VK_F5:
			return DIK_F5;
		case VK_F6:
			return DIK_F6;
		case VK_F7:
			return DIK_F7;
		case VK_F8:
			return DIK_F8;
		case VK_F9:
			return DIK_F9;
		case VK_F10:
			return DIK_F10;
		case VK_F11:
			return DIK_F11;
		case VK_F12:
			return DIK_F12;
		case VK_F13:
			return DIK_F13;
		case VK_F14:
			return DIK_F14;
		case VK_F15:
			return DIK_F15;
		case VK_F16:
		case VK_F17:
		case VK_F18:
		case VK_F19:
		case VK_F20:
		case VK_F21:
		case VK_F22:
		case VK_F23:
		case VK_F24:
			return 0;
		case VK_NUMLOCK:
			return DIK_NUMLOCK;
		case VK_SCROLL:
			return DIK_SCROLL;
		case VK_LWIN:
			return DIK_LWIN;
		case VK_RWIN:
			return DIK_RWIN;
		case VK_APPS:
			return DIK_APPS;
		case VK_OEM_102:
			return DIK_OEM_102;
		}
		return 0xff;
	}
	return key;
}
