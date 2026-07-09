#include "pch.h"
#include "MovementComponent.h"

#include "SceneComponent.h"

bool MovementComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    ActorComponent::Init(id, name, owner);
    _speed = 1.f;
    _type  = COMPONENT_TYPE::MOVEMENT;
    return true;
}

void MovementComponent::Tick(float deltaTime)
{
    ActorComponent::Tick(deltaTime);
    if (nullptr == _updateComponent)
        return;
    if (_moveAxis.Length() <= 0)
        return;
    FVector3D curPos = _updateComponent->GetWorldPosition();
    _nextPosition = curPos + (_moveAxis * _speed * deltaTime);
    _updateComponent->SetWorldPosition(_nextPosition);
}

void MovementComponent::Destroy()
{
    ActorComponent::Destroy();
}

void MovementComponent::SetUpdateComponent(Ptr<class SceneComponent> updateComp)
{
    _updateComponent = updateComp;
}

Ptr<class SceneComponent> MovementComponent::GetUpdateComponent() const
{
    return _updateComponent;
}

void MovementComponent::SetMoveAxis(const FVector3D& moveAxis)
{
    _moveAxis = moveAxis;
    _moveAxis.Normalize();
}

void MovementComponent::AddMoveAxis(const FVector3D& moveAxis)
{
    _moveAxis += moveAxis;
    _moveAxis.Normalize();
}

void MovementComponent::Stop()
{
    _moveAxis = FVector3D::Zero;
}
