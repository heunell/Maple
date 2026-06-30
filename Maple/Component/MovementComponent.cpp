#include "pch.h"
#include "MovementComponent.h"

bool MovementComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    return ActorComponent::Init(id, name, owner);
}

void MovementComponent::Tick(float deltaTime)
{
    ActorComponent::Tick(deltaTime);
}

void MovementComponent::Destroy()
{
    ActorComponent::Destroy();
}

void MovementComponent::SetUpdateComponent(Ptr<class SceneComponent> updateComp)
{
}

Ptr<class SceneComponent> MovementComponent::GetUpdateComponent() const
{
}

void MovementComponent::SetMoveAxis(const FVector3D& moveAxis)
{
}

void MovementComponent::AddMoveAxis(const FVector3D& moveAxis)
{
}

void MovementComponent::Stop()
{
}
