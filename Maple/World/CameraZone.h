#pragma once
#include "Object/Actor.h"

enum class eCameraZoneType
{
    None,
    Area,
    Wall,
    End
};

class CameraZone : public Actor
{
public:
    CameraZone() = default;
    virtual ~CameraZone() = default;

public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

    void SetArea(float X, float Y);

    void SetWall(float X, float Y, const FAABB2D& LockBound);
    
private:
    void OnTriggerZone(Weak<class CollisionComponent> Component);

    Ptr<class AABBCollisionComponent> _LockTrigger;

    eCameraZoneType _LockType = eCameraZoneType::None;

    FAABB2D _WallLockBound = {};
};

