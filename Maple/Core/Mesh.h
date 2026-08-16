#pragma once
#include "Asset.h"

struct FMeshSlot
{
    FIndexBuffer _IndexBuffer;
    Ptr<class Material> _Material;
};

class Mesh : public Asset
{
public:
    Mesh();
    virtual ~Mesh();

protected:
    FVertexBuffer _VertexBuffer;

    std::vector<FMeshSlot> _MeshSlots;

    D3D11_PRIMITIVE_TOPOLOGY _Primitive;

public:
    bool CreateMesh(void* VertexData, int32 Size, int32 Count, D3D11_USAGE VertexUsage,
        D3D11_PRIMITIVE_TOPOLOGY Primitive, void* IndexData, int32 IndexSize, int32 IndexCount,
        DXGI_FORMAT Format, D3D11_USAGE IndexUsage = D3D11_USAGE_DEFAULT);

    bool CreateBuffer(ComPtr<ID3D11Buffer>& Buffer, D3D11_BIND_FLAG Flag, void* Data, int32 Size, int32 Count, D3D11_USAGE Usage);

    void Render();

    void RenderInstancing(int32 Count);

    size_t GetgSlotCount()
    {
        return _MeshSlots.size();
    }

    const FMeshSlot* GetSlot(int Index)
    {
        if (Index < 0 || Index >= _MeshSlots.size())
        {
            return nullptr;
        }

        return &_MeshSlots[Index];
    }

    virtual void Destroy() override;
};
