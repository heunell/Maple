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
    
    const float _MaxFallSpeed = -200.f;

    if (nullptr == _UpdateComponent)
    {
        return;
    }

    if(!_IsLanding)
    {
        _Velocity._y += _Gravity * DeltaTime * 8.0f;

        if (_Velocity._y <= _MaxFallSpeed)
        {
            _Velocity._y = _MaxFallSpeed;
        }
    }
    else if(_Velocity._y < 0.f)
    {
        _Velocity._y = 0.f;
    }

    const FVector3D CurrentPosition = _UpdateComponent->GetWorldPosition();

    const FVector3D InputVelocity = _MoveAxis * _Speed;
    
    _NextPosition = CurrentPosition + (InputVelocity + _Velocity) * DeltaTime;

    _UpdateComponent->SetWorldPosition(_NextPosition);

    _IsLanding = false;
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

void MovementComponent::ResetMovement()
{
    Stop();

    _Velocity = FVector3D::Zero;

    _IsLanding = true;

    if (_UpdateComponent)
    {
        _NextPosition = _UpdateComponent->GetWorldPosition();
    }
    else
    {
        _NextPosition = FVector3D::Zero;
    }
}

void MovementComponent::SetLanding(bool IsLanding)
{
    _IsLanding = IsLanding;

    if(_IsLanding && _Velocity._y < 0.f)
    {
        _Velocity._y = 0.f;
    }
}

bool MovementComponent::IsLaning() const
{
    return _IsLanding;
}

bool MovementComponent::StartJump(float Force)
{
    if (!_IsLanding)
    {
        return false;
    }

    _Velocity._y = Force;
    
    _IsLanding = false;

    return true;
}

void MovementComponent::Blocking(const FVector3D &Correction)
{
    if(!_UpdateComponent)
    {
        return;
    }

    _NextPosition = _UpdateComponent->GetWorldPosition() + Correction;

    _UpdateComponent->SetWorldPosition(_NextPosition);

    // 벽 방향으로 진행 중인 수평 이동만 중단한다.
    if ((Correction._x > 0.f && _MoveAxis._x < 0.f) || (Correction._x < 0.f && _MoveAxis._x > 0.f))
    {
        _MoveAxis._x = 0.f;
    }
}
