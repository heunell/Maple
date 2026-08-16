#include "pch.h"
#include "TestActor.h"
#include "Component/StaticMeshComponent.h"

TestActor::TestActor()
{
}

TestActor::~TestActor()
{
}

bool TestActor::Init(
    int32 Id,
    const FVector3D& Position,
    const FVector3D& Scale,
    const FRotator& Rotator,
    const std::string& Name)
{
    if (!Actor::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    Ptr<StaticMeshComponent> MeshComponent = CreateSceneComponent<StaticMeshComponent>("DeadMesh");

    MeshComponent->SetMesh("TexRect");
    MeshComponent->AddTexture(0, "Dead", 0);

    SetRootComponent(MeshComponent);

    return true;
}