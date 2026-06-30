#pragma once
#include "ActorComponent.h"

class MovementComponent : public ActorComponent
{
public:
    MovementComponent() = default;
    virtual ~MovementComponent() = default;
    
protected:
    float _speed = 0.f;
    FVector3D _moveAxis  = FVector3D::Zero;
    FVector3D _nextPosition = FVector3D::Zero;
    Ptr<class SceneComponent> _updateComponent = nullptr;
    
public:
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime) override;
    virtual void Destroy() override;
    
public:
    void SetSpeed(float speed)           { _speed = speed; }
    const float GetSpeed() const         { return _speed;  }
    const FVector3D& GetMoveAxis() const { return _moveAxis; }
    void SetUpdateComponent(Ptr<class SceneComponent> updateComp);
    Ptr<class SceneComponent> GetUpdateComponent() const;
    void SetMoveAxis(const FVector3D& moveAxis);
    void AddMoveAxis(const FVector3D& moveAxis);
    void Stop();
};
