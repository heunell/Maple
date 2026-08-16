#include "pch.h"
#include "MeshComponent.h"
#include "Core/AssetManager.h"
#include "Core/Mesh.h"
#include "Core/Material.h"
#include "Shader/ShaderManager.h"
#include "Shader/TransformCBuffer.h"
#include "World/Level.h"

MeshComponent::MeshComponent()
{}

MeshComponent::~MeshComponent()
{}

bool MeshComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    SceneComponent::Init(Id, Name, Owner);

    _Type = COMPONENT_TYPE::MESH;

    return true;
}

void MeshComponent::Tick(float DeltaTime)
{
    SceneComponent::Tick(DeltaTime);
}

void MeshComponent::Collision(float DeltaTime)
{
    SceneComponent::Collision(DeltaTime);
}

void MeshComponent::Render(float DeltaTime)
{
    SceneComponent::Render(DeltaTime);

    Ptr<Level> level = Lock<Level>(_Level);

    if (nullptr == level)
    {
        return;
    }

    FMatrix View = level->GetViewMatrix();

    FMatrix Projection = level->GetProjectionMatrix();

    _TransformCBuffer->SetWorldMatrix(_Matrix._World);

    _TransformCBuffer->SetProjectionMatrix(Projection);

    _TransformCBuffer->SetViewMatrix(View);

    _TransformCBuffer->Update();
}

void MeshComponent::Destroy()
{
    SceneComponent::Destroy();

    for (auto& It : _MaterialSlots)
    {
        DESTROY(It);
    }
}
