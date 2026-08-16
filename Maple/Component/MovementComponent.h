#pragma once
#include "ActorComponent.h"

class MovementComponent : public ActorComponent
{
public:
    MovementComponent() = default;
    
    virtual ~MovementComponent() = default;
    
protected:
    float _Speed = 0.f;
    
    FVector3D _MoveAxis  = FVector3D::Zero;
    
    FVector3D _NextPosition = FVector3D::Zero;
    
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
    
    void Stop();
};
