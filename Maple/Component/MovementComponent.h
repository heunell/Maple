#pragma once
#include "ActorComponent.h"

class MovementComponent : public ActorComponent
{
public:
    MovementComponent() = default;
    
    virtual ~MovementComponent() = default;
    
protected:
    float _Speed = 0.f;
    
    float _Gravity = 0.f;

    bool _IsLanding = false;

    FVector3D _MoveAxis  = FVector3D::Zero;
    
    FVector3D _NextPosition = FVector3D::Zero;
    
    FVector3D _Velocity = FVector3D::Zero;

    Ptr<class SceneComponent> _UpdateComponent = nullptr;
    
public:
    virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;
    
    virtual void Tick(float DeltaTime) override;
    
    virtual void Destroy() override;
    
public:
    void SetSpeed(float Speed)           { _Speed = Speed; }
    
    const float GetSpeed() const         { return _Speed;  }
    
    const FVector3D& GetMoveAxis() const { return _MoveAxis; }
    
    void SetUpdateComponent(Ptr<class SceneComponent> UpdateComp);
    
    Ptr<class SceneComponent> GetUpdateComponent() const;
    
    void SetMoveAxis(const FVector3D& MoveAxis);
    
    void AddMoveAxis(const FVector3D& MoveAxis);
    
    void SetGravity(float Gravity)        { _Gravity = Gravity; }

    const FVector3D& GetVelocity() const  { return _Velocity;   }

    void SetLanding(bool IsLanding);

    bool IsLaning() const;

    bool StartJump(float Force);

    bool StartAirJump(float HorizontalForce, float VerticalForce);

    bool StartDownJump();

    void Stop();

    void ResetMovement();

    void Blocking(const FVector3D& Correction); // 벽에 막혔을때 호출해주는 함수 
};
