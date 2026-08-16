#include "pch.h"
#include "CBuffer.h"
#include "Core/Device.h"
#include "Common/Info.h"

CBuffer::CBuffer() {}

CBuffer::~CBuffer() {}

bool CBuffer::Create(int Size, int Register, int Type)
{
    _Size = Size;
    _Register = Register;
    _Type = Type;
    
    D3D11_BUFFER_DESC Description = {};
    Description.Usage = D3D11_USAGE_DYNAMIC;
    Description.ByteWidth = Size;
    Description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    if (FAILED(Device::Instance().GetDevice()->CreateBuffer(&Description, nullptr, _buffer.GetAddressOf())))
        return false;
    
    return true;
}

void CBuffer::SetData(void* Data)
{
    D3D11_MAPPED_SUBRESOURCE map = {};
    CONTEXT->Map(_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
    memcpy(map.pData, Data, _Size);
    CONTEXT->Unmap(_buffer.Get(), 0);
    if (_Type& SHADER_TYPE::VERTEX)
        CONTEXT->VSSetConstantBuffers(_Register, 1, _buffer.GetAddressOf());
    if (_Type& SHADER_TYPE::PIXEL)
        CONTEXT->PSSetConstantBuffers(_Register, 1, _buffer.GetAddressOf());
}

void CBuffer::Update()
{}

void CBuffer::Destroy()
{}
