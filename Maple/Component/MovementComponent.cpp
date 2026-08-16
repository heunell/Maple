#include "pch.h"
#include "MovementComponent.h"
#include "SceneComponent.h"
#include "Controller/PlayerController.h"
#include "Object/Pawn.h"

bool MovementComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    ActorComponent::Init(Id, Name, Owner);
    
    _Speed = 1.f;
    
    _Type  = COMPONENT_TYPE::MOVEMENT;
    
    return true;
}

void MovementComponent::Tick(float DeltaTime)
{
    ActorComponent::Tick(DeltaTime);
    
    if (nullptr == _UpdateComponent)
    {
        return;
    }

    if (_MoveAxis.Length() <= 0)
    {
        return;
    }

    FVector3D CurrentPos = _UpdateComponent->GetWorldPosition();
    
    _NextPosition = CurrentPos + (_MoveAxis * _Speed * DeltaTime);
    
    _UpdateComponent->SetWorldPosition(_NextPosition);
}

void MovementComponent::Destroy()
{
    ActorComponent::Destroy();
}

void MovementComponent::SetUpdateComponent(Ptr<class SceneComponent> updateComp)
{
    _UpdateComponent = updateComp;
}

Ptr<class SceneComponent> MovementComponent::GetUpdateComponent() const
{
    return _UpdateComponent;
}

void MovementComponent::SetMoveAxis(const FVector3D& moveAxis)
{
    _MoveAxis = moveAxis;
    
    _MoveAxis.Normalize();
}

void MovementComponent::AddMoveAxis(const FVector3D& moveAxis)
{
    _MoveAxis += moveAxis;
    
    _MoveAxis.Normalize();
}

void MovementComponent::Stop()
{
    _MoveAxis = FVector3D::Zero;
}
