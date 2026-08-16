#include "pch.h"
#include "MeshManager.h"
#include "Mesh.h"

MeshManager::MeshManager()
{}

MeshManager::~MeshManager()
{}

Ptr<class Mesh> MeshManager::FindMesh(const std::string& Name)
{
    auto it = _Mesh.find(Name);

    if (_Mesh.end() == it)
    {
        return nullptr;
    }

    return it->second;
}

bool MeshManager::CreateMesh(const std::string& Name, void* VertexData, int32 Size, int32 Count, D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive, void* IndexData, int32 IndexSize, int32 IndexCount, DXGI_FORMAT Fmt, D3D11_USAGE IndexUsage)
{
    Ptr<Mesh> mesh = FindMesh(Name);

    if (mesh)
    {
        return false;
    }

    mesh = New<Mesh>();
    if(false == mesh->CreateMesh(VertexData, Size, Count, VertexUsage, Primitive, IndexData, IndexSize, IndexCount, Fmt, IndexUsage))
    {
        Delete(mesh);

        return false;
    }

    _Mesh[Name] = mesh;

    return true;
}

bool MeshManager::Init()
{
    FVertexColor Triangle[3]
    {
        FVertexColor( 0.f,   0.5f, 0.f, 1.f, 0.f, 0.f, 1.f),
        FVertexColor( 0.5f, -0.5f, 0.f, 0.f, 1.f, 0.f, 1.f),
        FVertexColor(-0.5f, -0.5,  0.f, 0.f, 0.f, 1.f, 1.f)
    };

    uint16 TriangleIndex[3] = { 0, 1, 2 };

    if (!CreateMesh("Triangle", Triangle, sizeof(FVertexColor), 3, D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, TriangleIndex, sizeof(uint16), 3, DXGI_FORMAT_R16_UINT))
    {
        return false;
    }

    FVertexColor TriangleRed[3]
    {
        FVertexColor(0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f),
        FVertexColor(0.f, -0.5f, 0.f, 1.f, 0.f, 0.f, 1.f),
        FVertexColor(-0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f)
    };

    if (!CreateMesh("TriangleRed", TriangleRed, sizeof(FVertexColor), 3, D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, TriangleIndex, sizeof(uint16), 3, DXGI_FORMAT_R16_UINT))
    {
        return false;
    }

    FVector3D FrameRect[4]
    {
        FVector3D(-0.5, 0.5f, 0.f),
        FVector3D(0.5, 0.5f, 0.f),
        FVector3D(-0.5, -0.5f, 0.f),
        FVector3D(0.5, -0.5f, 0.f),
    };

    uint16 FrameRectIndex[5] = { 0,1,3,2,0 };

    if (!CreateMesh("FrameRect", FrameRect, sizeof(FVector3D), 4, D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, FrameRectIndex, sizeof(uint16), 5, DXGI_FORMAT_R16_UINT))
    {
        return false;
    }

    FVector3D SpherePoint[37];

    for (int i = 0; i < 37; ++i)
    {
        float Angle = DirectX::XMConvertToRadians(i * 10.f);

        SpherePoint[i]._x = cosf(Angle) * 0.5f;
        SpherePoint[i]._y = sinf(Angle) * 0.5f;
    }

    uint16 SpherIndex[38] = {};

    for (int i = 0; i < 37; ++i)
    {
        SpherIndex[i] = i;
    }

    SpherIndex[37] = 0;

    if (!CreateMesh("FrameSphere", SpherePoint, sizeof(FVector3D), 37, D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, SpherIndex, sizeof(uint16), 38, DXGI_FORMAT_R16_UINT))
    {
        return false;
    }

    FVertexTexture TexRect[4]
    {
        FVertexTexture(-0.5f, 0.5f, 0.f, 0.f, 0.f),
        FVertexTexture(0.5f, 0.5f, 0.f, 1.f, 0.f),
        FVertexTexture(-0.5f, -0.5f, 0.f, 0.f, 1.f),
        FVertexTexture(0.5f, -0.5f, 0.f, 1.f, 1.f)
    };

    uint16 RectIndex[6] = { 0, 1, 3, 0, 3, 2 };

    if (!CreateMesh("TexRect", TexRect, sizeof(FVertexTexture), 4, D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, RectIndex, sizeof(uint16), 6, DXGI_FORMAT_R16_UINT))
    {
        return false;
    }

    return true;
}

void MeshManager::Destroy()
{
    for (auto it : _Mesh)
    {
        if (nullptr == it.second)
        {
            continue;

            DESTROY(it.second)
        }
    }

    _Mesh.clear();
}
