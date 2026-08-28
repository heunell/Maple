#pragma once
#include "Object/Actor.h"

class CameraZone : public Actor
{
public:
    CameraZone() = default;
    virtual ~CameraZone() = default;

public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

    void SetTriggerSize(float X, float Y);

private:
    void OnTriggerZone(Weak<class CollisionComponent> Component);

    Ptr<class AABBCollisionComponent> _LockTrigger;
};

