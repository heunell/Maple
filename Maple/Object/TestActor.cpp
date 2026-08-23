#include "pch.h"
#include "TestActor.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AABBCollisionComponent.h"

TestActor::TestActor()
{
}

TestActor::~TestActor()
{
}

bool TestActor::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    if (!Actor::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    Ptr<StaticMeshComponent> MeshComponent = CreateSceneComponent<StaticMeshComponent>("DeadMesh");

    MeshComponent->SetMesh("TexRect");
    
    MeshComponent->AddTexture(0, "Dead", 0);

    SetRootComponent(MeshComponent);

    Ptr<AABBCollisionComponent> TestCollision = CreateSceneComponent<AABBCollisionComponent>("MonsterCollision");

    TestCollision->SetBoxSize(100.f, 100.f);

    TestCollision->SetRelativePosition(0.f, 0.f, 0.f);
    
    TestCollision->AttachToComponent(GetRoot());
    
    TestCollision->SetCollisionProfile("Monster");
    
    AddTag("Monster");

    return true;
}