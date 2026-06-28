#include "pch.h"
#include "Device.h"

FVector2D Device::GetRSRate() const
{
    RECT windowRC;
    GetClientRect(_hWnd, &windowRC);
    
    float width  = static_cast<float>(windowRC.right  - windowRC.left);
    float height = static_cast<float>(windowRC.bottom - windowRC.top );
    
    return FVector2D(_resolution._width / width, _resolution._height / height);
}

void Device::Destroy() {}

bool Device::Init(HWND hwnd, uint32 width, uint32 height, bool windowMode)
{
    _hWnd               = hwnd;
    _resolution._width  = width;
    _resolution._height = height;
    _windowMode         = windowMode;
    
    uint32 flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    
    // flag |= D3D11_CREATE_DEVICE_DEBUG;
    
    D3D_FEATURE_LEVEL fLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL fLevelResult;
    
    if (D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, flag, &fLevel, 1, D3D11_SDK_VERSION, _device.GetAddressOf(), &fLevelResult, _context.GetAddressOf()))
        return false;
    
    uint32 sampleCount = 4;
    uint32 check       = 0;
    _device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &check);
    
    if (check <1)
        sampleCount = 1;
    
    DXGI_SWAP_CHAIN_DESC swapDesc = {};
        swapDesc.BufferDesc.Width                   = width;
        swapDesc.BufferDesc.Height                  = height;
        swapDesc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        swapDesc.BufferDesc.RefreshRate.Numerator   = 60;
        swapDesc.BufferDesc.RefreshRate.Denominator = 1; 
        swapDesc.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
        swapDesc.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapDesc.BufferCount                        = 1;
        swapDesc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapDesc.OutputWindow                       = _hWnd;
        swapDesc.SampleDesc.Quality                 = 0;
        swapDesc.SampleDesc.Count                   = sampleCount;
        swapDesc.Windowed                           = _windowMode;
        swapDesc.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
    
    ComPtr<IDXGIDevice> dxgiDevice = nullptr;
        _device->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice.GetAddressOf());
    ComPtr<IDXGIAdapter> adapter = nullptr;
        dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)adapter.GetAddressOf());
    ComPtr<IDXGIFactory> factory = nullptr;
        adapter->GetParent(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf());
    
    if (factory->CreateSwapChain(_device.Get(), &swapDesc, _swapChain.GetAddressOf()))
        return false;
    
    ComPtr<ID3D11Texture2D> backBuffer = nullptr;
    if (_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf()))
        return false;
    
    if (_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _targetView.GetAddressOf()))
        return false;
    
    D3D11_TEXTURE2D_DESC depthDesc = {};
        depthDesc.Width              = width;
        depthDesc.Height             = height;
        depthDesc.ArraySize          = 1;
        depthDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
        depthDesc.Usage              = D3D11_USAGE_DEFAULT;
        depthDesc.SampleDesc.Count   = sampleCount;
        depthDesc.SampleDesc.Quality = 0;
        depthDesc.MipLevels          = 1;
    
    ComPtr<ID3D11Texture2D> depthBuffer = nullptr;    
    if (_device->CreateTexture2D(&depthDesc, nullptr, depthBuffer.GetAddressOf()))
        return false;
    
    if (_device->CreateDepthStencilView(depthBuffer.Get(), nullptr, _depthStencil.GetAddressOf()))
        return false;
    
    D3D11_VIEWPORT vp = {};
        vp.Width  = static_cast<float>(width);
        vp.Height = static_cast<float>(height);
        vp.MaxDepth = 1;
    
    _context->RSSetViewports(1, &vp);
    
    if (D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, _factory2D.GetAddressOf()))
        return false;
    
    ComPtr<IDXGISurface> backSurface = nullptr;
    _swapChain->GetBuffer(0, IID_PPV_ARGS(backSurface.GetAddressOf()));
    
    D2D1_RENDER_TARGET_PROPERTIES prob = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
    
    if (_factory2D->CreateDxgiSurfaceRenderTarget(backSurface.Get(), prob, _target2D.GetAddressOf()))
        return false;
    
    return true;
}   

void Device::ClearBackBuffer(float clearColor[4])
{
    _context->ClearRenderTargetView(_targetView.Get(), clearColor);
}

void Device::ClearDepthStencil(float depth, uint8 stencil)
{
    _context->ClearDepthStencilView(_depthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void Device::SetTarget()
{
    _context->OMSetRenderTargets(1, _targetView.GetAddressOf(), _depthStencil.Get());
}

void Device::Render()
{
    _swapChain->Present(0, 0);
}
