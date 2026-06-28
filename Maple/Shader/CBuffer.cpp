#include "pch.h"
#include "CBuffer.h"
#include "Core/Device.h"
#include "Common/Info.h"

CBuffer::CBuffer() {}

CBuffer::~CBuffer() {}

bool CBuffer::Create(int size, int regi, int tpye)
{
    _size = 0;
    _register = 0;
    int _type = 0;
    
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = size;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    if (FAILED(Device::Instance().GetDevice()->CreateBuffer(&desc, nullptr, _buffer.GetAddressOf())))
        return false;
    
    return true;
}

void CBuffer::SetData(void* data)
{
    D3D11_MAPPED_SUBRESOURCE map = {};
    CONTEXT->Map(_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
    memcpy(map.pData, data, _size);
    CONTEXT->Unmap(_buffer.Get(), 0);
    if (_type& SHADER_TYPE::VERTEX)
        CONTEXT->VSSetConstantBuffers(_register, 1, _buffer.GetAddressOf());
    if (_type& SHADER_TYPE::PIXEL)
        CONTEXT->PSSetConstantBuffers(_register, 1, _buffer.GetAddressOf());
}

void CBuffer::Update()
{}

void CBuffer::Destroy()
{}
