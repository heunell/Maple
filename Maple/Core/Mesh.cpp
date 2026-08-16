#include "pch.h"
#include "Mesh.h"
#include "Device.h"
#include "Material.h"
#include "AssetManager.h"

Mesh::Mesh()
{}

Mesh::~Mesh()
{}

bool Mesh::CreateMesh(void* VertexData, int32 Size, int32 Count, D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive, void* IndexData, int32 IndexSize, int32 IndexCount, DXGI_FORMAT Format, D3D11_USAGE IndexUsage)
{
    _VertexBuffer._Size = Size;
    
    _VertexBuffer._Count = Count;
    
    _VertexBuffer._Data.resize(Size * Count);

    memcpy(_VertexBuffer._Data.data(), VertexData, Size * Count);

    if (!CreateBuffer(_VertexBuffer._Buffer, D3D11_BIND_VERTEX_BUFFER, VertexData, Size, Count, VertexUsage))
    {
        return false;
    }

    _Primitive = Primitive;

    if (IndexData)
    {
        FMeshSlot Slot;

        Slot._IndexBuffer._Size   = IndexSize;
                                  
        Slot._IndexBuffer._Count  = IndexCount;
                                  
        Slot._IndexBuffer._Data.resize(IndexSize * IndexCount);
        
        Slot._IndexBuffer._Format = Format;
    
        memcpy(Slot._IndexBuffer._Data.data(), IndexData, IndexSize * IndexCount);

        if (!CreateBuffer(Slot._IndexBuffer._buffer, D3D11_BIND_INDEX_BUFFER, IndexData, IndexSize, IndexCount, IndexUsage))
        {
            return false;
        }

        Slot._Material = MATERIAL_MANAGER->CreateMaterialInstance("DefaultMaterial");

        _MeshSlots.push_back(Slot);
    }

    return true;
}

bool Mesh::CreateBuffer(ComPtr<ID3D11Buffer>& Buffer, D3D11_BIND_FLAG Flag, void* Data, int32 Size, int32 Count, D3D11_USAGE Usage)
{
    D3D11_BUFFER_DESC BufferDescription = {};

    BufferDescription.ByteWidth = Size * Count;

    BufferDescription.Usage = Usage;

    BufferDescription.BindFlags = Flag;

    BufferDescription.StructureByteStride = Size;

    if (Usage == D3D11_USAGE_DYNAMIC)
    {
        BufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else if (Usage == D3D11_USAGE_STAGING)
    {
        BufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
    }

    D3D11_SUBRESOURCE_DATA BufferData = {};

    BufferData.pSysMem = Data;

    if (FAILED(Device::Instance().GetDevice()->CreateBuffer(&BufferDescription, &BufferData, Buffer.GetAddressOf())))
    {
        return false;
    }

    return true;
}

void Mesh::Render()
{
    uint32 Stride = _VertexBuffer._Size;

    uint32 Offset = 0;

    Device::Instance().GetContext()->IASetPrimitiveTopology(_Primitive);

    Device::Instance().GetContext()->IASetVertexBuffers(0, 1, _VertexBuffer._Buffer.GetAddressOf(), &Stride, &Offset);

    size_t Size = _MeshSlots.size();

    if (Size > 0)
    {
        for (size_t i = 0; i < Size; ++i)
        {
            Device::Instance().GetContext()->IASetIndexBuffer(_MeshSlots[i]._IndexBuffer._buffer.Get(), _MeshSlots[i]._IndexBuffer._Format, 0);

            Device::Instance().GetContext()->DrawIndexed(_MeshSlots[i]._IndexBuffer._Count, 0, 0);
        }
    }
    else
    {
        Device::Instance().GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

        Device::Instance().GetContext()->Draw(_VertexBuffer._Count, 0);
    }
}

void Mesh::RenderInstancing(int32 Count)
{
    uint32 Stride = _VertexBuffer._Size;

    uint32 Offset = 0;

    Device::Instance().GetContext()->IASetPrimitiveTopology(_Primitive);
    
    Device::Instance().GetContext()->IASetVertexBuffers(0, 1, _VertexBuffer._Buffer.GetAddressOf(), &Stride, &Offset);

    Device::Instance().GetContext()->IASetIndexBuffer(_MeshSlots[0]._IndexBuffer._buffer.Get(), _MeshSlots[0]._IndexBuffer._Format, 0);

    Device::Instance().GetContext()->DrawIndexedInstanced(_MeshSlots[0]._IndexBuffer._Count, Count, 0, 0, 0);
}

void Mesh::Destroy()
{}
