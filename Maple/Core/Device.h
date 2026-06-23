#pragma once
#include "Common/Info.h"
#include "Common/Singleton.h"
#include "Common/Vector2D.h"
#include "Defines.h"

#define DEVICE  Device::Instance().GetDevice()
#define CONTEXT Device::Instance().GetContext()

class Device : public Singleton<Device>
{
public:
    Device() = default;
    ~Device() = default;
    Device(const Device&) = delete;
    Device(Device&&) = delete;
    Device& operator=(const Device&) = delete;
    Device& operator=(Device&&) = delete;
    
private:
    ComPtr<ID3D11Device>            _device       = nullptr;
    ComPtr<ID3D11DeviceContext>     _context      = nullptr;
    ComPtr<IDXGISwapChain>          _swapChain    = nullptr;
    ComPtr<ID3D11RenderTargetView>  _targetView   = nullptr;
    ComPtr<ID3D11DepthStencilView>  _depthStencil = nullptr;
    ComPtr<ID2D1RenderTarget>       _target2D;
    ComPtr<ID2D1Factory>            _factory2D;
    
public:
    ComPtr<ID2D1RenderTarget>       GetTarget2D()         const { return _target2D;     }
    ComPtr<ID3D11Device>            GetDevice()           const { return _device;       }
    ComPtr<ID3D11DeviceContext>     GetContext()          const { return _context;      }
    ComPtr<IDXGISwapChain>          GetSwapChain()        const { return _swapChain;    }
    ComPtr<ID3D11RenderTargetView>  GetTargetView()       const { return _targetView;   }
    ComPtr<ID3D11DepthStencilView>  GetDepthStencilView() const { return _depthStencil; }
    
    bool GetWindowMode()             { return _windowMode; }
    const FResolution& GetRS() const { return _resolution; }
    FVector2D GetRSRate() const;
    
private:
    HWND _hWnd = nullptr;
    FResolution _resolution = {};
    bool _windowMode = false;

public:
    virtual void Destroy() override;
    bool Init(HWND hwnd, uint32 width, uint32 height, bool windowMode);
    void ClearBackBuffer(float clearColor[4]);
    void ClearDepthStencil(float depth, uint8 stencil);
    void SetTarget();
    void Render();
};


